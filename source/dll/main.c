/*-------------------------------------------------------------
  main.c : entry point of tcdll.tclock,
           API functions, and hook procedure
  (C) 1997-2003 Kazuto Sato
  Please read readme.txt about the license.
  
  Written by Kazubon, Nanashi-san
---------------------------------------------------------------*/

#include "tcdll.h"

/* Globals */

/* shared data among processes */
#ifdef _MSC_VER
#pragma data_seg("MYDATA")
HHOOK g_hhook = NULL;
HWND  g_hwndTClockMain = NULL;
HWND  g_hwndClock = NULL;
#pragma data_seg()
#endif

/* Statics */
LRESULT CALLBACK CallWndProc(int nCode, WPARAM wParam, LPARAM lParam);

/*------------------------------------------------
  entry point of this DLL
--------------------------------------------------*/
#ifdef NODEFAULTLIB
BOOL WINAPI _DllMainCRTStartup(HANDLE hModule, DWORD dwFunction, LPVOID lpNot)
#else
int WINAPI DllMain(HANDLE hModule, DWORD dwFunction, LPVOID lpNot)
#endif
{
	return TRUE;
}

/*------------------------------------------------
  API: install my hook
  this function is called in tclock.exe process
--------------------------------------------------*/
BOOL WINAPI HookStart(HWND hwndMain)
{
	HWND hwndTaskbar, hwndTray;
	DWORD idThread;
	
	g_hwndTClockMain = hwndMain;
	
	g_hInst = GetModuleHandle(DLLFILENAME);
	
	// find the taskbar
	hwndTaskbar = GetTaskbarWindow();
	if(!hwndTaskbar)
	{
		SendMessage(hwndMain, TCM_CLOCKERROR, 0, 1);
		return FALSE;
	}
	// find the clock window
	hwndTray = FindWindowEx(hwndTaskbar, NULL, "TrayNotifyWnd", "");
	if(!hwndTray)
	{
		SendMessage(hwndMain, TCM_CLOCKERROR, 0, 2);
		return FALSE;
	}
	
	g_hwndClock = FindWindowEx(hwndTray, NULL, "TrayClockWClass", NULL);
	if(!g_hwndClock)
	{
		SendMessage(hwndMain, TCM_CLOCKERROR, 0, 3);
		return FALSE;
	}
	
	// get thread ID of taskbar (explorer)
	// Special thanks to T.Iwata.
	idThread = GetWindowThreadProcessId(hwndTaskbar, NULL);
	if(!idThread)
	{
		SendMessage(hwndMain, TCM_CLOCKERROR, 0, 4);
		return FALSE;
	}
	
	// install an hook to thread of taskbar
	g_hhook = SetWindowsHookEx(WH_CALLWNDPROC, (HOOKPROC)CallWndProc,
		g_hInst, idThread);
	if(!g_hhook)
	{
		SendMessage(hwndMain, TCM_CLOCKERROR, 0, 5);
		return FALSE;
	}
	
	// refresh taskbar
	PostMessage(hwndTaskbar, WM_SIZE, SIZE_RESTORED, 0);
	
	return TRUE;
}

/*------------------------------------------------
  API: uninstall my hook
  this function is called in tclock.exe process
--------------------------------------------------*/
void WINAPI HookEnd(void)
{
	// EndClock() will be called
	if(g_hwndClock && IsWindow(g_hwndClock))
		SendMessage(g_hwndClock, CLOCKM_EXIT, 0, 0);
	
	// uninstall my hook
	if(g_hhook != NULL)
		UnhookWindowsHookEx(g_hhook);
	g_hhook = NULL;
}


/*---------------------------------------------------------
  hook procedure
  this function is called in explorer.exe process
----------------------------------------------------------*/
LRESULT CALLBACK CallWndProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	LPCWPSTRUCT pcwps = (LPCWPSTRUCT)lParam;
	
	if(nCode == HC_ACTION && pcwps && pcwps->hwnd)
	{
		if(!g_bInitClock /*&& pcwps->hwnd == g_hwndClock*/)
		{
			InitClock(g_hwndClock); // main2.c
		}
	}
	return CallNextHookEx(g_hhook, nCode, wParam, lParam);
}

