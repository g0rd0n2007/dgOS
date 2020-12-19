#define TopBarHeight 40
#define BottomBarHeight 40
#define NO_OF_ALARMS 3

int hh, mm, ss, mmonth, dday, dayOW; // H, M, S variables
int yyear; // Year is 16 bit int

int Alarm_hh[NO_OF_ALARMS] = { 6, 10, 11};
int Alarm_mm[NO_OF_ALARMS] = { 45, 30, 00};
boolean Alarm_Active[NO_OF_ALARMS] = { false, false, false };
String Alarm_Names[NO_OF_ALARMS] = { "Budzik", "Obiad", "Spotkanie" };
boolean AlarmActive = false;
String CurAlmName = "Alarm";

String Days[]={"Niedziela", "Poniedzialek", "Wtorek", "Sroda", "Czwartek", "Piatek", "Sobota"};

/*uint8_t dayOfWeek(int d, int m, int y){
  m = (m + 9) % 12;
  y -= m / 10;
  uint32_t dn = 365*y + y/4 - y/100 + y/400 + (m*306 + 5)/10 + (d - 1);

  return dn % 7;  
}*/

void displayTime() {
  char txt[20];
  static int Last_mm, Last_hh;
    
  RTC_Date tnow = ttgo->rtc->getDateTime();
  hh = tnow.hour;
  mm = tnow.minute;
  ss = tnow.second;
  dday = tnow.day;
  mmonth = tnow.month;
  yyear = tnow.year;
  dayOW = ttgo->rtc->getDayOfWeek(dday, mmonth, yyear);

  // Set text datum to middle centre
  ttgo->tft->setTextDatum(TC_DATUM);  
  if(mm != Last_mm || hh != Last_hh) {
    ttgo->tft->fillRect(0, TopBarHeight, 240, 240 - TopBarHeight - BottomBarHeight, TFT_BLACK); 
    //ttgo->tft->pushRect(0, TopBarHeight, 240, 160, desktopBitmap);
  }
  Last_hh = hh;
  Last_mm = mm;
  
  sprintf(txt, "%d:%02d", hh, mm);
  ttgo->tft->setTextColor(TFT_WHITE);
  ttgo->tft->setFreeFont(FSSB24);
  ttgo->tft->drawString(txt, 120, 60, GFXFF);
  
  sprintf(txt, "%02d/%02d/%04d", dday, mmonth, yyear);
  //ttgo->tft->setTextColor(0x94B2);
  ttgo->tft->setFreeFont(FSS12);
  ttgo->tft->drawString(txt, 120, 130, GFXFF);

  
  
  ttgo->tft->drawString(Days[dayOW], 120, 160, GFXFF);

  //ttgo->tft->setTextColor(TFT_WHITE, TFT_BLACK);
  //sprintf(txt, "% 5d", sleepT.TargetTime - millis());
  //ttgo->tft->drawString(txt, 120, 185, GFXFF);
  
}
