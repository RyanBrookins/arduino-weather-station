# Wiring Guide (Arduino Uno / classic Nano)

Connect power only with the Arduino unplugged. Pin labels on your modules may vary; follow the labels printed on your actual boards.

| Module pin | Arduino pin | Notes |
|------------|-------------|-------|
| OLED VCC | 5V or 3.3V as marked on module | Many SSD1306 breakouts accept 5V; check your board |
| OLED GND | GND | Common ground |
| OLED SDA | A4 (SDA) | Uno / classic Nano I²C data |
| OLED SCL | A5 (SCL) | Uno / classic Nano I²C clock |
| DHT22 VCC / + | 5V | Power |
| DHT22 GND / − | GND | Common ground |
| DHT22 DATA / OUT | D2 | For a bare 4-pin DHT22, add a 10 kΩ pull-up from DATA to VCC |
| MQ-135 VCC | 5V | Heater can draw substantial current; use a stable supply |
| MQ-135 GND | GND | Common ground |
| MQ-135 AO / AOUT | A0 | Analog output used by the sketch; DO is not used |

## Power and safety
The MQ-135 contains a heater and may draw more current than a small USB port or regulator can comfortably provide. Use a stable supply rated for the board and sensor load, and connect all grounds together.

The MQ-135 reading is **not** a calibrated ppm value and must **not** be used as a life-safety or carbon-monoxide alarm.

## Troubleshooting
- **OLED stays black:** confirm VCC/GND, SDA=A4, SCL=A5 on Uno/Nano. Try I²C address `0x3D` if it is not `0x3C` — don't change it unless your module uses it.
- **COM port access denied on upload:** close the Serial Monitor and any other program using the port, then reselect the port.
- **DHT shows `--.-`:** check DATA on D2, power and ground, and the pull-up resistor if using a bare sensor.
- **Air value moves or stays high:** let the MQ-135 warm up; raw readings vary with module and environment and are not calibrated air-quality units.
