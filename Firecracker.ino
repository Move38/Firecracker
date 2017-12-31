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

bool shouldSendSpark = false;
int receivedSparkFrom = -1;
bool hasNeighbor[] = {0, 0, 0, 0, 0, 0};
uint32_t firecrackerTime_ms = 0;
uint16_t firecrackerDuration_ms = 1000;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  uint32_t now = millis();

  // if button pressed, firecracker
  if (buttonSingleClicked()) {
    receivedSparkFrom = -1;
    firecrackerTime_ms = now;
  }

  if (shouldSendSpark) {
    //    byte neighborFace;
    //    bool willSendSpark = false;
    //
    //    FOREACH_FACE(f) {
    //      if (f != receivedSparkFrom && hasNeighbor[f]) {
    //        willSendSpark = true;
    //      }
    //    }
    //
    //    do {
    //      neighborFace = millis() % 6; // pick random face
    //    }
    //    while (neighborFace == receivedSparkFrom || !hasNeighbor[neighborFace]);
    //
    //    if (willSendSpark) {
    //      byte face = 1 << neighborFace;
    //      irSendData(2, face);
    //    }

    FOREACH_FACE(f) {
      if (hasNeighbor[f]) {
        byte face = 1 << f;
        irSendData(2, face);
      }
    }
  }
  else {
    // let neighbors know I am here
    irBroadcastData(1);
  }

  // read my neighbors
  FOREACH_FACE(f) {
    if (irIsReadyOnFace(f)) {
      byte neighbor = irGetData(f);

      // acknowledge neighbors presence
      hasNeighbor[f] = true;

      if (neighbor == 2) {
        receivedSparkFrom = f;
        firecrackerTime_ms = now;
      }
    }
    else {
      // acknowledge neighbors absense
      hasNeighbor[f] = false;
    }
  }

  // display firecracker
  if (now - firecrackerTime_ms < firecrackerDuration_ms) {

    byte face = millis() % 6; // pick random face
    showOnlyNeighbors();
    setFaceColor(face, WHITE);
  }
  else if (now - firecrackerTime_ms < firecrackerDuration_ms + 100) {
    shouldSendSpark = true;
  }
  else {
    shouldSendSpark = false;
    receivedSparkFrom = -1;
    showOnlyNeighbors();
  }
}

void showOnlyNeighbors() {
  FOREACH_FACE(f) {
    // acknowledge neighbors presence w/ red glow
    if (hasNeighbor[f]) {
      setFaceColor(f, makeColorHSB(25,255,128));
    }
    else {
      setFaceColor(f, OFF);
    }
  }
}

