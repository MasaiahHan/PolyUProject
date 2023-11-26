#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#include <PubSubClient.h>
#include <PDM.h>
#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>


#define DHTPIN 3     // DHT11数据引脚连接到GPIO 25
#define DHTTYPE DHT11 // DHT11类型
#define LEDPIN 5     // 红色LED连接到GPIO 10

#define BUZZER_PIN  8      // 蜂鸣器连接到GPIO 11
int analogPin = A0;


// setting wifi
char ssid[] = "1103";   // WiFi network SSID (name) for EIE568 Lab
char pass[] = "13642849111"; // WiFi network password (use for WPA, or use as key for WEP)
const char server[] = "test.mosquitto.org";
int        port     = 1883;
const char topic[]  = "tyh_test";
String Message = "baby is crying"; 
//set interval for sending messages (milliseconds)
const int interval = 5000; // 5 seconds
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;


// WiFiClient wifiClient;
// PubSubClient client(wifiClient);
WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

// PDM麦克风配置
static const char channels = 1;
static const int frequency = 16000;
short sampleBuffer[512];
volatile int samplesRead;



DHT dht1(DHTPIN, DHTTYPE); // 声明DHT对象
void setup() {
  Serial.begin(9600);

  pinMode(BUZZER_PIN, OUTPUT); // 设置蜂鸣器引脚为输出模式
  digitalWrite(BUZZER_PIN, HIGH);
  dht1.begin();
  pinMode(LEDPIN, OUTPUT); // 设置LED引脚为输出模式

  // attempt to connect to Wi-Fi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.print(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }
  Serial.println();
  Serial.print("Connected successfully to WPA SSID: ");
  Serial.println(ssid);
  Serial.println();

 // attempt to connect to MQTT server network:
  Serial.print("Attempting to connect to the MQTT server: ");
  Serial.println(server);

  if (!mqttClient.connect(server, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.print("Connected successfully to the MQTT server: ");
  Serial.println(server);
  Serial.println();


  // 初始化PDM
  PDM.onReceive(onPDMdata);
  if (!PDM.begin(channels, frequency)) {
    Serial.println("Failed to start PDM!");
    while (1);
  }
}

void loop() {
  int lightLevel = analogRead(analogPin); // 读取光线强度
  Serial.print("Light Level: ");
  Serial.println(lightLevel);

  // light sensor part
  //当光线强度超过某个阈值时，蜂鸣器发声
  if (lightLevel < 500) { // 假设阈值为500，您可以根据需要调整
    digitalWrite(BUZZER_PIN, LOW); // 低电平触发蜂鸣器
    delay(1000); // 蜂鸣器响100毫秒
    digitalWrite(BUZZER_PIN, HIGH); // 关闭蜂鸣器
    delay(100); // 等待100毫秒
  } else {
    digitalWrite(BUZZER_PIN, HIGH); // 确保蜂鸣器关闭
  }

  // 读取温度和湿度
  float humidity = dht1.readHumidity();
  float temperature = dht1.readTemperature();

  // 检查读取失败的情况
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // 打印温度和湿度
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%  Temperature: ");
  Serial.print(temperature);
  Serial.println("°C ");

  // 当温度超过30摄氏度时点亮红色LED
  if (temperature > 31) {
    digitalWrite(LEDPIN, HIGH); // 点亮LED
  } else {
    digitalWrite(LEDPIN, LOW); // 关闭LED
  }

  // wifi mqtt
  currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // more than interval (10 seconds) have elasped 
    // since the last time a message was sent
    previousMillis = currentMillis;

    if (samplesRead) {
      long sum = 0;
      for (int i = 0; i < samplesRead; i++) {
        sum += abs(sampleBuffer[i]);
      }
      long average = sum / samplesRead;
      Serial.print("Average Sound Level: ");
      Serial.println(average);

      if (average > 1000) { // 假设阈值为1000
        // send message, the Print interface can be used to set the message contents
        mqttClient.beginMessage(topic);
        mqttClient.print(Message);
        mqttClient.endMessage();
      
      }

      samplesRead = 0;
    }
  }
  else {
    mqttClient.poll();
    delay(1000);
  }



  // delay(500); //
}


void onPDMdata() {
  int bytesAvailable = PDM.available();
  PDM.read(sampleBuffer, bytesAvailable);
  samplesRead = bytesAvailable / 2;
}