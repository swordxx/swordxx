TARGETS= ciphercng swmgrex
all: $(TARGETS)

clean:
	rm $(TARGETS)

.cpp:
	g++ -O3 `pkg-config --cflags sword` $< -o $@ `pkg-config --libs sword`


