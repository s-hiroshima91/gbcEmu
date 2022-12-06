#ifndef COMMON_HPP
#define COMMON_HPP

#define romBankSize 0x4000
#define ramBankSize 0x2000

#define winX 160
#define winY 144
#define dispX 1080
#define dispY 1920
#define posX 140
#define posY 200
#define buttonSize 45

#define magni 5

typedef unsigned short ushort;
typedef unsigned int uint;

//16bitに変換
inline ushort C2US(char value){
	return value & 0x00ff; //マイナスの扱いを明確にするため上8桁をマスク
}


//N番目のBit確認の関数
inline bool CheckBit(char operand, int bitN){
	operand >>= bitN;
	return static_cast<bool>(operand & 0b00000001); 
}

//N番目のBit確認の関数
inline char CheckBitFF(char operand, int bitN){
	char value[2] = {0x00, 0xff};
	operand >>= bitN;
	return value[operand & 0x01];
}

inline int C2I(char value){
	return static_cast<int>(value & 0x000000ff);
}



#endif