/*-------------------------------------------------------------
  config.h : TClock Light compiling configuration
---------------------------------------------------------------*/

//--------------------------------------------------------
// basic functions

#define TC_ENABLE_STARTBUTTON	0
#define TC_ENABLE_STARTMENU		0
#define TC_ENABLE_TASKBAR		0
#define TC_ENABLE_TRAYNOTIFY	0
#define TC_ENABLE_MOUSEDROP		0

//--------------------------------------------------------
// optional functions

#define TC_ENABLE_BATTERY			0
#define TC_ENABLE_CPU				0
#define TC_ENABLE_ETIME				0
#define TC_ENABLE_HDD				0
#define TC_ENABLE_MEMORY			0
#define TC_ENABLE_NETWORK			0
#define TC_ENABLE_VOLUME			0
#define TC_ENABLE_WHEEL				0
#define TC_ENABLE_DESKTOPICON		0
#define TC_ENABLE_CLOCKDECORATION	0
#define TC_ENABLE_ANALOGCLOCK		0

// Default interval for getting system information: default 4 [sec]
#define TC_DEFAULT_INTERVALSYSINFO	4000

//--------------------------------------------------------
// support OS

#define TC_SUPPORT_WINVISTA		0
#define TC_SUPPORT_WIN7			0
#define TC_SUPPORT_WIN8			0
#define TC_SUPPORT_WIN10		1


//--------------------------------------------------------
// correct the configuration dependencies
//--------------------------------------------------------

#define TC_ENABLE_SYSINFO		0



#undef TC_ENABLE_TASKBAR
#define TC_ENABLE_TASKBAR		0


#define TC_ENABLE_TASKSWITCH	0
