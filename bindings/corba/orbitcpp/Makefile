all: server testclient

#comment these out if you didn't compile sword with lucene support
#LIBS += -lclucene

#comment these out if you didn't compile sword with ICU support
#LIBS += -licui18n -licuuc -licudata -lpthread -licuio

LIBS = `pkg-config --libs sword`
CXXFLAGS += `pkg-config --cflags sword`
CFLAGS += `pkg-config --cflags sword`

#comment out for release
CXXFLAGS += -g -O0
CFLAGS += -g -O0
LDFLAGS += -g -O0
#----------------------------------

server: swordorb-common.o server.cpp swordorb-impl.o swordorb-cpp.o swordorb-cpp-common.o swordorb-stubs.o swordorb-cpp-stubs.o swordorb-cpp-skels.o swordorb-skels.o swordorb-cpp.o
	$(CXX) $(CXXFLAGS) -I. -I/usr/local/include/orbitcpp-2.0/ `orbit2-config --cflags` -o server server.cpp swordorb-impl.o swordorb-common.o swordorb-cpp-common.o swordorb-stubs.o swordorb-cpp-stubs.o swordorb-cpp-skels.o swordorb-skels.o `orbit2-config --libs` -lORBit-2-cpp ${LIBS}

testclient: testclient.cpp swordorb-impl.cpp swordorb-common.o swordorb-cpp-common.o swordorb-stubs.o swordorb-cpp-stubs.o swordorb-skels.o swordorb-cpp-skels.o
	$(CXX) $(CXXFLAGS) -I. -I/usr/local/include/orbitcpp-2.0/ `orbit2-config --cflags` -o testclient testclient.cpp swordorb-common.o swordorb-cpp-common.o swordorb-stubs.o swordorb-cpp-stubs.o swordorb-skels.o swordorb-cpp-skels.o `orbit2-config --libs` -lORBit-2-cpp

swordorb-cpp.o: swordorb-cpp.cc
	$(CXX) $(CXXFLAGS) -I. -I/usr/local/include/orbitcpp-2.0/ `orbit2-config --cflags`  -c swordorb-cpp.cc

swordorb-impl.o: swordorb-impl.cpp
	$(CXX) $(CXXFLAGS) -I. -I/usr/local/include/orbitcpp-2.0/ `orbit2-config --cflags` -c swordorb-impl.cpp

swordorb-common.o: swordorb-common.c
	$(CC) $(CFLAGS) -I. -I/usr/local/include/orbitcpp-2.0/ `orbit2-config --cflags` -c swordorb-common.c

swordorb-common.c: ./../swordorb.idl
	orbit-idl-2 -l c ./../swordorb.idl
	orbit-idl-2 -l cpp ./../swordorb.idl

swordorb-cpp-common.o: swordorb-cpp-common.cc
	$(CXX) $(CXXFLAGS) -I. -I/usr/local/include/orbitcpp-2.0/ `orbit2-config --cflags` -c swordorb-cpp-common.cc

swordorb-stubs.o: swordorb-stubs.c
	$(CC) $(CFLAGS) -I. -I/usr/local/include/orbitcpp-2.0/ `orbit2-config --cflags` -c swordorb-stubs.c

swordorb-cpp-stubs.o: swordorb-cpp-stubs.cc
	$(CXX) $(CXXFLAGS) -I. -I/usr/local/include/orbitcpp-2.0/ `orbit2-config --cflags` -c swordorb-cpp-stubs.cc

swordorb-skels.o: swordorb-skels.c
	$(CC) $(CFLAGS) -I. -I/usr/local/include/orbitcpp-2.0/ `orbit2-config --cflags` -c swordorb-skels.c

swordorb-cpp-skels.o: swordorb-cpp-skels.cc
	$(CXX) $(CXXFLAGS) -I. -I/usr/local/include/orbitcpp-2.0/ `orbit2-config --cflags` -c swordorb-cpp-skels.cc

clean:
	rm -f *.h *.cc *.o *.c server testclient

