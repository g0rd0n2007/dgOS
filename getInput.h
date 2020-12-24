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



String BPLUS_NONE[] = {"", "", "", ""};
String BPLUS_HOUR[] = {":", "", "", ""};
String BPLUS_DATE[] = {"/", "", "", ""};
String BPLUS_ALFA[] = {"", "", "Caps", "1aA"};

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

void StrQuickAdd(String *t, String chars, int q_idx, int max_len){
  
  if(q_idx == 0 && chars.length() > 0 && t->length() < max_len) *t = *t + chars[0];
  else {
    for(int i=1; i<9 && i<chars.length(); i++){
      if(q_idx == i) {
        t->setCharAt(t->length() - 1, chars[i]);
        return;
      }
    }
  }
  
            /*case 1: TXT->setCharAt(TXT->length() - 1, 'q'); break;
            case 2: TXT->setCharAt(TXT->length() - 1, 'w'); break;
            case 3: TXT->setCharAt(TXT->length() - 1, 'e'); break;
          }*/
}

void getInput_T2M(int Mode, cButton _b[]){
  switch(Mode){
            case 0:
              _b[0].Text = "0"; 
              _b[1].Text = "1"; 
              _b[2].Text = "2"; 
              _b[3].Text = "3"; 
              _b[4].Text = "4"; 
              _b[5].Text = "5"; 
              _b[6].Text = "6"; 
              _b[7].Text = "7"; 
              _b[8].Text = "8"; 
              _b[9].Text = "9";
              break;
            case 1:
              _b[0].Text = "_"; 
              _b[1].Text = "qwe"; 
              _b[2].Text = "rtyu"; 
              _b[3].Text = "iop"; 
              _b[4].Text = "asd"; 
              _b[5].Text = "fgh"; 
              _b[6].Text = "jkl"; 
              _b[7].Text = "zx"; 
              _b[8].Text = "cvb"; 
              _b[9].Text = "nm";
              break;
            case 2:
              _b[0].Text = "_"; 
              _b[1].Text = "QWE"; 
              _b[2].Text = "RTYU"; 
              _b[3].Text = "IOP"; 
              _b[4].Text = "ASD"; 
              _b[5].Text = "FGH"; 
              _b[6].Text = "JKL"; 
              _b[7].Text = "ZX"; 
              _b[8].Text = "CVB"; 
              _b[9].Text = "NM";
              break;
            case 3:
              _b[0].Text = "_"; 
              _b[1].Text = "."; 
              _b[2].Text = ","; 
              _b[3].Text = "()"; 
              _b[4].Text = "[]"; 
              _b[5].Text = ":;"; 
              _b[6].Text = "-_"; 
              _b[7].Text = "?"; 
              _b[8].Text = "@"; 
              _b[9].Text = "!";
              break;
          }
}

void getInputDraw(TFT_eSprite *g, String info, cButton _b[], cEdit *_e){
  
  g->fillSprite(TFT_BLACK);

  g->setTextDatum(TL_DATUM);   
  g->setTextColor(TFT_WHITE, TFT_BLACK);
  g->setFreeFont(FSS9);
  g->drawString(info, 10, 10, GFXFF);

  for(int i = 0; i < 17; i++) _b[i].Draw(g);  
  _e->Draw(g);  

  g->pushSprite(0, 0);
  
}
  
boolean getInput(TFT_eSprite *g, String info, String *TXT, int max_length, String bplus[4]){
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
    

    int Mode = 0;
    if(bplus[3] == "1aA") Mode = 1;
    getInput_T2M(Mode, _b);
  
    getInputDraw(g, info, _b, &Edit);    
  
    do{s.Run(ttgo, millis()); } while(s.Swiping);
    
    do{
      now = millis();
      s.Run(ttgo, now);
      for(int i = 0; i < 17; i++) _b[i].Run(ttgo, s, now);
      Edit.Run(ttgo, s, now);

      
      if(_b[0].IsPressed()) { 
          String buf = _b[0].Text;
          buf.replace("_", " ");
          StrQuickAdd(TXT, buf, _b[0].GetQuickPressIdx(), max_length);
          getInputDraw(g, info, _b, &Edit);
      }
      else if(_b[1].IsPressed()) { 
          StrQuickAdd(TXT, _b[1].Text, _b[1].GetQuickPressIdx(), max_length);
          getInputDraw(g, info, _b, &Edit);
      }
      else if(_b[2].IsPressed()) { 
          StrQuickAdd(TXT, _b[2].Text, _b[2].GetQuickPressIdx(), max_length);
          getInputDraw(g, info, _b, &Edit);
      }
      else if(_b[3].IsPressed()) { 
          StrQuickAdd(TXT, _b[3].Text, _b[3].GetQuickPressIdx(), max_length); 
          getInputDraw(g, info, _b, &Edit);
      }
      else if(_b[4].IsPressed()) { 
          StrQuickAdd(TXT, _b[4].Text, _b[4].GetQuickPressIdx(), max_length);
          getInputDraw(g, info, _b, &Edit);
      }
      else if(_b[5].IsPressed()) { 
          StrQuickAdd(TXT, _b[5].Text, _b[5].GetQuickPressIdx(), max_length);
          getInputDraw(g, info, _b, &Edit);
      }
      else if(_b[6].IsPressed()) { 
          StrQuickAdd(TXT, _b[6].Text, _b[6].GetQuickPressIdx(), max_length);
          getInputDraw(g, info, _b, &Edit);
      }
      else if(_b[7].IsPressed()) { 
          StrQuickAdd(TXT, _b[7].Text, _b[7].GetQuickPressIdx(), max_length);
          getInputDraw(g, info, _b, &Edit);
      }
      else if(_b[8].IsPressed()) { 
          StrQuickAdd(TXT, _b[8].Text, _b[8].GetQuickPressIdx(), max_length);
          getInputDraw(g, info, _b, &Edit);
      }
      else if(_b[9].IsPressed()) { 
          StrQuickAdd(TXT, _b[9].Text, _b[9].GetQuickPressIdx(), max_length);
          getInputDraw(g, info, _b, &Edit);
      }
      else if(_b[13].IsPressed()) { 
          StrQuickAdd(TXT, _b[13].Text, _b[13].GetQuickPressIdx(), max_length);
          getInputDraw(g, info, _b, &Edit);
      }
      else if(_b[14].IsPressed()) { 
          StrQuickAdd(TXT, _b[14].Text, _b[14].GetQuickPressIdx(), max_length);
          getInputDraw(g, info, _b, &Edit);
      }
      else if(_b[15].IsPressed() && _b[15].Text == "Caps") {
          if(Mode == 1) Mode = 2;
          else if(Mode == 2) Mode = 1;

          getInput_T2M(Mode, _b);
          //for(int i = 0; i < 17; i++) _b[i].Draw(ttgo, false); 
          getInputDraw(g, info, _b, &Edit);
      }        
            
      if(TXT->length() > 0) {
        if(_b[12].IsPressed()){//backspace
          TXT->remove(TXT->length() - 1, 1); 
          //Edit.Draw(ttgo, 2); 
          getInputDraw(g, info, _b, &Edit);       
        }
      }
      if(_b[16].IsPressed() && _b[16].Text == "1aA") { 
          if(Mode == 0) Mode = 1;
          else if(Mode == 1 || Mode == 2) Mode = 3;
          else if(Mode == 3) Mode = 0;

          getInput_T2M(Mode, _b);
          //for(int i = 0; i < 17; i++) _b[i].Draw(ttgo, false);
          getInputDraw(g, info, _b, &Edit);
      }
    }while(!_b[10].IsReleased() && !_b[11].IsReleased());

    do{s.Run(ttgo, millis()); } while(s.Swiping);
    //ttgo->tft->fillScreen(TFT_BLACK); 

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
        bOK.Run(ttgo, s, now);
        if(bOK.IsReleased()) break;
      }
      if(DC == Dialog_OK_Cancel || DC == Dialog_OK_No_Cancel){
        bCancel.Run(ttgo, s, now);
        if(bCancel.IsReleased()) break;
      }
      if(DC == Dialog_OK_No_Cancel || DC == Dialog_OK_No){
        bNo.Run(ttgo, s, now);
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
