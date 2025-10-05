
CC=gcc

TARGET_EXE := gpmu-daemon

BUILD_DIR := ./build
SRC_DIR := ./src
INC_DIR := ./inc

OBJS=
SRCS=
INCS=

MAIN_SRC := $(SRC_DIR)/main.c
MAIN_OBJ := $(BUILD_DIR)/main.o
MAIN_INC := $(INC_DIR)/main.h

OBJS += $(MAIN_OBJ)

MIBFILE=raspberryPiSensors
MIB_SRC := $(SRC_DIR)/$(MIBFILE).c
MIB_OBJ := $(BUILD_DIR)/$(MIBFILE).o
MIB_INC := $(INC_DIR)/$(MIBFILE).h

SRCS += $(MIB_SRC)
OBJS += $(MIB_OBJ)
INCS += $(MIB_INC)

SENSOR=ina260
SENSOR_SRC := $(SRC_DIR)/$(SENSOR).c
SENSOR_OBJ := $(BUILD_DIR)/$(SENSOR).o
SENSOR_INC := $(INC_DIR)/$(SENSOR).h

SRCS += $(SENSOR_SRC)
OBJS += $(SENSOR_OBJ)
INCS += $(SENSOR_INC)

TARGETS= $(TARGET_EXE) $(MAIN) $(OBJS)


WIRINGPI=-lwiringPi
CFLAGS=-I. -I$(INC_DIR) `net-snmp-config --cflags`
BUILDAGENTLIBS=`net-snmp-config --agent-libs`


all: $(BUILD_DIR) $(TARGETS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET_EXE): $(OBJS)
	$(CC) -o $@ $^ $(BUILDAGENTLIBS) $(WIRINGPI)

$(MAIN_OBJ): $(MAIN_SRC) $(MAIN_INC)
	$(CC) $(CFLAGS) -o $@ -c $<

$(MIB_OBJ): $(MIB_SRC) $(MIB_INC)
	$(CC) $(CFLAGS) -o $@ -c $<

$(SENSOR_OBJ): $(SENSOR_SRC) $(SENSOR_INC)
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY: clean all
clean:
	rm $(TARGET_EXE) $(OBJS)
