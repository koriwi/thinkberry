#include "HID-Project.h"

byte rows[] = {2, 3, 4, 5, 6, 7, 8};
const int rowCount = sizeof(rows) / sizeof(rows[0]);

byte cols[] = {15, A0, A1, A2, A3};
const int colCount = sizeof(cols) / sizeof(cols[0]);

bool keys[colCount][rowCount];
bool lastValue[colCount][rowCount];
bool changedValue[colCount][rowCount];

KeyboardKeycode memory[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

uint8_t memoryCounter = 0;
uint8_t lastMemoryCounter = 0;

uint16_t keyboard[colCount][rowCount];
uint16_t keyboard_symbol[colCount][rowCount];
uint16_t keyboard_alt[colCount][rowCount];

// symbol and alt are used to send alt and ctrl
// but also to access 2nd and 3d keyboard level
// if one was used to access a keyboard level, dont send 
// keypress on release
bool symbolSelected;
bool altSelected;

bool comboModeActive;

void setup() {
	Serial.begin(115200);
	Keyboard.begin();
	keyboard[0][0] = KEY_Q;
	keyboard[0][1] = KEY_W;
	keyboard[0][2] = NULL;	// symbol
	keyboard[0][3] = KEY_A;
	keyboard[0][4] = NULL;	// ALT
	keyboard[0][5] = KEY_SPACE;
	keyboard[0][6] = KEY_SMALLER;	// Mic

	keyboard[1][0] = KEY_E;
	keyboard[1][1] = KEY_S;
	keyboard[1][2] = KEY_D;
	keyboard[1][3] = KEY_P;
	keyboard[1][4] = KEY_X;
	keyboard[1][5] = KEY_Y;
	keyboard[1][6] = NULL;	// Left Shift

	keyboard[2][0] = KEY_R;
	keyboard[2][1] = KEY_G;
	keyboard[2][2] = KEY_T;
	keyboard[2][3] = NULL;	// Right Shift
	keyboard[2][4] = KEY_V;
	keyboard[2][5] = KEY_C;
	keyboard[2][6] = KEY_F;

	keyboard[3][0] = KEY_U;
	keyboard[3][1] = KEY_H;
	keyboard[3][2] = KEY_Z;
	keyboard[3][3] = KEY_ENTER;	// Enter
	keyboard[3][4] = KEY_B;
	keyboard[3][5] = KEY_N;
	keyboard[3][6] = KEY_J;

	keyboard[4][0] = KEY_O;
	keyboard[4][1] = KEY_L;
	keyboard[4][2] = KEY_I;
	keyboard[4][3] = KEY_BACKSPACE;	// Backspace
	keyboard[4][4] = NULL;
	keyboard[4][5] = KEY_M;
	keyboard[4][6] = KEY_K;

	keyboard_symbol[0][0] = KEY_HASHTAG;
	keyboard_symbol[0][1] = KEY_1;
	keyboard_symbol[0][2] = NULL;
	keyboard_symbol[0][3] = KEY_PLUS | MOD_LEFT_SHIFT;
	keyboard_symbol[0][4] = NULL;
	keyboard_symbol[0][5] = KEY_TAB;
	keyboard_symbol[0][6] = KEY_0;

	keyboard_symbol[1][0] = KEY_2;
	keyboard_symbol[1][1] = KEY_4;
	keyboard_symbol[1][2] = KEY_5;
	keyboard_symbol[1][3] = KEY_Q | MOD_RIGHT_ALT;
	keyboard_symbol[1][4] = KEY_8;
	keyboard_symbol[1][5] = KEY_7;
	keyboard_symbol[1][6] = NULL;

	keyboard_symbol[2][0] = KEY_3;
	keyboard_symbol[2][1] = KEY_7 | MOD_LEFT_SHIFT;
	keyboard_symbol[2][2] = KEY_8 | MOD_LEFT_SHIFT;
	keyboard_symbol[2][3] = NULL;
	keyboard_symbol[2][4] = KEY_SZ | MOD_LEFT_SHIFT;
	keyboard_symbol[2][5] = KEY_9;
	keyboard_symbol[2][6] = KEY_6;

	keyboard_symbol[3][0] = KEY_MINUS | MOD_LEFT_SHIFT;
	keyboard_symbol[3][1] = KEY_PERIOD | MOD_LEFT_SHIFT;
	keyboard_symbol[3][2] = KEY_9 | MOD_LEFT_SHIFT;
	keyboard_symbol[3][3] = KEY_PAGE_DOWN;
	keyboard_symbol[3][4] = KEY_1 | MOD_LEFT_SHIFT;
	keyboard_symbol[3][5] = KEY_COMMA;
	keyboard_symbol[3][6] = KEY_COMMA | MOD_LEFT_SHIFT;

	keyboard_symbol[4][0] = KEY_PLUS;
	keyboard_symbol[4][1] = KEY_2 | MOD_LEFT_SHIFT;
	keyboard_symbol[4][2] = KEY_MINUS;
	keyboard_symbol[4][3] = KEY_PAGE_UP;
	keyboard_symbol[4][4] = KEY_E | MOD_RIGHT_ALT;
	keyboard_symbol[4][5] = KEY_PERIOD;
	keyboard_symbol[4][6] = KEY_HASHTAG | MOD_LEFT_SHIFT;

	keyboard_alt[0][0] = KEY_ESC;
	keyboard_alt[0][1] = KEY_F1;
	keyboard_alt[0][2] = NULL;
	keyboard_alt[0][3] = NULL;
	keyboard_alt[0][4] = NULL;
	keyboard_alt[0][5] = KEY_LEFT_GUI;
	keyboard_alt[0][6] = KEY_SMALLER | MOD_RIGHT_ALT;

	keyboard_alt[1][0] = KEY_F2;
	keyboard_alt[1][1] = KEY_F4;
	keyboard_alt[1][2] = KEY_F5;
	keyboard_alt[1][3] = KEY_PLUS | MOD_RIGHT_ALT;
	keyboard_alt[1][4] = KEY_F8;
	keyboard_alt[1][5] = KEY_F7;
	keyboard_alt[1][6] = NULL;

	keyboard_alt[2][0] = KEY_F3;
	keyboard_alt[2][1] = KEY_8 | MOD_RIGHT_ALT;
	keyboard_alt[2][2] = KEY_7 | MOD_RIGHT_ALT;
	keyboard_alt[2][3] = NULL;
	keyboard_alt[2][4] = KEY_TAB;
	keyboard_alt[2][5] = KEY_F9;
	keyboard_alt[2][6] = KEY_F6;

	keyboard_alt[3][0] = KEY_SZ | MOD_RIGHT_ALT;
	keyboard_alt[3][1] = KEY_9 | MOD_RIGHT_ALT;
	keyboard_alt[3][2] = KEY_0 | MOD_RIGHT_ALT;
	keyboard_alt[3][3] = KEY_ESC;
	keyboard_alt[3][4] = KEY_TAB | MOD_LEFT_SHIFT;
	keyboard_alt[3][5] = NULL;
	keyboard_alt[3][6] = KEY_LEFT_ARROW;

	keyboard_alt[4][0] = KEY_0 | MOD_LEFT_SHIFT;
	keyboard_alt[4][1] = KEY_RIGHT_ARROW;
	keyboard_alt[4][2] = KEY_UP_ARROW;
	keyboard_alt[4][3] = KEY_DELETE;
	keyboard_alt[4][4] = KEY_4 | MOD_RIGHT_SHIFT;
	keyboard_alt[4][5] = NULL;
	keyboard_alt[4][6] = KEY_DOWN_ARROW;

	for (int x = 0; x < rowCount; x++) {
		Serial.print(rows[x]);
		Serial.println(' as input');
		pinMode(rows[x], INPUT);
	}

	for (int x = 0; x < colCount; x++) {
		Serial.print(cols[x]);
		Serial.println(' as input-pullup');
		pinMode(cols[x], INPUT_PULLUP);
	}

	symbolSelected = false;
	altSelected = false;
	comboModeActive = false;

	pinMode(17, OUTPUT);
	digitalWrite(17, HIGH);
}

void readMatrix() {
	int delayTime = 0;
	// iterate the columns
	for (int colIndex = 0; colIndex < colCount; colIndex++) {
		// col: set to output to low
		byte curCol = cols[colIndex];
		pinMode(curCol, OUTPUT);
		digitalWrite(curCol, LOW);

		// row: interate through the rows
		for (int rowIndex = 0; rowIndex < rowCount; rowIndex++) {
			byte rowCol = rows[rowIndex];
			pinMode(rowCol, INPUT_PULLUP);
			delay(1);  // arduino is not fast enought to switch input/output
					   // modes so wait 1 ms

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
}

bool keyPressed(int colIndex, int rowIndex) {
	return changedValue[colIndex][rowIndex] && keys[colIndex][rowIndex] == true;
}

bool keyReleased(int colIndex, int rowIndex) {
	return changedValue[colIndex][rowIndex] &&
		   keys[colIndex][rowIndex] == false;
}

bool keyActive(int colIndex, int rowIndex) {
	return keys[colIndex][rowIndex] == true;
}

bool checkKey(int colIndex, int rowIndex) {
	if (!comboModeActive)
		return keyPressed(colIndex, rowIndex);
	else
		return keyReleased(colIndex, rowIndex);
}

bool isPrintableKey(int colIndex, int rowIndex) {
	return (keyActive(0, 2) && keyboard_symbol[colIndex][rowIndex] != NULL) ||
		   keyboard[colIndex][rowIndex] != NULL ||
		   (keyActive(0, 4) && keyboard_alt[colIndex][rowIndex] != NULL);
}

void pressKey(KeyboardKeycode k) {
	if (comboModeActive) {
		if (lastMemoryCounter > 0) lastMemoryCounter = 0;
		memory[memoryCounter++] = k;
	} else {
		Keyboard.press(k);
	}
}

void releaseAll() {
	if (!comboModeActive) {
		Keyboard.releaseAll();
	}
}

void pressMemory() {
	for (uint8_t i = 0; i < memoryCounter; i++) {
		Keyboard.press(memory[i]);
		delay(5);
	}
	for (uint8_t i = 0; i < memoryCounter; i++) {
		Keyboard.release(memory[i]);
		delay(5);
	}
	Keyboard.releaseAll();
}

void printMatrix() {
	for (int rowIndex = 0; rowIndex < rowCount; rowIndex++) {
		for (int colIndex = 0; colIndex < colCount; colIndex++) {
			// we only want to print if the key is pressed and it is a printable
			// character
			if (isPrintableKey(colIndex, rowIndex)) {
				if ((comboModeActive && keyReleased(colIndex, rowIndex)) ||
					!comboModeActive && keyPressed(colIndex, rowIndex)) {
					uint16_t input;
					if (keyActive(0, 4)) {
						input = keyboard_alt[colIndex][rowIndex];
						altSelected = true;
					} else if (keyActive(1, 6) || keyActive(2, 3)) {
						pressKey(KeyboardKeycode(KEY_LEFT_SHIFT));
						input = keyboard[colIndex][rowIndex];
					} else if (keyActive(0, 2)) {
						input = keyboard_symbol[colIndex][rowIndex];
						symbolSelected = true;
					} else {
						input = keyboard[colIndex][rowIndex];
					}

					if (input >= MOD_RIGHT_GUI) {
						input -= MOD_RIGHT_GUI;
						pressKey(KeyboardKeycode(KEY_RIGHT_GUI));
					}
					if (input >= MOD_RIGHT_ALT) {
						input -= MOD_RIGHT_ALT;
						Serial.println("RIGHT ALT");
						pressKey(KeyboardKeycode(KEY_RIGHT_ALT));
					}
					if (input >= MOD_RIGHT_SHIFT) {
						input -= MOD_RIGHT_SHIFT;
						pressKey(KeyboardKeycode(KEY_RIGHT_SHIFT));
					}
					if (input >= MOD_RIGHT_CTRL) {
						input -= MOD_RIGHT_CTRL;
						pressKey(KeyboardKeycode(KEY_RIGHT_CTRL));
					}
					if (input >= MOD_LEFT_GUI) {
						input -= MOD_LEFT_GUI;
						pressKey(KeyboardKeycode(KEY_LEFT_GUI));
					}
					if (input >= MOD_LEFT_ALT) {
						input -= MOD_LEFT_ALT;
						pressKey(KeyboardKeycode(KEY_LEFT_ALT));
					}
					if (input >= MOD_LEFT_SHIFT) {
						input -= MOD_LEFT_SHIFT;
						pressKey(KeyboardKeycode(KEY_LEFT_SHIFT));
					}
					if (input >= MOD_LEFT_CTRL) {
						input -= MOD_LEFT_CTRL;
						pressKey(KeyboardKeycode(KEY_LEFT_CTRL));
					}
					pressKey(KeyboardKeycode(input));
					delay(10);
					releaseAll();
				}
			} else {
				if (keyReleased(0, 2) && colIndex == 0 && rowIndex == 2) {
					if (!symbolSelected)
						pressKey(KeyboardKeycode(KEY_LEFT_CTRL));
					else {
						symbolSelected = false;
					}
				}
				if (keyReleased(0, 4) && colIndex == 0 && rowIndex == 4) {
					if (!altSelected) {
						pressKey(KeyboardKeycode(KEY_LEFT_ALT));
					} else {
						altSelected = false;
					}
				}
				if (keyReleased(1, 6) && colIndex == 1 && rowIndex == 6) {
					pressKey(KeyboardKeycode(KEY_LEFT_SHIFT));
				}
				if (keyReleased(2, 3) && colIndex == 2 && rowIndex == 3) {
					pressKey(KeyboardKeycode(KEY_RIGHT_SHIFT));
				}
				releaseAll();
				if (keyPressed(4, 4) && colIndex == 4 && rowIndex == 4) {
					if (keyActive(1, 6) || keyActive(2, 3)) {
						if (!comboModeActive) {
							pressMemory();
						}
					} else {
						comboModeActive = !comboModeActive;
						digitalWrite(17, comboModeActive);
						if (!comboModeActive) {
							pressMemory();
						} else {
							memoryCounter = 0;
						}
					}
				}
			}
		}
	}
}

void loop() {
	readMatrix();

	printMatrix();
	delay(10);
}