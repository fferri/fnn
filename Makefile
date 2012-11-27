FANN_CXXFLAGS = $(shell pkg-config --cflags fann)
CXXFLAGS = -std=c++0x $(FANN_CXXFLAGS) -I/opt/local/include -DSOFTMAX

ifeq ($(DEBUG), 1)
	CXXFLAGS += -ggdb -O0
else
	CXXFLAGS += -O3
endif

LDLIBS = -lstdc++
FANN_LDLIBS = $(shell pkg-config --libs fann)

TARGETS = wordstats tokenizer makedataset train testnet predict

UNAME = $(shell uname)

ifeq ($(UNAME), Linux)
# Linux specific crap
endif
ifeq ($(UNAME), Darwin)
# OSX specific crap
endif

all: $(TARGETS)

SRCS = $(shell ls -1 *.cpp)
DEPS = $(SRCS:%.cpp=%.d)
OBJS = $(SRCS:%.cpp=%.o)

deps: $(DEPS)

$(DEPS) : %.d : %.cpp
	$(CXX) -MM $(FANN_CXXFLAGS) $(CXXFLAGS) $< -o $@

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPS)
endif

train: LDLIBS     += $(FANN_LDLIBS)
train: train.o words.o filenames.o

testnet: LDLIBS   += $(FANN_LDLIBS)
testnet: testnet.o words.o

predict: LDLIBS   += $(FANN_LDLIBS)
predict: predict.o words.o filenames.o

wordstats: wordstats.o words.o

makedataset: makedataset.o words.o filenames.o

tokenizer: tokenizer.o words.o filenames.o substwords.o

.PHONY: clean cleanobjs cleandeps cleantargets

clean: cleanobjs cleandeps cleantargets
	rm -f *~
	rm -rf *.dSYM

cleantargets:
	$(RM) -f $(TARGETS)

cleanobjs:
	$(RM) -f $(OBJS)

cleandeps:
	$(RM) -f $(DEPS)
