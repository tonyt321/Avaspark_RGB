#pragma once

#include "wled.h"


class UsermodAndon : public Usermod
{
private:


#ifdef PRO_VERSION
  int  LIGHT_BAR_R_ANALOG;
  bool LIGHT_BAR_R = false;
  bool LIGHT_BAR_G = false;
  bool LIGHT_BAR_B = false;

  bool Status_bar = false;
  bool battery_bar = false;

  float motor_duty_est; // slow on the build up quick on the slow down
  //unable to shape with caps diffrently without being more space on the pcb and thru hole sodlering
  float battery_voltage_est; // estimated battery voltage (might be a bit off due to ADC non linearaity) 
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
  int8_t choosen_preset = 1;
  #endif

  
  int8_t boot_preset = 1;  //preset played as a boot animation
  unsigned long boot_preset_time = 3; // boot animation length in msec
  unsigned long start_milisec; // for tracking how much time has past for boot animation preset


  bool FRONT_LIGHT_R = false;
  int FRONT_LIGHT_R_ANALOG;
  bool FRONT_LIGHT_W = false;
  int FRONT_LIGHT_W_ANALOG;
  
  // flag set at startup
  bool forward = true; //on startup assume forware movment
  bool app_lights_on;  // are the lights on in the app?
  bool skip_loop_wifi_clients_on = true;


  bool stock = false;
  
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
  static const char _choosen_preset[];
  #endif
  static const char _boot_preset[];
  static const char _boot_preset_time[];
  static const char _stock[];




  //ANDON PRO-specific functions:
  /**

    rider_present() // look at the white/red led signals from the controller and see if they are dim or bright. If dim, 
    //the board is idle and at rest, if they are bright the board is in use or has just been dismounted
    {

    }

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

    void IMU_POSN_INPUT() //determine board orientation 
    {

    }

    void isMallgrab() //using IMU_POSN_INPUT, determine if the board is being carried vertically by the front handle
    {

    }

    void isAirborne() //using IMU_POSN_INPUT, approximate if the board is travelling in an inverted parabola, if the board's trajectory
    // matches a perfect parabola within a programmable threshold, set a boolean to represent being airborne or not 
    {

    }

    void trailRate() //use IMU data input to quantify the following:
    // 1.) trail chunkiness - a function of z-variance detected by the IMU, measured over a long timescale
    // 2.) altitude gain/loss - a function of the average slope of z acceleration measured over long distances
    // 3.) average speed - a function of how high/low the average speed of a run is
    // 4.) motor disengagement - worse trails require you to dismount more frequently, better trails do not. count 
    // how many times the motor disengages during a given run
    //how is the start/stop of a trail or segment measured?
    //all of the above calculation results in a color assignment to the trail run, like how trails are rated green, blue, black, etc
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
    // if analog read is reading 5 it means there is a bad gnd connection, could be usefull info
    // for installation diagnostic and if it gets disconnected in useres board
  }
#endif

#ifdef PRO_VERSION
  void set_motor_duty_preset()
  {  
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


  void get_front_light()
  {

    FRONT_LIGHT_W = digitalRead(FRONT_LIGHT_W_PIN);
    FRONT_LIGHT_W_ANALOG = analogRead(FRONT_LIGHT_W_PIN);

    FRONT_LIGHT_R = digitalRead(FRONT_LIGHT_R_PIN);
    FRONT_LIGHT_R_ANALOG = analogRead(FRONT_LIGHT_R_PIN);

    if (((FRONT_LIGHT_W) == true) || ((FRONT_LIGHT_R) == true)){ 
      //switch to using analog input to detect switch sooner needs testing on live board
    app_lights_on = true;
    if ((FRONT_LIGHT_W) == true){  // if white rgbw front light is on board is going forward
       forward = true;
      } else {
       forward = false;
      }

    } else {
      app_lights_on = false;
      forward = true;  
      // if lights are off assume forward to avoid if someone turns lights
      // off while going backwards being stuck in backwards within the program
    }
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
if ((forward) == true) {
 handleSet(nullptr, "win&SB=255&FX=0&SM=0&SS=0&R=255&TT=1000" , false );
 handleSet(nullptr, "win&SB=255&FX=0&SM=1&SS=1&G=255&B=255&R=255&W=255&TT=1000" , false );
} else {
 handleSet(nullptr, "win&SB=255&FX=0&SM=1&SS=0&R=255&TT=1000" , false );
 handleSet(nullptr, "win&SB=255&FX=0&SM=0&SS=1&G=255&B=255&R=255&W=255&TT=1000" , false );
 }
}

public:
  void setup()
  {
    // set pinmode
    #ifdef PRO_VERSION
    pinMode(LIGHT_BAR_R_PIN, INPUT);
    pinMode(LIGHT_BAR_G_PIN, INPUT);
    pinMode(LIGHT_BAR_B_PIN, INPUT);

    pinMode(BATTERY_VOLTAGE_PIN, INPUT);
    pinMode(MOTOR_SPEED_PIN, INPUT);
    #endif
    pinMode(FRONT_LIGHT_W_PIN, INPUT);
    pinMode(FRONT_LIGHT_R_PIN, INPUT);


/*
///////////////////////////////////turn AP on copied from wled>wled.cpp
  escapedMac = WiFi.macAddress();
  escapedMac.replace(":", "");
  escapedMac.toLowerCase();
  if (!apSSID[0])
    strcpy_P(apSSID, PSTR(("Andon-" + escapedMac).c_str ()));
  DEBUG_PRINT(F("Opening access point "));
  DEBUG_PRINTLN(apSSID);
  WiFi.softAPConfig(IPAddress(4, 3, 2, 1), IPAddress(4, 3, 2, 1), IPAddress(255, 255, 255, 0));
  WiFi.softAP(apSSID, apPass, apChannel);


//////////////////////////////////////////////////start wifi and portal
    DEBUG_PRINTLN(F("Init AP interfaces"));
    server.begin();
    if (udpPort > 0 && udpPort != ntpLocalPort) {
      udpConnected = notifierUdp.begin(udpPort);
    }
    if (udpRgbPort > 0 && udpRgbPort != ntpLocalPort && udpRgbPort != udpPort) {
      udpRgbConnected = rgbUdp.begin(udpRgbPort);
    }
    if (udpPort2 > 0 && udpPort2 != ntpLocalPort && udpPort2 != udpPort && udpPort2 != udpRgbPort) {
      udp2Connected = notifier2Udp.begin(udpPort2);
    }
    e131.begin(false, e131Port, e131Universe, E131_MAX_UNIVERSE_COUNT);

    dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
    dnsServer.start(53, "*", WiFi.softAPIP());
    apActive = true;
/////////////////////////////////end of turn AP on

*/

    if (boot_preset_time != 0){ // skip if boot_preset_time set to 0
    start_milisec = millis();
    applyPreset(boot_preset);// start up animation plays for 3 sec or so (still need to implement switching back)
    }

  }

  void loop()
  {
    //if (strip.isUpdating())
    //  return;
   



   #ifndef TEST_MODE // test mode skip get direction from front light becuase we dont have the hardware on test esp32
   get_front_light();
   if (app_lights_on == (false)){ //turns lights off if in app lights are off
   handleSet(nullptr, "win&T=0" , false );// turn all off
   return; // skip rest of loop becuase we dont want to change lights besides forward/back
   }
   #endif

    
    //if (skip_loop_wifi_clients_on){
    
   // float wifi_on_time_extend;

    wifi_sta_list_t stationList;  //skip looping code if user is on wifi so we dont change stuff while they are editing
    esp_wifi_ap_get_sta_list(&stationList);
    int client_numb = stationList.num;
    if ( client_numb != 0 ){
   // wifi_on_time_extend = millis() + 15000;//add 15 sec to when the wifi will turn off if client is connect to wifi
    return; 
    }// else { /////////// usermod pertenent looping code starts here
    
   //    if (((millis()) - wifi_on_time_extend) > (wifi_on_time * 1000)){
   //    WiFi.mode(WIFI_OFF);    // Switch WiFi off
   //    apActive = false;
   //    skip_loop_wifi_clients_on = false;
   // }
   //}
   //}


   if ((stock) == true){
   emulate_stock();
   return; // skip rest of loop becuase we dont want to change lights besides forward/back
   }

             
     if (((millis()) - start_milisec) < (boot_preset_time * 1000)){
      return;  // returns loop if boot animation hasnt finished playing
     }







  




   #ifndef PRO_VERSION //if not pro version
   applyPreset(choosen_preset); //sets lights to the choosen preset for standard version
   #endif

   #ifdef PRO_VERSION  //rest of loop is pro only features
   GET_BATT_LEVEL();
   GET_DUTYCYCLE();

   if ((Status_bar) == true){
   MIMIC_ERROR_CODES(); // sets front and back to same color as status bar when error and disengaged
   }

   if ((low_bat_percent < battery_percent) && (low_bat_percent != 0)){  //if user set low battery %   less than    actual battery %
   set_motor_duty_preset();
   } else {
    applyPreset(low_bat_preset);
   }

   if ((MODEL == 0) || (MODEL == 1)){ // if GT or Pint both of witch have status bars on foot pad
   GET_LIGHT_BAR();
   }

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

      JsonArray lights = user.createNestedArray("App Lights on");  //left side thing
      lights.add(app_lights_on);                               //right side variable
      lights.add(F(" Lights"));                      //right side thing
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
    top[FPSTR(_choosen_preset)] = choosen_preset;  //int input
    #endif
    top[FPSTR(_boot_preset)] = boot_preset;  //int input
    top[FPSTR(_boot_preset_time)] = boot_preset_time;  //int input

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
    choosen_preset   = top[FPSTR(_choosen_preset)] | choosen_preset;     //int input
    #endif
    boot_preset   = top[FPSTR(_boot_preset)] | boot_preset;     //int input
    boot_preset_time   = top[FPSTR(_boot_preset_time)] | boot_preset_time;     //int input
    stock            = !(top[FPSTR(_stock)] | !stock);       //bool
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
const char UsermodAndon::_choosen_preset[] PROGMEM = "Preset animation to use while riding";
#endif
const char UsermodAndon::_boot_preset[] PROGMEM = "Preset animation to use as boot animation";
const char UsermodAndon::_boot_preset_time[] PROGMEM = "How long is boot preset in sec (0 to disable)";