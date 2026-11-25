# Arduino Weather Station

A compact Arduino weather station that reads temperature and relative humidity from a **DHT22** and an analog air-sensor value from an **MQ-135** module. A 128×64 **SSD1306 OLED** shows live readings, an ASCII startup screen, and trend symbols comparing each reading with its rolling average. Intended as a learning and demonstration project for electronics, sensor reading, and Arduino programming.

> The current code uses the OLED, DHT22, and MQ-135 only. It does **not** use the BMP280 pressure sensor from an earlier version, so there is no pressure reading.

## Features
- ASCII startup screen on boot
- Temperature (°C) and relative humidity from the DHT22
- MQ-135 raw analog value (0–1023 on Uno/Nano)
- Trend marker vs. rolling average: `^` above, `v` below, `-` near average
- Updates about every 2 seconds; rolling buffer of up to 15 samples

## Parts
| Part | Notes |
|------|-------|
| Arduino Uno or Nano (5 V) | Code is written for the classic Uno/Nano pin layout |
| 128×64 SSD1306 I²C OLED | Common I²C address `0x3C` |
| DHT22 sensor | Use a 10 kΩ pull-up from DATA to VCC if using a bare 4-pin sensor |
| MQ-135 module | Connect AO to A0; needs warm-up time |
| Jumper wires and breadboard | Prototyping |
| USB cable / suitable 5 V supply | Powers and programs the Arduino |

## Wiring
See [docs/wiring.md](docs/wiring.md) for the full pin table, safety notes, and troubleshooting.

## Software setup
1. Install the [Arduino IDE](https://www.arduino.cc/en/software).
2. In *Library Manager*, install **Adafruit SSD1306**, **Adafruit GFX Library**, and **DHT sensor library** (Adafruit).
3. Open `weather_station/weather_station.ino`.
4. Select your board and port, then upload.

## Safety note
The MQ-135 reading is not calibrated and must not be used as a life-safety or carbon-monoxide alarm.

## License
MIT — see [LICENSE](LICENSE).
