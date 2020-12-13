void MenuDraw(TTGOClass *ttgo, cButton b[]){
  ttgo->tft->fillScreen(TFT_BLACK); 
  ttgo->tft->setTextDatum(TC_DATUM);   
  ttgo->tft->setTextColor(TFT_WHITE, TFT_BLACK);
  ttgo->tft->setFreeFont(FSS9);
  ttgo->tft->drawString("Menu", 120, 10, GFXFF);
  for(int i = 0; i < 2; i++) b[i].Draw(ttgo, false);  
}

void Menu(TTGOClass *ttgo){
  Swipe s;
  uint32_t now;
  cButton _b[2] = {
    cButton(100, 200, ButtonWidth, ButtonHeight, "Back", ButtonOK_FG, ButtonOK_BG),
    cButton(0, 50, ButtonWidth, ButtonHeight, "Alarm")
  };
  
  MenuDraw(ttgo, _b);
  

  do{s.Run(ttgo, millis()); } while(s.Swiping);

  do{
    now = millis();
    s.Run(ttgo, now);
    for(int i = 0; i < 2; i++) _b[i].Run(ttgo, s);

    if(_b[1].IsReleased()){
      AlarmPage(ttgo);
      MenuDraw(ttgo, _b);
    }
  } while(!_b[0].IsReleased());

  do{s.Run(ttgo, millis()); } while(s.Swiping);
  ttgo->tft->fillScreen(TFT_BLACK); 
}
