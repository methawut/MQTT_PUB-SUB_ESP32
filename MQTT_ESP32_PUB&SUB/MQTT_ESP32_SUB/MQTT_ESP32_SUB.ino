//----------------------------------------------------->define WiFi(Esp32)
#include <WiFi.h>
char ssid[] = "Room1463-wifi_2.4GHz";
char pass[] = "14Room63";

//----------------------------------------------------->define PubSubClient
#include <PubSubClient.h>
#define mqtt_server "192.168.100.40" 

WiFiClient espClient;
PubSubClient client(espClient);

//----------------------------------------------------->define variable
int check;

//----------------------------------------------------->Setup
void setup() {
  //----> Serial
  Serial.begin(9600);
  
  //----> Connect WiFi
  WiFi.begin(ssid,pass);

  //----> MQTT
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  //----> GPIO
  pinMode(12,OUTPUT);
}

//----------------------------------------------------->Loop
void loop() {
  reconnect();
  client.loop();
  GPIO_Func();
}

//----------------------------------------------------->Function [SUB]
void callback(char* topic, byte* payload, unsigned int length){
    /*
    char ---> EX: msg(Pub) = "on" ---> {'o','n'}
                  payload(Sub) = {'o','n'} ---> lenght = 2 (0,1)                
    for loop ข้อมูลที่ละตัวใน payload
    */
    
    String Sub_data;
    for(int i = 0; i < length; i++){
      Sub_data += (char)payload[i];
      Serial.println(Sub_data);
    }
    Serial.println(Sub_data);

  if (String(topic) == "esp32Topic"){
    if((char)payload[0] == '1'){
      check = 1;
    }
    if((char)payload[0] == '0'){
      check = 0;
    }
  }
}

//----------------------------------------------------->Function connect
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()){
    client.connect("ESP32");         //Name of Device (can rename)
    client.subscribe("esp32Topic");  //Topic of Sub
  }
}

//----------------------------------------------------->Function GPIO
void GPIO_Func(){
  if(check == 1){
    digitalWrite(12,1);
  }
  if(check == 0){
    digitalWrite(12,0);
  }
}
