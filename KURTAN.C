#include "kurtan.h"
#include <stdlib.h>

int xCr[20] = {0, -4, -3, -4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int QKeys[] = { 'Y', 'y', 'Д'&0xFF, 'д'&0xFF,
				'N', 'n', 'Н'&0xFF, 'н'&0xFF };               

char *Dll = "$";

char *Msg[] = {
	"Тонус",
	"Лабиринт",
	"Очки",
	"Помощь",
	"Остановить",
	"Управление скоростью",
	"Помощь",
	"Карта",
	"Файл",
	"Звук",
	"Hачальник идет!",
	"Выход",
	"Сохранить",
	"Загрузить",
	"Перезапустить",
	"Отдохнуть",
	"Д/H",
	"Hов.файл",
	"Время"};

char Show = 0;
char Mask[8] = {128, 64, 32, 16, 8, 4, 2, 1};
int Key = 0;
int ActP = 0;
int End = 1;
int Sl = 41;
int y00 = 10;

int x0, y0;
void far *EC[3];
int xX0;
void far *EE[2];
int yY0;
int BBB[6][6];
int Mony0;
int BBC[6][6];
int Moves0;
int KolD;
int fhVOL1;
int SmPicH[6][6][25];
unsigned char PicBufA[4920];
char Ord;
int KrPcX, KrPcY;
char Name[9];
char Zaj;
int TmpKRT;
unsigned long Leng;
int SmPicV[6][6][16];
int Slp;
char DFree[8];
char Dance;
char Sur[20];
int Scr[6][6];
int DSlp;
char CurPic[26][16];
char BonCl[72];
char CrPal[16];
unsigned char Font[94][16];
time_t CrTime;
char Delete;
char Dinam;
char Inst;
long MyTime;
long pTime;
char NepMn;
ZOOMPIC PicBuf[68];
int Mony;
long chet;
char Loter;
int Moves;
unsigned char OutZmPc[280];
int PicTime[6][6];
ZOOMPIC ZoomPic[90];
char BnKey;
int PicMon[6][6];
int BonSz[36];
char PcHelp[6][6];
int BonEat[6][6];
char BonKey[6][6];
int PicMov[6][6];
char PicHelp[6][6];
int PicCost[6][6];
unsigned int SmPicMon;
long TotalScr;

void main(void)
{
	int dx, dy;
	int answer;
	time_t timer, ttt;
	int m, ii;

	fhVOL1 = open("VOL1R.KRT", O_RDWR | O_BINARY);
	if(fhVOL1 < 0)
	{
		printf("Отсутствует или неверный файл VOL1E.KRT");
		bioskey(0);
		return;
	}
	lseek(fhVOL1, 0x1B, SEEK_SET);
	read(fhVOL1, &Leng, sizeof(Leng));

	if((ii = open("KURTANR.EXE", O_RDWR | O_BINARY)) < 0)
	{
	}
	else if(lseek(ii, 0, SEEK_END) != Leng)
	{
		printf("\n\rПрограмма заражена вирусом !!! Продолжить ? (%s)", Msg[MSG_YN]);
		answer = getch();
		if(answer != QKeys[0] && answer != QKeys[1] && answer != QKeys[2] && answer != QKeys[3])
			return;
		close(ii);
	}

	if(Load() < 0
	|| Init() < 0
	|| Present() < 0
	|| PicOut(KrPcX, KrPcY) < 0)
		goto end;

	Score1(10);
	putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[19], 0);
	while(Key != QKeys[1] && End != 27)
	{
		time(&timer);
		for(ttt = timer;!bioskey(1);time(&ttt))
		{
			if(time(&CrTime) - MyTime != pTime)
				if(CurPic[25][11] != 27)
					Time(2);
			if((ttt - timer > 5) && (ttt - timer < 20))
				putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[19], 0);
			if(ttt - timer > 20)
			{
				Smoke(x0, y0);
				break;
			}
			if(End == 27)
			{
				Key = 0;
				goto part2;
			}
		}
		Key = ReadKey();
part2:;
		switch(Key)
		{
		case 0x14D:
			m = 21;
			dx = 1;
			dy = 0;
			goto move;
		case 0x14B:
			m = 27;
			dx = -1;
			dy = 0;
			goto move;
		case 0x150:
			m = 33;
			dx = 0;
			dy = 1;
			goto move;
		case 0x148:
			m = 39;
			dx = 0;
			dy = -1;
move:;
			if(OutMove(m, dx, dy) < 0)
				goto end;
			break;
		case 0x153:
			DelMove(m);
			break;
		case 0x110:
			Score1(11);
			setcolor(12);
			Gprintf(xCr[12] + 49, 0, "%s ? (%s)", Msg[MSG_Exit], Msg[MSG_YN]);
			while(Key != QKeys[0] && Key != QKeys[1] && Key != QKeys[2] && Key != QKeys[3]
			&&    Key != QKeys[4] && Key != QKeys[5] && Key != QKeys[6] && Key != QKeys[7])
			{
				if(time(&CrTime) - MyTime != pTime)
					if(CurPic[25][11] != 27)
						Time(2);
				Key = ReadKey();
			}
			if(Key == QKeys[0] || Key == QKeys[1] || Key == QKeys[2] || Key == QKeys[3])
			{
				putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[3], 0);
				Key = QKeys[1];
			}
			else
			{
				bar(0, 0, 639, 9);
				Score1(10);
			}
			break;
		case 0x1B:
			Menu1(NepMn);
			goto part2;
		case 0x13D:
			Menu2(NepMn);
			goto part2;
		case 0x13B:
			HelpSh(KrPcX, KrPcY);
			if(!Show)
				break;
			/* fall */
		case 0x143:
			BossKey();
			break;
		case 0x13C:
			Zoom();
			break;
		case 0x13E:
		case 0x13F:
			if(File() != 27)
				break;
			/* fall */
		case 0x140:
			Score1(12);
			if(Load() < 0
			|| PicOut(KrPcX, KrPcY) < 0)
				goto end;
			Money(0);
			putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[19], 0);
			Score1(14);
			Text(0, 7);
			Score1(10);
			break;
		default:
			Key = 0;
			break;
		}
		if(KrPcX == 5 && KrPcY == 2 && (x0 < 10) & (y0 < 7) && !Dance)	/*TODO: bug, missing?*/
		{
			Dance = 1;
			Sur[16] = 1;
			putimage(24, 120 + y00, PicBufA+0x4CE, 0);
			putimage(144, 20 + y00, PicBufA+0xF6, 0);
			putimage(168, 20 + y00, PicBufA+0x1EC, 0);
			putimage(72, 120 + y00, PicBufA+0x2E2, 0);
			putimage(144, 80 + y00, PicBufA+0x3D8, 0);
			Score1(12);
			Text(1, 29);
			Text(1, 30);
			putimage(24, 120 + y00, &ZoomPic[1], 0);
			putimage(144, 20 + y00, &ZoomPic[1], 0);
			putimage(168, 20 + y00, &ZoomPic[1], 0);
			putimage(72, 120 + y00, &ZoomPic[1], 0);
			putimage(144, 80 + y00, &ZoomPic[1], 0);
			while(bioskey(1)) bioskey(0);
			Score1(10);
		}
		if(Ord == 3 && TotalScr > 40000l)
			Orden();
		if(Loter == 85 && TotalScr > 25000l && KrPcX > 1 && KrPcY != 3)
			Lotr(1);
		if(CurPic[25][0] == 0)
		{
			if(CurPic[25][8] == 0x1B)
				CurPic[25][10] = 0x1B;
			if(CurPic[25][7] != 0x1B)
				Trunk(CurPic[25][7], CurPic[25][8], 1);
		}
		if(BonEat[KrPcX][KrPcY] > 0)
		{
			if(PicMov[KrPcX][KrPcY] - BonEat[KrPcX][KrPcY] > 32)
			{
				Score1(12);
				putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[19], 0);
				Text(1, 47);
				BonEat[KrPcX][KrPcY] = 0;
				Napluv(CurPic[25][1], CurPic[25][2], &ZoomPic[3]);
				CurPic[25][1] = 0x1B;
				for(ii = 0;ii < 10;ii++)
				{
					putimage(x0 * 24, y0 * 20 + y00, &PicBuf[3], 0);
					delay(70);
					putimage(x0 * 24, y0 * 20 + y00, &PicBuf[4], 0);
					delay(70);
				}
				putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[19], 0);
				while(bioskey(1)) bioskey(0);
				Score1(10);
			}
		}
		if(KrPcX == 5 && KrPcY == 4 && Scr[5][4] == 250 && CurPic[20][13] == 18)
		{
			Score1(12);
			putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[21], 0);
			Napluv(20, 13, &ZoomPic[14]);
			CurPic[20][13] = 14;
			Score1(10);
		}
		if(CurPic[25][1] != 27 && BonEat[KrPcX][KrPcY] == 0 && PicMov[KrPcX][KrPcY] >= BonSz[CurPic[25][9]]
		&& CurPic[CurPic[25][1]][CurPic[25][2]] < 5 && (CurPic[25][1] != x0 || CurPic[25][2] != y0))
		{
			setpalette(5, BonCl[CurPic[25][9]]);
			setpalette(6, BonCl[36 + CurPic[25][9]]);
			BonEat[KrPcX][KrPcY] = PicMov[KrPcX][KrPcY];
			putimage(CurPic[25][1] * 24, CurPic[25][2] * 20 + y00, &ZoomPic[54 + CurPic[25][9]], 0);
		}
		if(CurPic[25][0] <= 2 && CurPic[25][5] != 27 && CurPic[CurPic[25][5]][CurPic[25][6]] <= 8
		&& CurPic[25][5] != x0 && CurPic[25][6] != y0)
		{
			BonKey[KrPcX][KrPcY] = 1;
			putimage(CurPic[25][5] * 24, CurPic[25][6] * 20 + y00, &ZoomPic[53], 0);
		}
	}
	Key = GameOver();
	if(Key != QKeys[1])
		goto part2;


end:;
	close(TmpKRT);
	close(fhVOL1);
	closegraph();
}

void Text(int a0, int a2)
{
	int x;
	int ii = 0;
	int jj = 18;
	char *msgs[] = {
		"Ltytu lkz gjvjob ytljcnfnjxyj",
		"Yt kexit kb Dfv jnlj[yenm d N/bkbcb <!",
		"   $ gthtxbcktyj yf cxtn N/bkbCjan",
		"Buhf yt cj[hfybkfcm",
		"Ytn cj[hfytyys[ buh d ntreotq lbhtrnjhbb",
		"Buhf cj[hfybkfcm",
		"Buhf pfuhepbkfcm",
		"Buhf gthtpfgeotyf",
		"Tot hfp <!",
		"Ve[jvjh - yt kexifz gbof !",
		"|nj Dfit cgfctybt !",
		"Yfib gjplhfdktybz ! Ds buhfkb jnkbxyj,",
		"Ds ytljcnjqys 'nb[ ltytu gjckt gjvjob",
		"Jnkbxyj, Ds dpzkb    $ ",
		"?tlyzuf, jy nfr k`/bk cdjb[ 12 ltntq !",
		"Jn lehyjq ujkjds yjufv gjrjz ytn",
		"F hs/rf nj /skf c leirjv !",
		"Rehtybt dhtlbn pljhjdm` !",
		"Pltcm lkz Dfc kjnthtqysq /bktn",
		"C`hghbp cnjbn 25$, f e Dfc b[ ytn !",
		"Ds dsbuhfkb vfibye ! Tt vj;yj bcgjkmpjdfnm gjckt buhs",
		"Gjlj;lbnt, gj;fkeqcnf",
		"Pltcm 5 rk`xtq lkz Dfc !",
		"|nb 500$ - Dfib, djpmvbnt b[ !",
		"Ds djpyfuhf;ltys pf nfrjt ecthlbt",
		"Gjvjom pltcm ytljcnegyf !",
		"Yt /jqntcm ghbdbltybq ! Jyb recf`ncz, yj yt jxtym /jkmyj",
		"|njn ce/]rn jn/hjcbk Dfc vtnhjd yf 15 ...",
		"Dfc yfuhflbkb Jhltyjv Ytenjvbvs[ Yjcbkmobrjd !",
		"Pltcm nfywgkjoflrf, yj yt lkz Dfc !",
		"Pfyzkbcm /s Ds cdjbv ltkjv !",
		"Njkmrj lkz Dfc @199 GJKTPYS{ CJDTNJD RELF B RFR NJKRFNM@",
		"F djn 'nj /skf Dfif jib/rf !",
		"Jy pf/hfk Dfib wtyyjcnb !",
		"Jlby vjq pyfrjvsq nthhjhbcn nj;t edktrfkcz lbyfvbnfvb ...",
		"Njkmrj pf;bufkrb yt [dfnfkj !",
		"Bycnherwbz gj j/hfotyb` c lbyfvbnjv !",
		"C nfrjq htrjvtylfwbtq j/hfofqntcm d N/bkbCjan d k`/jt dhtvz !",
		"Yfrjytw-nj ljlevfkbcm dpjhdfnm cntye !",
		"Xnj vj;tn /snm kexit ghj[kflyjuj leif ...",
		"?thtv lbyfvbn, gjl;buftv ...",
		"Lbyfvbn - jgfcyfz inerf d ytevtks[ herf[ ! ",
		"Jib/rb d afqkt !",
		"yj 'nj yt ghtltk !!!",
		"Dfif buhf - dtkbrjktgyf !!! Ds pfcke;bkb ...",
		"C Dfc dpzkb inhfa 100$ pf ... d j/otcndtyyjv vtcnt",
		"Nfr Ds tot d ljkue e vtyz !",
		"Dfi ghbp ghjgfk yfdctulf !",
		"F rk`xbr nj Dfi ghjgfk !",
		"Drk`xbnt Dfie cnthtj ecnfyjdre b ljdjkmcndeqntcm vfksv !",
		"Hfpdt ctqxfc dhtvz vtyznm jlt;le <",
		"Djpdhfn [jlf cnjbn 50$, rjnjhs[ e Dfc ytn !",
		"|nj ytdjpvj;yj ... "
	};
	char *img;
	char s[20];
	int l;
	int b;
	int isz;
	char colors[2][2] = {{4, 12}, {1, 11}};

	while(bioskey(1) != 0)
		bioskey(0);

	if(a2 == 0 || a2 == 3 || a2 == 4 || a2 == 9 || a2 == 21 || a2 == 42)
		b = 0;
	else
		b = 1;

	l = strlen(msgs[a2]);
	x = (580 - l * 8) / 2;
	isz = imagesize(x, 161, x + l * 8 + 21, 192);
	img = malloc(isz);
	getimage(x, 161, x + l * 8 + 21, 192, img);
	bar(x, 161, x + l * 8 + 21, 192);
	setcolor(colors[b][0]);
	rectangle(x + 5, 161 + 5, x + l * 8 + 21 - 2, 192 - 2);
	setcolor(colors[b][1]);
	rectangle(x + 3, 161 + 3, x + l * 8 + 21 - 4, 192 - 4);

	if(a2 == 2)
	{
		itoa(PicMon[KrPcX][KrPcY], s, 10);
		msgs[2][ii] = s[0];
		msgs[2][ii+1] = s[1];
		msgs[2][ii+2] = s[2];
	}
	else if(a2 == 13)
	{
		itoa(PicMon[KrPcX][KrPcY], s, 10);
		msgs[13][jj] = s[0];
		msgs[13][jj+1] = s[1];
		msgs[13][jj+2] = s[2];
	}

	Gprint(0, x + 11, 171, colors[b][0], colors[b][1], msgs[a2]);

	if(a2 != 21)
	{
		l = 0;
		if(a0 == 0)
		{
			while(bioskey(1) == 0 && l == 0)
			{
				if((time(&CrTime) - MyTime) != pTime && CurPic[25][11] != 0x1B)
					Time(2);
				if(PicTime[KrPcX][KrPcY] < 4 && PicTime[KrPcX][KrPcY] > 0)
					l = 1;
			}
		}
		else
			delay(2000);
		putimage(x, 161, img, 0);
	}

	free(img);
}

int OutMove(int m, int dx, int dy)
{
	int key = 0;
	int stop = 0;
	int v = 0;
	int ddx = 0, ddy = 0;
	int r, q, i, flag, mm, w, xb, t, s, flg, u;
	int tt[4] = {1,  0, -1,  0};
	if(m == 0)
	{
		ddx = dx;
		ddy = dy;
	}
	while(key == 0 && stop == 0)
	{
		Delete = 0;
		if(CurPic[x0 + dx][y0 + dy] == 16)
		{
			Trunk(x0 + dx, y0 + dy, 0);
			return 0;
		}
		if((time(&CrTime) - MyTime) != pTime && CurPic[25][11] != 0x1B)
		{
			Time(3);
			if(CurPic[25][11] == 0x1B && Show == 0)
				return 0;
		}
		if(KrPcX == 3 && KrPcY == 3 && (x0 + dx) == 18 && y0 == 2 && Dinam == 1 && Zaj == 1)
		{
			Vzriv();
			return 0;
		}
		if((x0 + dx) > 24 || (x0 + dx) < 0 || (y0 + dy) > 15 || (y0 + dy) < 0)
		{
			putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[3], 0);
			Score1(12);
			Comp(0, KrPcX, KrPcY);
			x0 = x0 - dx * 24;
			y0 = y0 - dy * 15;
			lseek(TmpKRT, (KrPcX * 6 + KrPcY) * 26*16, SEEK_SET);
			write(TmpKRT, CurPic, 26*16);
			KrPcX += dx;
			KrPcY += dy;
			if(PicOut(KrPcX, KrPcY) < 0)
				return -1;
			putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[m], 0);
			Score1(10);
			return 0;			
		}
		flg = 0;
		if(CurPic[x0 + dx][y0 + dy] == 8)
		{
			if(Mony < 25)
			{
				Score1(12);
				Text(1, 19);
				for(i = 0;i < 5;i++)
				{
					putimage(x0 * 24, y0 * 20 + y00, &PicBuf[3], 0);
					delay(50);
					putimage(x0 * 24, y0 * 20 + y00, &PicBuf[4], 0);
					delay(50);
				}
				putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[19], 0);
				Score1(10);
				return 0;
			}
			else
			{
				Money(-25);
				CurPic[x0 + dx][y0 + dy] = 1;
				if(KrPcX == 2 && KrPcY == 0)
					Grab();
				else if(KrPcX == 4 && KrPcY == 3)
					Figa();
				else if(KrPcX == 1 && KrPcY == 5)
					Book();
				else if(KrPcX == 3 && KrPcY == 2)
					Recom();
				else if(KrPcX == 3 && KrPcY == 5)
					Clot();
				else if(KrPcX == 0 && KrPcY == 3)
					Lotr(0);
				else if(KrPcX == 2 && KrPcY == 3)
					Dush();
				else if(KrPcX == 5 && KrPcY == 0)
				{
					Score1(12);
					Napluv(2, 7, PicBufA+0x5C4);
					Show = 1;
					OutMove(27, -1, 0);
					Show = 0;
					putimage(604, 165 + y00, PicBufA+0x5C4, 0);
					putimage(x0 * 24, y0 * 20 + y00, &PicBuf[11], 0);
					Score1(14);
					Text(0, 35);
					Score(0, 250);
					Zaj = 1;
					Sur[13] = 0;
					Score1(10);
				}
				else if(KrPcX == 5 && KrPcY == 2)
				{
					Score1(12);
					Napluv(2, 13, PicBufA+0xA92);
					Show = 1;
					OutMove(27, -1, 0);
					Show = 0;
					putimage(604, 107 + y00, PicBufA+0xA92, 0);
					putimage(x0 * 24, y0 * 20 + y00, &PicBuf[3], 0);
					Score1(14);
					Text(0, 36);
					Score(0, 500);
					Inst = 1;
					Sur[12] = 0;
					Score1(10);
				}
				else if(KrPcX == 3 && KrPcY == 1)
				{
					Show = 1;
					OutMove(27, -1, 0);
					Sur[2] = Show = 0;
					putimage(x0 * 24, y0 * 20 + y00, &PicBuf[45], 0);
					PicTime[3][1] = -1;
					Time(0);
				}
				else if(KrPcX == 0 && KrPcY == 1)
				{
					Score1(12);
					Napluv(x0 + dx, y0, &ZoomPic[53]);
					Text(1, 22);
					Score(0, 100);
					Show = 1;
					OutMove(21, 1, 0);
					Show = 0;
					BnKey += 5;
					BKey();
					putimage(x0 * 24, y0 * 20 + y00, &PicBuf[30], 0);
					Sur[9] = 0;
					Score1(10);
				}
				else if(KrPcX == 0 && KrPcY == 4)
				{
					Score1(12);
					Napluv(10, 5, &ZoomPic[84]);
					Text(1, 23);
					Score(0, 100);
					Show = 1;
					OutMove(21, 1, 0);
					Sur[0] = Show = 0;
					putimage(x0 * 24, y0 * 20 + y00, &PicBuf[5], 0);
					Money(500);
					Score1(10);
				}
				else if(KrPcX == 0 && KrPcY == 0)
				{
					Score1(12);
					Napluv(21, 4, PicBufA+0x6BA);
					Show = 1;
					OutMove(33, 0, 1);
					Show = 0;
					putimage(604, 136 + y00, PicBufA+0x6BA, 0);
					putimage(x0 * 24, y0 * 20 + y00, &PicBuf[45], 0);
					Score1(14);
					Text(0, 34);
					Score(0, 250);
					Dinam = 1;
					Sur[11] = 0;
					Score1(10);
				}
				else if(KrPcX == 1 && KrPcY == 0)
				{
					Show = 1;
					OutMove(39, 0, -1);
					Show = 0;
					delay(100);
					putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[27], 0);
					delay(100);
					putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[33], 0);
					Score1(14);
					Text(0, 15);
					Score(0, 100);
					Sur[6] = 0;
					Score1(10);
				}
				else if(KrPcX == 4 && KrPcY == 2)
				{
					Score1(12);
					Napluv(21, 2, &ZoomPic[83]);
					Text(1, 27);
					putimage(504, 40 + y00, PicBufA+0x114C, 0);
					delay(500);
					putimage(504, 60 + y00, &ZoomPic[3], 0);
					if(Show == 0)
					{
						Score(0, 100);
						for(i = 14;i < 21;i++)
						{
							for(key = 5;key < 7;key++)
							{
								if(CurPic[i][key] < 5)
								{
									x0 = i;
									y0 = key;
								}
							}
						}
					}
					else
					{
						x0 = 16;
						y0 = 6;
					}
					for(i = 0;i < 4;i++)
					{
						putimage(x0 * 24, y0 * 20 + y00, &PicBuf[26 + i], 0);
						delay(100);
					}
					delay(500);
					putimage(504, 40 + y00, &ZoomPic[83], 0);
					delay(250);
					Sur[3] = 0;
					Napluv(21, 2, &ZoomPic[3]);
					if(Show == 0)
						Score1(10);
					else
						Score1(15);
				}
				return 0;
			}
		}
		if(CurPic[x0 + dx][y0 + dy] == 15)
		{
			Score1(12);
			Steps(dy);
			while((y0 + dy) > 15 || (y0 + dy) < 0)
			{
				Comp(0, KrPcX, KrPcY);
				lseek(TmpKRT, (KrPcX * 6 + KrPcY) * 26*16, SEEK_SET);
				write(TmpKRT, CurPic, 26*16);
				KrPcY += dy;
				y0 = y0 - dy * 15;
				if(PicOut(KrPcX, KrPcY) < 0)
					return -1;
				Score1(12);
				Steps(dy);
				yY0 = y0;
			}
			if(Show == 0)
				Score1(10);
			else
				Score1(15);
			return 0;
		}
		if(CurPic[x0 + dx][y0 + dy] < 8)
		{
			flag = 1;
			mm = m;
			r = dx * 3;
			q = dy * 2;
			w = 15;
			if((x0 + dx == CurPic[25][5]) && (y0 + dy == CurPic[25][6]) && (BonKey[KrPcX][KrPcY] == 1))
				BonKey[KrPcX][KrPcY] = 0x1B;
			if((x0 + dx == CurPic[25][1]) && (y0 + dy == CurPic[25][2]) && (BonEat[KrPcX][KrPcY] > 0))
				flg = 0x1B;
		}
		else if((CurPic[x0 + dx][y0 + dy] >= 9) && (CurPic[x0 + dx][y0 + dy] <= 12)
		&& (x0 + dx + dx < 24) && (x0 + dx + dx > 0) && (y0 + dy + dy < 15) && (y0 + dy + dy > 0)
		&& (CurPic[x0 + dx + dx][y0 + dy + dy] <= 8))
		{
			Delete = 1;
			if(CurPic[x0 + dx][y0 + dy] >= 11)
				s = 2;
			else
				s = 0;
			if(s != 0)
			{
				if(CurPic[x0 + dx + dx][y0 + dy + dy] == 6)
				{
					CurPic[25][7] = x0 + dx + dx;
					CurPic[25][8] = y0 + dy + dy;
				}
				else
					CurPic[25][7] = 27;
			}
			if((x0 + dx + dx == CurPic[25][1]) && (y0 + dy + dy == CurPic[25][2]) && (BonEat[KrPcX][KrPcY] > 0))
				flg = 0x38;
			if((x0 + dx + dx == CurPic[25][5]) && (y0 + dy + dy == CurPic[25][6]) && (BonKey[KrPcX][KrPcY] == 1))
				BonKey[KrPcX][KrPcY] = 0x2D;
			flag = 1;
			mm = m + 3;
			r = dx * 3;
			q = dy * 2;
			if(dy == 0)
				w = 20;
			else
				w = 16;
			if(dx > 0 && DSlp == 45)
			{
				m = 48;
				mm = 51;
				w = 70;
			}
			if(CurPic[x0 + dx + dx][y0 + dy + dy] == 6)
				xb = s + 10;
			else
				xb = s + 9;
		}
		else if((CurPic[x0 + dx][y0 + dy] == 14) && ((BnKey > 0) || (v > 200)))
		{
			if(v > 200)
			{
				BnKey++;
				Score1(14);
				Text(0, 24);
				Score1(10);
			}
			putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[m], 0);
			flag = 1;
			mm = m;
			r = dx * 3;
			q = dy * 2;
			w = 15;
			Gate(0, dx, dx + dy, x0 + dx, y0 + dy);
			CurPic[x0 + dx][y0 + dy] = 1;
			if(Show == 0)
				return 0;
		}
		else
		{
			flag = 0;
			mm = m + 3;
			r = q = 0;
			if(dy == 0)
				w = 45;
			else
				w = 36;
			if(CurPic[x0 + dx][y0 + dy] == 14)
				v++;
			else
				v = 0;
		}
		if(dy == 0)
			u = 8;
		else
			u = 10;
		for(i = 0;i < u;i++)
		{
			if(flag > 0)
			{
				if(dy == 0)
				{
					if(dx > 0)
						bar((x0 + ddx) * 24 + r - 3, (y0 + ddy) * 20 + y00, (x0 + ddx) * 24 + r - 3 + 3, (y0 + ddy) * 20 + y00 + 19);
					else
						bar((x0 + ddx) * 24 + r + 24, (y0 + ddy) * 20 + y00, (x0 + ddx) * 24 + r + 24 + 2, (y0 + ddy) * 20 + y00 + 19);
				}
				else if(dy > 0)
					bar((x0 + ddx) * 24, (y0 + ddy) * 20 + y00 + q - 2, (x0 + ddx) * 24 + 23, (y0 + ddy) * 20 + y00 + q - 2 + 2);
				else
					bar((x0 + ddx) * 24, (y0 + ddy) * 20 + y00 + q + 20, (x0 + ddx) * 24 + 23, (y0 + ddy) * 20 + y00 + q + 20 + 1);
			}
			if(m != 0)
				putimage(x0 * 24 + r, y0 * 20 + y00 + q, &ZoomPic[mm + tt[chet % 4]], 0);
			if(flag == 1 && (m + 3 == mm))
			{
				t = xb;
				if((xb == s + 10) && (CurPic[x0 + dx][y0 + dy] != s + 10))
				{
					if((dy == 0 && i > 4) || (dx == 0 && i > 5))
						t = s + 10;
					else
						t = s + 9;
					if(i == 0)
					{
						Score(1, 20);
						CurPic[25][0]--;
					}
				}
				else if((xb == s + 9) && (CurPic[x0 + dx][y0 + dy] != s + 9))
				{
					if((dy == 0 && i > 4) || (dx == 0 && i > 5))
						t = s + 9;
					else
						t = s + 10;
					if(i == 0)
					{
						Score(1, -20);
						CurPic[25][0]++;
					}
				}
				putimage((x0 + dx) * 24 + r, (y0 + dy) * 20 + y00 + q, &ZoomPic[t], 0);
			}

			if(flag > 0)
			{
				r += dx * 3;
				q += dy * 2;
			}
			if(m == 0
			&& ((CurPic[x0 + ddx][y0 + ddy] == 10) || (CurPic[x0 + ddx][y0 + ddy] == 12))
			&& ((dy == 0 && i == 5) || (dx == 0 && i == 9)))
				putimage((x0 + ddx) * 24 + 10, (y0 + ddy) * 20 + y00 + 9, OutZmPc+0x54, 0);
			else if((CurPic[x0][y0] == 6) && ((dy == 0 && i == 5) || (dx == 0 && i == 9)))
				putimage(x0 * 24 + 10, y0 * 20 + y00 + 9, OutZmPc+0x54, 0);
			if(Show == 0 || m == 0)
				delay(Slp + w);
			else
				delay(Sl);
			chet++;
		}
		PicMov[KrPcX][KrPcY]++;
		Moves++;
		if((Moves - Moves0) % DSlp == 0)
		{
			Slp += (130 - DSlp) / 5 / 3 + 1;
			DSlp -= 5;
			Moves0 = Moves;
			Score1(0);
			if(End == 0x1B)
			{
				if(flag != 0)
				{
					x0 += dx;
					y0 += dy;
				}
				return 0;
			}
		}
		if(flag > 0)
		{
			x0 += dx;
			y0 += dy;
		}

		if((PicMov[KrPcX][KrPcY] - KolD > 5) && CurPic[25][3] != 0x1B && (CurPic[25][3] != x0 || CurPic[25][4] != y0))
		{
			Gate(1, CurPic[25][12], dx + dy, CurPic[25][3], CurPic[25][4]);
			if(Show == 0)
				stop = 1;
		}
		if(flag == 1 && (m + 3 == mm))
		{
			CurPic[x0 + dx][y0 + dy] = xb;
			if(CurPic[x0][y0] == s + 10)
				CurPic[x0][y0] = 6;
			else
				CurPic[x0][y0] = 3;
		}
		if(flg == 0x38)
		{
			Score1(12);
			putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[19], 0);
			Text(1, 47);
			BonEat[KrPcX][KrPcY] = 0;
			CurPic[25][1] = 0x1B;
			for(i = 0;i < 10;i++)
			{
				putimage(x0 * 24, y0 * 20 + y00, &PicBuf[3], 0);
				delay(70);				
				putimage(x0 * 24, y0 * 20 + y00, &PicBuf[4], 0);
				delay(70);				
			}
			putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[19], 0);
			while(bioskey(1) != 0) bioskey(0);
			Score1(10);
			return 0;
		}
		if(BonKey[KrPcX][KrPcY] == 0x2D)
		{
			Score1(12);
			putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[19], 0);
			Text(1, 48);
			BonKey[KrPcX][KrPcY] = 0;
			CurPic[25][5] = 0x1B;
			for(i = 0;i < 10;i++)
			{
				putimage(x0 * 24, y0 * 20 + y00, &PicBuf[3], 0);
				delay(70);				
				putimage(x0 * 24, y0 * 20 + y00, &PicBuf[4], 0);
				delay(70);				
			}
			putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[19], 0);
			while(bioskey(1) != 0) bioskey(0);
			Score1(10);
			return 0;
		}
		if(flg == 0x1B)
		{
			Eat(CurPic[25][1], CurPic[25][2]);
			return 0;
		}
		if(BonKey[KrPcX][KrPcY] == 0x1B)
		{
			BnKey++;
			BKey();
			Score(1, 50);
			BonKey[KrPcX][KrPcY] = 0;
			CurPic[25][5] = 0x1B;
			CurPic[25][6] = 0x38;
			putimage(x0 * 24, y0 * 20 + y00, &PicBuf[30], 0);
			return 0;
		}
		if(Show == 1)
			return 0;
		key = bioskey(1);
	}
	if((Key == 0x14D && key == 0x4D00)
	|| (Key == 0x14B && key == 0x4B00)
	|| (Key == 0x148 && key == 0x4800)
	|| (Key == 0x150 && key == 0x5000))
		while(bioskey(1) != 0) bioskey(0);

	/*return 0; TODO: no return value?*/
}

void DelMove(int m)
{
	Score1(14);
	if(Delete == 0)
	{
		Text(0, 52);
		goto fin;
	}
	else if(Mony < 50)
	{
		Text(0, 51);
fin:;
		Score1(10);
		return;
	}
	Money(-50);
	Show = 1;
	Score1(12);
	switch(m)
	{
	case 21:
		OutMove(27, -1, 0);
		putimage(x0 * 24, y0 * 20 + y00, &PicBuf[45], 0);
		x0 += 3;
		OutMove(0, -1, 0);
		x0 -= 2;
		putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[23], 0);
		break;
	case 27:
		OutMove(21, 1, 0);
		putimage(x0 * 24, y0 * 20 + y00, &PicBuf[45], 0);
		x0 -= 3;
		OutMove(0, 1, 0);
		x0 += 2;
		putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[30], 0);
		break;
	case 33:
		OutMove(39, 0, -1);
		putimage(x0 * 24, y0 * 20 + y00, &PicBuf[45], 0);
		y0 += 3;
		OutMove(0, 0, -1);
		y0 -= 2;
		putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[36], 0);
		break;
	case 39:
		OutMove(33, 0, 1);
		putimage(x0 * 24, y0 * 20 + y00, &PicBuf[45], 0);
		y0 -= 3;
		OutMove(0, 0, 1);
		y0 += 2;
		putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[41], 0);
		break;
	}
	Delete = Show = 0;
	Score1(10);
}

void Smoke(int x, int y)
{
	int key;
	int i, j, k = 0;
	char t1[8] = {0, 1, 2, 3, 4, 5, 2, 1};
	int  t2[8] = {4, 4, 2, 1, 1, 1, 6, 1}; 
	Score1(14);
	putimage(x * 24, y * 20 + y00, &PicBuf[41], 0);
	delay(320);
	putimage(x * 24, y * 20 + y00, &PicBuf[42], 0);
	delay(320);
	putimage(x * 24, y * 20 + y00, &PicBuf[36], 0);
	delay(100);
	key = bioskey(1);
	while(key == 0)
	{
		if((time(&CrTime) - MyTime != pTime) && (CurPic[25][11] != 0x1B))
		{
			Time(1);
			if(CurPic[25][11] == 0x1B)
			{
				delay(100);
				return;
			}
		}
		for(i = 0;i < 8 && key == 0;i++)
		{
			for(j = 0;j < t2[i] * 10 && key == 0;j++)
			{
				putimage(x * 24, y * 20 + y00, PicBuf + 34 + t1[i], 0);
				key = bioskey(1);
				t2[0] = 12;
				delay(5);
			}
		}
		k++;
		if(Show && k > 3)
			key = 1;
		Moves++;
		if((Moves - Moves0) % DSlp == 0)
		{
			Slp += (130 - DSlp) / 5 / 3 + 1;
			DSlp -= 5;
			Moves0 = Moves;
			Score1(0);
			if(End == 0x1B)
				return;
		}
	}
	putimage(x * 24, y * 20 + y00, &PicBuf[42], 0);
	delay(320);
	putimage(x * 24, y * 20 + y00, &PicBuf[40], 0);
	Score1(10);	
}

void Vzriv(void)
{
	int i;
	Score1(12);
	Text(1, 38);
	putimage(604, 107 + y00, &ZoomPic[3], 0);
	Show = 1;
	putimage(604, 136 + y00, &ZoomPic[3], 0);
	putimage(421, 40 + y00, PicBufA+0x7B0, 0);
	putimage(397, 40 + y00, PicBufA+0xF60, 0);
	Text(1, 40);
	putimage(604, 165 + y00, &ZoomPic[1], 0);
	putimage(421, 40 + y00, PicBufA+0xB88, 0);
	putimage(397, 40 + y00, &ZoomPic[19], 0);
	delay(100);
	putimage(397, 40 + y00, &ZoomPic[1], 0);
	x0--;
	if(Inst == 1)
	{
		OutMove(27, -1, 0);
		putimage(421, 40 + y00, PicBufA+0xC7E, 0);
		delay(20);
		OutMove(27, -1, 0);
		putimage(421, 40 + y00, PicBufA+0xD74, 0);
		delay(20);
		putimage(x0 * 24, y0 * 20 + y00, PicBufA+0xE6A, 0);
	}
	else
	{
		putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[21], 0);
		putimage(421, 40 + y00, PicBufA+0xC7E, 0);
		delay(20);
		putimage(421, 40 + y00, PicBufA+0xD74, 0);
		delay(20);
	}
	Zaj = Dinam = 2;
	delay(450);
	putimage(426, 40 + y00, PicBufA+0x1056, 0);
	delay(450);
	CurPic[18][2] = 1;
	putimage(408, 40 + y00, &ZoomPic[3], 0);
	putimage(432, 40 + y00, &ZoomPic[3], 0);
	putimage(432, 20 + y00, PicBufA+0x8A6, 0);
	putimage(432, 60 + y00, PicBufA+0x99C, 0);
	Show = 0;
	if(Inst == 1)
	{
		delay(800);
		putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[19], 0);
		Score(0, 1000);
	}
	else
	{
		Text(1, 41);
		for(i = 0;i < 2;i++)
		{
			putimage(x0 * 24, y0 * 20 + y00, &PicBuf[3], 0);
			delay(30);
			putimage(x0 * 24, y0 * 20 + y00, &PicBuf[4], 0);
			delay(30);
		}
		DSlp = 35;
		End = 27;
		return;
	}
	Inst = 0;
	while(bioskey(1)) bioskey(0);
	Score1(10);
};

void Gate(int t1, int t2, int t3, int x, int y)
{
	int i, j = 0, q, r;
	Score1(12);
	if(t1 == 0)
	{
		putimage(x * 24, y * 20 + y00, &ZoomPic[13], 0);
		BnKey--;
		BKey();
		delay(300);
		CurPic[25][3] = x;
		CurPic[25][4] = y;
		CurPic[25][12] = t2;
		KolD = PicMov[KrPcX][KrPcY];
		if(t2 == 0)
		{
			for(i = 2;i <= 24;i++)
			{
				bar(x * 24 + i - 2, y * 20 + y00, x * 24 + i, y * 20 + y00 + 19);
				putimage(x * 24 + i, y * 20 + y00, &ZoomPic[13], 0);
				delay(30);
			}
			CurPic[x][y] = 1;
			CurPic[x + 1][y] = 13;
		}
		else
		{
			for(i = 2;i <= 20;i++)
			{
				bar(x * 24, y * 20 + y00 + 20 - i + 2, x * 24 + 23, y * 20 + y00 + 20 - i);
				putimage(x * 24, y * 20 + y00 - i, &ZoomPic[13], 0);
				delay(30);
			}
			CurPic[x][y] = 1;
			CurPic[x][y - 1] = 13;
		}
		if(KrPcX == 5 && KrPcY == 4)
		{
			Helen();
			return;
		}
		CurPic[25][6] = 27;
	}
	else
	{
		CurPic[25][3] = 27;
		if(t2 != 0)
		{
			if(x0 > x && t3 > 0)
			{
				q = 39;
				r = 27;
				j = -6;
			}
			else if(x0 < x && t3 < 0)
			{
				q = 33;
				r = 21;
				j = 6;
			}
		}
		else
		{
			if(y0 > y && t3 > 0)
			{
				q = 27;
				r = 39;
				j = -6;
			}
			else if(y0 < y && t3 < 0)
			{
				q = 21;
				r = 33;
				j = 6;
			}
		}
		if(j != 0)
		{
			putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[q], 0);
			delay(120);
			putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[r], 0);
		}
		if(t2 == 0)
		{
			for(i = 24;i >= 0;i--)
			{
				putimage((x + 1) * 24, y * 20 + y00, &ZoomPic[18], 0);
				putimage(x * 24 + i, y * 20 + y00, &ZoomPic[13], 0);
				delay(30);
			}
			CurPic[x][y] = 14;
			CurPic[x+1][y] = 18;
		}
		else
		{
			for(i = 20;i >= 0;i--)
			{
				putimage(x * 24, (y - 1) * 20 + y00, &ZoomPic[18], 0);
				putimage(x * 24, y * 20 + y00 - i, &ZoomPic[13], 0);
				delay(30);
			}
			CurPic[x][y] = 14;
			CurPic[x][y-1] = 18;
		}
		if(j != 0)
		{
			putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[q + j], 0);
			delay(120);
			putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[r + j], 0);
		}
		putimage(x * 24, y * 20 + y00, &ZoomPic[14], 0);
	}
	while(bioskey(1)) bioskey(0);
	if(Show == 0)
		Score1(10);
	else
		Score1(15);
};

void Eat(int x, int y)
{
	int i = 0, j;
	int q = 0, r = 1;
	char img[] = {21, 22, 23, 24, 25, 22, 21};
	BonEat[KrPcX][KrPcY] = 0;
	j = CurPic[25][9];
	CurPic[25][1] = 27;
	if(j == 34)
	{
		BonSz[34] = 0;
		Show = 1;
		Smoke(x0, y0);
		Show = 0;
		for(;i < 5 && DSlp > 35;i++)
		{
			Slp += (130 - DSlp) / 5 / 3 + 1;
			DSlp -= 5;
		}
		Score1(0);
		while(bioskey(1)) i = ReadKey();
		Score1(14);
		Text(0, 17);
		Score1(10);
	}
	else
	{
		Score1(15);
		while(i == 0 && q < 15)
		{
			putimage(x * 24, y * 20 + y00, &PicBuf[44], 0);
			delay(100);
			putimage(x * 24, y * 20 + y00, PicBuf + 34 + (r + 10), 0);
			delay(100);
			r *= -1;
			i = bioskey(1);
			q++;
			if(time(&CrTime) - MyTime != pTime)
				if(CurPic[25][11] != 27)
					Time(2);						
		}
		if(j == 9)
		{
			BonSz[9] = 0;
			Score1(12);
			Text(1, 16);
			putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[21], 0);
			delay(100);
			putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[39], 0);
			delay(350);
			for(i = 0;i < 7;i++)
			{
				putimage(x0 * 24, y0 * 20 + y00, &PicBuf[img[i]], 0);
				delay(350);
			}
			putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[39], 0);
			delay(200);
			putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[27], 0);
			delay(100);
			putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[19], 0);
			while(bioskey(1)) i = ReadKey();
			Money(-100);
			Score1(14);
			Text(0, 45);
			if(Mony < 0)
				Text(0, 46);
			Score1(10);
		}
		if(j == 33)
		{
			Score1(12);
			BonSz[33] = 0;
			Slp = 90;
			DSlp = 35;
			Score1(0);
			End = 27;
			Score1(12);
			Text(1, 9);
			return;
		}
		if(j != 9)
			putimage(x0 * 24, y0 * 20 + y00, &PicBuf[30], 0);
		Score(1, BonSz[j]);
		Slp = 20;
		DSlp = 130;
		Moves = Moves0 = 0;
		Score1(0);
		if(Show == 0)
			Score1(10);
		else
			Score1(15);
	}
};

void Steps(int n)
{
	int i, dy;
	int y = y0;
	if(y0 > 0 && n > 0)
	{
		putimage(x0 * 24, y0 * 20 + y00, &PicBuf[50], 0);
		delay(250);
		putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[3], 0);
		putimage(x0 * 24, y0 * 20 + y00 + 10, &PicBuf[51], 0);
		delay(250);
		putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[3], 0);
		y0 += n;
		PicMov[KrPcX][KrPcY]++;
		Moves++;
		Key = 20;
	}
	else if(y0 < 15 && n < 0)
	{
		putimage(x0 * 24, y0 * 20 + y00, &PicBuf[54], 0);
		delay(400);
		putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[3], 0);
		y0 += n;
		PicMov[KrPcX][KrPcY]++;
		Moves++;
		Key = 50;
	}
	while(CurPic[x0][y0+n] == 15)
	{
		dy = 0;
		for(i = 0;i < 2;i++)
		{
			if(n < 0)
			{
				dy += n * 5;
				putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[15], 0);
			}
			putimage(x0 * 24, y0 * 20 + y00 + dy, &PicBuf[48], 0);
			delay(Slp + Key);
			if(n > 0)
			{
				dy += n * 5;
				putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[15], 0);
			}
			putimage(x0 * 24, y0 * 20 + y00 + dy, &PicBuf[49], 0);
			delay(Slp + Key);
			if(n < 0)
			{
				dy += n * 5;
				putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[15], 0);
			}
			putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[15], 0);
			putimage(x0 * 24, y0 * 20 + y00 + dy, &PicBuf[47], 0);
			delay(Slp + Key);
			if(n > 0)
			{
				dy += n * 5;
				putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[15], 0);
			}
			putimage(x0 * 24, y0 * 20 + y00 + dy, &PicBuf[46], 0);
			delay(Slp + Key);
		}
		y0 += n;
		PicMov[KrPcX][KrPcY]++;
		Moves++;
		if(time(&CrTime) - MyTime != pTime)
			if(CurPic[25][11] != 27)
				Time(2);
		if(y0 < 1 || y0 > 14)
		{
			putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[15], 0);
			if(abs(y0 - y) != 15)
			{
				CurPic[25][1] = CurPic[25][5] = 0x1B;
			}
			if(Show == 0)
				while(bioskey(1)) bioskey(0);
			return;
		}
	}
	if(y0 < 15 && n > 0)
	{
		putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[15], 0);
		for(i = 0;i < 8;i++)
		{
			putimage(x0 * 24, y0 * 20 + y00 + 10, PicBuf + 34 + (i % 2 + 21), 0);
			delay(100);			
		}
		for(i = 0;i < 6;i++)
		{
			putimage(x0 * 24, y0 * 20 + y00 + 10, PicBuf + 34 + (i % 2 + 18), 0);
			delay(80);			
		}
		putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[15], 0);
		putimage(x0 * 24, (y0 + 1) * 20 + y00, &ZoomPic[3], 0);
		y0 += n;
		PicMov[KrPcX][KrPcY]++;
		Moves++;
		if(time(&CrTime) - MyTime != pTime)
			if(CurPic[25][11] != 27)
				Time(2);
		putimage(x0 * 24, y0 * 20 + y00, &PicBuf[54], 0);
		delay(200);
		putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[39], 0);
		delay(120);
		putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[27], 0);
		delay(120);
		putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[33], 0);
		delay(250);
	}
	else if(y0 > 0 && n < 0)
	{
		putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[15], 0);
		putimage(x0 * 24, y0 * 20 + y00 - 10, &PicBuf[51], 0);
		delay(250);
		putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[15], 0);
		y0 += n;
		PicMov[KrPcX][KrPcY]++;
		Moves++;
		putimage(x0 * 24, y0 * 20 + y00, &PicBuf[50], 0);
		delay(250);
		putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[21], 0);
		delay(250);
	}
	if(Show == 0)
		while(bioskey(1)) bioskey(0);
};

void Grab(void)
{
	int i;
	char img[] = {-1,  0,  1};
	Score1(12);
	Show = 1;
	OutMove(27, -1, 0);
	Show = 0;
	putimage(x0 * 24, y0 * 20 + y00, &PicBuf[45], 0);
	Text(1, 32);
	for(i = 1;i < 24;i++)
	{
		putimage((x0 + 2) * 24 - i, y0 * 20 + y00, PicBufA + 0, 0);
		putimage((x0 + 2) * 24, y0 * 20 + y00, &ZoomPic[18], 0);
		delay(20);
	}
	for(i = 0;i < 40;i++)
	{
		putimage(x0 * 24, y0 * 20 + y00, PicBuf + 34 + (img[i % 3] + 30), 0);
		delay(50);
	}
	Ord = 1;
	Inst = Zaj = BnKey = Mony = 0;
	putimage(604, 107 + y00, &ZoomPic[Ord], 0);
	putimage(604, 165 + y00, &ZoomPic[Ord], 0);
	putimage(604, 194 + y00, &ZoomPic[Ord], 0);
	if(Loter == 85)
	{
		Loter = 1;
		putimage(605, 223 + y00, &ZoomPic[Ord], 0);
	}
	Money(0);
	BKey();
	setcolor(12);
	Gprintf(xCr[6], 341, "%d%s", PicCost[KrPcX][KrPcY], Dll);
	Napluv(x0 + 1, y0, &ZoomPic[3]);
	putimage(x0 * 24, y0 * 20 + y00, &PicBuf[45], 0);
	Score1(14);
	Text(0, 33);
	Score1(10);
	Sur[8] = 0;
};

void Trunk(int x, int y, int z)
{
	int i, q;
	char img[] = {-1,  0,  1};
	Score1(12);
	Delete = 0;
	if(z == 1)
	{
		putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[19], 0);
		Napluv(x, y, &ZoomPic[16]);
		if(CurPic[x + 1][y] > 7 && CurPic[x - 1][y] > 7
		&& CurPic[x][y + 1] > 7 && CurPic[x][y - 1] > 7)
			q = 3;
		else
			q = 9;
		for(i = 0;i < 10;i++)
		{
			putimage(x0 * 24, y0 * 20 + y00, &PicBuf[q], 0);
			delay(70);
			putimage(x0 * 24, y0 * 20 + y00, &PicBuf[q + 1], 0);
			delay(70);
		}
		if(q == 3)
			putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[19], 0);
		else
			putimage(x0 * 24, y0 * 20 + y00, &PicBuf[11], 0);
		CurPic[25][7] = 0x1B;
		CurPic[x][y] = 16;
		setpalette(10, CrPal[10]);
		if(Show != 0)
			Score1(14);
		else
		{
			while(bioskey(1)) bioskey(0);
			Score1(10);
		}
	}
	else
	{
		delay(100);
		putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[3], 0);
		for(i = 0;i < 20;i++)
		{
			putimage(x * 24, y * 20 + y00, &PicBuf[img[i % 3] + 7], 0);
			delay(70);
		}
		if(KrPcX == 5 && KrPcY == 1 && y0 < 5)
		{
			y0++;
			CurPic[x0][y0] = 3;
		}
		else
		{
			CurPic[x][y] = 17;
			putimage(x * 24, y * 20 + y00, &ZoomPic[17], 0);
		}
		putimage(x0 * 24, y0 * 20 + y00, &PicBuf[5], 0);
		CurPic[25][8] = 0x1B;
		CurPic[25][10] = 0x1B;
		Time(0);		
		while(bioskey(1)) bioskey(0);
		Score1(12);
		if(Show == 0)
		{
			Score1(14);
			if(KrPcX == 5 && KrPcY == 1 && y0 < 5)
			{
				i = PicMon[5][1];
				PicMon[5][1] = 600;
				Money(600);
				Text(0, 13);
				Score(0, 100);
				Sur[15] = 0;
				PicMon[5][1] = i;
			}
			else if(PicHelp[KrPcX][KrPcY] == 0 && PcHelp[KrPcX][KrPcY] == 0)
			{
				Text(0, 13);
				if(KrPcX == 5 && KrPcY == 1)
				{
					CurPic[9][3] = 3;
					putimage(216, 60 + y00, &ZoomPic[3], 0);
				}
				Money(PicMon[KrPcX][KrPcY]);
				Score(1, 1000);
			}
			else
				Text(0, 12);
			Score1(10);
		}
		else
		{
			Score1(12);
			Text(Show, 2);
			Score1(15);
		}
	}
};

void Zoom(void)
{
	int x = 96;
	int y = 80;
	int i, j, ii, jj, dx, dy;
	int page;
	char buf[26][16];
	char *pic;
	int psz;
	char *hlp[] = {
		"К а р т а    лабиринтов",
		"Hажмите любую клавишу для продолжения игры"
	};
	psz = imagesize(206, 161, 395, 192);
	pic = malloc(psz);
	getimage(206, 161, 395, 192, pic);
	Score1(12);
	Text(0, 21);
	CurPic[x0][y0] = 19;
	if(ActP == 0)
		page = 1;
	else
		page = 0;
	setactivepage(page);
	cleardevice();
	setfillstyle(1, 0);
	setcolor(12);
	Gprintf(x - 22 + (476 - strlen(hlp[0]) * 8) / 2 , y - 31, hlp[0]);
	setcolor(11);
	rectangle(x - 22, y - 35, x + 453, y + 194);
	Gprintf(x - 22, y + 205, hlp[1]);
	line(x - 22, y - 20, x + 453, y - 20);
	for(i = 0;i < 6;i++)
	{
		if(i == KrPcX)
			setcolor(11);
		else
			setcolor(7);
		Gprintf(i * 75 + x + 35, y - 12, "%c", 'A' + i);
		for(j = 0;j < 6;j++)
		{
			if(j == KrPcY)
				setcolor(11);
			else
				setcolor(7);
			Gprintf(x - 15, j * 32 + y + 12, "%d", j + 1);
			if(PicMov[i][j] >= 0 || (i == KrPcX && j == KrPcY))
			{
				if(i == KrPcX && j == KrPcY)
					memcpy(buf, CurPic, 26*16);
				else
				{
					lseek(TmpKRT, (i * 6 + j) * 26*16, SEEK_SET);
					read(TmpKRT, buf, 26*16);
				}
				for(ii = 0;ii < 25;ii++)
				{
					dx = i * 75 + x;
					for(jj = 0;jj < 16;jj++)
					{
						dy = j * 32 + y;
						putimage(ii * 3 + dx, jj * 2 + dy, OutZmPc + buf[ii][jj] * 14, 0);
					}
				}
			}
		}
	}
	CurPic[x0][y0] = 3;
	setvisualpage(page);
	while(bioskey(1) == 0)
	{
		putimage(x0 * 3 + KrPcX * 75 + x, y0 * 2 + KrPcY * 32 + y, OutZmPc + 0x2A, 0);
		delay(100);
		putimage(x0 * 3 + KrPcX * 75 + x, y0 * 2 + KrPcY * 32 + y, OutZmPc + 0x10A, 0);
		delay(100);
	}
	Key = ReadKey();
	Key = 0;
	setactivepage(ActP);
	putimage(206, 161, pic, 0);
	free(pic);
	Score1(10);
	setvisualpage(ActP);
};

void Orden(void)
{
	char *pic1, *pic2;
	int h;
	int x = 237;
	int y = 20;
	char img[] = {11, 30, 31, 32, 31, 11};
	Score1(12);
	while(bioskey(1)) h = ReadKey();
	h = open("VOL0R.KRT", O_RDWR | O_BINARY);
	lseek(h, 0x1B5AAl, SEEK_SET);
	pic1 = malloc(9730);
	pic2 = malloc(9730);
	read(h, pic2, 9730);
	setpalette(5, 56);
	setpalette(6, 6);
	getimage(x, y, x + 128, y + 142, pic1);
	putimage(x, y, pic2, 0);
	Score1(14);
	Text(0, 28);
	Score1(12);
	Ord = 81;
	putimage(604, 194 + y00, &ZoomPic[Ord], 0);
	putimage(x, y, pic1, 0);
	free(pic1);
	free(pic2);
	close(h);
	for(x = 0;x < 6;x++)
	{
		putimage(x0 * 24, y0 * 20 + y00, &PicBuf[img[x]], 0);
		delay(300);
	}
	while(bioskey(1)) bioskey(0);
	Score1(10);
};

void Book(void)
{
	char *pic1, *pic2;
	int h;
	int x = 250;
	int y = 40;
	char img[] = {11, 30, 31, 32, 31, 11};
	Score1(12);
	Show = 1;
	OutMove(27, -1, 0);
	Show = 0;
	while(bioskey(1)) h = ReadKey();
	setpalette(5, 56);
	setpalette(6, 6);
	pic1 = malloc(6402);
	pic2 = malloc(6402);
	h = open("VOL0R.KRT", O_RDWR | O_BINARY);
	lseek(h, 0x22B32l, SEEK_SET);
	read(h, pic2, 6402);
	getimage(x, y, x + 102, y + 122, pic1);
	putimage(x, y, pic2, 0);
	Score1(14);
	Text(0, 31);
	Score(0, 100);
	Score1(12);
	putimage(x, y, pic1, 0);
	free(pic1);
	free(pic2);
	close(h);
	for(x = 0;x < 6;x++)
	{
		putimage(x0 * 24, y0 * 20 + y00, &PicBuf[img[x]], 0);
		delay(300);
	}
	while(bioskey(1)) bioskey(0);
	Score1(10);
	Sur[10] = 0;	
};

void Recom(void)
{
	char *pic1, *pic2;
	int h;
	int x = 258;
	int y = 190;
	char img[] = {11, 30, 31, 32, 31, 11};
	Score1(12);
	Show = 1;
	OutMove(21, 1, 0);
	Show = 0;
	delay(100);
	putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[19], 0);
	setpalette(5, 56);
	setpalette(6, 6);
	pic1 = malloc(5238);
	pic2 = malloc(5238);
	h = open("VOL0R.KRT", O_RDWR | O_BINARY);
	lseek(h, 0x24434l, SEEK_SET);
	read(h, pic2, 5238);
	getimage(x, y, x + 93, y + 108, pic1);
	putimage(x, y, pic2, 0);
	Score1(14);
	Text(0, 37);
	Score(0, 100);
	Score1(12);
	putimage(x, y, pic1, 0);
	free(pic1);
	free(pic2);
	close(h);
	for(x = 0;x < 6;x++)
	{
		putimage(x0 * 24, y0 * 20 + y00, &PicBuf[img[x]], 0);
		delay(300);
	}
	Sur[14] = 0;	
	while(bioskey(1)) bioskey(0);
	Score1(10);
};

void Lotr(int z)
{
	char *pic1, *pic2;
	int h;
	int i;
	int x = 175;
	int y = 4;
	Score1(12);
	if(z == 0)
	{
		Napluv(16, 14, &ZoomPic[85]);
		Text(1, 18);
		Score(0, 100);
		Show = 1;
		OutMove(33, 0, 1);
		Show = 0;
		putimage(x0 * 24, y0 * 20 + y00, &PicBuf[11], 0);
		Loter = 85;
		putimage(605, 223 + y00, &ZoomPic[Loter], 0);
		Sur[7] = 0;
		while(bioskey(1)) bioskey(0);
		Score1(10);
		return;
	}

	h = open("VOL0R.KRT", O_RDWR | O_BINARY);
	lseek(h, 0x1DBACl, SEEK_SET);
	pic1 = malloc(20358);
	pic2 = malloc(20358);
	read(h, pic2, 20358);
	setpalette(5, 56);
	getimage(x, y, x + 248, y + 158, pic1);
	putimage(x, y, pic2, 0);
	Score1(14);
	Text(0, 20);
	Score1(12);
	Loter = 86;
	putimage(605, 223 + y00, &ZoomPic[Loter], 0);
	putimage(x, y, pic1, 0);
	free(pic1);
	free(pic2);
	close(h);
	for(i = 0;i < 10;i++)
	{
		putimage(x0 * 24, y0 * 20 + y00, &PicBuf[9], 0);
		delay(70);
		putimage(x0 * 24, y0 * 20 + y00, &PicBuf[10], 0);
		delay(70);
	}
	putimage(x0 * 24, y0 * 20 + y00, &PicBuf[11], 0);
	while(bioskey(1)) bioskey(0);
	Score1(10);
};

void Clot(void)
{
	int i;
	char img[] = {11, 30, 31, 32, 31, 11};
	Score1(12);
	Show = 1;
	Text(1, 50);
	Score(0, 100);
	setpalette(6, BonCl[71]);
	Napluv(14, 6, &ZoomPic[89]);
	OutMove(21, 1, 0);
	Show = 0;
	setpalette(9, BonCl[71]);
	CrPal[9] = BonCl[71];
	for(i = 0;i < 6;i++)
	{
		putimage(x0 * 24, y0 * 20 + y00, &PicBuf[img[i]], 0);
		delay(300);
	}
	Sur[5] = 0;
	Score1(10);
};

void Napluv(int x, int y, void *pic)
{
	int j = 0;
	int page, dx, dy, p;
	int i;
	char tt[480];
	for(i = 0;i < 480;i++)
		tt[i] = 0;
	if(ActP == 0)
		page = 1;
	else
		page = 0;	
	setactivepage(page);
	putimage(x * 24, y * 20 + y00, pic, 0);
	srand((int)time(NULL));
	while(j < 480)
	{
		i = rand() % 480;
		if(tt[i] != 0)
			continue;
		tt[i]++;
		dx = i % 24;
		dy = i / 24;
		j++;
		setactivepage(page);
		p = getpixel(x * 24 + dx, y * 20 + y00 + dy);
		setactivepage(ActP);
		putpixel(x * 24 + dx, y * 20 + y00 + dy, p);
		delay(2);
	}
	delay(400);
};

void Menu1(int z)
{
	Score1(11);
	bar(0, 0, 639, 9);
	setcolor(11);
	Gprintf(0, 0, "F1");
	Gprintf(xCr[8], 0, "F2");
	Gprintf(xCr[9], 0, "F3");
	Gprintf(xCr[10], 0, "F8");
	Gprintf(xCr[11], 0, "F9");
	Gprintf(xCr[12], 0, "Alt+Q");
	setcolor(14);
	Gprintf(17, 0, "-%s", Msg[MSG_Help2]);
	Gprintf(17 + xCr[8], 0, "-%s", Msg[MSG_Map]);
	Gprintf(17 + xCr[9], 0, "-%s", Msg[MSG_File]);
	Gprintf(17 + xCr[10], 0, "-%s", Msg[MSG_Sound]);
	Gprintf(17 + xCr[11], 0, "-%s", Msg[MSG_Boss]);
	Gprintf(41 + xCr[12], 0, "-%s", Msg[MSG_Exit]);
	while(bioskey(1) == 0)
	{
		if(time(&CrTime) - MyTime != pTime)
			if(CurPic[25][11] != 27)
				Time(2);
	}
	Key = ReadKey();
	if(z == 0)
	{
		bar(0, 0, 639, 9);
		Score1(10);
		return;
	}
	switch(Key)
	{
	case 0x110:
		bar(xCr[12], 0, xCr[12] + 49, 9);
		return;
	case 0x13D:
		Menu2(1);
		break;
	case 0x13B:
	case 0x13C:
	case 0x13E:
	case 0x13F:
	case 0x140:
	case 0x143:
	case 0x148:
	case 0x14B:
	case 0x14D:
	case 0x150:
		break;
	case 0x142:
		Score1(14);
		Text(0, 49);
		break;
	default:
		Key = 0;
	}
	bar(0, 0, 639, 9);
	Score1(10);
};

void Menu2(int z)
{
	Score1(11);
	bar(0, 0, 639, 9);
	setcolor(11);
	Gprintf(0, 0, "F4");
	Gprintf(xCr[13], 0, "F5");
	Gprintf(xCr[14], 0, "F6");
	Gprintf(xCr[15], 0, "F7");
	setcolor(14);
	Gprintf(17, 0, "-%s", Msg[MSG_Save]);
	Gprintf(17 + xCr[13], 0, "-%s", Msg[MSG_Load]);
	Gprintf(17 + xCr[14], 0, "-%s", Msg[MSG_Restart]);
	Gprintf(17 + xCr[15], 0, "-%s", Msg[MSG_Pause]);
	while(bioskey(1) == 0)
	{
		if(time(&CrTime) - MyTime != pTime)
			if(CurPic[25][11] != 27)
				Time(2);
	}
	Key = ReadKey();
	if(z == 0)
	{
		bar(0, 0, 639, 9);
		Score1(10);
		return;
	}
	switch(Key)
	{
	case 0x141:
		Score1(14);
		Text(0, 1);
		break;
	case 0x110:
	case 0x13B:
	case 0x13C:
	case 0x13E:
	case 0x13F:
	case 0x140:
	case 0x143:
	case 0x148:
	case 0x14B:
	case 0x14D:
	case 0x150:
		break;
	default:
		Key = 0;
	}
	bar(0, 0, 639, 9);
	Score1(10);
};

void BossKey(void)
{
	int i;
	int GraphDriver = EGA;
	int GraphMode = EGAHI;
	Comp(0, KrPcX, KrPcY);
	lseek(TmpKRT, (KrPcX * 6 + KrPcY) * 26*16, SEEK_SET);
	write(TmpKRT, CurPic, 26*16);
	restorecrtmode();
	Key = PCTools();
	initgraph(&GraphDriver, &GraphMode, "");
	for(i = 0;i < 16;i++)
		setpalette(i, CrPal[i]);
	setfillstyle(1, 0);
	Show = ActP = 0;
	if(Key == QKeys[1])
		return;
	if(PicOut(KrPcX, KrPcY) < 0)
		Key = QKeys[1];
	putimage(x0 * 24, y0 * 20 + y00, &ZoomPic[19], 0);
	Score1(10);
};
