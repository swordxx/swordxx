
root := ..
subdir :=
subdirs := keys utilfuns mgr modules frontend
other-dist := yes
other-install := yes
include ${root}/Makefile.cfg

cpp += sword.cpp

s = $(c) $(cpp)
d = $(c:%.c=%.d) $(cpp:%.cpp=%.d)
o = $(c:%.c=%.o) $(cpp:%.cpp=%.o)
l = $(o) -lsword -lstdc++

sword: $(o)
	$(CC) $(LFLAGS) -o $@ $(l)

# Maintenance targets

other-install:
	@echo ""
	@echo "Hatman is installed! Hooray!"
	@echo "Assuming ${bindir} is in your path type hatman to play"
	@echo ""
	@echo "If you have a properly configured \"mail\" command and this is the first"
	@echo "time you have compiled this version of hatman, type \"make register\" to"
	@echo "help us keep track of how many people have seen it."
	@echo ""

remake: clean all

uninstall:
	@if [ -f ${bindir}/sword ]; then rm -i ${bindir}/sword; fi
	@if [ -f ${libdir} ]; then rm -rfi ${libdir}; fi

# These are rules to create the source distribution - Most people should never
# need them.

cp:
	find \( -name "*.h" -o -name "*.cpp" -o -name "*.c" \) -exec ./cp.sh {} \;

distro-dir := sword-${version}

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
