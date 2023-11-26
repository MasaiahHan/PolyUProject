
#include <PubSubClient.h>
#include <PDM.h>
#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>

// WiFi和MQTT配置
// const char* ssid = "yourSSID"; // 替换为您的WiFi名称
// const char* password = "yourPASSWORD"; // 替换为您的WiFi密码
// const char* mqttServer = "yourMQTTserver"; // 替换为您的MQTT服务器地址
// const int mqttPort = 1883; // MQTT端口（通常是1883）
// const char* mqttUser = "yourMQTTuser"; // MQTT用户名（如果有）
// const char* mqttPassword = "yourMQTTpassword"; // MQTT密码（如果有）
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

void setup() {
  Serial.begin(9600);
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
}

void onPDMdata() {
  int bytesAvailable = PDM.available();
  PDM.read(sampleBuffer, bytesAvailable);
  samplesRead = bytesAvailable / 2;
}

// void reconnect() {
//   while (!client.connected()) {
//     Serial.print("Attempting MQTT connection...");
//     if (client.connect("RP2040Client", mqttUser, mqttPassword)) {
//       Serial.println("connected");
//     } else {
//       Serial.print("failed, rc=");
//       Serial.print(client.state());
//       Serial.println(" try again in 5 seconds");
//       delay(5000);
//     }
//   }
// }
