#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

#include <esp_wifi.h>



void wifi_init() {
  WiFi.begin("1403","14031403");
  while (WiFi.status() != WL_CONNECTED)
	{ 
		delay(300);
		Serial.print("connecting ...");
	}
  Serial.println("connected successfully");

}

void getData() {
  HTTPClient http;
  http.begin("http://122.112.170.248/");
  http.addHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/101.0.4951.64 Safari/537.36 Edg/101.0.1210.53");
  int status_code = http.GET();

  if(status_code == 200) {
    String res = http.getString();
    Serial.println(res);
  }else {
    Serial.println("请求失败");
  }

}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  wifi_init();


}

void loop() {
  Serial.println("----------");
  getData();
  delay(5000);
  // put your main code here, to run repeatedly:
}