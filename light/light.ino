#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET LED_BUILTIN  //4
Adafruit_SSD1306 display(OLED_RESET);

int ledPin = D8;    // LED connected to digital pin 9
int soundSensor = A0;     // 사운드 센서 핀
int soundValue;
int threshold = 110; // 큰 소리 크기 기준

void setup() {
  // nothing happens in setup
 display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
 display.clearDisplay();
 display.display();
 display.setTextSize(2);
 display.setTextColor(WHITE);
 display.setCursor(0,0);

 Serial.begin(9600); // 시리얼모니터 출력
 pinMode(soundSensor, INPUT);  // 사운드 센서 입력으로 설정  

}

void loop() {
  soundValue = analogRead(soundSensor);
  Serial.println(soundValue);

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(1,1);
  display.print("24H cafe ->");
  display.display();
  
  analogWrite(ledPin, 255);
  
  // fade out from max to min in increments of 5 points:
  if(soundValue >= threshold){
    for (int fadeValue = 255 ; fadeValue >= 0; fadeValue -= 5) {
      // sets the value (range from 0 to 255):
      analogWrite(ledPin, fadeValue);
      // wait for 30 milliseconds to see the dimming effect
      delay(30);
    }
    analogWrite(ledPin, 0);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(1,1);
    display.clearDisplay();
    display.print("Please be Quiet.");
    display.display();
    display.clearDisplay();
    
    delay(6000);
  }
}#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET LED_BUILTIN  //4
Adafruit_SSD1306 display(OLED_RESET);

int ledPin = D8;    // LED connected to digital pin 9
int soundSensor = A0;     // 사운드 센서 핀
int soundValue;
int threshold = 110; // 큰 소리 크기 기준

void setup() {
  // nothing happens in setup
 display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
 display.clearDisplay();
 display.display();
 display.setTextSize(2);
 display.setTextColor(WHITE);
 display.setCursor(0,0);

 Serial.begin(9600); // 시리얼모니터 출력
 pinMode(soundSensor, INPUT);  // 사운드 센서 입력으로 설정  

}

void loop() {
  soundValue = analogRead(soundSensor);
  Serial.println(soundValue);

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(1,1);
  display.print("24H cafe ->");
  display.display();
  
  analogWrite(ledPin, 255);
  
  // fade out from max to min in increments of 5 points:
  if(soundValue >= threshold){
    for (int fadeValue = 255 ; fadeValue >= 0; fadeValue -= 5) {
      // sets the value (range from 0 to 255):
      analogWrite(ledPin, fadeValue);
      // wait for 30 milliseconds to see the dimming effect
      delay(30);
    }
    analogWrite(ledPin, 0);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(1,1);
    display.clearDisplay();
    display.print("Please be Quiet.");
    display.display();
    display.clearDisplay();
    
    delay(6000);
  }
}
