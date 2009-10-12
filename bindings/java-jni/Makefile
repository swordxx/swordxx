all:
	mkdir -p classes
	javac -d classes src/org/crosswire/android/sword/*.java
	javah -d jni -classpath classes -jni org.crosswire.android.sword.SWMgr
	javah -d jni -classpath classes -jni org.crosswire.android.sword.SWModule
	javap -s -classpath classes/ org.crosswire.android.sword.SWMgr > SWMgr.txt
	javap -s -classpath classes/ org.crosswire.android.sword.SWMgr.ModInfo > ModInfo.txt
	javap -s -classpath classes/ org.crosswire.android.sword.SWModule > SWModule.txt
	javap -s -classpath classes/ org.crosswire.android.sword.SWModule.SearchHit > SearchHit.txt
	cp src/org/crosswire/android/sword/*.java /home/scribe/workspace/bishop/src/org/crosswire/android/sword/


