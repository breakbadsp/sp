#Set CXX as a env variable
CXX = g++-11
#CXX = g++
#CXX = clang++
LIBS = -lpthread
CXXFLAGS = -g -O2 -Wall -Wextra -Werror -std=c++23 -I../sp/
TARGET = unit_test
OUTDIR = ../output

.PHONY: default all clean

default:	$(TARGET)
all:	default

OBJECTS = $(patsubst %.cpp, $(OUTDIR)/%.o, $(wildcard *.cpp))
HEADERS := $(wildcard *.h, wildcard *.hpp)
#HEADERS += $(../sp/wildcard *.h, ../sp/wildcard *.hpp)

$(OUTDIR)/%.o:	%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET):	$(OBJECTS)
	$(CXX) $(OBJECTS) $(LIBS) -o $(OUTDIR)/$@

clean:
	rm $(OBJECTS)
	rm $(OUTDIR)/$(TARGET)
