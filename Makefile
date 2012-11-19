CXXFLAGS = \
	-I/usr/include \
	-I/usr/local/include \
	-I/opt/local/include \
	-ggdb -O0

LDLIBS = \
	-L/usr/lib \
	-L/usr/local/lib \
	-L/opt/local/lib \
	-lstdc++

TARGETS = filter train wordstats tokenizer

all: $(TARGETS)

train: LDLIBS += -lfann

wordstats: wordstats.o words.o

clean:
	rm -f *.o *~ $(TARGETS)
	rm -rf *.dSYM
