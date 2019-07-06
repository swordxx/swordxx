all: srcdeps
	@echo
	@echo "native binary component is built from ../Android/"
	@echo

srcdeps:
	mkdir -p cordova-plugin-crosswire-sword/libs/ios/
	cp ../../include/defs.h ../../include/flatapi.h cordova-plugin-crosswire-sword/src/ios/


clean:
	# these are from ../Android
	rm -rf cordova-plugin-crosswire-sword/libs/android/*
	rm -rf cordova-plugin-crosswire-sword/libs/ios/*
	rm cordova-plugin-crosswire-sword/src/android/AndroidMgr.java cordova-plugin-crosswire-sword/src/android/InstallMgr.java cordova-plugin-crosswire-sword/src/android/SWMgr.java cordova-plugin-crosswire-sword/src/android/SWModule.java
	rm cordova-plugin-crosswire-sword/src/ios/defs.h cordova-plugin-crosswire-sword/src/ios/flatapi.h

publish:
	npm publish cordova-plugin-crosswire-sword/

strip:
	./libstrip.sh cordova-plugin-crosswire-sword/libs/ios/libSWORD.a
	./libstrip.sh cordova-plugin-crosswire-sword/libs/ios/libcurl.a
	mv cordova-plugin-crosswire-sword/libs/ios/libSWORD.a-stripped cordova-plugin-crosswire-sword/libs/ios/libSWORD.a
	mv cordova-plugin-crosswire-sword/libs/ios/libcurl.a-stripped cordova-plugin-crosswire-sword/libs/ios/libcurl.a
