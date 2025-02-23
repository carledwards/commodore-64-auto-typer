#!/usr/bin/env python3
import machine
import time
import random
import sys


BASIC_PROGRAM = "maze.bas"

# Commodore 64 Keyboard ASCII codes
C64_RUN_STOP_RESTORE = 0xFF  # Special value for RUN/STOP + RESTORE
C64_CLEAR_HOME = 0x93        # SHIFT + CLR/HOME CHR$(147)
C64_RETURN = 0x0D            # RETURN key CHR$(13)

# Commodore 64 function keys F1-F8
C64_F1 = 0x85                 # F1 key CHR$(133)
C64_F2 = 0x86                 # F2 key CHR$(134)
C64_F3 = 0x87                 # F3 key CHR$(135)
C64_F4 = 0x88                 # F4 key CHR$(136)
C64_F5 = 0x89                 # F5 key CHR$(137)
C64_F6 = 0x8A                 # F6 key CHR$(138)
C64_F7 = 0x8B                 # F7 key CHR$(139)
C64_F8 = 0x8C                 # F8 key CHR$(140)

# Commodore 64 color keys
C64_BLACK = 0x90             # BLACK key CHR$(144)
C64_WHITE = 0x05             # WHITE key CHR$(5)
C64_RED = 0x1C                # RED key CHR$(28)
C64_CYAN = 0x9F              # CYAN key CHR$(159)
C64_PURPLE = 0x9C            # PURPLE key CHR$(156)
C64_GREEN = 0x1E             # GREEN key CHR$(30)
C64_BLUE = 0x1F              # BLUE key CHR$(31)
C64_YELLOW = 0x9E            # YELLOW key CHR$(158)
C64_ORANGE = 0x81            # ORANGE key CHR$(129)
C64_BROWN = 0x95             # BROWN key CHR$(149)
C64_LIGHT_RED = 0x96         # LIGHT RED key CHR$(150)
C64_DARK_GRAY = 0x97         # DARK GRAY key CHR$(151)
C64_MEDIUM_GRAY = 0x98       # MEDIUM GRAY key CHR$(152)
C64_LIGHT_GREEN = 0x99       # LIGHT GREEN key
C64_LIGHT_BLUE = 0x9A        # LIGHT BLUE key
C64_LIGHT_GRAY = 0x9B       # LIGHT GRAY key


# Mapping escape tokens to their corresponding C64 keyboard ascii codes
token_map = {
    "{clear}": C64_CLEAR_HOME,
    "{white}": C64_WHITE,
    "{red}": C64_RED,
    "{green}": C64_GREEN,
    "{blue}": C64_BLUE,
    "{yellow}": C64_YELLOW,
    "{orange}": C64_ORANGE,
    "{brown}": C64_BROWN,
    "{light red}": C64_LIGHT_RED,
    "{dark gray}": C64_DARK_GRAY,
    "{medium gray}": C64_MEDIUM_GRAY,
    "{light green}": C64_LIGHT_GREEN,
    "{light blue}": C64_LIGHT_BLUE,
    "{light gray}": C64_LIGHT_GRAY,
}

# Initialize UART2 on GPIO16 (RX) and GPIO17 (TX)
teensy_uart = machine.UART(2, baudrate=9600, rx=16, tx=17)

# Configure the trigger pin on GPIO4 with an internal pull-up resistor
trigger_pin = machine.Pin(4, machine.Pin.IN, machine.Pin.PULL_UP)

def tokenize_line(line):
    """Convert a line of BASIC code into a list of bytes.
    Handles special tokens marked with {token}."""
    bytes_list = []
    i = 0
    
    while i < len(line):
        if line[i] == '{':
            # Look for the closing brace
            end = line.find('}', i)
            if end != -1:
                token = line[i:end+1]
                if token in token_map:
                    bytes_list.append(token_map[token])
                    i = end + 1
                    continue
        
        # If we get here, either it's not a token or not a recognized token
        bytes_list.append(ord(line[i]))
        i += 1
    
    return bytes_list

def process_basic_program(filename):
    """Read and process a BASIC program file, returning a list of bytes to send."""
    bytes_to_send = []
    line_num = 0
    
    with open(filename, 'r') as f:
        for line in f:
            line_num += 1
            line = line.lower().rstrip('\n')
            print(f"Line {line_num}: {line}")
            
            # Convert line to bytes
            line_bytes = tokenize_line(line)
            bytes_to_send.extend(line_bytes)
            bytes_to_send.append(C64_RETURN)
            
    return bytes_to_send

def send_keystroke_to_c64(b):
    """Send a single byte over UART"""
    teensy_uart.write(bytes([b]))
    time.sleep_ms(100)

def send_keystrokes_to_c64(b):
    for byte in b:
        send_keystroke_to_c64(byte)

def load_and_run_program(basic_program):
    # Process the BASIC program first
    print(f"\nProcessing {basic_program}...")
    bytes_to_send = process_basic_program(basic_program)
    print(f"Processed into {len(bytes_to_send)} bytes:")
    print([hex(b) for b in bytes_to_send])
    
    # Now start the actual sending sequence
    print("\nStarting transmission sequence...")
    
    # Send RUN/STOP + RESTORE
    print("Sending RUN/STOP + RESTORE")
    send_keystroke_to_c64(C64_RUN_STOP_RESTORE)

    # Send NEW command
    print("Sending NEW command")
    send_keystrokes_to_c64(b'new')
    send_keystroke_to_c64(C64_RETURN)
    
    # Send the processed program
    print("Sending program...")
    send_keystrokes_to_c64(bytes_to_send)

    # Send RUN command
    print("Sending RUN command")
    send_keystrokes_to_c64(b'run')
    send_keystroke_to_c64(C64_RETURN)


def run_hello_world():
    load_and_run_program("hello.bas")
    time.sleep_ms(10000)


def run_maze():
    load_and_run_program("maze.bas")
    
    # Run for 6 minutes
    start_time = time.ticks_ms()
    while time.ticks_diff(time.ticks_ms(), start_time) < 360000:  # 6 minutes in ms
        # Random delay between 15-30 seconds
        delay = random.randint(15, 30)
        time.sleep_ms(delay * 1000)
        
        # Randomly choose between F1 and F3
        if random.random() < 0.5:
            print("Sending F1")
            send_keystroke_to_c64(C64_F1)
        else:
            print("Sending F3") 
            send_keystroke_to_c64(C64_F3)

def run_dancing_mouse():
    load_and_run_program("mouse.bas")
    time.sleep_ms(20000)


def run_bounce():
    load_and_run_program("bounce.bas")
    time.sleep_ms(60000)


def main():
    print("Monitoring trigger pin. Pull the pin low to send command and BASIC program.")
    while True:
        if trigger_pin.value() == 0:  # Trigger activated: pin pulled low
            # Debounce: wait briefly and confirm trigger is still low
            time.sleep_ms(50)
            if trigger_pin.value() == 0:
                try:
                    while True:
                        run_hello_world()
                        run_dancing_mouse()
                        run_maze()
                        run_bounce()
                except Exception as e:
                    print("Error running program:", e)
                    sys.print_exception(e)

                # Wait until the trigger is released to avoid multiple sends
                while trigger_pin.value() == 0:
                    time.sleep_ms(50)
        time.sleep_ms(50)

if __name__ == "__main__":
    main() 