/*-------------------------------------------
  tcprop.h
---------------------------------------------*/

#include "../common/common.h"
#include "resource.h"

/* ---------- main.c --------------- */

BOOL ExecCommandString(HWND hwnd, const char* command);

extern HINSTANCE g_hInst;
extern char  g_mydir[];
extern BOOL  g_bIniSetting;
extern char  g_inifile[];
extern char  g_langfile[];
extern HFONT g_hfontDialog;
extern int   g_winver;
extern BOOL  g_bApplyClock;
extern BOOL  g_bApplyMain;

/* ---------- pagecolor.c ---------- */
INT_PTR CALLBACK PageColorProc(HWND hDlg, UINT message,
	WPARAM wParam, LPARAM lParam);

/* ---------- pagesize.c ----------- */
INT_PTR CALLBACK PageSizeProc(HWND hDlg, UINT message,
	WPARAM wParam, LPARAM lParam);

/* ---------- pageformat.c --------- */
INT_PTR CALLBACK PageFormatProc(HWND hDlg, UINT message,
	WPARAM wParam, LPARAM lParam);

/* ---------- pageformat2.c -------- */
INT_PTR CALLBACK DlgProcFormat2(HWND hDlg, UINT message,
	WPARAM wParam, LPARAM lParam);

