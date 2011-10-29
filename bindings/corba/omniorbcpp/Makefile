all: server testclient

PREFIX = $(shell grep ^prefix ../../../Makefile|cut -f3 -d' ')

#sword stuff
CXXFLAGS += $(shell PKG_CONFIG_PATH=${PREFIX}/lib/pkgconfig pkg-config --cflags sword)
CFLAGS += $(shell PKG_CONFIG_PATH=${PREFIX}/lib/pkgconfig pkg-config --cflags sword)
LIBS += $(shell PKG_CONFIG_PATH=${PREFIX}/lib/pkgconfig pkg-config --libs sword)

#orbit stuff
CFLAGS += $(shell orbit2-config --cflags)
CFLAGS += $(shell PKG_CONFIG_PATH=${PREFIX}/lib/pkgconfig pkg-config --cflags omniORB4)
CXXFLAGS += $(shell PKG_CONFIG_PATH=${PREFIX}/lib/pkgconfig pkg-config --cflags omniORB4)
LIBS += $(shell PKG_CONFIG_PATH=${PREFIX}/lib/pkgconfig pkg-config --libs omniORB4)

LIBS += -L/usr/lib64 

#comment out for release
#CXXFLAGS += -g -O0
#CFLAGS += -g -O0
#LDFLAGS += -g -O0
#-----------------------------------------------------------------------------


server: server.cpp swordorb-impl.o swordorb-stubs.o 
	$(CXX) $(CXXFLAGS) -I. -o server server.cpp swordorb-impl.o swordorb-stubs.o $(LIBS)

testclient: testclient.cpp swordorb-impl.cpp swordorb-stubs.o 
	$(CXX) $(CXXFLAGS) -I. -o testclient testclient.cpp swordorb-stubs.o $(LIBS)

swordorb-impl.o: swordorb.h swordorb-impl.cpp 
	$(CXX) $(CXXFLAGS) -I. -c swordorb-impl.cpp


swordorb.h: ./../swordorb.idl
	omniidl -bcxx -Wbh=.h -Wbs=-stubs.cpp ./../swordorb.idl

swordorb-stubs.o: swordorb-stubs.cpp
	$(CC) $(CFLAGS) -I. -c swordorb-stubs.cpp

clean:
	rm -f *.h *-stubs.cpp *.cc *.o *.c server testclient

install:
	mkdir -p $(PREFIX)/bin
	cp -f server $(PREFIX)/bin/swordorbserver
