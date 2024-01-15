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

int BatteryCells = 15;
//int display_battery;
//int display_duty_cycle;
int shop = 0;// shop mode
int motod = 0;// input from msense
int shutdown_display;
float display_tpmsp; //tpms pressure
float display_tpmst; //tpms temp
float percent_tpmst;
float percent_tpmsp;

int filteredx , filteredy , filteredz;
bool forward = true;
bool dimmed_lights = true;
float battery_voltage;
int humidity = -100;
int andonn_temp = -100;
float batpercentage;
float dutycycle = 1;
int display_trail_ruffness;

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
  int percent = map((motortemp), 50, 100, 0, 100); //100f to 200f range in c;
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
 static const char _data_fx_mode_wheel_temp[] = "Motor Temp 50-100c@, ,,,,One color;!,!;!";

  /*
 * mosfet temp display
 * Intesity values from 0-100 turn on the leds.
 */
 uint16_t mode_mosfet_temp(void) {
  int percent = map((mosfettemp), 50, 100, 0, 100); //100f to 200f range in c;
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
 static const char _data_fx_mode_mosfet_temp[] = "Mosfet Temp 50-100c@, ,,,,One color;!,!;!";


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


  /*
 * bat percentage display
 * Intesity values from 0-100 turn on the leds.
 */
 uint16_t mode_batpercent(void) {
  uint8_t percent = batpercentage;
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
 static const char _data_fx_mode_batpercent[] = "Battery percent % @,% of fill,,,,One color;!,!;!";



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

int smoothedrpm = 0;
int rpm;
float voltage;
float current;
int power;
float amphour;
float tach;
float distance;
float velocity;
float watthour;
float mosfettemp;
float motortemp;


  #ifndef SIMPLE_CONFIG
  int8_t choosen_slow_preset = 1;
  int8_t choosen_fast_preset = 1;

  int8_t motor_duty_slow = -30;
  int8_t motor_duty_fast = 70;
  #endif
  int8_t forwards_preset = 1;

  int8_t backwards_preset = 1;  //preset played as a boot animation
  int8_t dim_preset = 1;  //preset played as a boot animation
  int dim_standing_up_preset = 0;

  bool vesc_light_on = true;
  bool no_input = false;
  bool is_vesc_main = true;

  bool alt_mode_user = false;
  bool alt_toggle = false;
  bool alt_toggle_on = true; // editable parady for vesc_light_on
  bool alt_mode = true;
  int8_t alt_backwards_preset = 1;  //preset played as a boot animation
  int8_t alt_forwards_preset = 1;  //preset played as a boot animation



  bool FRONT_LIGHT_R = false;
  int FRONT_LIGHT_R_ANALOG;
  bool FRONT_LIGHT_W = false;
  int FRONT_LIGHT_W_ANALOG;

  int client_numb;
  bool app_lights_on;  // are the lights on in the app?

  bool app_lights_on_last; // last check value of lights on

  unsigned int free_fall_preset = 1; // preset after free fall
  float direction_threshold = 3; // direction_threshold when using IMU only to detect direction
  unsigned int free_fall_preset_time = 4; // animation length in sec
  unsigned long free_fall_milisec; // for tracking how much time has past for free_fall animation preset
  unsigned long active_milis_dim; // for tracking how much time has past for dim light activation
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


  bool imu_activity = false;
  bool imu_inactivity = true;
  int  imu_inactivity_count = 11;
  unsigned long imu_inactivity_milis = 0;

  int orientation = 0; //how is the board on the ground
  int last_orientation = 0; //how is the board on the ground

  int trick; // unused for trick detection module input


  int numSamples = 10;
float accelerationBuffer[10]; // Buffer to store acceleration values
int currentIndex = 0;

///////////////////////////////////////////////////////////////////////////////////
  unsigned long last_active_millis; // last time lights were bright for count down effect

  unsigned int trail_ruffness_max = 30;   // max activations per min for the bar graph
  unsigned long trail_ruffness = 0; //how ruff the trail is using active activations per min and how on pro version use motor disengadements
  unsigned long inactive_millis; // subrated inactive sec from trail calculations
  unsigned long inactive_millis_last; // subrated inactive sec from trail calculations



  // strings to reduce flash memory usage (used more than twice)
  static const char _name[];
  static const char _backwards_preset[];
  static const char _dim_preset[];
  static const char _dim_standing_up_preset[];
  static const char _vesc_light_on[];
  static const char _no_input[];
  static const char _is_vesc_main[];
  static const char _free_fall_preset[];
  static const char _direction_threshold[];
  static const char _BatteryCells[];
  static const char _alt_toggle[];

#ifndef SIMPLE_CONFIG
  static const char _alt_mode_user[];
  static const char _alt_backwards_preset[];
  static const char _alt_forwards_preset[];
  //static const char _trail_ruffness_max[];
  static const char _choosen_slow_preset[];
  static const char _choosen_fast_preset[];
  static const char _motor_duty_slow[];
  static const char _motor_duty_fast[];
  #endif
  static const char _forwards_preset[];



      //most of the time, this function should be performed when the baord is idle, because when the board is engaged, 
      //voltage will drop as more amperage is drawn. This can be programmed/accounted for, but will take time to develop for
      //a feature that is seldom used when riding





      //the top speed of these boards is defined as their freespin speed, take a range of 0 to [freespin speed]
      //subdivided into N sub-ranges. These sub-ranges should be of a high enough fidelity that animations are not
      //visually stuttery as speed increases/decreases (roughly 8-10? make N dynamic/programmable if easy to do).
      //N is proportional to a scalar value, which is used to scale up/down the speed of the lighting animation
      //when the board speed is within one of the ranges, scale the animation speed accordingly

#ifndef SIMPLE_CONFIG
  void set_motor_duty_preset()
  {

    if (alt_mode) {
        applyPreset(forwards_preset);
    } else {
        applyPreset(alt_forwards_preset);
    }

    if (dutycycle * 100 <= motor_duty_slow) {
        applyPreset(choosen_slow_preset);
    } else if (dutycycle * 100 >= motor_duty_fast) {
        applyPreset(choosen_fast_preset);
    }

  }
#endif

const int volt_percent_Table[341][2] = {
  {2700, 0}, {2705, 0}, {2710, 0}, {2715, 0}, {2720, 0},
  {2725, 0}, {2730, 0}, {2735, 0}, {2740, 0}, {2745, 0},
  {2750, 0}, {2755, 0}, {2760, 0}, {2765, 0}, {2770, 0},
  {2775, 0}, {2780, 0}, {2785, 0}, {2790, 0}, {2795, 0},
  {2800, 0}, {2805, 0}, {2810, 0}, {2815, 0}, {2820, 0},
  {2825, 0}, {2830, 0}, {2835, 0}, {2840, 0}, {2845, 0},
  {2850, 0}, {2855, 0}, {2860, 0}, {2865, 0}, {2870, 0},
  {2875, 0}, {2880, 0}, {2885, 0}, {2890, 0}, {2895, 0},
  {2900, 1}, {2905, 1}, {2910, 1}, {2915, 1}, {2920, 1},
  {2925, 1}, {2930, 1}, {2935, 1}, {2940, 1}, {2945, 1},
  {2950, 2}, {2955, 2}, {2960, 2}, {2965, 2}, {2970, 2},
  {2975, 2}, {2980, 2}, {2985, 2}, {2990, 2}, {2995, 2},
  {3000, 3}, {3005, 3}, {3010, 3}, {3015, 3}, {3020, 3},
  {3025, 3}, {3030, 3}, {3035, 3}, {3040, 3}, {3045, 3},
  {3050, 4}, {3055, 4}, {3060, 4}, {3065, 4}, {3070, 4},
  {3075, 4}, {3080, 4}, {3085, 4}, {3090, 4}, {3095, 4},
  {3100, 5}, {3105, 5}, {3110, 5}, {3115, 5}, {3120, 5},
  {3125, 6}, {3130, 6}, {3135, 6}, {3140, 6}, {3145, 6},
  {3150, 7}, {3155, 7}, {3160, 7}, {3165, 7}, {3170, 7},
  {3175, 7}, {3180, 7}, {3185, 7}, {3190, 7}, {3195, 7},
  {3200, 8}, {3205, 8}, {3210, 8}, {3215, 8}, {3220, 9},
  {3225, 9}, {3230, 9}, {3235, 10}, {3240, 10}, {3245, 10},
  {3250, 11}, {3255, 11}, {3260, 11}, {3265, 11}, {3270, 12},
  {3275, 12}, {3280, 12}, {3285, 13}, {3290, 13}, {3295, 13},
  {3300, 14}, {3305, 14}, {3310, 14}, {3315, 14}, {3320, 14},
  {3325, 15}, {3330, 15}, {3335, 15}, {3340, 15}, {3345, 15},
  {3350, 16}, {3355, 16}, {3360, 16}, {3365, 16}, {3370, 16},
  {3375, 17}, {3380, 17}, {3385, 17}, {3390, 17}, {3395, 17},
  {3400, 18}, {3405, 18}, {3410, 18}, {3415, 18}, {3420, 18},
  {3425, 18}, {3430, 18}, {3435, 18}, {3440, 18}, {3445, 18},
  {3450, 19}, {3455, 19}, {3460, 20}, {3465, 20}, {3470, 21},
  {3475, 22}, {3480, 22}, {3485, 23}, {3490, 23}, {3495, 24},
  {3500, 25}, {3505, 25}, {3510, 26}, {3515, 26}, {3520, 27},
  {3525, 27}, {3530, 28}, {3535, 28}, {3540, 29}, {3545, 29},
  {3550, 30}, {3555, 30}, {3560, 30}, {3565, 30}, {3570, 31},
  {3575, 31}, {3580, 31}, {3585, 32}, {3590, 32}, {3595, 32},
  {3600, 33}, {3605, 33}, {3610, 33}, {3615, 34}, {3620, 34},
  {3625, 35}, {3630, 35}, {3635, 35}, {3640, 36}, {3645, 36},
  {3650, 37}, {3655, 37}, {3660, 38}, {3665, 38}, {3670, 39},
  {3675, 40}, {3680, 40}, {3685, 41}, {3690, 41}, {3695, 42},
  {3700, 43}, {3705, 43}, {3710, 44}, {3715, 44}, {3720, 45},
  {3725, 45}, {3730, 46}, {3735, 46}, {3740, 47}, {3745, 47},
  {3750, 48}, {3755, 48}, {3760, 49}, {3765, 49}, {3770, 50},
  {3775, 50}, {3780, 51}, {3785, 51}, {3790, 52}, {3795, 52},
  {3800, 53}, {3805, 53}, {3810, 54}, {3815, 55}, {3820, 55},
  {3825, 56}, {3830, 57}, {3835, 57}, {3840, 58}, {3845, 59},
  {3850, 60}, {3855, 60}, {3860, 61}, {3865, 62}, {3870, 62},
  {3875, 63}, {3880, 64}, {3885, 64}, {3890, 65}, {3895, 66},
  {3900, 67}, {3905, 67}, {3910, 67}, {3915, 68}, {3920, 68},
  {3925, 69}, {3930, 69}, {3935, 69}, {3940, 70}, {3945, 70},
  {3950, 71}, {3955, 71}, {3960, 72}, {3965, 72}, {3970, 73},
  {3975, 73}, {3980, 74}, {3985, 74}, {3990, 75}, {3995, 75},
  {4000, 76}, {4005, 76}, {4010, 77}, {4015, 77}, {4020, 78},
  {4025, 79}, {4030, 79}, {4035, 80}, {4040, 80}, {4045, 81},
  {4050, 82}, {4055, 83}, {4060, 84}, {4065, 85}, {4070, 86},
  {4075, 87}, {4080, 88}, {4085, 89}, {4090, 90}, {4095, 91},
  {4100, 92}, {4105, 92}, {4110, 93}, {4115, 93}, {4120, 94},
  {4125, 94}, {4130, 95}, {4135, 95}, {4140, 96}, {4145, 96},
  {4150, 97}, {4155, 97}, {4160, 97}, {4165, 97}, {4170, 98},
  {4175, 98}, {4180, 98}, {4185, 99}
};


int volt_to_percent(int volts) {
  int cellvtable = ((round((volts* 100) /BatteryCells) / 5.0) * 5.0)*10;
  for (int i = 0; i < 329; i++) {
    if (volt_percent_Table[i][0] == cellvtable) {
      return volt_percent_Table[i][1];
    }
  }

  if (cellvtable > 4185){
    return 99;
    }
  if (cellvtable < 2700){
    return -1;
    }
    return 0;
}


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

   if (imu_inactivity_count > 2){
   if (filteredz < -10){orientation = 1;}
   if (filteredy < -17){orientation = 2;}
   if (filteredy > 17){orientation = 3;}
   if (filteredx < -17){orientation = 4;}
   if (filteredx > 17){orientation = 5;}
   }

   if (filteredz > 10){orientation = 0;}

if(alt_mode_user){
   if ((last_orientation == 0 || last_orientation == 1) && (orientation == 3)){
   alt_mode = !alt_mode;
   }
}

if(alt_toggle){
      if ((((last_orientation == 0) || (last_orientation == 1)) && (orientation == 2)) || ((!alt_mode_user) && (orientation == 3))){
   alt_toggle_on = !alt_toggle_on;
   }
}
last_orientation = orientation;
} // end of get IMU data

void get_front_light() {
    if (no_input) {
        get_front_light_accel(); // use accel data to get dim light
    } else if (is_vesc_main) {
        get_front_light_vesc(); // use duty cycle and power consumption to get dim light
    } else {
        get_front_light_stock(); // use analog input to get dim light
    }
}







void get_front_light_accel(){

  if (direction_threshold != 0){

  accelerationBuffer[currentIndex] = filteredx;
  currentIndex = (currentIndex + 1) % numSamples;
  float sum = 0;
  for (int i = 0; i < numSamples; i++) {
    sum += accelerationBuffer[i];
  }
  float accelx_avrg = sum / numSamples;


// direction change direction_threshold
    if (accelx_avrg > direction_threshold) {
        forward = true; // Set forward to true for forwards motion
    } else if (accelx_avrg < -direction_threshold) {
        forward = false; // Set forward to false for backwards motion
    }

}else{
forward = true;
}

   app_lights_on = alt_toggle_on;


     if (imu_inactivity_count > 50){
      dimmed_lights = true;
      }else{
        dimmed_lights = false;
        }

   if (alt_toggle_on == false){ //turns lights off if in app lights are off
    transitionDelay = 0;
    bri = 0;stateUpdated(1);
   }else{
    transitionDelay = 500;
    bri = 255;stateUpdated(1);
  }
}


  void get_front_light_stock()
   {

    //0      on
    //1659   dim (when you get off the board and it dims the lights)
    //4095   off


    FRONT_LIGHT_W_ANALOG = analogRead(FRONT_LIGHT_W_PIN);

    if (FRONT_LIGHT_W_ANALOG > 2000){
      forward = true;
      }else{
        forward = false;
        }

    FRONT_LIGHT_R_ANALOG = analogRead(FRONT_LIGHT_R_PIN);
    if (FRONT_LIGHT_R_ANALOG > 2000){
      FRONT_LIGHT_R = false; app_lights_on = false; dimmed_lights = false;
      }else{
        FRONT_LIGHT_R = true; app_lights_on = true;
        }


     if ((FRONT_LIGHT_R_ANALOG > 1000) && (FRONT_LIGHT_R_ANALOG < 2000)){
    dimmed_lights = true;
     } else {
      dimmed_lights = false;
     }


     if(app_lights_on_last != app_lights_on){
      if (app_lights_on == false){ //turns lights off if in app lights are off
       bri = 0;stateUpdated(1);
      }else{
       bri = 255;stateUpdated(1);
      }
     }

    app_lights_on_last = app_lights_on;
  }


  void get_front_light_vesc()
   {

////////// Read values //////////
 if ( UART.getVescValues() ) {
  voltage = (UART.data.inpVoltage);                                 //Battery Voltage
  current = (UART.data.avgInputCurrent);                            //Current Draw
  dutycycle = (UART.data.dutyCycleNow);                            //Current Draw
  motortemp = (UART.data.tempMotor);
  mosfettemp = (UART.data.tempMosfet);
  power = voltage*current;
//  amphour = (UART.data.ampHours);                                   //This doesn't seem to do anything!
  watthour = amphour*voltage;                                       //Likewise
  rpm = UART.data.rpm / (Poles / 2);                                // UART.data.rpm returns cRPM.  Divide by no of pole pairs in the motor for actual.
  distance = rpm*3.142*(1.0/1609.0)*WheelDia*GearReduction;         // Motor RPM x Pi x (1 / meters in a mile or km) x Wheel diameter x (motor pulley / wheelpulley)
  velocity = rpm*3.142*(60.0/1609.0)*WheelDia*GearReduction;        // Motor RPM x Pi x (seconds in a minute / meters in a mile) x Wheel diameter x (motor pulley / wheelpulley)
  batpercentage = volt_to_percent(voltage);


  bmss = batpercentage;
  smoothedrpm = ((rpm * 0.01 ) + (smoothedrpm * 0.99)); // higly smoothed
      }


    // rpm for direction 
    // current usage for if the board is engadged or not


     if (smoothedrpm > 10){forward = true;}
     if (smoothedrpm < -10){forward = false;}

     if ((current < 2) && (smoothedrpm > -1 && smoothedrpm < 1) && (dutycycle * 100 > -2 && dutycycle * 100 < 2) && (imu_inactivity_count > 20)){
      if(active_milis_dim + 10000 < millis()){
      dimmed_lights = true;
      }
      }else{
        dimmed_lights = false;
        active_milis_dim = millis();
        }



   app_lights_on = alt_toggle_on;



   if (alt_toggle_on == false){ //turns lights off if in app lights are off
   transitionDelay = 0;
    bri = 0;stateUpdated(1);
   }else{
    transitionDelay = 250;
    bri = 255;stateUpdated(1);
  }

  }




void set_preset() {

    // Handle special orientations
    if (orientation != 0) {
        if (orientation == 4 || orientation == 5) {
            applyPreset(dim_standing_up_preset);
        }
        return;
    }


       if (alt_toggle_on == false){ //turns lights off if in app lights are off
    return;
   }


    // Depending on direction, light dimming, and alt mode, apply the appropriate preset
        transitionDelay = 4000;
        if (dimmed_lights) {
        applyPreset(dim_preset);
        }else{

        if (forward) {
        set_motor_duty_preset();
        }else{
          if (alt_mode) {
        applyPreset(backwards_preset);
        } else {
            applyPreset(alt_backwards_preset);
        }
        }
        }
}


void get_humidity(){
  if (humiditySensor.available() == true)
  {
    humidity = humiditySensor.getTemperature();
    andonn_temp = humiditySensor.getHumidity();

  }
  if ((millis()) > (8 * 1000)){
    if (humidity == -100){
        Serial.println("CORRUPT HEAP: Bad head at 0x3ffbb0f0. Expected 0xabba1234 got 0x3ffb9a34 assert failed: multi_heap_free multi_heap_poisoning.c:253 (head != NULL) Backtrace:0x40083881:0x3ffb25400x4008e7e5:0x3ffb2560 0x40093d55:0x3ffb2580 0x4009399b:0x3ffb26b0 0x40083d41:0x3ffb26d0 0x40093d85:0x3ffb26f0 0x4014e3f5:0x3ffb2710 0x400d2dc6:0x3ffb2730 0x400d31e3:0x3ffb2750 0x400d9b02:0x3ffb2820");
      delay(10000);
      ESP.restart();
    }
  }
}



public:
  void setup()
  {


        alt_toggle_on = vesc_light_on;

  //** Default VESC brate is 115200, you can change it to any other value. */
  Serial2.begin(115200, SERIAL_8N1, VESC_RX, VESC_TX);
  /** Define which ports to use as UART */
  UART.setSerialPort(&Serial2);

    transitionDelay = 5000;
    bri = 0;stateUpdated(1);

    bootPreset = dim_preset;
    // set pin modes
    strip.addEffect(FX_MODE_FB_ACCELERATION_BLINK, &mode_fb_acceleration_blink, _data_fx_fb_acceleration_blink);
    strip.addEffect(FX_MODE_F_ACCELERATION_BLINK, &mode_f_acceleration_blink, _data_fx_f_acceleration_blink);
    strip.addEffect(FX_MODE_B_ACCELERATION_BLINK, &mode_b_acceleration_blink, _data_fx_b_acceleration_blink);

    strip.addEffect(FX_MODE_COUNTDOWN, &mode_countdown, _data_fx_mode_countdown);
    strip.addEffect(FX_MODE_TRAILRATE, &mode_rate_trail, _data_fx_mode_rate_trail);
    strip.addEffect(FX_MODE_WHEELTEMP, &mode_wheel_temp, _data_fx_mode_wheel_temp);
    strip.addEffect(FX_MODE_MOSFETTEMP, &mode_mosfet_temp, _data_fx_mode_mosfet_temp);
    strip.addEffect(FX_MODE_TIREPRESSURE, &mode_tire_pressure, _data_fx_mode_tire_pressure);

    strip.addEffect(FX_MODE_STOCK_FRONT, &mode_stock_front, _data_fx_mode_stock_front);
    strip.addEffect(FX_MODE_STOCK_BACK, &mode_stock_back, _data_fx_mode_stock_back);

    strip.addEffect(FX_MODE_ACCEL_TEST, &mode_accel_test, _data_fx_mode_accel_test);

    strip.addEffect(FX_MODE_COUNTDOWN_FADE, &mode_countdown_fade, _data_fx_mode_countdown_fade);
    strip.addEffect(FX_MODE_RATE_TRAIL_FADE, &mode_rate_trail_fade, _data_fx_mode_rate_trail_fade);
    strip.addEffect(FX_MODE_TIRE_PRESSURE_FADE, &mode_tire_pressure_fade, _data_fx_mode_tire_pressure_fade);
    strip.addEffect(FX_MODE_WHEEL_TEMP_FADE, &mode_wheel_temp_fade, _data_fx_mode_wheel_temp_fade);
    strip.addEffect(FX_MODE_HUMIDITY, &mode_humidity, _data_fx_mode_humidity);
    strip.addEffect(FX_MODE_BATPERCENT, &mode_batpercent, _data_fx_mode_batpercent);


    pinMode(FRONT_LIGHT_W_PIN, INPUT);
    pinMode(FRONT_LIGHT_R_PIN, INPUT);


    pinMode(ERROR_LED_PIN, OUTPUT);
    digitalWrite(ERROR_LED_PIN, LOW);

   adxl.powerOn();                     // Power on the ADXL345

   adxl.setRangeSetting(16);          // Accepted values are 2g, 4g, 8g or 16g
                                      // Higher Values = Wider Measurement Range
                                      // Lower Values = Greater Sensitivity

   adxl.setSpiBit(0);                  // Configure the device to be in 4 wire SPI mode when set to '0' or 3 wire SPI mode when set to 1
                                      // Default: Set to 1
                                      // SPI pins on the ATMega328: 11, 12 and 13 as reference in SPI Library

   adxl.setActivityXYZ(1, 1, 1);       // Set to activate movement detection in the axes "adxl.setActivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
   adxl.setActivityThreshold(50);      // 62.5mg per increment   // Set activity   // Inactivity thresholds (0-255)

   adxl.setInactivityXYZ(1, 1, 1);     // Set to detect inactivity in all the axes "adxl.setInactivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
   adxl.setInactivityThreshold(25);    // 62.5mg per increment   // Set inactivity // Inactivity thresholds (0-255)
   adxl.setTimeInactivity(1);         // How many seconds of no activity is inactive?

   adxl.setTapDetectionOnXYZ(1, 1, 1); // Detect taps in the directions turned ON "adxl.setTapDetectionOnX(X, Y, Z);" (1 == ON, 0 == OFF)

   // Set values for what is considered a TAP and what is a DOUBLE TAP (0-255)
   adxl.setTapThreshold(50);           // 62.5 mg per increment
   adxl.setTapDuration(15);            // 625 μs per increment
   adxl.setDoubleTapLatency(80);       // 1.25 ms per increment
   adxl.setDoubleTapWindow(200);       // 1.25 ms per increment

   // Set values for what is considered FREE FALL (0-255)
   adxl.setFreeFallThreshold(7);       // (5 - 9) recommended - 62.5mg per increment
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

   Wire.begin();// for humidity
   humiditySensor.begin();

   get_imu_data();
   get_imu_data();
   get_imu_data();
   get_imu_data();
   get_imu_data(); // get imu data twice to populate filtered ints
   if (filteredy < -10){orientation = 2;}
   if (filteredy > 10){orientation = 3;}
   ///////////////////////////////////////////////////////  wifi



   get_humidity();
   }// end of start up

  void loop()
  {

    if (strip.isUpdating()){return;}


          if (free_fall_preset != 250){
      if (person_on_ui){return;}
    }// end of free fall preset bypass mode

if ((a_read_milisec + 50) < millis()){    // limit loop to 20 times a sec
a_read_milisec = millis();

get_front_light();  // handels truning on/off lights and forward/back detection

}else{
  return; // skip rest of loop
}


     if ((millis()) < (8 * 1000)){
      get_humidity();
      imu_inactivity_count = 11;
      imu_activity = false;
      return;  // returns loop if boot animation hasnt finished playing
     }

get_imu_data();
last_active();//updates when board was last active for preset animation

    if (!imu_free_fall){
      set_preset();
    }

handle_tpms();


/////////////////////////////////////// activity (used for trail detection) "interrupt"
  if (imu_activity)
  {
    imu_activity = false;
    imu_inactivity_count = 0;
  }
/////////////////////////////////////////////////////inactivity "interrupt"
  if (imu_inactivity){
get_humidity();

        imu_inactivity = false;

              if (imu_inactivity_milis < millis()){
      imu_inactivity_milis = millis() + 1050;
    }else{
      imu_inactivity_milis = imu_inactivity_milis + 1050;
    }
    imu_inactivity_count = imu_inactivity_count + 1;
  }

    if (imu_inactivity_milis < millis()){
   imu_inactivity_count = 0;
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
      bmss = root["bmss"] | bmss; //bms state of charge




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
    lux1.add(dutycycle);                       //right side variable

            JsonArray bat1 = hidden.createNestedArray(F("tv")); //totale volatage
    bat1.add(battery_voltage);
                JsonArray bat3 = hidden.createNestedArray(F("bmss")); //bms state of charge
    bat3.add(bmss);


    JsonObject user = root[F("u")];
    if (user.isNull())
      user = root.createNestedObject(F("u"));

                        JsonArray vesc5 = user.createNestedArray("RPM");  //left side thing
      vesc5.add(smoothedrpm);  

                              JsonArray vesc6 = user.createNestedArray("Duty cycle");  //left side thing
      vesc6.add(dutycycle);  
                                   //right side variable
                        JsonArray vesc0 = user.createNestedArray("Current");  //left side thing
      vesc0.add(current);                               //right side variable

                              JsonArray vesc1 = user.createNestedArray("Distance");  //left side thing
      vesc1.add(distance);                               //right side variable

                              JsonArray vesc2 = user.createNestedArray("Mosfet Temp");  //left side thing
      vesc2.add(mosfettemp);                               //right side variable

                              JsonArray vesc3 = user.createNestedArray("Motor Temp");  //left side thing
      vesc3.add(motortemp);                               //right side variable

                              JsonArray vesc4 = user.createNestedArray("Batt Percentage");  //left side thing
      vesc4.add(batpercentage);                               //right side variable

                  JsonArray battery6 = user.createNestedArray("Bumpyness");  //left side thing
      battery6.add(display_trail_ruffness);                               //right side variable
      
                        JsonArray battery55 = user.createNestedArray("orientation");  //left side thing
      battery55.add(orientation);                               //right side variable

                              JsonArray battery59 = user.createNestedArray("Humidity %");  //left side thing
      battery59.add(humidity);                               //right side variable

                                    JsonArray battery591 = user.createNestedArray("AvaSpark-RGB Temp C");  //left side thing
      battery591.add(andonn_temp);                               //right side variable

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

     /*
     * addToConfig() (called from set.cpp) stores persistent properties to cfg.json
     */
  void addToConfig(JsonObject &root)
  {
    // we add JSON object.
    JsonObject top = root.createNestedObject(FPSTR(_name)); // usermodname
    top[FPSTR(_vesc_light_on)] = vesc_light_on;
    top[FPSTR(_alt_toggle)] = alt_toggle;
    #ifndef SIMPLE_CONFIG
    top[FPSTR(_alt_mode_user)] = alt_mode_user;
    top[FPSTR(_choosen_slow_preset)] = choosen_slow_preset;  //int input
    top[FPSTR(_choosen_fast_preset)] = choosen_fast_preset;  //int input
    top[FPSTR(_motor_duty_slow)] = motor_duty_slow;  //int input
    top[FPSTR(_motor_duty_fast)] = motor_duty_fast;  //int input
    top[FPSTR(_alt_forwards_preset)] = alt_forwards_preset;  //int input
    top[FPSTR(_alt_backwards_preset)] = alt_backwards_preset;  //int input
    #endif
    top[FPSTR(_forwards_preset)] = forwards_preset;  //int input
    top[FPSTR(_backwards_preset)] = backwards_preset;  //int input
    top[FPSTR(_dim_preset)] = dim_preset;  //int input

    top[FPSTR(_dim_standing_up_preset)] = dim_standing_up_preset;  //int input

    top[FPSTR(_free_fall_preset)] = free_fall_preset;  //int input

    top[FPSTR(_is_vesc_main)] = is_vesc_main;
    top[FPSTR(_no_input)] = no_input;

    top[FPSTR(_BatteryCells)] = BatteryCells;
    top[FPSTR(_direction_threshold)] = direction_threshold;


    DEBUG_PRINTLN(F("AvaSpark-RGB config saved."));
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
    #ifndef SIMPLE_CONFIG
    is_vesc_main            = (top[FPSTR(_is_vesc_main)] | is_vesc_main);       //bool
    no_input            = (top[FPSTR(_no_input)] | no_input);       //bool
    choosen_slow_preset   = top[FPSTR(_choosen_slow_preset)] | choosen_slow_preset;  //int input
    choosen_fast_preset   = top[FPSTR(_choosen_fast_preset)] | choosen_fast_preset;    //int input
    motor_duty_slow   = top[FPSTR(_motor_duty_slow)] | motor_duty_slow;          //int input
    motor_duty_fast   = top[FPSTR(_motor_duty_fast)] | motor_duty_fast;     //int input
    #endif
    forwards_preset   = top[FPSTR(_forwards_preset)] | forwards_preset;     //int input
    backwards_preset   = top[FPSTR(_backwards_preset)] | backwards_preset;     //int input
    dim_preset   = top[FPSTR(_dim_preset)] | dim_preset;     //int input
    #ifndef SIMPLE_CONFIG
    alt_mode_user            = (top[FPSTR(_alt_mode_user)] | alt_mode_user);       //bool
    alt_backwards_preset   = top[FPSTR(_alt_backwards_preset)] | alt_backwards_preset;     //int input
    alt_forwards_preset   = top[FPSTR(_alt_forwards_preset)] | alt_forwards_preset;     //int input
    #endif
    dim_standing_up_preset   = top[FPSTR(_dim_standing_up_preset)] | dim_standing_up_preset;     //int input
    vesc_light_on            = (top[FPSTR(_vesc_light_on)] | vesc_light_on);       //bool
    alt_toggle            = (top[FPSTR(_alt_toggle)] | alt_toggle);       //bool
    free_fall_preset   = top[FPSTR(_free_fall_preset)] | free_fall_preset;     //int input
    BatteryCells            = (top[FPSTR(_BatteryCells)] | BatteryCells);       //bool
    direction_threshold            = (top[FPSTR(_direction_threshold)] | direction_threshold);       //bool

    DEBUG_PRINT(FPSTR(_name));
    DEBUG_PRINTLN(F(" config (re)loaded."));

    // use "return !top["newestParameter"].isNull();" when updating Usermod with new features
    return true;
  }
};

// strings to reduce flash memory usage (used more than twice)
//                           _veriable         "what it says on the webpage"
const char Usermodvesc::_name[] PROGMEM = "AvaSpark-RGB user preset configuration";
const char Usermodvesc::_dim_preset[] PROGMEM = "Idle lighting preset";
const char Usermodvesc::_backwards_preset[] PROGMEM = "Reverse travel lighting preset";
const char Usermodvesc::_vesc_light_on[] PROGMEM = "Default lights on or off";
const char Usermodvesc::_is_vesc_main[] PROGMEM = "UART mode ON / RGB input mode off";
const char Usermodvesc::_no_input[] PROGMEM = "Aceleromter only input";
const char Usermodvesc::_dim_standing_up_preset[] PROGMEM = "Inactive standing up lighting preset";
const char Usermodvesc::_free_fall_preset[] PROGMEM = "Freefall lighting preset";
const char Usermodvesc::_direction_threshold[] PROGMEM = "Experimental Direction threshold for Accel only";

#ifndef SIMPLE_CONFIG
const char Usermodvesc::_choosen_slow_preset[] PROGMEM = "Low duty preset animation";
const char Usermodvesc::_choosen_fast_preset[] PROGMEM = "High duty preset animation";
const char Usermodvesc::_motor_duty_slow[] PROGMEM = "Low duty motor duty %";
const char Usermodvesc::_motor_duty_fast[] PROGMEM = "High duty motor duty %";
const char Usermodvesc::_alt_mode_user[] PROGMEM = "Toggle alt presets by laying on other side";
const char Usermodvesc::_alt_forwards_preset[] PROGMEM = "Alt forward travel lighting preset";
const char Usermodvesc::_alt_backwards_preset[] PROGMEM = "Alt reverse travel lighting preset";
#endif
const char Usermodvesc::_alt_toggle[] PROGMEM = "Toggle on off by laying on side";
const char Usermodvesc::_forwards_preset[] PROGMEM = "Forward travel lighting preset";
const char Usermodvesc::_BatteryCells[] PROGMEM = "Battery Cells series";