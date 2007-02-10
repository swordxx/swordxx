TARGETS= ciphercng swmgrex
all: $(TARGETS)

clean:
	rm $(TARGETS)

.cpp:
	g++ `pkg-config --cflags sword` $< -o $@ `pkg-config --libs sword`


