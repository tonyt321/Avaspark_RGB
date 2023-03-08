#pragma once



#include "VescUart.cpp"
#include "VescUart.h"
#include "buffer.cpp"
#include "buffer.h"
#include "crc.cpp"
#include "crc.h"
#include "datatypes.h"
#include "wled.h"

/** Initiate VescUart class */
VescUart UART;


class Usermodvesc : public Usermod
{
public:




//Change these values to suit your setup
int Poles = 30;                  //Usually 46 for hub motor
float WheelDia = 0.28;           //Wheel diameter in m
float GearReduction = 1;         //reduction ratio. 1 for direct drive. Otherwise motor pulley diameter / Wheel pulley diameter.
float BatteryCells = 15;         //Number of cells in Battery


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



  // strings to reduce flash memory usage (used more than twice)
  static const char _name[];
  
 // static const char _backwards_preset[];
 // static const char _dim_backwards_preset[];
 // static const char _dim_forwards_preset[];
  



public:
  void setup()
  {

  /** Setup UART port On TTGO Display, you have to assign the pins. 25(Tx) 26(Rx) in this case */
  //** Default VESC brate is 115200, you can change it to any other value. */
  Serial2.begin(115200, SERIAL_8N1, 32, 33);
  /** Define which ports to use as UART */
  UART.setSerialPort(&Serial2);

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

 // /*
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
      }
  } // end of main loop


      void readFromJsonState(JsonObject& root)  //serial json iputs go in here
    {
    //  tpmsp = root["tpmsp"] | tpmsp; //TPMS tire pressure
   //   tpmsb = root["tpmsb"] | tpmsb; //TPMS battery
    }

  void addToJsonInfo(JsonObject &root)  //serial json outputs go here
  {

    JsonObject hidden = root[F("h")];
    if (hidden.isNull())
      hidden = root.createNestedObject(F("h"));



    JsonObject user = root[F("u ")];
    if (user.isNull())
      user = root.createNestedObject(F("u"));

         JsonArray vesc0;
         vesc0 = user.createNestedArray("rpm");  //left side thing
         vesc0.add(rpm);

         JsonArray vesc1;
         vesc1 = user.createNestedArray("motortemp");  //left side thing
         vesc1.add(motortemp);



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
 
//   top[FPSTR(_stock_preset)] = stock_preset;  //int input
  //  top[FPSTR(_alt_mode_user)] = alt_mode_user;



    DEBUG_PRINTLN(F("VESC config saved."));
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
    
   // Status_bar       = !(top[FPSTR(_Status_bar)] | !Status_bar);    //bool
   // battery_bar      = !(top[FPSTR(_battery_bar)] | !battery_bar);  //bool
   // low_bat_percent   = top[FPSTR(_low_bat_percent)] | low_bat_percent;  //int input


    DEBUG_PRINT(FPSTR(_name));
    DEBUG_PRINTLN(F(" config (re)loaded."));

    // use "return !top["newestParameter"].isNull();" when updating Usermod with new features
    return true;
  }
};

// strings to reduce flash memory usage (used more than twice)
//                           _veriable         "what it says on the webpage"
const char Usermodvesc::_name[] PROGMEM = "Vesc settings";
//const char UsermodAndon::_stock_preset[] PROGMEM = "Stock lighting override preset";
//const char UsermodAndon::_Status_bar[] PROGMEM = "Mirror Status bar error";
//const char UsermodAndon::_battery_bar[] PROGMEM = "Display battery on dismount";
//const char UsermodAndon::_low_bat_percent[] PROGMEM = "Battery percent to change preset (0 to disable) overrides duty cycle preset";
//const char UsermodAndon::_low_bat_preset[] PROGMEM = "Low battery preset animation";
