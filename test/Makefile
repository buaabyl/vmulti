CFLAGS = /nologo /I wdk/ddk /I wdk/api /I vmulti
LFLAGS = /nologo /libpath:wdk

all:
	cl $(CFLAGS) /c vmulti/libvmulti.c
	cl $(CFLAGS) /c testvmulti.c
	link $(LFLAGS) testvmulti.obj libvmulti.obj \
		hid.lib setupapi.lib user32.lib \
		/out:a.exe

