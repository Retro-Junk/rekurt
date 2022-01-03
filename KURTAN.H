#ifndef _KURTAN_H_
#define _KURTAN_H_

#include <stdio.h>
#include <io.h>
#include <dos.h>
#include <fcntl.h>
#include <bios.h>
#include <conio.h>
#include <graphics.h>
#include <time.h>
#include <string.h>

enum {
	MSG_Tonus,
	MSG_Labirint,
	MSG_Score,
	MSG_Help,
	MSG_Stop,
	MSG_Speed,
	MSG_Help2,
	MSG_Map,
	MSG_File,
	MSG_Sound,
	MSG_Boss,
	MSG_Exit,
	MSG_Save,
	MSG_Load,
	MSG_Restart,
	MSG_Pause,
	MSG_YN,
	MSG_NewFile,
	MSG_Time
};

typedef char ZOOMPIC[246];

typedef char map_t[6][6][26][16];


extern int xCr[20];
extern int QKeys[8];
extern char *Dll;
extern char *Msg[19];
extern char Show;
extern char Mask[8];
extern int Key;
extern int ActP;
extern int End;
extern int Sl;
extern int y00;

extern int x0, y0;
extern void far *EC[3];
extern int xX0;
extern void far *EE[2];
extern int yY0;
extern int BBB[6][6];
extern int Mony0;
extern int BBC[6][6];
extern int Moves0;
extern int KolD;
extern int fhVOL1;
extern int SmPicH[6][6][25];
extern unsigned char PicBufA[4920];
extern char Ord;
extern int KrPcX, KrPcY;
extern char Name[9];
extern char Zaj;
extern int TmpKRT;
extern unsigned long Leng;
extern int SmPicV[6][6][16];
extern int Slp;
extern char DFree[8];
extern char Dance;
extern char Sur[20];
extern int Scr[6][6];
extern int DSlp;
extern char CurPic[26][16];
extern char BonCl[72];
extern char CrPal[16];
extern unsigned char Font[94][16];
extern time_t CrTime;
extern char Delete;
extern char Dinam;
extern char Inst;
extern long MyTime;
extern long pTime;
extern char NepMn;
extern ZOOMPIC PicBuf[68];
extern int Mony;
extern long chet;
extern char Loter;
extern int Moves;
extern unsigned char OutZmPc[280];
extern int PicTime[6][6];;
extern ZOOMPIC ZoomPic[90];
extern char BnKey;
extern int PicMon[6][6];
extern int BonSz[36];
extern char PcHelp[6][6];
extern int BonEat[6][6];
extern char BonKey[6][6];
extern int PicMov[6][6];
extern char PicHelp[6][6];
extern int PicCost[6][6];
extern unsigned int SmPicMon;
extern long TotalScr;


int Load(void);
int Init(void);
int Comp(int a, int x, int y);
int PicOut(int x, int y);
void BKey(void);
void Score(int a0, int score);
void Score1(int c);
void HelpSh(int x, int y);
int Present(void);
void Print(int a, int b, int c, char *s);
int FilMenu(int key);
int EnterN(int x, int y);
int File(void);
int GameOver(void);
void Gprint(int mode, int xloc, int yloc, int c0, int c1, char *fmt, ...);
void Helen(void);
void Ghost(void);
void Time(int t);
void Money(int m);
void Palet(int index, int color);
void Figa(void);
void Dush(void);

void Gprintf(int xloc, int yloc, char *fmt, ...);
int ReadKey(void);
int PCTools(void);

void Text(int a0, int a2);

int OutMove(int m, int dx, int dy);
void DelMove(int m);
void Smoke(int x, int y);
void Vzriv(void);
void Gate(int t1, int t2, int t3, int x, int y);
void Eat(int x, int y);
void Steps(int n);
void Grab(void);
void Trunk(int x, int y, int z);
void Zoom(void);
void Orden(void);
void Book(void);
void Recom(void);
void Lotr(int z);
void Clot(void);
void Napluv(int x, int y, void *pic);
void Menu1(int z);
void Menu2(int z);
void BossKey(void);

#endif
