# Compiler
CC=g++

# Compiler flags
# -g turn on debugging information
# -Wall turn on compiler warnings
# -D add macro to start of source
CFLAGS=-g -Wall -std=c++11 -Wno-misleading-indentation

# Executable Name
EXE= bsplineProgram

# Source files
SRC=*.cpp

# define any directories containing header files other than /usr/include
INCLUDES=

# define library paths
LFLAGS=

# define any libraries to link into executable
LIBS= 	`pkg-config --static --libs glfw3 gl`

# typing 'make' will invoke the first target entry in the file
# you can name this target entry anything, but "default" or "all"
# are the most commonly used names by convention
all:
	$(CC) $(CFLAGS) $(SRC) $(INCLUDES) -o $(EXE) $(LFLAGS) $(LIBS)

clean:
	rm $(EXE)
