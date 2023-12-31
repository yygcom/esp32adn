//# 需要注意电压如果电脑usb口可能会欠压导致内核恐慌
//# 需要注意电压如果电脑usb口可能会欠压导致内核恐慌
//# 需要注意电压如果电脑usb口可能会欠压导致内核恐慌
/*
如果你正在使用 platform IO （你正在使用 platformIO 不是吗？）你可以添加这个
Monitor_filters = esp32_exception_decoder
到您的 platformio.ini 文件以获取有问题的错误的源代码行。
*/
#include <Arduino.h>
#include <WiFi.h>

#define WIFI_NETWORK "APname"
#define WIFI_PASSWORD "password"
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
    WiFi.begin(WIFI_NETWORK, WIFI_PASSWORD);

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
 
void setup(void) 
{
  Serial.begin(115200);
  delay(50);
  TaskHandle_t myTask;//声明一个TaskHandle_t类型的变量，用于存储将要新建的任务的句柄
  xTaskCreatePinnedToCore(
     taskOne,    /* 任务函数 */
     "taskOne",  /* 任务名称 */
     5000,            /* 堆栈大小 */
     NULL,             /* 任务参数 */
     1,                /* 任务优先级 */
     &myTask,             /* 任务句柄 */
     CONFIG_ARDUINO_RUNNING_CORE                 /* 任务所在的核心 */
  );

  Serial.print("taskOne任务的优先级 = ");
  Serial.println(uxTaskPriorityGet(myTask));
}
 
void loop(void)
{
  //delay(1000);
  //Serial.print("loop()任务的优先级 = ");
  //Serial.println(uxTaskPriorityGet(NULL));
}
