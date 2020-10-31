CC=g++
CFLAGS=-Wall -Wextra -std=c++17 -O2
LFLAGS=-lpthread -lm
LIBS=

# Mostly just adds the include paths for simluink
INCLUDES=

CFILES=sim.cpp \
	DataStore.cpp \
	SimObject.cpp \
	SimObjectExample1.cpp \
	SimObjectExample2.cpp

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
