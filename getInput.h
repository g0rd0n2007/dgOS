#define ButtonWidth 55
#define ButtonHeight 30
#define X1 0
#define X2 62
#define X3 124
#define X4 185
#define Y1 100
#define Y2 135
#define Y3 170
#define Y4 205
#define DigitBG 0x01AE
#define DigitFG 0xFFFF

#define ButtonOK_FG 0xFFFF
#define ButtonOK_BG 0x255F
#define ButtonCancel_FG 0xFFFF
#define ButtonCancel_BG 0xD3E0
#define ButtonNo_FG 0xFFFF
#define ButtonNo_BG 0x8800

String BPLUS_NONE[] = {"", "", "", ""};
String BPLUS_HOUR[] = {":", "", "", ""};
String BPLUS_DATE[] = {".", "", "", ""};

/*void DrawTXT(TTGOClass *ttgo, String *TXT, boolean Cursor = true){
    ttgo->tft->setTextColor(DigitFG, DigitBG);
    ttgo->tft->setFreeFont(FSS12);
    ttgo->tft->setTextDatum(TL_DATUM);
    ttgo->tft->fillRoundRect(0, 45, 176, 30, 5, DigitBG);
    ttgo->tft->drawRoundRect(0, 45, 176, 30, 5, 0x4D1F);
    ttgo->tft->drawString(*TXT, 10, 50, GFXFF);
    if(Cursor) {
      ttgo->tft->drawFastHLine(9 + ttgo->tft->textWidth(*TXT), 49, 5, DigitFG);
      ttgo->tft->drawFastVLine(11 + ttgo->tft->textWidth(*TXT), 50, 20, DigitFG);
      ttgo->tft->drawFastHLine(9 + ttgo->tft->textWidth(*TXT), 70, 5, DigitFG);
    }
}*/
  
boolean getInput(TTGOClass *ttgo, String info, String *TXT, int max_length, String bplus[4]){
    Swipe s;
    uint32_t now;
    cButton _b[17] = {
      //0
      cButton(X2, Y4, ButtonWidth, ButtonHeight, "0"),     
            
      cButton(X1, Y1, ButtonWidth, ButtonHeight, "1"),
      cButton(X2, Y1, ButtonWidth, ButtonHeight, "2"),
      cButton(X3, Y1, ButtonWidth, ButtonHeight, "3"),
      
      cButton(X1, Y2, ButtonWidth, ButtonHeight, "4"),
      cButton(X2, Y2, ButtonWidth, ButtonHeight, "5"), 
      cButton(X3, Y2, ButtonWidth, ButtonHeight, "6"),
      
      cButton(X1, Y3, ButtonWidth, ButtonHeight, "7"),
      cButton(X2, Y3, ButtonWidth, ButtonHeight, "8"),
      cButton(X3, Y3, ButtonWidth, ButtonHeight, "9"),
      //10
      cButton(X1, Y4, ButtonWidth, ButtonHeight, "Anuluj", ButtonCancel_FG, ButtonCancel_BG),
      //11
      cButton(X4, Y4, ButtonWidth, ButtonHeight, "OK", ButtonOK_FG, ButtonOK_BG),
      //12
      cButton(X4, 45, ButtonWidth, ButtonHeight, "<"),
      //13
      cButton(X4, Y1, ButtonWidth, ButtonHeight, bplus[0]),
      cButton(X4, Y2, ButtonWidth, ButtonHeight, bplus[1]),
      cButton(X4, Y3, ButtonWidth, ButtonHeight, bplus[2]),
      cButton(X3, Y4, ButtonWidth, ButtonHeight, bplus[3]) 
    };
    cEdit Edit = cEdit(0, 45, 176, ButtonHeight, TXT, true);
    //boolean ShowCursor = true;
  
    
    ttgo->tft->fillScreen(TFT_BLACK); 
    ttgo->tft->setTextDatum(TL_DATUM);   
  
    ttgo->tft->setTextColor(TFT_WHITE, TFT_BLACK);
    ttgo->tft->setFreeFont(FSS9);
    ttgo->tft->drawString(info, 10, 10, GFXFF);
  
    for(int i = 0; i < 17; i++) _b[i].Draw(ttgo, false);  
    Edit.Draw(ttgo);  
    //CT.Set(millis(), 500);
    //DrawTXT(ttgo, TXT, ShowCursor);
  
    do{s.Run(ttgo, millis()); } while(s.Swiping);
    
    do{
      now = millis();
      s.Run(ttgo, now);
      for(int i = 0; i < 17; i++) _b[i].Run(ttgo, s);
      Edit.Run(ttgo, s, now);

      if(TXT->length() < max_length){ 
        if(_b[0].IsPressed()) { *TXT = *TXT + "0"; Edit.Draw(ttgo); }
        else if(_b[1].IsPressed()) { *TXT = *TXT + "1"; Edit.Draw(ttgo); }
        else if(_b[2].IsPressed()) { *TXT = *TXT + "2"; Edit.Draw(ttgo); }
        else if(_b[3].IsPressed()) { *TXT = *TXT + "3"; Edit.Draw(ttgo); }
        else if(_b[4].IsPressed()) { *TXT = *TXT + "4"; Edit.Draw(ttgo); }
        else if(_b[5].IsPressed()) { *TXT = *TXT + "5"; Edit.Draw(ttgo); }
        else if(_b[6].IsPressed()) { *TXT = *TXT + "6"; Edit.Draw(ttgo); }
        else if(_b[7].IsPressed()) { *TXT = *TXT + "7"; Edit.Draw(ttgo); }
        else if(_b[8].IsPressed()) { *TXT = *TXT + "8"; Edit.Draw(ttgo); }
        else if(_b[9].IsPressed()) { *TXT = *TXT + "9"; Edit.Draw(ttgo); }
        else if(_b[13].IsPressed()) { *TXT = *TXT + _b[13].Text; Edit.Draw(ttgo); }
        else if(_b[14].IsPressed()) { *TXT = *TXT + _b[14].Text; Edit.Draw(ttgo); }
        else if(_b[15].IsPressed()) { *TXT = *TXT + _b[15].Text; Edit.Draw(ttgo); }
        else if(_b[16].IsPressed()) { *TXT = *TXT + _b[16].Text; Edit.Draw(ttgo); }
      }
      if(TXT->length() > 0) {
        if(_b[12].IsPressed()){//backspace
          TXT->remove(TXT->length() - 1, 1); 
          Edit.Draw(ttgo);        
        }
      }      
    }while(!_b[10].IsReleased() && !_b[11].IsReleased());

    do{s.Run(ttgo, millis()); } while(s.Swiping);
    ttgo->tft->fillScreen(TFT_BLACK); 

    if(_b[11].IsReleased()) return true;
    else return false;
}










enum DialogResult{
  Result_OK = 1,
  Result_Cancel = 2,
  Result_No = 4
};

enum DialogCfg{
  Dialog_OK = 1,
  Dialog_OK_Cancel = 2,
  Dialog_OK_No_Cancel = 4,
  Dialog_OK_No = 8
};

int MessageBox(TTGOClass *ttgo, String caption, String message, uint8_t DC){
    Swipe s;
    uint32_t now;
    cButton bOK, bCancel, bNo;

    ttgo->tft->fillScreen(TFT_BLACK); 
    ttgo->tft->setTextDatum(TL_DATUM);   
  
    ttgo->tft->setTextColor(TFT_WHITE, TFT_BLACK);
    ttgo->tft->setFreeFont(FSS9);
    ttgo->tft->drawString(caption, 10, 10, GFXFF);

    ttgo->tft->setTextDatum(TC_DATUM);  
    ttgo->tft->drawString(message, 120, 60, GFXFF);

    switch(DC){
      case Dialog_OK:
        bOK = cButton(93, 150, ButtonWidth, ButtonHeight, "OK", ButtonOK_FG, ButtonOK_BG); bOK.Draw(ttgo);       
        break;
      case Dialog_OK_Cancel:
        bOK = cButton(150, 150, ButtonWidth, ButtonHeight, "OK", ButtonOK_FG, ButtonOK_BG); bOK.Draw(ttgo);
        bCancel = cButton(90, 150, ButtonWidth, ButtonHeight, "Anuluj", ButtonCancel_FG, ButtonCancel_BG); bCancel.Draw(ttgo);
        break;
      case Dialog_OK_No_Cancel:
        bOK = cButton(180, 150, ButtonWidth, ButtonHeight, "OK", ButtonOK_FG, ButtonOK_BG); bOK.Draw(ttgo);
        bCancel = cButton(60, 150, ButtonWidth, ButtonHeight, "Anuluj", ButtonCancel_FG, ButtonCancel_BG); bCancel.Draw(ttgo);
        bNo = cButton(120, 150, ButtonWidth, ButtonHeight, "Nie", ButtonNo_FG, ButtonNo_BG); bNo.Draw(ttgo);
        break;
      case Dialog_OK_No:
        bOK = cButton(150, 150, ButtonWidth, ButtonHeight, "OK", ButtonOK_FG, ButtonOK_BG); bOK.Draw(ttgo);
        bNo = cButton(50, 150, ButtonWidth, ButtonHeight, "Nie", ButtonNo_FG, ButtonNo_BG); bNo.Draw(ttgo);
        break;
    }  
    
    do{s.Run(ttgo, millis()); } while(s.Swiping);
    
    do{
      now = millis();
      s.Run(ttgo, now);
      
      if(DC == Dialog_OK || DC == Dialog_OK_Cancel || DC == Dialog_OK_No_Cancel || DC == Dialog_OK_No){
        bOK.Run(ttgo, s);
        if(bOK.IsReleased()) break;
      }
      if(DC == Dialog_OK_Cancel || DC == Dialog_OK_No_Cancel){
        bCancel.Run(ttgo, s);
        if(bCancel.IsReleased()) break;
      }
      if(DC == Dialog_OK_No_Cancel || DC == Dialog_OK_No){
        bNo.Run(ttgo, s);
        if(bNo.IsReleased()) break;
      }
    }while(1);

    do{s.Run(ttgo, millis()); } while(s.Swiping);
    ttgo->tft->fillScreen(TFT_BLACK); 

    if(DC == Dialog_OK || DC == Dialog_OK_Cancel || DC == Dialog_OK_No_Cancel || DC == Dialog_OK_No){
      if(bOK.IsReleased()) return Result_OK;
    }
    if(DC == Dialog_OK_Cancel || DC == Dialog_OK_No_Cancel){
      if(bCancel.IsReleased()) return Result_Cancel;
    }
    if(DC == Dialog_OK_No_Cancel || DC == Dialog_OK_No){
      if(bNo.IsReleased()) return Result_No;
    }

    return 0;
}
