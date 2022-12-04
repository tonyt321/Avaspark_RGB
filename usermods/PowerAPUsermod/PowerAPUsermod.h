#pragma once
#include "wled.h"

class PowerAPUsermod : public Usermod {
  private:
    unsigned long lastTime = 0;
    String fname = F("/boot.dat");

  public:
    void setup() {
      if (WLED_FS.exists(fname)) {
        File fl = WLED_FS.open(fname,"w");
        fl.seek(0);
        char data = fl.read();
        if (data == '0') {
          fl.seek(0);
          fl.write('1');
        } else if (data == '1') {

          //WLED::instance().initAP(true); //double reset code here
          handleSet(nullptr, "win&SB=255&FX=98&SM=1&SS=1&G=255&R2=255&IX=50" , false );
        }
        fl.close();
      } else {
        File fl = WLED_FS.open(fname,"w");
        fl.write('0');
        fl.close();
      }
    }

    void loop() {
      if (millis() < 10000 && millis() - lastTime > 5000) {
        lastTime = millis();
        if (WLED_FS.exists(fname)) {
          WLED_FS.remove(fname);
        }
     }
    }

    uint16_t getId() { return USERMOD_ID_UNSPECIFIED; }
};