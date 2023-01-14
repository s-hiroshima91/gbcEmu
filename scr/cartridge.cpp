#include "cartridge.hpp"
#include "cpu.hpp"
#include <fstream>
#include <iostream>


Cartridge::Cartridge(std::string Name)
{
	romName = Name;
	ReadHeader();
	ReadRom();
	ReadSaveDate();
	
	if ((mapper >= 0x1) && (mapper <= 0x3)){
		mbc = &Cartridge::MBC1;
		
	}else if ((mapper >= 0x5) && (mapper <= 0x6)){
		
	}else if ((mapper >= 0x19) && (mapper <= 0x1e)){
		mbc = &Cartridge::MBC5;
		
	}else{
		mbc = nullptr;
	}
}

void Cartridge::ReadHeader(){
	std::string path = "./rom/";
	
	path += romName;
	path += ".gb";
	
	std::ifstream ifs(path, std::ios::in | std::ios::binary);
	if (!ifs){		
		ifs.close();
		ifs.clear();
		path += "c";
		ifs.open(path, std::ios::in | std::ios::binary);
		
		if (!ifs){
			std::cout << "ファイルが開けませんでした。" << std::endl;
			return;
		}
	}
	
	boot = new char[romBankSize];

	ifs.read(boot, romBankSize * sizeof(char));
	
	cgbFlg = boot[0x0143];
	mapper = boot[0x0147];
	romSize = (1 << (boot[0x0148] + 1)) - 1;

	if (boot[0x0149] == 0){
		ramSize = -1;
	}else if (boot[0x0149] ==  2){
		ramSize = 0;
	}else if (boot[0x0149] == 3){
		ramSize = 3;
	}else if (boot[0x0149] == 4){
		ramSize = 15;
	}else if (boot[0x0149] == 5){
		ramSize = 7;
	}else{
		ramSize = -1;
	}
	
	ifs.close();
	
	std::cout << std::hex << "cgb " << +cgbFlg << " map " << +mapper << std::dec << " rom "
<< romSize << " ram " << ramSize << std::endl;
}

void Cartridge::ReadRom(){
	std::string path = "./rom/";
	
	path += romName;
	path += ".gb";
	
	std::ifstream ifs;
	ifs.open(path, std::ios::in | std::ios::binary);
	if (!ifs){
		ifs.close();
		ifs.clear();
		path += "c";
		ifs.open(path, std::ios::in | std::ios::binary);
		if (!ifs){
			std::cout << "ファイルが開けませんでした。" << std::endl;
			return;
		}
	}
	
	rom = new char[romBankSize * (romSize + 1)];
	ifs.read(rom, romBankSize * (romSize + 1));
	ifs.close();
}

void Cartridge::ReadSaveDate(){
	std::string path = "./save/";
	
	path += romName;
	path += ".sav";
	

	ram = new char[ramBankSize * (ramSize + 1)];
	
	std::ifstream ifs(path, std::ios::in | std::ios::binary);
	if (!ifs){
		std::cout << "セーブデータがありません。" << std::endl;
		return;
	}
	
	ifs.read(ram, ramBankSize * (ramSize + 1));
	ifs.close();
}

void Cartridge::WriteSaveDate(){
	std::string path = "./save/";
	
	path += romName;
	path += ".sav";
	
	std::ofstream ofs(path, std::ios::out | std::ios::binary);
	if (!ofs){
		std::cout << "セーブデータを新たに作成します。" << std::endl;
	}
	ofs.write(ram, ramBankSize * (ramSize + 1));
	ofs.close();

}

void Cartridge::MBC(ushort addr, char value){
	if(mbc == nullptr){
		return;
	}
	(this->*mbc)(addr, value);
}

void Cartridge::MBC1(ushort addr, char value){

	if (addr < 0x2000){
		
	}else if (addr < 0x4000){
		value = value & 0x1f & romSize;
		if (value == 0){
			value = 1;
		}
		romBankNum &= 0x20;
		romBankNum |= static_cast<int>(value);
		Cpu->rom2 = rom + romBankNum * romBankSize;
		
	}else if (addr < 0x6000){
		if (romSize < 0x20){
			ramBankNum = static_cast<int>(value) & ramSize;
			if (!mode0){
				Cpu->sRam = ram + ramBankNum * ramBankSize;
			}
		}else{
			romBankNum &= 0x1f;
			romBankNum |= static_cast<int>(value & 0x11) << 5;
			if (!mode0){
				Cpu->rom1 = rom + (romBankNum & 0x20) * romBankSize;
			}else{
				Cpu->rom1 = rom + (romBankNum & 0x20) * romBankSize;
			}
		}

	}else if (addr < 0x8000){
		if ((value & 0x01) == 0){
			mode0 = true;
			
		}else if ((value & 0x01) == 1){
			mode0 = false;
			if (romSize < 0x20){
				Cpu->sRam = ram + romBankNum * ramBankSize;
			}
		}
	}
	return;
}

void Cartridge::MBC5(ushort addr, char value){
	if (addr < 0x2000){
		
	}else if (addr < 0x3000){
		romBankNum &= 0xff00;
		romBankNum |= static_cast<int>(value) & 0xff;
		romBankNum &= romSize;
		Cpu->rom2 = rom + romBankNum * romBankSize;
		
	}else if (addr < 0x4000){
		romBankNum &= 0x00ff;
		romBankNum |= static_cast<int>(value) << 8;
		romBankNum &= romSize;
		Cpu->rom2 = rom + romBankNum * romBankSize;
		
	}else if (addr < 0x6000){
		Cpu->sRam = ram + (value & ramSize) * ramBankSize;
	}
	
	return;
}

Cartridge::~Cartridge(){
	
	delete[] boot;
	delete[] rom;
	delete[] ram; 

}
