#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <dir.h>
#include <ctype.h>
#include "kurtan.h"

int Load(void)
{
	long len = 0;
	map_t buf;
	int i, j, f, r, cksum;
	lseek(fhVOL1, 0x32, SEEK_SET);
	r = read(fhVOL1, buf, sizeof(buf)); len += r;
	r = read(fhVOL1, CrPal, 16); len += r;
	r = read(fhVOL1, OutZmPc, 0x118); len += r;
	r = read(fhVOL1, PicCost, 0x44); len += r;	/*TODO: overwritten*/
	r = read(fhVOL1, PicCost, 0x48); len += r;
	r = read(fhVOL1, BonSz, 0x48); len += r;
	r = read(fhVOL1, BonCl, 0x48); len += r;
	r = read(fhVOL1, ZoomPic, 0x567C); len += r;
	r = read(fhVOL1, PicBufA, 0x1338); len += r;
	r = read(fhVOL1, PicBuf, 0xD74); len += r;
	r = read(fhVOL1, PicBuf, 0x4158); len += r;
	r = read(fhVOL1, PicMon, 0x48); len += r;
	r = read(fhVOL1, &cksum, 2); len += r;
	r = read(fhVOL1, &SmPicMon, 2); len += r;
	r = read(fhVOL1, SmPicH, 0x708); len += r;
	r = read(fhVOL1, SmPicV, 0x480); len += r;
	if(len != 0x10118ul)
	{
badfile1:
		printf("Отсутствует или неверный файл VOL1E.KRT");
		bioskey(0);
		return -1;
	}
	r = 0;
	for(i = 0;i < 36;i++)
		r += BonSz[i];
	if(r != cksum)
		goto badfile1;
	r = 0;
	for(i = 0;i < 6;i++)
		for(j = 0;j < 6;j++)
			r += PicMon[i][j];
	if(r != SmPicMon)
		goto badfile1;

	KrPcX = buf[0][0][25][15];
	KrPcY = buf[0][1][25][15];
	x0 = buf[0][2][25][15];
	y0 = buf[0][3][25][15];

	buf[KrPcX][KrPcY][x0][y0] = 3;
	TmpKRT = open("TEMPO.KRT", O_RDWR | O_BINARY);
	if(TmpKRT < 0)
		TmpKRT = open("TEMPO.KRT", O_CREAT | O_BINARY);
	len = write(TmpKRT, buf, sizeof(buf));
	_chmod("TEMPO.KRT", 1, FA_ARCH);
	r = close(TmpKRT);
	if(r < 0 || len != sizeof(buf))
	{
		printf("Hевозможно создать рабочий файл TEMPO.KRT");
		bioskey(0);
		return -1;
		
	}
	TmpKRT = open("TEMPO.KRT", O_RDWR | O_BINARY);
	for(i = 0;i < 6;i++)
		for(j = 0;j < 6;j++)
		{
			Scr[i][j] = PicHelp[i][j] = 0;
			PicTime[i][j] = PicCost[i][j] + PicCost[i][j] + PicCost[i][j] + 500;
			PicMov[i][j] = -1;
			BonKey[i][j] = BonEat[i][j] = 0;
		}
	for(i = 0;i < 20;i++)
		Sur[i] = 1;
	f = open("VOL2.KRT", O_RDWR | O_BINARY);
	if(f < 0)
	{
badfile2:
		printf("Отсутствует или неверный файл VOL2.KRT");
		bioskey(0);
		return -1;
	}
	len = 0;
	lseek(f, 20, SEEK_SET);
	r = read(f, BBB, sizeof(BBB)); len += r;
	r = read(f, BBC, sizeof(BBC)); len += r;
	r = close(f);
	if(r < 0 || len != (sizeof(BBB) + sizeof(BBC)))
		goto badfile2;
	Loter = Ord = 3;
	Mony0 = Dinam = Zaj = Inst = TotalScr = Moves = Moves0 = BnKey = 0;
	Mony = 101;
	DSlp = 130;
	Slp = 20;
	if(BnKey + Mony + TotalScr + KrPcX * y0 + KrPcY * x0 != 158)
		goto badfile1;
	return 0;
}

extern void EGAVGA_driver(void);

int Init(void)
{
	int f;
	int i = EGA;
	int j = EGAHI;
	registerbgidriver(EGAVGA_driver);
	initgraph(&i, &j, "");
	if(graphresult() != grOk)
	{
	    printf("Режим EGA (256K) недоступен!\n\r");
		return -1;
	}
	f = open("FONTR.KRT", O_BINARY);
	if(f < 0)
	{
		printf("Отсутствует файл шрифта FONTR.KRT");
		bioskey(0);
		return -1;
	}
	read(f, Font, 328);
	read(f, Font, 1504);
	close(f);
	setactivepage(1);
	setfillstyle(SOLID_FILL, 0);
	for(i = 0;i < 16;i++)
		setpalette(i, CrPal[i]);
	delay(5);
	xCr[0] += 10 + strlen(Msg[0]) * 8;
	xCr[1] += xCr[0] + 160;
	xCr[2] += strlen(Msg[1]) * 8 + xCr[1] + 10;
	xCr[3] += xCr[2] + 36;
	xCr[4] += strlen(Msg[2]) * 8 + xCr[3] + 10;
	xCr[5] += xCr[4] + 56;
	xCr[6] += strlen(Msg[3]) * 8 + xCr[5] + 10;
	xCr[18] += 600 - strlen(Msg[18]) * 8;
	xCr[7] += xCr[18] - 34;
	xCr[8] += 40 + strlen(Msg[6]) * 8;
	xCr[9] += strlen(Msg[7]) * 8 + xCr[8] + 40;
	xCr[10] += strlen(Msg[8]) * 8 + xCr[9] + 40;
	xCr[11] += strlen(Msg[9]) * 8 + xCr[10] + 40;
	xCr[12] += strlen(Msg[10]) * 8 + xCr[11] + 40;
	xCr[13] += 40 + strlen(Msg[12]) * 8;
	xCr[14] += strlen(Msg[13]) * 8 + xCr[13] + 40;
	xCr[15] += strlen(Msg[14]) * 8 + xCr[14] + 40;
	xCr[17] += 461;
	xCr[16] += 420 - strlen(Msg[5]) * 8;
	for(i = 0;i < 6;i++)
		for(j = 0;j < 6;j++)
			PcHelp[i][j] = 0;	
	return 0;
}

int Comp(int a, int x, int y)
{
	int r1[25];
	int r2[16];
	int i, j;
	for(i = 0;i < 25;i++) r1[i] = 0;
	for(i = 0;i < 16;i++) r2[i] = 0;
	for(i = 0;i < 25;i++)
		for(j = 0;j < 16;j++)
		{
			r1[i] += CurPic[i][j];
			r2[j] += CurPic[i][j];
		}
	if(a == 0)
	{
		for(i = 0;i < 25;i++)
			SmPicH[x][y][i] = r1[i];
		for(j = 0;j < 16;j++)
			SmPicV[x][y][j] = r2[j];
		return 0;
	}

	for(i = 0;i < 25;i++)
		if(SmPicH[x][y][i] != r1[i])
			return -1;
	for(j = 0;j < 16;j++)
		if(SmPicV[x][y][j] != r2[j])
			return -1;
	return 0;
}

/*Draw the room with coords x/y*/
int PicOut(int x, int y)
{
	int i, j;
	setpalette(7, CrPal[7]);
	setpalette(4, CrPal[4]);
	Text(0, 21);
	if(Show == 1)
	{
		lseek(fhVOL1, 0x32 + (x * 6 + y) * 26*16, SEEK_SET);
		read(fhVOL1, CurPic, 26*16);
	}
	else
	{
		lseek(TmpKRT, (x * 6 + y) * 26*16, SEEK_SET);
		read(TmpKRT, CurPic, 26*16);
	}
#if 1
	if(ActP == 0)
		ActP = 1;
	else
		ActP = 0;
#endif
	setactivepage(ActP);
	cleardevice();
	if(x == 0 && y == 4 && Sur[0] == 0)
		CurPic[10][5] = 1;
	else if(x == 3 && y == 1 && Sur[2] == 0)
		CurPic[4][4] = 1;
	else if(x == 2 && y == 3 && Sur[3] == 0)
		CurPic[15][11] = 1;
	else if(x == 4 && y == 1 && Sur[4] == 0)
		CurPic[20][5] = 1;
	else if(x == 3 && y == 5 && Sur[5] == 0)
		CurPic[14][6] = 1;
	else if(x == 1 && y == 0 && Sur[6] == 0)
		CurPic[14][13] = 1;
	else if(x == 0 && y == 3 && Sur[7] == 0)
		CurPic[16][14] = 1;
	else if(x == 0 && y == 1 && Sur[9] == 0)
		CurPic[19][6] = 1;
	else if(x == 1 && y == 5 && Sur[10] == 0)
		CurPic[22][9] = 1;
	else if(x == 0 && y == 0 && Sur[11] == 0)
		CurPic[21][4] = 1;
	else if(x == 5 && y == 2 && Sur[12] == 0)
		CurPic[2][13] = 1;
	else if(x == 5 && y == 0 && Sur[13] == 0)
		CurPic[2][7] = 1;
	else if(x == 3 && y == 2 && Sur[14] == 0)
		CurPic[12][3] = 1;
	else if(x == 5 && y == 1 && Sur[15] == 0)
		CurPic[7][4] = 1;
	/*draw room blocks*/
	for(i = 0;i < 25;i++)
		for(j = 0;j < 16;j++)
			putimage(i * 24, j * 20 + y00, &ZoomPic[CurPic[i][j]], 0);
	/**/
	chet = KolD = Dance = 0;
	xX0 = x0;
	yY0 = y0;
	setcolor(14);
	Gprintf(0, 341, Msg[MSG_Tonus]);
	Gprintf(xCr[1], 341, Msg[MSG_Labirint]);
	Gprintf(xCr[3], 341, Msg[MSG_Score]);
	Gprintf(xCr[18], 341, Msg[MSG_Time]);
	if(CurPic[25][11] != 0x1B)
	{
		setcolor(12);
		Gprintf(xCr[18] - 9, 341, "%c", 0x1B);
	}
	if(Show == 1)
		setcolor(12);
	else
		setcolor(14);
	Gprintf(xCr[5], 341, Msg[MSG_Help]);
	setcolor(11);
	bar(xCr[2], 341, xCr[2] + 16, 349);
	Gprintf(xCr[2], 341, "%c%d", KrPcX + 'A', KrPcY + 1);
	bar(xCr[6], 341, xCr[6] + 24, 349);
	if(PicCost[x][y] > Mony)
		setcolor(12);
	else
		setcolor(10);
	Gprintf(xCr[6], 341, "%d%s", PicCost[KrPcX][KrPcY], Dll);
	if(Dinam == 1)
		putimage(604, y00 + 136, PicBufA + 0x6BA, 0);
	else
		putimage(604, y00 + 136, &ZoomPic[3], 0);
	if(Zaj == 1)
		putimage(604, y00 + 165, PicBufA + 0x5C4, 0);
	else
		putimage(604, y00 + 165, &ZoomPic[3], 0);
	if(Inst == 1)
		putimage(604, y00 + 107, PicBufA + 0xA92, 0);
	else
		putimage(604, y00 + 107, &ZoomPic[3], 0);
	putimage(604, y00 + 194, &ZoomPic[Ord], 0);
	putimage(605, y00 + 223, &ZoomPic[Loter], 0);
	NepMn = 0x1B;
	BKey();
	Score(1, 0);
	Money(0);
	Score1(0);
	if(x == 3 && y == 3)
	{
		if(Dinam == 2)
		{
			putimage(432, y00 + 20, PicBufA+0x8A6, 0);
			putimage(432, y00 + 60, PicBufA+0x99C, 0);
		}
		else
			putpixel(420, y00 + 50, 12);
	}
	setpalette(9, CrPal[9]);
	if(BonKey[x][y] == 1)
		putimage(CurPic[25][5] * 24, CurPic[25][6] * 20 + y00, &ZoomPic[53], 0);
	if(BonEat[x][y] > 0)
	{
		setpalette(5, BonCl[CurPic[25][9]]);
		setpalette(6, BonCl[CurPic[25][9] + 36]);
		putimage(CurPic[25][1] * 24, CurPic[25][2] * 20 + y00, &ZoomPic[CurPic[25][9] + 54], 0);
	}
	setvisualpage(ActP);
	if(Show == 0 && Comp(1, x, y) < 0)
	{
		Score1(14);
		Text(0, 42);
		return -1;
	}
	if(PicCost[x][y] + PicCost[x][y] + PicCost[x][y] + 500 == PicTime[x][y])
	{
		time(&CrTime);
		MyTime = CrTime;
	}
	Time(0);
	setcolor(14);
	Gprintf(xCr[5], 341, Msg[MSG_Help]);
	while(bioskey(1) != 0) bioskey(0);
	return 0;
}

void BKey(void)
{
	bar(605, y00 + 313, 630, y00 + 322);
	setcolor(14);
	if(BnKey == 0)
	{
		putimage(605, y00 + 290, &ZoomPic[3], 0);
	}
	else
	{
		putimage(605, y00 + 290, &ZoomPic[53], 0);
		Gprintf(606, y00 + 313, "%2d", BnKey);
	}
}

void Score(int a0, int score)
{
	if(a0 == 1)
	{
		Scr[KrPcX][KrPcY] += score;
	}
	TotalScr += score;

	if(TotalScr < 0)
		TotalScr = 0;
	if(Scr[KrPcX][KrPcY] < 0)
		Scr[KrPcX][KrPcY] = 0;

	setcolor(11);
	bar(xCr[4], 340, xCr[4] + 40, 349);
	Gprintf(xCr[4], 341, "%5ld", TotalScr);
}

void Score1(int c)
{
	int i;
	if(c != 0)
	{
		setfillstyle(1, c);
		bar(xCr[7], 340, xCr[7] + 10, 348);
	}
	else
	{
		setfillstyle(1, 11);
		for(i = 0;i < 90 - Slp;i++)
			bar(xCr[0] + i * 2, 340, xCr[0] + i * 2 + 1, 348);
		setfillstyle(1, 12);
		for(i = 90 - Slp;i < 70;i++)
			bar(xCr[0] + i * 2, 340, xCr[0] + i * 2 + 1, 348);
		if(DSlp == 35)
			End = Key = 0x1B;
		if(DSlp == 40)
			Palet(7, 44);
	}
	setfillstyle(1, 0);
}

void HelpSh(int x, int y)
{
	int ofs = 2;
	int i, mx, h;
	int key;
	char buf[2000];
	char *minmax[] = {
		"min",
		"max",
		"+"
	};
	int len;
	char img[] = {11, 30, 31, 32, 31, 11};
	char ktab[] = {33, 31, 28, 36};
	if(PicCost[KrPcX][KrPcY] > Mony)
	{
		Score1(14);
		Text(0, 0);
		Score1(10);
		return;
	}
	if(CurPic[x0][y0] == 1)
	{
		Score1(14);
		Text(0, 25);
		Score1(10);
		return;
	}
	Score1(12);
	if(PicHelp[x][y] > 0 || PcHelp[x][y] > 0)
		Text(1, 8);
	PicHelp[x][y]++;
	PcHelp[x][y]++;
	Show = 1;
	len = BBC[x][y];
	h = open("VOL2.KRT", O_RDWR | O_BINARY);
	if(h < 0)
	{
		printf("Отсутствует или неверный файл VOL2.KRT");
		bioskey(0);
		return;
	}
	lseek(h, BBB[x][y], SEEK_SET);
	read(h, buf, len);
	close(h);
	PicTime[x][y] = PicCost[x][y] + PicCost[x][y] + PicCost[x][y] + 500;
	x0 = buf[0];
	y0 = buf[1];
	TotalScr -= Scr[KrPcX][KrPcY];
	if(CurPic[25][6] == 56)
		BnKey--;
	PicMov[x][y] = BonEat[x][y] = BonKey[x][y] = Scr[x][y] = 0;
	PicOut(x, y);
	Money(PicCost[x][y] * -1);
	setpalette(7, 3);
	setpalette(4, 32);
	putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[19], 0);
	setcolor(14);
	Gprintf(17, 0, "-%s", Msg[MSG_Stop]);
	Gprintf(xCr[16], 0, "%s", Msg[MSG_Speed]);
	setcolor(11);
	Gprintf(0, 0, "F1");
	if(Sl == 81)
	{
		setcolor(12);
		Gprintf(xCr[17] - 28, 0, minmax[0]);
		setcolor(11);
	}
	else
		Gprintf(xCr[17] - 20, 1, "-");
	if(Sl == 1)
	{
		setcolor(12);
		Gprintf(xCr[17] + 115, 0, minmax[1]);
	}
	else
		Gprintf(xCr[17] + 123, 1, minmax[2]);
	setfillstyle(1, 3);
	bar(xCr[17] - 2, 0, xCr[17] + 112, 7);
	setfillstyle(1, 14);
	mx = 15 - Sl / 5;
	for(i = 0;i <= mx;i++)
		bar(xCr[17] + i * 7, 1, xCr[17] + i * 7 + 5, 6);
	setfillstyle(1, 0);
	Score1(15);
	while(ofs < len)
	{
		Key = ktab[buf[ofs]] + 0x12C;
		ofs++;
		switch(Key)
		{
		case 0x14D:
			OutMove(21, 1, 0);
			break;
		case 0x14B:
			OutMove(27, -1, 0);
			break;
		case 0x150:
			OutMove(33, 0, 1);
			break;
		case 0x148:
			OutMove(39, 0, -1);
			break;
		}
		if(PicTime[KrPcX][KrPcY] < 0)
			ofs = len;
		if(KrPcX == 5 && KrPcY == 4 && Scr[5][4] == 250 && CurPic[20][13] == 18)
		{
			Score1(12);
			putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[21], 0);
			Napluv(20, 13, &ZoomPic[14]);
			CurPic[20][13] = 14;
			Score1(15);
		}
		if(bioskey(1) != 0)
		{
			key = ReadKey();
			Key = key;
			switch(key)
			{
			case 0x13B:
				ofs = len;
				break;
			case 0x143:
				return;
			case '-':
			case 0x14B:
				if(Sl == 81)
					break;
				setfillstyle(1, 3);
				bar(xCr[17] + mx * 7, 1, xCr[17] + mx * 7 + 5, 1 + 5);
				setfillstyle(1, 0);
				Sl += 5;
				if(Sl == 81)
				{
					bar(xCr[17] - 20, 0, xCr[17] - 20 + 8, 7);
					setcolor(12);
					Gprintf(xCr[17] - 28, 0, minmax[0]);
				}
				else if(Sl == 6)
				{
					bar(xCr[17] + 115, 0, xCr[17] + 139, 7);
					setcolor(11);
					Gprintf(xCr[17] + 123, 1, minmax[2]);
				}
				mx--;
				break;
			case '+':
			case 0x14D:
				if(Sl == 1)
					break;
				Sl -= 5;
				if(Sl == 1)
				{
					bar(xCr[17] + 123, 0, xCr[17] + 131, 7);
					setcolor(12);
					Gprintf(xCr[17] + 115, 0, minmax[1]);
				}
				else if(Sl == 76)
				{
					bar(xCr[17] - 28, 0, xCr[17] - 4, 7);
					setcolor(11);
					Gprintf(xCr[17] - 20, 1, "-");
				}
				mx++;
				setfillstyle(1, 14);
				bar(xCr[17] + mx * 7, 1, xCr[17] + mx * 7 + 5, 6);
				setfillstyle(1, 0);
				break;
			}
			while(bioskey(1))
				Key = ReadKey();
		}
		if(End == 27)
		{
			Show = 0;
			return;
		}
		if(CurPic[25][0] == 0)
		{
			CurPic[25][10] = 27;
			if(CurPic[25][7] != 27)
				Trunk(CurPic[25][7], CurPic[25][8], 1);
		}
		if(BonEat[x][y] > 0 && (PicMov[KrPcX][KrPcY] - BonEat[x][y]) > 32)
		{
			BonEat[x][y] = 0;
			putimage(CurPic[25][1] * 24, CurPic[25][2] * 20 + y00, &ZoomPic[3], 0);
			CurPic[25][1] = 27;
		}
		if(CurPic[25][1] != 27 && BonEat[x][y] == 0
		&& PicMov[KrPcX][KrPcY] >= BonSz[CurPic[25][9]]
		&& CurPic[CurPic[25][1]][CurPic[25][2]] < 5
		&& (CurPic[25][1] != x0 || CurPic[25][2] != y0))
		{
			setpalette(5, BonCl[CurPic[25][9]]);
			setpalette(6, BonCl[36 + CurPic[25][9]]);
			BonEat[x][y] = PicMov[KrPcX][KrPcY];
			putimage(CurPic[25][1] * 24, CurPic[25][2] * 20 + y00, &ZoomPic[CurPic[25][9] + 54], 0);
		}
		if(CurPic[25][0] <= 2 && CurPic[25][5] != 27
		&& CurPic[CurPic[25][5]][CurPic[25][6]] <= 8
		&& CurPic[25][5] != x0 && CurPic[25][6] != y0)
		{
			BonKey[x][y] = 1;
			putimage(CurPic[25][5] * 24, CurPic[25][6] * 20 + y00, &ZoomPic[53], 0);
		}
	}
	bar(0, 0, 639, 8);
	Score1(12);
	for(ofs = 0;ofs < 6;ofs++)
	{
		putimage(x0 * 24, y0 * 20 + y00, &PicBuf[img[ofs]], 0);
		delay(300);
	}
	Show = 0;
	setpalette(7, CrPal[7]);
	setpalette(4, CrPal[4]);
	Delete = 0;
	while(bioskey(1) != 0)
		Key = ReadKey();
	Score1(10);
}

int Present(void)
{
	int psz = 28006;
	int x = 0, y = 0;
	int i, j, k, a;
	int speed = 128;
	int h, hx;
	int colors[] = {7, 4, 12, 2, 10, 2, 7, 11, 7, 14, 12, 15};
	char img1[] = {0, 18, 9, 6, 10, 12, 14, 53, 15, 16, 99, 8};
	char len1[] = {6, 12, 19, 19, 19, 19, 19, 14, 9, 7, 4, 3};
	char img2[] = {11, 30, 31, 32, 31, 11};
	char tt[19][12];
	char tmp[246];
	char *pic;
	char *txt1 = "Атрибуты игры";
	char *txt2 = "Edktrfntkmyfz kjubxtcrfz buhf";
	char *help1[] = {
		"  Ajy                    ",
		"                   Cntyf ",
		"  Uhep                   ",
		"         Vtcnj lkz uhepf ",
		"  Uhep yf vtcnt          ",
		"           Ghbpjdjq uhep ",
		"  Rfkbnrf                ",
		"                    Rk`x ",
		"  Ktcnybwf               ",
		"      Dfk`nf ^ rfntujhbb ",
		"  Ghbps                  ",
		"                 C`hghbp "
	};
	int h2cnt = 24;
	char *help2[] = {
		"   Wtkm buhs pfrk`xftncz d  hfccnfyjdrt uhepjd yf",
		"cgtwbfkmyj jndtltyyst lkz yb[ vtcnf dj dct[  36-b",
		"kf/bhbynf[. Uheps hfccnfdkz`ncz yjcbkmobrjv, ldb-",
		";tybt rjnjhjuj pflftncz rkfdbifvb eghfdktybz reh-",
		"cjhjv. Ghb jlyjrhfnyjv yf;fnbb jlyjq bp 'nb[ rkf-",
		"dbi, jy yfxbyftn  ldbufnmcz d ds/hfyyjv yfghfdkt-",
		"ybb.  Lkz cvtys yfghfdktybz  ldb;tybz  ytj/[jlbvj",
		"yf;fnm cjjndtncnde`oe` rkfdbie  eghfdktybz rehcj-",
		"hjv, kb/j k`/e` lheue` - lkz tuj  jcnfyjdrb.  Ghb",
		"ldb;tybb d njv bkb  byjv  yfghfdktybb,  yjcbkmobr",
		"vj;tn  njkrfnm (yj yt  nfobnm pf  cj/jq)  jlby  b",
		"njkmrj jlby uhep.",
		"   Gj vtht ldb;tybz yjcbkmobr ecnftn, tuj ;bpyty-",
		"ysq njyec cyb;ftncz b vj;tn yfcnegbnm vjvtyn, rj-",
		"ulf jy, eds, jnghfdbncz d vbh byjq.  Cjdtnetv Dfv",
		"dhtvz jn dhtvtyb gjlrhtgkznmcz ghbpjdsvb  ghjler-",
		"nfvb, yj ltkfnm 'nj yflj jcnjhj;yj ...",
		"  D ytrjnjhs[ uhepf[ [hfyzncz ltymub, rjnjhsvb Ds",
		"vj;tnt hfcgjkfufnm. Ghb yfkbxbb e Dfc ljcnfnjxyjq",
		"cevvs, yjcbkmobrf vj;yj eujdjhbnm hfccnfdbnm uhe-",
		"ps d kf/bhbynt /tp Dfituj exfcnbz.",
		"   Jlybv ckjdjv, Dfc ;ltn vyj;tcndj c`hghbpjd, vs",
		";tkftv Dfv elfxb !",
		"   Rkfdbif T&C - dspjd ukfdyjuj vty`"
	};
	while(bioskey(1))
		Key = ReadKey();
	pic = (char*)malloc(psz);
	h = open("VOL0R.KRT", O_BINARY);
	if(h < 0)
	{
		printf("Отсутствует или неверный файл VOL0R.KRT");
		bioskey(0);
		return -1;
	}
	read(h, pic, 18);
	read(h, pic, psz);
	putimage(0, 0, pic, 0);
	read(h, pic, psz);
	putimage(320, 0, pic, 0);
	read(h, pic, psz);
	putimage(0, 175, pic, 0);
	read(h, pic, psz);
	putimage(320, 175, pic, 0);
	free(pic);
	close(h);
	psz = imagesize(335, 219, 576, 323);
	pic = (char*)malloc(psz);
	getimage(395, 219, 636, 323, pic);
	setactivepage(0);
	setcolor(10);
	i = strlen(txt2) * 8;
	hx = 367 + (242 - i) / 2;
	Gprint(0, hx, 200, 2, 10, txt2);
	hx += i - 392;
	delay(400);
	putimage(367, 224, pic, 0);
	free(pic);
	if(bioskey(1) != 0)
		goto quit;
	delay(300);
	for(i = 0;i < 19;i++)
	{
		for(j = 0;j < 12;j++)
		{
			if(i < len1[j])
				tt[i][j] = 0;
			else
				tt[i][j] = 1;
		}
	}
	srand((int)time(NULL));
	for(k = 0;k < 150;)
	{
		a = rand() % 228;
		i = a % 19;
		j = a / 19;
		if(tt[i][j] != 0)
			continue;
		k++;
		tt[i][j] = 1;
		setactivepage(1);
		getimage(x + i * 24, y + j * 19, x + i * 24 + 23, y + j * 19 + 18, tmp);
		setactivepage(0);
		putimage(x + i * 24, y + j * 19, tmp, 0);
		delay(speed);
		if(k % 15 == 0)
			speed /= 2;
		if(bioskey(1) != 0)
			speed = 0;
	}
	if(bioskey(1) != 0)
		goto quit;
	bar(367, 224, 616, 328);
	psz = imagesize(474, 0, 635, 1);
	pic = (char*)malloc(psz);
	for(j = 0;j < 76;j += 2)
	{
		setactivepage(1);
		getimage(474, j, 635, j + 1, pic);
		setactivepage(0);
		putimage(474, j, pic, 0);
		setactivepage(1);
		getimage(474, 150 - j, 635, 151 - j, pic);
		setactivepage(0);
		putimage(474, 150 - j, pic, 0);
		delay(20);
	}
	free(pic);
	setactivepage(1);
	EE[0] = (char*)malloc(102);
	EE[1] = (char*)malloc(102);
	EC[0] = (char*)malloc(390);
	EC[1] = (char*)malloc(390);
	EC[2] = (char*)malloc(390);
	getimage(569, 26, 581, 37, EE[0]);
	getimage(569, 158, 581, 169, EE[1]);
	getimage(603, 114, 632, 137, EC[0]);
	getimage(603, 155, 632, 178, EC[1]);
	getimage(603, 180, 632, 203, EC[2]);
	if(bioskey(1) != 0)
		goto quit;
	setactivepage(0);
	for(k = 0;k < h2cnt;k++)
	{
		Gprint(1, hx, 230 + k * 18, 1, 15, help2[k]);
		if(bioskey(1) != 0)
			break;
	}
	if(bioskey(1) == 0)
		delay(1500);
	bar(hx, 228, hx + 393, 349);
	if(bioskey(1) != 0)
		goto quit;
	delay(800);
	yY0 = 320;
	xX0 = 40;
	setpalette(5, BonCl[6]);
	setpalette(6, BonCl[42]);
	setcolor(15);
	Gprintf((int)(289 - strlen(txt1) * 8) / 2 + 10, yY0 - 14, txt1);
	line(10, yY0 - 4, 298, yY0 - 4);
	a = 1;
	for(k = 0;k < 12;k++)
	{
		if(a > 0)
			i = 10;
		else
			i = 275;
		if(img1[k] < 90)
			putimage(i, yY0, &ZoomPic[img1[k]], 0);
		Print(a, colors[k], k, help1[k]);
		if(a > 0)
			bar(10, yY0, 240, yY0 + 20);
		else
			bar(70, yY0, 300, yY0 + 20);
		a *= -1;
		if(bioskey(1) != 0)
			goto quit;
	}
	bar(10, yY0 - 14, 258 + xX0, yY0 - 4);
	for(k = 0;k < 6;k++)
	{
		putimage(xX0, yY0, &PicBuf[img2[k]], 0);
		delay(250);
	}
	putimage(xX0, yY0, &ZoomPic[3], 0);
	if(bioskey(1) != 0)
		goto quit;
quit:;
	psz = imagesize(335, 219, 576, 323);
	pic = (char*)malloc(psz);
	setactivepage(1);
	getimage(395, 219, 636, 323, pic);
	setactivepage(0);
	putimage(367, 224, pic, 0);
	free(pic);
	bar(0, 300, 300, 349);
	free(EE[0]);
	free(EE[1]);
	free(EC[0]);
	free(EC[1]);
	free(EC[2]);
	return 0;
}

void Print(int a, int color, int c, char *s)
{
	int i, k, m, j;
	int mm, pp, c2 = 0;
	char img[] = {1, 0, -1, 0};
	delay(100);
	if(a > 0)
	{
		pp = 21;
		mm = 0;
	}
	else
	{
		pp = 27;
		mm = 7;
	}
	for(i = 0;i < 25;i++)
	{
		if(i % 5 == 0)
		{
			putimage(569, 26, EE[i % 2], 0);
			putimage(595, 26, EE[i % 2], 0);
		}
		putimage(603, 114, EC[i % 3], 0);
		if(c == 10 && (i % 3 == 0))
		{
			setpalette(5, BonCl[c2]);
			setpalette(6, BonCl[36 + c2]);
			putimage(10, yY0, &ZoomPic[c2 + 54], 0);
			c2++;
		}
		j = 0;
		m = mm;
		while(j <= 7)
		{
			if(xX0 % 4 == 0)
				putimage(xX0, yY0, &ZoomPic[img[i % 4] + pp], 0);
			for(k = 0;k < 16;k++)
			{
				if(Font[(s[(1 - a) * 12 + i * a] - ' ')][k] & Mask[m])
					putpixel(xX0 + 12 - a * 13, yY0 + k + 5, color);
			}
			m += a;
			j++;
			xX0 += a;
			if(bioskey(1))
				return;
			delay(10);
		}
	}
	if(a > 0)
		putimage(xX0, yY0, &ZoomPic[39], 0);
	else
		putimage(xX0, yY0, &ZoomPic[19], 0);
	delay(50);
}


int FilMenu(int key)
{
	int num;
	int x;
	int sel, psel, cnt;
	char names[16][9];
	int j, i;
	int res;
	struct ffblk ff;
	char *path = "*.ksg";
	char *pic;
	int psz;
retry:
	num = -1;
	cnt = num;
	sel = 0;
	x = (key - 0x13E) * xCr[13];
	x += 16;
	res = findfirst(path, &ff, 0);
	while(res >= 0 && num < 15)
	{
		num++;
		for(i = 0;i < 9 && ff.ff_name[i] != '.';i++)
			names[num][i] = ff.ff_name[i];
		names[num][i] = 0;
		if(!strcmp(Name, names[num]))
			sel = num;
		res = findnext(&ff);
	}
	if(key == 0x13F && num < 0)
		res = num;
	else
	{
		if(num < 15 && key == 0x13E)
		{
			num++;
			cnt = num;
			strcpy(names[num], Msg[MSG_NewFile]);
		}
		setcolor(11);
		if(key == 0x13E)
		{
			Gprintf(0, 0, "F4");
			setcolor(14);
			Gprintf(17, 0, "-%s", Msg[MSG_Save]);			
		}
		else
		{
			Gprintf(xCr[13], 0, "F5");
			setcolor(14);
			Gprintf(xCr[13] + 17, 0, "-%s", Msg[MSG_Load]);			
		}
		if(num == 0 && key == 0x13E)
			res = EnterN(x, y00);
		else
		{
			psz = imagesize(x - 4, y00, x + 77, num * 12 + y00 + 18);
			pic = (char*)malloc(psz);
			getimage(x - 4, y00, x + 77, num * 12 + y00 + 18, pic);
			bar(x - 4, y00, x + 77, num * 12 + y00 + 18);
			setcolor(11);
			rectangle(x, y00, x + 73, num * 12 + y00 + 15);
			for(j = 0;j <= num;j++)
			{
				if(j == cnt)
					setcolor(11);
				else
					setcolor(14);
				Gprintf(x + 6, j * 12 + y00 + 5, "%s", names[j]);
			}
			setfillstyle(SOLID_FILL, 4);
			setcolor(12);
			bar(x + 3, sel * 12 + y00 + 3, x + 71, sel * 12 + y00 + 13);
			Gprintf(x + 6, sel * 12 + y00 + 5, "%s", names[sel]);
			res = 0;
			while(res != 0x1B && res != 0x0D)
			{
				while(bioskey(1) == 0 && CurPic[25][11] != 0x1B)
				{
					if(PicTime[KrPcX][KrPcY] < 4)
					{
						res = 0x1B;
						goto over;
					}
					if(time(&CrTime) - MyTime != pTime)
						Time(2);
				}
				res = ReadKey();
over:;				
				psel = sel;
				switch(res)
				{
				case 0x14B:
				case 0x14D:
					if(NepMn == 0)
						break;
					putimage(x - 4, y00, pic, 0);
					free(pic);
					setfillstyle(SOLID_FILL, 0);
					bar(0, 0, 210, 9);
					if(key == 0x13E)
						key = 0x13F;
					else
						key = 0x13E;
					goto retry;
				case 0xD:
					if(sel == cnt)
					{
						res = EnterN(x, sel * 12 + y00 + 3);
						if(res == 0x1B)
							res = 0;
					}
					else
						strcpy(Name, names[sel]);
					Key = key;
					break;
				case 0x148:
					sel--;
					goto nav;
				case 0x150:
					sel++;
					goto nav;
				case 0x147:
					sel = 0;
					goto nav;
				case 0x14F:
					sel = num;
nav:;
					setfillstyle(SOLID_FILL, 0);
					if(psel == cnt)
						setcolor(11);
					else
						setcolor(14);
					bar(x + 3, psel * 12 + y00 + 3, x + 71, psel * 12 + y00 + 13);
					Gprintf(x + 6, psel * 12 + y00 + 5, "%s", names[psel]);
					if(sel < 0)
						sel = num;
					if(sel > num)
						sel = 0;
					setfillstyle(SOLID_FILL, 4);
					setcolor(12);
					bar(x + 3, sel * 12 + y00 + 3, x + 71, sel * 12 + y00 + 13);
					Gprintf(x + 6, sel * 12 + y00 + 5, "%s", names[sel]);
					break;										
				}
			}
			putimage(x - 4, y00, pic, 0);
			free(pic);
		}
	}
	setfillstyle(SOLID_FILL, 0);
	bar(0, 0, 639, 9);
	return res;
}

int EnterN(int x, int y)
{
	int i;
	int key = 0;
	char nam[10];
	char *pic, *pic2;
	char cur1, cur2 = 0, len = 0;
	int psz;
	psz = imagesize(x - 2, y - 2, x + 75, y + 13);
	pic = (char*)malloc(psz);
	getimage(x - 2, y - 2, x + 75, y + 13, pic);
	setfillstyle(SOLID_FILL, 0);
	bar(x - 2, y - 2, x + 75, y + 13);
	setcolor(11);
	rectangle(x - 2, y - 1, x + 75, y + 13);
	setfillstyle(SOLID_FILL, 12);
	bar(x + 6, y + 2, x + 13, y + 10);
	setcolor(0);
	psz = imagesize(x + 6, y + 2, x + 13, y + 10);
	pic2 = (char*)malloc(psz);
	getimage(x + 6, y + 2, x + 13, y + 10, pic2);
	setfillstyle(SOLID_FILL, 0);

	while(key != 0x1B && key != 0x0D)
	{
		while(bioskey(1) == 0 && CurPic[25][11] != 0x1B)
		{
			if(PicTime[KrPcX][KrPcY] < 4)
			{
				key = 0x1B;
				goto over;
			}
			if(time(&CrTime) - MyTime != pTime)
				Time(2);
		}
		key = ReadKey();
		cur1 = cur2;
over:;				
		switch(key)
		{
		case 0x0D:
			if(len == 0)
			{
				key = 0x1B;
				goto over;
			}
			nam[len] = 0;
			strcpy(Name, nam);
			break;
		case 0x1B:
			break;
		case 8:
			if(cur2 == 0)
				break;
			setcolor(12);
			putimage(cur2 * 8 + x + 6, y + 2, pic2, XOR_PUT);
			bar(len * 8 + x - 2, y + 2, len * 8 + x - 2 + 8, y + 2 + 8);
			len--;
			cur2--;
			for(i = cur2;i < len;i++)
			{
				nam[i] = nam[1 + i];
				bar(i * 8 + x + 6, y + 2, i * 8 + x + 6 + 8, y + 2 + 8);
				Gprintf(i * 8 + x + 6, y + 3, "%c", nam[i]);
			}
			putimage(cur2 * 8 + x + 6, y + 2, pic2, XOR_PUT);
			setcolor(0);
			break;
		default:
			if(key > 255)
				key = 0;
			key = toupper(key);
			if(!(key >= 'A' && key <= 'Z') && !(key >= '0' && key <= '9'))
			{
				sound(800);
				delay(50);
				nosound();
				break;
			}
			putimage(cur2 * 8 + x + 6, y + 2, pic2, COPY_PUT);
			Gprintf(cur2 * 8 + x + 6, y + 3, "%c", toupper(key));
			if(cur2 == len)
				len++;
			nam[cur2] = key;
			/*fall*/
		case 0x14D:
			cur1++;
			goto nav;
		case 0x14B:
			cur1--;
			goto nav;
		case 0x147:
			cur1 = 0;
			goto nav;
		case 0x14F:
			if(len > 7)
				cur1 = 7;
			else
				cur1 = len;
nav:;
			if(cur1 < 0 || cur1 > len || cur1 > 7)
				break;
			putimage(cur2 * 8 + x + 6, y + 2, pic2, XOR_PUT);
			cur2 = cur1;
			putimage(cur2 * 8 + x + 6, y + 2, pic2, XOR_PUT);
		}
	}
	putimage(x - 2, y - 2, pic, 0);
	free(pic);
	free(pic2);
	return key;
}

int File(void)
{
	int h;
	int vars[36];
	int lenreal = 18364;
	int len = 0;
	int l = 0;
	int i;
	char name[54];
	char buf[6][6][26][16];
	Score1(11);
	if(FilMenu(Key) == 0x1B)
	{
		Score1(10);
		return 0;
	}
	strcpy(name, "");
	strcat(name, Name);
	strcat(name, ".ksg");
	if(Key == 0x13E)
	{
		Comp(0, KrPcX, KrPcY);
		lseek(TmpKRT, (KrPcX * 6 + KrPcY) * 416, SEEK_SET);
		write(TmpKRT, CurPic, 416);
		lseek(TmpKRT, 0, SEEK_SET);
		read(TmpKRT, buf, sizeof(buf));
		h = open(name, O_RDWR | O_BINARY);
		if(h < 0)
			h = open(name, O_CREAT | O_BINARY);
		for(i = 0;i < 20;i++)
			vars[i] = Sur[i];
		vars[20] = Mony;
		vars[21] = DSlp;
		vars[22] = Slp;
		vars[23] = KolD;
		vars[24] = Moves0;
		vars[25] = BnKey;
		vars[26] = Loter;
		vars[27] = Moves;
		vars[28] = CrPal[9];
		vars[29] = Ord;
		vars[30] = Dinam;
		vars[31] = Zaj;
		vars[32] = Inst;
		vars[33] = Mony0;
        vars[34] = (Dinam + Zaj + Inst + 4) * (BnKey + 2) + Mony + Mony0 + (KrPcX + 1) * (y0 + 3) - (KrPcY + 1) * (x0 + 2);
		buf[0][0][25][15] = KrPcX;
		buf[0][1][25][15] = KrPcY;
		buf[0][2][25][15] = x0;
		buf[0][3][25][15] = y0;
		l = write(h, buf, sizeof(buf)); len += l;
		l = write(h, PicMov, sizeof(PicMov)); len += l;
		l = write(h, PicTime, sizeof(PicTime)); len += l;
		l = write(h, Scr, sizeof(Scr)); len += l;
		l = write(h, vars, sizeof(vars)); len += l;
		l = write(h, &TotalScr, sizeof(TotalScr)); len += l;
		l = write(h, SmPicH, sizeof(SmPicH)); len += l;
		l = write(h, SmPicV, sizeof(SmPicV)); len += l;
		l = write(h, PicHelp, sizeof(PicHelp)); len += l;
		l = write(h, BonKey, sizeof(BonKey)); len += l;
		l = write(h, BonEat, sizeof(BonEat)); len += l;
		_chmod(name, 1, FA_ARCH);
		l = close(h);
		Score1(14);
		if(l < 0 || len != lenreal)
			Text(0, 3);
		else
			Text(0, 5);
		Score1(10);
		return 0;
	}
	if((h = open(name, O_RDWR | O_BINARY)) < 0)
	{
		Score1(14);
		Text(0, 4);
		Score1(10);
		return 0;
	}
	len = 0;
	l = read(h, buf, sizeof(buf)); len += l;
	l = read(h, PicMov, sizeof(PicMov)); len += l;
	l = read(h, PicTime, sizeof(PicTime)); len += l;
	l = read(h, Scr, sizeof(Scr)); len += l;
	l = read(h, vars, sizeof(vars)); len += l;
	l = read(h, &TotalScr, sizeof(TotalScr)); len += l;
	l = read(h, SmPicH, sizeof(SmPicH)); len += l;
	l = read(h, SmPicV, sizeof(SmPicV)); len += l;
	l = read(h, PicHelp, sizeof(PicHelp)); len += l;
	l = read(h, BonKey, sizeof(BonKey)); len += l;
	l = read(h, BonEat, sizeof(BonEat)); len += l;
	for(i = 0;i < 20;i++)
		Sur[i] = vars[i];
	Mony = vars[20];
	DSlp = vars[21];
	Slp = vars[22];
	KolD = vars[23];
	Moves0 = vars[24];
	BnKey = vars[25];
	Loter = vars[26];
	Moves = vars[27];
	CrPal[9] = vars[28];
	Ord = vars[29];
	Dinam = vars[30];
	Zaj = vars[31];
	Inst = vars[32];
	Mony0 = vars[33];
	KrPcX = buf[0][0][25][15];
	KrPcY = buf[0][1][25][15];
	x0 = buf[0][2][25][15];
	y0 = buf[0][3][25][15];
	if(len != lenreal || lseek(h, 0, SEEK_END) != lenreal
	|| (Dinam + Zaj + Inst + 4) * (BnKey + 2) + Mony + Mony0 + (KrPcX + 1) * (y0 + 3) - (KrPcY + 1) * (x0 + 2) != vars[34])
	{
		Score1(12);
		Text(1, 42);
		return 0x1B;
	}
	close(h);
	lseek(TmpKRT, 0, SEEK_SET);
	write(TmpKRT, buf, sizeof(buf));
	Score1(12);
	if(PicOut(KrPcX, KrPcY) < 0)
		return 0x1B;
	putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[19], 0);
	Score1(14);
	Text(0, 6);
	Score1(10);
	return 0;
}

int GameOver(void)
{
	int i;
	int x, margin2, margin3, margin1, width;
	int colors[] = {14, 10, 12};
	char over[] = "Rjytw buhs";
	char cast[] = "Действующие лица и исполнители:";
	char note[] = "Будем рады Вашим замечаниям и пожеланиям";
	char *names[] = {
		"Ваш покорный слуга",
		"Малышка Розали",
		"Прокурор",
		"Интердевушка",
		"Служба контроля COPYRIGHT",
		"Главный киллер",
		"Красотка Hатали",
		"Налоговый инспектор",
		"Отношения к игре не имеет",
		"Комиссар"
	};
	char *cred[] = {
		"Разработка  ",
		"Игра написана и",
		"спроектирована",
		"Спасибо",
		"N/bkb",
		"Cjan",
		"380075 Тбилиси",
		"ул.Ленина 77, корпус VI",
		"тел. 36-54-76, 36-65-67",
		"телекс  212100 ALMAZ SU",
		"Htdfpjv C.Vtcfhrbidbkb",
		"Dfv  b"
	};
	if(DSlp == 35)
	{
		Score1(0);
		Score1(12);
		for(i = 0;i < 4;i++)
		{
			putimage(x0 * 24, y0 * 20 + y00, &PicBuf[i + 26], 0);
			delay(250);
		}
		delay(200);
		Napluv(x0, y0, &PicBuf[33]);
		Score1(14);
		Text(Show, 14);		
	}
	width = strlen(cast) * 8;
	margin1 = (639 - width) / 2;
	margin2 = (639 - (int)strlen(over) * 8) / 2;
	margin3 = (639 - (int)strlen(note) * 8) / 2;
	x = 296;
	if(ActP == 0)
		ActP = 1;
	else
		ActP = 0;
	setactivepage(ActP);
	cleardevice();
	setpalette(9, 9);
	setcolor(3);
	Gprintf(margin3, 227, cred[0]);
	Gprintf(margin3, 283, cred[1]);
	Gprintf(margin3, 293, cred[2]);
	Gprintf(margin3, 314, cred[3]);
	Gprintf(margin3, 340, note);
	Gprint(0, x - 2, 224, 4, 12, cred[4]);
	Gprint(0, x + 39, 224, 1, 9, cred[5]);
	setcolor(11);
	Gprintf(x, 240, cred[6]);
	Gprintf(x, 250, cred[7]);
	Gprintf(x, 260, cred[8]);
	Gprintf(x, 270, cred[9]);
	Gprint(0, x, 288, 1, 11, cred[10]);
	Gprint(0, x, 312, 1, 11, cred[11]);
	putimage(x + (strlen(cred[11]) + 2) * 8, 294 + y00, &PicBuf[30], 0);
	i = 0;
	setvisualpage(ActP);
	while(bioskey(1)) bioskey(0);
	while(bioskey(1) == 0)
	{
		if(i < 250)
			Gprint(0, margin2, 185, 1, 9, over);
		if(i == 4)
		{
			setcolor(3);
			Gprintf(margin1, 4, cast);
			line(margin1 - 5, 14, margin1 + width, 14);
			setcolor(11);
		}
		if(i == 6)
		{
			putimage(50, 25, &ZoomPic[22], 0);
			Gprintf(98, 33, names[(i - 6) / 3]);
		}
		if(i == 9)
		{
			putimage(50, 53, &PicBuf[1], 0);
			Gprintf(98, 61, names[(i - 6) / 3]);
		}
		if(i == 12)
		{
			putimage(50, 81, PicBufA+0x2E2, 0);
			Gprintf(98, 89, names[(i - 6) / 3]);
		}
		if(i == 15)
		{
			putimage(50, 109, PicBufA+0xF6, 0);
			Gprintf(98, 117, names[(i - 6) / 3]);
		}
		if(i == 18)
		{
			putimage(48, 137, &ZoomPic[83], 0);
			Gprintf(98, 145, names[(i - 6) / 3]);
		}
		if(i == 21)
		{
			putimage(570, 25, PicBufA+0x1EC, 0);
			Gprintf(546 - strlen(names[(i - 6) / 3]) * 8, 33, names[(i - 6) / 3]);
		}
		if(i == 24)
		{
			putimage(570, 53, PicBufA+0x3D8, 0);
			Gprintf(546 - strlen(names[(i - 6) / 3]) * 8, 61, names[(i - 6) / 3]);
		}
		if(i == 27)
		{
			putimage(564, 81, PicBufA, 0);
			Gprintf(546 - strlen(names[(i - 6) / 3]) * 8, 89, names[(i - 6) / 3]);
		}
		if(i == 30)
		{
			putimage(571, 109, PicBufA + 0x4CE, 0);
			Gprintf(546 - strlen(names[(i - 6) / 3]) * 8, 117, names[(i - 6) / 3]);
		}
		if(i == 33)
		{
			putimage(568, 137, &PicBuf[59], 0);
			Gprintf(546 - strlen(names[(i - 6) / 3]) * 8, 145, names[(i - 6) / 3]);
		}
		else if(i > 33)
			putpixel(rand() % 640, rand() % 350, colors[rand() % 3]);
		else
			delay(100);
		if(i < 250)
			Gprint(0, margin2, 185, 1, 11, over);
		if(i < 260)
			i++;
	}
	ReadKey();
	return QKeys[1];
}

void Gprint(int mode, int xloc, int yloc, int c0, int c1, char *fmt, ...)
{
	int i, j, k, l;
	va_list argptr;
	char str[80];
	char *img;
	int isz;

	if(mode == 1)
	{
		isz = imagesize(xloc, 247, xloc + 393, 318);
		img = malloc(isz);
	}

	va_start(argptr, fmt);
	l = vsprintf(str, fmt, argptr);
	va_end(argptr);

	if(mode == 1 && yloc > 310)
	{
		getimage(xloc, 247, xloc + 393, 318, img);
		putimage(xloc, 229, img, 0);
		bar(xloc, 301, xloc + 393, 318);
		yloc = 302;
	}

	for(i = 0;i < l;i++)
	{
		if(mode == 1)
		{
			putimage(603, 114, EC[i % 3], 0);
			if(i % 5 == 0)
			{
				putimage(569, 26, EE[i % 2], 0);
				putimage(595, 26, EE[i % 2], 0);
			}
		}
		for(j = 0;j < 8;j++)
		{
			if(mode == 1 && (j % 2 == 0))
			{
				bar(xloc + j - 3, yloc + 18, xloc + j - 1, yloc + 21);
				putimage(xloc + j, yloc + 10, &PicBuf[67], 0);
				delay(15);
			}
			for(k = 0;k < 16;k++)
			{
				if(Font[str[i] - ' '][k] & Mask[j])
				{
					putpixel(xloc + j + 2, yloc + k, c0);
					putpixel(xloc + j, yloc + k - 1, c1);
					if(bioskey(1) != 0 && mode == 1)
					{
						k = 16;
						j = 8;
						i = l;
					}
				}
			}
		}
		xloc += 8;
	}

	if(mode == 1)
	{
		putimage(xloc + j - 10, yloc + 10, &ZoomPic[3], 0);
		free(img);
	}
}

void Helen(void)
{
	int i, j, k;
	int yy = 2;
	int x = 117, y = 50;
	char img[] = {-1, 0, 1};
	char *pics[2], *pic;
	char *msg[] = {
		"Cgfkmysq ufhybneh KEB ^^^ - kexibq",
		"gjlfhjr lkz Dfc ctqxfc !"
	};
	Score1(12);
	putimage(480, 40 + y00, &PicBuf[12], 0);
	putimage(480, 40 + y00, &PicBuf[13], 0);
	delay(150);
	putimage(480, 40 + y00, &PicBuf[14], 0);
	delay(300);
	Napluv(20, yy, &PicBuf[1]);
	delay(100);
	putimage(480, y0 * 20 + y00, &PicBuf[19], 0);
	delay(100);
	End = Key = 0x1B;
	for(i = 0;i < 6;i++)
	{
		k = 0;
		for(j = 0;j < 10;j++)
		{
			bar(480, yy * 20 + y00 + k - 2, 503, yy * 20 + y00 + k);
			if(i + j != 0)
				bar(480, y0 * 20 + y00 - k + 20, 503, y0 * 20 + y00 - k + 21);
			putimage(480, yy * 20 + y00 + k, &PicBuf[img[j % 3] + 1], 0);
			putimage(x0 * 24, y0 * 20 + y00 - k, &PicBuf[img[j % 3] + 19], 0);
			k += 2;
			delay(80);
		}
		yy++;
		y0--;
	}
	putimage(480, 162 + y00, &PicBuf[19], 0);
	for(i = 0;i < 50;i++)
	{
		putimage(480, 160 + y00, &PicBuf[img[i % 3] + 16], 0);
		delay(60);
	}	
	putimage(480, 150 + y00, &ZoomPic[3], 0);
	putimage(480, 170 + y00, &ZoomPic[3], 0);
	if(TotalScr < 56210l || (Mony0 < SmPicMon + 1100))
	{
		Text(1, 11);
		Score1(14);
		Text(0, 43);
		goto quit;
	}
	Text(1, 44);
	if((pic = (char*)malloc(9482)) == NULL)
		goto quit;
	if((i = open("VOL3R.KRT", O_RDWR | O_BINARY)) < 0 || lseek(i, 0x12, SEEK_SET) < 0)
		goto quit;	/*TODO leaky, leaky*/
	if(ActP == 0)
		k = 1;
	else
		k = 0;
	setactivepage(k);
	if(read(i, pic, 9482) != 9482)
		goto quit;
	putimage(x, y + 103, pic, 0);
	pics[0] = (char*)malloc(114);
	pics[1] = (char*)malloc(114);
	getimage(268, 157, 285, 165, pics[0]);
	getimage(268, 167, 285, 175, pics[1]);
	setfillstyle(SOLID_FILL, 15);
	bar(268, 157, 285, 175);
	if(read(i, pic, 9482) != 9482)
		goto quit;
	putimage(x + 184, y, pic, 0);
	if(read(i, pic, 9482) != 9482)
		goto quit;
	putimage(x + 184, y + 103, pic, 0);
	if(read(i, pic, 9482) != 9482)
		goto quit;
	close(i);
	setpalette(5, 8);
	setpalette(6, 17);
	setpalette(9, 9);
	setfillstyle(SOLID_FILL, 0);
	setactivepage(ActP);
	bar(x - 4, y - 4, x + 372, y + 247);
	setcolor(9);
	rectangle(x - 2, y - 2, x + 370, y + 245);
	putimage(x, y, pic, 0);
	setactivepage(k);
	getimage(x + 184, y + 103, x + 367, y + 205, pic);
	setactivepage(ActP);
	putimage(x + 184, y + 103, pic, 0);
	setactivepage(k);
	getimage(x + 184, y, x + 367, y + 102, pic);
	setactivepage(ActP);
	putimage(x + 184, y, pic, 0);
	setactivepage(k);
	getimage(x, y + 103, x + 183, y + 205, pic);
	setactivepage(ActP);
	putimage(x, y + 103, pic, 0);
	free(pic);
	Gprint(0, x + (366 - (int)strlen(msg[0]) * 8) / 2, y + 209, 1, 11, msg[0]);
	Gprint(0, x + (366 - (int)strlen(msg[1]) * 8) / 2, y + 226, 1, 11, msg[1]);
	Score1(15);
	while(bioskey(1)) bioskey(0);
	setfillstyle(SOLID_FILL, 15);
	setcolor(15);
	j = k = 0;
	delay(2500);
	putimage(268, 157, pics[0], 0);
	putimage(268, 167, pics[1], 0);
	delay(500);
	while(bioskey(1) == 0)
	{
		if(j == 0 || j == 6 || j == 10)
		{
			for(i = 0;(i < j / 7 * 15 + 8) && bioskey(1) == 0;i++)
			{
				putimage(268, 155, pics[k], 0);
				bar(268, 164, 285, 165);
				delay(560 - j * 50);
				putimage(268, 157, pics[k], 0);
				bar(268, 155, 285, 156);
				delay(560 - j * 50);
			}
		}
		if(bioskey(1) != 0)
			break;

		if(j == 7)
		{
			for(i = 0;i < 3 && bioskey(1) == 0;i++)
			{
				putimage(266, 156, pics[k], 0);
				bar(266, 165, 285, 165);
				bar(284, 156, 285, 165);
				delay(560 - j * 50);
				putimage(268, 157, pics[k], 0);
				bar(266, 156, 285, 156);
				bar(266, 156, 267, 165);
				delay(560 - j * 50);
			}
		}
		if(j == 8)
		{
			for(i = 0;i < 3 && bioskey(1) == 0;i++)
			{
				putimage(270, 156, pics[k], 0);
				bar(268, 165, 287, 165);
				bar(268, 156, 269, 165);
				delay(560 - j * 50);
				putimage(268, 157, pics[k], 0);
				bar(268, 156, 287, 156);	/*TODO: bug?*/
				bar(286, 156, 290, 166);	/*TODO: bug?*/
				delay(560 - j * 50);
			}
		}
		if(bioskey(1) != 0)
			break;
		if(j == 1 || j == 5 || j == 9)
		{
			for(i = 0;(i < j / 6 * 3 + 5) && bioskey(1) == 0;i++)
			{
				putimage(268, 155, pics[k], 0);
				putimage(268, 167, pics[1 - k], 0);
				bar(268, 164, 285, 166);
				delay(560 - j * 50);
				putimage(268, 157, pics[k], 0);
				bar(268, 155, 285, 156);
				putimage(268, 166, pics[1 - k], 0);
				bar(268, 175, 285, 176);
				delay(560 - j * 50);
			}
		}
		if(bioskey(1) != 0)
			break;
		if(j == 2)
		{
			for(i = 0;i < 5 && bioskey(1) == 0;i++)
			{
				putimage(268, 157, pics[k], 0);
				putimage(268, 165, pics[1 - k], 0);
				bar(268, 174, 285, 176);
				delay(560 - j * 50);
				putimage(268, 157, pics[k], 0);
				bar(268, 166, 285, 167);
				putimage(268, 168, pics[1 - k], 0);
				delay(560 - j * 50);
			}
		}
		if(bioskey(1) != 0)
			break;
		if(j == 3)
		{
			for(i = 0;i < 6 && bioskey(1) == 0;i++)
			{
				putimage(268, 156, pics[k], 0);
				putimage(268, 167, pics[1 - k], 0);
				bar(268, 165, 285, 166);
				delay(560 - j * 50);
				putimage(268, 157, pics[k], 0);
				bar(268, 156, 285, 156);
				putimage(268, 166, pics[1 - k], 0);
				bar(268, 175, 285, 176);
				delay(560 - j * 50);
			}
		}
		if(bioskey(1) != 0)
			break;
		if(j == 4)
		{
			for(i = 0;i < 5 && bioskey(1) == 0;i++)
			{
				putimage(268, 156, pics[k], 0);
				putimage(268, 166, pics[1 - k], 0);
				bar(268, 165, 285, 165);
				delay(560 - j * 50);
				putimage(268, 158, pics[k], 0);
				bar(268, 156, 285, 157);
				putimage(268, 166, pics[1 - k], 0);
				delay(560 - j * 50);
			}
		}
		if(bioskey(1) != 0)
			break;
		if(j < 10)
			j++;
		else
			break;
		if(j == 2 || (j > 4 && j != 8))
		{
			for(i = 0;i < 10 && bioskey(1) == 0;i++)
			{
				putimage(268, i + 158, pics[k], 0);
				delay(15);
				putimage(268, 166 - i, pics[1 - k], 0);
				delay(15);
			}
			line(268, 166, 285, 166);
			k = 1 - k;
			if(bioskey(1) == 0)
				delay(700 - j * 50);
		}
	}
	bar(266, 155, 290, 176);
	free(pics[0]);
	free(pics[1]);
	if(bioskey(1) != 0)
	{
		Score1(12);
		Text(1, 25);
	}
quit:;
	Key = QKeys[1];
}

void Ghost(void)
{
	int i, j, k;
	int best, cnt, cdir, dir, diry;
	int iimg = 0;
	int ddir;
	int destr = 0;
	char img[] = {-1,  0,  1};
	int cell[4];
	int t2[4];
	int t1[25][16];

	Score1(12);
	Text(1, 26);
	if(KrPcX == 3 && KrPcY == 1)
		CurPic[4][4] = 3;
	for(i = 0;i < 25;i++)
	{
		for(j = 0;j < 16;j++)
		{
			if(CurPic[i][j] > 10 || CurPic[i][j] == 8
			|| i == 0 || i == 24 || j == 0 || j == 15)
				t1[i][j] = 1000;
			else if(CurPic[i][j] == 6 || CurPic[i][j] == 9 || CurPic[i][j] == 10)
				t1[i][j] = 0;
			else
				t1[i][j] = 5;
		}
	}
	i = CurPic[25][10];
	j = CurPic[25][11];
	srand((int)time(NULL));
	CurPic[25][10] = CurPic[25][11] = 0x1B;
	putimage(i * 24, j * 20 + y00, &PicBuf[58], 0);
	while(destr < 8 && CurPic[25][0] > 0)
	{
		t1[i][j]++;
		cell[2] = t1[i + 1][j];
		cell[1] = t1[i - 1][j];
		cell[3] = t1[i][j + 1];
		cell[0] = t1[i][j - 1];
		for(k = 0;k < 4;k++)
			t2[k] = 0;
		best = 1000;
		for(k = 0;k < 4;)
		{
			dir = rand() % 4;
			if(t2[dir] == 0)
			{
				t2[dir] = 1;
				k++;
				if(cell[dir] <= best)
				{
					best = cell[dir];
					cdir = dir;
				}
			}
		}
		if(cdir < 2)
			ddir = -1;
		else
			ddir = 1;
		if(cdir == 0 || cdir == 3)
		{
			dir = 0;
			diry = 1;
		}
		else
		{
			diry = 0;
			dir = 1;
		}
		if(i == x0 && j == y0)
			iimg = 3;
		if(CurPic[25][1] == i && CurPic[25][2] == j && BonEat[KrPcX][KrPcY] > 0)
		{
			BonEat[KrPcX][KrPcY] = 0;
			CurPic[25][1] = 27;
		}
		if(CurPic[25][5] == i && CurPic[25][6] == j && BonKey[KrPcX][KrPcY] == 1)
		{
			BonKey[KrPcX][KrPcY] = 0;
			CurPic[25][5] = 27;
		}
		if(CurPic[i + dir * ddir][j + diry * ddir] == 9)
		{
			Score(1, -50);
			CurPic[i + dir * ddir][j + diry * ddir] = 3;
			if(CurPic[25][0] > 0)
				CurPic[25][0]--;
			destr++;
		}
		if(CurPic[i + dir * ddir][j + diry * ddir] == 10)
		{
			Score(1, -25);
			CurPic[i + dir * ddir][j + diry * ddir] = 3;
			destr++;
		}
		if(CurPic[i + dir * ddir][j + diry * ddir] == 6)
		{
			if(CurPic[25][0] > 0)
				CurPic[25][0]--;
			Score(1, -20);
			destr++;
		}
		CurPic[i + dir * ddir][j + diry * ddir] = 3;
		if(dir > 0)
			cnt = 6;
		else
			cnt = 5;
		cdir = 0;
		for(best = 0;best < cnt * 2;best++)
		{
			if(best % 2 == 0)
			{
				putimage(i * 24, j * 20 + y00, &ZoomPic[3], 0);
				if(dir > 0)
				{
					cdir += ddir * 4;
					putimage(i * 24 + cdir, j * 20 + y00, PicBuf + 34 + (img[best % 3] + 24), 0);
				}
				else
				{
					cdir += ddir * 4;
					putimage(i * 24, j * 20 + y00 + cdir, PicBuf + 34 + (img[best % 3] + 24), 0);
				}
				delay(20);
			}
			putimage(x0 * 24, y0 * 20 + y00, PicBuf + 34 + (img[best % 3] + iimg + 27), 0);
		}
		i += dir * ddir;
		j += diry * ddir;
	}
	putimage(i * 24, j * 20 + y00, &ZoomPic[3], 0);
	if(iimg == 3)
	{
		putimage(x0 * 24, y0 * 20 + y00, &PicBuf[66], 0);
		delay(400);
	}
	putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[33], 0);
	while(bioskey(1)) bioskey(0);
	Score1(10);
}

void Time(int t)
{
	int color = 12;
	long left;
	pTime = CrTime - MyTime;
	PicTime[KrPcX][KrPcY] -= t;
	if(PicTime[KrPcX][KrPcY] < 0 && CurPic[25][10] != 0x1B)
	{
		Palet(7, 63);
		Ghost();
		Key = 0;
	}

	left = PicTime[KrPcX][KrPcY] * 599l;
	left /= 500l + PicCost[KrPcX][KrPcY] + PicCost[KrPcX][KrPcY] + PicCost[KrPcX][KrPcY];

	if(CurPic[25][10] == 0x1B)
	{
		bar(xCr[18] - 9, 341, xCr[18] - 1, 349);
		color = 11;
		CurPic[25][11] = 0x1B;
	}
	else
		setfillstyle(1, 7);

	bar(0, 333, (int)left - 1, 335);
	setfillstyle(1, color);
	bar((int)left, 333, 599, 335);
	setfillstyle(1, 0);
}

void Money(int m)
{
	int x = 0;
	Mony += m;
	if(m > 0)
		Mony0 += m;
	bar(600, y00 + 273, 639, y00 + 282);
	if(Mony != 0)
	{
		putimage(605, y00 + 250, &ZoomPic[16], 0);
		if(Mony < 0)
			x = 4;
		else if(Mony < 10)
			x = 16;
		else if(Mony < 100)
			x = 12;
		else if(Mony < 1000)
			x = 6;
		setcolor(10);
		Gprintf(600 + x, y00 + 273, "%d", Mony);
	}
	else
		putimage(605, y00 + 250, &ZoomPic[3], 0);
}

void Palet(int index, int color)
{
	int i;
	setpalette(index, 0);
	for(i = 0;i < 15;i++)
	{
		setpalette(index, color);
		delay(30);
		setpalette(index, CrPal[index]);
		delay(30);
	}
	setpalette(index, index);
}

void Figa(void)
{
	int i;
	char img[] = {21, 39, 28, 33};
	Score1(12);
	Text(1, 10);
	putimage(x0 * 24, y0 * 20 + y00, &PicBuf[45], 0);
	delay(150);
	Napluv(20, 5, &ZoomPic[45]);
	for(i = 0;i < 100;i++)
	{
		putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[img[i % 4]], 0);
		putimage(480, 100 + y00, &ZoomPic[i % 2 + 44], 0);
		delay(50);
	}
	for(i = 0;i < 4;i++)
	{
		putimage(x0 * 24, y0 * 20 + y00, &PicBuf[i + 26], 0);
		delay(100);
	}
	for(i = 0;i < 5 && DSlp > 35;)
	{
		Slp += (130 - DSlp) / 5 / 3 + 1;
		DSlp -= 5;
		i++;
		Score1(0);
	}
	delay(300);
	Napluv(20, 5, &ZoomPic[3]);
	CurPic[21][5] = 14;
	Napluv(21, 5, &ZoomPic[14]);
	delay(100);
	BnKey++;
	Gate(0, 2, 0, 21, 5);	
	putimage(x0 * 24, y0 * 20 + y00, &PicBuf[27], 0);
	delay(100);
	putimage(x0 * 24, y0 * 20 + y00, &PicBuf[50], 0);
	delay(100);
	putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[21], 0);
	Sur[4] = 0;
	while(bioskey(1)) bioskey(0);
	Score1(10);
};

void Dush(void)
{
	int i;
	char img[] = {-1,  0,  1};
	Score1(12);
	Text(1, 39);
	Score(0, 100);
	Show = 1;
	OutMove(27, -1, 0);
	Show = 0;
	putimage(x0 * 24, (y0 - 1) * 20 + y00, &ZoomPic[46], 0);
	putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[19], 0);
	delay(400);
	putimage(x0 * 24, y0 * 20 + y00, &PicBuf[66], 0);
	delay(400);
	i = 0;
	Score1(15);
	while(bioskey(1)) bioskey(0);
	while(i < 100 && bioskey(1) == 0)
	{
		putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[img[i % 3] + 48], 0);
		i++;
		delay(100);
		if(time(&CrTime) - MyTime != pTime)
			if(CurPic[25][11] != 27)
				Time(2);		
	}
	putimage(x0 * 24, (y0 - 1) * 20 + y00, &ZoomPic[18], 0);
	putimage(x0 * 24, y0 * 20 + y00, &PicBuf[66], 0);
	delay(400);
	putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[19], 0);
	Sur[1] = 0;
	while(bioskey(1)) bioskey(0);
	Score1(10);
};
