// #include <Adafruit_Sensor.h>
// #include <DHT.h>
// #include <DHT_U.h>

// #define DHTPIN 25     // DHT11数据引脚连接到GPIO 25
// #define DHTTYPE DHT11 // DHT11类型
// #define LEDPIN 10     // 红色LED连接到GPIO 10
// #define BUZZER_PIN 11 // 蜂鸣器连接到GPIO 11
// #define LIGHT_SENSOR_PIN A1 // 光感传感器连接到A1

// DHT dht(DHTPIN, DHTTYPE);

// void setup() {
//   Serial.begin(9600);
//   dht.begin();
//   pinMode(LEDPIN, OUTPUT); // 设置LED引脚为输出模式
//   pinMode(BUZZER_PIN, OUTPUT); // 设置蜂鸣器引脚为输出模式
// }

// void loop() {
//   // 读取温度和湿度
//   float humidity = dht.readHumidity();
//   float temperature = dht.readTemperature();
//   int lightLevel = analogRead(LIGHT_SENSOR_PIN); // 读取光线强度

//   // 检查读取失败的情况
//   if (isnan(humidity) || isnan(temperature)) {
//     Serial.println("Failed to read from DHT sensor!");
//     return;
//   }

//   // 打印温度、湿度和光线强度
//   Serial.print("Humidity: ");
//   Serial.print(humidity);
//   Serial.print("%  Temperature: ");
//   Serial.print(temperature);
//   Serial.print("°C  Light Level: ");
//   Serial.println(lightLevel);

//   // 当温度超过30摄氏度时点亮红色LED
//   if (temperature > 30) {
//     digitalWrite(LEDPIN, HIGH); // 点亮LED
//   } else {
//     digitalWrite(LEDPIN, LOW); // 关闭LED
//   }

//   // 当光线强度超过某个阈值时，蜂鸣器发声
//   if (lightLevel > 500) { // 假设阈值为500，您可以根据需要调整
//     digitalWrite(BUZZER_PIN, LOW); // 低电平触发蜂鸣器
//     delay(100); // 蜂鸣器响100毫秒
//     digitalWrite(BUZZER_PIN, HIGH); // 关闭蜂鸣器
//     delay(100); // 等待100毫秒
//   } else {
//     digitalWrite(BUZZER_PIN, HIGH); // 确保蜂鸣器关闭
//   }
// }




#include <Arduino.h>


#define BUZZER_PIN  8      // 蜂鸣器连接到GPIO 11
int analogPin=A0;

void setup() {
  Serial.begin(9600);
  pinMode(BUZZER_PIN, OUTPUT); // 设置蜂鸣器引脚为输出模式
  digitalWrite(BUZZER_PIN, HIGH);
}

void loop() {
  int lightLevel = analogRead(analogPin); // 读取光线强度
  Serial.print("Light Level: ");
  Serial.println(lightLevel);
        
  // 当光线强度超过某个阈值时，蜂鸣器发声
  if (lightLevel < 500) { // 假设阈值为500，您可以根据需要调整
    digitalWrite(BUZZER_PIN, LOW); // 低电平触发蜂鸣器
    delay(1000); // 蜂鸣器响100毫秒
    digitalWrite(BUZZER_PIN, HIGH); // 关闭蜂鸣器
    delay(100); // 等待100毫秒
  } else {
    digitalWrite(BUZZER_PIN, HIGH); // 确保蜂鸣器关闭
  }

  delay(500); //
}
