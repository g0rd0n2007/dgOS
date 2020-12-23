#define ButtonOK_FG 0xFFFF
#define ButtonOK_BG 0x255F
#define ButtonCancel_FG 0xFFFF
#define ButtonCancel_BG 0xD3E0
#define ButtonNo_FG 0xFFFF
#define ButtonNo_BG 0x8800

#define CheckboxON_BG 0x255F
#define CheckboxOFF_BG 0xA514
#define Checkbox_FG TFT_WHITE

class cControl{
public:
  int16_t X, Y, Width, Height;

  void Draw(TTGOClass *ttgo);
  //TFT_eSprite *g;
  void Draw(TFT_eSprite *g);
};

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
  uint32_t CatchTime[10] = {0, 0, 0, 0, 0};
    
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

  void Draw(TFT_eSprite *g){
    g->setTextDatum(MC_DATUM);  

    if(Pressed == BS_Pressed || Pressed == BS_Holded)  {
      g->setTextColor(TFT_BLACK, TFT_WHITE);
      g->fillRoundRect(X, Y, Width, Height, 15, TFT_WHITE);      
    }else {
      g->setTextColor(FG, BG);
      if(FG != BG) g->fillRoundRect(X, Y, Width, Height, 15, BG);
      else g->drawRoundRect(X, Y, Width, Height, 15, FG);
    }
        
    g->setFreeFont(FSS9);
    
    
    g->drawString(Text, X + Width / 2, Y + Height / 2, GFXFF);
  }

  void Run(TTGOClass *ttgo, Swipe s, uint32_t now){
    switch(Pressed){
      case BS_Idle:
        if(s.Swiping && s.CatchInRect(X, Y, Width, Height)) { 
          Pressed = BS_Pressed;  

          CatchTime[9] = CatchTime[8];
          CatchTime[8] = CatchTime[7];
          CatchTime[7] = CatchTime[6];
          CatchTime[6] = CatchTime[5];
          CatchTime[5] = CatchTime[4];
          CatchTime[4] = CatchTime[3];
          CatchTime[3] = CatchTime[2];
          CatchTime[2] = CatchTime[1];
          CatchTime[1] = CatchTime[0];
          CatchTime[0] = now;
          
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

  void Run(TFT_eSprite *g, Swipe s, uint32_t now){
    switch(Pressed){
      case BS_Idle:
        if(s.Swiping && s.CatchInRect(X, Y, Width, Height)) { 
          Pressed = BS_Pressed;  

          CatchTime[9] = CatchTime[8];
          CatchTime[8] = CatchTime[7];
          CatchTime[7] = CatchTime[6];
          CatchTime[6] = CatchTime[5];
          CatchTime[5] = CatchTime[4];
          CatchTime[4] = CatchTime[3];
          CatchTime[3] = CatchTime[2];
          CatchTime[2] = CatchTime[1];
          CatchTime[1] = CatchTime[0];
          CatchTime[0] = now;
          
          Draw(g); 
          g->pushSprite(0, 0);
        }
        break;
      case BS_Pressed:
        Pressed = BS_Holded;
        break;
      case BS_Holded:
        if(!s.Swiping) {
          Pressed = BS_Released;
          Draw(g);
          g->pushSprite(0, 0);
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

  uint8_t GetQuickPressIdx(){
    #define QuickPressThresshold 700
    
    if((CatchTime[0] - CatchTime[1]) > QuickPressThresshold) return 0;
    else if((CatchTime[1] - CatchTime[2]) > QuickPressThresshold) return 1;
    else if((CatchTime[2] - CatchTime[3]) > QuickPressThresshold) return 2;
    else if((CatchTime[3] - CatchTime[4]) > QuickPressThresshold) return 3;
    else if((CatchTime[4] - CatchTime[5]) > QuickPressThresshold) return 4;
    else if((CatchTime[5] - CatchTime[6]) > QuickPressThresshold) return 5;
    else if((CatchTime[6] - CatchTime[7]) > QuickPressThresshold) return 6;
    else if((CatchTime[7] - CatchTime[8]) > QuickPressThresshold) return 7;
    else if((CatchTime[8] - CatchTime[9]) > QuickPressThresshold) return 8;
  }
};










class cCheckbox:cControl{
private:
  ButtonState Pressed = BS_Idle;
  
public:
  boolean Checked = false;
  uint16_t FG = Checkbox_FG, BG_OFF = CheckboxOFF_BG, BG_ON = CheckboxON_BG;
    
  cCheckbox(){};

  cCheckbox(int16_t x, int16_t y, int16_t w, int16_t h, boolean ch){
    X = x; Y = y; Width = w; Height = h; Checked = ch;
  }

  cCheckbox(int16_t x, int16_t y, int16_t w, int16_t h, boolean ch, uint16_t fg, uint16_t bg_on, uint16_t bg_off){
    X = x; Y = y; Width = w; Height = h; Checked = ch; FG = fg; BG_ON = bg_on; BG_OFF = bg_off;
  }
  
  void Draw(TTGOClass *ttgo){
    ttgo->tft->setTextDatum(MC_DATUM);  
    int r = Height / 2;
    const int a = 3;
    int r2 = (Height - 2 * a) / 2;

    //ttgo->tft->fillRect(X, Y, Width, Height, TFT_BLACK);
    ttgo->tft->fillRoundRect(X, Y, Width, Height, r, (Checked)? BG_ON : BG_OFF); 
    ttgo->tft->drawRoundRect(X, Y, Width, Height, r, FG); 
    if(Checked){      
      ttgo->tft->fillRoundRect(X + Width - Height + a, Y + a, Height - 2 * a, Height - 2 * a, r2, FG);       
    }else{
      ttgo->tft->fillRoundRect(X + a, Y + a, Height - 2 * a, Height - 2 * a, r2, FG); 
    }   
   
  }

  void Run(TTGOClass *ttgo, Swipe s){
    switch(Pressed){
      case BS_Idle:
        if(s.Swiping && s.CatchInRect(X, Y, Width, Height)) { 
          Pressed = BS_Pressed;  
          Checked = !Checked;
        }
        break;
      case BS_Pressed:
        Pressed = BS_Holded;
        break;
      case BS_Holded:
        if(!s.Swiping) {
          Pressed = BS_Released;
          //Draw(ttgo, false);
        }
        break;
      case BS_Released:        
        Pressed = BS_Idle;
        Draw(ttgo);
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
  
  void Draw(TTGOClass *ttgo, int importance = 1){

    ttgo->tft->setFreeFont(FSS9);
    ttgo->tft->setTextDatum(ML_DATUM);
      
    if(importance > 1) {
      ttgo->tft->setTextColor(FG, BG);      
      ttgo->tft->fillRoundRect(X, Y, Width, Height, 5, BG);
      ttgo->tft->drawRoundRect(X, Y, Width, Height, 5, FG);
      ttgo->tft->drawString(*Text, X + 7, Y + Height / 2, GFXFF);
    }

    int w = ttgo->tft->textWidth(*Text);
    if(Blinks && Cursor) {
      ttgo->tft->drawFastHLine(X + 7 + w, Y + 4, 3, FG);
      ttgo->tft->drawFastVLine(X + 8 + w, Y + 5, Height - 10, FG);
      ttgo->tft->drawFastHLine(X + 7 + w, Y + Height - 5, 3, FG);
    }else if(Blinks && !Cursor) {
      ttgo->tft->drawFastHLine(X + 7 + w, Y + 4, 3, BG);
      ttgo->tft->drawFastVLine(X + 8 + w, Y + 5, Height - 10, BG);
      ttgo->tft->drawFastHLine(X + 7 + w, Y + Height - 5, 3, BG);
    }
  }

  void Run(TTGOClass *ttgo, Swipe s, uint32_t t){
    if(CT.TON(t, true)){
      Cursor = !Cursor;
      Draw(ttgo, 1);
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
