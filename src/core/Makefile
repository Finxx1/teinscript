.SUFFIXES: .cpp

all: teinmodcore.dll

CFLAGS = /nologo /EHsc /W3 /I ../../include /Z7
LINKFLAGS = /NOLOGO /DLL /INCREMENTAL:NO

!IFDEF DEBUG
LINKFLAGS = /DEBUG $(LINKFLAGS)
!ENDIF

OBJS = main.obj patch.obj hooks.obj
LIBS = user32.lib

teinmodcore.dll: $(OBJS)
	link $(LINKFLAGS) /OUT:teinmodcore.dll $(OBJS) $(LIBS)

.cpp.obj:
	$(CC) $(CFLAGS) /c /Fo:$@ $<