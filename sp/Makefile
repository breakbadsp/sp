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

TARGET = splib.a
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
	$(CXX) -c $(OBJECTS) $(LIBS) -o $(OUTDIR)/$@

clean:
	rm $(OBJECTS)
	rm $(OUTDIR)/$(TARGET)
