#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <TridentTD_LineNotify.h>
// 修改成上述寄到登入郵箱的 Token號碼
#define LINE_TOKEN "Ll3wShTmiBHR38LL0rwBAUSYuJP3KkeMMLdR5mIgWRR"

const int oneWireBus = 4;

// 設定網路基地台SSID跟密碼
char ssid[] = "米奇妙妙屋";
char pass[] = "69696699";

float temp_f;   // 從DS18B20讀取的值
String tempe1 = "";   // 顯示溫度的字串

unsigned long previousMillis = 0;        // 儲存上次讀取的時間
const long interval = 2000;              // 感測器讀取之間隔

OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);  // 呼叫函式庫

void setup(void)
{
  Serial.begin(9600);   // 設定速率
  sensors.begin();      // 感測開始

  WiFi.mode(WIFI_STA);  // 連接無線基地台
  WiFi.begin(ssid, pass);
  Serial.print("\n\r \n\rWorking to connect");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // 等待連線，並從 Console顯示 IP
  Serial.println("");
  Serial.println("temperature Reading Server");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  // 顯示 Line版本
  Serial.println(LINE.getVersion());
  LINE.setToken(LINE_TOKEN);
}

void loop(void)
{
  // 量測間等待至少 2 秒
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) { // 將最後讀取感測值的時間紀錄下來
    previousMillis = currentMillis;

    sensors.requestTemperatures(); // 請求溫度
    temp_f = sensors.getTempCByIndex(0); // 讀取溫度

    if (temp_f == -127.00) { // 檢查值是否為
      tempe1 = "\n Failed to read from sensor!";
    } else {
      tempe1 = "\n 現在溫度為：" + String(temp_f) + "℃";
    }
    Serial.println(tempe1);

    // 先換行再顯示
    LINE.notify(tempe1);
  }

  delay(1000); //10分鐘量測一次
}
