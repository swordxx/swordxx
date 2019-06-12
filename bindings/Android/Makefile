all: javawrapper
	rm -rf ../cordova/cordova-plugin-crosswire-sword/libs/android/*
	cp -a SWORD/app/build/intermediates/transforms/stripDebugSymbol/release/0/lib/* ../cordova/cordova-plugin-crosswire-sword/libs/android/

debug: javawrapper
	rm -rf ../cordova/cordova-plugin-crosswire-sword/libs/android/*
	cp -a SWORD/app/build/intermediates/cmake/release/obj/* ../cordova/cordova-plugin-crosswire-sword/libs/android/

javawrapper:
	cp -a SWORD/app/src/main/java/org/crosswire/android/sword/* ../cordova/cordova-plugin-crosswire-sword/src/android/

clean:
	#these are from ../java-jni
	rm SWORD/app/src/main/java/org/crosswire/android/sword/InstallMgr.java SWORD/app/src/main/java/org/crosswire/android/sword/SWMgr.java SWORD/app/src/main/java/org/crosswire/android/sword/SWModule.java


