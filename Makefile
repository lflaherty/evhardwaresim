# CC=g++						# Use this for compiling for PC
CC=arm-linux-gnueabihf-g++      # Use this for compiling for test device
CFLAGS=-Wall -Wextra -std=c++17 -O2
LFLAGS=-lpthread -lm
LIBS=

INCLUDES=-IBase -ISimObjects -IHardwareInterfaces

CFILES=main.cpp \
	Base/DataStore.cpp \
	Base/SimObject.cpp \
	SimObjects/PrintTask.cpp \
	SimObjects/SimObjectExample1.cpp \
	HardwareInterfaces/CANInterface.cpp \
	HardwareInterfaces/GPIOInterface.cpp \
	HardwareInterfaces/SPIInterface.cpp \
	HardwareInterfaces/UARTInterface.cpp \
	HardwareInterfaces/MCP4912.cpp

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
