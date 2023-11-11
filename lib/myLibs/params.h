////////////////////////////////////////////
#define BLYNK_PRINT Serial
#define DHTTYPE DHT11 

////////////////////////////////////////////
// Пины
#define DHTPIN    14  // d5 (use pins 3, 4, 5, 12, 13 or 14)
#define RED_PIN   15  // d8
#define GREEN_PIN 13  // d7
#define BLUE_PIN  12  // d6

////////////////////////////////////////////
// Создание объектов (классы)

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer; // Объявляем класс Таймер
BlynkTimer delayForDevice;
BH1750 lightMeter;
Adafruit_BMP085 bmp;
WidgetTerminal terminal(V6);
Motor M1(0x30,_MOTOR_A,1000);   //Motor shiled I2C Address: 0x30
Motor M2(0x30,_MOTOR_B,1000);   //PWM frequency: 1000Hz(1kHz)

////////////////////////////////////////////
// Переменные

float pmw;