#include "DBbutton.h"

void DBbutton::DBbutton(){
  this->signalPin = 0;
  this->state = false;
};

void DBbutton::DBbutton(int pinIn){
  this->signalPin = pinIn;
  this->state = false;
};

void DBbutton::changePin(int pinIn){
  this->signalPin = pinIn;
}

void DBbutton::changeState(bool stateIn){
  this->state = stateIn;
}

bool grabState(){
  return state;      
}

int DBbutton::grabPin(){
  return this->signalPin;
}
