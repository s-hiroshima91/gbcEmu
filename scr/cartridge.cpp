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
	
}

void Cartridge::ReadHeader(){
	std::string path = "./rom/";
	
	path += romName;
	path += ".gb";
	
	std::ifstream ifs(path, std::ios::in | std::ios::binary);
	if (!ifs){
		std::cout << "ファイルが開けませんでした。" << std::endl;
		return;
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
	
	std::ifstream ifs(path, std::ios::in | std::ios::binary);
	if (!ifs){
		std::cout << "ファイルが開けませんでした。" << std::endl;
		return;
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

void Cartridge::WriteSaveDate(char *sRam){
	std::string path = "./save/";
	
	path += romName;
	path += ".sav";
	
	std::ofstream ofs(path, std::ios::out | std::ios::binary);
	if (!ofs){
		std::cout << "セーブデータを新たに作成します。" << std::endl;
	}
	ofs.write(sRam, ramBankSize * (ramSize + 1));
	ofs.close();

}

void Cartridge::Mapper(ushort addr, char value){

	if (addr < 0x2000){
		
	}else if (addr < 0x4000){
		Cpu->rom2 = rom + (value & romSize) * romBankSize;
	}
	return;
}

Cartridge::~Cartridge(){
	
	delete[] boot;
	delete[] rom;
	delete[] ram; 

}
