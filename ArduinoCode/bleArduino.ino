
#include <Servo.h>
#include <SPI.h>
#include <EEPROM.h>
#include <boards.h>
#include <RBL_nRF8001.h>

#define UP '^'
#define DOWN 'v'
#define LEFT '<'
#define RIGHT '>'
#define STAY '-'
#define STOP 'x'
//global state
static byte eventState = STOP;

class Motor
{
    int  mPwm;
    int  mBrake;
    int  mDir;
    bool mFlip;

public:
  
    Motor(int pwm,
          int brake,
          int dir,
          bool flip)
    {
       mPwm = pwm;
       mBrake = brake;
       mDir = dir;
       mFlip = flip;
    }
    
    void init() const
    {
                    pinMode(mPwm, OUTPUT); 
      if(mBrake>=0) pinMode(mBrake, OUTPUT);
                    pinMode(mDir, OUTPUT); 
    }
    
    void forward(byte value=255) const
    {
      digitalWrite(mDir, mFlip ? LOW : HIGH);
      if(mBrake>=0) digitalWrite(mBrake, LOW);   
      analogWrite(mPwm, value);
    }
    
    void backward(byte value=255) const
    {
      digitalWrite(mDir, mFlip ? HIGH : LOW);
      if(mBrake>=0) digitalWrite(mBrake, LOW);   
      analogWrite(mPwm, value);
    }
    
    void stop() const
    {
      digitalWrite(mDir, HIGH);
      if(mBrake>=0) digitalWrite(mBrake, HIGH);   
      analogWrite(mPwm, 0);
    }
};


//right

static Motor mright(6, // pwm
                   -1, // brake
                    2, // dir
                    true) ;

//left
static Motor mleft (5, // pwm
                   -1, // brake
                    7, // dir
                    false) ;

//execute move comand
void doMove() //stop
{ 
  #define MAX_RIGHT 250//245 //240
  #define MID_RIGHT 230//200 //195
  #define MAX_LEFT 255
  #define MID_LEFT 230//220
  switch(eventState)
  {
    case STAY: default: break;
    //case UP:
    case DOWN:
       mright.backward(MAX_RIGHT);
       mleft.backward(MAX_LEFT);
    break;
    case UP:
       mright.forward(MAX_RIGHT);
       mleft.forward(MAX_LEFT);
    break;
    case RIGHT:
       mright.stop();
       mleft.forward(MID_LEFT);
    break;
    case LEFT:
       mright.forward(MID_RIGHT);
       mleft.stop();
    break;
    case STOP:;
       mright.stop();
       mleft.stop();
    break;
  }
}
//setup arduino
void setup() 
{
    Serial.begin(57600);
    Serial.println("BLE Arduino Slave 001");
    //init ble libary
    ble_begin();
    //init timer pin 6-5
    //https://prototyperobotics.com/steps/2
    TCCR0B = TCCR0B & 0b11111000 | 0x03;
    //init  standard arduino motors pins
    mleft.init();
    mright.init(); 
    //do action
    doMove();

}

//print command
bool moveIsChanged()
{
  static byte oldEventState = eventState;
  if(eventState == oldEventState) return false;
  oldEventState = eventState;
  return true;
}

void movePrint()
{
  switch(eventState)
  {
    case STAY: default:
      Serial.print("STAY");
    break;
    case DOWN:   
      Serial.print("DOWN");
    break;
    case UP:
      Serial.print("UP");
    break;
    case RIGHT:
      Serial.print("RIGHT");
    break;
    case LEFT:
      Serial.print("LEFT");
    break;
    case STOP:
      Serial.print("STOP");
    break;
  }
}

//loop arduino
void loop() 
{
    while(ble_available())
    {
        byte cmd = ble_read();
        switch(cmd)
        {
          case 'm':
          {
            //read new state
            byte move = ble_read();
            switch(move)
            {
              case STAY: default: break;
              case UP: eventState=UP; break;
              case DOWN: eventState=DOWN; break;
              case LEFT: eventState=LEFT; break;
              case RIGHT: eventState=RIGHT; break;
              case STOP: eventState=STOP; break;
            }
          } 
          break;
          case 'r':
            eventState = STOP;
          default: break;
        }
    }
    //if isn't connected
    if(!ble_connected())
    { 
      eventState = STOP;
    }
    //proc events
    ble_do_events();
    //move event
    if(moveIsChanged())
    {
      mleft.stop();
      mright.stop();
      //print
      Serial.print("|");
      movePrint();
      Serial.print("|\n");
    }
    //do move action
    doMove();
}
