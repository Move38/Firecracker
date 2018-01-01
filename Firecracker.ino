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
bool didReceivedSparkFrom[] = {0, 0, 0, 0, 0, 0};
bool hasNeighbor[] = {0, 0, 0, 0, 0, 0};
uint32_t firecrackerTime_ms = 0;
uint16_t firecrackerDuration_ms = 400;
uint16_t sendSparkDelay_ms = 100;
uint16_t sendSparkDuration_ms = firecrackerDuration_ms - sendSparkDelay_ms;

byte numColors = 6;
byte curColor = 0;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  uint32_t now = millis();

  // if button single pressed, white firecracker
  if (buttonSingleClicked()) {
    resetReceived();
    firecrackerTime_ms = now;
    curColor = 0;
  }

   // if button double pressed, colorful firecracker
  if (buttonDoubleClicked()) {
    resetReceived();
    firecrackerTime_ms = now;
    curColor = 1 + (millis() % numColors);
  }


  if (shouldSendSpark) {

    FOREACH_FACE(f) {
      if (hasNeighbor[f]) {
        if (!didReceivedSparkFrom[f]) {
          byte face = 1 << f;
          irSendData(2 + curColor, face);
        }
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

      // only if not ignited already
      if (now - firecrackerTime_ms > firecrackerDuration_ms) {

        if (neighbor >= 2 && neighbor <= 1 + 2 + numColors) { // not sure why the top needs to be constrained, but it does
          // TODO: ask Josh about constraining the byte on the top?
          curColor = neighbor - 2;  // set the color to the one just received
          didReceivedSparkFrom[f] = true;
          firecrackerTime_ms = now;
        }
        
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
    if(curColor == 0) {
      setFaceColor(face, WHITE);
    }
    else {
      byte hue = (curColor-1) * 255 / numColors;
      setFaceColor(face, makeColorHSB(hue, 196, 255));
    }

    if ( now - firecrackerTime_ms > sendSparkDelay_ms &&
         now - firecrackerTime_ms < sendSparkDelay_ms + sendSparkDuration_ms) {

      shouldSendSpark = true;
    }
  }
  else {
    shouldSendSpark = false;
    resetReceived();
    showOnlyNeighbors();
  }
}

void showOnlyNeighbors() {
  FOREACH_FACE(f) {
    // acknowledge neighbors presence w/ red glow
    if (hasNeighbor[f]) {
      setFaceColor(f, makeColorHSB(25, 255, 128));
    }
    else {
      setFaceColor(f, OFF);
    }
  }
}

void resetReceived() {
  FOREACH_FACE(f) {
    didReceivedSparkFrom[f] = 0;
  }
}

