// Example of the different modes of the X.509 validation options
// in the WiFiClientBearSSL object
//
// Jul 2019 by Taejun Kim at www.kist.ac.kr

/*
 bambit 1
 */
 
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <time.h>

#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <Wire.h>
#include <ArduinoJson.h>

//LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
LiquidCrystal_I2C lcd(0x3F, 16,2);

int soundSensor = A0;     // 사운드 센서 핀
int soundValue;
int threshold = 60; // 큰 소리 크기 기준
int ledPin = D15;
int num = 1;
String lcdString = ""; // 출력할 글자를 저장하기 위한 공간(버퍼))
String name = "light1";


const char *ssid = "";  // 와이파이 이름
const char *pass = "";      // 와이파이 비밀번호
const char *thingId = "light1";          // 사물 이름 (thing ID) anyname okay
const char *host = ""; // AWS IoT Core 주소



// 사물 인증서 (파일 이름: xxxxxxxxxx-certificate.pem.crt)
const char cert_str[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----
)EOF";
// 사물 인증서 프라이빗 키 (파일 이름: xxxxxxxxxx-private.pem.key)
const char key_str[] PROGMEM = R"EOF(
-----BEGIN RSA PRIVATE KEY-----

-----END RSA PRIVATE KEY-----
)EOF";
// Amazon Trust Services(ATS) 엔드포인트 CA 인증서 (서버인증 > "RSA 2048비트 키: Amazon Root CA 1" 다운로드)
const char ca_str[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----
)EOF";

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

}

X509List ca(ca_str);
X509List cert(cert_str);
PrivateKey key(key_str);
WiFiClientSecure wifiClient;
PubSubClient client(host, 8883, callback, wifiClient); //set  MQTT port number to 8883 as per //standard


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(thingId)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("bambit", "connected");
      // ... and resubscribe
      client.subscribe("bambit");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");

      char buf[256];
      wifiClient.getLastSSLError(buf,256);
      Serial.print("WiFiClientSecure SSL error: ");
      Serial.println(buf);

      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// Set time via NTP, as required for x.509 validation
void setClock() {
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
}

void setup() {
    lcd.begin(16,2); // LCD 크기 지정, 2줄 16칸
  lcd.clear();  // 화면 초기화
  
  Serial.begin(9600);
  Serial.setDebugOutput(true);
  Serial.println();
  Serial.println();

  // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  wifiClient.setTrustAnchors(&ca);
  wifiClient.setClientRSACert(&cert, &key);
  Serial.println("Certifications and key are set");

  setClock();
  //client.setServer(host, 8883);
  client.setCallback(callback);

 pinMode(soundSensor, INPUT);  // 사운드 센서 입력으로 설정
}

long lastMsg = 0;
char msg[50];
int value = 0;

void loop() {
  if (!client.connected()) {
    reconnect();
    Serial.print("not connected");
  }
  
  client.loop();
  lcd.setCursor(0,0);
  lcd.print("24h cafe -> ");
  
long now = millis();
  if (now - lastMsg > 1000) {
    lastMsg = now;
    soundValue = analogRead(soundSensor);
    time_t now = time(nullptr); 
    Serial.println(ctime(&now));


  analogWrite(ledPin, 255);

  StaticJsonBuffer<200> jsonBuffer;
  //char* data = "";
  
  JsonObject& root = jsonBuffer.createObject();
  root["num"] = num;
  root["date"] = ctime(&now);
  root["sound"] = soundValue;
  root["device"] = name;

  //root.printTo(root);
  char data[100];
  root.printTo(data, sizeof(data));



  sprintf (msg, "{\"num\": %d}{\"sound\": %d}{\"name\": %s}", num, soundValue, "light1");
  Serial.print("Publish message: ");
  Serial.println(msg);
  analogWrite(ledPin, 255);


  // fade out from max to min in increments of 5 points:
  if(soundValue >= threshold){
    client.publish("bambit", data); // 이부분을 수정해야함ㅠㅠ
    for (int fadeValue = 255 ; fadeValue >= 0; fadeValue -= 5) {
      // sets the value (range from 0 to 255):
      analogWrite(ledPin, fadeValue);
      // wait for 30 milliseconds to see the dimming effect
      delay(30);
    }
   // LCD로 바꾸기
    analogWrite(ledPin, 0);
    lcd.setCursor(0,1);
    lcd.print("Silence Please.");
    
    delay(3000);   // 조절하기
    
    lcd.clear();

    for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += 5) {
      // sets the value (range from 0 to 255):
      analogWrite(ledPin, fadeValue);
      // wait for 30 milliseconds to see the dimming effect
      delay(30);
    }   // LCD로 바꾸기
    
    delay(6000);
    Serial.print("Heap: ");
    Serial.println(ESP.getFreeHeap()); //Low heap can cause problems
  }
  //delay(500);
  num++;
  }
}
