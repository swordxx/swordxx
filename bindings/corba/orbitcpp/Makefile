all: server testclient

#comment these out if you didn't compile sword with lucene support
luceneflags=-DUSELUCENE
lucenelibs=-lclucene
#----------------------------------

LIBS += ${lucenelibs}
CFLAGS += ${luceneflags}
CXXFLAGS += ${luceneflags}

server: swordorb-common.o server.cpp swordorb-impl.o swordorb-cpp.o swordorb-cpp-common.o swordorb-stubs.o swordorb-cpp-stubs.o swordorb-cpp-skels.o swordorb-skels.o swordorb-cpp.o
	g++ -g -I. -I../../../include -I/usr/include/orbitcpp-2.0/ `orbit2-config --cflags` -o server server.cpp swordorb-impl.o swordorb-common.o swordorb-cpp-common.o swordorb-stubs.o swordorb-cpp-stubs.o swordorb-cpp-skels.o swordorb-skels.o `orbit2-config --libs` -lORBit-2-cpp -lsword -lz ${LIBS}

testclient: testclient.cpp swordorb-impl.cpp swordorb-common.o swordorb-cpp-common.o swordorb-stubs.o swordorb-cpp-stubs.o swordorb-skels.o swordorb-cpp-skels.o
	g++ -I. -I../../../include -I/usr/include/orbitcpp-2.0/ `orbit2-config --cflags` -o testclient testclient.cpp swordorb-common.o swordorb-cpp-common.o swordorb-stubs.o swordorb-cpp-stubs.o swordorb-skels.o swordorb-cpp-skels.o `orbit2-config --libs` -lORBit-2-cpp

swordorb-cpp.o: swordorb-cpp.cc
	g++ -I. -I../../../include -I/usr/include/orbitcpp-2.0/ `orbit2-config --cflags` -g -c swordorb-cpp.cc

swordorb-impl.o: swordorb-impl.cpp
	g++ -g -I. -I../../../include -I/usr/include/orbitcpp-2.0/ `orbit2-config --cflags` -g -c swordorb-impl.cpp

swordorb-common.o: swordorb-common.c
	gcc -I. -I../../../include -I/usr/include/orbitcpp-2.0/ `orbit2-config --cflags` -g -c swordorb-common.c

swordorb-common.c: ./../swordorb.idl
	orbit-idl-2 -l c ./../swordorb.idl
	orbit-idl-2 -l cpp ./../swordorb.idl

swordorb-cpp-common.o: swordorb-cpp-common.cc
	g++ -I. -I../../../include -I/usr/include/orbitcpp-2.0/ `orbit2-config --cflags` -g -c swordorb-cpp-common.cc

swordorb-stubs.o: swordorb-stubs.c
	gcc -I. -I../../../include -I/usr/include/orbitcpp-2.0/ `orbit2-config --cflags` -g -c swordorb-stubs.c

swordorb-cpp-stubs.o: swordorb-cpp-stubs.cc
	g++ -I. -I../../../include -I/usr/include/orbitcpp-2.0/ `orbit2-config --cflags` -g -c swordorb-cpp-stubs.cc

swordorb-skels.o: swordorb-skels.c
	gcc -I. -I../../../include -I/usr/include/orbitcpp-2.0/ `orbit2-config --cflags` -g -c swordorb-skels.c

swordorb-cpp-skels.o: swordorb-cpp-skels.cc
	g++ -I. -I../../../include -I/usr/include/orbitcpp-2.0/ `orbit2-config --cflags` -g -c swordorb-cpp-skels.cc

clean:
	rm -f *.h *.cc *.o *.c server testclient

