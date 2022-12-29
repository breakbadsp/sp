#Set CXX as a env variable
CXX = g++-11
#CXX = g++
#CXX = clang-cpp-14
#CXX = clang++


LIBS = -lpthread

#C++ version
CXXFLAGS = -std=c++2b

#Optimization
CXXFLAGS += -O2

#Debugging
CXXFLAGS += -g

#Warnings
CXXFLAGS += -Wall \
	    -Wextra \
	    -Werror

TARGET = splib_exe
OUTDIR = ../output

.PHONY: default all clean

default:	$(TARGET)
all:	default

OBJECTS = $(patsubst %.cpp, $(OUTDIR)/%.o, $(wildcard *.cpp))
HEADERS = $(wildcard *.h, wildcard *.hpp)

$(OUTDIR)/%.o:	%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET):	$(OBJECTS)
	$(CXX) $(OBJECTS) $(LIBS) -o $(OUTDIR)/$@

clean:
	rm $(OBJECTS)
	rm $(OUTDIR)/$(TARGET)
