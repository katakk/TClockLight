/*-------------------------------------------------------------
  autoformat.c : assemble time format automatically
  (C) 1997-2003 Kazuto Sato
  Please read readme.txt about the license.
  
  Written by Kazubon, Nanashi-san
---------------------------------------------------------------*/

/* memo: used in exe/main2.c, property/pageformat.c */

#include "common.h"

/* Statics */

static int  m_idate;              // 0: mm/dd/yy 1: dd/mm/yy 2: yy/mm/dd
static wchar_t m_sMon[11];        // abbreviated name for Monday
static BOOL m_bDayOfWeekIsLast;   // yy/mm/dd ddd
static BOOL m_bTimeMarkerIsFirst; // AM/PM hh:nn:ss

/*------------------------------------------------
  initialize locale information
--------------------------------------------------*/
void InitAutoFormat(int ilang)
{
	char s[21];
	int codepage;
	int i;
	int aLangDayOfWeekIsLast[] =
		{ LANG_JAPANESE, LANG_KOREAN, 0 };
	int aTimeMarkerIsFirst[] =
		{ LANG_CHINESE, LANG_JAPANESE, LANG_KOREAN, 0 };
	
	codepage = GetCodePage(ilang);
	
	MyGetLocaleInfoA(ilang, codepage, LOCALE_IDATE, s, 20);
	m_idate = atoi(s);
	MyGetLocaleInfoW(ilang, codepage, LOCALE_SABBREVDAYNAME1, m_sMon, 10);
	
	m_bDayOfWeekIsLast = FALSE;
	for(i = 0; aLangDayOfWeekIsLast[i]; i++)
	{
		if((ilang & 0x00ff) == aLangDayOfWeekIsLast[i])
		{
			m_bDayOfWeekIsLast = TRUE; break;
		}
	}
	
	m_bTimeMarkerIsFirst = FALSE;
	for(i = 0; aTimeMarkerIsFirst[i]; i++)
	{
		if((ilang & 0x00ff) == aTimeMarkerIsFirst[i])
		{
			m_bTimeMarkerIsFirst = TRUE; break;
		}
	}
}

/*------------------------------------------------
  create a format string automatically
--------------------------------------------------*/
void AutoFormat(char* dst, BOOL* parts)
{
	strcpy(dst, "_h:nn:ss.SSS\\nddd\\nyyyy/mm/dd");
}

