void DrawMultilineText(String s, int x, int y){
  
  
}

class cControl{
public:
  int16_t X, Y, Width, Height;

  void Draw(TTGOClass *ttgo);
};

typedef void (*PressMethodType)();

enum ButtonState{
  BS_Idle = 0,
  BS_Pressed = 1,
  BS_Holded = 2,
  BS_Released = 3
};

class cButton:cControl{
private:
  ButtonState Pressed = BS_Idle;
  
public:
  String Text;
  uint16_t FG = TFT_WHITE, BG = 0xA514;  
    
  cButton(){};

  cButton(int16_t x, int16_t y, int16_t w, int16_t h, String txt){
    X = x; Y = y; Width = w; Height = h; Text = txt; 
  }

   cButton(int16_t x, int16_t y, int16_t w, int16_t h, String txt, uint16_t fg, uint16_t bg){
    X = x; Y = y; Width = w; Height = h; Text = txt; FG = fg; BG = bg;
  }
  
  void Draw(TTGOClass *ttgo, boolean Pushed = false){
    ttgo->tft->setTextDatum(MC_DATUM);  
    
    if(Pushed)  {
      ttgo->tft->setTextColor(TFT_BLACK, TFT_WHITE);
      ttgo->tft->fillRoundRect(X, Y, Width, Height, 5, TFT_WHITE);      
    }else {
      ttgo->tft->setTextColor(FG, BG);
      ttgo->tft->fillRoundRect(X, Y, Width, Height, 5, BG);
      //ttgo->tft->drawRoundRect(X, Y, Width, Height, 5, FG);
    }
        
    ttgo->tft->setFreeFont(FSS9);
    
    
    ttgo->tft->drawString(Text, X + Width / 2, Y + Height / 2, GFXFF);
  }

  void Run(TTGOClass *ttgo, Swipe s){
    switch(Pressed){
      case BS_Idle:
        if(s.Swiping && s.CatchInRect(X, Y, Width, Height)) { 
          Pressed = BS_Pressed;  
          Draw(ttgo, true); 
        }
        break;
      case BS_Pressed:
        Pressed = BS_Holded;
        break;
      case BS_Holded:
        if(!s.Swiping) {
          Pressed = BS_Released;
          Draw(ttgo, false);
        }
        break;
      case BS_Released:
        Pressed = BS_Idle;
        break;
    }    
  }

  boolean IsIdle(){
    if(Pressed == BS_Idle) return true;
    else return false;
  }

  boolean IsPressed(){
    if(Pressed == BS_Pressed) return true;
    else return false;
  }

  boolean IsHolded(){
    if(Pressed == BS_Holded) return true;
    else return false;
  }

  boolean IsReleased(){
    if(Pressed == BS_Released) return true;
    else return false;
  }
};









class cEdit:cControl{
private:
  ButtonState Pressed = BS_Idle;
  boolean Cursor = true, Blinks = false;
  Timer CT;
  uint8_t C_idx = 0;
  
public:
  String *Text;
  uint16_t FG = TFT_WHITE, BG = 0x3186;  
    
  cEdit(){};

  cEdit(int16_t x, int16_t y, int16_t w, int16_t h, String *txt, boolean blinks = false){
    X = x; Y = y; Width = w; Height = h; Text = txt; Blinks = blinks; C_idx = txt->length() - 1; 
    CT.Set(millis(), 500);
  }

  cEdit(int16_t x, int16_t y, int16_t w, int16_t h, String *txt, uint16_t fg, uint16_t bg, boolean blinks = false){
    X = x; Y = y; Width = w; Height = h; Text = txt; FG = fg; BG = bg; Blinks = blinks; C_idx = txt->length() - 1;
    CT.Set(millis(), 500);
  }
  
  void Draw(TTGOClass *ttgo){
    ttgo->tft->setTextColor(FG, BG);
    ttgo->tft->setFreeFont(FSS12);
    ttgo->tft->setTextDatum(TL_DATUM);
    ttgo->tft->fillRoundRect(X, Y, Width, Height, 5, BG);
    ttgo->tft->drawRoundRect(X, Y, Width, Height, 5, FG);
    ttgo->tft->drawString(*Text, X + 10, Y + 5, GFXFF);
    if(Blinks && Cursor) {
      //String a;
      
      //if(C_idx >= Text->length() - 1) a = Text->substring(0, C_idx);
      ttgo->tft->drawFastHLine(X + 10 + ttgo->tft->textWidth(*Text), Y + 4, 3, FG);
      ttgo->tft->drawFastVLine(X + 11 + ttgo->tft->textWidth(*Text), Y + 5, 20, FG);
      ttgo->tft->drawFastHLine(X + 10 + ttgo->tft->textWidth(*Text), Y + 25, 3, FG);
    }
  }

  void Run(TTGOClass *ttgo, Swipe s, uint32_t t){
    if(CT.TON(t, true)){
      Cursor = !Cursor;
      Draw(ttgo);
    }
    
    switch(Pressed){
      case BS_Idle:
        if(s.Swiping && s.CatchInRect(X, Y, Width, Height)) { 
          Pressed = BS_Pressed; 
        }
        break;
      case BS_Pressed:
        Pressed = BS_Holded;
        break;
      case BS_Holded:
        if(!s.Swiping) {
          Pressed = BS_Released;
        }
        break;
      case BS_Released:
        Pressed = BS_Idle;
        break;
    }    
  }

  boolean IsIdle(){
    if(Pressed == BS_Idle) return true;
    else return false;
  }

  boolean IsPressed(){
    if(Pressed == BS_Pressed) return true;
    else return false;
  }

  boolean IsHolded(){
    if(Pressed == BS_Holded) return true;
    else return false;
  }

  boolean IsReleased(){
    if(Pressed == BS_Released) return true;
    else return false;
  }
};
