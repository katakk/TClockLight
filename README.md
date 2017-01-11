config は コンテキストメニューとか#if 0 とかけしました。
おわるときは強制終了です

#define TC_ENABLE_STARTBUTTON      0    
#define TC_ENABLE_STARTMENU        0 
#define TC_ENABLE_TASKBAR          0 
#define TC_ENABLE_TRAYNOTIFY       0    
#define TC_ENABLE_MOUSEDROP        0 
#define TC_ENABLE_BATTERY          0 
#define TC_ENABLE_CPU              0 
#define TC_ENABLE_ETIME            0 
#define TC_ENABLE_HDD              0 
#define TC_ENABLE_MEMORY           0 
#define TC_ENABLE_NETWORK          0 
#define TC_ENABLE_VOLUME           0 
#define TC_ENABLE_WHEEL            0 
#define TC_ENABLE_DESKTOPICON      0 
#define TC_ENABLE_CLOCKDECORATION  0    
#define TC_ENABLE_ANALOGCLOCK      0 
#define TC_DEFAULT_INTERVALSYSINFO 4000 
#define TC_SUPPORT_WINVISTA        0 
#define TC_SUPPORT_WIN7            0 
#define TC_SUPPORT_WIN8            0 
#define TC_SUPPORT_WIN10           1 
#define TC_ENABLE_SYSINFO          0 
#define TC_ENABLE_TASKBAR          0 
#define TC_ENABLE_TASKSWITCH       0    


[![Build status](https://ci.appveyor.com/api/projects/status/kbvfckj64cv5jiq5/branch/master?svg=true)](https://ci.appveyor.com/project/k-takata/tclocklight/branch/master)

# TClock Light kt

This is a modified version of TClock Light.


## Binary packages

Binary packages and Japanese documents are available at the following URL:

* https://github.com/k-takata/TClockLight/releases
* http://k-takata.o.oo7.jp/

The lastest packages might be also available on [AppVeyor](https://ci.appveyor.com/project/k-takata/tclocklight).

1. Select the build from HISTORY (or select LATEST BUILD).
2. Select the job name "Environment: VC10"
3. Click ARTIFACTS, then download the package(s) you need.

BE CAREFUL! The packages on AppVeyor are NOT tested.


## New features

* Supports Windows Vista and later (x86/x64).
* Increases the precision of SNTP.
* Supports some new text formats.
* etc.


## License

You may distribute under the terms of the GNU General Public License.


## References

* TClock Light (The original version by Kazubon):  
  http://homepage1.nifty.com/kazubon/tclocklight/ (404)
