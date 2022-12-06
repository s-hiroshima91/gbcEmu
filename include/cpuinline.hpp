#ifndef CPUINLINE_HPP
#define CPUINLINE_HPP

#define cFlg 4
#define hFlg 5
#define nFlg 6
#define zFlg 7

inline ushort Byte1to2(char upper, char lower){
	return C2US(lower) + (static_cast<ushort>(upper) << 8);
}

inline char HalfCarry(char x, char y){
	return (((x & 0x0f) + (y & 0x0f)) << 1) & 0x20; //0b0010 0000
}

inline char HalfCarry2(char x, char  y){
	return  (((x & 0x0f) + (y & 0x0f) + 0x01) << 1) & 0x20; //0b0010 0000
}
//16bitアドレスに変換、ただし符号を考慮
inline ushort C2S(char valueC){
	bool flg;
	ushort valueS;
	//最上位bitが1なら追加する桁を1で埋める
	flg = CheckBit(valueC, 7);
	if (flg == true){
		valueS = valueC | 0xff00;
	}else{
		valueS = valueC & 0x00ff;
	}
	return valueS;
}

inline void Inc16(char *upper, char *lower){
	ushort value;
	value = Byte1to2(*upper, *lower);
	++value;
	*upper = static_cast<char>((value >> 8) & 0x00ff);
	*lower = static_cast<char>(value & 0x00ff);
}

inline void Dec16(char *upper, char *lower){
	ushort value;
	value = Byte1to2(*upper, *lower);
	--value;
	*upper = static_cast<char>((value >> 8) & 0x00ff);
	*lower = static_cast<char>(value & 0x00ff);
}

inline void LdUpperR8(ushort *register16, char value){
	*register16 &= 0x00ff;
	*register16 |= static_cast<ushort>(value) << 8;
}

inline void LdLowerR8(ushort *register16, char value){
	*register16 &= 0xff00;
	*register16 |= C2US(value);
}



#endif