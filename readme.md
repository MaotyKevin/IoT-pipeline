# IoT Environmental Monitoring System (ESP8266 + MQTT + Node-RED + InfluxDB + Grafana)

This project is a complete end-to-end IoT system that collects temperature and humidity data using an ESP8266 with an SHT30 sensor, transmits it via MQTT, processes it with Node-RED, stores it in InfluxDB, and visualizes it in Grafana with real-time dashboards and alerts.

---

# Requirements

## Hardware

* ESP8266 board, such as a Wemos D1 mini
* SHT30 temperature and humidity sensor
* USB cable for flashing and power
* Local network with access to the MQTT broker machine

## Software and Tools

* Arduino IDE 2.x or Arduino IDE 1.8.x
* ESP8266 board package for Arduino IDE
* WEMOS SHT3x Arduino Library
* PubSubClient library for MQTT
* Node-RED
* Mosquitto MQTT broker
* InfluxDB
* Grafana
* A web browser for dashboard access

## Optional but Useful

* VS Code for editing the project files
* Serial monitor tool for debugging sensor output

# Architecture Overview

ESP8266 (SHT30 Sensor)
→ MQTT Broker (Mosquitto)
→ Node-RED (Processing & formatting)
→ InfluxDB (Time-series storage)
→ Grafana (Visualization & alerts)

---

# Hardware Layer

- ESP8266 (Wemos D1 mini)
- SHT30 temperature & humidity sensor (I2C)

The ESP8266 reads:
- Temperature (°C)
- Humidity (%)

Then publishes data via MQTT topics:
- `iot/weather/temp`
- `iot/weather/hum`

---

# Communication Layer (MQTT)

MQTT broker used: Mosquitto

Role:
- Lightweight messaging system
- Transfers sensor data between ESP8266 and backend

---

# Processing Layer (Node-RED)

:contentReference[oaicite:0]{index=0}  

Flow:
MQTT IN → Function → InfluxDB OUT

Role:
- Subscribe to MQTT topics
- Extract temperature & humidity values
- Convert values to numeric format
- Structure data for database storage

---

# Database Layer (InfluxDB)

:contentReference[oaicite:1]{index=1}  

- Bucket: `ENI`
- Measurements:
  - temperature
  - humidity
- Field: `value`
- Stores time-series sensor data with timestamps

---

# Visualization Layer (Grafana)

:contentReference[oaicite:2]{index=2}  

Features:
- Real-time graphs
- Time-series analysis
- Aggregation (mean, max, min)
- Alert system

Example query:
```flux
from(bucket: "ENI")
  |> range(start: -1h)
  |> filter(fn: (r) => r._measurement == "temperature")
  |> filter(fn: (r) => r._field == "value")
```
---

## Credits

* WEMOS SHT3x Arduino Library by Wemos: [https://github.com/wemos/WEMOS_SHT3x_Arduino_Library](https://github.com/wemos/WEMOS_SHT3x_Arduino_Library)

* Working influxDB version 2.x : https://github.com/influxdata/influxdb/releases