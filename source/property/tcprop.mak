# ------------------------------------------
# tcprop.mak
#-------------------------------------------

!IFNDEF SRCDIR
SRCDIR=.
!ENDIF

!IFNDEF COMMONDIR
COMMONDIR=..\common
!ENDIF

!IFNDEF OUTDIR
OUTDIR=..\out
!ENDIF

EXEFILE=$(OUTDIR)\tcprop.exe
RCFILE=$(SRCDIR)\tcprop.rc
RESFILE=tcprop.res
TDSFILE=$(OUTDIR)\tcprop.tds
TCPROPH=$(SRCDIR)\tcprop.h $(SRCDIR)\resource.h $(COMMONDIR)\common.h
COMMONH=$(COMMONDIR)\common.h

OBJS=pagecolor.obj pagesize.obj pageformat.obj pageformat2.obj\
	tclang.obj list.obj\
	combobox.obj autoformat.obj localeinfo.obj  \
	utl.obj exec.obj reg.obj font.obj

LIBS=kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib\
	comctl32.lib

all: $(EXEFILE)

CC=cl
LINK=link
RC=rc
RCOPT=/fo 

!IFDEF NODEFAULTLIB

COPT=/c /GS- /W3 /O2 /Oi /DNODEFAULTLIB /D_CRT_SECURE_NO_WARNINGS /nologo /Fo
LOPT=/SUBSYSTEM:WINDOWS /DNODEFAULTLIB /merge:.rdata=.text /nologo /MAP
!IFNDEF WIN64
#LOPT=$(LOPT) /OPT:NOWIN98
!ENDIF
!IF $(MSVC_MAJOR) >= 14
LIBS=$(LIBS) libvcruntime.lib
!ENDIF

$(EXEFILE): propmain.obj $(OBJS) nodeflib.obj $(RESFILE)
	$(LINK) $(LOPT) propmain.obj nodeflib.obj $(OBJS) $(RESFILE) $(LIBS) /OUT:$@

!ELSE

COPT=/c /W3 /O2 /Oi /D_CRT_SECURE_NO_WARNINGS /nologo /Fo
LOPT=/SUBSYSTEM:WINDOWS /merge:.rdata=.text /nologo
!IFNDEF WIN64
#LOPT=$(LOPT) /OPT:NOWIN98
!ENDIF

$(EXEFILE): propmain.obj $(OBJS) $(RESFILE)
	$(LINK) $(LOPT) propmain.obj $(OBJS) $(RESFILE) $(LIBS) /OUT:$@

!ENDIF

{$(COMMONDIR)\}.c{}.obj::
	$(CC) $(COPT).\ $<
{$(SRCDIR)\}.c{}.obj::
	$(CC) $(COPT).\ $<


# obj files

propmain.obj: $(SRCDIR)\main.c $(TCPROPH) ..\config.h
	$(CC) $(COPT)$@ $(SRCDIR)\main.c
pagecolor.obj: $(SRCDIR)\pagecolor.c $(TCPROPH)
pagesize.obj: $(SRCDIR)\pagesize.c $(TCPROPH)
pageformat.obj: $(SRCDIR)\pageformat.c $(TCPROPH) ..\config.h
pageformat2.obj: $(SRCDIR)\pageformat2.c $(TCPROPH)

# common obj files

tclang.obj: $(COMMONDIR)\tclang.c $(COMMONH)
combobox.obj: $(COMMONDIR)\combobox.c $(COMMONH)
autoformat.obj: $(COMMONDIR)\autoformat.c $(COMMONH)
localeinfo.obj: $(COMMONDIR)\localeinfo.c $(COMMONH)
list.obj: $(COMMONDIR)\list.c $(COMMONH)
utl.obj: $(COMMONDIR)\utl.c $(COMMONH)
exec.obj: $(COMMONDIR)\exec.c $(COMMONH)
reg.obj: $(COMMONDIR)\reg.c $(COMMONH)
font.obj: $(COMMONDIR)\font.c $(COMMONH)
nodeflib.obj: $(COMMONDIR)\nodeflib.c $(COMMONH)

# res file

$(RESFILE): $(RCFILE) ..\config.h $(SRCDIR)\tclock.manifest
	$(RC) $(RCOPT)$@ $(RCFILE)
