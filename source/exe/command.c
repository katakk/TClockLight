/*-------------------------------------------------------------
  command.c : process WM_COMMAND message
  (C) 1997-2003 Kazuto Sato
  Please read readme.txt about the license.
  
  Written by Kazubon, Nanashi-san
---------------------------------------------------------------*/

#include "tclock.h"
#include "../common/command.h"

/* Statics */

typedef struct{
	BYTE   key;
	DWORD  flag;
} KEYEVENT, *LPKEYEVENT;

static void ShowHelp(HWND hwnd);
static void PostMessageCommand(const char *option);
static void ExecHiddenCmdPrompt(HWND hwnd, const char *str);
static BOOL CALLBACK doKyu(HWND hwnd, LPARAM lParam);
static void PushKeybd(LPKEYEVENT lpkey);

/*------------------------------------------------
  WM_COMMAND message
--------------------------------------------------*/
void OnTClockCommand(HWND hwnd, int id, int code)
{
	switch(id)
	{
		case IDC_EXIT: // Exit
			PostMessage(g_hwndClock, CLOCKM_EXIT, 0, 0);
			break;

		case IDC_TCLOCKMENU: // context menu
		{
			POINT pt;
			GetCursorPos(&pt);
			OnContextMenu(hwnd, NULL, pt.x, pt.y);
			break;
		}
		
		case IDC_SCREENSAVER: // screen saver
			SendMessage(GetDesktopWindow(), WM_SYSCOMMAND, SC_SCREENSAVE, 0);
			break;
	}
	
	// commands of task bar
	if(400 < id && id <= 510)
	{
		HWND hwndBar = GetTaskbarWindow();
		if(hwndBar)
			PostMessage(hwndBar, WM_COMMAND, id, 0);
	}
	


}

/*------------------------------------------------
  post message to window
  option : "WindowClass [WindowTitle] message [wParam] [lParam]"
--------------------------------------------------*/
void PostMessageCommand(const char *option)
{
	char param2[81], param3[11], param4[11], param5[11];
	char wndclass[81], title[81];
	int message = 0, wParam = 0, lParam = 0;
	HWND hwnd;
	
	parsespace(wndclass, option, 0, 81);
	parsespace(param2, option, 1, 81);
	parsespace(param3, option, 2, 11);
	parsespace(param4, option, 3, 11);
	parsespace(param5, option, 4, 11);
	
	if(isdigitstr(param2))
	{
		title[0] = 0;
		message = atoi(param2);
		if(param3[0]) wParam = atoi(param3);
		if(param4[0]) lParam = atoi(param4);
	}
	else
	{
		strcpy(title, param2);
		if(param3[0]) message = atoi(param3);
		if(param4[0]) wParam = atoi(param4);
		if(param5[0]) lParam = atoi(param5);
	}
	
	if(wndclass[0] == 0 && title[0] == 0) return;
	if(message == 0) return;
	
	hwnd = FindWindow(wndclass[0] ? wndclass : NULL,
		title[0] ? title : NULL);
	if(hwnd)
		PostMessage(hwnd, message, wParam, lParam);
}

