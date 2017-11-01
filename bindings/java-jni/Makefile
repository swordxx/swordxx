all:
	mkdir -p classes
	javac -d classes src/org/crosswire/android/sword/*.java
	javah -d jni -classpath classes -jni org.crosswire.android.sword.SWMgr
	javah -d jni -classpath classes -jni org.crosswire.android.sword.SWModule
	javah -d jni -classpath classes -jni org.crosswire.android.sword.InstallMgr
	javap -s -classpath classes/ org.crosswire.android.sword.SWMgr > SWMgr.txt
	javap -s -classpath classes/ org.crosswire.android.sword.SWMgr.ModInfo > ModInfo.txt
	javap -s -classpath classes/ org.crosswire.android.sword.SWModule > SWModule.txt
	javap -s -classpath classes/ org.crosswire.android.sword.SWModule.SearchHit > SearchHit.txt
	javap -s -classpath classes/ org.crosswire.android.sword.SWModule.SearchProgressReporter > SearchProgressReporter.txt
	javap -s -classpath classes/ org.crosswire.android.sword.InstallMgr > InstallMgr.txt
	cp src/org/crosswire/android/sword/*.java ../Android/SWORD/app/src/main/java/org/crosswire/android/sword/


clean:
	rm -rf classes jni/org_crosswire_android_sword_*.h *.txt
