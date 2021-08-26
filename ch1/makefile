TARGET := $(patsubst %.cpp, %.o, $(wildcard *.cpp))
.PHONY: all
all: $(TARGET)
%.o : %.cpp
	g++ -g  $<  -o  $@ -lpthread
