#include "config.h"
#include <soc/rtc.h>
#include <time.h>
#include <inttypes.h>

#include "timer.h"
#include "Free_Fonts.h"
//#include "SPIFFS.h"
TTGOClass *ttgo;
TFT_eSprite *g;

Timer drawT, sleepT;
uint32_t Now;
boolean irqAPX202 = false, irqRTC = false, irqBMA = false;

String SecToTime(long int t);
void StepsMoveHistory(int s, bool shift);

long int StepCount = 0;
void ReadBMA_IRQ();

#include "icons.h"
#include "liscie.h"
#define Wallpaper liscie
#include "displayTime.h"
#include "displayBattery.h"
#include "sleepMode.h"
#include "swipeHandle.h"
#include "Controls.h"
#include "getInput.h"

#include "alarm_page.h"
#include "screen_page.h" 
#include "stoper_page.h"
#include "timer_page.h"
#include "steps_page.h"
#include "menu.h"


cButton cMenu = cButton(85, 205, 70, ButtonHeight, "...", TFT_WHITE, TFT_WHITE);
Swipe S;

void S_Release(){
  sleepT.SetNext(millis());
}



boolean S_Timeout(Swipe *s, uint32_t t){
  if((s->CatchTime + 1000) < t){
    if(s->CatchInRect(0, 40, 240, 80)){  
      String a;
      char buf[10];

      sprintf(buf, "%d:%02d", hh, mm);
      a = String(buf);
      
      if(getInput(g, "Ustawianie godziny:", &a, 5, BPLUS_HOUR)){
        sscanf(a.c_str(), "%d:%d", &hh, &mm);
        ttgo->rtc->setDateTime(yyear, mmonth, dday, hh, mm, 0);
      }
      sleepT.SetNext(millis());
      return true;
    }
    if(s->CatchInRect(0, 120, 240, 80)){
      String a;
      char buf[15];

      sprintf(buf, "%d/%d/%04d", dday, mmonth, yyear);
      a = String(buf);

      if(getInput(g, "Ustawianie daty:", &a, 10, BPLUS_DATE)){
        sscanf(a.c_str(), "%d/%d/%d", &dday, &mmonth, &yyear);
        ttgo->rtc->setDateTime(yyear, mmonth, dday, hh, mm, ss);
      }
      sleepT.SetNext(millis());
      return true;
    }
  }
  
  return false;
}

void DrawDesktop(bool push = true){
  
  
  //TFT_eSprite G(ttgo->tft);
  //G.setColorDepth(16);
  //G.createSprite(240, 240);
  //SP.createSprite(240, 240);
  //G.setSwapBytes(true);
  //ttgo->tft->setSwapBytes(true);
  g->pushImage(0, 0, 240, 240, Wallpaper);
  
  displayTime(g); 
  DisplaySteps(g);
  displayBattery(g); 
  displayAlarm(g);
  cMenu.Draw(g);

  //SP.pushSprite(0, 0);

  
  //G.fillSprite(TFT_GREEN);
  if(push) g->pushSprite(0, 0);
  //G.deleteSprite();  
}

void setup()
{    
    ttgo = TTGOClass::getWatch();
    ttgo->begin();

    g = new TFT_eSprite(ttgo->tft);
    g->setColorDepth(16);
    g->createSprite(240, 240);
    g->setSwapBytes(true);

    ttgo->openBL(); // Turn on the backlight
    ttgo->bl->adjust(BacklightLevel);


    //test
    /*for(int i=0;i<60;i++){
      g->fillSprite(TFT_YELLOW);
      g->pushSprite(0, 0);
      g->fillSprite(TFT_BLUE);
      g->pushSprite(0, 0);
    }*/


    //Stepcount
    // Accel parameter structure
    Acfg cfg;
    /*!
        Output data rate in Hz, Optional parameters:
            - BMA4_OUTPUT_DATA_RATE_0_78HZ
            - BMA4_OUTPUT_DATA_RATE_1_56HZ
            - BMA4_OUTPUT_DATA_RATE_3_12HZ
            - BMA4_OUTPUT_DATA_RATE_6_25HZ
            - BMA4_OUTPUT_DATA_RATE_12_5HZ
            - BMA4_OUTPUT_DATA_RATE_25HZ
            - BMA4_OUTPUT_DATA_RATE_50HZ
            - BMA4_OUTPUT_DATA_RATE_100HZ
            - BMA4_OUTPUT_DATA_RATE_200HZ
            - BMA4_OUTPUT_DATA_RATE_400HZ
            - BMA4_OUTPUT_DATA_RATE_800HZ
            - BMA4_OUTPUT_DATA_RATE_1600HZ
    */
    cfg.odr = BMA4_OUTPUT_DATA_RATE_100HZ;
    /*!
        G-range, Optional parameters:
            - BMA4_ACCEL_RANGE_2G
            - BMA4_ACCEL_RANGE_4G
            - BMA4_ACCEL_RANGE_8G
            - BMA4_ACCEL_RANGE_16G
    */
    cfg.range = BMA4_ACCEL_RANGE_2G;
    /*!
        Bandwidth parameter, determines filter configuration, Optional parameters:
            - BMA4_ACCEL_OSR4_AVG1
            - BMA4_ACCEL_OSR2_AVG2
            - BMA4_ACCEL_NORMAL_AVG4
            - BMA4_ACCEL_CIC_AVG8
            - BMA4_ACCEL_RES_AVG16
            - BMA4_ACCEL_RES_AVG32
            - BMA4_ACCEL_RES_AVG64
            - BMA4_ACCEL_RES_AVG128
    */
    cfg.bandwidth = BMA4_ACCEL_NORMAL_AVG4;

    /*! Filter performance mode , Optional parameters:
        - BMA4_CIC_AVG_MODE
        - BMA4_CONTINUOUS_MODE
    */
    cfg.perf_mode = BMA4_CONTINUOUS_MODE;

    // Configure the BMA423 accelerometer
    ttgo->bma->accelConfig(cfg);

    // Enable BMA423 accelerometer
    // Warning : Need to use steps, you must first enable the accelerometer
    // Warning : Need to use steps, you must first enable the accelerometer
    // Warning : Need to use steps, you must first enable the accelerometer
    ttgo->bma->enableAccel();
    
    // Enable BMA423 step count feature
    ttgo->bma->enableFeature(BMA423_STEP_CNTR, true);

    // Reset steps
    ttgo->bma->resetStepCounter();
        
    // ADC monitoring must be enabled to use the AXP202 monitoring function
    ttgo->power->adc1Enable(AXP202_VBUS_VOL_ADC1 | AXP202_VBUS_CUR_ADC1 | AXP202_BATT_CUR_ADC1 | AXP202_BATT_VOL_ADC1, true);
    //Przycisk
    pinMode(AXP202_INT, INPUT_PULLUP);
    pinMode(RTC_INT_PIN, INPUT_PULLUP);
    pinMode(BMA423_INT1, INPUT);
    
    attachInterrupt(AXP202_INT, [] {
        irqAPX202 = true;
    }, FALLING);
    attachInterrupt(RTC_INT_PIN, [] {
        irqRTC = true;
    }, FALLING);
    attachInterrupt(BMA423_INT1, [] {
        // Set interrupt to set irq value to 1
        irqBMA = true;
    }, RISING); //It must be a rising edge
    
    //!Clear IRQ unprocessed  first
    ttgo->power->enableIRQ(AXP202_PEK_SHORTPRESS_IRQ | AXP202_VBUS_REMOVED_IRQ | AXP202_VBUS_CONNECT_IRQ | AXP202_CHARGING_FINISHED_IRQ, true);
    ttgo->power->clearIRQ();
    // Turn on step interrupt
    ttgo->bma->enableStepCountInterrupt();

    ttgo->motor_begin();
    
    ttgo->rtc->check();
    ttgo->rtc->syncToSystem();
    ttgo->rtc->disableTimer();
    ttgo->rtc->disableCLK();
    ttgo->rtc->clearTimer();
    //RTC_Alarm alm = ttgo->rtc->getAlarm();
    //alarm_hh = alm.hour;
    //alarm_mm = alm.minute;
    ttgo->rtc->setAlarm(Alarm_hh[0], Alarm_mm[0], PCF8563_NO_ALARM, PCF8563_NO_ALARM, Alarm_Active[0]);
    ttgo->rtc->disableAlarm();
    AlarmActive = ttgo->rtc->isAlarmEnabled();
    
    
    Now = millis();
    drawT.Set(Now, 1000);
    sleepT.Set(Now, 5000);
    S.Release = S_Release;
    S.TimeOut = S_Timeout;
        
    DrawDesktop();
    //ttgo->motor->onec();
    setCpuFrequencyMhz(40);
}

void loop()
{
    Now = millis();

    if(!SleepMode){
      S.Run(ttgo, Now);
      cMenu.Run(g, S, Now);

      if(S.Swiping && S.CatchInRect(0, 0, 240, TopBarHeight)){
        int h;
        do{
          Now = millis();
          S.Run(ttgo, Now);
          h = S.Y - S.CatchY;

          DrawDesktop(false);
          g->fillRect(0, 0, 240, h, TFT_BLACK);
          g->drawXBitmap(5, h - 16 - 5, alarmIcon, 16, 16, TFT_WHITE);
          g->pushSprite(0, 0);
        }while(S.Swiping);

        if(h>60){//bedzie aktywne
          if(h>120){
            while(h>120){
              DrawDesktop(false);
              g->fillRect(0, 0, 240, h, TFT_BLACK);
              g->drawXBitmap(5, h - 16 - 5, alarmIcon, 16, 16, TFT_WHITE);
              g->pushSprite(0, 0);

              h-=10;
            }
          }else{
             while(h<120){
              DrawDesktop(false);
              g->fillRect(0, 0, 240, h, TFT_BLACK);
              g->drawXBitmap(5, h - 16 - 5, alarmIcon, 16, 16, TFT_WHITE);
              g->pushSprite(0, 0);

              h+=10;
            }
          }

          h = 120;
          DrawDesktop(false);
          g->fillRect(0, 0, 240, h, TFT_BLACK);
          g->drawXBitmap(5, h - 16 - 5, alarmIcon, 16, 16, TFT_WHITE);
          g->pushSprite(0, 0);

          delay(1000);

          while(h>0){
            DrawDesktop(false);
            g->fillRect(0, 0, 240, h, TFT_BLACK);
            g->drawXBitmap(5, h - 16 - 5, alarmIcon, 16, 16, TFT_WHITE);
            g->pushSprite(0, 0);

            h-=10;
          }
        }else{
          while(h>0){
            DrawDesktop(false);
            g->fillRect(0, 0, 240, h, TFT_BLACK);
            g->drawXBitmap(5, h - 16 - 5, alarmIcon, 16, 16, TFT_WHITE);
            g->pushSprite(0, 0);

            h-=10;
          }
        }


        DrawDesktop();
        sleepT.SetNext(millis());
      }

      if(cMenu.IsReleased()){
        Menu(ttgo);
        sleepT.SetNext(millis());
        drawT.TargetTime = 0;
        irqAPX202 = false;
        ttgo->power->clearIRQ();
      }
      
      if (drawT.TON(Now)) {
        drawT.SetNext(Now);
        DrawDesktop();
      }

      if(sleepT.TON(Now)){
        //Zasypiamy              
        StartLowPowerMode();   
        ttgo->power->clearIRQ(); irqAPX202 = false;     
      }  
    }
    
    
    if(irqAPX202){
      irqAPX202 = false;
      ttgo->power->readIRQ();
      if (ttgo->power->isVbusPlugInIRQ()) {
        if(SleepMode) WakeUp(Now);
        DrawDesktop();
        sleepT.SetNext(Now);
      }
      if (ttgo->power->isVbusRemoveIRQ()) {
        if(SleepMode) WakeUp(Now);
        DrawDesktop();
        sleepT.SetNext(Now);
      }
      if(ttgo->power->isChargingDoneIRQ()){
        if(SleepMode) WakeUp(Now);
        DrawDesktop();
        sleepT.SetNext(Now);
        ttgo->motor->onec();
      }
      if (ttgo->power->isPEKShortPressIRQ()) {
        if(SleepMode){
          WakeUp(Now);
        }else{
          ttgo->power->clearIRQ();
          StartLowPowerMode();
        }
      }
      ttgo->power->clearIRQ();      
    }


    
    if(ttgo->rtc->alarmActive()){
      irqRTC = false;
      ttgo->rtc->resetAlarm();
      Timer alm_t, to;
      boolean bo = true;

      alm_t.Set(0, 500);
      to.Set(millis(), 20000);
      if(SleepMode) WakeUp(millis());

      ttgo->tft->setFreeFont(FSS18);
      ttgo->tft->setTextDatum(MC_DATUM);

      do{
        Now = millis();
        S.Run(ttgo, Now); 

        if(alm_t.TON(Now, true)){
          if(bo){
            ttgo->tft->fillScreen(TFT_BLACK); 
            ttgo->tft->setTextColor(TFT_WHITE, TFT_BLACK);
            ttgo->motor->onec();
          }else{
            ttgo->tft->fillScreen(TFT_WHITE);
            ttgo->tft->setTextColor(TFT_BLACK, TFT_WHITE); 
          }
          ttgo->tft->drawString(CurAlmName, 120, 120, GFXFF);
          
          bo = !bo;          
        }
      } while(!S.Swiping && !to.TON(Now));

      ttgo->tft->fillScreen(TFT_BLACK); 
      sleepT.SetNext(Now);
      drawT.TargetTime = 0;
      TrySetNearestAlarm();
    }







    if (irqBMA) {
        ReadBMA_IRQ();
    }
}



void ReadBMA_IRQ(){
        irqBMA = false;
        bool  rlst;
        do {
            // Read the BMA423 interrupt status,
            // need to wait for it to return to true before continuing
            rlst =  ttgo->bma->readInterrupt();
        } while (!rlst);

        // Check if it is a step interrupt
        if (ttgo->bma->isStepCounter()) {
            RTC_Date tnow = ttgo->rtc->getDateTime();
            if(tnow.day != dday){
              StepsMoveHistory(StepCount, true);        
              ttgo->bma->resetStepCounter();
              dday = tnow.day;
            }
          
            // Get step data from register
            StepCount = ttgo->bma->getCounter(); 
            StepsMoveHistory(StepCount, false);            
        }
}
