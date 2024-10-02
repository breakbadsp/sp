LIBS = -lpthread
CXXFLAGS = -g -O3 -Werror -std=c++23 -I../sp/ -I../experiments/

#Warning flags 
# Pending Winfinite-recursion, 
CXXFLAGS += -Wall \
			-Wextra \
			-Werror \
			-Wnull-dereference \
			-Wshift-overflow \
			-Wno-div-by-zero \
			-Wfloat-equal \
			-Wshadow \
			-Wfree-nonheap-object \


#Instrumentation-Options flags
# Pending -fsanitize=shadow-call-stack, -fstack-check
#CXXFLAGS += -fsanitize=thread
CXXFLAGS += 	-fsanitize=undefined \
		-fsanitize=address \
		-fsanitize=leak \
		-fsanitize-address-use-after-scope \
		-fsanitize-recover=all \
		-fno-omit-frame-pointer \
		-fstack-protector-all \
		-fno-inline \

#LDDFLAGS = 	-fsanitize=thread 
LDDFLAGS = 	-fsanitize=undefined \
		-fsanitize=address \
		-fsanitize=leak \
		-fsanitize-address-use-after-scope \
		-fsanitize-recover=all \
		-fno-omit-frame-pointer \
		-fno-inline \
		   

#CXXFLAGS += -fext-numeric-literals #conditional for g++ and not for clang
#LDDFLAGS += -fext-numeric-literals

TARGET = unit_test
OUTDIR = ../output

.PHONY: default all clean

default:	$(TARGET)
all:	default

OBJECTS = $(patsubst %.cpp, $(OUTDIR)/%.o, $(wildcard *.cpp))
HEADERS := $(wildcard *.h, wildcard *.hpp)
#HEADERS += $(../sp/wildcard *.h, ../sp/wildcard *.hpp)

$(OUTDIR)/%.o:	%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ -c $< 

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET):	$(OBJECTS)
	$(CXX) $(LDDFLAGS) $(OBJECTS) $(LIBS) -o $(OUTDIR)/$@

clean:
	rm $(OBJECTS)
	rm $(OUTDIR)/$(TARGET)
