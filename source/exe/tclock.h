/*-------------------------------------------
  tclock.h
---------------------------------------------*/

#include "../common/common.h"

// IDs for timer
#define IDTIMER_START       2

// Windows Vista UIPI definitions
#ifndef MSGFLT_ADD
#define MSGFLT_ADD    1
#define MSGFLT_REMOVE 2
#endif


/* --- API of tcdll.tclock ---------- */
BOOL WINAPI HookStart(HWND hwnd);
void WINAPI HookEnd(void);


/* ---------- main.c ---------------- */
extern HINSTANCE g_hInst;

/* ---------- main2.c --------------- */
int TClockExeMain(void);
void MyHelp(HWND hwnd, int id);

extern char  g_mydir[MAX_PATH];
extern BOOL  g_bIniSetting;
extern char  g_inifile[MAX_PATH];
extern int   g_winver;

/* ---------- wndproc.c ------------- */
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

extern HWND  g_hwndClock;


/* ---------- menu.c ---------------- */
void SetFocusTClockMain(HWND hwnd);




