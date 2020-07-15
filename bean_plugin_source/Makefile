OS := $(shell uname)
ifeq ($(OS),Darwin)
  OUTPUT := bean_plugin_Mac
  CC=clang++
  CXXFLAGS=-std=c++11 -stdlib=libc++
else
  OUTPUT := bean_plugin_Linux
  CC=g++
  CXXFLAGS=-std=c++11
endif

all:
	$(CC) $(CXXFLAGS) bean_plugin.cpp -o $(OUTPUT)
clean:
	rm -f $(OUTPUT)
