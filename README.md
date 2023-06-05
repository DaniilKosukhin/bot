# bot

Creation of a radio-controlled robot with additional elements for passing the test lane

![bot front](https://github.com/DaniilKosukhin/bot/blob/main/photo%20materials/1.%20bot%20front.jpg)

![bot backside](https://github.com/DaniilKosukhin/bot/blob/main/photo%20materials/2.%20bot%20backside.jpg)

![joystick](https://github.com/DaniilKosukhin/bot/blob/main/photo%20materials/3%20Joystick.jpg)

![joystick](https://github.com/DaniilKosukhin/bot/blob/main/photo%20materials/moving.mp4)

### componenets:
+ 3D jostick
+ amperka iskra NEO 2x
+ NRF
+ NRF +
+ 5v to 3.3 v
+ potentiometer
+ powerbank
+ servo 360
+ RGB LED matrix
+ battery holder
+ Motor Shield
+ DC motor 2x

### bot code:

```Python
#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <Servo.h>
#include <Adafruit_NeoPixel.h>

#define SPEED_1      5 
#define DIR_1        4
 
#define SPEED_2      6
#define DIR_2        7

#define LED_COUNT 3
#define MATRIX_PIN 12

Adafruit_NeoPixel matrix = Adafruit_NeoPixel(LED_COUNT, MATRIX_PIN, NEO_GRB + NEO_KHZ800);

const uint64_t pipe = 0xF0F1F2F3F4LL;

RF24 radio(9, 10);

int data[5];

Servo rotServo; 
int servoAngle;

void setup(){
 Serial.begin(9600);
 radio.begin(); 
 radio.setChannel(3);
 radio.setDataRate(RF24_1MBPS);
 radio.setPALevel (RF24_PA_HIGH); 
 radio.openReadingPipe(1, pipe); 
 radio.startListening();
 
  for (int i = 4; i < 8; i++) {     
    pinMode(i, OUTPUT);
  } 

  rotServo.attach(2);
  matrix.begin();
} 


void loop() {

 if (radio.available()) {

    radio.read(&data, sizeof(data));

    int ptmrVal = data[0];
    int btnState = data[1];

    servoAngle = map(ptmrVal, 0, 1023, 0, 180); 
    rotServo.write(servoAngle); 
    Serial.println(ptmrVal);

      if (btnState == 0) {
        for (int i = 0; i < LED_COUNT; i++) {
          matrix.setPixelColor(i, matrix.Color(255,0,100));
        }
        matrix.show();
      }
      else {
        for (int i = 0; i < LED_COUNT; i++) {
          matrix.setPixelColor(i, matrix.Color(0,0,0));
        }
        matrix.show();
      }
      
    switch (data[2]) {
      case 0: {
        digitalWrite(DIR_1, HIGH); // set direction
        analogWrite(SPEED_1, data[3]); // set speed

        digitalWrite(DIR_2, HIGH); 
        analogWrite(SPEED_2, data[3]); 

        break;
      }
      case 1: {
            digitalWrite(DIR_1, LOW);
            analogWrite(SPEED_1, data[3]); 

            digitalWrite(DIR_2, LOW);
            analogWrite(SPEED_2, data[3]); 

            break;
          }
        case 2: {
          digitalWrite(DIR_1, LOW);
          analogWrite(SPEED_1, data[3]); 

          digitalWrite(DIR_2, HIGH);
          analogWrite(SPEED_2, data[3]); 

          break;
        }
        case 3: {
          digitalWrite(DIR_1, HIGH);
          analogWrite(SPEED_1, data[3]); 

          digitalWrite(DIR_2, LOW);
          analogWrite(SPEED_2, data[3]); 

          break;
        }
        case 4: {
          digitalWrite(DIR_1, LOW);
          analogWrite(SPEED_1, 0); 

          digitalWrite(DIR_2, LOW);
          analogWrite(SPEED_2, 0); 

          break;
        }
    }
  }
}
```

### jostick code:

```Python
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
```
