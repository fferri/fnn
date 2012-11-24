CXXFLAGS = -ggdb -O0
FANN_CXXFLAGS = $(shell pkg-config --cflags fann)

LDLIBS = -lstdc++
FANN_LDLIBS = $(shell pkg-config --libs fann)

TARGETS = filter wordstats tokenizer makedataset merge substwords train testnet predict

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
	$(CXX) -MM $(CXXFLAGS) $(FANN_CXXFLAGS) $< -o $@

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPS)
endif

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
