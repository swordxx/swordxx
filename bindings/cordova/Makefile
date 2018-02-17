all: srcdeps
	@echo
	@echo "native binary component is built from ../Android/"
	@echo

srcdeps:
	mkdir -p org.crosswire.sword.cordova.SWORD/libs/ios/
	cp ../include/defs.h ../include/flatapi.h org.crosswire.sword.cordova.SWORD/src/ios/


clean:
	# these are from ../Android
	rm -rf org.crosswire.sword.cordova.SWORD/libs/android/*
	rm -rf org.crosswire.sword.cordova.SWORD/libs/ios/*
	rm org.crosswire.sword.cordova.SWORD/src/android/AndroidMgr.java org.crosswire.sword.cordova.SWORD/src/android/InstallMgr.java org.crosswire.sword.cordova.SWORD/src/android/SWMgr.java org.crosswire.sword.cordova.SWORD/src/android/SWModule.java
	rm org.crosswire.sword.cordova.SWORD/src/ios/defs.h org.crosswire.sword.cordova.SWORD/src/ios/flatapi.h

