#!/bin/sh

export PATH=$PATH:/usr/local/arm/2.95.3/bin/
#export CXXFLAGS="-I/usr/include/g++-3 -O3"
export CXXFLAGS="-O3"
export CFLAGS="-O3"
./configure --host=arm-linux --prefix=/usr \
	--disable-shared \
	--without-conf --sysconfdir=/etc 

echo ""
echo ""
echo ""
echo "Configured to NOT write a global /etc/sword.conf on 'make install'."
echo "If this is the first time you've installed sword, be sure to run"
echo "'make install_config' if you would like a basic configuration installed"
echo ""
echo "Next you might try something like: "
echo ""
echo "make"
echo "su"
echo "make install"
echo "(and optionally)"
echo "make install_config"
echo "make register"
echo ""
echo ""
echo ""
