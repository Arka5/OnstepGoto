#pragma once
#include <Arduino.h>
#include "Pad.h"
#include "u8g2_ext.h"
#include "Telescope.h"
#include "LX200.h"

class SmartHandController
{
public:
  enum OLED { OLED_SH1106, OLED_SSD1306};
  void update();
  void drawIntro();
  void drawLoad();
  void drawReady();
  void setup(const int pin[7], const bool active[7], const int SerialBaud, const OLED model);
private:
  U8G2_EXT *display = NULL;
  Pad buttonPad;
  Telescope telInfo;

  void updateMainDisplay( u8g2_uint_t page);
  bool sleepDisplay = false;
  bool lowContrast = false;
  uint8_t maxContrast = 255;
  bool wifiOn = false;
  bool powerCylceRequired = false;
  bool buttonCommand = false;
  bool Move[4] = { false, false, false, false };

  unsigned long lastpageupdate = millis();
  unsigned long time_last_action = millis();

  byte page = 0;
  uint8_t current_selection_L0 = 1;
  uint8_t current_selection_L1 = 1;
  uint8_t current_selection_L2 = 1;
  uint8_t current_selection_L3 = 1;
  uint8_t current_selection_L4 = 1;
  uint8_t current_selection_speed = 5;
  unsigned short current_selection_Herschel = 1;
  unsigned short current_selection_Messier = 1;
  unsigned short current_selection_SolarSys = 1;
  unsigned short current_selection_Star = 1;
  long angleRA = 0;
  long angleDEC = 0;
  void tickButtons();
  bool buttonPressed();
  void menuMain();
  void menuSpeedRate();
  void menuTrack();
  void menuSyncGoto(bool sync);
  void menuSolarSys(bool sync);
  void menuHerschel(bool sync);
  void menuMessier(bool sync);
  void menuAlignment();
  void menuPier();
  void menuStar(bool sync);
  bool SelectStarAlign();
  void menuRADec(bool sync);
  void menuSettings();
  void menuMount();
  void menuMountType();
  void menuPredefinedMount();
  void menuAltMount();
  void menuMotor(uint8_t idx);
  void menuSite();
  void menuSites();
  void menuUTCTime();
  void menuDisplay();
  void menuContrast();
  void menuDate();
  void menuLatitude();
  void menuLongitude();
  void menuLimits();
  void menuWifi();
  void menuHorizon();
  void menuOverhead();
  void menuMeridian();

  bool menuSetStepperGearBox(uint8_t &axis, unsigned short &worm);
  bool menuSetReverse(uint8_t &axis);
  bool menuSetBacklash(uint8_t &axis);
  bool menuSetTotGear(uint8_t &axis);
  bool menuSetStepPerRot(uint8_t &axis);
  bool menuSetMicro(uint8_t &axis);
  bool menuSetLowCurrent(uint8_t &axis);
  bool menuSetHighCurrent(uint8_t &axis);
  void DisplayMotorSettings(uint8_t &axis);
  void DisplayMessage(const char* txt1, const char* txt2 = NULL, int duration = 0);
  void DisplayLongMessage(const char* txt1, const char* txt2 = NULL, const char* txt3 = NULL, const char* txt4 = NULL, int duration = 0);
  bool DisplayMessageLX200(LX200RETURN val, bool silentOk = true);
};
