#!/bin/sh

./configure --prefix=/usr \
	--enable-debug --disable-shared \
	--without-conf --sysconfdir=/etc

echo ""
echo ""
echo ""
echo "Configured to NOT write a global /etc/sword.conf on 'make install'."
echo "If this is the first time you've installed sword, be sure to run"
echo "'make install_config' if you would like a basic configuration installed"
echo ""
echo "Now you make type: "
echo ""
echo "make"
echo "su"
echo "make install"
echo "(and optionally)"
echo "make install_config"
echo ""
echo ""
echo ""
