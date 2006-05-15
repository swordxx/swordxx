for i in `find . -name Makefile -exec grep -il "\-O2" {} \;`; do sed s/-O2/-O0/g $i > $i.new; mv $i.new $i; done
