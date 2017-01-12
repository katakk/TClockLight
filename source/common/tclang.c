/*-------------------------------------------------------------
  tclang.c : read settings of tclang*.txt
  (C) 1997-2003 Kazuto Sato
  Please read readme.txt about the license.
  
  Written by Kazubon, Nanashi-san
---------------------------------------------------------------*/

#include "common.h"

/* Statics */
static const char *GetControlTitle(char *dst, const char *src, int nMax, int *ccAmp);

/* Externs */

extern HINSTANCE g_hInst;
extern char g_langfile[];


/*-------------------------------------------
  returns a resource string
---------------------------------------------*/
char* MyString(UINT uID, const char *entry)
{
	static char buf[80];
	
	if(g_langfile[0] == 0 ||
		GetPrivateProfileString("String", entry, "", buf,
			80, g_langfile) == 0)
	{
		LoadString(g_hInst, uID, buf, 80);
	}
	
	return buf;
}

/*-------------------------------------------
  create dialog font
---------------------------------------------*/
HFONT CreateDialogFont(void)
{
	char buf[80], name[80], nstr[10];
	int size;
	
	if(g_langfile[0] == 0) return NULL;
	
	GetPrivateProfileString("Main", "DialogFont", "", buf,
		80, g_langfile);
	parse(name, buf, 0, 80);
	if(name[0] == 0) return NULL;
	
	parse(nstr, buf, 1, 10);
	if(nstr[0] == 0) size = 9;
	else
	{
		size = atoi(nstr);
		if(size == 0) size = 9;
	}
	
	return CreateMyFont(name, size, FW_NORMAL, 0, 0);
}




/*-------------------------------------------
  get "AAA" in "[  AAA   ]"
---------------------------------------------*/
const char *GetControlTitle(char *dst, const char *src, int nMax, int *ccAmp)
{
	const char *sp, *ep;
	int i;
	
	*ccAmp = 0;
	
	if(*src != '[') return src;
	src++;
	
	while(*src == ' ') src++;
	sp = src;
	
	ep = NULL;
	while(*src && *src != ']')
	{
		if(*src == ' ')
		{
			if(!ep) ep = src;
		}
		else
		{
			if(*src == '&')
			{
				src = CharNext(src);
				++*ccAmp;
			}
			ep = NULL;
		}
		src = CharNext(src);
	}
	if(!ep) ep = src;
	
	for(i = 0; i < nMax - 1 && sp != ep; i++)
		*dst++ = *sp++;
	*dst = 0;
	
	return src;
}

