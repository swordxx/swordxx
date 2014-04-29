all: LookupExample.exe

LookupExample.exe: LookupExample.cs
	xbuild LookupExample.csproj /p:Configuration=Release

clean:
	xbuild LookupExample.csproj /p:Configuration=Release /t:Clean
