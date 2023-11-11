////////////////////////////////////////////
// Подключение библиотек
#include <Arduino.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_I2CDevice.h>
#include <WEMOS_Motor.h>
#include <BH1750.h>
// Мои
#include <secrets.h>
#include <params.h>
#include <connect.h>
#include <voids.h>

void setup()
{
  Serial.begin(115200); // 115200 – скорость на которой работает контроллер ESP8266
  Serial.println("\n\nStarting setup");

  // Активация датчиков и прочего
  Wire.begin(D2, D1); // Подключение к I2C шине
  dht.begin();
  lightMeter.begin();
  bmp.begin();

  // обозначаем что наши пины работают как выходы
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  // Вызываем функцию подключения к Blynk
  ConnectBlynk(); // Blynk.begin(AUTH, SSID, PASS, SERVER_ADDRESS, SERVER_PORT);

  // Таймеры 
  IDt_delayForDevice = timer.setInterval(1L * 1000L, getDevicesData); // Каждую 1 секундку считываем данные с датчиков
  IDt_reconnectBlynk = timer.setInterval(10 * 1000, reconnectBlynk);  // Проверяем есть ли связь с сервером каждые 10 секунд

  Blynk.virtualWrite(V10, 0);

}

void loop()
{
  if (Blynk.connected())
  {
    Blynk.run();
  }
  timer.run();
}

BLYNK_WRITE(V1)
{
  int r = param[0].asInt(); // 15 - gpio пин к которому подключен красный цвет светодиода
  int g = param[1].asInt(); // аналогично для зеленого
  int b = param[2].asInt(); // аналогично для синего
  RGB_color(r, g, b);
  Serial.println("Цвет светодиода изменен");
}

BLYNK_WRITE(V10)
{
  int pwm = param.asInt();
  if (pwm == 0) 
  {
    M1.setmotor(_STOP);
    M2.setmotor(_STOP);
    terminal.println("motor stop");
  } else if (pwm > 0 && pwm <= 30) 
  {
    M1.setmotor(_CW, 30);
    M2.setmotor(_CW, 30);
    terminal.println(30);
  } else if (pwm > 30) 
  {
    M1.setmotor(_CW, pwm);
    M2.setmotor(_CW, pwm);
    terminal.println(pwm);
  } else if (pwm < 0 && pwm >= -30) 
  {
    M1.setmotor(_CCW, 30);
    M2.setmotor(_CCW, 30);
    terminal.println(-30);
  } else 
  {
    M1.setmotor(_CCW, pwm * (-1));
    M2.setmotor(_CCW, pwm * (-1));
    terminal.println(pwm);
  }
}

BLYNK_WRITE(V11)
{
  int off = param.asInt();
  if (off == 1) 
  {
    M1.setmotor(_STOP);
    M2.setmotor(_STOP);
    Blynk.virtualWrite(V10, 0);
    terminal.println("Motors stop");
  }
}