#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SHTC3_ADDR 0x70
#define OLED_ADDR 0x3C

#define TEMP_LED D7
#define HUM_LED D8 

// tolerances for turning on the warning leds
const float TEMP_ON = 30.0;
const float TEMP_OFF = 28.0;
const float HUM_ON = 70.0;
const float HUM_OFF = 65.0;

bool tempWarning = false;
bool humidityWarning = false;


// setup oled
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  pinMode(TEMP_LED, OUTPUT);
  pinMode(HUM_LED, OUTPUT);
  Serial.begin(9600);
  Wire.begin();

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  delay(100);
}

void loop() {
  wakeSensor();
  measure();
  float humidity;
  float temperature;
  if (read(humidity, temperature)){ // if reading succeded, then display data
    displayData(humidity, temperature);
  }

  // temperature warning
  if (!tempWarning && temperature >= TEMP_ON){
    digitalWrite(TEMP_LED, HIGH);
    tempWarning = true;
  }
  else if (tempWarning && temperature <= TEMP_OFF){
    digitalWrite(TEMP_LED, LOW);
    tempWarning = false;
  }

  // humidity warning
  if (!humidityWarning && humidity >= HUM_ON){
    digitalWrite(HUM_LED, HIGH);
    humidityWarning = true;
  }
  else if (humidityWarning && humidity <= HUM_OFF){
    digitalWrite(HUM_LED, LOW);
    humidityWarning = false;
  }

  sleepSensor();
  delay(2000);
}

void sleepSensor(){
  Wire.beginTransmission(SHTC3_ADDR);
  // send sleep signal
  Wire.write(0xB0); // MSB
  Wire.write(0x98); // LSB
  Wire.endTransmission();
  delay(1);
}

void displayData(float humidity, float temperature){
  display.clearDisplay();
  // print temperature on the left
  display.setCursor(0,0);
  display.print("Temperature: ");
  display.setCursor(0, 24);
  display.print(temperature, 1);
  display.print(" C");

  // print humdity on the right
  display.setCursor(40, 0);
  display.print("Humidity: ");
  display.setCursor(40, 24);
  display.print(humidity, 1);
  display.print(" %");

  display.display();
}

void wakeSensor(){
  // wake up
  Wire.beginTransmission(SHTC3_ADDR);
  // send wakeup signal
  Wire.write(0x35); // MSB
  Wire.write(0x17); // LSB
  Wire.endTransmission();
  delay(1);
}

void measure(){
  // send temperature first with clock stretching
  Wire.beginTransmission(SHTC3_ADDR);
  Wire.write(0x7C); // MSB
  Wire.write(0xA2); // LSB
  Wire.endTransmission();
}

void displayError(){
  display.clearDisplay();
  display.setCursor(24,4);
  display.print("CRC ERROR!");
  display.display();
}

bool read(float &humidity, float &temperature){
  Wire.requestFrom(SHTC3_ADDR, 6); // request 6 bytes from the sensor (humidity msb, lsb, crc, temperature msb, lsb, crc)
  if (Wire.available() < 6) return false; // error if less than 6 bytes are recieved

  uint8_t data[6]; // since there are 6 bytes, they will be stored in a array with type uint8_t which is one byte
  for (int i = 0; i < 6; i++){
    data[i] = Wire.read(); // read each byte one by one
  }

  // check CRC for humidity
  if (checkCRC(data, 2) != data[2]){ // data is a pointer to the first value in the array and data[2] is the location of the crc in the response
    displayError();
    return false;
  }

  // check CRC for temperature  
  if (checkCRC(data + 2, 2) != data[5]){ // the temperature data is 2 spaces after the first value 
    displayError();
    return false;
  }

  // convert data into one 16 bit number
  uint16_t raw_humidity, raw_temperature;
  raw_humidity = (data[0] << 8) | data[1];
  raw_temperature = (data[3] << 8) | data[4];

  humidity = 100.0f * (raw_humidity / 65536.0f);
  temperature = -45.0f + (175.0f * (raw_temperature / 65536.0f));

  return true;
}


uint8_t checkCRC(uint8_t *data, uint8_t len){ // * is the location of the data
  uint8_t crc = 0xFF; // the initialisation
  for (uint8_t i = 0; i < len; i++){ // iterate over each byte
    crc ^= data[i]; // apply XOR (if the inputs don't match, then return 1 else 0)
    for (uint8_t bit = 0; bit < 8; bit++){ // iterate over every bit in the byte from earlier
      if (crc & 0x80) crc = (crc << 1) ^ 0x31; // check if the polynomial needs to be applied (if the msb is 1 then yes)
      else crc << 1; // if not, then just shift the crc by one
    }
  }
  return crc;
}