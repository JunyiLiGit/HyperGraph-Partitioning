# CC = compiler (i.e. g++, clang++, etc)
CC=clang++ -std=c++11
# CFLAGS = compiler flags (i.e. -g,  etc)
CFLAGS= -g -O3  
# LFLAGS = linker flags (can leave blank for now)
LFLAGS =
# INCLUDES = header file paths (-I/path)
INCLUDES= -I include
# SOURCES = all the source code in the src directory
SOURCES=$(wildcard src/*.cpp)

#EXEC = name of the executable
EXEC= run

#result:what is needed to get the result
$(EXEC): $(SOURCES) main/main.cpp
	$(CC) $(CFLAGS) $(INCLUDES) main/main.cpp $(SOURCES) -o $(EXEC)

clean:
	rm -fr $(EXEC)
