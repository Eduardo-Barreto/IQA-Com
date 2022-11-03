#include "config/stepperMotor.h"
#include "config/servo.h"
#include "config/button.h"

stepperMotor motor(
    5,    // Step pin
    4,    // Direction pin
    6,    // Enable pin
    400,  // Resolution
    4490, // Minimum velocity
    256   // Maximum velocity
);
Servo blocker(11, 90);

bool rightPart = false;
unsigned long timeToClose = 0;
bool closed = true;
int dump = 0;

void setup()
{
    // put your setup code here, to run once:
    motor.on();
    Serial.begin(9600);
}

void loop()
{
    // put your main code here, to run repeatedly:
    motor.move(75);

    rightPart = Serial.available();
    dump = Serial.read();

    if (rightPart)
    {
        blocker.write(0);
        timeToClose = millis() + 3000;
        closed = false;
    }

    if (!closed && millis() >= timeToClose)
    {
        blocker.write(90);
        closed = true;
    }
}
