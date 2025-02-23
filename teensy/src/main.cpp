/*
  main.cpp
  This sketch receives Commodore ASCII codes from Serial1 (ESP32) and maps them
  to appropriate USB keyboard outputs.

  Protocol:
    - Each byte received represents a Commodore ASCII code
    - The Teensy maps these to appropriate USB HID key codes and modifiers
  
  Wiring Notes:
    - ESP32 TX (from UART2) should be connected to Teensy RX (Serial1)
    - Ensure a common ground
*/

#include <Arduino.h>
#include <Keyboard.h>

#define LED_DEBUG LED_BUILTIN

// Structure to hold USB key mapping
struct KeyMapping {
    uint16_t keyCode;    // USB HID key code (needs to be 16-bit to include 0xF000 flags)
    uint8_t modifiers;   // Modifier keys (shift, ctrl, etc)
};

// Define modifier bits
#define MOD_CTRL   0x01
#define MOD_SHIFT  0x02
#define MOD_ALT    0x04
#define MOD_GUI    0x08
#define MOD_ESC    0x10

// Commodore special key codes
#define C64_CLR_HOME   0x93    // Clear/Home CHR$(147)
#define C64_RESTORE    0xFF    // Special value for RUN/STOP + RESTORE CHR$(255)
#define C64_CRSR_UP    0x91    // Cursor Up CHR$(145)
#define C64_CRSR_DOWN  0x11    // Cursor Down CHR$(17)
#define C64_CRSR_LEFT  0x9D    // Cursor Left CHR$(157)
#define C64_CRSR_RIGHT 0x1D    // Cursor Right CHR$(29)
#define C64_RUN_STOP   0x03    // RUN/STOP CHR$(3)
#define C64_RETURN     0x0D    // RETURN CHR$(13)
#define C64_F1         0x85    // F1 CHR$(133)
#define C64_F2         0x86    // F2 CHR$(134)  
#define C64_F3         0x87    // F3 CHR$(135)
#define C64_F4         0x88    // F4 CHR$(136)
#define C64_F5         0x89    // F5 CHR$(137)
#define C64_F6         0x8A    // F6 CHR$(138)
#define C64_F7         0x8B    // F7 CHR$(139)
#define C64_F8         0x8C    // F8 CHR$(140)

#define C64_BLACK      0x90    // BLACK key CHR$(144)
#define C64_WHITE      0x05    // WHITE key CHR$(5)
#define C64_RED        0x1C    // RED key CHR$(28)
#define C64_CYAN       0x9F    // CYAN key CHR$(159)
#define C64_PURPLE     0x9C    // PURPLE key CHR$(156)
#define C64_GREEN      0x1E    // GREEN key CHR$(30)
#define C64_BLUE       0x1F    // BLUE key CHR$(31)
#define C64_YELLOW     0x9E    // YELLOW key CHR$(158)
#define C64_ORANGE     0x81    // ORANGE key CHR$(129)
#define C64_BROWN      0x95    // BROWN key CHR$(149)
#define C64_LIGHT_RED  0x96    // LIGHT RED key CHR$(150)
#define C64_DARK_GRAY  0x97    // DARK GRAY key CHR$(151)
#define C64_MEDIUM_GRAY 0x98    // MEDIUM GRAY key CHR$(152)
#define C64_LIGHT_GREEN 0x99    // LIGHT GREEN key CHR$(153)
#define C64_LIGHT_BLUE  0x9A    // LIGHT BLUE key CHR$(154)
#define C64_LIGHT_GRAY  0x9B    // LIGHT GRAY key CHR$(155)

// Map Commodore ASCII codes to USB HID codes and modifiers
KeyMapping commodoreToUSB(uint8_t cbmCode) {
    KeyMapping mapping = {0, 0};  // Default to no key, no modifiers
    
    switch(cbmCode) {
        case C64_CLR_HOME:
            mapping.keyCode = KEY_HOME;
            mapping.modifiers = MOD_SHIFT;  // SHIFT+HOME = CLEAR
            break;
            
        case C64_RESTORE:
            mapping.keyCode = KEY_F12;      // Using F12 for RESTORE
            mapping.modifiers = MOD_ESC;    // With ALT modifier
            break; 
            
        case C64_CRSR_UP:
            mapping.keyCode = KEY_UP_ARROW;
            break;
            
        case C64_CRSR_DOWN:
            mapping.keyCode = KEY_DOWN_ARROW;
            break;
            
        case C64_CRSR_LEFT:
            mapping.keyCode = KEY_LEFT_ARROW;
            break;
            
        case C64_CRSR_RIGHT:
            mapping.keyCode = KEY_RIGHT_ARROW;
            break;
            
        case C64_RUN_STOP:
            mapping.keyCode = KEY_ESC;      // Map RUN/STOP to ESC
            break;
            
        case C64_RETURN:
            mapping.keyCode = KEY_RETURN;
            break;

        case C64_F1:
            mapping.keyCode = KEY_F1;
            break;

        case C64_F2:
            mapping.keyCode = KEY_F2;
            break;

        case C64_F3:
            mapping.keyCode = KEY_F3;
            break;

        case C64_F4:
            mapping.keyCode = KEY_F4;
            break;

        case C64_F5:
            mapping.keyCode = KEY_F5;
            break;

        case C64_F6:
            mapping.keyCode = KEY_F6;
            break;  

        case C64_F7:
            mapping.keyCode = KEY_F7;
            break;

        case C64_F8:
            mapping.keyCode = KEY_F8;
            break;

        case C64_BLACK:
            mapping.keyCode = '1';
            mapping.modifiers = MOD_CTRL;
            break;

        case C64_WHITE:
            mapping.keyCode = '2';
            mapping.modifiers = MOD_CTRL;
            break;

        case C64_RED:
            mapping.keyCode = '3';
            mapping.modifiers = MOD_CTRL;
            break;

        case C64_CYAN:
            mapping.keyCode = '4';
            mapping.modifiers = MOD_CTRL;
            break;

        case C64_PURPLE:
            mapping.keyCode = '5';
            mapping.modifiers = MOD_CTRL;
            break;

        case C64_GREEN:
            mapping.keyCode = '6';
            mapping.modifiers = MOD_CTRL;
            break;

        case C64_BLUE:
            mapping.keyCode = '7';
            mapping.modifiers = MOD_CTRL;
            break;

        case C64_YELLOW:
            mapping.keyCode = '8';
            mapping.modifiers = MOD_CTRL;
            break;

        case C64_ORANGE:
            mapping.keyCode = '1';
            mapping.modifiers = MOD_CTRL;
            break;

        case C64_BROWN:
            mapping.keyCode = '2';
            mapping.modifiers = MOD_CTRL;
            break;

        case C64_LIGHT_RED:
            mapping.keyCode = '3';
            mapping.modifiers = MOD_GUI;
            break;

        case C64_DARK_GRAY:
            mapping.keyCode = '4';
            mapping.modifiers = MOD_GUI;
            break;

        case C64_MEDIUM_GRAY:
            mapping.keyCode = '5';
            mapping.modifiers = MOD_GUI;
            break;

        case C64_LIGHT_GREEN:
            mapping.keyCode = '6';
            mapping.modifiers = MOD_GUI;
            break;

        case C64_LIGHT_BLUE:
            mapping.keyCode = '7';
            mapping.modifiers = MOD_GUI;
            break;

        case C64_LIGHT_GRAY:
            mapping.keyCode = '8';
            mapping.modifiers = MOD_GUI;
            break;


        default:
            // For standard ASCII characters (assuming they match USB HID codes)
            if (cbmCode >= 32 && cbmCode <= 126) {
                mapping.keyCode = cbmCode;
            }
            break;
    }
    
    return mapping;
}

void sendKey(KeyMapping mapping) {
    // Press modifiers first
    if (mapping.modifiers & MOD_CTRL)
        Keyboard.press(KEY_LEFT_CTRL);
    if (mapping.modifiers & MOD_SHIFT)
        Keyboard.press(KEY_LEFT_SHIFT);
    if (mapping.modifiers & MOD_ALT)
        Keyboard.press(KEY_LEFT_ALT);
    if (mapping.modifiers & MOD_GUI)
        Keyboard.press(KEY_LEFT_GUI);
    if (mapping.modifiers & MOD_ESC)
        Keyboard.press(KEY_ESC);
    // Press the key if one is specified
    if (mapping.keyCode != 0) {
        Keyboard.press(mapping.keyCode);
        delay(50);  // Brief delay for key registration
    }
    
    // Release all keys
    Keyboard.releaseAll();
}

void setup() {
    Serial1.begin(9600);
    Keyboard.begin();
    pinMode(LED_DEBUG, OUTPUT);
    
    // Blink LED to indicate setup completion
    digitalWrite(LED_DEBUG, HIGH);
    delay(200);
    digitalWrite(LED_DEBUG, LOW);
}

void loop() {
    if (Serial1.available() > 0) {
        uint8_t cbmCode = Serial1.read();
        
        // Map the Commodore code to USB
        KeyMapping mapping = commodoreToUSB(cbmCode);
        
        // Send the key if valid
        if (mapping.keyCode != 0) {
            sendKey(mapping);
            
            // Blink LED to indicate key was processed
            digitalWrite(LED_DEBUG, HIGH);
            delay(50);
            digitalWrite(LED_DEBUG, LOW);
        }
    }
}