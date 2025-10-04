#
# Warning: you may need more libraries than are included here on the
# build line.  The agent frequently needs various libraries in order
# to compile pieces of it, but is OS dependent and we can't list all
# the combinations here.  Instead, look at the libraries that were
# used when linking the snmpd master agent and copy those to this
# file.
#

CC=gcc

TARGET_EXE := gpmu-daemon

BUILD_DIR := ./build
SRC_DIR := ./src
INC_DIR := ./src

MAIN = $(SRC_DIR)/main.c
MAIN_INC = $(INC_DIR)/main.h

OBJS=
SRCS=
INCS=

MIBFILE=raspberryPiSensors
MIB_SRC = $(SRC_DIR)/$(MIBFILE).c
MIB_OBJ = $(BUILD_DIR)/$(MIBFILE).o
MIB_INC = $(INC_DIR)/$(MIBFILE).h

SRCS += $(MIB_SRC)
OBJS += $(MIB_OBJ)
INCS += $(MIB_INC)

SENSOR=ina260
SENSOR_SRC = $(SRC_DIR)/$(SENSOR).c
SENSOR_OBJ = $(BUILD_DIR)/$(SENSOR).o
SENSOR_INC = $(INC_DIC)/$(SENSOR).h

SRCS += $(SENSOR_SRC)
OBJS += $(SENSOR_OBJ)
INCS += $(SENSOR_INC)

TARGETS= $(TARGET_EXE) $(MAIN) $(OBJS)


WIRINGPI=-lwiringPi
CFLAGS=-I. `net-snmp-config --cflags`
BUILDAGENTLIBS=`net-snmp-config --agent-libs`


all: $(TARGETS)

gpmu-daemon: $(MAIN) $(OBJS)
	$(CC) -o $@ $^ $(BUILDAGENTLIBS)

$(MIB_OBJ): $(MIB_SRC)
	$(CC) $(CFLAGS) -o $@ -c $^

$(SENSOR_OBJ): $(SENSOR_SRC)
	$(CC) $(WIRINGPI) -o $@ -c $^

.PHONY: clean
clean:
	rm $(TARGET_EXE) $(OBJS)
