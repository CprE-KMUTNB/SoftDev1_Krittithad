#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <BH1750FVI.h>
#include <Wire.h>
#include <AHT20.h>

#define SOIL_SENSOR 34
#define ECHO_PIN 33
#define TRIG_PIN 32
#define PUMP 25

WiFiClient espClient;
PubSubClient client(espClient);

AHT20 AHT;
BH1750FVI LightSensor(BH1750FVI::k_DevModeContLowRes);


const char* ssid = "Frank";
const char* password = "irapiluvu";
const char* mqtt_server = "192.168.43.114";
const int mqtt_port = 1883;

const char* subTopic = "dataSub";
const char* pubTopic = "dataPub";

float airHumi, airTemp;
uint16_t lux;
int ret, soilHumi, waterLevel;
bool pumpStatus;

int humidity_limit = 50;
int water_limit = 14;
bool pump_command, mode = 0;

void setup() {
  Serial.begin(9600);
  connectWIFI();
  connectMQTT();

  Wire.begin(23, 19);

  AHT.begin();
  LightSensor.begin();


  pinMode(SOIL_SENSOR, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(PUMP, OUTPUT);

  digitalWrite(PUMP, HIGH);
}

void loop() {
  lux = LightSensor.GetLightIntensity();
  ret = AHT.getSensor(&airHumi, &airTemp);
  waterLevel = getWaterLevel();
  pumpStatus = !digitalRead(PUMP);
  soilHumi = getSoilHumidity();

  sendDataToNodeRed(lux, waterLevel, airHumi, airTemp, soilHumi, pumpStatus);

  if (mode == 0) {
    if (soilHumi < humidity_limit && waterLevel > water_limit) {
      digitalWrite(PUMP, LOW);  // ON PUMP
    } else {
      digitalWrite(PUMP, HIGH);  //OFF PUMP
    }
  } else {
     digitalWrite(PUMP, pump_command);
  }

  if (!client.connected()) {
    connectMQTT();
  }
  client.loop();
}

void sendDataToNodeRed(uint16_t _lux, uint16_t _waterLevel, float _airHumi, float _airTemp, int _soilHumi, bool _pumpStatus) {
  StaticJsonDocument<200> doc;

  doc["lux"] = _lux;
  doc["waterLevel"] = _waterLevel;
  doc["airHumi"] = _airHumi * 100;
  doc["airTemp"] = _airTemp;
  doc["soilHumi"] = _soilHumi;
  doc["pumpStatus"] = _pumpStatus;


  char jsonBuffer[256];
  serializeJson(doc, jsonBuffer);

  client.publish(pubTopic, jsonBuffer);
}

void callback(char* subTopic, byte* payload, unsigned int length) {
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, payload, length);

  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }

  humidity_limit = doc["humi"];
  water_limit = doc["water"];
  pump_command = doc["PUMP"];
  mode  = doc["mode"];
}


long getDistance() {

  long duration, cm;

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(3);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(6);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);

  cm = microsecondsToCentimeters(duration);

  return cm;
}

int getSoilHumidity() {
  int dataHumidity, buffer, newRange, convertUnit;

  dataHumidity = analogRead(SOIL_SENSOR);
  // newRange = map(dataHumidity, 0, 4095, 3500, 2000);
  convertUnit = map(dataHumidity, 2000, 4095, 100, 0);

  for (int i = 0; i < 10; i++) {
    buffer += convertUnit;
  }

  return buffer / 10.0;
}

int getWaterLevel() {
  int distance, _waterLV;

  distance = getDistance();
  _waterLV = 20 - distance;

  return _waterLV;
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}

void connectWIFI() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void connectMQTT() {
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  while (!client.connected()) {
    if (client.connect("ESP32Client")) {
      Serial.println("Connected to MQTT Broker");
      client.subscribe(subTopic);
    } else {
      Serial.println("Failed to connect to MQTT Broker");
      delay(5000);
    }
  }
}