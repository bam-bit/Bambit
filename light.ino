// LED가 연결된 PIN번호

// Example of the different modes of the X.509 validation options
// in the WiFiClientBearSSL object
//
// Jul 2019 by Taejun Kim at www.kist.ac.kr

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <time.h>

const char *ssid = "SO070VOIP0C4D";  // 와이파이 이름
const char *pass = "BEEDCB0C4C";      // 와이파이 비밀번호
const char *thingId = "bambit_2";          // 사물 이름 (thing ID) anyname okay
const char *host = "aiaz3f44owv3t-ats.iot.ap-northeast-2.amazonaws.com"; // AWS IoT Core 주소


int soundSensor = A0;     // 사운드 센서 핀
int soundValue;
int threshold = 200; // 큰 소리 크기 기준
const int fade = 10; // delay time
int led = D15;

void setup()
{
 pinMode(led, OUTPUT);

 Serial.begin(115200); // 시리얼모니터 출력
 pinMode(soundSensor, INPUT);  // 사운드 센서 입력으로 설정  
}

void loop()
{
  //setColor(255, 255, 0); // yellow
  //delay(1000); 
  //setColor(255, 0, 0); // red
  //delay(1000);
  soundValue = analogRead(soundSensor);
  Serial.println(soundValue); 
  if( soundValue >= threshold){    // 감지된 소리의 크기가 500 이상일 때
      for(int i=255; i>0; i--){ 
          analogWrite(led, i); // LED 끄기
          delay(10);
      }
      delay(1000);
      for(int i=0; i<255; i++){ 
          analogWrite(led, i); // LED 켜기
          delay(10);
      }
      
  }else{       // 감지된 소리의 크기가 500 이하일 때 
      digitalWrite(led, HIGH); // LED 켜기
      delay(1);
  }
}



 
