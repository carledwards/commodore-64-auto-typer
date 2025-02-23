# Teensy 3.2 Keyboard HID Code

This folder holds the Arduino sketch for the Teensy 3.2. The current code sends the letter `a` when pin 6 is pulled low.

## Getting Started

1. Open `teensy_keyboard.ino` in the Arduino IDE configured for your Teensy 3.2.
2. Compile and upload the sketch.
3. Pull pin 6 low to trigger the keyboard output.

## Using PlatformIO

This project is also configured to work with PlatformIO. To build and upload the firmware using PlatformIO and a Python virtual environment, follow these steps:

1. **Set up a virtual environment:**  
   If you haven't already, create and activate a virtual environment, then install PlatformIO:

   ```bash
   python3 -m venv .venv
   source .venv/bin/activate
   pip install platformio
   ```

2. **Initialize the project (if needed):**  
   (This step is optional if `platformio.ini` already exists)

   ```bash
   pio project init --board teensy31
   ```

3. **Build the project:**

   ```bash
   pio run
   ```

4. **Upload the firmware:**

   ```bash
   pio run -t upload
   ```

5. **Monitor serial output (optional):**

   ```bash
   pio device monitor
   ``` 