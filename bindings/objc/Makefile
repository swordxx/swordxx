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
	mkdir -p ../cordova/cordova-plugin-crosswire-sword/libs/ios/
	cp ./dependencies/curl/lib/libcurl.a ../cordova/cordova-plugin-crosswire-sword/libs/ios/
#	cp ./Build/Debug-iphonesimulator/libSWORD.a ../cordova/cordova-plugin-crosswire-sword/libs/ios/
#	cp ./Build/Debug-iphoneos/libSWORD.a ../cordova/cordova-plugin-crosswire-sword/libs/ios/
	cp SWORD/Release-universal/libSWORD.a ../cordova/cordova-plugin-crosswire-sword/libs/ios/
	cp ../../include/defs.h ../../include/flatapi.h ../cordova/cordova-plugin-crosswire-sword/src/ios/



