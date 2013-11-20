CFLAGS = -I../
CPP = g++

%.o: %.cpp
	$(CPP) -c -std=c++0x -o $@ $< $(CFLAGS)

ring:ring.o
	$(CPP) -o $@ $^ $(LDFLAGS)

all:ring
