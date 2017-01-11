/*-------------------------------------------------------------
  wndproc.c : window procedure of tclock.exe
  (C) 1997-2003 Kazuto Sato
  Please read readme.txt about the license.
  
  Written by Kazubon, Nanashi-san
---------------------------------------------------------------*/

#include "tclock.h"

/* Globals */

HWND g_hwndClock = NULL; // clock window

/* Statics */

static void OnCreate(HWND hwnd);
static void OnDestroy(HWND hwnd);
static void ClearTClockMain(HWND hwnd);
static void OnTimerStart(HWND hwnd);

static void OnTaskbarRestart(HWND hwnd);

static void InitError(int n);

static UINT m_uTaskbarRestart;     // taskbar recreating message
static BOOL m_bHook = FALSE;
static BOOL m_bStartTimer = FALSE;


/*-------------------------------------------
   the window procedure
---------------------------------------------*/
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_CREATE:
			OnCreate(hwnd);
			return 0;
		case WM_DESTROY:
			OnDestroy(hwnd);
			return 0;
		case WM_ENDSESSION:
			if(wParam) ClearTClockMain(hwnd);
			break;

		
		case WM_TIMER:
			switch(wParam)
			{
				case IDTIMER_START:
					OnTimerStart(hwnd); break;

			}
			return 0;


		


	}
	
	if(message == m_uTaskbarRestart)
		OnTaskbarRestart(hwnd);
	
	return DefWindowProc(hwnd, message, wParam, lParam);
}

/* ------------------------------- Statics -------------------------------- */

/*-------------------------------------------------------
  WM_CREATE message
---------------------------------------------------------*/
void OnCreate(HWND hwnd)
{
	// Message of the taskbar recreating
	// Special thanks to Mr.Inuya
	m_uTaskbarRestart = RegisterWindowMessage("TaskbarCreated");
	OnTimerStart(hwnd);
}

/*-------------------------------------------------------
  WM_DESTROY message
---------------------------------------------------------*/
void OnDestroy(HWND hwnd)
{
	ClearTClockMain(hwnd);
	PostQuitMessage(0);
}

/*-------------------------------------------------------
  clear up
---------------------------------------------------------*/
void ClearTClockMain(HWND hwnd)
{
	static BOOL bCleared = FALSE;
	
	if(bCleared) return;
	bCleared = TRUE;
		
	if(m_bStartTimer) KillTimer(hwnd, IDTIMER_START);
	m_bStartTimer = FALSE;
	
	if(m_bHook) HookEnd();  // dll/main.c - uninstall the hook
	m_bHook = FALSE;
}

/*-------------------------------------------------------
  WM_TIMER message, wParam = IDTIMER_START
  start customizing the tray clock
---------------------------------------------------------*/
void OnTimerStart(HWND hwnd)
{
	if(m_bStartTimer) KillTimer(hwnd, IDTIMER_START);
	m_bStartTimer = FALSE;
	if(!m_bHook)
		m_bHook = HookStart(hwnd); // dll/main.c - install a hook
	

}

/*-------------------------------------------------------
  When Explorer is hung up,
  and the taskbar is recreated.
---------------------------------------------------------*/
void OnTaskbarRestart(HWND hwnd)
{
	if(m_bHook) HookEnd();
	m_bHook = FALSE; g_hwndClock = NULL;
	
	if(GetMyRegLong(NULL, "TaskbarRestart", TRUE))
	{
		SetTimer(hwnd, IDTIMER_START, 1000, NULL);
		m_bStartTimer = TRUE;
	}
	else
		PostMessage(hwnd, WM_CLOSE, 0, 0);
}

