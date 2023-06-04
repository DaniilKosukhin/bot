# bot

![bot front](https://github.com/DaniilKosukhin/bot/blob/main/photo%20materials/1.%20bot%20front.jpg)

![bot backside](https://github.com/DaniilKosukhin/bot/blob/main/photo%20materials/2.%20bot%20backside.jpg)

![joystick](https://github.com/DaniilKosukhin/bot/blob/main/photo%20materials/3%20Joystick.jpg)

## code
+ bot:
***
// Bot

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

// motor pins
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

// 0 - Forward, 1 - backward, 2 - right, 3 - left
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
***
