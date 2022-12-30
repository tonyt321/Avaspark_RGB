#pragma once

#include "ADXL345.h"
#include "ADXL345.cpp"
#include "wled.h"
#include <Wire.h>



class UsermodAndon : public Usermod
{
private:

ADXL345 accelerometer;

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
  int8_t choosen_preset = 1;
  #endif

  
unsigned long usermod_loop_time; // for tracking how much time has past for each loop
unsigned long usermod_loop_time_last; // for tracking how much time has past for each loop
unsigned long usermod_loop; // for tracking how much time has past for each loop

  int8_t boot_preset = 1;  //preset played as a boot animation
  unsigned long boot_preset_time = 3; // boot animation length in sec
  unsigned long start_milisec; // for tracking how much time has past for boot animation preset


  bool FRONT_LIGHT_R = false;
  int FRONT_LIGHT_R_ANALOG;
  bool FRONT_LIGHT_W = false;
  int FRONT_LIGHT_W_ANALOG;
  
  // flag set at startup
  int client_numb;
  bool skip_loop_wifi_clients_on = true;
  bool forward = true; //on startup assume forware movment
  bool app_lights_on;  // are the lights on in the app?

  bool app_lights_on_last; // last check value of lights on
  int blink_app_lights = 0;
  unsigned long blink_app_lights_timing;


  bool stock = true;

  unsigned long free_fall_duration = 5; // inches fallen
  unsigned long free_fall_threshold = 35; // Recommended 10 s
  unsigned long free_fall_preset_time = 3; // animation length in sec
  unsigned long free_fall_preset = 1; // preset after free fall
  unsigned long free_fall_milisec; // for tracking how much time has past for free_fall animation preset

  float rawx;
  float rawy;
  float rawz;

  float normx;
  float normy;
  float normz;

  float roll;
  float pitch;
  float froll;
  float fpitch;

  float filteredx;
  float filteredy;
  float filteredz;

  bool imu_activity;
  bool imu_free_fall = false;
  
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
  static const char _free_fall_preset_time[];
  static const char _free_fall_duration[];
  static const char _free_fall_threshold[];
  static const char _free_fall_preset[];




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



  void get_imu_data(){

      // Read activities
  Activites activ = accelerometer.readActivites();

  // Read raw values
  //Vector raw = accelerometer.readRaw();

      // Read normalized values
  Vector norm = accelerometer.readNormalize();

  // Low Pass Filter to smooth out data. 0.1 - 0.9
  Vector filtered = accelerometer.lowPassFilter(norm, 0.15);



  // Calculate Pitch & Roll
  //pitch = -(atan2(norm.XAxis, sqrt(norm.YAxis*norm.YAxis + norm.ZAxis*norm.ZAxis))*180.0)/M_PI;
  //roll  = (atan2(norm.YAxis, norm.ZAxis)*180.0)/M_PI;

  // Calculate Pitch & Roll (Low Pass Filter)
  //fpitch = -(atan2(filtered.XAxis, sqrt(filtered.YAxis*filtered.YAxis + filtered.ZAxis*filtered.ZAxis))*180.0)/M_PI;
  //froll  = (atan2(filtered.YAxis, filtered.ZAxis)*180.0)/M_PI;


 // Serial.print(raw.ZAxis); //  Serial.print(norm.ZAxis);


  //rawx = (raw.XAxis);
  //rawy = (raw.YAxis);
  //rawz = (raw.ZAxis);

  //normx = (norm.XAxis);
  //normy = (norm.YAxis);
  //normz = (norm.ZAxis);

filteredx = filtered.XAxis;
filteredy = filtered.YAxis;
filteredz = filtered.ZAxis;
 //delay(5);
  if (activ.isFreeFall)
  {
    //Serial.println("Free Fall Detected!");
    imu_free_fall = true;
    free_fall_milisec = millis();
  }

  if (activ.isActivity)
  {
    imu_activity = true;
    //Serial.println("Activity Detected");
  }

  if (activ.isInactivity)
  {
    imu_free_fall = false;
    imu_activity = false;
    //Serial.println("Inactivity Detected");
  }
/*
    if (activ.isDoubleTap)
  {
    Serial.println("Double Tap Detected");
  } else if (activ.isTap)
  {
    Serial.println("Tap Detected");
  }
*/

  }


  void get_front_light()
  {

    FRONT_LIGHT_W_ANALOG = analogRead(FRONT_LIGHT_W_PIN);
    if (FRONT_LIGHT_W_ANALOG > 1000){FRONT_LIGHT_W = false;}else{FRONT_LIGHT_W = true;}

    FRONT_LIGHT_R_ANALOG = analogRead(FRONT_LIGHT_R_PIN);
    if (FRONT_LIGHT_R_ANALOG > 1000){
      FRONT_LIGHT_R = false;
      app_lights_on = false;
      }else{
        FRONT_LIGHT_R = true;
        app_lights_on = true;
        }

      if (FRONT_LIGHT_W == FRONT_LIGHT_R){ 
        forward = true;
      } else {
        forward = false;
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
 handleSet(nullptr, "win&S=0&S2=13&SS=0&SM=0&SV=2" , false );  // select seg 0 & set main seg 0 & de select other seg
 handleSet(nullptr, "win&SB=255&FX=0&G=0&B=0&R=255&W=0&TT=1000&T=1&S=0&S2=13" , false );
 
 handleSet(nullptr, "win&S=13&S2=26&SS=1&SM=1&SV=2" , false );
 handleSet(nullptr, "win&SB=255&FX=0&G=255&B=255&R=255&W=255&TT=3000&T=1&S=13&S2=26" , false );

} else {
 handleSet(nullptr, "win&S=0&S2=13&SS=0&SM=0&SV=2" , false );  // select seg 0 & set main seg 0 & de select other seg
 handleSet(nullptr, "win&SB=255&FX=0&G=255&B=255&R=255&W=255&TT=1000&T=1&S=0&S2=13" , false );
 
 handleSet(nullptr, "win&S=13&S2=26&SS=1&SM=1&SV=2" , false );
 handleSet(nullptr, "win&SB=255&FX=0&G=0&B=0&R=255&W=0&TT=1000&T=1&S=13&S2=26" , false );
  }
 }
}

public:
  void setup()
  {
    // set pin modes

    pinMode(FRONT_LIGHT_W_PIN, INPUT);
    pinMode(FRONT_LIGHT_R_PIN, INPUT);
    
    #ifdef PRO_VERSION
    pinMode(LIGHT_BAR_R_PIN, INPUT);
    pinMode(LIGHT_BAR_G_PIN, INPUT);
    pinMode(LIGHT_BAR_B_PIN, INPUT);

    pinMode(BATTERY_VOLTAGE_PIN, INPUT);
    pinMode(MOTOR_SPEED_PIN, INPUT);
    #endif


 if (!accelerometer.begin())
  {
    //Serial.println("Could not find a valid ADXL345 sensor, check wiring!");
    delay(5);
  }

  ///////////////////////////////////////////////////// Set measurement range
  // +/-  2G: ADXL345_RANGE_2G
  // +/-  4G: ADXL345_RANGE_4G
  // +/-  8G: ADXL345_RANGE_8G
  // +/- 16G: ADXL345_RANGE_16G
  accelerometer.setRange(ADXL345_RANGE_16G);

    //////////////////////////////////////////////////// Set measurement rate
    // ADXL345_DATARATE_3200HZ
    // ADXL345_DATARATE_1600HZ
    // ADXL345_DATARATE_800HZ 
    // ADXL345_DATARATE_400HZ   
    // ADXL345_DATARATE_200HZ  
    // ADXL345_DATARATE_100HZ  
    // ADXL345_DATARATE_50HZ   
    // ADXL345_DATARATE_25HZ    
    // ADXL345_DATARATE_12_5HZ  
    // ADXL345_DATARATE_6_25HZ  
    // ADXL345_DATARATE_3_13HZ  
    // ADXL345_DATARATE_1_56HZ 
    // ADXL345_DATARATE_0_78HZ 
    // ADXL345_DATARATE_0_39HZ  
    // ADXL345_DATARATE_0_20HZ 
    // ADXL345_DATARATE_0_10HZ
    accelerometer.setDataRate(ADXL345_DATARATE_25HZ);


    /////////////////////////////////////////////////////////////////////////// Set Free Fall detection
  accelerometer.setFreeFallThreshold(0.3); // Recommended 0.3 -0.6 g
  accelerometer.setFreeFallDuration(sqrt(2 * (free_fall_duration * 0.0254)/ 9.81));  // Recommended 0.1 s

  // Select INT 1 for get activities
  //accelerometer.useInterrupt(ADXL345_INT1);


  ////////////////////////////////////////////////////////////////////////////// Set inactivity detection only on X,Y,Z-Axis

      // Values for Inactivity detection
  accelerometer.setInactivityThreshold(2.0);  // Recommended 2 g
  accelerometer.setTimeInactivity(10);         // Recommended 5 s
  
  accelerometer.setInactivityXYZ(1);       // Check inactivity on X,Y,Z-Axis
  // or
  // accelerometer.setInactivityX(1);      // Check inactivity on X_Axis
  // accelerometer.setInactivityY(1);      // Check inactivity on Y-Axis
  // accelerometer.setInactivityZ(1);      // Check inactivity on Z-Axis
  
  ////////////////////////////////////////////////////////////////////////////// Set activity detection only on X,Y,Z-Axis
    // Values for Activity detection
  accelerometer.setActivityThreshold(8);    // Recommended 2 g
    
  accelerometer.setActivityXYZ(1);         // Check activity on X,Y,Z-Axis
  // or
  // accelerometer.setActivityX(1);        // Check activity on X_Axis
  // accelerometer.setActivityY(1);        // Check activity on Y-Axis
  // accelerometer.setActivityZ(1);        // Check activity on Z-Axis
  
  // Select INT 1 for get activities
  //accelerometer.useInterrupt(ADXL345_INT1);

  ///////////////////////////////////////////////////////////////////////////// Set tap detection on Z-Axis

  //accelerometer.setTapDetectionXYZ(1);  // Check tap on X,Y,Z-Axis
  //or
  //accelerometer.setTapDetectionX(0);       // Don't check tap on X-Axis
  //accelerometer.setTapDetectionY(0);       // Don't check tap on Y-Axis
  //accelerometer.setTapDetectionZ(1);       // Check tap on Z-Axis
  

  //accelerometer.setTapThreshold(2.5);      // Recommended 2.5 g
  //accelerometer.setTapDuration(0.02);      // Recommended 0.02 s
  //accelerometer.setDoubleTapLatency(0.10); // Recommended 0.10 s
  //accelerometer.setDoubleTapWindow(0.30);  // Recommended 0.30 s

  // Select INT 1 for get activities
  //accelerometer.useInterrupt(ADXL345_INT1);




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

 
  #ifdef TEST_MODE
  app_lights_on = true;  // set as if lights are detected as always on in test mode
  #endif

   #ifndef TEST_MODE // test mode skip get front light becuase we dont have the hardware on test esp32
   get_front_light();
   if (FRONT_LIGHT_R == (false)){ //turns lights off if in app lights are off
   handleSet(nullptr, "win&S=0&S2=13&SS=0&SM=0&SV=2" , false );  // select seg 0 & set main seg 0 & de select other seg
   handleSet(nullptr, "win&T=0&SB=0&S=0&S2=13" , false );// turn all off

   handleSet(nullptr, "win&S=13&S2=26&SS=1&SM=1&SV=2" , false );
   handleSet(nullptr, "win&T=0&SB=0&S=13&S2=26" , false );// turn all off
   return; // skip rest of loop becuase we dont want to change lights besides forward/back
   }else{
    handleSet(nullptr, "win&T=1&SB=255" , false );// turn all on
   }
   #endif
   

   if ((stock) == true){ // if emulate stock is off use boot up preset

    if (boot_preset_time != 0){ // skip if boot_preset_time set to 0
    start_milisec = millis();
    applyPreset(boot_preset);// start up animation plays for 3 sec or so (still need to implement switching back)
    }

   }

  }// end of start up 

  void loop()
  {

    if (strip.isUpdating())
      return;
    

    


    
 

    wifi_sta_list_t stationList;  //skip looping code if user is on wifi so we dont change stuff while they are editing
    esp_wifi_ap_get_sta_list(&stationList);
    client_numb = stationList.num;
    if ( client_numb != 0 ){
    return; 
    }



    usermod_loop = ((millis()) - usermod_loop_time_last);
    usermod_loop_time_last = millis();

   #ifndef TEST_MODE // test mode skip get direction from front light becuase we dont have the hardware on test esp32
   get_front_light();
   if (app_lights_on == (false)){ //turns lights off if in app lights are off

   handleSet(nullptr, "win&S=0&S2=13&SS=0&SM=0&SV=2" , false );  // select seg 0 & set main seg 0 & de select other seg
   handleSet(nullptr, "win&T=0&SB=0&S=0&S2=13" , false );// turn all off

   handleSet(nullptr, "win&S=13&S2=26&SS=1&SM=1&SV=2" , false );
   handleSet(nullptr, "win&T=0&SB=0&S=13&S2=26" , false );// turn all off


   return; // skip rest of loop
   }else{
    handleSet(nullptr, "win&S=0&S2=13&SS=0&SM=0&SV=2" , false );
    handleSet(nullptr, "win&S=13&S2=26&SS=1&SM=1&SV=2" , false );
    handleSet(nullptr, "win&T=1&SB=255" , false );// turn all on
    }
   #endif


   if ((stock) == false){
   emulate_stock();
   return; // skip rest of loop becuase we dont want to change lights besides forward/back
   }

             
     if (((millis()) - start_milisec) < (boot_preset_time * 1000)){
      return;  // returns loop if boot animation hasnt finished playing
     }


//flash lights to turn on wifi
/*
   if ((blink_app_lights >= 3) || (filteredz > 0)){ //if upside down or lights in Onewheel app are flashed on off 3 times
    // turn wifi on
   }else{
    if ( client_numb == 0 ){ // if no one is on the wifi
    // turn wifi off
    }
   }
*/

  


/////////////////////////////////////// imu things



  Vector norm = accelerometer.readNormalize();

  // Read activities
  Activites activ = accelerometer.readActivites();




  if (activ.isActivity)
  {
//applyPreset(3);
  }

  if (activ.isInactivity)
  {
applyPreset(4);
  }

    if (activ.isFreeFall)
  {
    //Serial.println("Free Fall Detected!");
applyPreset(2);
  }

return;
  //  get_imu_data();

/////////////////////////////get this function working
/*
    if ((free_fall_preset_time != 0) && imu_free_fall == true){ // skip if free_fall_preset_time set to 0
    applyPreset(free_fall_preset);
    if ((free_fall_milisec + (free_fall_preset_time * 1000)) < millis()){
      imu_free_fall = false;
    }
    return;  // returns loop if boot animation hasnt finished playing
    }
    

/////////////////////////////////////// end of imu things
if (imu_free_fall == true){
applyPreset(free_fall_preset); //sets lights to the choosen preset for standard version
return;
}

if (imu_activity == true){
applyPreset(2); //sets lights to the choosen preset for standard version
return;
}

if (imu_activity == false){
applyPreset(3); 
return;
}
*/



     #ifndef PRO_VERSION //if not pro version
  // applyPreset(choosen_preset); //sets lights to the choosen preset for standard version
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

                  JsonArray battery3 = user.createNestedArray("red bool");  //left side thing
      battery3.add(FRONT_LIGHT_R);                               //right side variable
      battery3.add(F(" RED GPIO read"));                      //right side thing

            JsonArray battery4 = user.createNestedArray("white bool");  //left side thing
      battery4.add(FRONT_LIGHT_W);                               //right side variable
      battery4.add(F(" WHITE GPIO read"));                      //right side thing

                  JsonArray battery5 = user.createNestedArray("loop time");  //left side thing
      battery5.add(usermod_loop);                               //right side variable
      battery5.add(F(" ms"));                      //right side thing
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
    top[FPSTR(_free_fall_duration)] = free_fall_duration;  //int input
    top[FPSTR(_free_fall_threshold)] = free_fall_threshold;  //int input
    top[FPSTR(_free_fall_preset_time)] = free_fall_preset_time;  //int input
    top[FPSTR(_free_fall_preset)] = free_fall_preset;  //int input
    

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
    free_fall_preset   = top[FPSTR(_free_fall_preset)] | free_fall_preset;     //int input
    free_fall_preset_time   = top[FPSTR(_free_fall_preset_time)] | free_fall_preset_time;     //int input
    free_fall_threshold   = top[FPSTR(_free_fall_threshold)] | free_fall_threshold;     //int input
    free_fall_duration   = top[FPSTR(_free_fall_duration)] | free_fall_duration;     //int input
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

const char UsermodAndon::_free_fall_preset[] PROGMEM = "Preset to display after a free fall";
const char UsermodAndon::_free_fall_preset_time[] PROGMEM = "How long is free fall preset in sec (0 to disable)";

const char UsermodAndon::_free_fall_threshold[] PROGMEM = "how sensitive is a free fall detection 30 -60 g";
const char UsermodAndon::_free_fall_duration[] PROGMEM = "inches for free fall detection";