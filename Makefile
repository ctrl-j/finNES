# Compiler
CXX = $(shell fltk-config --cxx)
# Debug level
DEBUG = -g
# Compiler arguments
CXXFLAGS = $(shell fltk-config --use-gl --use-images --cxxflags ) -I.

# Linker arguments (library locations)
# The "fltk-config" command loads the necessary libraries, as long as they are installed
LDFLAGS  = $(shell fltk-config --use-gl --use-images --ldflags )
# Linker arguments (library names)
LDSTATIC = $(shell fltk-config --use-gl --use-images --ldstaticflags )
# Add references to the Cg and TIFF libraries, since we use those in our pipeline
LDSTATIC += -lCg -lCgGL -ltiff

# Name of compiled application/output
TARGET = pipeline
# Get list of all the cpp source files (if in a subdirectory named "src")
SRCS = $(wildcard src/*/*.cpp)
# Add to the list a reference to the GUI source file (if located in root of project)
SRCS += gui.cxx

# Rule to compile the project using FLTK-specified options
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(DEBUG) $^ -o $@ $(LDSTATIC)

# Rule to delete the project executable, surpresses any errors returned (ie exectuable doesn't exist)
clean: $(TARGET)
	rm -f $(TARGET) 2> /dev/null