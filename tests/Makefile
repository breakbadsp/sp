#Set CXX as a env variable
CXX = g++-12
#CXX = g++
#CXX = clang++
LIBS = -lpthread
CXXFLAGS = -g -O2 -Werror -std=c++23 -I../sp/

#Warning flags 
# Pending Winfinite-recursion, 
CXXFLAGS += -Wall \
			-Wextra \
			-Wnull-dereference \
			-Wshift-overflow \
			-Wno-div-by-zero \
			-Wfloat-equal \
			-Wshadow \
			-Wfree-nonheap-object \


#Instrumentation-Options flags
# Pending -fsanitize=shadow-call-stack, -fstack-check
CXXFLAGS += -fsanitize=address \
			-fsanitize=undefined \
			-fsanitize=leak \
			-fsanitize-address-use-after-scope \
			-fsanitize-recover=all \
			-fno-omit-frame-pointer \
			-fstack-protector-all \

LDDFLAGS = 	-fsanitize=address \
		   	-fsanitize=undefined \
		   	-fsanitize=leak \
		   	-fsanitize-address-use-after-scope \
		   	-fsanitize-recover=all \
			-fno-omit-frame-pointer \
		   

CXXFLAGS += -fext-numeric-literals
LDDFLAGS += -fext-numeric-literals

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
	$(CXX) $(LDDFLAGS) $(OBJECTS) $(LIBS) -o $(OUTDIR)/$@

clean:
	rm $(OBJECTS)
	rm $(OUTDIR)/$(TARGET)
