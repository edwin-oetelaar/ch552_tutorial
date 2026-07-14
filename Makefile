SDCC        ?= sdcc
PACKIHX     ?= packihx
FLASH_TOOL  ?= wchisp

TARGET      ?= ch552_blink
SRC_DIR     := src
INC_DIR     := include
BUILD_DIR   := build

CFLAGS      := -mmcs51 --model-small --xram-size 1024 --iram-size 256 --code-size 14336
CPPFLAGS    := -I$(INC_DIR)

SRC         := $(SRC_DIR)/main.c
OBJ         := $(BUILD_DIR)/main.rel
IHX         := $(BUILD_DIR)/$(TARGET).ihx
HEX         := $(BUILD_DIR)/$(TARGET).hex

.PHONY: all flash clean distclean

all: $(HEX)

flash: $(HEX)
	$(FLASH_TOOL) flash $(HEX)

$(BUILD_DIR):
	mkdir -p $@

$(OBJ): $(SRC) $(INC_DIR)/ch552.h | $(BUILD_DIR)
	$(SDCC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(IHX): $(OBJ)
	$(SDCC) $(CFLAGS) $(CPPFLAGS) $< -o $@

$(HEX): $(IHX)
	$(PACKIHX) $< > $@

clean:
	rm -rf $(BUILD_DIR)

distclean: clean
