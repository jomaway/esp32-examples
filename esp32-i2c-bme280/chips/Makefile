# SPDX-FileCopyrightText: © 2022 Uri Shaked <uri@wokwi.com>
# SPDX-License-Identifier: MIT

SOURCES = bme280.chip.c
TARGET  = dist/bme280.chip.wasm

.PHONY: all
all: $(TARGET) dist/bme280.chip.json

.PHONY: clean
clean:
		rm -rf dist

dist:
		mkdir -p dist

$(TARGET): dist $(SOURCES) wokwi-api.h
	  clang --target=wasm32-unknown-wasi --sysroot /opt/wasi-libc -nostartfiles -Wl,--import-memory -Wl,--export-table -Wl,--no-entry -Werror -o $(TARGET) $(SOURCES)

dist/bme280.chip.json: dist bme280.chip.json
	  cp bme280.chip.json dist

.PHONY: test
test:
	  cd test && arduino-cli compile -e -b arduino:avr:uno blink