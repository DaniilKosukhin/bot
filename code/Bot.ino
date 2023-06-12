// Joystick

#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

const uint64_t pipe = 0xF0F1F2F3F4LL;

RF24 radio(9, 10);

int data[5];

const int ptmrPin = A5;
const int buttonPin = 7;
const int xPin = A0;
const int yPin = A1;

int ptmrState = 0;
int buttonState = 0;
int xPosition = 0;
int yPosition = 0;

int speed = 0;
int direction = 0;

void setup(){
 Serial.begin(9600);

 radio.begin(); 
 radio.setChannel(3);
 radio.setDataRate(RF24_1MBPS);
 radio.setPALevel (RF24_PA_HIGH); 
 radio.openWritingPipe(pipe); 
 radio.stopListening(); 

 pinMode(ptmrPin, INPUT);
 pinMode(buttonPin, INPUT_PULLUP);
 pinMode(xPin, INPUT);
 pinMode(yPin, INPUT);
} 


void loop() {

 ptmrState = analogRead(ptmrPin);
 buttonState = digitalRead(buttonPin);
 xPosition = analogRead(xPin);
 yPosition = analogRead(yPin);

  if (yPosition < 400) {
        // go forward
        direction = 0; // 0 - Forward, 1 - backward, 2 - right, 3 - left
        speed = map(yPosition, 400, 10, 5, 255);
      }
      else if (yPosition > 600) {
        // go backward
        direction = 1;
        speed = map(yPosition, 600, 950, 5, 255);
      }
      else if (xPosition < 400) {
        // go right
        direction = 2; // 0 - Forward, 1 - backward, 2 - right, 3 - left
        speed = map(xPosition, 400, 0, 5, 255);
      }
      else if (xPosition > 600) {
      // go left
        direction = 3; // 0 - Forward, 1 - backward, 2 - right, 3 - left
        speed = map(xPosition, 600, 1000, 5, 255);
      }
      else if (yPosition < 600 || yPosition > 400  ) {
        // stop
        direction = 4;
        speed = 0;
      }


 data[0] = ptmrState;
 data[1] = !buttonState;
 data[2] = direction;
 data[3] = speed;

    radio.write(&data, sizeof(data));

    Serial.println(ptmrState); Serial.print(!buttonState);

    delay(100);

}


