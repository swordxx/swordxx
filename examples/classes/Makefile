TARGETS= ciphercng swmgrex verseranges lastVerseInChapter verseposition simplechapter flatapilookup flatapiparsekey flatapisearch
all: $(TARGETS)

clean:
	rm $(TARGETS)

.cpp:
	g++ -O0 -g -Wall -Werror `pkg-config --cflags sword` $< -o $@ `pkg-config --libs sword`
.c:
	gcc -O0 -g -Wall -Werror `pkg-config --cflags sword` $< -o $@ `pkg-config --libs sword` -lstdc++


