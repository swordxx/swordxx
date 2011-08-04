TARGETS= ciphercng swmgrex verseranges lastVerseInChapter
all: $(TARGETS)

clean:
	rm $(TARGETS)

.cpp:
	g++ -O0 -g `pkg-config --cflags sword` $< -o $@ `pkg-config --libs sword`


