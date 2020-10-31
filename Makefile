CC=g++
CFLAGS=-Wall -Wextra -std=c++17 -O2
LFLAGS=-lpthread -lm
LIBS=

INCLUDES=-IBase -ISimObjects

CFILES=sim.cpp \
	Base/DataStore.cpp \
	Base/SimObject.cpp \
	SimObjects/SimObjectExample1.cpp \
	SimObjects/SimObjectExample2.cpp

OBJS=$(CFILES:.cpp=.o)

MAIN=sim.out


# BUILD -----------------
.PHONY: depend clean

all: $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) $(MAIN)
	$(RM) $(OBJS)

depend: $(SRCS)
	makedepend $(INCLUDES) $^

# END
