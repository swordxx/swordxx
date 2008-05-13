all: server testclient

PREFIX = $(shell grep ^prefix ../../../Makefile|cut -f3 -d' ')

#sword stuff
CXXFLAGS += $(shell PKG_CONFIG_PATH=${PREFIX}/lib/pkgconfig pkg-config --cflags sword)
CFLAGS += $(shell PKG_CONFIG_PATH=${PREFIX}/lib/pkgconfig pkg-config --cflags sword)
LIBS += $(shell PKG_CONFIG_PATH=${PREFIX}/lib/pkgconfig pkg-config --libs sword)

#orbit stuff
CFLAGS += $(shell orbit2-config --cflags)
CXXFLAGS += $(shell orbit2-config --cflags)
LIBS += $(shell orbit2-config --libs)

#orbitcpp stuff-- this is silly.  Is there really NOT a way to derive this?
CFLAGS += -I/usr/include/orbitcpp-2.0
CXXFLAGS += -I/usr/include/orbitcpp-2.0
LIBS += -L/usr/lib64 -lORBit-2-cpp

#comment out for release
#CXXFLAGS += -g -O0
#CFLAGS += -g -O0
#LDFLAGS += -g -O0
#-----------------------------------------------------------------------------


server: swordorb-common.o server.cpp swordorb-impl.o swordorb-cpp.o swordorb-cpp-common.o swordorb-stubs.o swordorb-cpp-stubs.o swordorb-cpp-skels.o swordorb-skels.o swordorb-cpp.o
	$(CXX) $(CXXFLAGS) -I. -o server server.cpp swordorb-impl.o swordorb-common.o swordorb-cpp-common.o swordorb-stubs.o swordorb-cpp-stubs.o swordorb-cpp-skels.o swordorb-skels.o $(LIBS)

testclient: testclient.cpp swordorb-impl.cpp swordorb-common.o swordorb-cpp-common.o swordorb-stubs.o swordorb-cpp-stubs.o swordorb-skels.o swordorb-cpp-skels.o
	$(CXX) $(CXXFLAGS) -I. -o testclient testclient.cpp swordorb-common.o swordorb-cpp-common.o swordorb-stubs.o swordorb-cpp-stubs.o swordorb-skels.o swordorb-cpp-skels.o $(LIBS)

swordorb-cpp.o: swordorb-cpp.cc
	$(CXX) $(CXXFLAGS) -I. -c swordorb-cpp.cc

swordorb-impl.o: swordorb-impl.cpp
	$(CXX) $(CXXFLAGS) -I. -c swordorb-impl.cpp

swordorb-common.o: swordorb-common.c
	$(CC) $(CFLAGS) -I. -c swordorb-common.c

swordorb-common.c: ./../swordorb.idl
	orbit-idl-2 -l c ./../swordorb.idl
	orbit-idl-2 -l cpp ./../swordorb.idl

swordorb-cpp-common.o: swordorb-cpp-common.cc
	$(CXX) $(CXXFLAGS) -I. -c swordorb-cpp-common.cc

swordorb-stubs.o: swordorb-stubs.c
	$(CC) $(CFLAGS) -I. -c swordorb-stubs.c

swordorb-cpp-stubs.o: swordorb-cpp-stubs.cc
	$(CXX) $(CXXFLAGS) -I. -c swordorb-cpp-stubs.cc

swordorb-skels.o: swordorb-skels.c
	$(CC) $(CFLAGS) -I. -c swordorb-skels.c

swordorb-cpp-skels.o: swordorb-cpp-skels.cc
	$(CXX) $(CXXFLAGS) -I. -c swordorb-cpp-skels.cc

clean:
	rm -f *.h *.cc *.o *.c server testclient

install:
	mkdir -p $(PREFIX)/bin
	cp -f server $(PREFIX)/bin/swordorbserver
