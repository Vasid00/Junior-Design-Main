

class DBbutton{
  public:
    DBbutton();
    DBbutton(int pinIn);
    
    void changePin(int pinIn);
    bool isPressed();
    int grabPin();
  
  private:
    bool state;
    int signalPin;


}
