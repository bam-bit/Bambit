#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>

#define OLED_RESET LED_BUILTIN  //4
Adafruit_SSD1306 display(OLED_RESET);

int soundSensor = A0;     // 사운드 센서 핀
int soundValue;
int threshold = 68; // 큰 소리 크기 기준
const int fade = 10; // delay time
int led = D15;

void setup()
{
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();

 Serial.begin(9600); // 시리얼모니터 출력
 pinMode(soundSensor, INPUT);  // 사운드 센서 입력으로 설정  


}

void loop()
{
  soundValue = analogRead(soundSensor);
  Serial.println(soundValue);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(1,1);
    display.clearDisplay();
    display.print("24 Hour Cafe");
    display.display();
  if(soundValue >= threshold){    // 감지된 소리의 크기가 500 이상일 때
    draw();
     for(int i=255; i>0; i--){ 
        analogWrite(led, i); // LED 끄기
        delay(50);
      }

     for(int i=0; i<255; i++){ 
        analogWrite(led, i); // LED 켜기
        delay(50);
       }
  }
}

void draw(){
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(1,1);
  display.clearDisplay();
  display.print("Please be Quiet.");
  display.display();
  delay(1000);
}


 
