#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define DQ_pin 2  

// 設定網路基地台SSID跟密碼
char ssid[] = "米奇妙妙屋";
char pass[] = "69696699";
 
float temp_f;   // 從DS18B20讀取的值
String tempe1="";      // 顯示溫度的字串

unsigned long previousMillis = 0;        // 儲存上次讀取的時間
const long interval = 2000;              // 感測器讀取之間隔
const char* host = "maker.ifttt.com";
const int httpPort = 80;  //http

OneWire oneWire(DQ_pin);
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
}
 
void loop(void)
{
// 量測間等待至少 2 秒
  unsigned long currentMillis = millis();
 
  if(currentMillis - previousMillis >= interval) {  // 將最後讀取感測值的時間紀錄下來     
    previousMillis = currentMillis;   

    sensors.requestTemperatures(); // sensor溫度轉換
    temp_f = sensors.getTempCByIndex(0); // 讀取溫度
    
    if (temp_f==127.00) {
       Serial.println("Failed to read from sensor!");    // 檢查值是否為空值
       return;
    }
  }


  tempe1="Now Temperature is "+String(temp_f)+"oC";   
  Serial.println(tempe1);

  WiFiClient client;
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }


  String getStr_line = "GET /trigger/Line_msg/with/key/c2TpfMf1jPsOY7UBy99wlu?value1="+tempe1+" HTTP/1.1\r\n"
  + "Host: " + host + "\r\n" + "User-Agent: BuildFailureDetectorESP32\r\n" + "Connection: close\r\n\r\n";
  Serial.println(getStr_line);
  client.print(getStr_line);
  client.stop();
  
  delay(600000); //10分鐘量測一次
}
