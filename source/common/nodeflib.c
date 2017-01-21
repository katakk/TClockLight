/*-------------------------------------------------------------
  nodeflib.c : replacements of standard library's functions
  (C) 1997-2003 Kazuto Sato
  Please read readme.txt about the license.
  
  Written by Kazubon, Nanashi-san
---------------------------------------------------------------*/

#include "common.h"

#ifdef NODEFAULTLIB

/*-------------------------------------------
  memcpy
---------------------------------------------*/
#pragma function(memcpy)
void *memcpy(void *d, const void *s, size_t l)
{
	size_t i;
	void *start = d;
	for(i = 0; i < l; i++) *((char*)d)++ = *((char*)s)++;
	return start;
}

/*-------------------------------------------
  memset
---------------------------------------------*/
#pragma function(memset)
void *memset(void *d, int c, size_t l)
{
	size_t i;
	void *start = d;
	for(i = 0; i < l; i++) *((char*)d)++ = (char)c;
	return start;
}

/*-------------------------------------------
  atoi
---------------------------------------------*/
int r_atoi(const char *p)
{
	int r = 0;
	BOOL sign = ( *p == '-' )? TRUE: FALSE;
	if(*p =='+' || sign)
		p++;
	
	while(*p)
	{
		if('0' <= *p && *p <= '9')
			r = r * 10 + *p - '0';
		else break;
		p++;
	}
	if(sign)
		r = -r;
	return r;
}

/*-------------------------------------------
  convert a string (hexadecimal) to int
---------------------------------------------*/
int r_atox(const char *p)
{
	int r = 0;
	while(*p)
	{
		if('0' <= *p && *p <= '9')
			r = r * 16 + *p - '0';
		else if('A' <= *p && *p <= 'F')
			r = r * 16 + *p - 'A' + 10;
		else if('a' <= *p && *p <= 'f')
			r = r * 16 + *p - 'a' + 10;
		p++;

	}
	return r;
}

/*-------------------------------------------
  wtoi
---------------------------------------------*/
int r__wtoi(const WCHAR *p)
{
	int r = 0;
	while(*p)
	{
		if('0' <= *p && *p <= '9')
			r = r * 10 + *p - '0';
		else break;
		p++;
	}
	return r;
}

/*-------------------------------------------
  wcslen
---------------------------------------------*/
size_t r_wcslen(const wchar_t *p)
{
	size_t n = 0; while(*p) { n++; p++; } return n;
}

/*-------------------------------------------
  wcscpy
---------------------------------------------*/
wchar_t *r_wcscpy(wchar_t *dp, const wchar_t *sp)
{
	wchar_t *p = dp;
	while(*sp) *p++ = *sp++; *p = 0; return dp;
}

/*-------------------------------------------
  wcsncmp
---------------------------------------------*/
int r_wcsncmp(const wchar_t *p1, const wchar_t *p2, size_t count)
{
	size_t i;
	for(i = 0; i < count; i++)
	{
		if(*p1 == 0 && *p2 == 0) break;
		if(*p1 != *p2) return ((int)*p1 - (int)*p2);
		p1++; p2++;
	}
	return 0;
}

/*-------------------------------------------
  wcscat
---------------------------------------------*/
wchar_t *r_wcscat(wchar_t *dp, const wchar_t *sp)
{
	wchar_t *p = dp;
	while(*p) p++;
	while(*sp) *p++ = *sp++; *p = 0; return dp;
}


