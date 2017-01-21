/*-------------------------------------------
  tcdll.h
---------------------------------------------*/

#include "../common/common.h"

#define DLLFILENAME "tcdll.tclock"

// ThemeChanged Message
#ifndef WM_THEMECHANGED
#define WM_THEMECHANGED                 0x031A
#endif

// timer id
#define IDTIMER_MAIN         1001


/* structure for format handler functions */

typedef struct {
	wchar_t* dp;
	const wchar_t* sp;
	SYSTEMTIME* pt;
	BOOL bZeroPad;
} FORMATHANDLERSTRUCT;

/* ---------- main.c -------------- */
extern HHOOK g_hhook;
extern HWND  g_hwndTClockMain;
extern HWND  g_hwndClock;

/* ---------- main2.c ------------- */
void InitClock(HWND hwnd);
void EndClock(HWND hwnd);
void OnDestroy(HWND hwnd);
void LoadSetting(HWND hwnd);

extern BOOL    g_bInitClock;
extern HANDLE  g_hInst;
extern BOOL    g_bIniSetting;
extern char    g_inifile[];
extern char    g_mydir[];
extern int     g_winver;
extern BOOL    g_bVisualStyle;

extern int     g_OrigClockWidth;
extern int     g_OrigClockHeight;
extern BOOL    g_bVertTaskbar;
extern BOOL    g_bTaskbarPosChanging;

/* ---------- wndproc.c ----------- */
LRESULT CALLBACK SubclassProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
LRESULT CALLBACK SubclassTrayProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
BOOL IsVertTaskbar(HWND hwndTaskbar);

/* ---------- draw.c -------------- */
void LoadDrawingSetting(HWND hwnd);
void ClearDrawing(void);
void ClearClockDC(void);
HDC GetClockBackDC(void);
void OnPaint(HWND hwnd, HDC hdc, const SYSTEMTIME* pt);
LRESULT OnCalcRect(HWND hwnd);
void CreateClockDC(HWND hwnd);

extern BOOL g_bFitClock;

/* ---------- format.c ------------ */
void LoadFormatSetting(HWND hwnd);
void MakeFormat(wchar_t* dst, const SYSTEMTIME* pt,
	const wchar_t* fmt, int nMax);
void MakeFormatEx(wchar_t* dst, const SYSTEMTIME* pt,
	const wchar_t* fmt, int nMax, BOOL bZeroPad);

/* ---------- formattime.c -------- */
void InitFormatTime(void);
void SDateHandler(FORMATHANDLERSTRUCT* pstruc);
void STimeHandler(FORMATHANDLERSTRUCT* pstruc);
void YearHandler(FORMATHANDLERSTRUCT* pstruc);
void MonthHandler(FORMATHANDLERSTRUCT* pstruc);
void DateHandler(FORMATHANDLERSTRUCT* pstruc);
void DayOfWeekHandler(FORMATHANDLERSTRUCT* pstruc);
void HourHandler(FORMATHANDLERSTRUCT* pstruc);
void MinuteHandler(FORMATHANDLERSTRUCT* pstruc);
void SecondHandler(FORMATHANDLERSTRUCT* pstruc);
void AMPMHandler(FORMATHANDLERSTRUCT* pstruc);
void CRLFHandler(FORMATHANDLERSTRUCT* pstruc);
void CharaHandler(FORMATHANDLERSTRUCT* pstruc);
void AltYearHandler(FORMATHANDLERSTRUCT* pstruc);
void EraHandler(FORMATHANDLERSTRUCT* pstruc);
void TimeDifHandler(FORMATHANDLERSTRUCT* pstruc);
void LDATEHandler(FORMATHANDLERSTRUCT* pstruc);
void DATEHandler(FORMATHANDLERSTRUCT* pstruc);
void TIMEHandler(FORMATHANDLERSTRUCT* pstruc);
void MSecondHandler(FORMATHANDLERSTRUCT* pstruc);
void M24KOMAHandler(FORMATHANDLERSTRUCT* pstruc);

/* ---------- userstr.c ----------- */
void InitUserStr(void);
void UStrHandler(FORMATHANDLERSTRUCT* pstruc);

extern wchar_t g_userstr[10][BUFSIZE_USTR];
extern wchar_t g_sdisp1[], g_sdisp2[], g_scat1[], g_scat2[];

/* ---------- newapi.c ------------ */
void EndNewAPI(void);

/* ---------- dllutl.c --------------- */
BOOL IsSubclassed(HWND hwnd);
BOOL CreateOffScreenDC(HDC hdc, HDC *phdcMem, HBITMAP *phbmp,
	int width, int height);
void CopyParentSurface(HWND hwnd, HDC hdcDest, int xdst, int ydst,
	int w, int h, int xsrc, int ysrc);
