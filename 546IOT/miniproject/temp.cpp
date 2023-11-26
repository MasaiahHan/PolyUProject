#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 3     // DHT11数据引脚连接到GPIO 25
#define DHTTYPE DHT11 // DHT11类型
#define LEDPIN 5     // 红色LED连接到GPIO 10

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(LEDPIN, OUTPUT); // 设置LED引脚为输出模式
}

void loop() {
  // 等待几秒钟之间的读取
  delay(2000);

  // 读取温度和湿度
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

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
  if (temperature > 30) {
    digitalWrite(LEDPIN, HIGH); // 点亮LED
  } else {
    digitalWrite(LEDPIN, LOW); // 关闭LED
  }
}
