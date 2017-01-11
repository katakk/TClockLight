/*-------------------------------------------
  tclock.h
---------------------------------------------*/

#include "../common/common.h"

// IDs for timer
#define IDTIMER_START       2
#define IDTIMER_MAIN        3


#define IDTIMER_MONOFF      6




// Windows Vista UIPI definitions
#ifndef MSGFLT_ADD
#define MSGFLT_ADD    1
#define MSGFLT_REMOVE 2
#endif


/* --- API of tcdll.tclock ---------- */
BOOL WINAPI HookStart(HWND hwnd);
void WINAPI HookEnd(void);
void WINAPI GetTClockVersion(char *dst);

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

void EndContextMenu(void);
void OnContextMenu(HWND hwnd, HWND hwndClicked, int xPos, int yPos);
void OnExitMenuLoop(HWND hwnd);
void SetFocusTClockMain(HWND hwnd);



