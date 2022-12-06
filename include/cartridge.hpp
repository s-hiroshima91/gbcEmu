#ifndef CARTRIDGE_HPP
#define CARTRIDGE_HPP

#include "common.hpp"
#include <string>

class Cpu;

class Cartridge{
	private:
		std::string romName;
		int romSize, ramSize;
		void ReadHeader();
		void ReadRom();
		void ReadSaveDate();

	public:
		Cpu *Cpu;
		char *boot, *rom, *ram;
		char cgbFlg, mapper;
		Cartridge(std::string Name);
		void Mapper(ushort addr, char value);
		void WriteSaveDate(char *sRam);
		~Cartridge();
};


#endif