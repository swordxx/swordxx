all: javawrapper
	cp -a SWORD/app/build/intermediates/cmake/debug/obj/* ../cordova/org.crosswire.sword.cordova.SWORD/libs/android/

release: javawrapper
	cp -a SWORD/app/build/intermediates/cmake/release/obj/* ../cordova/org.crosswire.sword.cordova.SWORD/libs/android/

javawrapper:
	cp -a SWORD/app/src/main/java/org/crosswire/android/sword/* ../cordova/org.crosswire.sword.cordova.SWORD/src/android/

clean:
	#these are from ../java-jni
	rm SWORD/app/src/main/java/org/crosswire/android/sword/InstallMgr.java SWORD/app/src/main/java/org/crosswire/android/sword/SWMgr.java SWORD/app/src/main/java/org/crosswire/android/sword/SWModule.java


