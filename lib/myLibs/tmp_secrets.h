
/////////////////////////////////////////
// Wi-Fi

#define SSID ""
#define PASS ""

/////////////////////////////////////////
// Blynk server
#define SERVER_ADDRESS "" // Адрес сервера Blynk
#define SERVER_PORT 8080

/////////////////////////////////////////
// Blynk device
#define AUTH ""

////////////////////////////////////////
// Таймеры
int IDt_reconnectBlynk; // ID таймера для перезагрузки
int IDt_delayForDevice; // ID таймера для считывания данных с датчиков

bool isFirstConnection = true;