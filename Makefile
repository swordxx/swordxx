root := .
subdir := 
targets := testlib
#bin := testlib
subdirs := src obj lib tests utilities include
other-dist := yes
other-install := yes

docs += README
docs += INSTALL
docs += LICENSE
docssamples += samples/mods.conf.large
docssamples += samples/mods.conf.small
docssmodsd += samples/mods.d/globals.conf
docssmodsd += samples/mods.d/rwp.conf
docssmodsd += samples/mods.d/strongsnt.conf
docssmodsd += samples/mods.d/strongsot.conf
docssmodsd += samples/mods.d/vines.conf
docssmodsd += samples/mods.d/web.conf
docssmodsd += samples/mods.d/websters.conf

include ${root}/Makefile.cfg

ifeq ($(buildshare),yes)
targets += buildso
endif

cpp += testlib.cpp

s = $(c) $(cpp)
d = $(c:%.c=%.d) $(cpp:%.cpp=%.d)
o = $(c:%.c=%.o) $(cpp:%.cpp=%.o)
l = $(o) -lsword -lstdc++

testlib: $(o)
	$(CC) $(LFLAGS) -o $@ $(l)

buildso:
	$(CC) -fpic -shared -o ${root}/lib/libsword.so ${objdir}/*.so


# Maintenance targets

other-install:
	@echo ""
	@echo "sword is installed"
	@echo "Type \"make register\" to help us keep track of how many"
	@echo "people have seen it."
	@echo ""

remake: clean all

uninstall:
	@if [ -f ${bindir}/testlib ]; then rm -i ${bindir}/testlib; fi
	@if [ -f ${libdir} ]; then rm -rfi ${libdir}; fi

# These are rules to create the source distribution - Most people should never
# need them.

cp:
	find \( -name "*.h" -o -name "*.cpp" -o -name "*.c" \) -exec ./cp.sh {} \;

distro-dir := testlib-${version}

other-dist:
	mkdir /tmp/sword /tmp/sword/1 /tmp/sword/2 /tmp/sword/1/${distro-dir}/
	cp -lr .[^.]* * /tmp/sword/1/${distro-dir}/
	make -C /tmp/sword/1/${distro-dir} clean
	cd /tmp/sword/1/${distro-dir} && rm -rf $$(find -name RCS)
	cp -r /tmp/sword/1/${distro-dir}/ /tmp/sword/2
	cd /tmp/sword/2/${distro-dir}/ && chmod -R o-rwx,ug+w *
	cd /tmp/sword/2 && tar cf - sword-${version} -X sword-${version}/.exclude | gzip -9c > /tmp/sword/sword-${version}.tgz
	mv /tmp/sword/sword-${version}.tgz ./
	rm -rf /tmp/sword

register:
	echo "(***) $$USER@$$HOSTNAME, $$(date), $$(uname -m -r -s), SWORD ${version}" | mail sword.users@crosswire.org
