#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <TridentTD_LineNotify.h>

#define LINE_TOKEN "Ll3wShTmiBHR38LL0rwBAUSYuJP3KkeMMLdR5mIgWRR"   // 修改成上述寄到登入郵箱的 Token號碼

char ssid[] = "米奇妙妙屋";
char pass[] = "69696699";   // 設定網路基地台,SSID和密碼

const int oneWireBus = 4;
float temp_f;   // 從DS18B20讀取的值
String tempe1 = "";   // 顯示字串

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
    Serial.print(".");  // 等待連線
  }
  
  Serial.println("");
  Serial.println("temperature Reading Server");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  LINE.setToken(LINE_TOKEN);
}

void loop(void)
{
  
    sensors.requestTemperatures(); // 請求溫度
    temp_f = sensors.getTempCByIndex(0); // 讀取溫度

    if (temp_f == -127.00) { 
      tempe1 = "\n Failed to read from sensor!";
      
    } else {                                                
      tempe1 = "\n 現在溫度為：" + String(temp_f) + "℃";    // 檢查值是否錯誤,若正常則輸出
    }
    Serial.println(tempe1);

    LINE.notify(tempe1);   //output
    
    delay(1000); //10分鐘量測一次
}
