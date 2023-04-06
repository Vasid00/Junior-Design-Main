

class DBbutton{
  public:
    DBbutton();
    DBbutton(int pinIn);
    
    void changePin(int pinIn);
    void changeState(bool stateIn);
    bool grabState();
    int grabPin();
  
  private:
    bool state;
    int signalPin;


}
