CXXFLAGS = -ggdb -O0

LDLIBS = -lstdc++

TARGETS = filter train wordstats tokenizer makedataset merge substwords

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
# Linux specific crap
endif
ifeq ($(UNAME), Darwin)
# OSX specific crap
endif

all: $(TARGETS)

train: CXXFLAGS += $(shell pkg-config --cflags fann)
train: LDLIBS   += $(shell pkg-config --libs fann)

wordstats: wordstats.o words.o

makedataset: makedataset.o words.o

tokenizer: tokenizer.o words.o

clean:
	rm -f *.o *~ $(TARGETS)
	rm -rf *.dSYM
