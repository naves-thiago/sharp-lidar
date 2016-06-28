#include <SharpIR.h>

#define IR_PIN     A7
#define MODEL      1080
#define STEP_PIN   3
#define DIR_PIN    4
#define STEP_MAX   1500
#define INTERVAL   5
#define STEP_DEG   3.0

SharpIR sharp(IR_PIN, 25, 1, MODEL);

// ir: the pin where your sensor is attached
// 25: the number of readings the library will make before calculating a mean distance
// 93: the difference between two consecutive measurements to be taken as valid
// model: an int that determines your sensor:  1080 for GP2Y0A21Y
//                                            20150 for GP2Y0A02Y
//                                            (working distance range according to the datasheets)
void setup(){
  Serial.begin(9600);
  pinMode(IR_PIN, INPUT);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
}

unsigned int steps = 0;
int dir = 0;

void loop(){

  if ((steps == STEP_MAX) || (steps == 0)) {
    dir = !dir;
    digitalWrite(DIR_PIN, dir);
  }

  steps += dir ? 1 : -1;

  digitalWrite(STEP_PIN, 1);
  delay(1);
  digitalWrite(STEP_PIN, 0);
  delay(INTERVAL-1);

  int dis=sharp.distance();
  Serial.print(360 * (steps / (float)STEP_MAX));
  Serial.print(" ");
  if (dis > 100)
    dis = 100;

  Serial.println(dis < 0 ? 0 : dis);
}
  

