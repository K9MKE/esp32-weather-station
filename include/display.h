#ifndef DISPLAY_H
#define DISPLAY_H

#include <TFT_eSPI.h>
#include "config.h"

extern TFT_eSPI tft;

// Function declarations
void displayInit();
void displaySplashScreen();
void displayMessage(String message);
void updateDisplay();
void update7DayForecastDisplay();
void updateHourlyForecastDisplay();
void updateAirQualityDisplay();
void updateAstronomyDisplay();
void updateSpaceWeatherDisplay();
void updateAuroraTodayDisplay();
void updateAuroraTomorrowDisplay();
void drawWeatherIcon(int x, int y, String iconCode);
void drawTemperature(int x, int y, float temp);
void drawHumidity(int x, int y, int humidity);
void drawPressure(int x, int y, float pressure);
void drawWind(int x, int y, float speed, int direction);
void drawBackground();
String formatTime(unsigned long timestamp);
void drawWeatherIconLarge(int x, int y, String iconCode);
void drawMoonPhase(int x, int y);
void drawMoonPhaseBitmap(int x, int y);
void drawSmallIcon(int x, int y, String iconType);
void drawStandardHeader(String title);
void drawUpdateTime(unsigned long lastUpdate);

#endif