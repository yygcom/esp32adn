#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoNvs.h>

String ssid = "noapname";  
String password = "nopassword";
TaskHandle_t myTask;//声明一个TaskHandle_t类型的变量，用于存储将要新建的任务的句柄

#define WIFI_TIMEOUT_MS 20000 // 20 second WiFi connection timeout
#define WIFI_RECOVER_TIME_MS 30000 // Wait 30 seconds after a failed connection attempt

void taskOne( void *pvParameters ){
  for(;;){

    if(WiFi.status() == WL_CONNECTED){
        vTaskDelay(10000 / portTICK_PERIOD_MS);
        continue;
    }

    Serial.println("[WIFI] Connecting");
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    unsigned long startAttemptTime = millis();

    // Keep looping while we're not connected and haven't reached the timeout
    while (WiFi.status() != WL_CONNECTED && 
            millis() - startAttemptTime < WIFI_TIMEOUT_MS){}

    // When we couldn't make a WiFi connection (or the timeout expired)
    // sleep for a while and then retry.
    if(WiFi.status() != WL_CONNECTED){
        Serial.println("[WIFI] FAILED");
        vTaskDelay(WIFI_RECOVER_TIME_MS / portTICK_PERIOD_MS);
    continue;
    }

    Serial.println("[WIFI] Connected: ");
    Serial.println(WiFi.localIP());
  }
  Serial.println("Ending task 1");
  vTaskDelete( NULL );
}


void onekeywifi(){

  vTaskSuspend(myTask); 
  //Init WiFi as Station, start SmartConfig
  WiFi.mode(WIFI_AP_STA);
  WiFi.beginSmartConfig();
 
  //Wait for SmartConfig packet from mobile
  Serial.println("Waiting for SmartConfig.");
  while (!WiFi.smartConfigDone()) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("SmartConfig received.");
 
  //Wait for WiFi to connect to AP
  Serial.println("Waiting for WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("WiFi Connected.");
 
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  ssid = WiFi.SSID().c_str();
  password = WiFi.psk().c_str();
  //Serial.println(ssid);  
  //Serial.println(password);  

  NVS.setString("ssid", ssid);  
  NVS.setString("password", password);
  Serial.println("配网参数已写入");
  vTaskResume(myTask);  
}
 
void setup() {
  NVS.begin();
  Serial.begin(115200);
  delay(10);
  pinMode(2, INPUT_PULLUP); // 设置引脚2为输入，启用内置上拉电阻

  //从NVS读取ssid和密码
  ssid = NVS.getString("ssid");
  password = NVS.getString("password");

  Serial.println("配网参数已读取");
  delay(10);

  xTaskCreatePinnedToCore(
     taskOne,    /* 任务函数 */
     "taskOne",  /* 任务名称 */
     5000,            /* 堆栈大小 */
     NULL,             /* 任务参数 */
     1,                /* 任务优先级 */
     &myTask,             /* 任务句柄 */
     CONFIG_ARDUINO_RUNNING_CORE                 /* 任务所在的核心 */
  );


}
 
void loop() {
  // put your main code here, to run repeatedly:
 if (digitalRead(2) == LOW) { // 如果按钮按下  
    Serial.println("Button is pressed.");  
    // 在这里添加您想要在按钮按下时执行的代码逻辑  
    onekeywifi();
  } 
}
