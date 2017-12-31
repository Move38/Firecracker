/*
    Happy New Years - 2018

    Firecracker

    Rules:
    If button pressed...
    explode with firecracker like flashing of lights
    when done, pick one of my neighbors to send the spark to
    if receive spark, explode like firecracker
    remember who I received it from
    when done, pick one of my other neighbors to send the spark to
    if no other neighbors available, firecracker is extinguished

*/

#include "blinklib.h"

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

  // let neighbors know I am here
  irBroadcastData(1);

  // read my neighbors
  FOREACH_FACE(f) {
    if(irIsReadyOnFace(f)){
      irGetData(f);
      setFaceColor(f, RED);
    }
    else {
      setFaceColor(f, OFF);
    }
  }

}
