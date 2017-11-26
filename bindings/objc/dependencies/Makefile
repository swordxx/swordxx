LIBCURLVERSION=7.57.0
ICUVERSION=53.1
CLUCENEVERSION=9.21b

all: curl/unpacked icu/unpacked clucene/unpacked

clean: curlclean icuclean cluceneclean

libcurl_src.tar.gz:
	wget http://crosswire.org/ftpmirror/pub/sword/dependencies/apple/libcurl-${LIBCURLVERSION}/libcurl_src.tar.gz

clucene_src.tar.gz:
	wget http://crosswire.org/ftpmirror/pub/sword/dependencies/apple/clucene-${CLUCENEVERSION}/clucene_src.tar.gz

icu_src.tar.gz:
	wget http://crosswire.org/ftpmirror/pub/sword/dependencies/apple/icu-${ICUVERSION}/icu_src.tar.gz


curl/unpacked: libcurl_src.tar.gz
	tar xjfv libcurl_src.tar.gz
	touch curl/unpacked

icu/unpacked: icu_src.tar.gz
	tar xzfv icu_src.tar.gz
	touch icu/unpacked

clucene/unpacked: clucene_src.tar.gz
	tar xzfv clucene_src.tar.gz
	touch clucene/unpacked

curlclean:
	rm -rf libcurl* curl

icuclean:
	rm -rf icu*

cluceneclean:
	rm -rf clucene*

