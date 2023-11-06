
////////////////////////////////////////////
// Работа с RGB светодиодом

int r_last = 0;
int g_last = 0;
int b_last = 0;

void change_color_smooth(int pin, int now, int last)
{
  if (now > last)
  {
    for (int i = last; i < now; i++)
    {
      analogWrite(pin, i);
    }
  }
  else
  {
    for (int i = last; i > now; i--)
    {
      analogWrite(pin, i);
    }
  }
}

void RGB_color(int r, int g, int b)
{
  change_color_smooth(RED_PIN, r, r_last);
  change_color_smooth(GREEN_PIN, g, g_last);
  change_color_smooth(BLUE_PIN, b, b_last);

  r_last = r;
  g_last = g;
  b_last = b;
}

////////////////////////////////////////////
//  DHT11

float getHumidity()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  Serial.print("Влажность: ");
  Serial.println(h);
  if (isnan(h) || isnan(t))
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    // terminal.println("Failed to read from DHT sensor!");
    return 0;
  }

  return h;
}

////////////////////////////////////////////
// BH1750

float getLight()
{
  float lux = lightMeter.readLightLevel();
  Serial.print("Освещенность: ");
  Serial.print(lux);
  Serial.println(" lx");
  return lux;
}

////////////////////////////////////////////
// BMP180

bool tempNotfySend = false;
int tempNotifyDelay = 120; // рах в 2 минуты, т.к. считывание данных происходит раз в 1 секунду

float getTemp() // получить текущую температуру
{
  float temp = bmp.readTemperature();
  Serial.print("Температура = ");
  Serial.print(temp);
  Serial.println(" *C");

  if (temp >= 30 && !tempNotfySend)
  {
    Blynk.notify("Температура больше 30 градусов!");
    tempNotfySend = true;
  } else if (temp >= 30 && tempNotfySend) 
  {
    --tempNotifyDelay;
    if (tempNotifyDelay == 0) { tempNotfySend = true; }
  }

  return temp;
}

float getPressure() // получить текущее давление
{
  float pressure = bmp.readPressure();
  Serial.print("Давление = ");
  Serial.print(pressure);
  Serial.println(" Pa");

  return pressure;
}

float getAltitude() // получить высоту над уровенем моря
{
  float altitude = bmp.readAltitude(); // рассчитывает высоту исходя из стандартного барометрического давления (1013.25 миллибар = 101325 Па)

  Serial.print("Высота = ");
  Serial.print(altitude);
  Serial.println(" м.");

  // Serial.print("Давление на уровне моря (расчитано) = ");
  // Serial.print(bmp.readSealevelPressure());
  // Serial.println(" Па");

  // вы можете получить более точное измерение высоты
  // если вы знаете текущее давление на уровне моря, которое будет
  // меняется в зависимости от погоды и тому подобного. Если это 1015 миллибар
  // что равно 101500 паскалям.
  // Serial.print("Real altitude = ");
  // Serial.print(bmp.readAltitude(102000));
  // Serial.println(" meters");
  
  return altitude;
}

////////////////////////////////////////////
// Гавное

void getDevicesData()
{
  Blynk.virtualWrite(V2, getLight());
  Blynk.virtualWrite(V3, getHumidity());
  Blynk.virtualWrite(V4, getTemp());
  Blynk.virtualWrite(V5, getPressure());
  Serial.println();
}
