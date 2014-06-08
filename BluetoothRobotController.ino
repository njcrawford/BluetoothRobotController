/*************************************************************************
* File Name          : TestSlaveBluetoothBySoftSerial.ino
* Author             : Steve
* Updated            : Steve
* Version            : V1.0.0
* Date               : 12/12/2012
* Description        : Example for Makeblock Electronic modules of Me -  
                       Bluetooth. The module can only be connected to the 
                       port 3, 4, 5, 6, 7 of Me - Base Shield.
* License            : CC-BY-SA 3.0
* Copyright (C) 2011 Hulu Robot Technology Co., Ltd. All right reserved.
* http://www.makeblock.cc/
**************************************************************************/
// Bluetooth includes
#include <Me_BaseShield.h>
#include <SoftwareSerial.h>
#include <Me_Bluetooth.h>

// Motor includes
#include <Me_BaseShieldMotorDriver.h>
Me_BaseShieldMotorDriver baseShieldMotorDriver;

/*
Blue module can only be connected to port 3, 4, 5, 6, 7 of base shield.
When connected to port 5, Serial.print function can not be used, and 
the code can be uploaded to the Arduino only when there's no module
connected to port 5 physicaly.
*/
Me_Bluetooth bluetooth(4);

int motorSpeed = 254;

// Store previous input character
char previousChar = '\0';

// Variables for control decay
bool useControlDecay = false;
int motor1CurrentSpeed = 0;
int motor1GoalSpeed = 0;
int motor2CurrentSpeed = 0;
int motor2GoalSpeed = 0;
int delayCounter = 0;

void setup()
{
    Serial.begin(9600);
    bluetooth.begin(9600);
    baseShieldMotorDriver.begin();
    Serial.println("Application Start!");
}

void loop()
{
    char inDat;
    char outDat;
    if(bluetooth.available())
    {
        inDat = bluetooth.read();
        Serial.println(inDat);
    }
    /*if(Serial.available())
    {
        outDat = Serial.read();
        bluetooth.write(outDat);
    }*/
    
    if(inDat == ';')
    {
      switch(previousChar)
      {
        case 'W':
          // Forward
          motor1GoalSpeed = -motorSpeed;
          motor2GoalSpeed = -motorSpeed;
          break;
        case 'X':
          // Backward
          motor1GoalSpeed = motorSpeed;
          motor2GoalSpeed = motorSpeed;
          break;
        case 'A':
          // Left
          motor1GoalSpeed = -motorSpeed;
          //motor2GoalSpeed = motorSpeed;
          motor2GoalSpeed = 0;
          break;
        case 'D':
          // Right
          //motor1GoalSpeed = motorSpeed;
          motor1GoalSpeed = 0;
          motor2GoalSpeed = -motorSpeed;
          break;
        case 'S':
          // Stop
          motor1GoalSpeed = 0;
          motor2GoalSpeed = 0;
          baseShieldMotorDriver.stopMotors();
          break;
      }
    }
    
    delayCounter++;
    if(delayCounter == 75)
    {
      delayCounter = 0;
      // Speed decay for motor 1
      if(motor1CurrentSpeed != motor1GoalSpeed)
      {
        if(motor1GoalSpeed > motor1CurrentSpeed)
        {
          motor1CurrentSpeed++;
        }
        else
        {
          motor1CurrentSpeed--;
        }
        
        if(motor1CurrentSpeed == 0)
        {
          baseShieldMotorDriver.stopMotor1();
        }
        else
        {
          baseShieldMotorDriver.runMotor1(motor1CurrentSpeed);
        }
      }
      
      // Speed decay for motor 2
      if(motor2CurrentSpeed != motor2GoalSpeed)
      {
        if(motor2GoalSpeed > motor2CurrentSpeed)
        {
          motor2CurrentSpeed++;
        }
        else
        {
          motor2CurrentSpeed--;
        }
        
        if(motor2CurrentSpeed == 0)
        {
          baseShieldMotorDriver.stopMotor2();
        }
        else
        {
          baseShieldMotorDriver.runMotor2(motor2CurrentSpeed);
        }
      }
    }
    previousChar = inDat;
}
