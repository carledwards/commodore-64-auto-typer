.PHONY: all esp32 teensy
.PHONY: pio_init pio_build pio_upload pio_monitor
.PHONY: esp32_monitor esp32_basic

# Set default ESP32 serial port (update if necessary)
ESP32_PORT ?= /dev/tty.usbserial-0001

# List of BASIC programs to deploy
BASIC_PROGRAMS = hello.bas mouse.bas maze.bas bounce.bas

all: esp32 teensy

esp32: esp32_basic
	@echo "Deploying ESP32 code..."
	ampy --port $(ESP32_PORT) put esp32/main.py

esp32_basic:
	@echo "Deploying BASIC programs..."
	@for prog in $(BASIC_PROGRAMS); do \
		if [ -f esp32/basic/$$prog ]; then \
			echo "Copying $$prog..."; \
			ampy --port $(ESP32_PORT) put esp32/basic/$$prog /$$prog; \
		else \
			echo "Warning: $$prog not found in esp32/basic/"; \
		fi \
	done

esp32_monitor:
	@echo "Opening ESP32 serial monitor using screen..."
	screen $(ESP32_PORT) 115200

teensy: pio_build
	@echo "Deploying Teensy code..."
	
pio_init:
	@echo "Initializing PlatformIO project..."
	cd teensy && pio project init --board teensy31

pio_build:
	@echo "Building Teensy project with PlatformIO..."
	cd teensy && pio run

pio_upload:
	@echo "Uploading Teensy firmware with PlatformIO..."
	cd teensy && pio run -t upload

pio_monitor:
	@echo "Opening PlatformIO serial monitor..."
	cd teensy && pio device monitor

help:
	@echo "Available targets:"
	@echo "  make esp32      - Flash the ESP32 with MicroPython code and BASIC programs"
	@echo "  make esp32_basic- Upload only BASIC programs to ESP32"
	@echo "  make teensy     - Build and upload Teensy Arduino code"
	@echo "  make esp32_monitor  - Open ESP32 serial monitor using screen"
	@echo "  make pio_init   - Initialize the PlatformIO project for Teensy"
	@echo "  make pio_build  - Build Teensy project with PlatformIO"
	@echo "  make pio_upload - Upload Teensy firmware with PlatformIO"
	@echo "  make pio_monitor- Open PlatformIO serial monitor" 