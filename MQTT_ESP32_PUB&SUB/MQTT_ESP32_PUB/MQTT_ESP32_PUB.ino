//----------------------------------------------------->define WiFi(Esp32)
#include <WiFi.h>
char ssid[] = "Room1463-wifi_2.4GHz";
char pass[] = "14Room63";

//----------------------------------------------------->define PubSubClient
#include <PubSubClient.h>
#define mqtt_server "192.168.100.40" 

WiFiClient espClient;
PubSubClient client(espClient);

//----> Pub variable
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];

//----------------------------------------------------->define variable
unsigned long startmillis = 0;
unsigned long currentmillis = 0;

int check;

int buttonState = 0;
int buttonPushCounter = 0;
int lastButtonState = 0;
  
//----------------------------------------------------->Setup
void setup() {
  //----> Serial
  Serial.begin(9600);
  
  //----> Connect WiFi
  WiFi.begin(ssid,pass);

  //----> MQTT
  client.setServer(mqtt_server, 1883);

  //----> GPIO
  pinMode(15,INPUT_PULLUP);
}

//----------------------------------------------------->Loop
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  GPIO_Func();

  /*  msg(Pub) send data every 1000ms */
  currentmillis = millis();
  if(currentmillis - startmillis > 400){
    startmillis = currentmillis;

    snprintf(msg,MSG_BUFFER_SIZE,"%ld",check);
    client.publish("esp32Topic", msg);
  }
}

//----------------------------------------------------->Function connect
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()){
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())){
      client.subscribe("esp32Topic");
    }
  }
}

//----------------------------------------------------->Function GPIO
void GPIO_Func(){
  Falling_Edge();
}

//----------------------------------------------------->Function Falling_Edge
void Falling_Edge(){
  
  //----> Falling Edge condition
  buttonState = digitalRead(15);
  if(buttonState != lastButtonState){
    if(buttonState == HIGH){
      buttonPushCounter++;
      //Serial.println(buttonPushCounter);
    }
    else{}
    delay(50);
  } 
  lastButtonState = buttonState;

  //----> Hold State condition
  if(buttonPushCounter%2 != 0){
    check = 0;
    Serial.println(check);
  }

  if(buttonPushCounter%2 == 0){
    check = 1;
    Serial.println(check);
  }
}
