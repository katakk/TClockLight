/*-------------------------------------------------------------
  main2.c : TClockExeMain, and initializing functions
  (C) 1997-2003 Kazuto Sato
  Please read readme.txt about the license.
  
  Written by Kazubon, Nanashi-san
---------------------------------------------------------------*/

#include "tclock.h"

/* Globals */

char g_mydir[MAX_PATH];     // path to tclock.exe
BOOL g_bIniSetting = FALSE; // save setting to ini file?
char g_inifile[MAX_PATH];   // ini file name
int  g_winver;              // windows version



/* Statics */
static void InitTClockMain(void);
static void InitTextColor(void);
static void InitFormat(void);
static void AddMessageFilters(HWND hwnd);
static void DelMessageFilters(HWND hwnd);
int CheckWinVersion(void);
static BOOL (WINAPI *m_pChangeWindowMessageFilterEx)(HWND, UINT, DWORD, int) = NULL;
static BOOL (WINAPI *m_pChangeWindowMessageFilter)(UINT, DWORD) = NULL;

#define MSGFLT_RESET 0
#define MSGFLT_ALLOW 1

/*-------------------------------------------
   main routine
---------------------------------------------*/
int TClockExeMain(void)
{
	HMODULE dll;
	MSG msg;
	WNDCLASS wndclass;
	HWND hwnd, hwndParent;
#ifndef _WIN64
	BOOL wow64;
	
	if(IsWow64Process(GetCurrentProcess(), &wow64))
	{
		if(wow64)
		{
			MessageBox(NULL, "Please use x64 version.",
					"TClock", MB_OK|MB_ICONEXCLAMATION);
			return 1;
		}
	}
#endif
	
	// not to execute the program twice
	hwnd = GetTClockMainWindow();
	if(hwnd != NULL)
	{
		return 1;
	}
	
	ImmDisableIME(0);
	
	if(FindWindow("ObjectBar Toolbar", NULL))
	{
		MessageBox(NULL, "ObjectBar is running",
			"TClock", MB_OK|MB_ICONEXCLAMATION);
		return 1;
	}
	
	InitTClockMain();
	
	// register a window class
	wndclass.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wndclass.lpfnWndProc   = WndProc;
	wndclass.cbClsExtra    = 0;
	wndclass.cbWndExtra    = 0;
	wndclass.hInstance     = g_hInst;
	wndclass.hIcon         = NULL;
	wndclass.hCursor       = NULL;
	wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wndclass.lpszMenuName  = NULL;
	wndclass.lpszClassName = CLASS_TCLOCKMAIN;
	RegisterClass(&wndclass);
	


	// message-only window can't receive WM_POWERBROADCAST
	hwndParent = NULL;

	// create a hidden window
	hwnd = CreateWindowEx(0,
		CLASS_TCLOCKMAIN, TITLE_TCLOCKMAIN,
		/*WS_OVERLAPPEDWINDOW*/ WS_POPUP|WS_DISABLED,
		CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
		hwndParent, NULL, g_hInst, NULL);
	ShowWindow(hwnd, SW_MINIMIZE);
	ShowWindow(hwnd, SW_HIDE);

	dll = LoadLibrary(TEXT("user32.dll"));
	(FARPROC)m_pChangeWindowMessageFilterEx = GetProcAddress(dll, "ChangeWindowMessageFilterEx");
	if(m_pChangeWindowMessageFilterEx == NULL)
		(FARPROC)m_pChangeWindowMessageFilter = GetProcAddress(dll, "ChangeWindowMessageFilter");
    
	// Windows Vista UIPI filter
	AddMessageFilters(hwnd);
		
	while(GetMessage(&msg, NULL, 0, 0))
	{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
	}
	
	DelMessageFilters(hwnd);
	UnregisterClass(CLASS_TCLOCKMAIN, g_hInst);
	FreeLibrary(dll);
	return (int)msg.wParam;
}

/*-------------------------------------------
  initialize variables, format, etc.
---------------------------------------------*/
void InitTClockMain(void)
{
	// get the path where .exe is positioned
	GetModuleFileName(g_hInst, g_mydir, MAX_PATH);
	del_title(g_mydir);
	
	g_bIniSetting = FALSE;
	SetMyRegStr(NULL, "ExePath", g_mydir);
	
	// ini file name
	strcpy(g_inifile, g_mydir);
	add_title(g_inifile, "tclock.ini");
	g_bIniSetting = TRUE;
	
	g_winver = CheckWinVersion();
	
	DelMyRegKey("OnContextMenu"); // temporarily
	
	InitTextColor();
	
	InitFormat();
}

/*------------------------------------------------
  for Luna theme of Windows XP
--------------------------------------------------*/
void InitTextColor(void)
{
	int len;
	char s[80];
	char themekey[] =
		"Software\\Microsoft\\Windows\\CurrentVersion\\ThemeManager";
	
	if(GetMyRegStr(NULL, "ForeColor", s, 20, "") > 0) return;
	
	len = GetRegStr(HKEY_CURRENT_USER, themekey,
		"DllName", s, 80, "");
	while((len > 0) && (s[len] != '\\'))
		len--;
	if(lstrcmpi(s + len, "\\luna.msstyles") == 0
		|| lstrcmpi(s + len, "\\Aero.msstyles") == 0)
	{
		GetRegStr(HKEY_CURRENT_USER, themekey,
			"ColorName", s, 80, "");
		if(strcmp(s, "NormalColor") == 0)
			SetMyRegLong(NULL, "ForeColor", RGB(255,255,255));
	}
}

/*------------------------------------------------
  initialize format string
--------------------------------------------------*/
void InitFormat(void)
{
	char s[BUFSIZE_FORMAT];
	BOOL parts[NUM_FORMATPART];
	RECT rc;
	HWND hwnd;
	BOOL bbreak;
	int ilang;
	int i;
	
	if(GetMyRegStr("", "Format", s, BUFSIZE_FORMAT, "") > 0)
		return;
	
	ilang = GetMyRegLong("", "Locale", (int)GetUserDefaultLangID());
	if(GetMyRegStr("", "Locale", s, 20, "") == 0)
		SetMyRegLong("", "Locale", ilang);
	
	InitAutoFormat(ilang); // common/autoformat.c
	
	for(i = 0; i < NUM_FORMATPART; i++)
		parts[i] = FALSE;
	
	parts[PART_YEAR] = TRUE;
	parts[PART_MONTH] = TRUE;
	parts[PART_DAY] = TRUE;
	parts[PART_WEEKDAY] = TRUE;
	parts[PART_HOUR] = TRUE;
	parts[PART_MINUTE] = TRUE;
	parts[PART_SECOND] = TRUE;
	
	bbreak = FALSE;
	hwnd = GetTaskbarWindow();
	if(hwnd != NULL)
	{
		GetClientRect(hwnd, &rc);
		// vertical task bar
		if(rc.right < rc.bottom) bbreak = TRUE;
		
		hwnd = FindWindowEx(hwnd, NULL, "TrayNotifyWnd", NULL);
		if(hwnd)
		{
			RECT rc;
			GetClientRect(hwnd, &rc);
			if(rc.bottom - rc.top > 32) bbreak = TRUE;
		}
	}
	
	parts[PART_BREAK] = bbreak;
	
	AutoFormat(s, parts);  // common/autoformat.c
	
	if(!SetMyRegStr("", "Format", s))
	{
		MessageBox(NULL, "Can't save the settings",
			"Error", MB_OK|MB_ICONEXCLAMATION);
		ExitProcess(1);
	}
	
	SetMyRegLong("", "Kaigyo", parts[PART_BREAK]);
}

/*------------------------------------------------
  add the messages to the UIPI message filter
--------------------------------------------------*/
	const UINT messages[] = {
	//	WM_CREATE,
		WM_CLOSE,
		WM_DESTROY,
	//	WM_ENDSESSION,
	//	WM_POWERBROADCAST,
	//	WM_TIMER,
		//WM_COMMAND,
		//WM_CONTEXTMENU,
		//WM_EXITMENULOOP,
		
		//TCM_HWNDCLOCK,
		//TCM_CLOCKERROR,
		//TCM_EXIT,
		//TCM_RELOADSETTING,
		//WM_COPYDATA,
		//MM_MCINOTIFY,
		//MM_WOM_DONE,
		//TCM_STOPSOUND,
		
		//WM_DROPFILES,
		//WM_MOUSEWHEEL,
		//WM_LBUTTONDOWN,
		//WM_RBUTTONDOWN,
		//WM_MBUTTONDOWN,
		//WM_XBUTTONDOWN,
		//WM_LBUTTONUP,
		//WM_RBUTTONUP,
		//WM_MBUTTONUP,
		//WM_XBUTTONUP,
	};

void AddMessageFilters(HWND hwnd)
{
	int i;	
	for(i = 0; i < ARRAYSIZE(messages); i++)
	{
		if(m_pChangeWindowMessageFilterEx != NULL){
			m_pChangeWindowMessageFilterEx(hwnd, messages[i], MSGFLT_ALLOW, 0);
		}else{
			if(m_pChangeWindowMessageFilter != NULL){
				m_pChangeWindowMessageFilter(messages[i], MSGFLT_ADD);
			}
		}
	}
}

void DelMessageFilters(HWND hwnd)
{
	int i;	
	for(i = 0; i < ARRAYSIZE(messages); i++)
	{
		if(m_pChangeWindowMessageFilterEx != NULL){
			m_pChangeWindowMessageFilterEx(hwnd, WM_APP, MSGFLT_RESET, 0);
		}else{
			if(m_pChangeWindowMessageFilter != NULL){
				m_pChangeWindowMessageFilter(messages[i], MSGFLT_REMOVE);
			}
		}
	}
}

