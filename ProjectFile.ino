#include "LedControl.h"
void bargraph(short int *bars);
/*
   We use pins 12,11 and 10 on the Arduino for the SPI interface
   Pin 12 is connected to the DATA IN-pin of the first MAX7219
   Pin 11 is connected to the CLK-pin of the first MAX7219
   Pin 10 is connected to the LOAD(/CS)-pin of the first MAX7219
*/
LedControl mat = LedControl(12, 11, 10, 1);
short int bars[8] = {0};

void setup(){
    mat.shutdown(0, false);
    mat.clearDisplay(0);
    mat.setIntensity(0, 8); // 0-16 intensity
    Serial.begin(9600);
}

void loop(){
    if (Serial.available()) { //See if data is there
        String in = Serial.readStringUntil('\n');
        for (int i = 0; i < 8; i++) {
            bars[i] = in.charAt(i);
        }
    }
    bargraph(bars);
    while (Serial.available() == 0) {
    };
}

// Lights appropriate bars in 8x8 matrix 0=off 1->1bar ...
// The height of each bar is stored in the bar array.
void bargraph(short int *bars){
    int i, j;
    for (i = 0; i < 8; i++) { //iterate over rows
        byte row_states = 0;
        for (j = 0; j < 8; j++) { //iterate over all columns
            if (bars[j] >= i + 1) {
                row_states |= (0x80 >> j);
            }
        }
        mat.setRow(0, 7 - i, row_states);
    }
}
