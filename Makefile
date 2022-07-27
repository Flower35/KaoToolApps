################################################################
# Makefile for Kao 2 tools

# Common compilation flags
CFLAGS = -O3 -Wall -pedantic-errors

# GNU C compiler: Win32 target, Windows Subsystem.
CCOMP_WIN32 = i686-w64-mingw32-gcc
CLIB_WIN32  = -Wl,--subsystem,windows -lgdi32 -lpsapi

# GNU C compiler: Linux target, no additional libraries.
CCOMP_LINUX = x86_64-linux-gnu-gcc
CLIB_LINUX  =

################################################################
# Listings of "WinApi ToolBase" headers and source files

TOOLBASE_HDR_FILES = \
  src/kao_tools_common/common.h \
  src/kao_winapi_toolbase/toolbase_main.h \
  src/kao_winapi_toolbase/toolbase_kao.h \
  src/kao_winapi_toolbase/toolbase_kao_mem.h \
  src/kao_winapi_toolbase/toolbase_kao_hacks.h \
  src/kao_winapi_toolbase/toolbase_kao_objects.h \
  src/kao_winapi_toolbase/toolbase_gui.h \
  src/kao_winapi_toolbase/toolbase_misc.h \
  src/kao_winapi_toolbase/toolbase_anyarray.h

TOOLBASE_SRC_FILES = \
  src/kao_winapi_toolbase/toolbase_main.c \
  src/kao_winapi_toolbase/toolbase_kao.c \
  src/kao_winapi_toolbase/toolbase_kao_mem.c \
  src/kao_winapi_toolbase/toolbase_kao_hacks.c \
  src/kao_winapi_toolbase/toolbase_kao_objects.c \
  src/kao_winapi_toolbase/toolbase_gui.c \
  src/kao_winapi_toolbase/toolbase_misc.c \
  src/kao_winapi_toolbase/toolbase_anyarray.c

################################################################
# Listings of "kao2_xray" headers and source files

XRAY_HDR_FILES = \
  src/kao2_xray/xray_main.h \
  src/kao2_xray/xray_nodes.h \
  src/kao2_xray/xray_gui.h

XRAY_SRC_FILES = \
  src/kao2_xray/xray_main.c \
  src/kao2_xray/xray_nodes.c \
  src/kao2_xray/xray_gui.c

XRAY_FILES = \
  $(TOOLBASE_HDR_FILES) \
  $(TOOLBASE_SRC_FILES) \
  $(XRAY_HDR_FILES) \
  $(XRAY_SRC_FILES) \
  res/out/kao2_xray_splash.inc \
  res/out/kao2_xray.res

################################################################
# Makefile dummy rules

.PHONY: \
  all \
  xray \
  clean \
  cleanall

all: \
  xray

xray:   bin/kao2_xray.exe

clean:
	rm -fv \
  res/out/kao2_xray_splash.inc \
  res/out/kao2_xray.res \
  res/out/kao2mp_client_splash.inc \
  res/out/kao2mp_client.res

cleanall: clean
	rm -fv \
  bin/kao2_xray.exe \
  res/out/bmp2inc

################################################################
# Building "kao2_xray" application

bin/kao2_xray.exe: $(XRAY_FILES)
	mkdir -p ./bin
	$(CCOMP_WIN32) $(CFLAGS) \
  -DINC_SPLASH_PATH="../../res/out/kao2_xray_splash.inc" \
  -o $@ \
  $(TOOLBASE_SRC_FILES) $(XRAY_SRC_FILES) \
  res/out/kao2_xray.res \
  $(CLIB_WIN32) -lcomdlg32

################################################################
# Preparing resources for Windows applications

res/out/bmp2inc: res/bmp2inc.c
	mkdir -p ./res/out
	$(CCOMP_LINUX) $(CFLAGS) -o $@ $<

################################################################
# Preparing resources for "kao2_xray" application

res/out/kao2_xray_splash.inc: res/out/bmp2inc
	mkdir -p ./res/out
	res/out/bmp2inc \
  res/kao2_xray_splash.bmp \
  res/out/kao2_xray_splash.inc

res/out/kao2_xray.res: res/kao2_xray.rc
	mkdir -p ./res/out
	i686-w64-mingw32-windres \
  res/kao2_xray.rc \
  -O coff \
  res/out/kao2_xray.res

################################################################
