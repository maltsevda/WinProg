# variables

CC = cl
CFLAGS = /nologo /W4 /utf-8
DEFLAGS = /D "WIN32" /D "UNICODE" /D "_UNICODE"
LD = link
LDFLAGS = /nologo
RC = rc
RCFLAGS = /nologo /r
RM = del

# static libraries for linker

SRCS = main.cpp proc.cpp main.rc
LIBS = User32.lib Gdi32.lib

OBJS = $(SRCS:.cpp=.obj)
OBJS = $(OBJS:.c=.obj)
OBJS = $(OBJS:.rc=.res)

# rules

.PHONY: all

all: main.exe

main.exe: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) $(LIBS)
	$(RM) *.obj
	$(RM) *.res

.rc.res:
	$(RC) $(RCFLAGS) $(DEFLAGS) /Fo$@ $<

.cpp.obj:
	$(CC) /c $(CFLAGS) $(DEFLAGS) /Fo$@ $<
