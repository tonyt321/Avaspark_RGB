#pragma once

#include "SparkFun_ADXL345.cpp"         // SparkFun ADXL345 Library
#include "SparkFun_ADXL345.h"         // SparkFun ADXL345 Library
#include "wled.h"
#include <Wire.h>



class UsermodAndon : public Usermod
{
private:


ADXL345 adxl = ADXL345();  // USE FOR I2C COMMUNICATION

#ifdef PRO_VERSION
  int  LIGHT_BAR_R_ANALOG;
  bool LIGHT_BAR_R = false;
  bool LIGHT_BAR_G = false;
  bool LIGHT_BAR_B = false;

  bool Status_bar = false;
  bool battery_bar = false;

  unsigned long motor_duty_est; // slow on the build up quick on the slow down
  //unable to shape with caps diffrently without being more space on the pcb and thru hole sodlering
  unsigned long battery_voltage_est; // estimated battery voltage (might be a bit off due to ADC non linearaity) 
  //https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2019/05/ADC-non-linear-ESP32.png?w=768&quality=100&strip=all&ssl=1
  int battery_percent;
  
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
  int dim_left_preset = 0;   //unused yet
  int dim_right_preset = 0;   //unused yet

              
  unsigned int free_fall_preset = 1; // preset after free fall
  int8_t boot_preset = 1;  //preset played as a boot animation
  
  int boot_preset_time = 3; // boot animation length in sec
  unsigned long start_milisec; // for tracking how much time has past for boot animation preset


  bool FRONT_LIGHT_R = false;
  int FRONT_LIGHT_R_ANALOG;
  bool FRONT_LIGHT_W = false;
  int FRONT_LIGHT_W_ANALOG;
  
  // flag set at startup
  int client_numb;
  bool forward = true; //on startup assume forware movment
  bool app_lights_on;  // are the lights on in the app?

  bool app_lights_on_last; // last check value of lights on
  int blink_app_lights = 0;
  unsigned long blink_app_lights_timing;

  bool stock = true;

  unsigned int free_fall_preset_time = 3; // animation length in sec
  unsigned long free_fall_milisec; // for tracking how much time has past for free_fall animation preset
  bool imu_free_fall = false;
  
  int rawx , rawy , rawz;
  int filteredx , filteredy , filteredz;
  int normx , normy , normz;
  int smoothedy;

  bool wifi_change = true;

//////////////////////////////Global var for effects
//int_display_tire_temp
//int_display_battery
//int_display_duty_cycle
//int_display_tire_psi
//int_display_trail_ruffness
//int_imu_speed
  
  bool dimmed_lights = false; // are the front lights dimmed

  bool imu_activity = true;
  bool imu_inactivity = true;

  bool side_left = false;   //how is the board on the ground
  bool side_right = false;
  bool upside_down = false;
  bool upright = true;


  unsigned int trail_ruffness_max = 30;   // max activations per min for the bar graph
  unsigned long trail_ruffness = 0; //how ruff the trail is using active activations per min and how on pro version use motor disengadements
  unsigned long inactive_millis; // subrated inactive sec from trail calculations
   unsigned long inactive_millis_last; // subrated inactive sec from trail calculations

  // strings to reduce flash memory usage (used more than twice)
  static const char _name[];
  #ifdef PRO_VERSION
  static const char _low_bat_percent[];
  static const char _low_bat_preset[];
  static const char _Status_bar[];
  static const char _battery_bar[];
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

  static const char _boot_preset[];
  static const char _boot_preset_time[];
  static const char _stock[];
  static const char _trail_ruffness_max[];
  static const char _free_fall_preset_time[];
  static const char _free_fall_preset[];






  //ANDON PRO-specific functions:
  /**



    void is_CHARGING()
    {
      //look at VBATT and see if the voltage is going up at a steady rate, if so pass this charge rate value onto a different function to
      // scale the animation by that amount (allows you to the user the ability to see how fast a board is charging/if its balancing by visual confirmation alone)
    }

    void //IS_REGEN() //look at motor duty cycle to see if it is negative (?) indicating if the board is receiving regen current
    // from the motor. (verify power is being gained via with is_CHARGING(), and if the IMU measures decelleration to confirm that the board is regen'ing)
    //motor duty may also spike after drop maybe? so maybe use IMU free fall to block that out?
    {

    }

    void OVERRIDE_LIGHTBAR() //when the lightbar is not being turned on by FM controls, enable this function
    {
      //to tell Andon that it is able to output data to the lightbar (requires additinal LEDs/hardware to light up the lightbar)
    }

    void SHOP_MODE() //enables shop diagnostics tools
    {

    }

    void SPEEDSCALE_BRIGHTNESS() //As the board goes faster, make the headlights get brighter and the taillights dimmer, so the 
    //total amount of power used remains the same. This increases headlight throw, which is more useful at speed
    {

    }


    void isMallgrab() //using IMU_POSN_INPUT, determine if the board is being carried vertically by the front handle
    {

    }

    void isBraking() //using IMU_POSN_INPUT, determine when the board is decellerating, blink the brake light 
    // eitherred/ or something with high visibility like F1 cars in the rain
    {

    }

    void trickTrainer() //using IMU_POSN_INPUT, take pre-defined profiles for things like curb nudges, bonks, revert 180's, etc
    //and quantify/define them as a function of motor speed/acceleration, board orientation, and cartesian position to help new
    //riders better achieve tricks. the idea is to provide feedback of "you need to push the nose down harder to nudge" or
    //you need to be going faster and sink the nose more to 180" upon attempting a trick, the LEDs are on a color-scale from
    //red to green (good to bad) of how closely your attempt matched the profile for that trick. (identifying how a trick is
    //defined could conceptuallybe as simple as recording the same metrics used to evaluate the new rider to compare against)
    {

    }

    void timer_countdown() //auxilliary countdown timer to be used within other functions, mainly for transition delays
    // between board enganged/disengaged and direction change states

  **/


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
         handleSet(nullptr, together2 , false );

         String redstring1 = "win&SB=255&FX=98&SM=0&SS=0&IX=0&R=" + error_red;  //combining multiple strings at once can result in unpredictable outcomes
         String greenstring1 = "&G=" + error_green;
         String bluestring1 = "&B=" + error_blue;
         String together3 = redstring + greenstring;
         String together4 = together1 + bluestring;
         handleSet(nullptr, together4 , false );

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
    if (MODEL == 0){battery_percent = map(battery_voltage_est, 59.4, 73.8, 0, 100);}
    if (MODEL == 1){battery_percent = map(battery_voltage_est, 49.5, 61.5, 0, 100);}
    if (MODEL == 2){battery_percent = map(battery_voltage_est, 49.5, 61.5, 0, 100);}
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
    int ar = analogRead(MOTOR_SPEED_PIN); // ar analog read
    motor_duty_est = ((ar / 380) * 100); // outputs a 

    if (ar < 15) // if analog read is less the 15 set dudty cycle to 0 (disengaded)
    {
      motor_duty_est = 0;
    }
  }
#endif

#ifdef PRO_VERSION
  void set_motor_duty_preset()
  {  
    GET_DUTYCYCLE();
   if (motor_duty_est == 0)
   { return; }

   if (motor_duty_slow < motor_duty_est)
   { applyPreset(choosen_slow_preset); return; }

   if (motor_duty_med < motor_duty_est)
   { applyPreset(choosen_med_preset); return; }

   if (motor_duty_fast < motor_duty_est)
   { applyPreset(choosen_fast_preset); return; }
  }
#endif

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
     int_display_trail_ruffness = (activations_per_min / trail_ruffness_max);
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
    imu_free_fall = true;
    free_fall_milisec = millis();
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

   if (dimmed_lights){  //only detect a left right or upside down orientaion if the lights are dim 
   if (filteredz < -10){upside_down = true; side_left = false; side_right = false; upright = false;}
   if (filteredy < -20){side_right = true; side_left = false; upside_down = false; upright = false;}
   if (filteredy > 20){side_left = true; side_right = false; upside_down = false; upright = false;}
   }

   if (filteredz > 10){upright = true;side_left = false; side_right = false; upside_down = false;}

} // end of get IMU data


  void get_front_light()
   {

    //0      on
    //1659   inactive (when you get off the board and it dims the lights)
    //4095   off
    

    FRONT_LIGHT_W_ANALOG = analogRead(FRONT_LIGHT_W_PIN);

    if (FRONT_LIGHT_W_ANALOG > 2000){
      FRONT_LIGHT_W = false;
      }else{
        FRONT_LIGHT_W = true;
        }

    FRONT_LIGHT_R_ANALOG = analogRead(FRONT_LIGHT_R_PIN);
    if (FRONT_LIGHT_R_ANALOG > 2000){
      FRONT_LIGHT_R = false; app_lights_on = false; dimmed_lights = false;
      }else{
        FRONT_LIGHT_R = true; app_lights_on = true;
        }

      if (FRONT_LIGHT_W == FRONT_LIGHT_R){ 
        forward = true;
      } else {
        forward = false;
      }

   if ((FRONT_LIGHT_R_ANALOG > 1000) && (FRONT_LIGHT_R_ANALOG < 2000)){
    dimmed_lights = true;
   } else {
    dimmed_lights = false;
   }

        if (app_lights_on_last != app_lights_on){
          if ((millis() - blink_app_lights_timing) < BLINK_APP_LIGHTS_DELAY){ //if time seince last toggle less then 1 sec
            blink_app_lights = blink_app_lights + 1;
          }else{
          blink_app_lights = 0;
          }
    blink_app_lights_timing = millis();
    }
    app_lights_on_last = app_lights_on;

   if (app_lights_on == false){ //turns lights off if in app lights are off
   turn_all_light_off();
   }else{
    turn_all_light_on();
   }
  }

void turn_all_light_off(){
     handleSet(nullptr, "win&S=0&S2=13&SS=0&SM=0&SV=2" , false );  // select seg 0 & set main seg 0 & de select other seg
   handleSet(nullptr, "win&T=0&SB=0&S=0&S2=13" , false );// turn all off

   handleSet(nullptr, "win&S=13&S2=26&SS=1&SM=1&SV=2" , false );
   handleSet(nullptr, "win&T=0&SB=0&S=13&S2=26" , false );// turn all off
}

void turn_all_light_on(){
     handleSet(nullptr, "win&S=0&S2=13&SS=0&SM=0&SV=2" , false );  // select seg 0 & set main seg 0 & de select other seg
   handleSet(nullptr, "win&T=1&SB=255&S=0&S2=13" , false );// turn all on

   handleSet(nullptr, "win&S=13&S2=26&SS=1&SM=1&SV=2" , false );
   handleSet(nullptr, "win&T=1&SB=255&S=13&S2=26" , false );// turn all on
}

//now it shows battery on front and back
//when the board is idle, display the battery level as a bar graph upon the rear led pcb upon the ground
//optionally (if easy to program) set the front led pcb to visualize cell balance levels at a very dim brightness
#ifdef PRO_VERSION
  void BATTERY_VISUALIZER() //show battery % on lights
  {

 handleSet(nullptr, "win&SB=255&FX=98&SM=0&SS=0&G=255&R2=255&IX=0" , false );
 handleSet(nullptr, "win&SB=255&FX=98&SM=0&SS=0&G=255&R2=255&IX=" + battery_percent , false );

 handleSet(nullptr, "win&SB=255&FX=98&SM=1&SS=1&G=255&R2=255&IX=0" , false );
 handleSet(nullptr, "win&SB=255&FX=98&SM=1&SS=1&G=255&R2=255&IX=" + battery_percent , false );

 }
#endif

  void emulate_stock()
   {
       if (app_lights_on == (false)){ //turns lights off if in app lights are off
   handleSet(nullptr, "win&S=0&S2=13&SS=0&SM=0&SV=2" , false );  // select seg 0 & set main seg 0 & de select other seg
   handleSet(nullptr, "win&T=0&SB=0&S=0&S2=13" , false );// turn all off

   handleSet(nullptr, "win&S=13&S2=26&SS=1&SM=1&SV=2" , false );
   handleSet(nullptr, "win&T=0&SB=0&S=13&S2=26" , false );// turn all off
   return; // skip rest of loop becuase we dont want to change lights besides forward/back
   }else{

   if ((forward) == true) {

   if (dimmed_lights == true){
   handleSet(nullptr, "win&S=0&S2=13&SS=0&SM=0&SV=2" , false );  // select seg 0 & set main seg 0 & de select other seg
   handleSet(nullptr, "win&FX=0&G=0&B=0&R=100&W=0&TT=1000&T=1&S=0&S2=13" , false );
 
   handleSet(nullptr, "win&S=13&S2=26&SS=1&SM=1&SV=2" , false );
   handleSet(nullptr, "win&FX=0&G=100&B=100&R=100&W=100&TT=3000&T=1&S=13&S2=26" , false );

   } else {
   handleSet(nullptr, "win&S=0&S2=13&SS=0&SM=0&SV=2" , false );  // select seg 0 & set main seg 0 & de select other seg
   handleSet(nullptr, "win&FX=0&G=0&B=0&R=255&W=0&TT=1000&T=1&S=0&S2=13" , false );
 
   handleSet(nullptr, "win&S=13&S2=26&SS=1&SM=1&SV=2" , false );
   handleSet(nullptr, "win&FX=0&G=255&B=255&R=255&W=255&TT=3000&T=1&S=13&S2=26" , false );
   }
   } else {
   if (dimmed_lights == true){
   handleSet(nullptr, "win&S=0&S2=13&SS=0&SM=0&SV=2" , false );  // select seg 0 & set main seg 0 & de select other seg
   handleSet(nullptr, "win&FX=0&G=100&B=100&R=100&W=100&TT=1000&T=1&S=0&S2=13" , false );
 
   handleSet(nullptr, "win&S=13&S2=26&SS=1&SM=1&SV=2" , false );
   handleSet(nullptr, "win&FX=0&G=0&B=0&R=100&W=0&TT=1000&T=1&S=13&S2=26" , false );
   } else {
     handleSet(nullptr, "win&S=0&S2=13&SS=0&SM=0&SV=2" , false );  // select seg 0 & set main seg 0 & de select other seg
     handleSet(nullptr, "win&FX=0&G=255&B=255&R=255&W=255&TT=1000&T=1&S=0&S2=13" , false );
 
     handleSet(nullptr, "win&S=13&S2=26&SS=1&SM=1&SV=2" , false );
     handleSet(nullptr, "win&FX=0&G=0&B=0&R=255&W=0&TT=1000&T=1&S=13&S2=26" , false );
     }
    }
   }
}


void set_preset(){


  bool side_left = false;   //how is the board on the ground
  bool side_right = false;
  bool dimmed_lights = false; // are the front lights dimmed
  bool forward = true; //on startup assume forware movment
  
  if(upright == true){
  if(forward){
    if(dimmed_lights == false){
   #ifdef PRO_VERSION
   set_motor_duty_preset();
   #else
   applyPreset(forwards_preset);
   #endif

   }else{
    applyPreset(dim_forwards_preset);
   }

  }else{
    if(dimmed_lights == false){
      applyPreset(backwards_preset);  
    }else{
  applyPreset(dim_backwards_preset);
    }
  }
  }else{
 if (side_left == true){applyPreset(dim_left_preset);}
 if (side_right == true){applyPreset(dim_right_preset);}
  }

}


public:
  void setup()
  {
    // set pin modes

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

   adxl.setRangeSetting(16);           // Give the range settings
                                      // Accepted values are 2g, 4g, 8g or 16g
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



 
   #ifdef TEST_MODE
   app_lights_on = true;  // set as if lights are detected as always on in test mode
   #endif

   #ifndef TEST_MODE // test mode skip get front light becuase we dont have the hardware on test esp32
   get_front_light();  // handels truning on/off lights and forward/back detection
   #endif
   

   if (stock){ // if emulate stock is off use boot up preset

    if (boot_preset_time != 0){ // skip if boot_preset_time set to 0
    start_milisec = millis();
    applyPreset(boot_preset);// start up animation plays for 3 sec or so (still need to implement switching back)
    }

   }

   get_imu_data();
   get_imu_data();
   get_imu_data(); // get imu data twice to populate filtered ints
   if (filteredz < -10){upside_down = true;} //  only used out side of inactivity interupt because we cant wait in startup for that
   ///////////////////////////////////////////////////////  wifi
   //#ifndef TEST_MODE
   if (upside_down == true){
          apBehavior = AP_BEHAVIOR_BUTTON_ONLY;
          apActive = false;    
          WLED::instance().initAP(false);
          //dnsServer.stop();
          WiFi.softAPdisconnect(true);           // Disable Wifi
          WLED::instance().disableWiFi();
          WLED::instance().handleConnection();
   }
   //#endif

}// end of start up 

  void loop()
  {

    if (strip.isUpdating()){return;}




    wifi_sta_list_t stationList;  //skip looping code if user is on wifi so we dont change stuff while they are editing
    esp_wifi_ap_get_sta_list(&stationList);
    client_numb = stationList.num;
    if ( client_numb != 0 ){
      handleSet(nullptr, "win&T=1" , false );// turn all on
    return; 
    }


   #ifndef TEST_MODE // test mode skip get direction from front light becuase we dont have the hardware on test esp32
   get_front_light();  // handels truning on/off lights and forward/back detection
   if (app_lights_on == false){
       return; // skip rest of loop
   }
   #endif


   if (stock == false){
   emulate_stock();
   return; // skip rest of loop becuase we dont want to change lights besides forward/back
   }

             
     if (((millis()) - start_milisec) < (boot_preset_time * 1000)){
      return;  // returns loop if boot animation hasnt finished playing
     }


   get_imu_data();
   set_preset();


 //  if (blink_app_lights >= 3){ //if lights in Onewheel app are flashed on off 3 times
 //  } else{
  // }

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
    if (imu_free_fall && (free_fall_preset_time != 0))
  {
    applyPreset(free_fall_preset);
    if ((free_fall_milisec + (free_fall_preset_time * 1000)) < millis()){
      imu_free_fall = false;
    }
    return;  // returns loop if free fall animation hasnt finished playing
  }
///////////////////////////////////////////////////////////




   #ifdef PRO_VERSION  //rest of loop is pro only features
    GET_BATT_LEVEL();
    

    if ((Status_bar) == true){
    MIMIC_ERROR_CODES(); // sets front and back to same color as status bar when error and disengaged
    }

    if ((low_bat_percent < battery_percent) && (low_bat_percent != 0)){  //if user set low battery %   less than    actual battery %
    set_motor_duty_preset();
    } else {
     applyPreset(low_bat_preset);
    }

     GET_LIGHT_BAR();

     if (((battery_bar) == true) && (motor_duty_est == 0))
     {
     BATTERY_VISUALIZER();   //show battery % of front and back lights
     }
   #endif
   
  } // end of main loop

  void addToJsonInfo(JsonObject &root)
  {
    JsonObject user = root[F("u")];
    if (user.isNull())
      user = root.createNestedObject(F("u"));


#ifdef PRO_VERSION
    JsonArray lux = user.createNestedArray(F(" lol")); //left side thing
    lux.add(LIGHT_BAR_R_ANALOG);                       //right side variable
    lux.add(F(" RED analog read"));                    //right side thing


      JsonArray battery = user.createNestedArray("blue level");  //left side thing
      battery.add(LIGHT_BAR_B);                               //right side variable
      battery.add(F(" BLUE GPIO read"));                      //right side thing
#endif
      JsonArray shop = user.createNestedArray("Andon Origin");  //left side thing
      shop.add(SHOP_NAME);                               //right side variable

      JsonArray lights = user.createNestedArray("X Axis");  //left side thing
      lights.add(filteredx);                               //right side variable
      lights.add(F(" xAxis"));                      //right side thing

            JsonArray lights1 = user.createNestedArray("y Axis");  //left side thing
      lights1.add(filteredy);                               //right side variable
      lights1.add(F(" yAxis"));                      //right side thing

            JsonArray lights2 = user.createNestedArray("z Axis");  //left side thing
      lights2.add(filteredz);                               //right side variable
      lights2.add(F(" zAxis"));                      //right side thing

            JsonArray battery1 = user.createNestedArray("app_lights_on");  //left side thing
      battery1.add(app_lights_on);                               //right side variable
      battery1.add(F(" app_lights_on"));                      //right side thing

                  JsonArray battery3 = user.createNestedArray("red analog");  //left side thing
      battery3.add(FRONT_LIGHT_R_ANALOG);                               //right side variable
      battery3.add(F(" RED GPIO read"));                      //right side thing

            JsonArray battery4 = user.createNestedArray("white analog");  //left side thing
      battery4.add(FRONT_LIGHT_W_ANALOG);                               //right side variable
      battery4.add(F(" WHITE GPIO read"));                      //right side thing

                  JsonArray battery6 = user.createNestedArray("activations per min");  //left side thing
      battery6.add(int_display_trail_ruffness);                               //right side variable
      battery6.add(F(""));                      //right side thing
  }

  uint16_t getId()
  {
    return USERMOD_ID_ANDON_MOD;
  }

  /**
     * addToConfig() (called from set.cpp) stores persistent properties to cfg.json
     */
  void addToConfig(JsonObject &root)
  {
    // we add JSON object.
    JsonObject top = root.createNestedObject(FPSTR(_name)); // usermodname
    top[FPSTR(_stock)] = !stock;
    #ifdef PRO_VERSION
    top[FPSTR(_Status_bar)] = !Status_bar;   //bool
    top[FPSTR(_battery_bar)] = !battery_bar; //bool
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
    top[FPSTR(_dim_backwards_preset)] = backwards_preset;  //int input
    top[FPSTR(_dim_forwards_preset)] = backwards_preset;  //int input
    top[FPSTR(_dim_left_preset)] = dim_left_preset;  //int input
    top[FPSTR(_dim_right_preset)] = dim_right_preset;  //int input

    top[FPSTR(_boot_preset)] = boot_preset;  //int input
    top[FPSTR(_boot_preset_time)] = boot_preset_time;  //int input
    top[FPSTR(_free_fall_preset)] = free_fall_preset;  //int input
    top[FPSTR(_trail_ruffness_max)] = trail_ruffness_max;  //int input

      

    DEBUG_PRINTLN(F("Andon config saved."));
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
    Status_bar       = !(top[FPSTR(_Status_bar)] | !Status_bar);    //bool
    battery_bar      = !(top[FPSTR(_battery_bar)] | !battery_bar);  //bool
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
    backwards_preset   = top[FPSTR(_dim_backwards_preset)] | backwards_preset;     //int input
    backwards_preset   = top[FPSTR(_dim_forwards_preset)] | backwards_preset;     //int input
    dim_left_preset   = top[FPSTR(_dim_left_preset)] | dim_left_preset;     //int input
    dim_right_preset   = top[FPSTR(_dim_right_preset)] | dim_right_preset;     //int input

    boot_preset   = top[FPSTR(_boot_preset)] | boot_preset;     //int input
    boot_preset_time   = top[FPSTR(_boot_preset_time)] | boot_preset_time;     //int input
    stock            = !(top[FPSTR(_stock)] | !stock);       //bool
    free_fall_preset   = top[FPSTR(_free_fall_preset)] | free_fall_preset;     //int input
    trail_ruffness_max   = top[FPSTR(_trail_ruffness_max)] | trail_ruffness_max;     //int input
    DEBUG_PRINT(FPSTR(_name));
    DEBUG_PRINTLN(F(" config (re)loaded."));




    // use "return !top["newestParameter"].isNull();" when updating Usermod with new features
    return true;
  }
};

// strings to reduce flash memory usage (used more than twice)
//                           _veriable         "what it says on the webpage"
const char UsermodAndon::_name[] PROGMEM = "Enabled Features";
const char UsermodAndon::_stock[] PROGMEM = "Emulate stock lighting (overrides everything)";
#ifdef PRO_VERSION
const char UsermodAndon::_Status_bar[] PROGMEM = "Mirror Status bar error";
const char UsermodAndon::_battery_bar[] PROGMEM = "Display battery on dismount";
const char UsermodAndon::_low_bat_percent[] PROGMEM = "Battery percent to change preset (0 to disable) overrides duty cycle preset";
const char UsermodAndon::_low_bat_preset[] PROGMEM = "Low battery preset animation";

const char UsermodAndon::_choosen_slow_preset[] PROGMEM = "Slow preset animation";
const char UsermodAndon::_choosen_med_preset[] PROGMEM = "Med preset animation";
const char UsermodAndon::_choosen_fast_preset[] PROGMEM = "Fast preset animation";
const char UsermodAndon::_motor_duty_slow[] PROGMEM = "Slow motor duty %";
const char UsermodAndon::_motor_duty_med[] PROGMEM = "Med motor duty %";
const char UsermodAndon::_motor_duty_fast[] PROGMEM = "fast motor duty %";
#else
const char UsermodAndon::_forwards_preset[] PROGMEM = "Preset animation to use while riding forwards";
#endif
const char UsermodAndon::_dim_forwards_preset[] PROGMEM = "Preset animation to use when board is inactive going forwards";
const char UsermodAndon::_backwards_preset[] PROGMEM = "Preset animation to use when riding backwards";
const char UsermodAndon::_dim_backwards_preset[] PROGMEM = "Preset animation to use when board is inactive going backwards";

const char UsermodAndon::_dim_left_preset[] PROGMEM = "Preset animation to use when board is inactive on left side";
const char UsermodAndon::_dim_right_preset[] PROGMEM = "Preset animation to use when board is inactive on right side";


const char UsermodAndon::_boot_preset[] PROGMEM = "Preset animation to use as boot animation";
const char UsermodAndon::_boot_preset_time[] PROGMEM = "How long is boot preset in sec (0 to disable)";

const char UsermodAndon::_free_fall_preset[] PROGMEM = "Preset to display after a free fall";
const char UsermodAndon::_free_fall_preset_time[] PROGMEM = "How long is free fall preset in sec (0 to disable)";

const char UsermodAndon::_trail_ruffness_max[] PROGMEM = "max trail ruffness for bar graph";



