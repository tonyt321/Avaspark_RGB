#pragma once

/*
 * Welcome! 
 * You can use the file "my_config.h" to make changes to the way WLED is compiled!
 * It is possible to enable and disable certain features as well as set defaults for some runtime changeable settings.
 * 
 * How to use:
 * PlatformIO: Just compile the unmodified code once! The file "my_config.h" will be generated automatically and now you can make your changes.
 * 
 * ArduinoIDE: Make a copy of this file and name it "my_config.h". Go to wled.h and uncomment "#define WLED_USE_MY_CONFIG" in the top of the file.
 * 
 * DO NOT make changes to the "my_config_sample.h" file directly! Your changes will not be applied.
 */

// force the compiler to show a warning to confirm that this file is included
#warning **** my_config.h: Settings from this file are honored ****

/* Uncomment to use your WIFI settings as defaults
  //WARNING: this will hardcode these as the default even after a factory reset
*/
#ifdef OW_PINT
  #define DATA_PINS 18 ,17  //front then back lights GPIO segment 0 then 1 ect
  #define USERMOD_ANDON_MOD
  #define DEFAULT_LED_COUNT 13 //11 for GT 13 for pint
  #define OW_TYPE "Pint"
  #define WLED_VERSION "Pint 2.2.2"  //shows up in info on UI
  #define FRONT_LIGHT_R_PIN 39   //front light red
  #define FRONT_LIGHT_W_PIN 36   //front light white
#endif

#ifdef OW_GT
  #define DATA_PINS 18 ,17  //front then back lights GPIO segment 0 then 1 ect
  #define USERMOD_ANDON_MOD
  #define DEFAULT_LED_COUNT 11 //11 for GT 13 for pint
  #define OW_TYPE "GT"
  #define WLED_VERSION "GT 2.2.2"  //shows up in info on UI
  #define FRONT_LIGHT_R_PIN 39   //front light red
  #define FRONT_LIGHT_W_PIN 36   //front light white
#endif

#ifdef OW_VESC
  #define DATA_PINS 18 ,19  //front then back lights GPIO segment 0 then 1 ect
  #define USERMOD_VESC
  #define DEFAULT_LED_COUNT 11 //11 for GT 13 for pint
  #define OW_TYPE "Vesc"
  #define WLED_VERSION "Vesc 2.2.2"  //shows up in info on UI
  #define VESC_RX 32   //vesc rx
  #define VESC_TX 33   //vesc tx
  #define FRONT_LIGHT_R_PIN 39   //front light red
  #define FRONT_LIGHT_W_PIN 36   //front light white
#endif

#define INTERFACE_UPDATE_COOLDOWN 2000 //time in ms to wait between websockets, alexa, and MQTT updates



//#define TEST_MODE //comment out for normal mode
// disables turning lights off when detected from what should be the OW controller as off
//#define PRO_VERSION    // comment out for standard
#define SERVERNAME "AvaSpark-RGB"



//disable testing wifi for fast AP start up
//#define CLIENT_SSID "TRENDnet828_2.4GHz_3FDB"  //for testing only will be disabled in production should be blank
//#define CLIENT_PASS "8280RH90029"    //for testing only will be disabled in production should be blank


#define ERROR_LED_PIN 16  // on pcb led


#define MOTOR_SPEED_PIN 35   //motor phase pin
#define BATTERY_VOLTAGE_PIN 34   //battery voltage pin



#define DEFAULT_AP_PASS     "avasparkrgb"
#define DEFAULT_OTA_PASS    "avasparkrgb"


#define DEFAULT_LED_TYPE TYPE_SK6812_RGBW
//#define WLED_AP_SSID_UNIQUE
#define WLED_DISABLE_1D_AUDIO
#define WLED_DISABLE_2D

#define BLINK_APP_LIGHTS_DELAY 1000 //ms between blinks
#define ALPHA 0.15 // how much smoothing is used for accel readings

//#define MAX_LEDS 1500       //Maximum total LEDs. More than 1500 might create a low memory situation on ESP8266.


//#define DEFAULT_BRIGHTNESS  255
//#define DEFAULT_MODE       (uint8_t)0
//#define DEFAULT_SPEED      (uint8_t)128
//#define DEFAULT_INTENSITY  (uint8_t)128 
//#define DEFAULT_COLOR      (uint32_t)0x000000