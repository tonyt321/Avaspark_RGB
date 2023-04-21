#pragma once



#include "VescUart.cpp"
#include "VescUart.h"
#include "buffer.cpp"
#include "buffer.h"
#include "crc.cpp"
#include "crc.h"
#include "datatypes.h"
#include "wled.h"
#include "SparkFun_ADXL345.cpp"         // SparkFun ADXL345 Library
#include "SparkFun_ADXL345.h"         // SparkFun ADXL345 Library
#include "SparkFun_Qwiic_Humidity_AHT20.cpp"         // SparkFun ADXL345 Library 
#include "SparkFun_Qwiic_Humidity_AHT20.h"         // SparkFun ADXL345 Library
#include <Wire.h>


AHT20 humiditySensor;

int type = 0;
//int display_battery;
//int display_duty_cycle;
int shop = 0;// shop mode
int motod = 0;// input from msense
int motor_duty_display;
int shutdown_display;
float display_tpmsp; //tpms pressure
float display_tpmst; //tpms temp
float percent_tpmst;
float percent_tpmsp;
int display_trail_ruffness;
int filteredx , filteredy , filteredz;
bool forward = true;
bool dimmed_lights = false;
float battery_voltage;
int vesc_state = 0;
int state_switch = 0;
int humidity = -100;
int andonn_temp = -100;

float mosfettemp;
float motortemp;


int cvt[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  //cell voltage table
int tempt[] = {0, 0, 0, 0, 0};  // battery tempature table

int tv2 = 0; //totale volatage
int ca2 = 0; //current amps
int bmss = 0; //bms state of charge
int ucm = 0; //used charge mah                stuff for pint and XR owie
int rcm = 0; //regen mah




#define PALETTE_SOLID_WRAP (strip.paletteBlend == 1 || strip.paletteBlend == 3)



/*
 * Blink/strobe function
 * Alternate between color1 and color2
 * if(strobe == true) then create a strobe effect
 */
 uint16_t blink(uint32_t color1, uint32_t color2, uint32_t intensity1, bool do_palette, uint32_t speed1) {
  uint32_t cycleTime = (255 - speed1)*20;
  uint32_t onTime = FRAMETIME;
  if (!false) onTime += ((cycleTime * intensity1) >> 8);
  cycleTime += FRAMETIME*2;
  uint32_t it = strip.now / cycleTime;
  uint32_t rem = strip.now % cycleTime;
  
  bool on = false;
  if (it != SEGENV.step //new iteration, force on state for one frame, even if set time is too brief
      || rem <= onTime) { 
    on = true;
  }
  
  SEGENV.step = it; //save previous iteration

  uint32_t color = on ? color1 : color2;
  if (color == color1 && do_palette)
  {
    for (int i = 0; i < SEGLEN; i++) {
      SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(i, true, PALETTE_SOLID_WRAP, 0));
    }
  } else SEGMENT.fill(color);

  return FRAMETIME;
 }


 uint16_t mode_accel_test(void)
 {  
  int x = (filteredx * 3); if(x > 255){x = 255;} if(x < 0){x = 0;}
  int y = (filteredy * 3); if(y > 255){y = 255;} if(y < 0){y = 0;}
  int z = (filteredz * 3); if(z > 255){z = 255;} if(z < 0){z = 0;}
  
  for (int i = 0 ; i < SEGLEN; i++) {SEGMENT.setPixelColor(i, x, y, z, 0);}
  return FRAMETIME;
 }
 static const char _data_fx_mode_accel_test[] PROGMEM = "Accel test@!;;";


 uint16_t mode_stock_front(void)
 {
 if (forward){
  if(dimmed_lights){
  for (int i = 0 ; i < SEGLEN; i++) {SEGMENT.setPixelColor(i, 100, 100, 100, 100);}
  }else{
      for (int i = 0 ; i < SEGLEN; i++) {SEGMENT.setPixelColor(i, 255, 255, 255, 255);}
  }
  }else{
  if(dimmed_lights){
    for (int i = 0 ; i < SEGLEN; i++) {SEGMENT.setPixelColor(i, 100, 0, 0, 0);}
  }else{
  for (int i = 0 ; i < SEGLEN; i++) {SEGMENT.setPixelColor(i, 255, 0, 0, 0);}
 }
 }
  return FRAMETIME;
 }
 static const char _data_fx_mode_stock_front[] PROGMEM = "Stock front@!;;";


 uint16_t mode_stock_back(void)
 {
 if (forward){
  if(dimmed_lights){
  for (int i = 0 ; i < SEGLEN; i++) {SEGMENT.setPixelColor(i, 100, 0, 0, 0);}
  }else{
      for (int i = 0 ; i < SEGLEN; i++) {SEGMENT.setPixelColor(i, 255, 0, 0, 0);}
  }
  }else{
  if(dimmed_lights){
    for (int i = 0 ; i < SEGLEN; i++) {SEGMENT.setPixelColor(i, 100, 100, 100, 100);}
  }else{
  for (int i = 0 ; i < SEGLEN; i++) {SEGMENT.setPixelColor(i, 255, 255, 255, 255);}
 }
 }
  return FRAMETIME;
 }
 static const char _data_fx_mode_stock_back[] PROGMEM = "Stock back@!;;";

 /*
 * blink forwards on accel Intensity sets duty cycle.
 */
 uint16_t mode_f_acceleration_blink(void) {
    int speed1;
    int filteredz1 = filteredz;
    if (filteredz1 < 0){filteredz1 = 0;}
    speed1 = (filteredz1  * (SEGMENT.speed / 10));
    if (speed1 > 255){speed1 = 255;}else if(speed1 < 100){speed1 = 0;}
    return blink(SEGCOLOR(0), SEGCOLOR(1), SEGMENT.intensity, true, speed1);
 }
 static const char _data_fx_f_acceleration_blink[] PROGMEM = "Forwards Accel blink@!,Base speed;!,!;!";

 /*
 * blink backwards on accel Intensity sets duty cycle.
 */
 uint16_t mode_b_acceleration_blink(void) {
    int speed1;
    int filteredz1 = filteredz;
    if (filteredz1 > 0){filteredz1 = 0;}else{filteredz1 = 0 - filteredz1;}
    speed1 = (filteredz1 * (SEGMENT.speed / 10));
    if (speed1 > 255){speed1 = 255;}else if(speed1 < 100){speed1 = 0;}
  return blink(SEGCOLOR(0), SEGCOLOR(1), SEGMENT.intensity, true, speed1);
 }
 static const char _data_fx_b_acceleration_blink[] PROGMEM = "Backwards Accel blink@!,Base speed;!,!;!";

 /*
 * blink backwards on accel Intensity sets duty cycle.
 */
 uint16_t mode_fb_acceleration_blink(void) {
    int speed1;
    int intensity1;

    speed1 = map((filteredz * (SEGMENT.speed / 6)),-255,255,0,255);
    intensity1 = map((filteredz * 5),-255,255,0,255);

    if (speed1 > 255){speed1 = 255;}else if(speed1 < 100){speed1 = 0;}
    if (intensity1 > 255){intensity1 = 255;}else if(intensity1 < 100){intensity1 = 0;}
  return blink(SEGCOLOR(0), SEGCOLOR(1), intensity1, true, speed1);
 }
 static const char _data_fx_fb_acceleration_blink[] PROGMEM = "Both Accel blink@!,!,!;!";


 /*
 * shut down count down display
 * Intesity values from 0-100 turn on the leds.
 */
 uint16_t mode_countdown(void) {
  uint8_t percent = shutdown_display;
  percent = constrain(percent, 0, 200);
  uint16_t active_leds = (percent < 100) ? SEGLEN * percent / 100.0
                                         : SEGLEN * (200 - percent) / 100.0;
  uint8_t size = (1 + (SEGLEN >> 11));

  if (percent <= 100) {
    for (int i = 0; i < SEGLEN; i++) {
    	if (i < SEGENV.aux1) {
        if (SEGMENT.check1)
          SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(map(percent,0,100,0,255), false, false, 0));
        else
          SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(i, true, PALETTE_SOLID_WRAP, 0));
    	}
    	else {
        SEGMENT.setPixelColor(i, SEGCOLOR(1));
    	}
    }
  } else {
    for (int i = 0; i < SEGLEN; i++) {
    	if (i < (SEGLEN - SEGENV.aux1)) {
        SEGMENT.setPixelColor(i, SEGCOLOR(1));
    	}
    	else {
        if (SEGMENT.check1)
          SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(map(percent,100,200,255,0), false, false, 0));
        else
          SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(i, true, PALETTE_SOLID_WRAP, 0));
    	}
    }
  }
  if(active_leds > SEGENV.aux1) {  // smooth transition to the target value
    SEGENV.aux1 += size;
    if (SEGENV.aux1 > active_leds) SEGENV.aux1 = active_leds;
  } else if (active_leds < SEGENV.aux1) {
    if (SEGENV.aux1 > size) SEGENV.aux1 -= size; else SEGENV.aux1 = 0;
    if (SEGENV.aux1 < active_leds) SEGENV.aux1 = active_leds;
  }
 	return FRAMETIME;
 }
 static const char _data_fx_mode_countdown[] = "Shutdown Count down@,% of fill,,,,One color;!,!;!";


 /*
 * trail ratings display Fade LEDs between two colors
 */
 uint16_t mode_countdown_fade(void) {
  uint16_t counter = shutdown_display;

  for (int i = 0; i < SEGLEN; i++) {
    SEGMENT.setPixelColor(i, color_blend(SEGCOLOR(1), SEGMENT.color_from_palette(i, true, PALETTE_SOLID_WRAP, 0), counter));
  }

  return FRAMETIME;
 }
 static const char _data_fx_mode_countdown_fade[] PROGMEM = "Shutdown Count Fade@!;!,!;!";




 /*
 * trail ratings display
 * Intesity values from 0-100 turn on the leds.
 */
 uint16_t mode_rate_trail(void) {
  uint8_t percent = display_trail_ruffness;
  percent = constrain(percent, 0, 200);
  uint16_t active_leds = (percent < 100) ? SEGLEN * percent / 100.0
                                         : SEGLEN * (200 - percent) / 100.0;
  uint8_t size = (1 + (SEGLEN >> 11));

  if (percent <= 100) {
    for (int i = 0; i < SEGLEN; i++) {
    	if (i < SEGENV.aux1) {
        if (SEGMENT.check1)
          SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(map(percent,0,100,0,255), false, false, 0));
        else
          SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(i, true, PALETTE_SOLID_WRAP, 0));
    	}
    	else {
        SEGMENT.setPixelColor(i, SEGCOLOR(1));
    	}
    }
  } else {
    for (int i = 0; i < SEGLEN; i++) {
    	if (i < (SEGLEN - SEGENV.aux1)) {
        SEGMENT.setPixelColor(i, SEGCOLOR(1));
    	}
    	else {
        if (SEGMENT.check1)
          SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(map(percent,100,200,255,0), false, false, 0));
        else
          SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(i, true, PALETTE_SOLID_WRAP, 0));
    	}
    }
  }
  if(active_leds > SEGENV.aux1) {  // smooth transition to the target value
    SEGENV.aux1 += size;
    if (SEGENV.aux1 > active_leds) SEGENV.aux1 = active_leds;
  } else if (active_leds < SEGENV.aux1) {
    if (SEGENV.aux1 > size) SEGENV.aux1 -= size; else SEGENV.aux1 = 0;
    if (SEGENV.aux1 < active_leds) SEGENV.aux1 = active_leds;
  }
 	return FRAMETIME;
 }
 static const char _data_fx_mode_rate_trail[] = "Trail Rating Bar@,% of fill,,,,One color;!,!;!";


 /*
 * trail ratings display Fade LEDs between two colors
 */
 uint16_t mode_rate_trail_fade(void) {
  uint16_t counter = display_trail_ruffness;

  for (int i = 0; i < SEGLEN; i++) {
    SEGMENT.setPixelColor(i, color_blend(SEGCOLOR(1), SEGMENT.color_from_palette(i, true, PALETTE_SOLID_WRAP, 0), counter));
  }

  return FRAMETIME;
 }
 static const char _data_fx_mode_rate_trail_fade[] PROGMEM = "Trail Rating Fade@!;!,!;!";


 /*
 * trail ratings display
 * Intesity values from 0-100 turn on the leds.
 */
 uint16_t mode_tire_pressure(void) {
  int percent = percent_tpmsp;
  percent = constrain(percent, 0, 200);
  uint16_t active_leds = (percent < 100) ? SEGLEN * percent / 100.0
                                         : SEGLEN * (200 - percent) / 100.0;
  uint8_t size = (1 + (SEGLEN >> 11));

  if (percent <= 100) {
    for (int i = 0; i < SEGLEN; i++) {
    	if (i < SEGENV.aux1) {
        if (SEGMENT.check1)
          SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(map(percent,0,100,0,255), false, false, 0));
        else
          SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(i, true, PALETTE_SOLID_WRAP, 0));
    	}
    	else {
        SEGMENT.setPixelColor(i, SEGCOLOR(1));
    	}
    }
  } else {
    for (int i = 0; i < SEGLEN; i++) {
    	if (i < (SEGLEN - SEGENV.aux1)) {
        SEGMENT.setPixelColor(i, SEGCOLOR(1));
    	}
    	else {
        if (SEGMENT.check1)
          SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(map(percent,100,200,255,0), false, false, 0));
        else
          SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(i, true, PALETTE_SOLID_WRAP, 0));
    	}
    }
  }
  if(active_leds > SEGENV.aux1) {  // smooth transition to the target value
    SEGENV.aux1 += size;
    if (SEGENV.aux1 > active_leds) SEGENV.aux1 = active_leds;
  } else if (active_leds < SEGENV.aux1) {
    if (SEGENV.aux1 > size) SEGENV.aux1 -= size; else SEGENV.aux1 = 0;
    if (SEGENV.aux1 < active_leds) SEGENV.aux1 = active_leds;
  }
 	return FRAMETIME;
 }
 static const char _data_fx_mode_tire_pressure[] = "Tire Pressure@,% of fill,,,,One color;!,!;!";


 /*
 * trail ratings display Fade LEDs between two colors
 */
 uint16_t mode_tire_pressure_fade(void) {
  int counter = percent_tpmsp;

  for (int i = 0; i < SEGLEN; i++) {
    SEGMENT.setPixelColor(i, color_blend(SEGCOLOR(1), SEGMENT.color_from_palette(i, true, PALETTE_SOLID_WRAP, 0), counter));
  }

  return FRAMETIME;
 }
 static const char _data_fx_mode_tire_pressure_fade[] PROGMEM = "Tire Pressure Fade@!;!,!;!";


 /*
 * Wheel Temp Fade LEDs between two colors
 */
 uint16_t mode_wheel_temp_fade(void) {
  int counter = percent_tpmst;

  for (int i = 0; i < SEGLEN; i++) {
    SEGMENT.setPixelColor(i, color_blend(SEGCOLOR(1), SEGMENT.color_from_palette(i, true, PALETTE_SOLID_WRAP, 0), counter));
  }

  return FRAMETIME;
 }
 static const char _data_fx_mode_wheel_temp_fade[] PROGMEM = "Tire Tempature Fade@!;!,!;!";


 /*
 * wheel temp display
 * Intesity values from 0-100 turn on the leds.
 */
 uint16_t mode_wheel_temp(void) {
  int percent = percent_tpmst;
  percent = constrain(percent, 0, 200);
  uint16_t active_leds = (percent < 100) ? SEGLEN * percent / 100.0
                                         : SEGLEN * (200 - percent) / 100.0;
  uint8_t size = (1 + (SEGLEN >> 11));

  if (percent <= 100) {
    for (int i = 0; i < SEGLEN; i++) {
    	if (i < SEGENV.aux1) {
        if (SEGMENT.check1)
          SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(map(percent,0,100,0,255), false, false, 0));
        else
          SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(i, true, PALETTE_SOLID_WRAP, 0));
    	}
    	else {
        SEGMENT.setPixelColor(i, SEGCOLOR(1));
    	}
    }
  } else {
    for (int i = 0; i < SEGLEN; i++) {
    	if (i < (SEGLEN - SEGENV.aux1)) {
        SEGMENT.setPixelColor(i, SEGCOLOR(1));
    	}
    	else {
        if (SEGMENT.check1)
          SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(map(percent,100,200,255,0), false, false, 0));
        else
          SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(i, true, PALETTE_SOLID_WRAP, 0));
    	}
    }
  }
  if(active_leds > SEGENV.aux1) {  // smooth transition to the target value
    SEGENV.aux1 += size;
    if (SEGENV.aux1 > active_leds) SEGENV.aux1 = active_leds;
  } else if (active_leds < SEGENV.aux1) {
    if (SEGENV.aux1 > size) SEGENV.aux1 -= size; else SEGENV.aux1 = 0;
    if (SEGENV.aux1 < active_leds) SEGENV.aux1 = active_leds;
  }
 	return FRAMETIME;
 }
 static const char _data_fx_mode_wheel_temp[] = "Tire Tempature@,% of fill,,,,One color;!,!;!";


  /*
 * humidity display
 * Intesity values from 0-100 turn on the leds.
 */
 uint16_t mode_humidity(void) {
  uint8_t percent = humidity;
  percent = constrain(percent, 0, 200);
  uint16_t active_leds = (percent < 100) ? SEGLEN * percent / 100.0
                                         : SEGLEN * (200 - percent) / 100.0;
  uint8_t size = (1 + (SEGLEN >> 11));

  if (percent <= 100) {
    for (int i = 0; i < SEGLEN; i++) {
    	if (i < SEGENV.aux1) {
        if (SEGMENT.check1)
          SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(map(percent,0,100,0,255), false, false, 0));
        else
          SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(i, true, PALETTE_SOLID_WRAP, 0));
    	}
    	else {
        SEGMENT.setPixelColor(i, SEGCOLOR(1));
    	}
    }
  } else {
    for (int i = 0; i < SEGLEN; i++) {
    	if (i < (SEGLEN - SEGENV.aux1)) {
        SEGMENT.setPixelColor(i, SEGCOLOR(1));
    	}
    	else {
        if (SEGMENT.check1)
          SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(map(percent,100,200,255,0), false, false, 0));
        else
          SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(i, true, PALETTE_SOLID_WRAP, 0));
    	}
    }
  }
  if(active_leds > SEGENV.aux1) {  // smooth transition to the target value
    SEGENV.aux1 += size;
    if (SEGENV.aux1 > active_leds) SEGENV.aux1 = active_leds;
  } else if (active_leds < SEGENV.aux1) {
    if (SEGENV.aux1 > size) SEGENV.aux1 -= size; else SEGENV.aux1 = 0;
    if (SEGENV.aux1 < active_leds) SEGENV.aux1 = active_leds;
  }
 	return FRAMETIME;
 }
 static const char _data_fx_mode_humidity[] = "humidity % @,% of fill,,,,One color;!,!;!";
 

/** Initiate VescUart class */
VescUart UART;


class Usermodvesc : public Usermod
{
public:

ADXL345 adxl = ADXL345();  // USE FOR I2C COMMUNICATION


//Change these values to suit your setup
int Poles = 30;                  //Usually 46 for hub motor
float WheelDia = 0.28;           //Wheel diameter in m
float GearReduction = 1;         //reduction ratio. 1 for direct drive. Otherwise motor pulley diameter / Wheel pulley diameter.
float BatteryCells = 15;         //Number of cells in Battery


float smoothedrpm;
float rpm;
float voltage;
float current;
int power;
float amphour;
float tach;
float distance;
float velocity;
float watthour;
float batpercentage;
float dutycycle;
float mosfettemp;
float motortemp;

#ifdef PRO_VERSION

  int8_t low_bat_preset = 1;
  int8_t low_bat_percent = 10;

  int8_t choosen_slow_preset = 1;
  int8_t choosen_med_preset = 1;
  int8_t choosen_fast_preset = 1;

  int8_t motor_duty_slow = 20;
  int8_t motor_duty_med = 40;
  int8_t motor_duty_fast = 70;
  bool   MOTOR_ENGAGEMENT = false;
  #else
  int8_t forwards_preset = 1;
  #endif

  int8_t backwards_preset = 1;  //preset played as a boot animation
  int8_t dim_backwards_preset = 1;  //preset played as a boot animation
  int8_t dim_forwards_preset = 1;  //preset played as a boot animation
  int dim_left_preset = 0;   
  int dim_right_preset = 0; 
  int dim_standing_up_preset = 0;

  bool alt_mode_user = true;
  bool alt_mode = true;
  int8_t alt_backwards_preset = 1;  //preset played as a boot animation
  int8_t alt_forwards_preset = 1;  //preset played as a boot animation

  int8_t boot_preset = 1;  //preset played as a boot animation

  int boot_preset_time = 3; // boot animation length in sec


  bool FRONT_LIGHT_R = false;
  int FRONT_LIGHT_R_ANALOG;
  bool FRONT_LIGHT_W = false;
  int FRONT_LIGHT_W_ANALOG;

  // flag set at startup
  int client_numb;
  //bool forward = true; //moved to global
  bool app_lights_on;  // are the lights on in the app?

  bool app_lights_on_last; // last check value of lights on
  int blink_app_lights = 0;
  unsigned long blink_app_lights_timing;

  unsigned int stock_preset = 0;

  unsigned int free_fall_preset = 1; // preset after free fall
  unsigned int free_fall_preset_time = 3; // animation length in sec
  unsigned long free_fall_milisec; // for tracking how much time has past for free_fall animation preset
  bool imu_free_fall = false;

  int rawx , rawy , rawz;
  //int filteredx , filteredy , filteredz;  //moved to global values
  int normx , normy , normz;
  int smoothedy;  // unused

unsigned long a_read_milisec;  // analog read limit

//////////////////////////////tpsm vars

  int tpmsb = 0; //tpms batt
  float tpmsp = 0; //tpms pressure
  float tpmst = 0; //tpms temp  shut off at 186 f

  int pressure_range_low = 0;
  int pressure_range_high = 30;
  bool fahrenheit = true; // f or c
  bool psi = true;     //psi or bar


  //bool dimmed_lights = false; // moved to global

  bool imu_activity = true;
  bool imu_inactivity = true;

  int orientation = 0; //how is the board on the ground

  int trick; // unused for trick detection module input

///////////////////////////////////////////////////////////////////////////////////
  unsigned long last_active_millis; // last time lights were bright for count down effect

  unsigned int trail_ruffness_max = 30;   // max activations per min for the bar graph
  unsigned long trail_ruffness = 0; //how ruff the trail is using active activations per min and how on pro version use motor disengadements
  unsigned long inactive_millis; // subrated inactive sec from trail calculations
  unsigned long inactive_millis_last; // subrated inactive sec from trail calculations



  // strings to reduce flash memory usage (used more than twice)
  static const char _name[];
  #ifdef PRO_VERSION
  static const char _low_bat_percent[];
  static const char _low_bat_preset[];
  static const char _choosen_slow_preset[];
  static const char _choosen_med_preset[];
  static const char _choosen_fast_preset[];
  static const char _motor_duty_slow[];
  static const char _motor_duty_med[];
  static const char _motor_duty_fast[];
  #else
  static const char _forwards_preset[];
  #endif
  static const char _backwards_preset[];
  static const char _dim_backwards_preset[];
  static const char _dim_forwards_preset[];
  static const char _dim_left_preset[];
  static const char _dim_right_preset[];
  static const char _dim_standing_up_preset[];

  static const char _alt_mode_user[];
  static const char _alt_backwards_preset[];
  static const char _alt_forwards_preset[];

  static const char _boot_preset[];
  static const char _boot_preset_time[];
  static const char _trail_ruffness_max[];
  static const char _free_fall_preset_time[];
  static const char _free_fall_preset[];
  static const char _stock_preset[];

  static const char _pressure_range_low[];
  static const char _pressure_range_high[];
  static const char _fahrenheit[];
  static const char _psi[];
  static const char _type[];

  #ifdef PRO_VERSION
    void MOTOR_ENGAGED() //determine if the motor is engaged (used as trigger for board-idle animations)
    {
    int ar = analogRead(MOTOR_SPEED_PIN); // ar analog read
    if (ar > 15) // if analog read is less the 15 set dudty cycle to 0 (disengaded)
    {
      MOTOR_ENGAGEMENT = true;
    } else {
      MOTOR_ENGAGEMENT = false;
    }
    }
  #endif

 #ifdef PRO_VERSION
    void MIMIC_ERROR_CODES() //if the lightbar is blinking (condition 1) orange (condition 2), make the head/taillights do the same
    {
      if ((MOTOR_ENGAGEMENT) == false && (LIGHT_BAR_B) == false) //light bar blue being on means either foot pad engagement or white for charging
      {
        int error_red;
        int error_green;
        int error_blue;

        if (LIGHT_BAR_R == true){
          error_red = 255;
          } else {
            error_red = 0;
            }
        if (LIGHT_BAR_G == true){
          error_green = 255;
          } else {
            error_green = 0;
            }
        if (LIGHT_BAR_B == true){
          error_blue = 255;
          } else {
            error_blue = 0;
            }

         String redstring = "win&SB=255&FX=98&SM=1&SS=1&IX=0&R=" + error_red;   //combining multiple strings at once can result in unpredictable outcomes
         String greenstring = "&G=" + error_green;
         String bluestring = "&B=" + error_blue;
         String together1 = redstring + greenstring;
         String together2 = together1 + bluestring;
         //handleSet(nullptr, together2 , false );

         String redstring1 = "win&SB=255&FX=98&SM=0&SS=0&IX=0&R=" + error_red;  //combining multiple strings at once can result in unpredictable outcomes
         String greenstring1 = "&G=" + error_green;
         String bluestring1 = "&B=" + error_blue;
         String together3 = redstring + greenstring;
         String together4 = together1 + bluestring;
         //handleSet(nullptr, together4 , false );

      }
    }
 #endif

 #ifdef PRO_VERSION
  void GET_LIGHT_BAR()
  {
    // http://forum.arduino.cc/index.php?topic=37555.0
    // https://forum.arduino.cc/index.php?topic=185158.0

    LIGHT_BAR_B = digitalRead(LIGHT_BAR_B_PIN);

    if ((LIGHT_BAR_B) == false){
      LIGHT_BAR_R = digitalRead(LIGHT_BAR_R_PIN);
      LIGHT_BAR_R_ANALOG = analogRead(LIGHT_BAR_R_PIN);
      LIGHT_BAR_G = digitalRead(LIGHT_BAR_G_PIN);
    }
    // if status bar rgb blue is on (in the case of white charging or blue foot pad engadement) ignore
  }
 #endif


      //most of the time, this function should be performed when the baord is idle, because when the board is engaged, 
      //voltage will drop as more amperage is drawn. This can be programmed/accounted for, but will take time to develop for
      //a feature that is seldom used when riding
 #ifdef PRO_VERSION
  void GET_BATT_LEVEL() //determine the battery level based upon battery voltage input
  {                            // (analong read 0 - 4095) * (max v/max A read) * (resistor voltage divider) 
    battery_voltage_est = ((analogRead(BATTERY_VOLTAGE_PIN)) * (3.2/4095) * (1/24)); //resistor voltage divider used 1k and 24k
    //                map(value, fromLow, fromHigh, toLow, toHigh)            does not use voltage curves
    battery_percent = map(battery_voltage_est, 59.4, 73.8, 0, 100);


    if (tv2 == 0){
      battery_voltage = battery_voltage_est;
      }else{
        battery_voltage = tv2;
        }

    bmss = battery_percent;

  }
 #endif


      //the top speed of these boards is defined as their freespin speed, take a range of 0 to [freespin speed]
      //subdivided into N sub-ranges. These sub-ranges should be of a high enough fidelity that animations are not
      //visually stuttery as speed increases/decreases (roughly 8-10? make N dynamic/programmable if easy to do).
      //N is proportional to a scalar value, which is used to scale up/down the speed of the lighting animation
      //when the board speed is within one of the ranges, scale the animation speed accordingly 
 #ifdef PRO_VERSION
  void GET_DUTYCYCLE()
  {  //(9 analog read at disengaded) (380 analog read at free spin) (assume max 80% duty cycle)
    
    if (motod == 0){
    int ar = analogRead(MOTOR_SPEED_PIN); // ar analog read
    motor_duty_est = ((ar / 380) * 100); // outputs a 

    if (ar < 15) // if analog read is less the 15 set dudty cycle to 0 (disengaded)
    {
      motor_duty_est = 0;
    }
    motor_duty_display = motor_duty_est;
    }else{
      motor_duty_display = motod;
    }
  }
 #endif

 #ifdef PRO_VERSION
  void set_motor_duty_preset()
  {  
    GET_DUTYCYCLE();
   if (motor_duty_display == 0)
   { return; }

   if (motor_duty_slow < motor_duty_display)
   { applyPreset(choosen_slow_preset); return; }

   if (motor_duty_med < motor_duty_display)
   { applyPreset(choosen_med_preset); return; }

   if (motor_duty_fast < motor_duty_display)
   { applyPreset(choosen_fast_preset); return; }
  }
#endif


  void handle_tpms() {

        if (psi) {
          display_tpmsp = (tpmsp * 0.001450377); //Kpa to PSI
    } else {
      display_tpmsp = (tpmsp * 0.0001); //Kpa to bar
    }

    if (fahrenheit) {
      display_tpmst = (((tpmst/100) * (9/5)) + 32);
    } else {
      display_tpmst = (tpmst/100);
    }


      percent_tpmsp = map(display_tpmsp, pressure_range_low, pressure_range_high, 0, 100);

      percent_tpmst = map((tpmst/100), 38, 93, 0, 100); //100f to 200f range in c

  }


    //use IMU data input to quantify the following:
    // 1.) trail chunkiness - a function of z-variance detected by the IMU, measured over a long timescale
    // 2.) altitude gain/loss - a function of the average slope of z acceleration measured over long distances
    // 3.) average speed - a function of how high/low the average speed of a run is
    // 4.) motor disengagement - worse trails require you to dismount more frequently, better trails do not. count 
    // how many times the motor disengages during a given run
    //how is the start/stop of a trail or segment measured?
    //all of the above calculation results in a color assignment to the trail run, like how trails are rated green, blue, black, etc
    void trailRate() {
      if (dimmed_lights == true){
        inactive_millis = inactive_millis + (millis() - inactive_millis_last);
      }
      inactive_millis_last = millis();

     float activations_per_min = (trail_ruffness / ((millis() - inactive_millis) / 6000));
      display_trail_ruffness = (activations_per_min / trail_ruffness_max);
    }


   void last_active(){
    if(app_lights_on){
     if ((dimmed_lights == false)){
     last_active_millis = millis();
     }else{
      int time_left = ((last_active_millis + (60000 * 23)) - millis());
       shutdown_display = map(time_left, 0, (60000 * 23), 0, 100);
     }
    }
   }

  void get_imu_data(){

    // Accelerometer Readings
    adxl.readAccel(&rawx, &rawy, &rawz);         // Read the accelerometer values and store them in variables declared above x,y,z


    normx = rawx * 0.004 * 9.80665f;
    normy = rawy * 0.004 * 9.80665f;
    normz = rawz * 0.004 * 9.80665f;

    filteredx = rawx * ALPHA + (filteredx * (1.0 - ALPHA));
    filteredy = rawy * ALPHA + (filteredy * (1.0 - ALPHA));
    filteredz = rawz * ALPHA + (filteredz * (1.0 - ALPHA));

    smoothedy = rawy * 0.5 + (smoothedy * (1.0 - 0.5)); // higly smoothed for left/right turn signal

    // Calculate Pitch & Roll (Low Pass Filter)  only works when board isnt moving so when lights are dimmed basicly
    //fpitch = -(atan2(filteredx, sqrt(filteredy*filteredy + filteredz*filteredz))*180.0)/M_PI;
    //froll  = (atan2(filteredy, filteredz)*180.0)/M_PI;


   byte interrupts = adxl.getInterruptSource();

    // Free Fall Detection
    if(adxl.triggered(interrupts, ADXL345_FREE_FALL)){
      if (free_fall_preset_time != 0 && !dimmed_lights){
    imu_free_fall = true;
    free_fall_milisec = millis();
    }
   }

   // Inactivity
   if(adxl.triggered(interrupts, ADXL345_INACTIVITY)){
    imu_inactivity = true;
   }


   // Activity
   if(adxl.triggered(interrupts, ADXL345_ACTIVITY)){
    imu_inactivity = false;
    imu_activity = true;
    trail_ruffness = trail_ruffness + 1;
   }

   //0 = upright (normal)
   //1 = upside down
   //2 = left side
   //3 = right side
   //4 = front pointing down
   //5 = back pointing down

   if (dimmed_lights){  //only detect a left right or upside down orientaion if the lights are dim
   if (filteredz < -10){orientation = 1;}
   if (filteredy < -17){orientation = 2;}
   if (filteredy > 17){orientation = 3;}
   if (filteredx < -17){orientation = 4;}
   if (filteredx > 17){orientation = 5;}
   }

   if (filteredz > 10){orientation = 0;}

} // end of get IMU data


  void get_front_light()
   {

    // rpm for direction
    // current usage for if the board is engadged or not 

    smoothedrpm = rpm * 0.5 + (smoothedrpm * (1.0 - 0.5)); // higly smoothed for left/right turn signal

     if (smoothedrpm > 5){forward = true;}
     if (smoothedrpm < -5){forward = false;}

     if (current < .2){
       dimmed_lights = true;
     }else{
      dimmed_lights = false;
      }



   app_lights_on = true;





   if (app_lights_on == false){ //turns lights off if in app lights are off
    bri = 0;stateUpdated(1);
   }else{
    bri = 255;stateUpdated(1);
  }

  }




void set_preset() { // pick which preset based on direction, speed, dim, alt mode

    if (stock_preset != 0){
    applyPreset(stock_preset);
    return;}

   //0 = upright (normal)
   //1 = upside down
   //2 = left side
   //3 = right side
   //4 = front pointing down
   //5 = back pointing down

  if (orientation != 0) {
    if (orientation == 2) {applyPreset(dim_left_preset);}
        if (orientation == 3) {applyPreset(dim_right_preset);}
           if (orientation == 5) {applyPreset(dim_standing_up_preset);}
            return;
              }


    if(forward){
      if (dimmed_lights == false) {
        if(alt_mode){
          #ifdef PRO_VERSION
          set_motor_duty_preset();
          #else
          applyPreset(forwards_preset);
          #endif
        } else {
          applyPreset(alt_forwards_preset);
        }
      } else {
        applyPreset(dim_forwards_preset);
      }
    } else {
      if (dimmed_lights == false) {
        if (alt_mode) {
          applyPreset(backwards_preset);
        } else {
          applyPreset(alt_backwards_preset);
        }
      } else {
        applyPreset(dim_backwards_preset);
      }
    }
}






public:
  void setup()
  {

  /** Setup UART port On TTGO Display, you have to assign the pins. 25(Tx) 26(Rx) in this case */
  //** Default VESC brate is 115200, you can change it to any other value. */
  Serial2.begin(115200, SERIAL_8N1, 32, 33);
  /** Define which ports to use as UART */
  UART.setSerialPort(&Serial2);
      briS = 255;
    bootPreset = boot_preset;
    // set pin modes
    strip.addEffect(FX_MODE_FB_ACCELERATION_BLINK, &mode_fb_acceleration_blink, _data_fx_fb_acceleration_blink);
    strip.addEffect(FX_MODE_F_ACCELERATION_BLINK, &mode_f_acceleration_blink, _data_fx_f_acceleration_blink);
    strip.addEffect(FX_MODE_B_ACCELERATION_BLINK, &mode_b_acceleration_blink, _data_fx_b_acceleration_blink);

    strip.addEffect(FX_MODE_COUNTDOWN, &mode_countdown, _data_fx_mode_countdown);
    strip.addEffect(FX_MODE_TRAILRATE, &mode_rate_trail, _data_fx_mode_rate_trail);
    strip.addEffect(FX_MODE_WHEELTEMP, &mode_wheel_temp, _data_fx_mode_wheel_temp);
    strip.addEffect(FX_MODE_TIREPRESSURE, &mode_tire_pressure, _data_fx_mode_tire_pressure);

    strip.addEffect(FX_MODE_STOCK_FRONT, &mode_stock_front, _data_fx_mode_stock_front);
    strip.addEffect(FX_MODE_STOCK_BACK, &mode_stock_back, _data_fx_mode_stock_back);

    strip.addEffect(FX_MODE_ACCEL_TEST, &mode_accel_test, _data_fx_mode_accel_test);

    strip.addEffect(FX_MODE_COUNTDOWN_FADE, &mode_countdown_fade, _data_fx_mode_countdown_fade);
    strip.addEffect(FX_MODE_RATE_TRAIL_FADE, &mode_rate_trail_fade, _data_fx_mode_rate_trail_fade);
    strip.addEffect(FX_MODE_TIRE_PRESSURE_FADE, &mode_tire_pressure_fade, _data_fx_mode_tire_pressure_fade);
    strip.addEffect(FX_MODE_WHEEL_TEMP_FADE, &mode_wheel_temp_fade, _data_fx_mode_wheel_temp_fade);

    pinMode(FRONT_LIGHT_W_PIN, INPUT);
    pinMode(FRONT_LIGHT_R_PIN, INPUT);

    pinMode(ERROR_LED_PIN, OUTPUT);
    digitalWrite(ERROR_LED_PIN, LOW);

    #ifdef PRO_VERSION
    pinMode(LIGHT_BAR_R_PIN, INPUT);
    pinMode(LIGHT_BAR_G_PIN, INPUT);
    pinMode(LIGHT_BAR_B_PIN, INPUT);

    pinMode(BATTERY_VOLTAGE_PIN, INPUT);
    pinMode(MOTOR_SPEED_PIN, INPUT);
    #endif


   adxl.powerOn();                     // Power on the ADXL345

   adxl.setRangeSetting(16);          // Accepted values are 2g, 4g, 8g or 16g
                                      // Higher Values = Wider Measurement Range
                                      // Lower Values = Greater Sensitivity

   adxl.setSpiBit(0);                  // Configure the device to be in 4 wire SPI mode when set to '0' or 3 wire SPI mode when set to 1
                                      // Default: Set to 1
                                      // SPI pins on the ATMega328: 11, 12 and 13 as reference in SPI Library

   adxl.setActivityXYZ(1, 1, 1);       // Set to activate movement detection in the axes "adxl.setActivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
   adxl.setActivityThreshold(150);      // 62.5mg per increment   // Set activity   // Inactivity thresholds (0-255)

   adxl.setInactivityXYZ(1, 1, 1);     // Set to detect inactivity in all the axes "adxl.setInactivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
   adxl.setInactivityThreshold(75);    // 62.5mg per increment   // Set inactivity // Inactivity thresholds (0-255)
   adxl.setTimeInactivity(10);         // How many seconds of no activity is inactive?

   adxl.setTapDetectionOnXYZ(1, 1, 1); // Detect taps in the directions turned ON "adxl.setTapDetectionOnX(X, Y, Z);" (1 == ON, 0 == OFF)

   // Set values for what is considered a TAP and what is a DOUBLE TAP (0-255)
   adxl.setTapThreshold(50);           // 62.5 mg per increment
   adxl.setTapDuration(15);            // 625 μs per increment
   adxl.setDoubleTapLatency(80);       // 1.25 ms per increment
   adxl.setDoubleTapWindow(200);       // 1.25 ms per increment

   // Set values for what is considered FREE FALL (0-255)
   adxl.setFreeFallThreshold(10);       // (5 - 9) recommended - 62.5mg per increment
   //int fall_sec = ((sqrt((free_fall_inches * 50800)/ 981))*2);  // convert inches fallen to ms fallen devided by 5
   adxl.setFreeFallDuration(20);       // (20 - 70) recommended - 5ms per increment

   // Setting all interupts to take place on INT1 pin
   adxl.setImportantInterruptMapping(1, 1, 1, 1, 1);     // Sets "adxl.setEveryInterruptMapping(single tap, double tap, free fall, activity, inactivity);" 
                                                        // Accepts only 1 or 2 values for pins INT1 and INT2. This chooses the pin on the ADXL345 to use for Interrupts.
                                                        // This library may have a problem using INT2 pin. Default to INT1 pin.

   // Turn on Interrupts for each mode (1 == ON, 0 == OFF)
   adxl.InactivityINT(1);
   adxl.ActivityINT(1);
   adxl.FreeFallINT(1);
   adxl.doubleTapINT(0);
   adxl.singleTapINT(0);

   // adxl.setFIFOMode("FIFO"); //four available modes - Bypass, FIFO, Stream and Trigger.
   // adxl.set_bw(ADXL345_BW_25);         //set bitrate

   shop = 1;// if display / other esp connected set to 1 to allow more devices connected with blocking main loop

   #ifdef TEST_MODE
   app_lights_on = true;  // set as if lights are detected as always on in test mode
   #endif

   #ifndef TEST_MODE // test mode skip get front light becuase we dont have the hardware on test esp32
   get_front_light();  // handels truning on/off lights and forward/back detection
   #endif

   get_imu_data();
   get_imu_data();
   get_imu_data();
   get_imu_data();
   get_imu_data(); // get imu data twice to populate filtered ints
   if (filteredy < -10){orientation = 2;}
   if (filteredy > 10){orientation = 3;}
   ///////////////////////////////////////////////////////  wifi
   //#ifndef TEST_MODE
   if (orientation == 2 || orientation == 3){
        ////////////////////////when wifi is on
    apHide = false; // show wifi
    applyPreset(free_fall_preset);//use free fall preset as wifi on boot animation
   /////////////////////////when wifi is off
   }else{

    //apHide = true; // hide wifi

    if (boot_preset_time != 0 && ((stock_preset != 0) == false)){ // skip if boot_preset_time set to 0
    applyPreset(boot_preset);// start up animation plays for 3 sec or so (still need to implement switching back)
    }else{
      set_preset();
    }//end of if not stock preset

   }


   }// end of start up

  void loop()
  {

    if (strip.isUpdating()){return;}

////////// Read values //////////
 if ( UART.getVescValues() ) {
  rpm = UART.data.rpm / (Poles / 2);                                // UART.data.rpm returns cRPM.  Divide by no of pole pairs in the motor for actual.
  voltage = (UART.data.inpVoltage);                                 //Battery Voltage
  current = (UART.data.avgInputCurrent);                            //Current Draw
  dutycycle = (UART.data.dutyCycleNow);                            //Current Draw
  motortemp = (UART.data.tempMotor);
  mosfettemp = (UART.data.tempMosfet);
  power = voltage*current;
  amphour = (UART.data.ampHours);                                   //This doesn't seem to do anything!
  watthour = amphour*voltage;                                       //Likewise
  distance = rpm*3.142*(1.0/1609.0)*WheelDia*GearReduction;         // Motor RPM x Pi x (1 / meters in a mile or km) x Wheel diameter x (motor pulley / wheelpulley)
  velocity = rpm*3.142*(60.0/1609.0)*WheelDia*GearReduction;        // Motor RPM x Pi x (seconds in a minute / meters in a mile) x Wheel diameter x (motor pulley / wheelpulley)
  batpercentage = (voltage-(3.0*BatteryCells)/BatteryCells)*100;   // Based on a minimum of 3V per cell
  state_switch = (UART.appData.switchState);
  vesc_state = (UART.appData.state);

  /*
  //Debug
  Serial.print("RPM="); Serial.print(rpm);
  Serial.print("|V="); Serial.print(voltage);
  Serial.print("|I="); Serial.print(current);
  Serial.print("|Ah="); Serial.print(amphour);
  Serial.print("|Vel-"); Serial.print(velocity);
  Serial.print("|Dist-"); Serial.print(distance);
  Serial.print("|Duty-"); Serial.print(dutycycle);
  Serial.print("|mos temp-"); Serial.print(mosfettemp);
  Serial.print("|motor temp-"); Serial.print(motortemp);
  Serial.println();
  */
      }


          if (free_fall_preset != 250){
      if (person_on_ui){return;}
    }// end of free fall preset shop mode

if ((a_read_milisec + 100) < millis()){    // limit loop to 10 times a sec
a_read_milisec = millis();

get_front_light();  // handels truning on/off lights and forward/back detection

}else{
  return; // skip rest of loop
}


   if (app_lights_on == false){
       return; // skip rest of loop
   }


last_active();//updates when board was last active
get_imu_data();

    if (stock_preset != 0){
    applyPreset(stock_preset);
    return;}

  if(alt_mode_user){  //if alt mode user is set true enable alt mode detection
     if (millis() < (10 * 1000)){
      if (orientation != 0){alt_mode = false;}
     }
     }

     if ((millis()) < (boot_preset_time * 1000)){
      return;  // returns loop if boot animation hasnt finished playing
     }

 //  if (blink_app_lights >= 3){ //if lights in Onewheel app are flashed on off 3 times
 //  }





    if (!imu_free_fall){
      set_preset();
    }

handle_tpms();


/////////////////////////////////////// activity (used for trail detection) "interrupt"
  if (imu_activity)
  {
    imu_activity = false;
  }
/////////////////////////////////////////////////////inactivity "interrupt"
  if (imu_inactivity)
  {
    imu_inactivity = false;
  }
//////////////////////////////////////////////////////////////  free fall "interrupt"
    if (imu_free_fall){
    applyPreset(free_fall_preset);
    if ((free_fall_milisec + (free_fall_preset_time * 1000)) < millis()){
      imu_free_fall = false;
    }
    return;  // returns loop if free fall animation hasnt finished playing
  }
///////////////////////////////////////////////////////////
   #ifdef PRO_VERSION  //rest of loop is pro only features
    GET_BATT_LEVEL();


    if ((low_bat_percent < battery_percent) && (low_bat_percent != 0)){  //if user set low battery %   less than    actual battery %
    set_motor_duty_preset();
    } else {
     applyPreset(low_bat_preset);
    }

     GET_LIGHT_BAR();

   #endif


  } // end of main loop



      void readFromJsonState(JsonObject& root)  //serial json iputs go in here
    {
      tpmsp = root["tpmsp"] | tpmsp; //TPMS tire pressure
      tpmsb = root["tpmsb"] | tpmsb; //TPMS battery
      tpmst = root["tpmst"] | tpmst; //TPMS temp
      trick = root["trick"] | trick; //trick number
      motod = root["motod"] | motod; //motor duty cycle from msense
      shop  = root["shop"] | shop; //allows for display to be connected while user mod still runs loop

      tv2 = root["tv2"] | tv2; //totale volatage
      ca2 = root["ca2"] | ca2; //current amps
      bmss = root["bmss"] | bmss; //bms state of charge
      ucm = root["ucm"] | ucm; //used charge mah                stuff for pint and XR owie
      rcm = root["rcm"] | rcm; //regen mah
      tempt[0] = root["tempt"][0] | tempt[0]; //temp table
      tempt[1] = root["tempt"][1] | tempt[1]; //temp table
      tempt[2] = root["tempt"][2] | tempt[2]; //temp table
      tempt[3] = root["tempt"][3] | tempt[3]; //temp table
      tempt[4] = root["tempt"][4] | tempt[4]; //temp table

      cvt[1] = root["cvt"][1] | cvt[1]; // voltage table
      cvt[2] = root["cvt"][2] | cvt[2]; //voltage table  there is probably a better way to do this
      cvt[3] = root["cvt"][3] | cvt[3]; //voltage table
      cvt[4] = root["cvt"][4] | cvt[4]; // voltage table
      cvt[5] = root["cvt"][5] | cvt[5]; // voltage table
      cvt[6] = root["cvt"][6] | cvt[6]; //voltage table
      cvt[7] = root["cvt"][7] | cvt[7]; //voltage table
      cvt[8] = root["cvt"][8] | cvt[8]; //voltage table
      cvt[9] = root["cvt"][9] | cvt[9]; //voltage table
      cvt[10] = root["cvt"][10] | cvt[10]; // voltage table
      cvt[11] = root["cvt"][11] | cvt[11]; //voltage table
      cvt[12] = root["cvt"][12] | cvt[12]; //voltage table
      cvt[13] = root["cvt"][13] | cvt[13]; // voltage table
      cvt[14] = root["cvt"][14] | cvt[14]; //voltage table

    }

  void addToJsonInfo(JsonObject &root)  //serial json outputs go here
  {

    JsonObject hidden = root[F("h")];
    if (hidden.isNull())
      hidden = root.createNestedObject(F("h"));



        JsonArray lux2 = hidden.createNestedArray(F("tpsm")); //left side thing
    lux2.add((tpmsp/100));
    lux2.add(psi);
    lux2.add((tpmst/100));
    lux2.add(fahrenheit);
    lux2.add(tpmsb);

            JsonArray lux6 = hidden.createNestedArray(F("lights")); //left side thing
    lux6.add(app_lights_on);
    lux6.add(forward);
    lux6.add(dimmed_lights);
    lux6.add(blink_app_lights);
    lux6.add(alt_mode);
    lux6.add(alt_mode_user);

        JsonArray lux7 = hidden.createNestedArray(F("accel")); //left side thing
    lux7.add(orientation);
    lux7.add(filteredx);
    lux7.add(filteredy);
    lux7.add(filteredz);

        JsonArray lux4 = hidden.createNestedArray(F("trail")); //left side thing
    lux4.add(display_trail_ruffness);                       //right side variable

            JsonArray lux1 = hidden.createNestedArray(F("duty")); //left side thing
    lux1.add(motor_duty_display);                       //right side variable

            JsonArray bat1 = hidden.createNestedArray(F("tv")); //totale volatage
    bat1.add(battery_voltage);
                JsonArray bat2 = hidden.createNestedArray(F("ca")); //current amps
    bat2.add(ca2);
                JsonArray bat3 = hidden.createNestedArray(F("bmss")); //bms state of charge
    bat3.add(bmss);
                JsonArray bat4 = hidden.createNestedArray(F("ucm")); //used charge mah
    bat4.add(ucm);
                JsonArray bat5 = hidden.createNestedArray(F("rcm")); //regen mah
    bat5.add(rcm);
                JsonArray bat6 = hidden.createNestedArray(F("cvt")); //batt cell voltage table
    bat6.add(cvt[0]);bat6.add(cvt[1]);bat6.add(cvt[2]);bat6.add(cvt[3]);bat6.add(cvt[4]);bat6.add(cvt[5]);bat6.add(cvt[6]);bat6.add(cvt[7]);bat6.add(cvt[8]);bat6.add(cvt[9]);bat6.add(cvt[10]);bat6.add(cvt[11]);bat6.add(cvt[12]);bat6.add(cvt[13]);bat6.add(cvt[14]);

                JsonArray bat7 = hidden.createNestedArray(F("tempt")); //batt temp table
    bat7.add(tempt[0]);bat7.add(tempt[1]);bat7.add(tempt[2]);bat7.add(tempt[3]);bat7.add(tempt[4]);




    JsonObject user = root[F("u")];
    if (user.isNull())
      user = root.createNestedObject(F("u"));

#ifdef PRO_VERSION
    JsonArray lux = user.createNestedArray(F("RED analog read")); //left side thing
    lux.add(LIGHT_BAR_R_ANALOG);                       //right side variable


      JsonArray battery = user.createNestedArray("blue level");  //left side thing
      battery.add(LIGHT_BAR_B);                               //right side variable
#endif


                  JsonArray battery6 = user.createNestedArray("activations per min");  //left side thing
      battery6.add(display_trail_ruffness);                               //right side variable

          JsonArray battery9;
           if (psi) {battery9 = user.createNestedArray("Tire PSI");}else{battery9 = user.createNestedArray("Tire Bar");}  //left side thing
      battery9.add(display_tpmsp);

          JsonArray battery16;
         if (fahrenheit) {battery16 = user.createNestedArray("Tire Temp F");}else{battery16 = user.createNestedArray("Tire Temp C");}  //left side thing
         battery16.add(display_tpmst);

                        JsonArray battery26 = user.createNestedArray("Tire sensor battery %");  //left side thing
      battery26.add(tpmsb);                               //right side variable
  }

  uint16_t getId()
  {
    return USERMOD_ID_VESC;
  }

  /**
     * addToConfig() (called from set.cpp) stores persistent properties to cfg.json
     */
  void addToConfig(JsonObject &root)
  {
    // we add JSON object.
    JsonObject top = root.createNestedObject(FPSTR(_name)); // usermodname
    top[FPSTR(_stock_preset)] = stock_preset;  //int input
    top[FPSTR(_alt_mode_user)] = alt_mode_user;
    #ifdef PRO_VERSION
    top[FPSTR(_low_bat_percent)] = low_bat_percent;  //int input
    top[FPSTR(_low_bat_preset)] = low_bat_preset;  //int input
    top[FPSTR(_choosen_slow_preset)] = choosen_slow_preset;  //int input
    top[FPSTR(_choosen_med_preset)] = choosen_med_preset;  //int input
    top[FPSTR(_choosen_fast_preset)] = choosen_fast_preset;  //int input
    top[FPSTR(_motor_duty_slow)] = motor_duty_slow;  //int input
    top[FPSTR(_motor_duty_med)] = motor_duty_med;  //int input
    top[FPSTR(_motor_duty_fast)] = motor_duty_fast;  //int input
    #else
    top[FPSTR(_forwards_preset)] = forwards_preset;  //int input
    #endif

    top[FPSTR(_backwards_preset)] = backwards_preset;  //int input
    top[FPSTR(_dim_backwards_preset)] = dim_backwards_preset;  //int input
    top[FPSTR(_dim_forwards_preset)] = dim_forwards_preset;  //int input
    top[FPSTR(_alt_forwards_preset)] = alt_forwards_preset;  //int input
    top[FPSTR(_alt_backwards_preset)] = alt_backwards_preset;  //int input
    top[FPSTR(_dim_left_preset)] = dim_left_preset;  //int input
    top[FPSTR(_dim_right_preset)] = dim_right_preset;  //int input
    top[FPSTR(_dim_standing_up_preset)] = dim_standing_up_preset;  //int input

    top[FPSTR(_boot_preset)] = boot_preset;  //int input
    top[FPSTR(_boot_preset_time)] = boot_preset_time;  //int input
    top[FPSTR(_free_fall_preset)] = free_fall_preset;  //int input
    top[FPSTR(_trail_ruffness_max)] = trail_ruffness_max;  //int input

    top[FPSTR(_pressure_range_low)] = pressure_range_low;  //int input
    top[FPSTR(_pressure_range_high)] = pressure_range_high;  //int input

    top[FPSTR(_psi)] = !psi;
    top[FPSTR(_fahrenheit)] = !fahrenheit;
    top[FPSTR(_type)] = type;


    DEBUG_PRINTLN(F("Andonn config saved."));
  }

  /**
  * readFromConfig() is called before setup() to populate properties from values stored in cfg.json
  */
  bool readFromConfig(JsonObject &root)
  {
    // we look for JSON object.
    JsonObject top = root[FPSTR(_name)];
    if (top.isNull()) {
      DEBUG_PRINT(FPSTR(_name));
      DEBUG_PRINTLN(F(": No config found. (Using defaults.)"));
      return false;
    }
    #ifdef PRO_VERSION
    low_bat_percent   = top[FPSTR(_low_bat_percent)] | low_bat_percent;  //int input
    low_bat_preset   = top[FPSTR(_low_bat_preset)] | low_bat_preset;  //int input
    choosen_slow_preset   = top[FPSTR(_choosen_slow_preset)] | choosen_slow_preset;  //int input
    choosen_med_preset   = top[FPSTR(_choosen_med_preset)] | choosen_med_preset;      //int input
    choosen_fast_preset   = top[FPSTR(_choosen_fast_preset)] | choosen_fast_preset;    //int input
    motor_duty_slow   = top[FPSTR(_motor_duty_slow)] | motor_duty_slow;          //int input
    motor_duty_med   = top[FPSTR(_motor_duty_med)] | motor_duty_med;      //int input
    motor_duty_fast   = top[FPSTR(_motor_duty_fast)] | motor_duty_fast;     //int input
    #else
    forwards_preset   = top[FPSTR(_forwards_preset)] | forwards_preset;     //int input
    #endif
    backwards_preset   = top[FPSTR(_backwards_preset)] | backwards_preset;     //int input
    dim_backwards_preset   = top[FPSTR(_dim_backwards_preset)] | dim_backwards_preset;     //int input
    dim_forwards_preset   = top[FPSTR(_dim_forwards_preset)] | dim_forwards_preset;     //int input

    alt_backwards_preset   = top[FPSTR(_alt_backwards_preset)] | alt_backwards_preset;     //int input
    alt_forwards_preset   = top[FPSTR(_alt_forwards_preset)] | alt_forwards_preset;     //int input

    dim_left_preset   = top[FPSTR(_dim_left_preset)] | dim_left_preset;     //int input
    dim_right_preset   = top[FPSTR(_dim_right_preset)] | dim_right_preset;     //int input
    dim_standing_up_preset   = top[FPSTR(_dim_standing_up_preset)] | dim_standing_up_preset;     //int input
    boot_preset   = top[FPSTR(_boot_preset)] | boot_preset;     //int input
    stock_preset   = top[FPSTR(_stock_preset)] | stock_preset;     //int input
    boot_preset_time   = top[FPSTR(_boot_preset_time)] | boot_preset_time;     //int input
    alt_mode_user            = (top[FPSTR(_alt_mode_user)] | alt_mode_user);       //bool
    free_fall_preset   = top[FPSTR(_free_fall_preset)] | free_fall_preset;     //int input
    trail_ruffness_max   = top[FPSTR(_trail_ruffness_max)] | trail_ruffness_max;     //int input

    pressure_range_low   = top[FPSTR(_pressure_range_low)] | pressure_range_low;     //int input
    pressure_range_high   = top[FPSTR(_pressure_range_high)] | pressure_range_high;     //int input
    if(type == 1){
    fahrenheit            = !(top[FPSTR(_fahrenheit)] | !fahrenheit);       //bool
    psi            = !(top[FPSTR(_psi)] | !psi);       //bool
    }
    type            = (top[FPSTR(_type)] | type);       //bool

    DEBUG_PRINT(FPSTR(_name));
    DEBUG_PRINTLN(F(" config (re)loaded."));

    // use "return !top["newestParameter"].isNull();" when updating Usermod with new features
    return true;
  }
};

// strings to reduce flash memory usage (used more than twice)
//                           _veriable         "what it says on the webpage"
const char Usermodvesc::_name[] PROGMEM = "Andonn user preset configuration";
const char Usermodvesc::_stock_preset[] PROGMEM = "Stock lighting override preset";
#ifdef PRO_VERSION
const char Usermodvesc::_low_bat_percent[] PROGMEM = "Battery percent to change preset (0 to disable) overrides duty cycle preset";
const char Usermodvesc::_low_bat_preset[] PROGMEM = "Low battery preset animation";

const char Usermodvesc::_choosen_slow_preset[] PROGMEM = "Slow preset animation";
const char Usermodvesc::_choosen_med_preset[] PROGMEM = "Med preset animation";
const char Usermodvesc::_choosen_fast_preset[] PROGMEM = "Fast preset animation";
const char Usermodvesc::_motor_duty_slow[] PROGMEM = "Slow motor duty %";
const char Usermodvesc::_motor_duty_med[] PROGMEM = "Med motor duty %";
const char Usermodvesc::_motor_duty_fast[] PROGMEM = "fast motor duty %";
#else
const char Usermodvesc::_forwards_preset[] PROGMEM = "Forward travel lighting preset";
#endif
const char Usermodvesc::_dim_forwards_preset[] PROGMEM = "Forward creep lighting preset";

const char Usermodvesc::_backwards_preset[] PROGMEM = "Reverse travel lighting preset";
const char Usermodvesc::_dim_backwards_preset[] PROGMEM = "Reverse creep lighting preset";

const char Usermodvesc::_alt_mode_user[] PROGMEM = "Enable alternative presets";
const char Usermodvesc::_alt_forwards_preset[] PROGMEM = "Alt forward travel lighting preset";
const char Usermodvesc::_alt_backwards_preset[] PROGMEM = "Alt reverse travel lighting preset";

const char Usermodvesc::_dim_left_preset[] PROGMEM = "Inactive left tilt lighting preset";
const char Usermodvesc::_dim_right_preset[] PROGMEM = "Inactive right tilt lighting preset";
const char Usermodvesc::_dim_standing_up_preset[] PROGMEM = "Inactive standing up lighting preset";

const char Usermodvesc::_boot_preset[] PROGMEM = "Boot animation lighting preset";
const char Usermodvesc::_boot_preset_time[] PROGMEM = "Boot duration (sec)";

const char Usermodvesc::_free_fall_preset[] PROGMEM = "Freefall lighting preset";
const char Usermodvesc::_free_fall_preset_time[] PROGMEM = "Freefall duration trigger (sec)";

const char Usermodvesc::_trail_ruffness_max[] PROGMEM = "trail variability maximum (DEV ONLY)";

const char Usermodvesc::_pressure_range_low[] PROGMEM = "PSI minimum trigger";
const char Usermodvesc::_pressure_range_high[] PROGMEM = "PSI maximum trigger";
const char Usermodvesc::_fahrenheit[] PROGMEM = "Temperature units (F/C)";
const char Usermodvesc::_psi[] PROGMEM = "Pressure units (PSI/BAR)";
const char Usermodvesc::_type[] PROGMEM = "0=vesc 1=pint/pint 2=XR";