boolean SleepMode = false;
Timer sleepT;


void WakeUp(uint32_t now){
  setCpuFrequencyMhz(40);
  SleepMode = false;
  sleepT.SetNext(now);
  ttgo->displayWakeup();
  ttgo->rtc->syncToSystem();  
  ttgo->openBL();  
}

void StartSleep(){
  SleepMode = true;
  
  // Set  touchscreen sleep
  ttgo->displaySleep();

  setCpuFrequencyMhz(5);
  ttgo->closeBL();
}

void ShutDown(){
  SleepMode = true;

  // Set  touchscreen sleep
  ttgo->displaySleep();
  setCpuFrequencyMhz(5);
  
#ifdef LILYGO_WATCH_2020_V1
    ttgo->powerOff();
    // LDO2 is used to power the display, and LDO2 can be turned off if needed
    // power->setPowerOutPut(AXP202_LDO2, false);
#else
    ttgo->power->setPowerOutPut(AXP202_LDO3, false);
    ttgo->power->setPowerOutPut(AXP202_LDO4, false);
    ttgo->power->setPowerOutPut(AXP202_LDO2, false);
    // The following power channels are not used
    ttgo->power->setPowerOutPut(AXP202_EXTEN, false);
    ttgo->power->setPowerOutPut(AXP202_DCDC2, false);
#endif

    // Use ext0 for external wakeup
    esp_sleep_enable_ext0_wakeup((gpio_num_t)AXP202_INT, LOW);

    // Use ext1 for external wakeup
    //esp_sleep_enable_ext1_wakeup(GPIO_SEL_35, ESP_EXT1_WAKEUP_ALL_LOW);

    esp_sleep_enable_timer_wakeup(10000000);

    esp_deep_sleep_start();
}


void StartLowPowerMode(){
  ShutDown();
}
