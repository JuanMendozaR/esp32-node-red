/***********************************************************
JuanDe Mendoza  
************************************************************/
#include <ESP32Servo.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
// Replace the next variables with your SSID/Password combination
const char* ssid = "red WiFi";
const char* password = "password";
Servo myServo;
// Add your MQTT Broker IP address, example:
const char* mqtt_server = "localhost";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
int val;

float temperature = 0;
float humidity = 0;

// LED Pin
const int LedPin = 25;

void setup() {
  Serial.begin(115200);
  // default settings 
  myServo.attach(13);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pinMode(LedPin, OUTPUT);

}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {


  

  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;



  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
  
  // Feel free to add more if statements to control more GPIOs with MQTT
  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  if(messageTemp > "0"){
    myServo.write(String(messageTemp).toInt());
      
      }else{
      myServo.write(0);
  
    }
    if(messageTemp == "on"){
     
      digitalWrite(LedPin, HIGH);
    }

    else if(messageTemp == "off"){
     
      digitalWrite(LedPin, LOW);
    
    
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
      // Subscribe
      
    
      client.subscribe("esp32/servo");
      client.subscribe("esp32/output");

    
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
}
}
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    
    // Temperature in Celsius
    temperature = analogRead(33);
  
    // Convert the value to a char
    char tempString[8];
  
    dtostrf(temperature, 1, 2, tempString);
    
    Serial.print("Temperature: ");
    Serial.println(tempString);
    client.publish("esp32/temperature", tempString);
    
    
    

    humidity = analogRead(35) ;
    
    // Convert the value to a char array
    char humString[8];
    dtostrf(humidity, 1, 2, humString);
    Serial.print("Humidity: ");
    Serial.println(humString);
    client.publish("esp32/humidity", humString);
  }
}


