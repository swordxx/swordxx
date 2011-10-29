#CHANGE THESE TO MATCH YOUR SYSTEM
TOMCAT_HOME=/opt/tomcat
#instdir=${TOMCAT_HOME}/webapps/swordweb

# Typically you'll point this to your home directory, e.g.
instdir=/home/swordweb/livehtml/webapp
#instdir=/home/sword/html
#instdir=/home/scribe/src/swordweb/webapp

SERVLET_LIB=${TOMCAT_HOME}/lib/servlet-api.jar

# Older tomcat uses this path
#SERVLET_LIB=${TOMCAT_HOME}/common/lib/servlet-api.jar


all: src/org/crosswire/sword/orb/SWMgr.java classes/org/crosswire/sword/orb/SwordOrb.class

src/org/crosswire/sword/orb/SWMgr.java: ../swordorb.idl
	idlj -pkgTranslate swordorb org.crosswire.sword.orb -td src ../swordorb.idl

classes/org/crosswire/sword/orb/SwordOrb.class: src/org/crosswire/sword/orb/SwordOrb.java
	javac -classpath ${SERVLET_LIB} -d classes -sourcepath src src/org/crosswire/sword/orb/*.java src/org/crosswire/util/*.java


clean:
	rm -f src/org/crosswire/sword/orb/SW*.java
	rm -f src/org/crosswire/sword/orb/_SW*.java
	rm -f src/org/crosswire/sword/orb/Mod*.java
	rm -f src/org/crosswire/sword/orb/Search*.java
	rm -f src/org/crosswire/sword/orb/String*.java
	rm -rf classes/*

install:
	cp -r classes/org/ ${instdir}/WEB-INF/classes/
