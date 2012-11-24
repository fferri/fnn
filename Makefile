CXXFLAGS = -ggdb -O0
FANN_CXXFLAGS = $(shell pkg-config --cflags fann)

LDLIBS = -lstdc++
FANN_LDLIBS = $(shell pkg-config --libs fann)

TARGETS = filter wordstats tokenizer makedataset merge substwords train testnet predict

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
# Linux specific crap
endif
ifeq ($(UNAME), Darwin)
# OSX specific crap
endif

all: $(TARGETS)

-include Makefile.deps

Makefile.deps:
	$(CXX) $(CXXFLAGS) $(FANN_CXXFLAGS) -MM *.[ch]pp > Makefile.deps

train: CXXFLAGS += $(FANN_CXXFLAGS)
train: LDLIBS   += $(FANN_LDLIBS)
train: train.o words.o

testnet: CXXFLAGS += $(FANN_CXXFLAGS)
testnet: LDLIBS   += $(FANN_LDLIBS)
testnet: testnet.o words.o

predict: CXXFLAGS += $(FANN_CXXFLAGS) 
predict: LDLIBS   += $(FANN_LDLIBS)
predict: predict.o words.o

wordstats: wordstats.o words.o

makedataset: makedataset.o words.o

tokenizer: tokenizer.o words.o

clean:
	rm -f *.o *~ $(TARGETS)
	rm -rf *.dSYM
