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
#define DEFAULT_LED_COUNT 13 //11 for GT 13 for pint

#define MODEL 0    // what model of onewheel is this for? 
//0   1    2   3
//GT Pint XR Vesc
// should effect voltage curves motor speed estimates as voltages are diffrent
// if vesc should bring up option to input number of cells 

//#define PRO_VERSION    // comment out for standard
#define SHOP_NAME "Andon"    // Andon Origin shop who installed it just andon for customer installed versions
#define WLED_VERSION "2.2.2"  //shows up in info on UI 
#define TEST_MODE //comment out for normal mode
// disables turning lights off when detected from what should be the OW controller as off

//disable testing wifi for fast AP start up
//#define CLIENT_SSID "TRENDnet828_2.4GHz_3FDB"  //for testing only will be disabled in production should be blank
//#define CLIENT_PASS "8280RH90029"    //for testing only will be disabled in production should be blank    

#define wifi_on_time 60  // how many sec wifi is on at boot
//#define LEDPIN 26           //front rgbw lights
#define DATA_PINS 18 ,17  //front then back lights GPIO segment 0 then 1 ect
#define LIGHT_BAR_R_PIN 36  //status bar red
#define LIGHT_BAR_G_PIN 39   //status bar green
#define LIGHT_BAR_B_PIN 18   //status bar blue

#define MOTOR_SPEED_PIN 35   //motor phase pin
#define BATTERY_VOLTAGE_PIN 34   //battery voltage pin

#define FRONT_LIGHT_R_PIN 36   //status bar blue
#define FRONT_LIGHT_W_PIN 39   //status bar blue

#define DEFAULT_AP_PASS     "andon123"
#define DEFAULT_OTA_PASS    "andon123"

#define DEFAULT_LED_TYPE TYPE_SK6812_RGBW


//#define MAX_LEDS 1500       //Maximum total LEDs. More than 1500 might create a low memory situation on ESP8266.