all: gsoapsword testclient

gsoapsword:	gsoapsword.h gsoapsword.cpp stdsoap.o
	./soapcpp gsoapsword.h
	g++ -g -ggdb -I/usr/include/sword -I./include -o gsoapsword gsoapsword.cpp yoyo.cpp stdsoap.o soapC.cpp soapServer.cpp -lsword -lz

testclient:	gsoapsword.h testclient.cpp stdsoap.o
	./soapcpp gsoapsword.h
	g++ -I./include -o testclient testclient.cpp stdsoap.o soapC.cpp soapClient.cpp

clean:
	rm gsoapsword testclient soapC.cpp soapServer.cpp soapClient.cpp soapH.h soapStub.h sword.xsd sword.nsmap sword.wsdl
