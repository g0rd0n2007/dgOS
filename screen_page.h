int BacklightLevel = 50;

void ScreenPageDraw(TTGOClass *ttgo, cButton b[], cEdit e[]){
  ttgo->tft->fillScreen(TFT_BLACK); 
  //ttgo->tft->setTextDatum(TC_DATUM);   
  ttgo->tft->setTextColor(TFT_WHITE, TFT_BLACK);
  ttgo->tft->setFreeFont(FSS9);
  //ttgo->tft->drawString("Alarm", 120, 10, GFXFF);*/

  ttgo->tft->setTextDatum(TL_DATUM);
  ttgo->tft->drawString("Wylaczenie po:", 5, 50, GFXFF);
  ttgo->tft->drawString("Jasnosc:", 5, 100, GFXFF);
  
  for(int i = 0; i < 1; i++) b[i].Draw(ttgo, false);     
  for(int i = 0; i < 2; i++) e[i].Draw(ttgo, 2);  
}



void ScreenPage(TTGOClass *ttgo){
  Swipe s;
  uint32_t now; 
  String Stimeout = String(5);
  String Sbacklight = String(BacklightLevel);
  
  cButton _b[1] = {
    cButton(85, 205, 70, ButtonHeight, "Wstecz", ButtonOK_FG, ButtonOK_BG)//,
    //cButton(0, 30, ButtonWidth, ButtonHeight, "Alarm")
  };
  cEdit _e[] = {
    cEdit(170,  45, 60, ButtonHeight, &Stimeout, false),
    cEdit(170,  95, 60, ButtonHeight, &Sbacklight, false)
  };
  
    
  ScreenPageDraw(ttgo, _b, _e);
  

  do{s.Run(ttgo, millis()); } while(s.Swiping);

  do{
    now = millis();
    s.Run(ttgo, now);
    for(int i = 0; i < 1; i++) _b[i].Run(ttgo, s, now); 
    for(int i = 0; i < 2; i++) _e[i].Run(ttgo, s, now);    

    if(_e[0].IsReleased()){
      getInput(ttgo, "Timeout", &Stimeout, 2, BPLUS_NONE);
      //sleepT.Duration = 1000 * constrain(Stimeout.toInt(), 2, 15);
      ScreenPageDraw(ttgo, _b, _e);
    }
    if(_e[1].IsReleased()){
      getInput(ttgo, "Podswietlanie", &Sbacklight, 3, BPLUS_NONE);
      BacklightLevel = constrain(Sbacklight.toInt(), 1, 100);
      ttgo->bl->adjust(BacklightLevel);
      ScreenPageDraw(ttgo, _b, _e);
    }
  } while(!_b[0].IsReleased());

  do{s.Run(ttgo, millis()); } while(s.Swiping);
  ttgo->tft->fillScreen(TFT_BLACK); 
}
