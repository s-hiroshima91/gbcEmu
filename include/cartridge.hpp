#ifndef CARTRIDGE_HPP
#define CARTRIDGE_HPP

#include "common.hpp"
#include <string>

class Cpu;

class Cartridge{
	private:
		std::string romName;
		int romSize, ramSize;
		int romBankNum = 1;
		int ramBankNum = 0;
		bool mode0 = false;
		void (Cartridge::*mbc)(ushort, char);
		void ReadHeader();
		void ReadRom();
		void ReadSaveDate();

	public:
		Cpu *Cpu;
		char *boot, *rom, *ram;
		char cgbFlg, mapper;
		Cartridge(std::string Name);
		void MBC(ushort addr, char value);
		void MBC1(ushort addr, char value);
		void MBC5(ushort addr, char value);
		void WriteSaveDate();
		~Cartridge();
};


#endif