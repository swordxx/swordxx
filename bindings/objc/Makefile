#
# First enter dependencies/ and run make
#
# Then open this project ObjCSword.xcodeproj with xcode, select Product|Scheme|SWORD (note all caps)
# Build
#
# Then come here and type make
#

all: deploycordova

deploycordova:
	mkdir -p ../cordova/org.crosswire.sword.cordova.SWORD/libs/ios/
	cp ./dependencies/curl/lib/libcurl.a ../cordova/org.crosswire.sword.cordova.SWORD/libs/ios/
	cp ./Build/Debug-iphonesimulator/libSWORD.a ../cordova/org.crosswire.sword.cordova.SWORD/libs/ios/
	cp ../../include/defs.h ../../include/flatapi.h ../cordova/org.crosswire.sword.cordova.SWORD/src/ios/



