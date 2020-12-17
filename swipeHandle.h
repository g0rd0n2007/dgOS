class Swipe{
public:
  boolean Swiping = false;
  int16_t CatchX = -1, CatchY = -1, ReleaseX = -1, ReleaseY = -1, X = -1, Y = -1;
  uint32_t CatchTime = 0;

  void (*Catch)();
  void (*Release)();
  boolean (*TimeOut)(Swipe *, uint32_t t);

  Swipe(){
    Catch = nullptr;
    Release = nullptr;
    TimeOut = nullptr;
  }

  void Run(TTGOClass *ttgo, uint32_t Now){
    if(SleepMode) return;

    boolean NewSwiping = ttgo->getTouch(X, Y);

    if(NewSwiping && !Swiping){//Start Swipingu
      CatchX = X;
      CatchY = Y;
      Swiping = NewSwiping;

      CatchTime = Now;
      
      if(Catch != nullptr) Catch();
    }else if(!NewSwiping && Swiping){//Koniec Swipingu
      ReleaseX = X;
      ReleaseY = Y;
      Swiping = NewSwiping;
      if(Release != nullptr) Release();
    }else{
      Swiping = NewSwiping;
      if(Swiping) {
        if(TimeOut != nullptr) {
          if(TimeOut(this, Now)) Swiping = false;
        }
      }
    }
  }

  boolean CatchInRect(int16_t X, int16_t Y, int16_t Width, int16_t Height){
    return (X <= CatchX && CatchX <= (X + Width) && Y <= CatchY && CatchY <= (Y + Height));
  }
}Swiping;
