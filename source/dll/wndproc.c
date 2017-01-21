/*-------------------------------------------------------------
  wndproc.c : subclassified window procedure of clock
  (C) 1997-2003 Kazuto Sato
  Please read readme.txt about the license.
  
  Written by Kazubon, Nanashi-san
---------------------------------------------------------------*/

#include "tcdll.h"

/* Statics */
static void OnTimerMain(HWND hwnd);
static void OnRefreshClock(HWND hwnd);
static void OnWindowPosChanging(HWND hwnd, LPWINDOWPOS pwp);

/*------------------------------------------------
  subclass procedure of the clock
--------------------------------------------------*/
LRESULT CALLBACK SubclassProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch(message)
	{
		/* -------- drawing & sizing ------------- */
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc;
			
			hdc = BeginPaint(hwnd, &ps);
			OnPaint(hwnd, hdc, NULL);
			EndPaint(hwnd, &ps);
			return 0;
		}
		case WM_ERASEBKGND:
			break;
		
		case (WM_USER+100):        // a message requesting for clock size
			  // sent from parent window
			if(g_winver&WIN10RS1) break;
			return OnCalcRect(hwnd);  // (only before Win10RS1)
		
		case WM_WINDOWPOSCHANGING:  // size arrangement
			
			OnWindowPosChanging(hwnd, (LPWINDOWPOS)lParam);
			return 0;
		
		case WM_SIZE:
			
			CreateClockDC(hwnd);    // create offscreen DC
			return 0;
		case WM_SYSCOLORCHANGE:
		case WM_THEMECHANGED:
			
			CreateClockDC(hwnd);   // create offscreen DC
			InvalidateRect(hwnd, NULL, FALSE);

			return 0;
		case WM_WININICHANGE:
		case WM_TIMECHANGE:
		case (WM_USER+101):
		case WM_SETFOCUS:
		case WM_KILLFOCUS:
			InvalidateRect(hwnd, NULL, FALSE);
			return 0;
		
		/* -------- Timers ------------- */
		
		case WM_TIMER:
			switch(wParam)
			{
				case IDTIMER_MAIN:
					OnTimerMain(hwnd); return 0;

			}
			
			return 0;
		
		/* -------- Mouse messages ------------- */
		case WM_NCHITTEST:     // not to pass to the original wndproc
			return DefWindowProc(hwnd, message, wParam, lParam);

		case WM_MOUSEMOVE:
		case WM_NCRBUTTONUP:
			return 0;

		case WM_MOUSEACTIVATE:
			return MA_ACTIVATE;
		
		/* messages sent from other program */
		case CLOCKM_EXIT:   // clean up all
			EndClock(hwnd);
			return 0;
		case CLOCKM_REFRESHCLOCK: // refresh the clock
			OnRefreshClock(hwnd);
			return 0;
		case CLOCKM_DELUSRSTR:    // clear user strings
			InitUserStr();
			return 0;

		/* WM_DESTROY is sent only when Win95+IE4/98/Me is shut down */
		case WM_DESTROY:
			OnDestroy(hwnd); // main2.c
			break;
	}
	return DefSubclassProc(hwnd, message, wParam, lParam);
}

/*------------------------------------------------
  Rearrange the notify area
--------------------------------------------------*/
static void RearrangeNotifyArea(HWND hwnd, HWND hwndClock)
{
	LRESULT size;
	POINT posclk = {0, 0};
	int wclock, hclock;
	HWND hwndChild;
	
	size = OnCalcRect(hwndClock);
	wclock = LOWORD(size);
	hclock = HIWORD(size);
	SetWindowPos(hwndClock, NULL, 0, 0, wclock, hclock,
			SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);
	MapWindowPoints(hwndClock, hwnd, &posclk, 1);
	posclk.x += g_OrigClockWidth;
	posclk.y += g_OrigClockHeight;
	hwndChild = hwndClock;
	while((hwndChild = GetWindow(hwndChild, GW_HWNDNEXT)) != NULL)
	{
		POINT pos = {0, 0};
		MapWindowPoints(hwndChild, hwnd, &pos, 1);
		if(pos.x >= posclk.x)
		{
			// Horizontal taskbar
			pos.x += wclock - g_OrigClockWidth;
			SetWindowPos(hwndChild, NULL, pos.x, pos.y, 0, 0,
					SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER);
		}
		else if(pos.y >= posclk.y)
		{
			// Vertical taskbar
			pos.y += hclock - g_OrigClockHeight;
			SetWindowPos(hwndChild, NULL, pos.x, pos.y, 0, 0,
					SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER);
		}
	}
}

/*------------------------------------------------
  vertical taskbar ?
--------------------------------------------------*/
BOOL IsVertTaskbar(HWND hwndTaskbar)
{
	RECT rc;
	
	GetWindowRect(hwndTaskbar, &rc);
	return (rc.bottom - rc.top) > (rc.right - rc.left);
}

/*------------------------------------------------
  subclass procedure of the tray
--------------------------------------------------*/
LRESULT CALLBACK SubclassTrayProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch(message)
	{
		case (WM_USER+100):	// a message requesting for notify area size
		{
			LRESULT ret, size;
			HWND hwndClock = (HWND)dwRefData;

			ret = DefSubclassProc(hwnd, message, wParam, lParam);
			size = OnCalcRect(hwndClock);
			ret = MAKELONG(LOWORD(size) + LOWORD(ret) - g_OrigClockWidth,
				HIWORD(size) + HIWORD(ret) - g_OrigClockHeight);
			return ret;
		}
		case WM_NOTIFY:
		{
			LRESULT ret;
			NMHDR *nmh = (NMHDR*)lParam;
			HWND hwndClock = (HWND)dwRefData;

			if(nmh->code != PGN_CALCSIZE ||
					g_bTaskbarPosChanging)
				break;
			ret = DefSubclassProc(hwnd, message, wParam, lParam);
			RearrangeNotifyArea(hwnd, hwndClock);
			return ret;
		}
		case WM_WINDOWPOSCHANGING:
		{
			BOOL vert;
			
			vert = IsVertTaskbar(GetParent(hwnd));
			if (g_bVertTaskbar != vert)
			{
				g_bVertTaskbar = vert;
				g_bTaskbarPosChanging = TRUE;
			}
			else
			{
				g_bTaskbarPosChanging = FALSE;
			}
			break;
		}
	}
	
	return DefSubclassProc(hwnd, message, wParam, lParam);
}

/*------------------------------------------------
  WM_TIMER message, wParam = IDTIMER_MAIN
--------------------------------------------------*/
void OnTimerMain(HWND hwnd)
{
	static SYSTEMTIME LastTime = { 0, 0, 0, 0, 0, 0, 0, 0 };

	SYSTEMTIME t;
	HDC hdc;
	
	GetLocalTime(&t);

	// date changed
	if(LastTime.wDay != t.wDay || LastTime.wMonth != t.wMonth ||
		LastTime.wYear != t.wYear)
	{
		InitFormatTime(); // formattime.c
	}
	
	hdc = GetDC(hwnd);
	if(hdc)
	{
		OnPaint(hwnd, hdc, &t); // draw.c: draw the clock
		ReleaseDC(hwnd, hdc);
	}
	memcpy(&LastTime, &t, sizeof(t));
}

/*------------------------------------------------
  CLOCKM_REFRESHCLOCK message
--------------------------------------------------*/
void OnRefreshClock(HWND hwnd)
{
	LoadSetting(hwnd); // reload settings

	CreateClockDC(hwnd); // draw.c

	PostMessage(GetParent(GetParent(hwnd)), WM_SIZE,
		SIZE_RESTORED, 0);

	InvalidateRect(hwnd, NULL, FALSE);
	InvalidateRect(GetParent(hwnd), NULL, TRUE);
}

/*------------------------------------------------
  WM_WINDOWPOSCHANGING message
--------------------------------------------------*/
void OnWindowPosChanging(HWND hwnd, LPWINDOWPOS pwp)
{
	DWORD dw;
	int h, w;
	
	if(g_bFitClock) return;
	
	if(!IsWindowVisible(hwnd) || (pwp->flags & SWP_NOSIZE))
		return;
	
	dw = (DWORD)OnCalcRect(hwnd);
	w = LOWORD(dw); h = HIWORD(dw);
	if(pwp->cx > w) pwp->cx = w;
	if(pwp->cy > h) pwp->cy = h;
}

