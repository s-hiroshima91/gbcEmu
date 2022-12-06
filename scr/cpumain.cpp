#include "cpu.hpp"
#include "cpuinline.hpp"
#include "cartridge.hpp"
#include <iostream>
#include <fstream>

void Boot(char *rom, ushort *registerPC, char *bank){
	std::ifstream ifs("./dmg_boot.bin", std::ios::in | std::ios::binary);
	if (!ifs){
		std::cout << "ファイルが開けませんでした。" << std::endl;
		*registerPC = 0x0100;
		*bank = 0x01;
		return;
	}

	ifs.read(rom, 0x100 * sizeof(char));
	ifs.close();

}

void Cpu::BootFin(){
	rom1 = Crg->rom;
	
}

Cpu::Cpu(Cartridge *cartridge){
	
	Crg = cartridge;
	Crg->Cpu = this;
	rom1 = Crg->boot;
	rom2 = &Crg->rom[romBankSize * 1];
	sRam = Crg->ram;
	wRam1 = new char [0x1000];
	wRam2 = new char [0x1000];
	vRam = new char [0x2000];
	ioReg[0x50] = 0x00;
	ioReg[0x40] = 0x91;

	registerSP = 0xfffe;
	
	registerPC = 0x0000;
	Boot(rom1, &registerPC, &ioReg[0x50]);

}

char *Cpu::MemoryMap(ushort addr){
	char *ptr;
	
	if (addr < 0x4000){
		ptr = &rom1[addr];
		
	}else if (addr < 0x8000){
		addr -= 0x4000;
		ptr = &rom2[addr];
		
	}else if (addr < 0xa000){
		addr -= 0x8000;
		ptr = &vRam[addr];
		
	}else if (addr < 0xc000){
		addr -= 0xa000;
		ptr = &sRam[addr];
		
	}else if (addr < 0xd000){
		addr -= 0xc000;
		ptr = &wRam1[addr];
		
	}else if (addr < 0xe000){
		addr -= 0xd000;
		ptr = &wRam2[addr];
		
	}else if (addr >= 0xfe00){
		addr -= 0xe000;
		ptr = &etcRam[addr];

	}else{
		addr -= 0x2000;
		ptr = MemoryMap(addr);
	}
	return ptr;
}

int Cpu::Interrupt(){
	if (ime){
		char flg;
		flg = IE & IF & 0x1f;
		if (flg == 0){
			return 0;
		}
		for (int i = 0; i < 5; ++i){
			if ((flg & 0x01) == 0x01){
				if (halt){
					++registerPC;
					halt = false;
				}
				Push16(registerPC);
				registerPC = 0x0040 + (i << 3);
				IF &= (0x01 << i) ^ 0xff;
				ime = false;
				return 20;
			}else{
				flg >>= 1;
			}
		}
	}
	return 0;
}

void Cpu::Timer(){
	ushort temp;
//	temp = clockCounter;
	
	clockCounter += 4;
//	temp &= (clockCounter ^ 0xffff);
	temp = clockCounter;
	temp >>= timerStep[ioReg[0x07] & 0x03];
	temp &= ioReg[0x07] >> 2;
	ioReg[0x04] = static_cast<char>(clockCounter >> 8);
	
	if (preTimer == 1){
		IF |= 0x04;
		ioReg[0x05] = ioReg[0x06];
//		nextTimer = ioReg[0x05] + 1;
		preTimer = 2;
	}else if(preTimer == 2){
		preTimer = 0;
	}
	
	
	
	preMux &= (temp ^ 0xffff);
	
//	if ((temp & 0x01) == 0x01){
	if (preMux){
		if (ioReg[0x05] == 0xff){
			preTimer = 1;
		}
//		ioReg[0x05] = nextTimer;
//		++nextTimer;
			++ioReg[0x05];
	}
	preMux = temp;
	
	
}

int Cpu::Sequence(){
	int counter;
	int i = 0;
	ushort opeCode;
	counter = Interrupt();
	
	while (i < counter){
		Timer();
		i += 4;
	}
	
	opeCode = C2US(Imm());

	counter += (this->*cpuInstrs[opeCode])();
	counter += cpuCycle[opeCode];
	
	while (i < counter){
		Timer();
		i += 4;
	}
	
	return counter;
}

void Cpu::IOReg(ushort addr, char value){
	addr &= 0x00ff;
	
	if (addr == 0x00){
		if ((value & 0xf0) == 0x20){
			ioReg[0x00] = key[0];
			ioReg[0x00] |= 0xe0;
		}else if ((value & 0xf0) == 0x10){
			ioReg[0x00] = key[1];
			ioReg[0x00] |= 0xd0;
		}else if ((value & 0xf0) == 0x30){
			ioReg[0x00] = 0xff;
		}
		
	}else if (addr == 0x04){
		clockCounter = 0xfffc;
		
	}else if (addr == 0x05){
		if (preTimer == 0){
			ioReg[0x05] = value;
			nextTimer = value + 1;
		}else if (preTimer == 1){	
			preTimer = 0;
			ioReg[0x05] = value;
			nextTimer = value + 1;
		}
		
	}else if (addr == 0x06){
		nextTimer = value;
		ioReg[0x06] = value;
		
	}else if (addr == 0x41){
		ioReg[0x41] &= 0x07;
		ioReg[0x41] |= (value | 0x80) & 0xf8;
		
	}else if (addr == 0x46){
		char *dma;
		addr = static_cast<ushort>(value) << 8;
		dma = MemoryMap(addr);
		for (int i = 0; i < 0xa0; ++i){
			oam[i] = dma[i];
		}
		
	}else if (addr == 0x50){
		ioReg[0x50] = value;
		BootFin();
		
	}else{
		ioReg[addr] = value;
	}

}



Cpu::~Cpu(){
	
	delete[] wRam1;
	delete[] wRam2;
	delete[] vRam;
}