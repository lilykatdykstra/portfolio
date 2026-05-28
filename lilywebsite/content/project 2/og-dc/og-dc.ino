#include <math.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>  // Required for 16 MHz Adafruit Trinket
#include <IRremote.h>

#endif


int digit_pin[] = {6, 9, 10, 11}; // PWM Display  digit pins from left to right
int speakerPin = 15;
const int hot = 80; //set hot parameter, change for oven
const int TEMP_PIN = A2;
const int HOT_PIN = 18;

#define PIN_NEO_PIXEL 25  // Arduino pin that connects to NeoPixel
#define NUM_PIXELS 14    // The number of LEDs (pixels) on NeoPixel

#define DELAY_INTERVAL 250  // 250ms pause between each pixel

#define DIGIT_ON  LOW
#define DIGIT_OFF  HIGH

Adafruit_NeoPixel NeoPixel(NUM_PIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);
const int RECV_PIN = 12;
IRrecv irrecv(RECV_PIN);
decode_results results;
unsigned long keyValue = 0;

int segA = 2; 
int segB = 3; 
int segC = 4; 
int segD = 5; 
int segE = A0; //pin 6 is used bij display 1 for  its pwm function
int segF = 7; 
int segG = 8; 
//int segPD = ; 

int  countdown_time = 3600;

unsigned long timeCountdownEnded;

// four digits in our display
struct struct_digits {
  int digit[4];
};


void  setup() {    
  irrecv.enableIRIn();
  irrecv.blink13(true);

  NeoPixel.begin();  // INITIALIZE NeoPixel strip object (REQUIRED)

  pinMode(segA, OUTPUT);
  pinMode(segB, OUTPUT);
  pinMode(segC, OUTPUT);
  pinMode(segD, OUTPUT);
  pinMode(segE, OUTPUT);
  pinMode(segF, OUTPUT);
  pinMode(segG, OUTPUT);


  pinMode(HOT_PIN, OUTPUT); //red
  pinMode(TEMP_PIN, INPUT);

  Serial.begin(9600);

  for (int i=0; i<4;  i++) {
    pinMode(digit_pin[i], OUTPUT);
  }

  pinMode(speakerPin,  OUTPUT);

}

// the buzzer once timer ends
void  playTone(int tone, int duration) {
  for (long k = 0; k < duration * 1000L; k  += tone * 2) {  
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(tone);
  }
}


void  lightNumber(int numberToDisplay) {

#define SEGMENT_ON  HIGH
#define SEGMENT_OFF  LOW

  switch (numberToDisplay){

  case 0:
    digitalWrite(segA,  SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF,  SEGMENT_ON);
    digitalWrite(segG, SEGMENT_OFF);
    break;

  case  1:
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_OFF);
    break;

  case 2:
    digitalWrite(segA,  SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_OFF);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF,  SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case  3:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE,  SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG,  SEGMENT_ON);
    break;

  case 4:
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD,  SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF,  SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case  5:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE,  SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 6:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB,  SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG,  SEGMENT_ON);
    break;

  case 7:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD,  SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF,  SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_OFF);
    break;

  case  8:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE,  SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 9:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB,  SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 10:
    digitalWrite(segA,  SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC,  SEGMENT_OFF);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE,  SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG,  SEGMENT_OFF);
    break;  
  }
 
}



void SwitchDigit(int digit) {
  for (int i=0; i<4; i++) {
    if (i == digit) {
      digitalWrite(digit_pin[i],  DIGIT_ON);
    } else {
      digitalWrite(digit_pin[i], DIGIT_OFF);
    }
  }
}

// the values of the numbers for minutes, minutes, seconds, seconds
struct struct_digits IntToDigits(int n){
  struct struct_digits dig;
  int minutes = n / 60;
  int seconds = n % 60;
  dig.digit[0] = minutes / 10;
  dig.digit[1] = minutes % 10;
  dig.digit[2] = seconds / 10;
  dig.digit[3] = seconds % 10;

  return dig;
}

// controls display and changing numbers
void PrintNumber(int n, int time) { 
  struct struct_digits dig; 

  dig = IntToDigits(n);
  
  for (int i=0; i<= time/20; i++) {
    if (keyValue == 0xFFA25D) {
      return;
    }
    for (int j=0;  j<4; j++) {
      SwitchDigit(j);
      lightNumber(dig.digit[j]);
      delay(5);
    }
  }
}

// countdown display and controls
bool Countdown(int n, int del){
  for (int q=n;  q>0; q--){
    PrintNumber(q,del);
    delay(20); // change to minutes
    float tempC = 100*((analogRead(TEMP_PIN)*5.0 / 1024)-0.5);
    float tempF = (tempC * 1.8) + 32;
    Serial.print("temp: ");
    Serial.print(tempF);

    if ((keyValue == 0xFFE21D)==LOW) {
      return false;
    }
  }

  PrintNumber(0,0);

  timeCountdownEnded = millis();
  //turn off LEDS
  playTone(1519,1000);
  while((unsigned long)(millis() - timeCountdownEnded < 10000)) {
    if(keyValue == 0xFFE21D) {
      return false;
    }
  }
  float tempC = 100*((analogRead(TEMP_PIN)*5.0 / 1024)-0.5);
  float tempF = (tempC * 1.8) + 32;
  if (tempF >= hot) { //hot
    digitalWrite(HOT_PIN, HIGH);
    Serial.println(" It's Hot.");
    playTone(1519,1000);
    digitalWrite(HOT_PIN, LOW);
  }
  return true;
}

void setTimer() {
  int pressedButton = 0;
  int newTime = countdown_time;

  struct struct_digits dig;

  while (pressedButton != 0xFFC23D) {
    dig = IntToDigits(newTime);

    for (int j = 0; j < 4; j++) {
      SwitchDigit(j);
      lightNumber(dig.digit[j]);
      delay(5);
    }

    if (irrecv.decode(&results)) {
      keyValue = results.value;
      irrecv.resume();  // Receive the next value

      switch (keyValue) {
        case 0xFF6897:  // Button 0 on the remote
        case 0xFF30CF:  // Button 1 on the remote
        case 0xFF18E7:  // Button 2 on the remote
        case 0xFF7A85:  // Button 3 on the remote
        case 0xFF10EF:  // Button 4 on the remote
        case 0xFF38C7:  // Button 5 on the remote
        case 0xFF5AA5:  // Button 6 on the remote
        case 0xFF42BD:  // Button 7 on the remote
        case 0xFF4AB5:  // Button 8 on the remote
        case 0xFF52AD:  // Button 9 on the remote

          // Calculate the new time based on the button pressed
          int digitValue = keyValue - 0xFF6897;  // Adjust the base value
          newTime = newTime * 10 + digitValue;
          break;

        case 0xFF02FD:  // OK or Enter button on the remote
          // Exit the loop when OK/Enter is pressed
          pressedButton = keyValue;
          break;
      }
    }
  }



// button controls
void reset() {
  int m, zeros, d, pressed3 = 0, pressed4 = 0, pressed5 = 0, pressed6 = 0;
  m=countdown_time;

  struct struct_digits dig;

  dig = IntToDigits(m);
  
  while (keyValue == 0xFF02FD) {
    for (int j=0; j<4; j++) {
      SwitchDigit(j);
      lightNumber(dig.digit[j]);
      delay(5);
    }
    if (digitalRead(button3)==LOW) {
      if (pressed3  == 0 || pressed3 > 30) {
        countdown_time += 60;
        if(countdown_time > 6000) {
          countdown_time -= 6000;
        }
        dig = IntToDigits(countdown_time);
      } 
      pressed3 += 1;
    }
    else if (digitalRead(button4)==LOW) { 
      if (pressed4 == 0 || pressed4 > 30) {
        countdown_time -= 60;
        if(countdown_time <= 0) {
          countdown_time += 6000;
        }
        dig = IntToDigits(countdown_time);
      } 
      pressed4 += 1;
    }
    if (digitalRead(button5)==LOW) { 
      if (pressed5  == 0 || pressed5 > 30) {
        countdown_time += 1;
        if(countdown_time > 6000) {
          countdown_time -= 6000;
        }
        dig = IntToDigits(countdown_time);
      } 
      pressed5 += 1;
    }
    else if (digitalRead(button6)==LOW) { 
      if (pressed6 == 0 || pressed6 > 30) {
        countdown_time -= 1;
        if(countdown_time <= 0) {
          countdown_time += 6000;
        }
        dig = IntToDigits(countdown_time);
      } 
      pressed6 += 1;
    }
    if (digitalRead(button3)==HIGH)  {
      pressed3=0;
    }
    if (digitalRead(button4)==HIGH) {
      pressed4=0;
    }
    if (digitalRead(button5)==HIGH)  {
      pressed5=0;
    }
    if (digitalRead(button6)==HIGH) {
      pressed6=0;
    }
    
  }
}
void loop() {
  // Read temperature
  float tempC = 100 * ((analogRead(TEMP_PIN) * 5.0 / 1024) - 0.5);
  float tempF = (tempC * 1.8) + 32;
  Serial.print("Temperature: ");
  Serial.println(tempF);

  int sensorValue = analogRead(TEMP_PIN);
  Serial.print("Sensor Value: ");
  Serial.println(sensorValue);

  // Display temperature on NeoPixels
  if (tempF >= 100 && tempF <= 150) {
    // If temperature is between 60 and 70 degrees, light up 3 pixels in red
    for (int pixel = 0; pixel < 3; pixel++) {
      NeoPixel.setPixelColor(pixel, NeoPixel.Color(255, 0, 0));
    }
  } else if (tempF > 150 && tempF <= 200) {
    // If temperature is between 70 and 80 degrees, light up 5 pixels in red
    for (int pixel = 0; pixel < 5; pixel++) {
      NeoPixel.setPixelColor(pixel, NeoPixel.Color(255, 0, 0));
    }
  } else if (tempF > 200 && tempF <= 250) {
    // If temperature is between 80 and 90 degrees, light up 7 pixels in red
    for (int pixel = 0; pixel < 7; pixel++) {
      NeoPixel.setPixelColor(pixel, NeoPixel.Color(255, 0, 0));
    }
  } else if (tempF > 250 && tempF <= 300) {
    // If temperature is between 90 and 100 degrees, light up 10 pixels in red
    for (int pixel = 0; pixel < 10; pixel++) {
      NeoPixel.setPixelColor(pixel, NeoPixel.Color(255, 0, 0));
    }
  } else if (tempF > 300) {
    // If it's above 100 degrees, light up 14 pixels in red
    for (int pixel = 0; pixel < 14; pixel++) {
      NeoPixel.setPixelColor(pixel, NeoPixel.Color(255, 0, 0));
    }
  } else {
    // If it's not in any specified temperature range, set NeoPixels to blue
    for (int pixel = 0; pixel < 14; pixel++) {
      NeoPixel.setPixelColor(pixel, NeoPixel.Color(0, 0, 255));
    }
  }

  NeoPixel.show();

  reset();
  Serial.println("A");

  while (!Countdown(countdown_time, 962)) {
    reset();
    Serial.println("B");
  }

  while (keyValue == 0xFFE21D) {
  }


  Serial.println("C");


}