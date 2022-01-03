#include "kurtan.h"

#include <stdlib.h>
#include <dos.h>
#include <bios.h>
#include <conio.h>

#include "rgfont.h"

void Gprintf(int xloc, int yloc, char *fmt, ...)
{
	va_list argptr;
	char str[80];
	bdosptr(0x25, RG_Font, 0x1F);
	va_start(argptr, format);
	vsprintf(str, fmt, argptr);
	outtextxy(xloc, yloc, str);
	va_end(argptr);
}

int ReadKey(void)
{
	unsigned int key;
	int normkey;
	int extkey;
	key = bioskey(0);
	normkey = key & 0xFF;
	extkey = (key & 0xFF00) >> 8;
	return (normkey == 0) ? (extkey + 0x100) : normkey;
}

void FOut(int clr, int bgclr, int index, int num, long size)
{
	struct date d;
	char attrs[6];
	const char *names[] = {
		"KURTAN   ADA",
		"VOL0R    KRT",
		"VOL1R    KRT",
		"VOL2     KRT",
		"VOL3R    KRT",
		"KURTAN   EXE",
		"FONTR    KRT",
		"PORNO    PCX"
	};
	if(index != 7)
		strcpy(attrs, "...A");
	else
		strcpy(attrs, "H.RA");
	textattr((bgclr << 4) + clr);
	gotoxy(1, index + 5);
	cprintf("  %c %s%9ld %s  ", '0'+num, names[index], size, attrs);
	if(index != 7)
		cprintf("10/15/57  ");
	else
	{
		getdate(&d);
		cprintf("%2d/%02d/%2d  ", d.da_mon, d.da_day, d.da_year % 100);
	}
}

int PCTools(void)
{
	struct REGPACK r;
	int k = 1;
	int l = 0;
	int key = 0;
	int i, j = 5;
	int siz[7];
	int hotkeys[] = {'A','C','D','E','F','H','L','M','P','R','S','V','W','a','c','d','e','f','h','l','m','p','r','s','v','w',0x13C,0x13D,0x144};
	long sizes[] = {227367l,153770l,65882l,28971l,37946l,0l,1848l,47476l};
	long size = 563260l;
	char *header = "    Name     Ext     Sise Attr   Date   ";
	char *temp;
	char *bytes = "bytes.";
	char *files = "files";
	int x = 20;
	char *text[] = {"’¡¨«¨‘®äâ ¢á¥£¤  ¯®¬®¦¥â ‚ ¬ à §à ¡®â âì ¯à®£à ¬¬ë !",
					"Ÿ â ª ¨ §­ «, çâ® ‚ë ­¥ ¯®­¨¬ ¥â¥ èãâ®ª !",
					"„  ­¥â ¦¥ íâ®£® ä ©«  ! Š á®¦ «¥­¨î ...",
					"’¡¨«¨",
					"‘®äâ",
					"‘®¢¬¥áâ­®¥ ‘®¢¥âáª®-€¬¥à¨ª ­áª®¥",
					"     à¥¤¯à¨ïâ¨¥",
					"",
					"380075  ã«.‹¥­¨­  77, ª®à¯. VI",
					"        ’¡¨«¨á¨, ƒ‘‘",
					"",
					"â¥«¥ä. (8832)  365476, 366567",
					"â¥«¥ªá  212100 ALMAZ SU"};
	r.r_ax = 0x100;
	r.r_cx = 0x2000;
	intr(0x10, &r);
	sizes[5] = Leng;
	size += Leng;
	textattr(0); clrscr();
	textattr(31); cprintf("PC Tools Deluxe R4.21"); for(i = 0;i < 38;i++) cprintf(" "); textcolor(7); cprintf("Vol Label="); textcolor(15); cprintf("KURTAN     "); textcolor(7); for(i = 0;i < 33;i++) cprintf("Ä"); cprintf("File Functions"); for(i = 0;i < 18;i++) cprintf("Ä"); cprintf("Scroll Lock OFF");
	gotoxy(1, 17); cprintf("Ú"); for(i = 0;i < 78;i++) cprintf("Ä"); cprintf("¿³\n\r³");
	gotoxy(80, 18); cprintf("³");
	gotoxy(80, 19); cprintf("³Ã"); for(i = 0;i < 78;i++) cprintf("Ä"); cprintf("´³    opy  ove c mp  ind  ename  elete  er view/ dit  ttrib  ordp  rint  ist    ³³");
	cprintf(" ort  elp   =SELECT   =Unselect   =alt dir lst   =other menu    =exit PC Tools³");
	cprintf("³    =%s    =chg drive/path  ³À", text[0]);
	for(i = 0;i < 78;i++) cprintf("Ä");	cprintf("Ù");
	textcolor(15);
	gotoxy(5, 21); cprintf("C");
	gotoxy(10, 21); cprintf("M");
	gotoxy(16, 21); cprintf("O");
	gotoxy(20, 21); cprintf("F");
	gotoxy(25, 21); cprintf("R");
	gotoxy(32, 21); cprintf("D");
	gotoxy(39, 21); cprintf("V");
	gotoxy(48, 21); cprintf("E");
	gotoxy(53, 21); cprintf("A");
	gotoxy(60, 21); cprintf("W");
	gotoxy(66, 21); cprintf("P");
	gotoxy(72, 21); cprintf("L");
	gotoxy(2, 22); cprintf("S");
	gotoxy(7, 22); cprintf("H");
	gotoxy(12, 22); cprintf("%cÙ", 0x11);
	gotoxy(22, 22); cprintf("F1");
	gotoxy(34, 22); cprintf("F2");
	gotoxy(49, 22); cprintf("F3");
	gotoxy(63, 22); cprintf("Esc");
	gotoxy(4, 23); cprintf("F8");
	gotoxy(60, 23); cprintf("F10");
	gotoxy(1, 3); textattr(7); cprintf("Path=C:\\KURTAN\\*.*");
	gotoxy(1, 4); textattr(47); cprintf("%s%s", header, header);
	textattr(7); gotoxy(5, 18);	cprintf("8 %s LISTed   =", files);
	gotoxy(23, 18); cprintf("%9ld %s    8 %s in sub-dir =%9ld %s", size, bytes, files, size, bytes);
	gotoxy(7, 19); cprintf("%s SELECTed =          %s   Available on volume =  3964928 %s", files, bytes, bytes);
	size = 0;
	for(i = 0;i < 8;i++)
	{
		siz[i] = -16;
		if(i != 5)
			FOut(7, 0, i, siz[i], sizes[i]);
		else
			FOut(15, 4, i, siz[i], sizes[i]);
	}
	while(key != 27)
	{
		textattr(7);
		gotoxy(5, 19); cprintf("%d", k - 1);
		gotoxy(23, 19); cprintf("%9ld", size);
		key = ReadKey();
		gotoxy(5, 15); textattr(0); cprintf("                                           ");
	retry:
		switch(key)
		{
		case 0x1B:
			temp = (char*)malloc(396);
			gettext(16, 21, 63, 24, temp);
			textattr(71);
			gotoxy(16, 21); cprintf("É"); for(i = 0;i < 46;i++) cprintf("Í"); cprintf("»");
			gotoxy(16, 22); cprintf("º             Are you SURE you want            º");
			gotoxy(16, 23); cprintf("º            return to KURTAN? (Y/N)           º");
			gotoxy(16, 24); cprintf("È"); for(i = 0;i < 46;i++) cprintf("Í"); cprintf("¼");
			i = ReadKey();
			if(!(i == 'y' || i == 'n' || i == 'Y' || i == 'N'))
			{
				key = 0;
				puttext(16, 21, 63, 24, temp);
			}
			free(temp);
			break;
		case 0x13B:
			for(i = 0;i < 7;i++)
			{
				siz[i] = -16;
				if(i == j)
					FOut(15, 4, i, siz[i], sizes[i]);
				else
					FOut(7, 0, i, siz[i], sizes[i]);
			}
			k = 1;
			size = 0;
			break;
		case 13:
			if(siz[j] < 0)
			{
				siz[j] = k;
				k++;
				size += sizes[j];
			}
			else
			{
				for(i = 0;i < 7;i++)
				{
					if(siz[i] > siz[j])
					{
						siz[i]--;
						FOut(0, 3, i, siz[i], sizes[i]);
					}
				}
				siz[j] = -16;
				k--;
				size -= sizes[j];
			}
			if(j < 6)
				key = 0x150;
			else
			{
				FOut(15, 4, j, siz[j], sizes[j]);
				break;
			}
			/* fall */
		case 0x147:
		case 0x148:
		case 0x14F:
		case 0x150:
			i = j;
			switch(key)
			{
			case 0x150:
				i++;
				if(i == 7)
					l++;
				break;
			case 0x14F:
				i = 6;
				l++;
				break;
			case 0x148:
				i--;
				break;
			case 0x147:
				i = 0;
				/*fall*/
			}
			if(l > 3)
			{
				gotoxy(5, 15); textattr(14); cprintf("%s", text[2]);
				l = 0;
			}
			if(i < 0 || i > 6 || i == j)
				break;
			if(siz[j] < 0)
				FOut(7, 0, j, siz[j], sizes[j]);
			else
				FOut(0, 3, j, siz[j], sizes[j]);
			j = i;
			FOut(15, 4, j, siz[j], sizes[j]);
			break;
		case 0x142:
			delay(1);
			textattr(27);
			for(i = 0;i < 5;i++)
			{
				window(62 - i * 3, 10 - i, 63 + i * 3, 11 + i);
				clrscr();
				while(bioskey(1)) bioskey(0);
				delay(10);
			}
			window(45, 6, 80, 15);
			clrscr();
			cprintf("Õ"); for(i = 0;i < 34;i++) cprintf("Í"); cprintf("¸³");
			for(i = 2;i < 9;i++)
			{
				gotoxy(36, i); cprintf("³³");
			}
			gotoxy(36, 9); cprintf("³Ô"); for(i = 0;i < 34;i++) cprintf("Í"); window(80, 15, 80, 16); cprintf("¾");
			window(45, 6, 80, 15);
			textcolor(14);
			for(i = 2;i < 10;i++)
			{
				gotoxy(3, i); cprintf("%s", text[i+3]);
			}
			gotoxy(x, 3); textcolor(12); cprintf("%s", text[3]); textcolor(9); cprintf("%s", text[4]);
			key = ReadKey();
			textattr(0);
			for(i = 0;i < 5;i++)
			{
				window(62 - i * 3, 10 - i, 63 + i * 3, 11 + i);
				clrscr();
				while(bioskey(1)) bioskey(0);
				delay(10);
			}
			window(45, 6, 80, 15);
			clrscr();
			window(1, 1, 80, 25);
			goto retry;
		default:
			for(i = 0;i < 29;i++)
			{
				if(key == hotkeys[i])
				{
					gotoxy(5, 15);
					textattr(14);
					cprintf("%s", text[1]);
					break;
				}
			}
		}
	}
	textattr(0); clrscr();
	return (i == 'Y' || i == 'y') ? (0x100 + 0x3F) : QKeys[1];
}
