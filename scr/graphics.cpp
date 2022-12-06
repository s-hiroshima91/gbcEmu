#include "graphics.hpp"
#include <iostream>

Graphics::Graphics(char *vRam, char *ioReg, char *OAM){
	ram = vRam;
	IF = &ioReg[0x0f];
	lcdc = &ioReg[0x40];
	stat = &ioReg[0x41];
	scy = &ioReg[0x42];
	scx = &ioReg[0x43];
	ly = &ioReg[0x44];
	lyc = &ioReg[0x45];
	wy = &ioReg[0x4a];
	wx = &ioReg[0x4b];
	bgp = &ioReg[0x47];
	obp = &ioReg[0x48];
	*stat = 0xff;
	
	oam = OAM;
	for (int i = 0; i < 10; ++i){
		sprite[i].x = 0xff;
	}
	
}
	
ushort Graphics::Pos2Tile(){
	ushort tileID;
	tileID = C2US(*scx) >> 3;
	tileID += (C2US(*scy + *ly) & 0xf8) << 2;
	tileID += (*lcdc & 0x08) << 7;
	tileID += 0x1800;
	return tileID;
}

void Graphics::Tile2Img(ushort tileID, ushort y, char *img){
	ushort addr;
	char table1, table2;
	
	addr = static_cast<ushort>(*lcdc & 0x10) << 7;
	addr ^= 0x0800;
	y += addr;
	addr ^= C2US(ram[tileID]) << 4;
	addr += y;
	
	table1 = ram[addr];
	table2 = ram[addr + 0x01];
	
	for (int j = 0; j < 8; ++j){
		img[j] = CheckBitFF(table1, 7 - j) & 0x02;
		img[j] |= CheckBitFF(table2, 7 - j) & 0x04;
		img[j] = (*bgp >> img[j]) & 0x03;
	}
}

int Graphics::SpriteSearch(int x, int line, int num){
	int delta, addr;
	char table1, table2;
	x <<= 3;
	for (int i = 0; i < 2; ++i){
		if (num >= 10){
			return num;
		}
		delta = (static_cast<int>(oam[x]) & 0xff) -16;
		delta = line - delta;
		if ((delta >= 0) && (delta < spriteSize)){
			++x;
			sprite[num].x = static_cast<int>(oam[x]) & 0xff;
			++x;
			addr = static_cast<int>(oam[x]) << 4;
			addr &= 0x0fff ^ spriteSize;
			++x;
			
			sprite[num].palette = obp[CheckBitFF(oam[x], 4) & 0x01];
			sprite[num].priority = CheckBit(oam[x], 7);
			
			if (CheckBit(oam[x], 6)){
				addr += (spriteSize - 1 - delta) << 1;
			}else{
				addr += delta << 1;
			}
		
			table1 = ram[addr];
			table2 = ram[addr + 0x01];

			if (CheckBit(oam[x], 5)){
				addr = 0;
				delta = 1;
			}else{
				addr = 7;
				delta = -1;
			}

			for (int j = 0; j < 8; ++j){
				sprite[num].img[j] = CheckBitFF(table1, addr) & 0x02;
				sprite[num].img[j] |= CheckBitFF(table2, addr) & 0x04;
				addr += delta;
			}
			++x;
			++num;
		}else{
			x += 4;
		}
	}
	return num;
}	

void Graphics::Dummy(char *dummy){
	
}

void Graphics::NoDrow(ushort dummy1, ushort dummy2, char *img){
	for (int i = 0; i < 8; ++i){
		img[i] = 0;
	}
}	

void Graphics::Sequence(char *img){
	void (Graphics::*BgDrow)(ushort, ushort, char*);
	ushort tileID, y;
	char *temp;
	int line;
	int winFlg, bgEnd;
	temp = img;

	tileID = Pos2Tile();
	img += 8 - (*scx & 0x07);
		
	line = static_cast<int>(*ly) & 0xff;
	
	 if (CheckBit(*lcdc, 0)){
	 	BgDrow = &Graphics::Tile2Img;
	 }else{
	 	BgDrow = &Graphics::NoDrow;
	 }
	 
	if (CheckBit(*lcdc, 5)){
		winFlg = line - (static_cast<int>(*wy) & 0xff);
		if (winFlg >= 0){
			bgEnd = (static_cast<int>(*wx) & 0xff) >> 3;

			if (bgEnd > 20){
				bgEnd = 20;
			}
		}else{
			bgEnd = 20;
		}
	}else{
		bgEnd = 20;
	}
	
	if (CheckBit(*lcdc, 2)){
		spriteSize = 16;
	}else{
		spriteSize = 8;
	}

	y = (C2US(*ly + *scy) & 0x07) << 1;
	
	int num;
	
	if (CheckBit(*lcdc, 1)){
		num = 0;
	}else{
		num = 10;
	}
	
	
	int i = 0;
	while (i < bgEnd){
		num = SpriteSearch(i, line, num);
		/*Tile2Img*/(this->*BgDrow)(tileID, y, img);
		img += 8;
		
		if ((tileID & 0x1f) == 0x1f){
			tileID &= 0xffe0;
		}else{
			++tileID;
		}
		++i;
	}
	/*Tile2Img*/(this->*BgDrow)(tileID, y, img);
	
	if (i != 20){
		
		tileID = 0;
		tileID += (windowCounter & 0xf8) << 2;
		tileID += (*lcdc & 0x40) << 4;
		tileID += 0x1800;
		y = (windowCounter & 0x07) << 1;
		img = temp + (static_cast<int>(*wx) & 0xff) + 1;
		/*Tile2Img*/(this->*BgDrow)(tileID, y, img);
		++windowCounter;
	}


	while (i < 20){
		img += 8;
		++tileID;
		num = SpriteSearch(i, line, num);
		/*Tile2Img*/(this->*BgDrow)(tileID, y, img);
		++i;
	}
	char bg = (*bgp & 0x03);
	for (i = 9; i >= 0; --i){
		if (sprite[i].x < 168){
			if (!sprite[i].priority){
				for (int j = 0; j < 8; ++j){
					if (sprite[i].img[j] != 0){
						temp[sprite[i].x + j] = (sprite[i].palette >> sprite[i].img[j]) & 0x03;
					}
				}
			}else{
				for (int j = 0; j < 8; ++j){
					if ((sprite[i].img[j] != 0) && (temp[sprite[i].x + j] == bg)){
						temp[sprite[i].x + j] = (sprite[i].palette >> sprite[i].img[j]) & 0x03;
					}
				}
			}
			sprite[i].x = 0xff;
		}
	}
}

void Graphics::NextLine(char num){
	++*ly;
	*stat &= 0xf8;
	if (*ly == *lyc){		
		if (CheckBit(*stat, 6) && !preIFlg){
			preIFlg = true;
			*IF |=0x02;
		}
		*stat |= num | 0x04;
	}else{
		bool iFlg;
		*stat |= num;
		iFlg = CheckBit(*stat, static_cast<int>(num) + 3);
	
		preIFlg = (!preIFlg) & iFlg;
		if (preIFlg){
			*IF |= 0x02;
		}
		preIFlg = iFlg;
	}
}

void Graphics::Mode0(){
	*stat &= 0xfc;
	if (CheckBit(*stat, 3)){
		preIFlg = true;
		*IF = 0x02;
	}else{
		preIFlg = false;
	}	
}

void Graphics::Mode3(){
	*stat |= 0x03;
	preIFlg = false;
}