#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// ---------- OLED ----------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ---------- DHT22 ----------
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// ---------- MQ135 ----------
#define MQ135_PIN A0

// ---------- AVERAGING BUFFERS ----------
#define BUFFER_SIZE 15
float tempBuffer[BUFFER_SIZE];
float humBuffer[BUFFER_SIZE];
int airBuffer[BUFFER_SIZE];
int indexPos = 0;
int samplesStored = 0;

// ---------- AVERAGING ----------
float averageFloat(float arr[]) {
  if (samplesStored == 0) return 0;
  float sum = 0;
  for (int i = 0; i < samplesStored; i++) {
    sum += arr[i];
  }
  return sum / samplesStored;
}

int averageInt(int arr[]) {
  if (samplesStored == 0) return 0;
  long sum = 0;
  for (int i = 0; i < samplesStored; i++) {
    sum += arr[i];
  }
  return sum / samplesStored;
}

// ---------- TREND ----------
char trend(float current, float average, float deadband) {
  if (current > average + deadband) return '^';
  if (current < average - deadband) return 'v';
  return '-';
}

// ---------- ASCII BOOT SCREEN ----------
void bootScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(" WEATHER STATION");
  display.println(" .------------.");
  display.println(" |    TEMP    |");
  display.println(" |  HUMIDITY  |");
  display.println(" |    AIR     |");
  display.println(" '------------'");
  display.println(" SYSTEM READY");
  display.display();
  delay(1800);
}

// ---------- SETUP ----------
void setup() {
  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED initialization failed. Check address/wiring.");
    while (true) {
      // Stop here if the OLED cannot initialize.
    }
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);

  dht.begin();
  bootScreen();

  for (int i = 0; i < BUFFER_SIZE; i++) {
    tempBuffer[i] = 0;
    humBuffer[i] = 0;
    airBuffer[i] = 0;
  }
}

// ---------- MAIN LOOP ----------
void loop() {
  // Read sensors
  float temp = dht.readTemperature(); // Celsius
  float hum = dht.readHumidity();
  int air = analogRead(MQ135_PIN);    // Raw ADC value, 0-1023 on Uno/Nano

  // Keep invalid DHT readings out of the averages.
  bool validDHT = !isnan(temp) && !isnan(hum);

  if (!validDHT) {
    Serial.println("DHT22 read failed.");
  } else {
    tempBuffer[indexPos] = temp;
    humBuffer[indexPos] = hum;
  }

  airBuffer[indexPos] = air;

  indexPos++;
  if (indexPos >= BUFFER_SIZE) indexPos = 0;

  if (samplesStored < BUFFER_SIZE) {
    samplesStored++;
  }

  // Calculate rolling averages
  float avgTemp = averageFloat(tempBuffer);
  float avgHum = averageFloat(humBuffer);
  int avgAir = averageInt(airBuffer);

  // Trend indicators: ^ above average, v below average, - near average
  char tempTrend = validDHT ? trend(temp, avgTemp, 0.2) : '-';
  char humTrend = validDHT ? trend(hum, avgHum, 1.0) : '-';
  char airTrend = trend(air, avgAir, 5.0);

  // Draw screen
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(0, 0);
  display.println("WEATHER STATION");

  display.setCursor(0, 14);
  display.print("Temp: ");
  if (validDHT) {
    display.print(temp, 1);
    display.print(" C ");
    display.print(tempTrend);
  } else {
    display.print("--.- C");
  }

  display.setCursor(0, 26);
  display.print("Humidity: ");
  if (validDHT) {
    display.print(hum, 1);
    display.print("% ");
    display.print(humTrend);
  } else {
    display.print("--.- %");
  }

  display.setCursor(0, 38);
  display.print("Air raw: ");
  display.print(air);
  display.print(" ");
  display.print(airTrend);

  display.setCursor(0, 52);
  display.print("Trend vs rolling avg");

  display.display();

  delay(2000);
}
