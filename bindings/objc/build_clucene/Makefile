.PHONY: default release-ppc release-intel release-fat debug-ppc debug-intel debug-fat clean

default: release-fat

release-ppc:
	./build_mac_clucene.sh release ppc

release-intel:
	./build_mac_clucene.sh release intel

release-fat:
	./build_mac_clucene.sh release fat 

debug-ppc:
	./build_mac_clucene.sh debug ppc

debug-intel:
	./build_mac_clucene.sh debug intel

debug-fat:
	./build_mac_clucene.sh debug fat 

clean:
	rm -rf build

