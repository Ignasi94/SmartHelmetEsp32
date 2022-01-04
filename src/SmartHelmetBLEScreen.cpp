/*
    Simple sketch to control a led with BLE protocol by
    Daniel Carrasco (https://www.electrosoftcloud.com)
*/
/*
   #define ST77XX_BLACK 0x0000
  #define ST77XX_WHITE 0xFFFF
  #define ST77XX_RED 0xF800
  #define ST77XX_GREEN 0x07E0
  #define ST77XX_BLUE 0x001F
  #define ST77XX_CYAN 0x07FF
  #define ST77XX_MAGENTA 0xF81F
  #define ST77XX_YELLOW 0xFFE0
  #define ST77XX_ORANGE 0xFC00*/

//#include <Arduino.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>


#include "icons.h"
#include "NameStreet.h"
#include "SpeedLimit.h"
#include "distanceRemaining.h"
#include <Adafruit_I2CDevice.h>
#include "BluetoothA2DPSink.h"


//Defines of UUID's BLE
// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
#define SERVICE_UUID        "3feb1e8a-3981-4045-ad39-b225135013a0"
#define MANEUVER_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define SPEED_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a9"
#define STREET_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a7"
#define DISTANCE_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a6"

#define LED 23


//Defines of tft lcd 1.8" screen
#define TFT_CS          5
#define TFT_RST         15 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC          13
#define TFT_MOSI        23  // Data out
#define TFT_SCLK        18  // Clock out


volatile unsigned long elapsedTimeInMicroseconds = 0;
int firstBLEWrite = 0, firstBLENameStreet = 0, firstSpeedLim = 0;
//int last_has_2_lines = 0;
char lastRxValue[100];


static int clean_text_name_of_street = 1;

static int clear_remain_meters = 0;

int come_from_disconnect_speed_limiter = 0;

BluetoothA2DPSink a2dp_sink;






// For 1.44" and 1.8" TFT with ST7735 use:
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// Callback function reciving id Maneuver
class NextManeuverIdCallback: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      int static last_id_maneuver = 0;
     
      //time between interrupts
      static unsigned long previousTimeInMicroseconds = 0;
      unsigned long time = micros();
      elapsedTimeInMicroseconds = time - previousTimeInMicroseconds;
      previousTimeInMicroseconds = time;

      if (elapsedTimeInMicroseconds > 10000) {
        BLEDevice::startAdvertising();

      }

      uint8_t* rxValue = pCharacteristic->getData();
      //if (rxValue.length() > 0) {
      Serial.println("ID MANEUVER... new value: " );
      Serial.println(rxValue[0]);

      if (firstBLEWrite == 0) {
        firstBLEWrite++;
        //Si es la primera vez  que escrivimos, limpiamos pantalla
        //printIcon(2, 0, -5, -5, &tft);
        tft.fillScreen(ST77XX_WHITE);
        printIcon(1, (int)rxValue[0], -10, -10, &tft);
      }else{
        if (last_id_maneuver != (int)rxValue[0]){
          printIcon(1, (int)rxValue[0], -10, -10, &tft);
        }
      }
      last_id_maneuver = (int)rxValue[0];
    }
};

// Callback function distance Remaining for nex maneuver
class distanceRemainingCallback: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      
      uint8_t* rxValue = pCharacteristic->getData();
      int totalMetr = atoi((char *)rxValue);
      printRemainingDistance(totalMetr, &tft, &clear_remain_meters);
      
    }
};

// Callback function next street
class NextStreetCallback: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      uint8_t* rxValue = pCharacteristic->getData();
      setNameStreet((char *) rxValue, 5, 110, &tft);
    }
};

// Callback function actual speed limit
class NextSpeedCallback: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      uint8_t* rxValue = pCharacteristic->getData();
      //if (rxValue.length() > 0) {
      Serial.println("SPEED... new value: " );
      Serial.println(rxValue[0]);
      setSpeedLimitCircle(124, 35, 6, 32, (int)rxValue[0], &tft, &come_from_disconnect_speed_limiter);
    }
};

void(* resetFunc) (void) = 0;

class ServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* MyServer) {
      come_from_disconnect_speed_limiter = 1;
      clear_remain_meters = 1;
      firstBLEWrite = 0;
      firstSpeedLim = 0;
      clean_text_name_of_street = 1;
      Serial.println("new cli\n");
      a2dp_sink.start("SmartHelmetA2dp");
      tft.fillScreen(ST77XX_WHITE);
      //Print waiting ble icon in the midle of the screen
      printIcon(0, 1, 30, 14, &tft);
      //Print tex waiting connection
      tft.setTextWrap(false);

      tft.setCursor(25, 110);
      tft.setTextColor(ST77XX_GREEN);
      tft.setTextSize(2);
      tft.println("Connected");
    };
    void onDisconnect(BLEServer* MyServer) {
      come_from_disconnect_speed_limiter = 1;
      clear_remain_meters = 1;
      BLEDevice::startAdvertising();
      tft.fillScreen(ST77XX_WHITE);
      firstBLEWrite = 0;
      firstSpeedLim = 0;
      clean_text_name_of_street = 1;
      printIcon(0, 0, 30, 14, &tft);
      tft.setCursor(10, 110);
      tft.setTextColor(ST77XX_BLACK);
      tft.setTextSize(2);
      tft.println("Searching...");
      Serial.println("client disc\n");
      resetFunc();
    }
};

// New characteristic with object to manage it maneuver
BLECharacteristic maneuverCharacteristic(
  MANEUVER_CHARACTERISTIC_UUID,
  BLECharacteristic::PROPERTY_READ |
  BLECharacteristic::PROPERTY_WRITE |
  BLECharacteristic::PROPERTY_WRITE_NR
);

// New characteristic with object to manage it speed
BLECharacteristic speedCharacteristic(
  SPEED_CHARACTERISTIC_UUID,
  BLECharacteristic::PROPERTY_READ |
  BLECharacteristic::PROPERTY_WRITE |
  BLECharacteristic::PROPERTY_WRITE_NR
);

// New characteristic with object to manage name of street
BLECharacteristic streetCharacteristic(
  STREET_CHARACTERISTIC_UUID,
  BLECharacteristic::PROPERTY_READ |
  BLECharacteristic::PROPERTY_WRITE |
  BLECharacteristic::PROPERTY_WRITE_NR
);

// New characteristic with object to manage name of street
BLECharacteristic distanceCharacteristic(
  DISTANCE_CHARACTERISTIC_UUID,
  BLECharacteristic::PROPERTY_READ |
  BLECharacteristic::PROPERTY_WRITE |
  BLECharacteristic::PROPERTY_WRITE_NR
);

void i2sPinConfig(){
  i2s_pin_config_t my_pin_config = {
      .bck_io_num = 17,//23,
      .ws_io_num = 25,
      .data_out_num = 16,//18,
      .data_in_num = I2S_PIN_NO_CHANGE        
  };
  a2dp_sink.set_pin_config(my_pin_config);
}

void i2sConfig(){
  static i2s_config_t i2s_config = {
    .mode = (i2s_mode_t) (I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = 88200, // updated automatically by A2DP
    .bits_per_sample = (i2s_bits_per_sample_t) I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ALL_LEFT,
    .communication_format = (i2s_comm_format_t) (I2S_COMM_FORMAT_STAND_I2S),
    .intr_alloc_flags = 0, // default interrupt priority
    .dma_buf_count = 8,
    .dma_buf_len = 64,
    .use_apll = true,
    .tx_desc_auto_clear = true // avoiding noise in case of data unavailability
  };
  a2dp_sink.set_i2s_config(i2s_config); 
}

void blClassicSetup(){
  i2sPinConfig();
  i2sConfig();
  

}

void setup() {
  
    /*come_from_disconnect_speed_limiter = 1;
    clear_remain_meters = 1;
    firstBLEWrite = 0;
    firstSpeedLim = 0;
    clean_text_name_of_street = 1;
    come_from_disconnect_speed_limiter = 1;
    clear_remain_meters = 1;
    BLEDevice::startAdvertising();
    tft.fillScreen(ST77XX_WHITE);
    firstBLEWrite = 0;
    firstSpeedLim = 0;*/
  /*
     Serial setup
  */
  Serial.begin(115200);
  Serial.println("Starting BLE!");
  //Serial.println("Initializing device");

  /*
     LCD TFT 1.8 SETUP
  */
  // Use this initializer if using a 1.8" TFT screen:
  tft.initR(INITR_BLACKTAB);      // Init ST7735S chip, black tab
  tft.setRotation(1);

  Serial.println("LCD Init");

  uint16_t time = millis();

  time = millis() - time;

  //Serial.println(time, DEC);
  delay(500);

  //printIcon(55, 39);
  tft.fillScreen(ST77XX_WHITE);
  //Print waiting ble icon in the midle of the screen
  printIcon(0, 0, 30, 14, &tft);
  //Print tex waiting connection
  tft.setTextWrap(false);

  tft.setCursor(10, 110);
  tft.setTextColor(ST77XX_BLACK);
  tft.setTextSize(2);
  tft.println("Searching...");

  Serial.println("lcd done");
  delay(1000);


  /*
    BL CLASSIC SETUP
  */

  blClassicSetup();
  //a2dp_sink.start("SmartHelmet");

  /*
     BLE SETUP
  */

  BLEDevice::init("SmartHelmet"); // Initializing the device with its name
  Serial.println("Creating server");
  BLEServer *pServer = BLEDevice::createServer(); // Create the server
  Serial.println("Adding service UUID");
  BLEService *pService = pServer->createService(SERVICE_UUID); // Creating a new service into server
  // Adding a characteristic maneuver with the object name (official UUID), without object (this characteristic will not change)
  Serial.println("Adding name characteristic");

  BLECharacteristic *nameCharacteristic = pService->createCharacteristic(
      BLEUUID((uint16_t)0x2A00),
      BLECharacteristic::PROPERTY_READ
                                          );
  nameCharacteristic->setValue("id");


  // Adding a characteristic maneuver
  //Serial.println("Adding char");
  pService->addCharacteristic(&maneuverCharacteristic);
  maneuverCharacteristic.setValue("0"); // Value uint8_t with length 1
  //maneuverCharacteristic.setCallbacks(new NextManeuverIdCallback());


  // Adding a characteristic speed
  //Serial.println("Adding control char");
  pService->addCharacteristic(&speedCharacteristic);
  speedCharacteristic.setValue("0"); // Value uint8_t with length 1


  // Adding a characteristic name of street
  //Serial.println("Adding control characteristic");
  pService->addCharacteristic(&streetCharacteristic);
  streetCharacteristic.setValue("0"); // Value uint8_t with length 1
  //streetCharacteristic.setCallbacks(new NextStreetCallback());

  // Adding a characteristic name of street
  //Serial.println("Adding control characteristic");
  pService->addCharacteristic(&distanceCharacteristic);
  distanceCharacteristic.setValue("0"); // Value uint8_t with length 1
  //streetCharacteristic.setCallbacks(new distanceRemainingCallback());


  speedCharacteristic.setCallbacks(new NextSpeedCallback());
  maneuverCharacteristic.setCallbacks(new NextManeuverIdCallback());
  streetCharacteristic.setCallbacks(new NextStreetCallback());
  distanceCharacteristic.setCallbacks(new distanceRemainingCallback());

  pServer->setCallbacks(new ServerCallbacks());
  Serial.println("Starting...");
  pService->start();
  Serial.println("Creating advertising");
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();

  /*delay(3000);
  a2dp_sink.start("SmartHelmetA2dp");*/

  Serial.println("Char defined! Now you can read it in your phone!");
  pinMode (LED, OUTPUT); // Set the LED pin as OUTPUT



    //TEST ZONE

    //Test of setspeedlimitcircle
    /*tft.fillScreen(ST77XX_WHITE);
      setSpeedLimitCircle(120, 35, 6, 32, 50);




    //Test of setNameStreet
    setNameStreet((char *)("AAAAABBBBBCCCCCAAAAAAAAAA "), 5, 110);
    delay(2000);
    setNameStreet((char *)("xxxxxxxxxxxxxxxxxxxxxxXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXxxxxxxxxxxxxxxxxx"), 5, 110);
    delay(2000);
    setNameStreet((char *)("ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ"), 5, 110);
    delay(2000);

    //Test of setNameStreet
    setNameStreet((char *)("AAAAABBBBBCCCCCAAAAAAAAAA BBBBCCC"), 5, 110);
    delay(2000);
    setNameStreet((char *)("xxxxxxxxxxxxxxxxxxxx"), 5, 110);
    delay(2000);
    setNameStreet((char *)("ZZZZZZZ"), 5, 110);
    tft.fillScreen(ST77XX_WHITE);
    printIcon(1, 17, 0, 0, &tft);
    delay(500);

    //tft.fillScreen(ST77XX_WHITE);
    printIcon(1, 11, 0, 0, &tft);
    delay(500);

    tft.fillScreen(ST77XX_WHITE);
    printIcon(1, 58, 0, 0, &tft);
    delay(500);

    //tft.fillScreen(ST77XX_WHITE);
    printIcon(1, 61, 0, 0, &tft);
    delay(500);

    printIcon(1, 17, 0, 0, &tft);
    delay(500);

    //tft.fillScreen(ST77XX_WHITE);
    printIcon(1, 11, 0, 0, &tft);
    delay(500);

    tft.fillScreen(ST77XX_WHITE);
    printIcon(1, 58, 0, 0, &tft);
    delay(500);

    //tft.fillScreen(ST77XX_WHITE);
    printIcon(1, 61, 0, 0, &tft);
    delay(500);

    //tft.fillScreen(ST77XX_WHITE);
    printIcon(1, 17, 0, 0, &tft);
    delay(500);

    tft.fillScreen(ST77XX_WHITE);
    printIcon(3, 58, 0, 0, &tft);
    delay(500);
    
    tft.fillScreen(ST77XX_WHITE);
    //tft.fillScreen(ST77XX_WHITE);
    printIcon(4, 61, 0, 0, &tft);
    delay(500);

    tft.fillScreen(ST77XX_WHITE);
    //tft.fillScreen(ST77XX_WHITE);
    printIcon(5, 17, 0, 0, &tft);
    delay(500);

    tft.fillScreen(ST77XX_WHITE);     //tft.fillScreen(ST77XX_WHITE);
    printIcon(6, 1, 0, 0, &tft);
    delay(500);*/


    //tft.fillScreen(ST77XX_WHITE);
    /*tft.drawBitmap(0, 0, image_data_direction_continue_uturn, 100, 100, 0x0000, 0xffff);
    delay(2000);
    
    //tft.fillScreen(ST77XX_WHITE);
    tft.drawBitmap(0, 0, image_data_direction_arrive_right, 100, 100, 0x0000, 0xffff);
    delay(2000);

    //tft.fillScreen(ST77XX_WHITE);
    tft.drawBitmap(0, 0, image_data_direction_continue_uturn, 100, 100, 0x0000, 0xffff);
    delay(2000);
    
    //tft.fillScreen(ST77XX_WHITE);
    tft.drawBitmap(0, 0, image_data_direction_arrive_right, 100, 100, 0x0000, 0xffff);
    delay(2000);*/
    //drawBitmap();   
}

void loop() {
}




