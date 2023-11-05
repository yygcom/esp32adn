#include "WiFi.h"

void onekeywifi(){
  //Init WiFi as Station, start SmartConfig
  WiFi.mode(WIFI_AP_STA);
  WiFi.beginSmartConfig();
 
  //Wait for SmartConfig packet from mobile
  Serial.println("Waiting for SmartConfig.");
  while (!WiFi.smartConfigDone()) {
    delay(500);
    Serial.print("w.");
  }
 
  Serial.println("");
  Serial.println("SmartConfig received.");
 
  //Wait for WiFi to connect to AP
  Serial.println("Waiting for WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("x.");
  }
 
  Serial.println("WiFi Connected.");
 
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.println(WiFi.SSID().c_str());  
  Serial.println(WiFi.psk().c_str());  
}
 
void setup() {
  Serial.begin(115200);

  pinMode(2, INPUT_PULLUP); // 设置引脚2为输入，启用内置上拉电阻  

}
 
void loop() {
  // put your main code here, to run repeatedly:
 if (digitalRead(2) == LOW) { // 如果按钮按下  
    Serial.println("Button is pressed.");  
    // 在这里添加您想要在按钮按下时执行的代码逻辑  
    onekeywifi();
  } 
}
