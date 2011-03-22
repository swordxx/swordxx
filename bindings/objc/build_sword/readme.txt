Building sword library for Eloquent:

- make a symlink for libtoolize:
ln -s /usr/bin/glibtoolize /usr/bin/libtoolize

- build the library as it is a dependency for the Xcode project:
./build_mac_sword.sh -a fat -c
(check -? for possible options).

