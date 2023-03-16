CXXFLAGS=-Os `pkg-config --cflags libevdev`
LDFLAGS=`pkg-config --libs libevdev` -lasound

.PHONY: all clean

all: push-to-talk

push-to-talk: push-to-talk.cpp

clean:
	rm -f push-to-talk
