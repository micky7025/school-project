#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <TridentTD_LineNotify.h>

#define LINE_TOKEN "Ll3wShTmiBHR38LL0rwBAUSYuJP3KkeMMLdR5mIgWRR"   // 修改成上述寄到登入郵箱的 Token號碼
#define Skip_ROM    0xcc //用於1對1時，省略每次作ROM序號確認程序(定)
#define Convert_T   0x44 //啟動溫度轉換(定)
#define Read_Scratchpad  0xbe //讀取暫存器值，有9個字元(定)

char ssid[] = "米奇妙妙屋";
char pass[] = "69696699";   // 設定網路基地台,SSID和密碼

const uint8_t  DQ_pin = 4;
float temp_f;   // 從DS18B20讀取的值
String tempe1 = "";   // 顯示字串


void setup(void)
{
  Serial.begin(9600);   // 設定速率

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
  
    temp_f = getTempC();// 讀取溫度

    if (temp_f == -127.00) { 
      tempe1 = "\n Failed to read from sensor!";
      
    }

    else if(temp_f > 30){
      tempe1 = " \n 溫度過高! 已超過30度!(" + String(temp_f) + ")";
     
    }

    else if(temp_f < 20){
      tempe1 = " \n 溫度過低! 已低於20度!(" + String(temp_f) + ")";
     
    }

    else {
      tempe1 = "\n 現在溫度為：" + String(temp_f) + "℃";
                            // 檢查值是否錯誤,若正常則輸出
    }
    Serial.println(tempe1);

    LINE.notify(tempe1);   //output

    delay(4000); //10分鐘量測一次
}
float getTempC()    //獲取溫度指令
{
 while(!CommandReset());         //重啟成功，再進行動作
 SendCommand(Skip_ROM);          //主機下0xcc命令（因為1對1,省略ROM確認作業）
 SendCommand(Convert_T);         //0x44啟動溫度轉換
 delay(750);
 //再次重啟，因為要作通訊作業
 while(!CommandReset());         //重啟成功，再進行動作
 SendCommand(Skip_ROM);          //主機下0xcc命令（1對1的省略ROM確認作業）
 SendCommand(Read_Scratchpad);   //0xBE溫度暫存器中的訊息（共9個字元）
 //前兩個字元就是溫度的訊息
 uint8_t LSB = ReceiveData();    //第一個讀到的是低位
 uint8_t MSB = ReceiveData();    //第二個讀到的是高位
 return Caculate_Temperature(LSB,MSB);
}

float Caculate_Temperature(uint8_t temp_LSB , uint8_t temp_MSB)   //計算溫度
{
 boolean  fp_minus = false;      //温度正負標誌：預設為false，因為通常量測溫度大於零度
 if(temp_MSB > 0x7f)             //當temp_MSB>7代表此溫度為負數值時，測到的數值需要先反相再加 1
 {
  fp_minus = true;               //溫度正負指標，負數時fg_Minus=0
  temp_MSB = ~temp_MSB;          //將temp_MSB中的每一位元反相（0、1互換）
  temp_LSB = ~temp_LSB + 1;      //將temp_LSB中的每一位元反相（0、1互換），要記得加一，才能正確的反應其值
 }
 //以十六位元空箱來結合MSB及LSB
 uint16_t raw_temp =
     (((uint16_t)temp_MSB) << 8) | //將MSB先用空的十六位元左移8個位元，等於乘256
     (((uint16_t)temp_LSB));
 float fp_temp = raw_temp * 0.0625;
 //將十六位元的整數值再乘於0.0625的單位值，既得溫度值
 if(fp_minus) {fp_temp = -fp_temp;} //當fp_minus是1，代表是負數，將溫度加上負號
 return fp_temp;
}

uint8_t ReceiveData()
{
 uint8_t byte_in=0;
 for(uint8_t i = 0; i < 8; i++)
 {
  //此時所測到的電位，就是此位元的資料
  if(ReadSlot()) {
   //看看此時主機線的電位狀況若為高位，就是1
   bitSet(byte_in, i);     //將byte_in第i個位元值，設置為1
  }
  else {
   bitClear(byte_in, i);   //將byte_in第i個位元值，設置為0
  }
 }
 return (byte_in);
}

uint8_t ReadSlot() {
 delayMicroseconds(1);
 pinMode(DQ_pin, OUTPUT);       //轉為輸出，可達到高電位
 digitalWrite(DQ_pin, LOW);   //將電位拉低告訴DS18B20，主機已準備好了
 delayMicroseconds(2);
 pinMode(DQ_pin, INPUT);    //轉為輸入狀態，同時釋放線路
 delayMicroseconds(10);     //加前面的延時，約於12~13us時取樣
 uint8_t fp=digitalRead(DQ_pin);
 delayMicroseconds(55);    //加上延時過渡此段作業時間60us
 return fp;
}

void SendCommand(uint8_t instruction)
{
 for(uint8_t i = 0; i < 8; i++) {
  WriteSolt(bitRead(instruction, i));
 }
}

void WriteSolt(uint8_t order_bit)
{
 if(order_bit) {        //當值為1時的處理，
  pinMode(DQ_pin, OUTPUT);      //先將pin腳改為輸出狀態
  digitalWrite(DQ_pin, LOW);    //將電位拉低，等於通知DS18B20要do something
  delayMicroseconds(10);       //至少要等待1us，但於15us前轉為高電位
  pinMode(DQ_pin, INPUT);     //將接收轉成INPUT狀態，轉為高電位
  delayMicroseconds(60);      //加前段的延時至少等待60us過此周期
 }
 else {           //當寫入值為'0'時，Tx拉低電位時段60~120us
  pinMode(DQ_pin, OUTPUT);    //先轉為輸出狀態
  digitalWrite(DQ_pin, LOW);   //將電位輸出低電位
  delayMicroseconds(65);         //靜靜的等待DS18B20來讀取資料
  pinMode(DQ_pin, INPUT);      //釋放電位控制轉回輸入狀態
  delayMicroseconds(5);     //等待上拉電阻將電位復位為HIGH
 }
}

uint8_t CommandReset()
{
 pinMode(DQ_pin, OUTPUT);
 digitalWrite(DQ_pin, LOW);
 delayMicroseconds(720);
 pinMode(DQ_pin, INPUT);
 delayMicroseconds(70);
 uint8_t is_exist = !digitalRead(DQ_pin);
 delayMicroseconds(410);
 return is_exist;
}
