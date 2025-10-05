
CC=gcc

TARGET_EXE := ./build/gpmu-daemon

BUILD_DIR := ./build
SRC_DIR := ./src
INC_DIR := ./inc

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

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(INC_DIR)/%.h
	$(CC) $(CFLAGS) -o $@ -c $< 

.PHONY: clean all
clean:
	rm $(TARGET_EXE) $(OBJS)
