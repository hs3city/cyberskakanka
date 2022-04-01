SRC := $(wildcard *.ino)
FQBN := arduino:samd:mkr1000
TTY := /dev/ttyACM0
BUILD_DIR := $(subst :,.,build/$(FQBN))
BIN := $(BUILD_DIR)/$(SRC).bin

.PHONY: all upload

all: $(BIN)

$(BIN): $(SRC)
	arduino-cli compile --fqbn $(FQBN) $(SRC) -e

upload: $(BIN)
	arduino-cli upload -p $(TTY) --fqbn $(FQBN) $(SRC)
