LIBS = -lpthread
CXXFLAGS = -g -O3 -Werror -std=c++23 -I../sp/

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
#CXXFLAGS += -fsanitize=address \
#			-fsanitize=undefined \
#			-fsanitize=leak \
#			-fsanitize-address-use-after-scope \
#			-fsanitize-recover=all \
#			-fno-omit-frame-pointer \
#			-fstack-protector-all \
#			-Wno-float-equal \
#
#LDDFLAGS = 	-fsanitize=address \
#		   	-fsanitize=undefined \
#		   	-fsanitize=leak \
#		   	-fsanitize-address-use-after-scope \
#		   	-fsanitize-recover=all \
#			-fno-omit-frame-pointer \
		   

TARGET = experiment
TARGET2 = CacheLatencyBenchmark
OUTDIR = ../output

.PHONY: default all clean

.PHONY: cache

default:	$(TARGET)

# Convenience target to build only the CacheLatencyBenchmark
cache:	$(TARGET2)
all:	default

OBJECTS = $(patsubst %.cpp, $(OUTDIR)/%.o, $(wildcard *.cpp))
HEADERS := $(wildcard *.hpp, wildcard *.hpp)
#HEADERS += $(../sp/wildcard *.h, ../sp/wildcard *.hpp)

$(OUTDIR)/%.o:	%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET):	$(OBJECTS)
	$(CXX) $(LDDFLAGS) $(OBJECTS) $(LIBS) -o $(OUTDIR)/$@

$(TARGET2):	CpuCacheLatencyMeasure.cpp
	$(CXX) $(LDDFLAGS) $< $(LIBS) -o $(OUTDIR)/$@

clean:
	rm $(OBJECTS)
	rm $(OUTDIR)/$(TARGET)
