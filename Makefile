CXXFLAGS = \
	-I/usr/include \
	-I/usr/local/include \
	-I/opt/local/include \
	-ggdb -O0

LDLIBS = \
	-L/usr/lib \
	-L/usr/local/lib \
	-L/opt/local/lib \
	-lfann

all: filter train words

