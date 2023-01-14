#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "common.hpp"

struct sprite{
	int  x;
	char img[8];
	char palette;
};

class Graphics{
	private:
	char *ram;
	char *IF, *lcdc, *stat, *scy, *scx, *ly, *lyc, *wy, *wx, *bgp, *obp;
	char *oam;
	bool preIFlg;
	int spriteSize;
	struct sprite sprite[10];
	
	ushort Pos2Tile();
	void Tile2Img(ushort tileID, ushort y, char *img);
	int SpriteSearch(int x, int line, int num);
	void NoDrow(ushort dummy1, ushort dummy2, char *img);
	
	public:
	ushort windowCounter;
	char cgbFlg[2] = {0xe4, 0xef};
	
	Graphics(char *vRam, char *ioReg, char *OAM, char ***palette);
	void Dummy(char *dummy);
	void Sequence(char *img);
	void NextLine(char num);
	void Mode0();
	void Mode3();
	
	

	
};




#endif