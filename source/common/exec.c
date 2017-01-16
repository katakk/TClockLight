/*-------------------------------------------------------------
  exec.c : executing a file
  (C) 1997-2004 Kazuto Sato
  Please read readme.txt about the license.
  
  Written by Kazubon, Nanashi-san
---------------------------------------------------------------*/

#include "common.h"

#define PATH_REL 0
#define PATH_ABS 1
#define PATH_URL 2

/* Statics */
static int GetPathType(const char *src);
static BOOL GetRealFileName(char *dst, const char *src);

extern HINSTANCE g_hInst;
extern char g_mydir[];

/*------------------------------------------------
  relative path -> absolute path
--------------------------------------------------*/
void RelToAbs(char *dst, const char *src)
{
	if(GetPathType(src) == PATH_REL)
	{
		strcpy(dst, g_mydir);
	}
	else
		strcpy(dst, src);
}

/*------------------------------------------------
  relative path / absolute path / URL
--------------------------------------------------*/
int GetPathType(const char *src)
{
	const char *p = src;
	
	if(*p == '\\' && *(p + 1) == '\\') return PATH_ABS;
	else if((('A' <= *p && *p <= 'Z') || ('a' <= *p && *p <= 'z')) &&
		*(p + 1) == ':') return PATH_ABS;
	else
	{
		while(*p &&
			(('A' <= *p && *p <= 'Z') || ('a' <= *p && *p <= 'z')))
		{
			p++;
			if(*p == ':') return PATH_URL;
		}
	}
	return PATH_REL;
}

/*------------------------------------------------
  find a file and add path
--------------------------------------------------*/
BOOL GetRealFileName(char *dst, const char *src)
{
	char fname[MAX_PATH];
	
	RelToAbs(fname, src);
	
	if(IsFile(fname))
	{
		strcpy(dst, fname);
		return TRUE;
	}
	else
		return FALSE;
}
