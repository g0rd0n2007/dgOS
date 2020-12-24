int BacklightLevel = 50;

void ScreenPageDraw(TFT_eSprite *g, cButton b[], cEdit e[]){
  g->fillSprite(TFT_BLACK); 
  //ttgo->tft->setTextDatum(TC_DATUM);   
  g->setTextColor(TFT_WHITE, TFT_BLACK);
  g->setFreeFont(FSS9);
  //ttgo->tft->drawString("Alarm", 120, 10, GFXFF);*/

  g->setTextDatum(TL_DATUM);
  g->drawString("Wylaczenie po:", 5, 50, GFXFF);
  g->drawString("Jasnosc:", 5, 100, GFXFF);
  
  for(int i = 0; i < 1; i++) b[i].Draw(g);     
  for(int i = 0; i < 2; i++) e[i].Draw(g);  

  g->pushSprite(0, 0);
}



void ScreenPage(TTGOClass *ttgo){
  Swipe s;
  uint32_t now; 
  String Stimeout = String(sleepT.Duration / 1000);
  String Sbacklight = String(BacklightLevel);
  
  cButton _b[1] = {
    cButton(85, 205, 70, ButtonHeight, "<", ButtonOK_FG, ButtonOK_BG)//,
    //cButton(0, 30, ButtonWidth, ButtonHeight, "Alarm")
  };
  cEdit _e[] = {
    cEdit(170,  45, 60, ButtonHeight, &Stimeout, false),
    cEdit(170,  95, 60, ButtonHeight, &Sbacklight, false)
  };
  
    
  ScreenPageDraw(g, _b, _e);
  

  do{s.Run(ttgo, millis()); } while(s.Swiping);

  do{
    now = millis();
    s.Run(ttgo, now);
    for(int i = 0; i < 1; i++) _b[i].Run(ttgo, s, now); 
    for(int i = 0; i < 2; i++) _e[i].Run(ttgo, s, now);    

    if(_e[0].IsReleased()){
      getInput(g, "Timeout", &Stimeout, 2, BPLUS_NONE);
      sleepT.Duration = 1000 * constrain(Stimeout.toInt(), 2, 15);
      Stimeout = String(sleepT.Duration / 1000);
      ScreenPageDraw(g, _b, _e);
    }
    if(_e[1].IsReleased()){
      getInput(g, "Podswietlanie", &Sbacklight, 3, BPLUS_NONE);
      BacklightLevel = constrain(Sbacklight.toInt(), 1, 100);
      ttgo->bl->adjust(BacklightLevel);
      ScreenPageDraw(g, _b, _e);
    }
  } while(!_b[0].IsReleased());

  do{s.Run(ttgo, millis()); } while(s.Swiping);
  //ttgo->tft->fillScreen(TFT_BLACK); 
}
