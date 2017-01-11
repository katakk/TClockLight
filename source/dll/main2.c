/*-------------------------------------------------------------
  main2.c : initialize and clear up
  (C) 1997-2003 Kazuto Sato
  Please read readme.txt about the license.
  
  Written by Kazubon, Nanashi-san
---------------------------------------------------------------*/

#include "tcdll.h"

/* Globals */

BOOL    g_bInitClock = FALSE;  // InitTClock() has been called
HANDLE  g_hInst;               // instance handle
BOOL    g_bIniSetting;         // use tclock.ini
char    g_inifile[MAX_PATH];   // ini file name
char    g_mydir[MAX_PATH];     // path of tcdll.dll
int     g_winver;              // Windows version
BOOL    g_bVisualStyle;        // Windows XP theme is used


int     g_OrigClockWidth;      // original clock width
int     g_OrigClockHeight;     // original clock height
BOOL    g_bVertTaskbar;        // vertical taskbar ?
BOOL    g_bTaskbarPosChanging;
BOOL    g_bLMousePassThru;     // pass through left button messages

#define SUBCLASS_ID			1
#define SUBCLASSTRAY_ID		2

int CheckWinVersion(void);
/*------------------------------------------------
  initialize the clock
--------------------------------------------------*/
void InitClock(HWND hwnd)
{
	RECT rc;
	
	if(g_bInitClock) return;
	g_bInitClock = TRUE;
	
	g_hInst = GetModuleHandle(DLLFILENAME);
	g_winver = CheckWinVersion();       // common/utl.c
	g_bVisualStyle = IsXPVisualStyle(); // common/utl.c
	
	GetModuleFileName(g_hInst, g_mydir, MAX_PATH);
	del_title(g_mydir);
	
	strcpy(g_inifile, g_mydir);
	add_title(g_inifile, "tclock.ini");
	g_bIniSetting = TRUE;
	
	// Save the original window size
	GetWindowRect(hwnd, &rc);
	g_OrigClockWidth = rc.right - rc.left;
	g_OrigClockHeight = rc.bottom - rc.top;
	// Save taskbar direction
	g_bVertTaskbar = IsVertTaskbar(GetParent(GetParent(hwnd)));
	g_bTaskbarPosChanging = FALSE;
	
	// tell tclock.exe clock's HWND
	PostMessage(g_hwndTClockMain, TCM_HWNDCLOCK, 0, (LPARAM)hwnd);
	
	// read settings
	LoadSetting(hwnd);

	InitUserStr();     // userstr.c
	
	// subclassfy the clock window !!
	if(g_winver&WIN10RS1)
	{
		SetWindowSubclass(GetParent(hwnd), SubclassTrayProc,
				SUBCLASSTRAY_ID, (DWORD_PTR)hwnd);
	}
	SetWindowSubclass(hwnd, SubclassProc, SUBCLASS_ID, 0);
	
	// don't accept double clicks
	SetClassLong(hwnd, GCL_STYLE,
		GetClassLong(hwnd, GCL_STYLE) & ~CS_DBLCLKS);

	PostMessage(GetParent(GetParent(hwnd)), WM_SIZE, SIZE_RESTORED, 0);
	InvalidateRect(GetParent(GetParent(hwnd)), NULL, TRUE);
	SetTimer(hwnd, IDTIMER_MAIN, 5, NULL);
}

/*------------------------------------------------
  ending process
--------------------------------------------------*/
void EndClock(HWND hwnd)
{
	static BOOL bEndClock = FALSE;
	
	if(bEndClock) return; // avoid to be called twice
	bEndClock = TRUE;
	
	g_bVisualStyle = IsXPVisualStyle();
	ClearDrawing();     // drawing.c
	
	EndNewAPI();		// newapi.c
	
	// Stop timers
	KillTimer(hwnd, IDTIMER_MAIN);
	
	// restore window procedure
	RemoveWindowSubclass(hwnd, SubclassProc, SUBCLASS_ID);
	if(g_winver&WIN10RS1)
	{
		RemoveWindowSubclass(GetParent(hwnd), SubclassTrayProc,
				SUBCLASSTRAY_ID);
	}
	
	PostMessage(GetParent(GetParent(hwnd)), WM_SIZE, SIZE_RESTORED, 0);
	InvalidateRect(GetParent(GetParent(hwnd)), NULL, TRUE);
	
	// close window of tclock.exe
	PostMessage(g_hwndTClockMain, WM_CLOSE, 0, 0);
	
	PostMessage(hwnd, WM_TIMER, 0, 0);
}

/*-------------------------------------------------------------
  WM_DESTROY message
---------------------------------------------------------------*/
void OnDestroy(HWND hwnd)
{
	ClearDrawing();             // drawing.c
}

/*-------------------------------------------------------------
   read settings and initialize
   this function is called in InitClock() and OnRefreshClock()
   ReadData() in old version
---------------------------------------------------------------*/
void LoadSetting(HWND hwnd)
{

	g_bLMousePassThru = GetMyRegLong("Mouse", "LeftMousePassThrough",
			(g_winver&WIN10RS1) != 0);
	
	LoadFormatSetting(hwnd);   // format.c
	LoadDrawingSetting(hwnd);  // drawing.c
}

