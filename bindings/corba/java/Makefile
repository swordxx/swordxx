all: src/org/crosswire/sword/orb/SWMgr.java classes/org/crosswire/sword/orb/SwordOrb.class

src/org/crosswire/sword/orb/SWMgr.java: ../swordorb.idl
	idlj -pkgTranslate swordorb org.crosswire.sword.orb -td src ../swordorb.idl

classes/org/crosswire/sword/orb/SwordOrb.class: src/org/crosswire/sword/orb/SwordOrb.java
	javac -classpath /var/tomcat4/common/lib/servlet.jar -d classes -sourcepath src src/org/crosswire/sword/orb/*.java src/org/crosswire/util/*.java


clean:
	rm src/org/crosswire/sword/orb/SW*.java
	rm src/org/crosswire/sword/orb/_SW*.java
	rm src/org/crosswire/sword/orb/Mod*.java
	rm src/org/crosswire/sword/orb/String*.java
	rm -rf classes/*

install:
	cp -a classes/org/ /var/tomcat4/webapps/sword/WEB-INF/classes/
