# 1 "/tmp/tmpts934sqd"
#include <Arduino.h>
# 1 "/workspace/andon/wled00/wled00.ino"
# 13 "/workspace/andon/wled00/wled00.ino"
#include "wled.h"
void setup();
void loop();
#line 15 "/workspace/andon/wled00/wled00.ino"
void setup() {
  WLED::instance().setup();
}

void loop() {
  WLED::instance().loop();
}