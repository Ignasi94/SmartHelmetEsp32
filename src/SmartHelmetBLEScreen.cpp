#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include <BH1750.h>


#include "icons.h"
#include "NameStreet.h"
#include "SpeedLimit.h"
#include "distanceRemaining.h"
#include <Adafruit_I2CDevice.h>
#include "BluetoothA2DPSink.h"
#include "TITTIMER.h"
#include "SwButton.h"
#include "Windows.h"
#include "BrightControl.h"
#include "DisplayControl.h"
#include "PWM.h"

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

//bUTTONS INIT
Button * but_up, *but_low, *but_left, *but_right, *but_center;
Window * window;
bool windows_changed = false;
Bright * bright;
Display * display;
PWM * pwm;



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
  

  /*
     Serial setup
  */
  Serial.begin(115200);
  Serial.println("Starting BLE!");


  /*
     LCD TFT 1.8 SETUP
  */
  // Use this initializer if using a 1.8" TFT screen:
  tft.initR(INITR_BLACKTAB);      // Init ST7735S chip, black tab
  tft.setRotation(1);

  Serial.println("LCD Init");

  uint16_t time = millis();

  time = millis() - time;

  delay(500);

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

  // Initializing the device with its name
  BLEDevice::init("SmartHelmet"); 

  // Create the Server
  BLEServer *pServer = BLEDevice::createServer(); 

  // Adding a service with his UUID into Server
  BLEService *pService = pServer->createService(SERVICE_UUID); 
  
  // Creating a characteristic with an UUID and name "SHelmet"
  BLECharacteristic *nameCharacteristic = pService->createCharacteristic(
      BLEUUID((uint16_t)0x2A00),
      BLECharacteristic::PROPERTY_READ);
                                          
  nameCharacteristic->setValue("SHelmet");

  // Adding the characteristic of maneuver
  pService->addCharacteristic(&maneuverCharacteristic);
  maneuverCharacteristic.setValue("0"); 

  // Adding a characteristic of speed
  pService->addCharacteristic(&speedCharacteristic);
  speedCharacteristic.setValue("0"); 

  // Adding a characteristic of name of street
  pService->addCharacteristic(&streetCharacteristic);
  streetCharacteristic.setValue("0"); 

  // Adding a characteristic remaining distance
  pService->addCharacteristic(&distanceCharacteristic);
  distanceCharacteristic.setValue("0"); // Value uint8_t with length 1
  //streetCharacteristic.setCallbacks(new distanceRemainingCallback());

  // Setting Characteristic's Callbacks
  speedCharacteristic.setCallbacks(new NextSpeedCallback());
  maneuverCharacteristic.setCallbacks(new NextManeuverIdCallback());
  streetCharacteristic.setCallbacks(new NextStreetCallback());
  distanceCharacteristic.setCallbacks(new distanceRemainingCallback());

  // Setting Server Callbacks
  pServer->setCallbacks(new ServerCallbacks());

  // Starting Server
  pService->start();


  Serial.println("Creating advertising");
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();

  Serial.println("Char defined! Now you can read it in your phone!");
  pinMode (LED, OUTPUT); // Set the LED pin as OUTPUT

  //Switch inputs pullup
  pinMode (BUTTON_UP, INPUT); // Set the LED pin as OUTPUT
  pinMode (BUTTON_LOW, INPUT); 
  pinMode (BUTTON_LEFT, INPUT); 
  pinMode (BUTTON_RIGHT, INPUT); 
  pinMode (BUTTON_CENTER, INPUT); 

  pinMode (32, OUTPUT); // Set the LED pin as OUTPUT

  //Timer init
  TiInit();

  //BUTTONS CREATE
  
  but_up = Button_Create(BUTTON_UP);
  but_low = Button_Create(BUTTON_LOW);
  but_left = Button_Create(BUTTON_LEFT);
  but_right = Button_Create(BUTTON_RIGHT);
  but_center = Button_Create(BUTTON_CENTER);

  //Windows create
  window = WINDOW_Create();
    
  //Bright Control create
  bright = Bright_Create();

  //Display Control create
  display = Display_Create();

  //PWM create
  pwm = PWM_Create(PIN_PWM);
  
}

void loop() {

  bool b_up = Motor_Button(but_up);
  bool b_low = Motor_Button(but_low);
  bool b_left = Motor_Button(but_left);
  bool b_right = Motor_Button(but_right);
  bool b_center = Motor_Button(but_center);
  

  windows_changed = Motor_WINDOW_SELECT(window, b_left, b_right);

  switch (window->status_machine)
  {
    case WINDOW_NAVIGATION:
        //Serial.println("Window navigation...");
        break;

    case WINDOW_BRIGHT:
        Motor_Bright_Control(bright, b_up, b_low, b_center, windows_changed, &tft);
        break;

    case WINDOW_DISPLAY:
        Motor_Display_Control(display, b_up, b_low, b_center, windows_changed, &tft);
        //Serial.println("Window display...");
        break;
    
    case WINDOW_VOLUMEN:
        //Serial.println("Window volumen...");
        break;
    default:
        break;
  }
  Motor_PWM(pwm, *bright);
}




