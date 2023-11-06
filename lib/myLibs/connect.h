
// Подключение к Wi-Fi
void WIFIinit()
{
    // Попытка подключения к точке доступа
    byte tries = 5; // Количество попыток
    WiFi.begin(SSID, PASS);
    Serial.println("Connecting to Wi-Fi"); 
    terminal.println("Connecting to Wi-Fi");
    // Делаем проверку подключения до тех пор пока счетчик tries не станет равен нулю или не получим подключение
    while (--tries && WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        terminal.println(".");
        delay(1000);
    }
    if (WiFi.status() != WL_CONNECTED)
    {
        // Если не удалось подключиться Скажем об этом
        Serial.println("\nWiFi not connected");
        terminal.println("\nWiFi not connected");
    }
    else
    {
        // Иначе удалось подключиться отправляем сообщение о подключении и выводим адрес IP
        Serial.println("\nWiFi connected");
        terminal.println("\nWiFi connected");
    }
}

// подключение к Блинк
void ConnectBlynk()
{
    timer.disable(IDt_reconnectBlynk); // Выключаем таймер

    /////////////////////////////////////////////////////////
    // Подключение к Wi-Fi
    
    if (WiFi.status() != WL_CONNECTED) // Если нет соединения, то устанавливаем
    {
        WIFIinit();
    }
    
    /*  Дисконект - для справки
      WiFi.disconnect(); // отключаемся от сети
      Serial.println("Disconnect WiFi.");
    */

    /////////////////////////////////////////////////////////
    // Подключение к Blynk

    if (WiFi.status() == WL_CONNECTED) // Если Wi-Fi есть, то можно подключаться
    {
        Blynk.config(AUTH, SERVER_ADDRESS, SERVER_PORT);
        Blynk.connect(5000);
        if (Blynk.connected() && isFirstConnection) {
            Serial.println("Blynk connected");
            Blynk.syncAll();
            terminal.println("Blynk connected");
        }
        // Serial.println(Blynk.connect(5000));
    }

    timer.enable(IDt_reconnectBlynk);       // Включаем таймер
    timer.restartTimer(IDt_reconnectBlynk); // Перезапускаем таймер

    // До бесконечности будем оставаться в цикле while
    // пока не установим связь с сервером
    // while (Blynk.connect() == false) {}
    
} // ConnectBlynk()

// Реконектимся если обрыв связи
void reconnectBlynk()
{
    if (!Blynk.connected())
    {
        isFirstConnection = false;
        Serial.println("\nBlynk disconnected...");
        terminal.println("\nBlynk disconnected...");
        ConnectBlynk();
        if (Blynk.connected())
        {
            Serial.println("Reconnected to Blynk");
            terminal.println("Reconnected to Blynk");
        }
        else
        {
            Blynk.disconnect();
            Serial.print("Not reconnected to ");
            terminal.print("Not reconnected to ");
            if (WiFi.status() == WL_CONNECTED)
            {
                Serial.println("Blynk Server");
                terminal.println("Blynk Server");
            }
            else
            {
                Serial.println("Wi-Fi");
                terminal.println("Wi-FI");
            }
        }
    }

} // reconnectBlynk()