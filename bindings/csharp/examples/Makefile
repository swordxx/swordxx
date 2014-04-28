SWORD_BINDINGS=	\
	../AssemblyInfo.cs \
	../InstallManager.cs \
	../Manager.cs \
	../Module.cs \
	../NativeMethods.cs

all: LookupExample.exe

LookupExample.exe: LookupExample.cs
	gmcs LookupExample.cs ${SWORD_BINDINGS}

clean:
	rm -f *.exe
