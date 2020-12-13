#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__
 
int freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}

#include "HID-Project.h"

byte rows[] = {2,3,4,5,6,7,8};
const int rowCount = sizeof(rows)/sizeof(rows[0]);
 
byte cols[] = {15, A0, A1, A2, A3};
const int colCount = sizeof(cols)/sizeof(cols[0]);

bool keys[colCount][rowCount];
bool lastValue[colCount][rowCount];
bool changedValue[colCount][rowCount];

unsigned int keyboard[colCount][rowCount];
unsigned int keyboard_symbol[colCount][rowCount];
// uint8_t keyboard_alt[colCount][rowCount];


bool symbolSelected;
bool altSelected;
348348354354354354356356356356
void setup() {
    Serial.begin(115200);
    Keyboard.begin();
    delay(5000);
    keyboard[0][0] = "q";
    keyboard[0][1] = "w";
    keyboard[0][2] = NULL; // symbol
    keyboard[0][3] = "a";
    keyboard[0][4] = NULL; // ALT
    keyboard[0][5] = " ";
    keyboard[0][6] = NULL; // Mic
    
    keyboard[1][0] = "e";
    keyboard[1][1] = "s";
    keyboard[1][2] = "d";
    keyboard[1][3] = "p";
    keyboard[1][4] = "x";
    keyboard[1][5] = "y";
    keyboard[1][6] = NULL; // Left Shift
    
    keyboard[2][0] = "r";
    keyboard[2][1] = "g";
    keyboard[2][2] = "t";
    keyboard[2][3] = NULL; // Right Shit
    keyboard[2][4] = "t";
    keyboard[2][5] = "c";
    keyboard[2][6] = "f";
    
    keyboard[3][0] = "u";
    keyboard[3][1] = "h";
    keyboard[3][2] = "z";
    keyboard[3][3] = NULL; // Enter
    keyboard[3][4] = "b";
    keyboard[3][5] = "n";
    keyboard[3][6] = "j";
    
    keyboard[4][0] = "o";
    keyboard[4][1] = "l";
    keyboard[4][2] = "i";
    keyboard[4][3] = NULL; // Backspace
    keyboard[4][4] = NULL; // Dollar used as Ctrl
    keyboard[4][5] = "m";
    keyboard[4][6] = "k";
    Serial.println(freeMemory());
    keyboard_symbol[0][0] = "#";
    keyboard_symbol[0][1] = "1";
    keyboard_symbol[0][2] = NULL;
    keyboard_symbol[0][3] = "}";
    keyboard_symbol[0][4] = NULL;
    keyboard_symbol[0][5] = NULL;
    keyboard_symbol[0][6] = "0";
    
    keyboard_symbol[1][0] = "2";
    keyboard_symbol[1][1] = "4";
    keyboard_symbol[1][2] = "5";
    keyboard_symbol[1][3] = "@";
    keyboard_symbol[1][4] = "8";
    keyboard_symbol[1][5] = "7";
    keyboard_symbol[1][6] = NULL;
    
    keyboard_symbol[2][0] = "3";
    keyboard_symbol[2][1] = "/";
    keyboard_symbol[2][2] = "(";
    keyboard_symbol[2][3] = NULL;
    keyboard_symbol[2][4] = "_";
    keyboard_symbol[2][5] = "9";
    keyboard_symbol[2][6] = "6";
    
    keyboard_symbol[3][0] = (int)'?' << 8;
    keyboard_symbol[3][1] = (int)'>' << 8;
    keyboard_symbol[3][2] = (int)'(' << 8;
    keyboard_symbol[3][3] = NULL;
    keyboard_symbol[3][4] = (int)'!' << 8;
    keyboard_symbol[3][5] = (int)',' << 8;
    keyboard_symbol[3][6] = (int)'<' << 8;
    
    keyboard_symbol[4][0] = (int)']' << 8;
    keyboard_symbol[4][1] = (int)'@' << 8;
    keyboard_symbol[4][2] = (int)'/' << 8;
    keyboard_symbol[4][3] = NULL;
    keyboard_symbol[4][4] = (int)"-" << 8;
    keyboard_symbol[4][5] = (int)'.' << 8;
    keyboard_symbol[4][6] = (int)'|' << 8;

    // keyboard_alt[0][0] = NULL;
    // keyboard_alt[0][1] = KEY_F1;
    // keyboard_alt[0][2] = NULL;
    // keyboard_alt[0][3] = NULL;
    // keyboard_alt[0][4] = NULL;
    // keyboard_alt[0][5] = NULL;
    // keyboard_alt[0][6] = KEY_F10;
    
    // keyboard_alt[1][0] = KEY_F2;
    // keyboard_alt[1][1] = KEY_F4;
    // keyboard_alt[1][2] = KEY_F5;
    // keyboard_alt[1][3] = KEY_LEFT_GUI;
    // keyboard_alt[1][4] = KEY_F8;
    // keyboard_alt[1][5] = KEY_F7;
    // keyboard_alt[1][6] = NULL;
    
    // keyboard_alt[2][0] = KEY_F3;
    // keyboard_alt[2][1] = NULL;
    // keyboard_alt[2][2] = KEY_TAB;
    // keyboard_alt[2][3] = NULL;
    // keyboard_alt[2][4] = NULL;
    // keyboard_alt[2][5] = KEY_F9;
    // keyboard_alt[2][6] = KEY_F6;
    
    // keyboard_alt[3][0] = NULL;
    // keyboard_alt[3][1] = NULL;
    // keyboard_alt[3][2] = NULL;
    // keyboard_alt[3][3] = NULL;
    // keyboard_alt[3][4] = NULL;
    // keyboard_alt[3][5] = NULL;
    // keyboard_alt[3][6] = KEY_LEFT_ARROW;
    // keyboard_alt[4][0] = NULL;
    // keyboard_alt[4][1] = KEY_RIGHT_ARROW;
    // keyboard_alt[4][2] = KEY_UP_ARROW;
    // keyboard_alt[4][3] = NULL;
    // keyboard_alt[4][4] = NULL;
    // keyboard_alt[4][5] = NULL;
    // keyboard_alt[4][6] = KEY_DOWN_ARROW;
  
 
    for(int x=0; x<rowCount; x++) {
        Serial.print(rows[x]); Serial.println(" as input");
        pinMode(rows[x], INPUT);
    }
 
    for (int x=0; x<colCount; x++) {
        Serial.print(cols[x]); Serial.println(" as input-pullup");
        pinMode(cols[x], INPUT_PULLUP);
    }

    symbolSelected = false;
    altSelected = false;
}

void readMatrix() {
    int delayTime = 0;
    // iterate the columns
    for (int colIndex=0; colIndex < colCount; colIndex++) {
        // col: set to output to low
        byte curCol = cols[colIndex];
        pinMode(curCol, OUTPUT);
        digitalWrite(curCol, LOW);
 
        // row: interate through the rows
        for (int rowIndex=0; rowIndex < rowCount; rowIndex++) {
            byte rowCol = rows[rowIndex];
            pinMode(rowCol, INPUT_PULLUP);
            delay(1); // arduino is not fast enought to switch input/output modes so wait 1 ms

            bool buttonPressed = (digitalRead(rowCol) == LOW);
            
            keys[colIndex][rowIndex] = buttonPressed;
            if ((lastValue[colIndex][rowIndex] != buttonPressed)) {
              changedValue[colIndex][rowIndex] = true;
            } else {
              changedValue[colIndex][rowIndex] = false;
            }

            lastValue[colIndex][rowIndex] = buttonPressed;
            pinMode(rowCol, INPUT);
        }
        // disable the column
        pinMode(curCol, INPUT);
    }

    if (keyPressed(0, 2)) {
      symbolSelected = true;
    }
    if (keyPressed(0, 4)) {
      altSelected = true;
    }
}

bool keyPressed(int colIndex, int rowIndex) {
  return changedValue[colIndex][rowIndex] && keys[colIndex][rowIndex] == true;
}

bool keyActive(int colIndex, int rowIndex) {
  return keys[colIndex][rowIndex] == true;
}

bool isPrintableKey(int colIndex, int rowIndex) {
  return keyboard_symbol[colIndex][rowIndex] != NULL || keyboard[colIndex][rowIndex] != NULL;
}

void printMatrix() {
    for (int rowIndex=0; rowIndex < rowCount; rowIndex++) {
        for (int colIndex=0; colIndex < colCount; colIndex++) {
          // we only want to print if the key is pressed and it is a printable character
          if (keyPressed(colIndex, rowIndex) && isPrintableKey(colIndex, rowIndex)) {
            
            if (keyActive(0, 4)) {
              // Keyboard.press(KeyboardKeycode(keyboard_alt[colIndex][rowIndex]));
              // delay(10);
              // Keyboard.releaseAll();
            } else {
              unsigned int toPrint;
              if (symbolSelected) {
                symbolSelected = false;
                toPrint = keyboard_symbol[colIndex][rowIndex];
              } else {
                toPrint = keyboard[colIndex][rowIndex];
              }
              // keys 1,6 and 2,3 are Shift keys, so we want to upper case
              // if (keyActive(1,6) || keyActive(2,3)) {
              //   toPrint.toUpperCase();
              // }
              
              // if(toPrint <= 255) {
                Serial.println(KeyboardKeycode((int)"g"));
                Serial.println(KeyboardKeycode(keyboard[colIndex][rowIndex]));
                Keyboard.print(String(keyboard[colIndex][rowIndex]));
                // delay(10);
                // Keyboard.releaseAll();
              // } else {
                // Serial.println(toPrint >> 8);
                // Serial.println( (int)'@');
                // Keyboard.print((char)(toPrint >> 8));
              // }
            }
          }
        }   
    }
}

 
void loop() {
    readMatrix();
    
    printMatrix();

    // key 3,3 is the enter key
    if (keyPressed(3,3)) {
      Keyboard.write(KEY_ENTER);
    }

    // key 4,3 is the backspace key
    if (keyPressed(4,3)) {
      Keyboard.write(KEY_BACKSPACE);
    }

    // increase backlight if symbol key is pressed
    // if (keyPressed(0,2)) {
    //   changeBackgroundLight(keyboardLight + keyboardLightSteps);
    // }

    // // decrease backlight if right shift key is pressed
    // if (keyPressed(2,3)) {
    //   changeBackgroundLight(keyboardLight - keyboardLightSteps);
    // }
    delay(10);
}