# Smart Farm using Zigbee2MQTT & Arduino

## Team Member & Task Assignments
- **SERMCHART LEELASAKULMEKEART (6501012620047)**  
  Task: Hardware Interaction
- **NORAWIT BOONLERTRAT (6501012610068)**  
  Task: Database
- **KRITTITHAD VIBOONSUNTI (6501012630018)**  
  Task: Node-red interface

## Scope of Work
- Using Zigbee2mqtt button to manually control the function of Smart Farm
- Design a system to detect and record the amount of water in the tank, light intensity, temperature, and the humidity in air and soil
- Make automatic system control UV Light source and water pump based on data from sensor

## Hardware
- Wemos lolin 32 lite (Micro Controller)
- AHT 20 (Air Temperature and Humidity sensors)
- GY-30 BH1750FVI (Light intensity sensors)
- Ultrasonic Module HC-SR04 (Distance measurement sensors)
- Capacitive Soil Moisture Sensor v2.0 (Soil Moisture sensors)
- Relay module 2 ch 5VDC
- Step Down
- Pump 5V
- Zigbee Smart plug (TS011F_plug_1)
- Zigbee 3-button wireless switch (TS0043)
- Zigbee 3.0 USB Dongle Plus–ZBDongle-E
- LED Light

## OS
- Ubuntu 22.04 LTS (VMware v17.0.2 build-21581411/VirtualBox Version 6.1)

## Software
- Arduino IDE v2.1.1
- Libraries: Wifi, PubSubClient, ArduinoJson, BH1750FVI, Wire, AHT20
- Node red v3.1.0
- Node-RED Nodes: node-red-contrib-influxdb 0.6.1, node-red-dashboard 3.6.0, node-red-node-ui-table 0.4.3
- Node.js version v18.18.0
- Zigbee2mqtt v1.33.0
- InfluxDB v2.7.3
