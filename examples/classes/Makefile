TARGETS= ciphercng swmgrex verseranges lastVerseInChapter verseposition showchapter
all: $(TARGETS)

clean:
	rm $(TARGETS)

.cpp:
	g++ -O0 -g -Wall -Werror `pkg-config --cflags sword` $< -o $@ `pkg-config --libs sword`


