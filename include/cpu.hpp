#ifndef CPU_HPP
#define CPU_HPP
#include "common.hpp"

class Cartridge;

class Cpu{
	private:
		Cartridge *Crg;
		char registerA, registerB, registerC, registerD, registerE, registerF;
		ushort registerHL, registerSP, registerPC;
		
		int timerStep[4] = {9, 3, 5, 7};
		char preTimer = 0;
		char nextTimer = 0;
		ushort preMux = 0;
		
		bool halt;
		
		ushort clockCounter = 0;
		
		char *wRam1, *wRam2;
		

	
		char *MemoryMap(ushort addr);
		int Interrupt();
		void Timer();
		void BootFin();
		
		
		char Imm();
		ushort Abs();
		
		void Adc(char value);
		void Add(char value);
		void Sbc(char value);
		void Sub(char value);
		
		void Add16(ushort *registerXY, ushort registerZW);
		void Inc(char *value);
		void Dec(char *value);
		
		void And(char value);
		void Or(char value);
		void Xor(char value);
		void Cp(char value);
		
		void RlcX(char *value);
		void RlX(char *value);
		void RrcX(char *value);
		void RrX(char *value);
		void Rlc(char *value, char carry);
		void Rrc(char *value, char carry);
		
		
		void LdR88Imm(char *registerX, char *registerY);
		void LdR16Imm(ushort *registerXY);
		void StX(ushort addr, char value);
		void StX16(ushort addr, ushort value);
		void IOReg(ushort addr, char value);
		
		void Pop88(char *registerX, char *registerY);
		void Push88(char registerX, char registerY);
		void Push16(ushort registerXY);
		
		int JrBranch(int flg, bool status);
		int JpBranch(int flg, bool status);
		int CallBranch(int flg, bool status);
		int RetBranch(int flg, bool status);
		

	public:
		bool ime;
		char key[2] = {0x3f, 0x3f};
		char *rom1, *rom2, *vRam, *sRam, etcRam[0x2000];
		char &IF = etcRam[0x1f0f];
		char &IE = etcRam[0x1fff];
		char *ioReg = &etcRam[0x1f00];
		char *oam  = &etcRam[0x1e00];
		
		Cpu(Cartridge *cartridge);
		int Sequence();
		~Cpu();
	
	
	private:
	
		int OpError();
		/*0x00*/
		int Nop();
		int LdBCImm();
		int LdbcA();
		int IncBC();
		int IncB();
		int DecB();
		int LdBImm();
		int RlcA();
		int LdAbsSP();
		int AddHLBC();
		int LdAbc();
		int DecBC();
		int IncC();
		int DecC();
		int LdCImm();
		int RrcA();
		
		/*0x10*/
		int StopCpu();
		int LdDEImm();
		int LddeA();
		int IncDE();
		int IncD();
		int DecD();
		int LdDImm();
		int RlA();
		int Jr();
		int AddHLDE();
		int LdAde();
		int DecDE();
		int IncE();
		int DecE();
		int LdEImm();
		int RrA();
		
		/*0x20*/
		int JrNZ();
		int LdHLImm();
		int LdhliA();
		int IncHL();
		int IncH();
		int DecH();
		int LdHImm();
		int Daa();
		int JrZ();
		int AddHLHL();
		int LdAhli();
		int DecHL();
		int IncL();
		int DecL();
		int LdLImm();
		int Cpl();
		
		/*0x30*/
		int JrNC();
		int LdSPImm();
		int LdhldA();
		int IncSP();
		int Inchl();
		int Dechl();
		int LdhlImm();
		int Scf();
		int JrC();
		int AddHLSP();
		int LdAhld();
		int DecSP();
		int IncA();
		int DecA();
		int LdAImm();
		int Ccf();
		
		/*0x40*/
		//int LdBB(); equiv int Nop();
		int LdBC();
		int LdBD();
		int LdBE();
		int LdBH();
		int LdBL();
		int LdBhl();
		int LdBA();
		int LdCB();
		//int LdCC(); equiv int Nop();
		int LdCD();
		int LdCE();
		int LdCH();
		int LdCL();
		int LdChl();
		int LdCA();
		
		/*0x50*/
		int LdDB();
		int LdDC();
		//int LdDD(); equiv int Nop();
		int LdDE();
		int LdDH();
		int LdDL();
		int LdDhl();
		int LdDA();
		int LdEB();
		int LdEC();
		int LdED();
		//int LdEE(); equiv int Nop();
		int LdEH();
		int LdEL();
		int LdEhl();
		int LdEA();
		
		/*0x60*/
		int LdHB();
		int LdHC();
		int LdHD();
		int LdHE();
		//int LdHH(); equiv int Nop();
		int LdHL();
		int LdHhl();
		int LdHA();
		int LdLB();
		int LdLC();
		int LdLD();
		int LdLE();
		int LdLH();
		//int LdLL(); equiv int Nop();
		int LdLhl();
		int LdLA();
		
		/*0x70*/
		int LdhlB();
		int LdhlC();
		int LdhlD();
		int LdhlE();
		int LdhlH();
		int LdhlL();
		int Halt();
		int LdhlA();
		int LdAB();
		int LdAC();
		int LdAD();
		int LdAE();
		int LdAH();
		int LdAL();
		int LdAhl();
		// int LdLA(); equiv int Nop();
		
		/*0x80*/
		int AddB();
		int AddC();
		int AddD();
		int AddE();
		int AddH();
		int AddL();
		int Addhl();
		int AddA();
		int AdcB();
		int AdcC();
		int AdcD();
		int AdcE();
		int AdcH();
		int AdcL();
		int Adchl();
		int AdcA();
		
		/*0x90*/
		int SubB();
		int SubC();
		int SubD();
		int SubE();
		int SubH();
		int SubL();
		int Subhl();
		int SubA();
		int SbcB();
		int SbcC();
		int SbcD();
		int SbcE();
		int SbcH();
		int SbcL();
		int Sbchl();
		int SbcA();
		
		/*0xa0*/
		int AndB();
		int AndC();
		int AndD();
		int AndE();
		int AndH();
		int AndL();
		int Andhl();
		int AndA();
		int XorB();
		int XorC();
		int XorD();
		int XorE();
		int XorH();
		int XorL();
		int Xorhl();
		int XorA();
		
		/*0xb0*/
		int OrB();
		int OrC();
		int OrD();
		int OrE();
		int OrH();
		int OrL();
		int Orhl();
		int OrA();
		int CpB();
		int CpC();
		int CpD();
		int CpE();
		int CpH();
		int CpL();
		int Cphl();
		int CpA();
		
		/*0xc0*/
		int RetNZ();
		int PopBC();
		int JpNZ();
		int Jp();
		int CallNZ();
		int PushBC();
		int AddImm();
		int Rst00();
		int RetZ();
		int Ret();
		int JpZ();
		int Prefix();
		int CallZ();
		int Call();
		int AdcImm();
		int Rst08();
		
		/*0xd0*/
		int RetNC();
		int PopDE();
		int JpNC();
		// No OpCode
		int CallNC();
		int PushDE();
		int SubImm();
		int Rst10();
		int RetC();
		int RetI();
		int JpC();
		//No OpCode
		int CallC();
		//No OpCode
		int SbcImm();
		int Rst18();
		
		/*0xe0*/
		int LdffImmA();
		int PopHL();
		int LdffcA();
		// No OpCode
		// No OpCode
		int PushHL();
		int AndImm();
		int Rst20();
		int AddSPImm();
		int JpHL();
		int LdAbsA();
		// No OpCode
		// No OpCode
		// No OpCode
		int XorImm();
		int Rst28();
		
		/*0xf0*/
		int LdAffImm();
		int PopAF();
		int LdAffc();
		int Di();
		// No OpCode
		int PushAF();
		int OrImm();
		int Rst30();
		int LdHLSPImm();
		int LdSPHL();
		int LdAAbs();
		int Ei();
		// No OpCode
		// No OpCode
		int CpImm();
		int Rst38();
		
		//prefix
		//0x00~0x3f
		void Shift(ushort num, char *value);
		void SraX(char *value);
		void SlaX(char *value);
		void SrlX(char *value);
		void Swap(char *value);
		//0x40~0x7f
		void BitCheck(ushort num, char *value);
		//0x80~0xbf
		void BitRes(ushort num, char *value);
		//0xc0~0xff
		void BitSet(ushort num, char *value);
		
		
		int (Cpu::*cpuInstrs[0x100])() ={
				/*0x00*/ &Cpu::Nop, &Cpu::LdBCImm, &Cpu::LdbcA, &Cpu::IncBC, &Cpu::IncB, &Cpu::DecB, &Cpu::LdBImm, &Cpu::RlcA, &Cpu::LdAbsSP, &Cpu::AddHLBC, &Cpu::LdAbc, &Cpu::DecBC, &Cpu::IncC, &Cpu::DecC, &Cpu::LdCImm, &Cpu::RrcA,
				/*0x10*/ &Cpu::StopCpu, &Cpu::LdDEImm, &Cpu::LddeA, &Cpu::IncDE, &Cpu::IncD, &Cpu::DecD, &Cpu::LdDImm, &Cpu::RlA, &Cpu::Jr, &Cpu::AddHLDE, &Cpu::LdAde, &Cpu::DecDE, &Cpu::IncE, &Cpu::DecE, &Cpu::LdEImm, &Cpu::RrA,
				/*0x20*/ &Cpu::JrNZ, &Cpu::LdHLImm, &Cpu::LdhliA, &Cpu::IncHL, &Cpu::IncH, &Cpu::DecH, &Cpu::LdHImm, &Cpu::Daa, &Cpu::JrZ, &Cpu::AddHLHL, &Cpu::LdAhli, &Cpu::DecHL, &Cpu::IncL, &Cpu::DecL, &Cpu::LdLImm, &Cpu::Cpl,
				/*0x30*/ &Cpu::JrNC, &Cpu::LdSPImm, &Cpu::LdhldA, &Cpu::IncSP, &Cpu::Inchl, &Cpu::Dechl, &Cpu::LdhlImm, &Cpu::Scf, &Cpu::JrC, &Cpu::AddHLSP, &Cpu::LdAhld, &Cpu::DecSP, &Cpu::IncA, &Cpu::DecA, &Cpu::LdAImm, &Cpu::Ccf,
				/*0x40*/ &Cpu::Nop, &Cpu::LdBC, &Cpu::LdBD, &Cpu::LdBE, &Cpu::LdBH, &Cpu::LdBL, &Cpu::LdBhl, &Cpu::LdBA, &Cpu::LdCB, &Cpu::Nop, &Cpu::LdCD, &Cpu::LdCE, &Cpu::LdCH, &Cpu::LdCL, &Cpu::LdChl, &Cpu::LdCA,
				/*0x50*/ &Cpu::LdDB, &Cpu::LdDC, &Cpu::Nop, &Cpu::LdDE, &Cpu::LdDH, &Cpu::LdDL, &Cpu::LdDhl, &Cpu::LdDA, &Cpu::LdEB, &Cpu::LdEC, &Cpu::LdED, &Cpu::Nop, &Cpu::LdEH, &Cpu::LdEL, &Cpu::LdEhl, &Cpu::LdEA,
				/*0x60*/ &Cpu::LdHB, &Cpu::LdHC, &Cpu::LdHD, &Cpu::LdHE, &Cpu::Nop, &Cpu::LdHL, &Cpu::LdHhl, &Cpu::LdHA, &Cpu::LdLB, &Cpu::LdLC, &Cpu::LdLD, &Cpu::LdLE, &Cpu::LdLH, &Cpu::Nop, &Cpu::LdLhl, &Cpu::LdLA,
				/*0x70*/ &Cpu::LdhlB, &Cpu::LdhlC, &Cpu::LdhlD, &Cpu::LdhlE, &Cpu::LdhlH, &Cpu::LdhlL, &Cpu::Halt, &Cpu::LdhlA, &Cpu::LdAB, &Cpu::LdAC, &Cpu::LdAD, &Cpu::LdAE, &Cpu::LdAH, &Cpu::LdAL, &Cpu::LdAhl, &Cpu::Nop,
				/*0x80*/ &Cpu::AddB, &Cpu::AddC, &Cpu::AddD, &Cpu::AddE, &Cpu::AddH, &Cpu::AddL, &Cpu::Addhl, &Cpu::AddA, &Cpu::AdcB, &Cpu::AdcC, &Cpu::AdcD, &Cpu::AdcE, &Cpu::AdcH, &Cpu::AdcL, &Cpu::Adchl, &Cpu::AdcA,
				/*0x90*/ &Cpu::SubB, &Cpu::SubC, &Cpu::SubD, &Cpu::SubE, &Cpu::SubH, &Cpu::SubL, &Cpu::Subhl, &Cpu::SubA, &Cpu::SbcB, &Cpu::SbcC, &Cpu::SbcD, &Cpu::SbcE, &Cpu::SbcH, &Cpu::SbcL, &Cpu::Sbchl, &Cpu::SbcA,
				/*0xa0*/ &Cpu::AndB, &Cpu::AndC, &Cpu::AndD, &Cpu::AndE, &Cpu::AndH, &Cpu::AndL, &Cpu::Andhl, &Cpu::AndA, &Cpu::XorB, &Cpu::XorC, &Cpu::XorD, &Cpu::XorE, &Cpu::XorH, &Cpu::XorL, &Cpu::Xorhl, &Cpu::XorA,
				/*0xb0*/ &Cpu::OrB, &Cpu::OrC, &Cpu::OrD, &Cpu::OrE, &Cpu::OrH, &Cpu::OrL, &Cpu::Orhl, &Cpu::OrA, &Cpu::CpB, &Cpu::CpC, &Cpu::CpD, &Cpu::CpE, &Cpu::CpH, &Cpu::CpL, &Cpu::Cphl, &Cpu::CpA,
				/*0xc0*/ &Cpu::RetNZ, &Cpu::PopBC, &Cpu::JpNZ, &Cpu::Jp, &Cpu::CallNZ, &Cpu::PushBC, &Cpu::AddImm, &Cpu::Rst00, &Cpu::RetZ, &Cpu::Ret, &Cpu::JpZ, &Cpu::Prefix, &Cpu::CallZ, &Cpu::Call, &Cpu::AdcImm, &Cpu::Rst08,
				/*0xd0*/ &Cpu::RetNC, &Cpu::PopDE, &Cpu::JpNC, &Cpu::OpError, &Cpu::CallNC, &Cpu::PushDE, &Cpu::SubImm, &Cpu::Rst10, &Cpu::RetC, &Cpu::RetI, &Cpu::JpC, &Cpu::OpError, &Cpu::CallC, &Cpu::OpError, &Cpu::SbcImm, &Cpu::Rst18,
				/*0xe0*/ &Cpu::LdffImmA, &Cpu::PopHL, &Cpu::LdffcA, &Cpu::OpError, &Cpu::OpError, &Cpu::PushHL, &Cpu::AndImm, &Cpu::Rst20, &Cpu::AddSPImm, &Cpu::JpHL, &Cpu::LdAbsA, &Cpu::OpError, &Cpu::OpError, &Cpu::OpError, &Cpu::XorImm, &Cpu::Rst28,
				/*0xf0*/ &Cpu::LdAffImm, &Cpu::PopAF, &Cpu::LdAffc, &Cpu::Di, &Cpu::OpError, &Cpu::PushAF, &Cpu::OrImm, &Cpu::Rst30, &Cpu::LdHLSPImm, &Cpu::LdSPHL, &Cpu::LdAAbs, &Cpu::Ei, &Cpu::OpError, &Cpu::OpError, &Cpu::CpImm, &Cpu::Rst38
		};
		
		char *operand[4] = {&registerB, &registerC, &registerD, &registerE};
		
		void (Cpu::*shiftFunc[8])(char*) = {&Cpu::RlcX, &Cpu::RrcX, &Cpu::RlX, &Cpu::RrX, &Cpu::SlaX, &Cpu::SraX, &Cpu::Swap, &Cpu::SrlX};
		
		
		void (Cpu::*cbCpuInstrs[4])(ushort, char*) = {&Cpu::Shift, &Cpu::BitCheck, &Cpu::BitRes, &Cpu::BitSet};
		
		int cpuCycle[0x100] = {
				/*0x00*/ 4, 12, 8, 8, 4, 4, 8, 4, 20, 8, 8, 8, 4, 4, 8, 4,
				/*0x10*/ 4, 12, 8, 8, 4, 4, 8, 4, 12, 8, 8, 8, 4, 4, 8, 4,
				/*0x20*/ 8, 12, 8, 8, 4, 4, 8, 4, 8, 8, 8, 8, 4, 4, 8, 4,
				/*0x30*/ 8, 12, 8, 8, 12, 12, 12, 4, 8, 8, 8, 8, 4, 4, 8, 4,
				/*0x40*/ 4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,
				/*0x50*/ 4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,
				/*0x60*/ 4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,
				/*0x70*/ 8, 8, 8, 8, 8, 8, 4, 8, 4, 4, 4, 4, 4, 4, 8, 4,
				/*0x80*/ 4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,
				/*0x90*/ 4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,
				/*0xa0*/ 4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,
				/*0xb0*/ 4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,
				/*0xc0*/ 8, 12, 12, 16, 12, 16, 8, 16, 8, 16, 12, 0, 12, 24, 8, 16,
				/*0xd0*/  8, 12, 12, 0, 12, 16, 8, 16, 8, 16, 12, 0, 12, 0, 8, 16,
				/*0xe0*/ 12, 12, 8, 0, 0, 16, 8,16,16, 4, 16, 0, 0, 0, 8, 16,
				/*0xf0*/  12, 12, 8, 4, 0, 16, 8,16,12, 8, 16, 4, 0, 0, 8, 16,
		};
		
		int cbCpuCycle[0x20] = {
				/*0x00*/8, 8, 8, 8, 8, 8, 16, 8,
				/*0x40*/8, 8, 8, 8, 8, 8, 12, 8,
				/*0x80*/8, 8, 8, 8, 8, 8, 16, 8,
				/*0xc0*/8, 8, 8, 8, 8, 8, 16, 8
		};
		
		char daaP[16] = {
						0x00, 0x60, 0x06, 0x66,
						0x06, 0x66, 0x06, 0x66, 
						0x60, 0x60, 0x66, 0x66,
						0x66, 0x66, 0x66, 0x66};
		char daaN[4] = {0x00, 0xa0, 0xfa, 0x9a};
		
};

#endif
