# -------------------------------------------
# tcdll.mak
#--------------------------------------------

!IFNDEF SRCDIR
SRCDIR=.
!ENDIF

!IFNDEF COMMONDIR
COMMONDIR=..\common
!ENDIF

!IFNDEF OUTDIR
OUTDIR=..\out
!ENDIF

DLLFILE=$(OUTDIR)\tcdll.tclock
RCFILE=$(SRCDIR)\tcdll.rc
RESFILE=tcdll.res
LIBFILE=tcdll.lib
DEFFILE=$(SRCDIR)\tcdll.def
TDSFILE=$(OUTDIR)\tcdll.tds
TCDLLH=$(SRCDIR)\tcdll.h $(COMMONDIR)\common.h
COMMONH=$(COMMONDIR)\common.h

OBJS=dllmain.obj dllmain2.obj dllwndproc.obj draw.obj\
	format.obj formattime.obj userstr.obj\
	newapi.obj dllutl.obj\
	exec.obj utl.obj reg.obj font.obj localeinfo.obj

LIBS=kernel32.lib user32.lib gdi32.lib advapi32.lib \
	comctl32.lib

!IFDEF WIN64
DLLBASE=0x60066040000
!ELSE
DLLBASE=0x66040000
!ENDIF

all: $(DLLFILE)

CC=cl
LINK=link
RC=rc
RCOPT=/fo 

!IFDEF NODEFAULTLIB

COPT=/c /GS- /W3 /O2 /Oi /DNODEFAULTLIB /D_CRT_SECURE_NO_WARNINGS /nologo /Fo
LOPT=/SUBSYSTEM:WINDOWS /DNODEFAULTLIB /DLL /merge:.rdata=.text /nologo /BASE:$(DLLBASE) /MAP
!IFNDEF WIN64
#LOPT=$(LOPT) /OPT:NOWIN98
!ENDIF
!IF $(MSVC_MAJOR) >= 14
LIBS=$(LIBS) libvcruntime.lib
!ENDIF

$(DLLFILE): $(OBJS) nodeflib.obj $(RESFILE)
	$(LINK) $(LOPT) $(OBJS) nodeflib.obj $(RESFILE) $(LIBS) /DEF:$(DEFFILE) /IMPLIB:$(LIBFILE) /OUT:$@

!ELSE

COPT=/c /W3 /O2 /Oi /D_CRT_SECURE_NO_WARNINGS /nologo /Fo
LOPT=/SUBSYSTEM:WINDOWS /DLL /merge:.rdata=.text /nologo /BASE:$(DLLBASE) /MAP
!IFNDEF WIN64
#LOPT=$(LOPT) /OPT:NOWIN98
!ENDIF

$(DLLFILE): $(OBJS) $(RESFILE)
	$(LINK) $(LOPT) $(OBJS) $(RESFILE) $(LIBS) /DEF:$(DEFFILE) /IMPLIB:$(LIBFILE) /OUT:$@

!ENDIF

{$(COMMONDIR)\}.c{}.obj::
	$(CC) $(COPT).\ $<
{$(SRCDIR)\}.c{}.obj::
	$(CC) $(COPT).\ $<


# obj files

dllmain.obj: $(SRCDIR)\main.c $(TCDLLH)
	$(CC) $(COPT)$@ $(SRCDIR)\main.c
dllmain2.obj: $(SRCDIR)\main2.c $(TCDLLH) ..\config.h
	$(CC) $(COPT)$@ $(SRCDIR)\main2.c
dllwndproc.obj: $(SRCDIR)\wndproc.c $(TCDLLH) ..\config.h
	$(CC) $(COPT)$@ $(SRCDIR)\wndproc.c
format.obj: $(SRCDIR)\format.c $(TCDLLH) ..\config.h
formattime.obj: $(SRCDIR)\formattime.c $(TCDLLH)

userstr.obj: $(SRCDIR)\userstr.c $(TCDLLH)
draw.obj: $(SRCDIR)\draw.c $(TCDLLH)
dllutl.obj: $(SRCDIR)\dllutl.c $(TCDLLH)
newapi.obj: $(SRCDIR)\newapi.c $(TCDLLH)

# common obj files

utl.obj: $(COMMONDIR)\utl.c $(COMMONH)
exec.obj: $(COMMONDIR)\exec.c $(COMMONH)
reg.obj: $(COMMONDIR)\reg.c $(COMMONH)
font.obj: $(COMMONDIR)\font.c $(COMMONH)
localeinfo.obj: $(COMMONDIR)\localeinfo.c $(COMMONH)
nodeflib.obj: $(COMMONDIR)\nodeflib.c $(COMMONH)

# res file

$(RESFILE): $(RCFILE)
	$(RC) $(RCOPT)$@ $(RCFILE)

