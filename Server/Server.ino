#include <SPI.h>
#include <DHT.h>
#include <Ethernet.h>
#include <ArduinoJson.h>

#define COOLER 3
#define WATER_PUMP 4

byte mac[]={0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192,168,1,10);
EthernetServer server(8080);
EthernetClient client;

DHT dht(2,DHT11);

const size_t CAPACITY = JSON_OBJECT_SIZE(5);
StaticJsonDocument<CAPACITY> doc;

char output[200];
float set_temperature = 21.5;
float set_soil_moisture = 60;
int trigPin = 9;   
int echoPin = 8;    

float duration_us, level;


void set_regulation(float temp, float set_temp, byte actuator)
{
  if( temp >= set_temp){
    digitalWrite(actuator,LOW);
  }
  if( temp < set_temp){
    digitalWrite(actuator,HIGH);
  }
}
float getWaterLevel(){
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration_us = pulseIn(echoPin, HIGH);
  level = 0.017 * duration_us;
  return level;
}

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac,ip);
  dht.begin();
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP()); 

  pinMode(COOLER,OUTPUT);
  digitalWrite(COOLER,HIGH);
  pinMode(WATER_PUMP,OUTPUT);
  digitalWrite(WATER_PUMP,HIGH);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {

  float h = dht.readHumidity( );
  float t = dht.readTemperature( );
  int soilMoistureInput = analogRead(A0);
  int soilMoisture = map(soilMoistureInput,200,1100,100,0);
  int light = analogRead(A1);

  set_regulation(t,set_temperature,COOLER);
  set_regulation(soilMoisture,set_soil_moisture,WATER_PUMP);

  JsonObject object = doc.to<JsonObject>();

  object["temperature"] = t;
  object["humidity"] = h;
  object["soil_moisture"] = soilMoisture;
  object["light"] = light;
  object["water_level"] = getWaterLevel();

  serializeJsonPretty(object,output);

  client = server.available();
  if (client) 
   {
      boolean currentLineIsBlank = true;
      while (client.connected ( ) ) 
        {
          if (client.available ( ) ) 
            {
              char character = client.read ( );
              Serial.write(character);
              if (character == '\n' && currentLineIsBlank) 
                {
                  client.println ("HTTP/1.1 200 OK");
                  client.println ("Content-Type: application/json");
                  client.println("Access-Control-Allow-Origin: *");
                  client.println ("Connection: close");
                  client.println ("Refresh: 2");
                  client.println ( );
                  client.print (output);
                  break;
                }
                 
                if ( character == '\n') 
                  {
                    currentLineIsBlank = true;
                  } 
                else if (character != '\r') 
                  {
                    currentLineIsBlank = false;
            }
        }
    }
    delay(1);
    client.stop();
    }
}