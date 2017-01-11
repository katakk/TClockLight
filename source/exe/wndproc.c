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
static void OnTimerMain(HWND hwnd);

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
				case IDTIMER_MAIN:
					OnTimerMain(hwnd);  break;

				case IDTIMER_MONOFF:
					KillTimer(hwnd, wParam);
					PostMessage(hwnd, WM_SYSCOMMAND, SC_MONITORPOWER, 2);
					break;
			}
			return 0;
		
		// menu and commands
		case WM_COMMAND:
			// command.c
{
			void OnTClockCommand(HWND hwnd, int id, int code);
			OnTClockCommand(hwnd, LOWORD(wParam), HIWORD(wParam));
}
			return 0;
		case WM_CONTEXTMENU:
			// menu.c
			OnContextMenu(hwnd, (HWND)wParam,
				GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;
		case WM_EXITMENULOOP:
			// menu.c
			OnExitMenuLoop(hwnd);
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
	

	// InitSNTP(hwnd); // sntp.c
	

OnTimerStart(hwnd);
	
	SetTimer(hwnd, IDTIMER_MAIN, 1000, NULL);
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

	EndContextMenu();
	
	KillTimer(hwnd, IDTIMER_MAIN);
	
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
  WM_TIMER message, wParam = IDTIMER_MAIN
---------------------------------------------------------*/
void OnTimerMain(HWND hwnd)
{
	static BOOL bTimerAdjusting = FALSE;
	SYSTEMTIME st;
	
	GetLocalTime(&st);
	
	// adjusting milliseconds gap
	if(st.wMilliseconds > 50)
	{
		SetTimer(hwnd, IDTIMER_MAIN, 1001 - st.wMilliseconds, NULL);
		bTimerAdjusting = TRUE;
	}
	else if(bTimerAdjusting)
	{
		bTimerAdjusting = FALSE;
		SetTimer(hwnd, IDTIMER_MAIN, 1000, NULL);
	}
}

/*-------------------------------------------------------
  TCM_HWNDCLOCK message
---------------------------------------------------------*/
void OnTCMHwndClock(HWND hwnd, LPARAM lParam)
{
	g_hwndClock = (HWND)lParam;
}

/*-------------------------------------------------------
  TCM_CLOCKERROR message
---------------------------------------------------------*/
void OnTCMClockError(HWND hwnd, LPARAM lParam)
{
	char s[160];
	
	wsprintf(s, "Failed to start TClock: %d", lParam);
	MessageBox(NULL, s, "Error", MB_OK|MB_ICONEXCLAMATION);
	
	PostMessage(hwnd, WM_CLOSE, 0, 0);
}

/*-------------------------------------------------------
  TCM_EXIT message
---------------------------------------------------------*/
void OnTCMExit(HWND hwnd)
{
	if(g_hwndClock)
		PostMessage(g_hwndClock, CLOCKM_EXIT, 0, 0);
}

/*-------------------------------------------------------
  TCM_RELOADSETTING message
---------------------------------------------------------*/
void OnTCMReloadSetting(HWND hwnd)
{

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

