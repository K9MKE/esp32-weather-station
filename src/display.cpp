#include "display.h"
#include "weather.h"
#include <WiFi.h>

extern TFT_eSPI tft;

void drawMoonPhaseBitmap(int x, int y) {
  // Draw accurate moon phase shapes based on the phase name
  tft.setTextDatum(MC_DATUM);
  
  // Draw different moon shapes based on phase
  if (currentWeather.moonPhaseName == "New Moon") {
    // New Moon - dark circle with light outline
    tft.fillCircle(x, y, 16, TFT_BLACK);
    tft.drawCircle(x, y, 16, TFT_DARKGREY);
    
  } else if (currentWeather.moonPhaseName == "Waxing Crescent") {
    // Waxing Crescent - dark circle with bright crescent on right side
    tft.fillCircle(x, y, 16, TFT_BLACK);
    tft.drawCircle(x, y, 16, TFT_DARKGREY);
    
    // Draw bright crescent on the right side
    for (int i = 0; i < 16; i++) {
      int yOffset = i - 8;
      int xWidth = sqrt(16*16 - yOffset*yOffset);
      int crescentWidth = xWidth / 3; // Crescent is about 1/3 of radius
      
      // Draw crescent pixels
      for (int j = 0; j < crescentWidth; j++) {
        tft.drawPixel(x + xWidth - j, y + yOffset, TFT_WHITE);
        tft.drawPixel(x + xWidth - j, y - yOffset, TFT_WHITE);
      }
    }
    
  } else if (currentWeather.moonPhaseName == "First Quarter") {
    // First Quarter - half moon (right half bright)
    tft.fillCircle(x, y, 16, TFT_BLACK);
    tft.drawCircle(x, y, 16, TFT_DARKGREY);
    
    // Fill right half with white
    for (int i = -16; i <= 16; i++) {
      for (int j = 0; j <= 16; j++) {
        if (i*i + j*j <= 16*16) {
          tft.drawPixel(x + j, y + i, TFT_WHITE);
        }
      }
    }
    
  } else if (currentWeather.moonPhaseName == "Waxing Gibbous") {
    // Waxing Gibbous - mostly bright with dark crescent on left
    tft.fillCircle(x, y, 16, TFT_WHITE);
    tft.drawCircle(x, y, 16, TFT_LIGHTGREY);
    
    // Draw dark crescent on the left side
    for (int i = 0; i < 16; i++) {
      int yOffset = i - 8;
      int xWidth = sqrt(16*16 - yOffset*yOffset);
      int crescentWidth = xWidth / 3;
      
      for (int j = 0; j < crescentWidth; j++) {
        tft.drawPixel(x - xWidth + j, y + yOffset, TFT_BLACK);
        tft.drawPixel(x - xWidth + j, y - yOffset, TFT_BLACK);
      }
    }
    
  } else if (currentWeather.moonPhaseName == "Full Moon") {
    // Full Moon - bright white circle
    tft.fillCircle(x, y, 16, TFT_WHITE);
    tft.drawCircle(x, y, 16, TFT_LIGHTGREY);
    
  } else if (currentWeather.moonPhaseName == "Waning Gibbous") {
    // Waning Gibbous - mostly bright with dark crescent on right
    tft.fillCircle(x, y, 16, TFT_WHITE);
    tft.drawCircle(x, y, 16, TFT_LIGHTGREY);
    
    // Draw dark crescent on the right side
    for (int i = 0; i < 16; i++) {
      int yOffset = i - 8;
      int xWidth = sqrt(16*16 - yOffset*yOffset);
      int crescentWidth = xWidth / 3;
      
      for (int j = 0; j < crescentWidth; j++) {
        tft.drawPixel(x + xWidth - j, y + yOffset, TFT_BLACK);
        tft.drawPixel(x + xWidth - j, y - yOffset, TFT_BLACK);
      }
    }
    
  } else if (currentWeather.moonPhaseName == "Last Quarter") {
    // Last Quarter - half moon (left half bright)
    tft.fillCircle(x, y, 16, TFT_BLACK);
    tft.drawCircle(x, y, 16, TFT_DARKGREY);
    
    // Fill left half with white
    for (int i = -16; i <= 16; i++) {
      for (int j = -16; j <= 0; j++) {
        if (i*i + j*j <= 16*16) {
          tft.drawPixel(x + j, y + i, TFT_WHITE);
        }
      }
    }
    
  } else if (currentWeather.moonPhaseName == "Waning Crescent") {
    // Waning Crescent - dark circle with bright crescent on left side
    tft.fillCircle(x, y, 16, TFT_BLACK);
    tft.drawCircle(x, y, 16, TFT_DARKGREY);
    
    // Draw bright crescent on the left side
    for (int i = 0; i < 16; i++) {
      int yOffset = i - 8;
      int xWidth = sqrt(16*16 - yOffset*yOffset);
      int crescentWidth = xWidth / 3;
      
      for (int j = 0; j < crescentWidth; j++) {
        tft.drawPixel(x - xWidth + j, y + yOffset, TFT_WHITE);
        tft.drawPixel(x - xWidth + j, y - yOffset, TFT_WHITE);
      }
    }
    
  } else {
    // Default fallback - simple circle
    tft.fillCircle(x, y, 16, TFT_LIGHTGREY);
    tft.drawCircle(x, y, 16, TFT_WHITE);
  }
  
  tft.setTextDatum(TL_DATUM); // Reset text alignment
}

void displayInit() {
  tft.fillScreen(COLOR_BACKGROUND);
  tft.setTextColor(COLOR_TEXT, COLOR_BACKGROUND);
}

void displaySplashScreen() {
  tft.fillScreen(COLOR_BACKGROUND);
  tft.setTextColor(COLOR_ACCENT, COLOR_BACKGROUND);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(2);
  tft.drawString("Weather Station", SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 20);
  tft.setTextSize(1);
  tft.setTextColor(COLOR_TEXT, COLOR_BACKGROUND);
  tft.drawString("ESP32 T-Display", SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + 10);
  delay(2000);
}

void displayMessage(String message) {
  tft.fillScreen(COLOR_BACKGROUND);
  tft.setTextColor(COLOR_TEXT, COLOR_BACKGROUND);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(1);
  tft.drawString(message, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
}

void updateDisplay() {
  static unsigned long lastDisplayUpdate = 0;
  static String lastDisplayedTime = "";
  
  // Update display every 5 seconds OR when time changes OR on startup OR when forced
  extern String currentTime;
  extern bool forceDisplayUpdate;
  bool timeChanged = (currentTime != lastDisplayedTime);
  static bool firstRun = true;
  
  if (millis() - lastDisplayUpdate < 5000 && !timeChanged && !firstRun && !forceDisplayUpdate) {
    return;
  }
  
  firstRun = false;
  lastDisplayedTime = currentTime;
  
  drawBackground();
  
  // Check if we have valid weather data
  if (currentWeather.cityName.length() > 0) {
    // Draw standardized header
    drawStandardHeader(currentWeather.cityName);
    
    // Draw temperature (large) - moved to LEFT side like moon position
    drawTemperature(5, 35, currentWeather.temperature);
    
    // Draw large weather icon - moved further RIGHT to avoid text interference
    drawWeatherIconLarge(150, 25, currentWeather.icon);
    
    // Draw main weather condition in large font - positioned below temperature
    tft.setTextColor(COLOR_ACCENT, COLOR_BACKGROUND);
    tft.setTextDatum(TL_DATUM);
    tft.setTextSize(2);
    
    // Get main weather condition (first word or simplified version)
    String mainCondition = currentWeather.description;
    if (mainCondition.indexOf("few clouds") >= 0 || mainCondition.indexOf("scattered clouds") >= 0) {
      mainCondition = "Partly Cloudy";
    } else if (mainCondition.indexOf("broken clouds") >= 0 || mainCondition.indexOf("overcast") >= 0) {
      mainCondition = "Cloudy";
    } else if (mainCondition.indexOf("clear") >= 0) {
      mainCondition = "Clear";
    } else if (mainCondition.indexOf("rain") >= 0) {
      mainCondition = "Rainy";
    } else if (mainCondition.indexOf("snow") >= 0) {
      mainCondition = "Snowy";
    } else if (mainCondition.indexOf("thunder") >= 0) {
      mainCondition = "Stormy";
    } else if (mainCondition.indexOf("mist") >= 0 || mainCondition.indexOf("fog") >= 0) {
      mainCondition = "Foggy";
    }
    
    tft.drawString(mainCondition, 5, 75);
    tft.setTextSize(1);
    
    // Draw other weather data - adjusted positions with more spacing
    drawHumidity(5, 95, currentWeather.humidity);  // Moved down to 95
    drawPressure(120, 105, currentWeather.pressure); // Aligned with wind at y=105
    drawWind(5, 105, currentWeather.windSpeed, currentWeather.windDirection); // Moved up from 110 to 105
    
    // Add standardized update time and WiFi status
    drawUpdateTime(currentWeather.lastUpdate);
    
  } else {
    // No weather data available
    tft.setTextColor(COLOR_TEXT, COLOR_BACKGROUND);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("Waiting for weather data...", SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
  }
  
  lastDisplayUpdate = millis();
}

void drawBackground() {
  // Clear screen with solid background
  tft.fillScreen(COLOR_BACKGROUND);
}

void drawWeatherIcon(int x, int y, String iconCode) {
  // Simple ASCII text-based weather icons
  tft.setTextColor(COLOR_ACCENT, COLOR_BACKGROUND);
  tft.setTextDatum(TL_DATUM);
  tft.setTextSize(1);
  
  if (iconCode.startsWith("01")) {
    tft.drawString("SUNNY", x, y);  // Clear sky
  } else if (iconCode.startsWith("02") || iconCode.startsWith("03")) {
    tft.drawString("PARTLY", x, y);  // Few clouds / Scattered clouds
  } else if (iconCode.startsWith("04")) {
    tft.drawString("CLOUDY", x, y);  // Broken clouds
  } else if (iconCode.startsWith("09") || iconCode.startsWith("10")) {
    tft.drawString("RAINY", x, y);  // Rain
  } else if (iconCode.startsWith("11")) {
    tft.drawString("STORM", x, y);  // Thunderstorm
  } else if (iconCode.startsWith("13")) {
    tft.drawString("SNOW", x, y);  // Snow
  } else if (iconCode.startsWith("50")) {
    tft.drawString("MISTY", x, y);  // Mist
  } else {
    tft.drawString("UNKNOWN", x, y);   // Unknown
  }
}

void drawTemperature(int x, int y, float temp) {
  tft.setTextColor(COLOR_TEMP, COLOR_BACKGROUND);
  tft.setTextDatum(TL_DATUM);
  tft.setTextSize(5);  // Increased from 4 to 5 for maximum readability
  
  String tempStr = String((int)temp) + "F";
  tft.drawString(tempStr, x, y);
  
  tft.setTextSize(1);
}

void drawHumidity(int x, int y, int humidity) {
  tft.setTextColor(COLOR_HUMIDITY, COLOR_BACKGROUND);
  tft.setTextDatum(TL_DATUM);
  tft.setTextSize(1);
  
  String humidityStr = "H: " + String(humidity) + "%";
  tft.drawString(humidityStr, x, y);
}

void drawPressure(int x, int y, float pressure) {
  tft.setTextColor(COLOR_PRESSURE, COLOR_BACKGROUND);
  tft.setTextDatum(TL_DATUM);
  tft.setTextSize(1);
  
  // Convert hPa to inches of mercury (1 hPa = 0.02953 inHg)
  float pressureInHg = pressure * 0.02953;
  String pressureStr = "P: " + String(pressureInHg, 2) + "inHg";
  tft.drawString(pressureStr, x, y);
}

void drawWind(int x, int y, float speed, int direction) {
  tft.setTextColor(COLOR_WIND, COLOR_BACKGROUND);
  tft.setTextDatum(TL_DATUM);
  tft.setTextSize(1);
  
  // Convert m/s to mph (1 m/s = 2.237 mph)
  float speedMPH = speed * 2.237;
  String windStr = "W: " + String(speedMPH, 1) + "mph " + String(direction) + "deg";
  tft.drawString(windStr, x, y);
}

void updateAstronomyDisplay() {
  static unsigned long lastAstronomyUpdate = 0;
  static String lastAstronomyTime = "";
  
  // Update display every 5 seconds OR when time changes OR on startup OR when forced
  extern String currentTime;
  extern bool forceDisplayUpdate;
  bool timeChanged = (currentTime != lastAstronomyTime);
  static bool firstAstronomyRun = true;
  
  if (millis() - lastAstronomyUpdate < 5000 && !timeChanged && !firstAstronomyRun && !forceDisplayUpdate) {
    return;
  }
  
  firstAstronomyRun = false;
  lastAstronomyTime = currentTime;
  
  drawBackground();
  
  // Draw standardized header
  drawStandardHeader("ASTRONOMY");
  
  lastAstronomyUpdate = millis();
  
  tft.setTextSize(1);
  tft.setTextDatum(TL_DATUM);
  
  // Check if we have astronomical data
  if (currentWeather.sunrise > 0) {
    // Use larger text size for better readability
    tft.setTextSize(2);
    
    // Sunrise - aligned with moonrise
    drawSmallIcon(5, 40, "sunrise");
    tft.setTextColor(COLOR_TEMP, COLOR_BACKGROUND);
    tft.drawString("Sunrise:", 25, 40);
    tft.setTextColor(COLOR_TEXT, COLOR_BACKGROUND);
    tft.drawString(formatTime(currentWeather.sunrise), 25, 55);
    
    // Sunset - aligned with moonset
    drawSmallIcon(5, 75, "sunset");
    tft.setTextColor(COLOR_TEMP, COLOR_BACKGROUND);
    tft.drawString("Sunset:", 25, 75);
    tft.setTextColor(COLOR_TEXT, COLOR_BACKGROUND);
    tft.drawString(formatTime(currentWeather.sunset), 25, 90);
    
    // Right column for moon data - aligned with sun data
    tft.setTextSize(2); // Match sunrise/sunset text size
    
    // Moonrise (if available) - aligned with sunrise
    if (currentWeather.moonrise > 0) {
      drawSmallIcon(160, 40, "moonrise");
      tft.setTextColor(COLOR_HUMIDITY, COLOR_BACKGROUND);
      tft.drawString("Moonrise:", 180, 40);
      tft.setTextColor(COLOR_TEXT, COLOR_BACKGROUND);
      tft.drawString(formatTime(currentWeather.moonrise), 180, 55);
    }
    
    // Moonset (if available) - aligned with sunset
    if (currentWeather.moonset > 0) {
      drawSmallIcon(160, 75, "moonset");
      tft.setTextColor(COLOR_HUMIDITY, COLOR_BACKGROUND);
      tft.drawString("Moonset:", 180, 75);
      tft.setTextColor(COLOR_TEXT, COLOR_BACKGROUND);
      tft.drawString(formatTime(currentWeather.moonset), 180, 90);
    }
    
    // Moon phase display - positioned between sunrise/sunset and moonrise/moonset
    if (currentWeather.moonPhaseName.length() > 0) {
      Serial.println("Displaying simple moon graphic for phase: " + currentWeather.moonPhaseName);
      drawMoonPhaseBitmap(115, 60); // Centered horizontally, positioned between sun and moon times
    } else {
      // Fallback to simple graphic moon
      Serial.println("No moon data available, using fallback graphic");
      drawMoonPhase(115, 60);
    }
    
    // Moon phase information below the moon graphic
    tft.setTextSize(1);
    tft.setTextColor(COLOR_HUMIDITY, COLOR_BACKGROUND);
    tft.setTextDatum(TC_DATUM); // Top center alignment
    
    // Calculate illumination percentage
    float illumination;
    if (currentWeather.moonIllumination > 0) {
      illumination = currentWeather.moonIllumination;
    } else {
      illumination = 0.5 * (1 - cos(2 * PI * currentWeather.moonPhase)) * 100;
    }
    
    // Display phase name
    String phaseName;
    if (currentWeather.moonPhaseName.length() > 0) {
      phaseName = currentWeather.moonPhaseName;
    } else {
      phaseName = "Unknown Phase";
    }
    
    // Moon phase name - positioned below moon graphic
    tft.drawString(phaseName, 115, 95);
    
    // Illumination text - positioned below phase name
    String illuminationText = String((int)illumination) + "% lit";
    tft.drawString(illuminationText, 115, 105);
    
    tft.setTextDatum(TL_DATUM); // Reset text alignment
    
    // Reset text size
    tft.setTextSize(1);
    
    // Add standardized update time and WiFi status
    drawUpdateTime(currentWeather.lastUpdate);
    
  } else {
    tft.setTextColor(COLOR_TEXT, COLOR_BACKGROUND);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("No astronomy data available", 120, 120);
    tft.setTextDatum(TL_DATUM);
  }
}

String formatTime(unsigned long timestamp) {
  // The timestamp from OpenWeatherMap is already in UTC
  // We need to convert it to local time
  // Adjust timezone offset as needed for your location
  // Let's assume daylight saving time for now (October should be CDT)
  timestamp -= 5 * 3600; // Adjust for Central Daylight Time (UTC-5)
  
  int hours = (timestamp % 86400) / 3600;
  int minutes = (timestamp % 3600) / 60;
  
  String ampm = hours >= 12 ? "PM" : "AM";
  if (hours > 12) hours -= 12;
  if (hours == 0) hours = 12;
  
  String timeStr = String(hours) + ":";
  if (minutes < 10) timeStr += "0";
  timeStr += String(minutes) + ampm;
  
  return timeStr;
}

void drawWeatherIconLarge(int x, int y, String iconCode) {
  // Draw larger, more realistic weather icons (80x80px) with day/night logic
  
  // Check if it's nighttime (after sunset or before sunrise)
  extern WeatherData currentWeather;
  
  // Get current time from system
  time_t now;
  time(&now);
  
  // Debug output
  Serial.print("Current time: "); Serial.println(now);
  Serial.print("Sunrise: "); Serial.println(currentWeather.sunrise);
  Serial.print("Sunset: "); Serial.println(currentWeather.sunset);
  Serial.print("Icon code: "); Serial.println(iconCode);
  
  // Convert to readable times for debug
  if (currentWeather.sunrise > 0) {
    struct tm* sr = localtime((time_t*)&currentWeather.sunrise);
    Serial.printf("Sunrise: %02d:%02d\n", sr->tm_hour, sr->tm_min);
  }
  if (currentWeather.sunset > 0) {
    struct tm* ss = localtime((time_t*)&currentWeather.sunset);
    Serial.printf("Sunset: %02d:%02d\n", ss->tm_hour, ss->tm_min);
  }
  struct tm* ct = localtime(&now);
  Serial.printf("Current: %02d:%02d\n", ct->tm_hour, ct->tm_min);
  
  // Check if current time is between sunset and sunrise (nighttime)
  bool isNightTime = false;
  if (currentWeather.sunset > 0 && currentWeather.sunrise > 0) {
    // Simple day/night logic: night if before sunrise OR after sunset
    isNightTime = (now < currentWeather.sunrise || now > currentWeather.sunset);
  }
  
  Serial.print("Is night time: "); Serial.println(isNightTime ? "YES" : "NO");
  
  // Strip the day/night indicator from icon code for consistent logic
  String baseIcon = iconCode;
  if (baseIcon.length() >= 3) {
    baseIcon = baseIcon.substring(0, 2); // Remove 'd' or 'n' suffix
  }
  
  Serial.print("Base icon code: "); Serial.println(baseIcon);
  
  // FORCE DAY MODE FOR TESTING - Override the night logic using actual sunrise/sunset times
  if (currentWeather.sunrise > 0 && currentWeather.sunset > 0) {
    // Use actual sunrise/sunset times for forced day mode
    if (now >= currentWeather.sunrise && now <= currentWeather.sunset) {
      isNightTime = false;
      Serial.println("FORCED DAY MODE (Between actual sunrise and sunset)");
    }
  }
  
  if (baseIcon.startsWith("01")) {
    // Clear sky - Sun during day, Moon at night
    if (isNightTime) {
      // Night - realistic moon with craters and stars
      tft.fillCircle(x+40, y+40, 22, 0xDEFB);  // Light gray moon
      // Craters for realism
      tft.fillCircle(x+35, y+35, 4, 0xC618);   // Crater 1
      tft.fillCircle(x+45, y+42, 3, 0xC618);   // Crater 2
      tft.fillCircle(x+38, y+45, 2, 0xC618);   // Crater 3
      // Scattered stars
      tft.fillCircle(x+15, y+20, 1, COLOR_TEXT);
      tft.fillCircle(x+65, y+25, 1, COLOR_TEXT);
      tft.fillCircle(x+20, y+65, 1, COLOR_TEXT);
      tft.fillCircle(x+70, y+60, 1, COLOR_TEXT);
    } else {
      // Day - Realistic sun with radiating rays
      tft.fillCircle(x+40, y+40, 22, 0xFD20);  // Large orange sun center
      tft.fillCircle(x+40, y+40, 18, 0xFFE0);  // Inner yellow glow
      // Draw 12 sun rays for more realism
      for (int i = 0; i < 12; i++) {
        float angle = i * 30 * PI / 180;
        int rayX1 = x + 40 + cos(angle) * 28;
        int rayY1 = y + 40 + sin(angle) * 28;
        int rayX2 = x + 40 + cos(angle) * 36;
        int rayY2 = y + 40 + sin(angle) * 36;
        // Thick rays
        tft.drawLine(rayX1, rayY1, rayX2, rayY2, 0xFFE0);
        tft.drawLine(rayX1+1, rayY1, rayX2+1, rayY2, 0xFFE0);
        tft.drawLine(rayX1, rayY1+1, rayX2, rayY2+1, 0xFFE0);
      }
    }
    
  } else if (iconCode.startsWith("02") || iconCode.startsWith("03")) {
    // Few/scattered clouds - More realistic fluffy clouds
    if (isNightTime) {
      tft.fillCircle(x+22, y+22, 16, 0x7BEF);  // Moon behind clouds
    } else {
      tft.fillCircle(x+22, y+22, 16, 0xFD20);  // Sun behind clouds
    }
    // Realistic puffy cloud shapes
    tft.fillCircle(x+40, y+40, 16, COLOR_TEXT);   // Main cloud body
    tft.fillCircle(x+50, y+35, 12, COLOR_TEXT);   // Cloud puff 1
    tft.fillCircle(x+32, y+35, 10, COLOR_TEXT);   // Cloud puff 2
    tft.fillCircle(x+45, y+48, 10, COLOR_TEXT);   // Cloud puff 3
    tft.fillCircle(x+35, y+50, 8, COLOR_TEXT);    // Cloud puff 4
    
  } else if (iconCode.startsWith("04")) {
    // Broken/overcast clouds - Layered realistic cloud cover
    // Multiple overlapping cloud layers for depth
    tft.fillCircle(x+25, y+30, 16, 0xC618);      // Back cloud layer
    tft.fillCircle(x+40, y+25, 15, 0xC618);      // Mid cloud layer
    tft.fillCircle(x+55, y+30, 16, 0xC618);      // Front cloud layer
    tft.fillCircle(x+32, y+45, 14, 0xC618);      // Lower cloud layer
    tft.fillCircle(x+48, y+45, 14, 0xC618);      // Lower cloud layer
    // Add some darker areas for depth
    tft.fillCircle(x+35, y+35, 8, 0xA534);       // Shadow 1
    tft.fillCircle(x+50, y+40, 6, 0xA534);       // Shadow 2
    
  } else if (iconCode.startsWith("09") || iconCode.startsWith("10")) {
    // Rain - Dark storm clouds with realistic rain
    tft.fillCircle(x+32, y+25, 20, 0x7BEF);      // Large dark cloud
    tft.fillCircle(x+48, y+32, 16, 0x7BEF);      // Overlapping cloud
    tft.fillCircle(x+25, y+38, 12, 0x5AEB);      // Very dark area
    // Realistic rain drops in varying sizes
    for (int i = 0; i < 15; i++) {
      int dropX = x + 20 + i*3;
      int dropY = y + 55 + (i%3)*2;
      tft.fillCircle(dropX, dropY, 1, 0x07FF);   // Rain drops
      tft.fillCircle(dropX+1, dropY+8, 1, 0x07FF); // Staggered drops
    }
    
  } else if (iconCode.startsWith("11")) {
    // Thunderstorm - Dramatic storm cloud with lightning
    tft.fillCircle(x+32, y+25, 20, 0x39E7);      // Very dark storm cloud
    tft.fillCircle(x+48, y+32, 16, 0x39E7);      // Overlapping cloud
    tft.fillCircle(x+25, y+38, 12, 0x2124);      // Almost black center
    // Realistic zigzag lightning bolt
    tft.drawLine(x+40, y+50, x+35, y+60, 0xFFE0);
    tft.drawLine(x+35, y+60, x+42, y+68, 0xFFE0);
    tft.drawLine(x+42, y+68, x+38, y+75, 0xFFE0);
    // Thicker lightning for visibility
    tft.drawLine(x+41, y+50, x+36, y+60, 0xFFE0);
    tft.drawLine(x+36, y+60, x+43, y+68, 0xFFE0);
    
  } else if (iconCode.startsWith("13")) {
    // Snow - Light clouds with detailed snowflakes
    tft.fillCircle(x+32, y+25, 20, 0xDEFB);      // Light gray cloud
    tft.fillCircle(x+48, y+32, 16, 0xDEFB);      // Overlapping cloud
    // Detailed snowflake patterns
    for (int i = 0; i < 12; i++) {
      int snowX = x + 22 + i*4;
      int snowY = y + 55 + (i%4)*3;
      // Draw detailed snowflake (6-pointed star)
      tft.drawPixel(snowX, snowY, COLOR_TEXT);     // Center
      tft.drawPixel(snowX-2, snowY, COLOR_TEXT);   // Left
      tft.drawPixel(snowX+2, snowY, COLOR_TEXT);   // Right
      tft.drawPixel(snowX, snowY-2, COLOR_TEXT);   // Top
      tft.drawPixel(snowX, snowY+2, COLOR_TEXT);   // Bottom
      tft.drawPixel(snowX-1, snowY-1, COLOR_TEXT); // Diagonal
      tft.drawPixel(snowX+1, snowY+1, COLOR_TEXT); // Diagonal
    }
    
  } else if (iconCode.startsWith("50")) {
    // Mist/Fog - Layered atmospheric effect
    for (int i = 0; i < 8; i++) {
      int layerY = y + 25 + i*7;
      int opacity = 0xBDF7 - (i * 0x1000); // Fading effect
      tft.drawFastHLine(x+10, layerY, 60, opacity);
      tft.drawFastHLine(x+15, layerY+2, 50, opacity);
      tft.drawFastHLine(x+12, layerY+4, 56, opacity);
    }
    
  } else {
    // Unknown - Larger question mark
    tft.fillRect(x+25, y+25, 30, 30, COLOR_TEXT);
    tft.setTextColor(COLOR_BACKGROUND, COLOR_TEXT);
    tft.setTextDatum(MC_DATUM);
    tft.setTextSize(3);
    tft.drawString("?", x+40, y+40);
    tft.setTextSize(1);
  }
  
  tft.setTextDatum(TL_DATUM);
}

void drawMoonPhase(int x, int y) {
  // Calculate moon phase with realistic lunar cycle
  extern WeatherData currentWeather;
  unsigned long currentTime = millis() / 1000 + 1697900000;
  unsigned long daysSinceNewMoon = (currentTime / 86400) % 29;
  float phase = (float)daysSinceNewMoon / 29.0; // 0 = new moon, 0.5 = full moon
  
  // Draw realistic moon phases with proper lighting effects (50x50px)
  int radius = 22;
  int centerX = x;
  int centerY = y;
  
  // Base moon with realistic lunar gray color
  tft.fillCircle(centerX, centerY, radius, 0xDEFB);
  
  // Add subtle surface details (craters and maria)
  tft.fillCircle(centerX-6, centerY-4, 2, 0xC618);   // Mare Tranquillitatis
  tft.fillCircle(centerX+5, centerY-6, 1, 0xC618);   // Crater Tycho
  tft.fillCircle(centerX-2, centerY+5, 1, 0xC618);   // Crater Copernicus
  tft.fillCircle(centerX+8, centerY+3, 1, 0xC618);   // Small crater
  tft.fillCircle(centerX-9, centerY+1, 1, 0xC618);   // Small crater
  
  // Phase-based lighting with realistic shadows
  if (phase < 0.02 || phase > 0.98) {
    // New Moon - Almost completely dark with thin crescent
    tft.fillCircle(centerX, centerY, radius-1, 0x2104);  // Very dark
    // Barely visible edge
    tft.drawCircle(centerX, centerY, radius, 0x4208);
    
  } else if (phase < 0.25) {
    // Waxing Crescent - Realistic curved shadow
    int shadowWidth = (int)(radius * 2 * (0.25 - phase) / 0.25);
    for (int dy = -radius; dy <= radius; dy++) {
      for (int dx = -radius; dx <= radius; dx++) {
        if (dx*dx + dy*dy <= radius*radius) {
          int moonX = centerX + dx;
          int moonY = centerY + dy;
          
          // Create curved shadow boundary (right side dark)
          if (dx > radius - shadowWidth) {
            tft.drawPixel(moonX, moonY, 0x2104); // Dark shadow
          }
        }
      }
    }
    
  } else if (phase < 0.48) {
    // First Quarter - Exact half moon with terminator
    for (int dy = -radius; dy <= radius; dy++) {
      for (int dx = 0; dx <= radius; dx++) {  // Only right half illuminated
        if (dx*dx + dy*dy <= radius*radius) {
          tft.drawPixel(centerX + dx, centerY + dy, 0xF7BE); // Bright side
        }
      }
    }
    // Left side in shadow
    for (int dy = -radius; dy <= radius; dy++) {
      for (int dx = -radius; dx < 0; dx++) {
        if (dx*dx + dy*dy <= radius*radius) {
          tft.drawPixel(centerX + dx, centerY + dy, 0x2104); // Dark side
        }
      }
    }
    
  } else if (phase < 0.52) {
    // Full Moon - Bright with enhanced surface details
    tft.fillCircle(centerX, centerY, radius, 0xF7BE);  // Bright lunar white
    // Enhanced crater details for full moon visibility
    tft.fillCircle(centerX-6, centerY-4, 3, 0xDEFB);   // Large Mare
    tft.fillCircle(centerX+5, centerY-6, 2, 0xDEFB);   // Tycho crater
    tft.fillCircle(centerX-2, centerY+5, 2, 0xDEFB);   // Copernicus
    tft.fillCircle(centerX+9, centerY+5, 1, 0xDEFB);   // Crater rim
    tft.fillCircle(centerX-11, centerY-7, 1, 0xDEFB);  // Highland crater
    // Subtle rays from major craters
    for (int i = 0; i < 8; i++) {
      float angle = i * 45 * PI / 180;
      int rayX = centerX + cos(angle) * 15;
      int rayY = centerY + sin(angle) * 15;
      tft.drawPixel(rayX, rayY, 0xFFFF);
    }
    
  } else if (phase < 0.75) {
    // Waning Gibbous - Shadow starts from right
    int shadowWidth = (int)(radius * 2 * (phase - 0.52) / 0.23);
    for (int dy = -radius; dy <= radius; dy++) {
      for (int dx = -radius; dx <= radius; dx++) {
        if (dx*dx + dy*dy <= radius*radius) {
          int moonX = centerX + dx;
          int moonY = centerY + dy;
          
          // Shadow creeping from right side
          if (dx > radius - shadowWidth) {
            tft.drawPixel(moonX, moonY, 0x2104); // Dark shadow
          } else {
            tft.drawPixel(moonX, moonY, 0xF7BE); // Illuminated portion
          }
        }
      }
    }
    
  } else {
    // Waning Crescent - Thin illuminated crescent on left
    int lightWidth = (int)(radius * 2 * (1.0 - phase) / 0.25);
    for (int dy = -radius; dy <= radius; dy++) {
      for (int dx = -radius; dx <= radius; dx++) {
        if (dx*dx + dy*dy <= radius*radius) {
          int moonX = centerX + dx;
          int moonY = centerY + dy;
          
          // Illuminated crescent on left side only
          if (dx < -radius + lightWidth) {
            tft.drawPixel(moonX, moonY, 0xF7BE); // Bright crescent
          } else {
            tft.drawPixel(moonX, moonY, 0x2104); // Dark portion
          }
        }
      }
    }
  }
  
  // Add subtle glow around the moon for realism
  tft.drawCircle(centerX, centerY, radius + 1, 0x5AEB);
  tft.drawCircle(centerX, centerY, radius + 2, 0x39E7);
}

void drawSmallIcon(int x, int y, String iconType) {
  tft.setTextSize(1);
  tft.setTextDatum(TL_DATUM);
  
  if (iconType == "sunrise") {
    // Draw larger sun rising over horizon - bright orange
    tft.setTextColor(0xFD20, COLOR_BACKGROUND); // Orange color
    tft.fillCircle(x+6, y, 4, 0xFD20);     // Larger sun circle
    tft.setTextColor(COLOR_WIND, COLOR_BACKGROUND);
    tft.drawFastHLine(x+1, y+6, 10, COLOR_WIND);    // Longer horizon line
  } else if (iconType == "sunset") {
    // Draw larger sun setting below horizon - bright red/orange
    tft.setTextColor(COLOR_WIND, COLOR_BACKGROUND);
    tft.drawFastHLine(x+1, y+3, 10, COLOR_WIND);    // Horizon line
    tft.setTextColor(0xF800, COLOR_BACKGROUND); // Red color for sunset
    tft.fillCircle(x+6, y+9, 4, 0xF800);     // Larger sun circle below
  } else if (iconType == "moonrise") {
    // Draw larger crescent moon rising - light blue/cyan
    tft.setTextColor(COLOR_WIND, COLOR_BACKGROUND);
    tft.drawFastHLine(x+1, y+6, 10, COLOR_WIND);    // Horizon line
    tft.setTextColor(COLOR_HUMIDITY, COLOR_BACKGROUND);
    tft.drawCircle(x+6, y, 4, COLOR_HUMIDITY); // Larger moon outline
    tft.fillCircle(x+4, y, 3, COLOR_HUMIDITY); // Crescent fill
  } else if (iconType == "moonset") {
    // Draw larger moon setting below horizon - light blue/cyan
    tft.setTextColor(COLOR_WIND, COLOR_BACKGROUND);
    tft.drawFastHLine(x+1, y+3, 10, COLOR_WIND);    // Horizon line
    tft.setTextColor(COLOR_HUMIDITY, COLOR_BACKGROUND);
    tft.drawCircle(x+6, y+9, 4, COLOR_HUMIDITY); // Larger moon outline below
    tft.fillCircle(x+4, y+9, 3, COLOR_HUMIDITY); // Crescent fill
  }
}

void updateSpaceWeatherDisplay() {
  static unsigned long lastSpaceWeatherUpdate = 0;
  static String lastSpaceWeatherTime = "";
  
  // Update display every 5 seconds OR when time changes OR on startup OR when forced
  extern String currentTime;
  extern bool forceDisplayUpdate;
  extern SpaceWeatherData currentSpaceWeather;
  bool timeChanged = (currentTime != lastSpaceWeatherTime);
  static bool firstSpaceWeatherRun = true;
  
  if (millis() - lastSpaceWeatherUpdate < 5000 && !timeChanged && !firstSpaceWeatherRun && !forceDisplayUpdate) {
    return;
  }
  
  firstSpaceWeatherRun = false;
  lastSpaceWeatherTime = currentTime;
  
  drawBackground();
  
  // Draw standardized header
  drawStandardHeader("SPACE WEATHER");
  
  lastSpaceWeatherUpdate = millis();
  
  tft.setTextSize(1);
  tft.setTextDatum(TL_DATUM);
  
  // Left Column - Aurora Data
  // KP Index (most important for aurora watchers)
  tft.setTextColor(COLOR_TEMP, COLOR_BACKGROUND);
  tft.drawString("Kp:", 5, 35);
  String kpStr = String(currentSpaceWeather.kpIndex, 1);
  
  // Color code KP index
  uint16_t kpColor = COLOR_WIND; // Green for quiet
  if (currentSpaceWeather.kpIndex >= 7) {
    kpColor = 0xF800; // Red for storm
  } else if (currentSpaceWeather.kpIndex >= 5) {
    kpColor = 0xFD20; // Orange for active
  } else if (currentSpaceWeather.kpIndex >= 3) {
    kpColor = 0xFFE0; // Yellow for unsettled
  }
  
  tft.setTextColor(kpColor, COLOR_BACKGROUND);
  tft.drawString(kpStr, 25, 35);
  
  // Aurora Forecast (most important!)
  tft.setTextSize(1);
  tft.setTextColor(COLOR_HUMIDITY, COLOR_BACKGROUND);
  tft.drawString("Aurora:", 5, 50);
  
  // Color code aurora forecast
  uint16_t auroraColor = COLOR_WIND; // Green for minimal
  if (currentSpaceWeather.auroraForecast == "HIGH") {
    auroraColor = 0xF800; // Red for high
  } else if (currentSpaceWeather.auroraForecast == "MODERATE") {
    auroraColor = 0xFD20; // Orange for moderate
  } else if (currentSpaceWeather.auroraForecast == "LOW") {
    auroraColor = 0xFFE0; // Yellow for low
  }
  
  tft.setTextColor(auroraColor, COLOR_BACKGROUND);
  tft.drawString(currentSpaceWeather.auroraForecast, 45, 50);
  
  // Bz Field (critical for aurora)
  tft.setTextColor(COLOR_WIND, COLOR_BACKGROUND);
  tft.drawString("Bz:", 5, 65);
  
  // Color code Bz (negative is good for aurora)
  uint16_t bzColor = COLOR_WIND;
  if (currentSpaceWeather.magneticFieldBz < -10) {
    bzColor = 0xF800; // Red for very favorable
  } else if (currentSpaceWeather.magneticFieldBz < -5) {
    bzColor = 0xFD20; // Orange for favorable
  } else if (currentSpaceWeather.magneticFieldBz < 0) {
    bzColor = 0xFFE0; // Yellow for somewhat favorable
  }
  
  tft.setTextColor(bzColor, COLOR_BACKGROUND);
  tft.drawString(String(currentSpaceWeather.magneticFieldBz, 1) + "nT", 25, 65);
  
  // Solar Wind Speed
  tft.setTextColor(COLOR_PRESSURE, COLOR_BACKGROUND);
  tft.drawString("SW:", 5, 80);
  tft.setTextColor(COLOR_TEXT, COLOR_BACKGROUND);
  tft.drawString(String(currentSpaceWeather.solarWindSpeed, 0), 25, 80);
  
  // Right Column - Solar Data (NOAA)
  // Solar Flux Index
  tft.setTextColor(COLOR_TEMP, COLOR_BACKGROUND);
  tft.drawString("SFI:", 120, 35);
  tft.setTextColor(COLOR_TEXT, COLOR_BACKGROUND);
  tft.drawString(String(noaaSpaceWeather.solarFluxIndex, 0), 145, 35);
  
  // X-ray Class
  tft.setTextColor(COLOR_ACCENT, COLOR_BACKGROUND);
  tft.drawString("X-ray:", 120, 50);
  tft.setTextColor(COLOR_TEXT, COLOR_BACKGROUND);
  tft.drawString(noaaSpaceWeather.xrayFlux, 155, 50);
  
  // A Index
  tft.setTextColor(COLOR_HUMIDITY, COLOR_BACKGROUND);
  tft.drawString("A-idx:", 120, 65);
  tft.setTextColor(COLOR_TEXT, COLOR_BACKGROUND);
  tft.drawString(String(noaaSpaceWeather.aIndex, 0), 155, 65);
  
  // Sunspots
  tft.setTextColor(COLOR_PRESSURE, COLOR_BACKGROUND);
  tft.drawString("Spots:", 120, 80);
  tft.setTextColor(COLOR_TEXT, COLOR_BACKGROUND);
  tft.drawString(String(noaaSpaceWeather.sunspotNumber), 160, 80);
  
  // Alerts (if any)
  if (noaaSpaceWeather.alertCount > 0) {
    tft.setTextColor(0xF800, COLOR_BACKGROUND); // Red
    tft.drawString("ALERTS: " + String(noaaSpaceWeather.alertCount), 5, 95);
  } else {
    tft.setTextColor(COLOR_WIND, COLOR_BACKGROUND); // Green
    tft.drawString("No Alerts", 5, 95);
  }
  
  // Last update time
  drawUpdateTime(currentSpaceWeather.lastUpdate);
  
  tft.setTextDatum(TL_DATUM);
}

void update7DayForecastDisplay() {
  static unsigned long last7DayUpdate = 0;
  static String last7DayTime = "";
  
  // Update display every 5 seconds OR when time changes OR on startup OR when forced
  extern String currentTime;
  extern bool forceDisplayUpdate;
  extern WeeklyForecast weeklyForecast;
  bool timeChanged = (currentTime != last7DayTime);
  static bool first7DayRun = true;
  
  if (millis() - last7DayUpdate < 5000 && !timeChanged && !first7DayRun && !forceDisplayUpdate) {
    return;
  }
  
  first7DayRun = false;
  last7DayTime = currentTime;
  
  drawBackground();
  
  // Draw standardized header
  drawStandardHeader("7-DAY FORECAST");
  
  // Column Headers - positioned below header line
  tft.setTextSize(1);
  tft.setTextDatum(TL_DATUM);
  tft.setTextColor(COLOR_ACCENT, COLOR_BACKGROUND);
  tft.drawString("Day", 5, 30);
  tft.drawString("Lo/Hi", 35, 30);  
  tft.drawString("Conditions", 75, 30);
  tft.drawString("Rain", 200, 30);  // Moved further right
  
  last7DayUpdate = millis();
  
  // Display all 7 days with spacing to fit above bottom line (y=115)
  for (int i = 0; i < 7; i++) {
    int yPos = 45 + (i * 10);  // 10 pixel spacing fits 7 days (45 to 105)
    
    // Day name - truncated to fit
    tft.setTextColor(COLOR_TEXT, COLOR_BACKGROUND);
    String dayName = weeklyForecast.days[i].dayName;
    if (dayName == "Tomorrow") dayName = "Tom";  // Shorten to prevent overlap
    if (dayName.length() > 5) dayName = dayName.substring(0, 5);
    tft.drawString(dayName, 5, yPos);
    
    // Low/High temps - swapped order as requested
    tft.setTextColor(COLOR_TEMP, COLOR_BACKGROUND);
    String tempStr = String((int)weeklyForecast.days[i].tempLow) + "/" + String((int)weeklyForecast.days[i].tempHigh);
    tft.drawString(tempStr, 35, yPos);
    
    // Weather description - allow more text to show
    tft.setTextColor(COLOR_TEXT, COLOR_BACKGROUND);
    String desc = weeklyForecast.days[i].description;
    if (desc.length() > 16) desc = desc.substring(0, 16); // More text allowed
    tft.drawString(desc, 75, yPos);
    
    // Rain percentage moved further right
    if (weeklyForecast.days[i].precipChance > 0) {
      tft.setTextColor(COLOR_HUMIDITY, COLOR_BACKGROUND);
      tft.drawString(String(weeklyForecast.days[i].precipChance) + "%", 200, yPos);  // Moved right
    }
  }
  
  // Last update
  drawUpdateTime(weeklyForecast.lastUpdate);
  
  tft.setTextDatum(TL_DATUM);
}

void updateAuroraTodayDisplay() {
  static unsigned long lastAuroraTodayUpdate = 0;
  static String lastAuroraTodayTime = "";
  
  // Update display every 5 seconds OR when time changes OR on startup OR when forced
  extern String currentTime;
  extern bool forceDisplayUpdate;
  extern AuroraForecastData auroraToday;
  bool timeChanged = (currentTime != lastAuroraTodayTime);
  static bool firstAuroraTodayRun = true;
  
  if (millis() - lastAuroraTodayUpdate < 5000 && !timeChanged && !firstAuroraTodayRun && !forceDisplayUpdate) {
    return;
  }
  
  firstAuroraTodayRun = false;
  lastAuroraTodayTime = currentTime;
  
  drawBackground();
  
  // Draw standardized header
  drawStandardHeader("AURORA TODAY");
  
  lastAuroraTodayUpdate = millis();
  
  tft.setTextSize(1);
  tft.setTextDatum(TL_DATUM);
  
  // Current date and forecast timeframe - moved down to avoid blue line
  time_t now = time(nullptr);
  if (now < 1000000000) { // If time is not set properly, use approximate current time
    now = millis() / 1000 + 1729800000; // Approximate Oct 2024 timestamp
  }
  struct tm* timeinfo = localtime(&now);
  char dateBuffer[20];
  strftime(dateBuffer, sizeof(dateBuffer), "%m/%d/%Y", timeinfo);
  
  tft.setTextColor(COLOR_ACCENT, COLOR_BACKGROUND);
  tft.drawString("Today: " + String(dateBuffer), 5, 30); // Moved down from 25
  tft.setTextColor(COLOR_TEXT, COLOR_BACKGROUND);
  tft.drawString("Valid for next 24 hours", 5, 40); // Moved down from 35
  
  // KP Prediction
  tft.setTextColor(COLOR_TEMP, COLOR_BACKGROUND);
  tft.drawString("Predicted KP:", 5, 55); // Moved down from 50
  tft.setTextSize(2);
  
  // Color code KP prediction
  uint16_t kpColor = COLOR_WIND; // Green for low
  if (auroraToday.kpPredicted >= 7) {
    kpColor = 0xF800; // Red for very high
  } else if (auroraToday.kpPredicted >= 5) {
    kpColor = 0xFD20; // Orange for high
  } else if (auroraToday.kpPredicted >= 4) {
    kpColor = 0xFFE0; // Yellow for moderate
  }
  
  tft.setTextColor(kpColor, COLOR_BACKGROUND);
  tft.drawString(String(auroraToday.kpPredicted, 1), 120, 52); // Moved down from 47
  
  // Activity Level and Visibility on same line to save space
  tft.setTextSize(1);
  tft.setTextColor(COLOR_ACCENT, COLOR_BACKGROUND);
  tft.drawString("Activity: " + auroraToday.activity, 5, 70); // Compressed
  
  // Visibility - more compact
  tft.setTextColor(COLOR_HUMIDITY, COLOR_BACKGROUND);
  tft.drawString("Visible: " + auroraToday.visibility, 5, 82); // Combined text, moved up
  
  // Peak Time - more compact
  tft.setTextColor(COLOR_WIND, COLOR_BACKGROUND);
  tft.drawString("Best: " + auroraToday.peakTime, 5, 94); // Shortened text, moved up
  
  // Last update
  drawUpdateTime(auroraToday.lastUpdate);
  
  tft.setTextDatum(TL_DATUM);
}

void updateAuroraTomorrowDisplay() {
  static unsigned long lastAuroraTomorrowUpdate = 0;
  static String lastAuroraTomorrowTime = "";
  
  // Update display every 5 seconds OR when time changes OR on startup OR when forced
  extern String currentTime;
  extern bool forceDisplayUpdate;
  extern AuroraForecastData auroraTomorrow;
  bool timeChanged = (currentTime != lastAuroraTomorrowTime);
  static bool firstAuroraTomorrowRun = true;
  
  if (millis() - lastAuroraTomorrowUpdate < 5000 && !timeChanged && !firstAuroraTomorrowRun && !forceDisplayUpdate) {
    return;
  }
  
  firstAuroraTomorrowRun = false;
  lastAuroraTomorrowTime = currentTime;
  
  drawBackground();
  
  // Draw standardized header
  drawStandardHeader("AURORA TOMORROW");
  
  lastAuroraTomorrowUpdate = millis();
  
  tft.setTextSize(1);
  tft.setTextDatum(TL_DATUM);
  
  // Tomorrow's date and forecast timeframe - moved down and fixed date
  time_t now = time(nullptr);
  if (now < 1000000000) { // If time is not set properly, use approximate current time
    now = millis() / 1000 + 1729800000; // Approximate Oct 2024 timestamp
  }
  time_t tomorrow = now + 86400; // Add 24 hours
  struct tm* timeinfo = localtime(&tomorrow);
  char dateBuffer[20];
  strftime(dateBuffer, sizeof(dateBuffer), "%m/%d/%Y", timeinfo);
  
  tft.setTextColor(COLOR_ACCENT, COLOR_BACKGROUND);
  tft.drawString("Tomorrow: " + String(dateBuffer), 5, 30); // Moved down from 25
  tft.setTextColor(COLOR_TEXT, COLOR_BACKGROUND);
  tft.drawString("Valid 24-48 hours ahead", 5, 40); // Moved down from 35
  
  // KP Prediction - moved further down to match today's layout
  tft.setTextColor(COLOR_TEMP, COLOR_BACKGROUND);
  tft.drawString("Predicted KP:", 5, 55); // Match today's position
  tft.setTextSize(2);
  
  // Color code KP prediction
  uint16_t kpColor = COLOR_WIND; // Green for low
  if (auroraTomorrow.kpPredicted >= 7) {
    kpColor = 0xF800; // Red for very high
  } else if (auroraTomorrow.kpPredicted >= 5) {
    kpColor = 0xFD20; // Orange for high
  } else if (auroraTomorrow.kpPredicted >= 4) {
    kpColor = 0xFFE0; // Yellow for moderate
  }
  
  tft.setTextColor(kpColor, COLOR_BACKGROUND);
  tft.drawString(String(auroraTomorrow.kpPredicted, 1), 120, 52); // Match today's position
  
  // Activity Level and Visibility - exact same layout as today
  tft.setTextSize(1);
  tft.setTextColor(COLOR_ACCENT, COLOR_BACKGROUND);
  tft.drawString("Activity: " + auroraTomorrow.activity, 5, 70); // Match today
  
  // Visibility - exact same format as today
  tft.setTextColor(COLOR_HUMIDITY, COLOR_BACKGROUND);
  tft.drawString("Visible: " + auroraTomorrow.visibility, 5, 82); // Match today
  
  // Peak Time - exact same format as today
  tft.setTextColor(COLOR_WIND, COLOR_BACKGROUND);
  tft.drawString("Best: " + auroraTomorrow.peakTime, 5, 94); // Match today
  
  // Last update
  drawUpdateTime(auroraTomorrow.lastUpdate);
  
  tft.setTextDatum(TL_DATUM);
}

// Hourly Forecast Display
void updateHourlyForecastDisplay() {
  static unsigned long lastHourlyUpdate = 0;
  static String lastHourlyTime = "";
  
  // Update display every 5 seconds OR when time changes OR on startup OR when forced
  extern String currentTime;
  extern bool forceDisplayUpdate;
  extern HourlyForecastData hourlyForecast;
  bool timeChanged = (currentTime != lastHourlyTime);
  static bool firstHourlyRun = true;
  
  if (!firstHourlyRun && !forceDisplayUpdate && !timeChanged && 
      (millis() - lastHourlyUpdate < 5000)) {
    return; // Skip update to prevent flickering
  }
  
  drawBackground();
  
  // Draw standardized header
  drawStandardHeader("HOURLY FORECAST");
  
  // Bar graph for next 12 hours
  tft.setTextSize(1);
  
  // Find min/max temps for scaling
  float minTemp = 999, maxTemp = -999;
  for (int i = 0; i < 12; i++) {
    if (hourlyForecast.hours[i].temperature < minTemp) minTemp = hourlyForecast.hours[i].temperature;
    if (hourlyForecast.hours[i].temperature > maxTemp) maxTemp = hourlyForecast.hours[i].temperature;
  }
  
  float tempRange = maxTemp - minTemp;
  if (tempRange < 1) tempRange = 1; // Prevent division by zero
  
  // Draw 12 temperature bars
  for (int i = 0; i < 12; i++) {
    int xPos = 5 + (i * 18);  // 18 pixels per hour (fits in 216 pixels)
    
    // Calculate bar height (25 pixels max to fit new layout)
    float tempPercent = (hourlyForecast.hours[i].temperature - minTemp) / tempRange;
    int barHeight = (int)(tempPercent * 25) + 5; // 5-30 pixel range
    int barTop = 65 - barHeight;
    
    // Color code temperature bars
    uint16_t barColor = COLOR_TEMP;
    if (hourlyForecast.hours[i].temperature < 32) {
      barColor = 0x067F; // Blue for freezing
    } else if (hourlyForecast.hours[i].temperature > 80) {
      barColor = 0xF800; // Red for hot
    }
    
    // Draw temperature bar
    tft.fillRect(xPos, barTop, 15, barHeight, barColor);
    tft.drawRect(xPos, barTop, 15, barHeight, COLOR_TEXT);
    
    // Hour label (every other hour to avoid crowding)
    if (i % 2 == 0) {
      tft.setTextColor(COLOR_ACCENT, COLOR_BACKGROUND);
      String timeStr = hourlyForecast.hours[i].time;
      if (timeStr.length() > 2) timeStr = timeStr.substring(0, 2);
      tft.drawString(timeStr, xPos, 70);
    }
    
    // Temperature below bar
    tft.setTextColor(COLOR_TEMP, COLOR_BACKGROUND);
    String temp = String((int)hourlyForecast.hours[i].temperature);
    tft.drawString(temp, xPos, 80);
    
    // Precipitation chance (always show)
    tft.setTextColor(COLOR_HUMIDITY, COLOR_BACKGROUND);
    tft.drawString(String(hourlyForecast.hours[i].precipChance), xPos, 90);
  }
  
  // Enhanced legend with better positioning and colors
  tft.setTextColor(COLOR_TEMP, COLOR_BACKGROUND);
  tft.drawString("Temp (F)", 5, 105);
  tft.setTextColor(COLOR_HUMIDITY, COLOR_BACKGROUND);
  tft.drawString("Rain %", 85, 105);
  
  // Add horizontal separator only for better readability
  tft.drawFastHLine(0, 103, 240, COLOR_ACCENT);
  
  // Last update
  drawUpdateTime(hourlyForecast.lastUpdate);
  
  lastHourlyUpdate = millis();
  lastHourlyTime = currentTime;
  firstHourlyRun = false;
  
  tft.setTextDatum(TL_DATUM);
}

// Air Quality & UV Index Display
void updateAirQualityDisplay() {
  static unsigned long lastAirQualityUpdate = 0;
  static String lastAirQualityTime = "";
  
  // Update display every 5 seconds OR when time changes OR on startup OR when forced
  extern String currentTime;
  extern bool forceDisplayUpdate;
  extern AirQualityData airQuality;
  bool timeChanged = (currentTime != lastAirQualityTime);
  static bool firstAirQualityRun = true;
  
  if (!firstAirQualityRun && !forceDisplayUpdate && !timeChanged && 
      (millis() - lastAirQualityUpdate < 5000)) {
    return; // Skip update to prevent flickering
  }
  
  drawBackground();
  
  // Draw standardized header
  drawStandardHeader("AIR & UV");
  
  tft.setTextSize(1);
  
  // Air Quality Index
  tft.setTextColor(COLOR_TEXT, COLOR_BACKGROUND);
  tft.drawString("Air Quality Index:", 5, 30);
  
  // AQI color coding
  uint16_t aqiColor = COLOR_WIND; // Default green
  if (airQuality.aqi >= 4) aqiColor = 0xF800; // Red for poor
  else if (airQuality.aqi >= 3) aqiColor = 0xFD20; // Orange for moderate
  else if (airQuality.aqi >= 2) aqiColor = COLOR_PRESSURE; // Yellow for fair
  
  tft.setTextColor(aqiColor, COLOR_BACKGROUND);
  tft.drawString(String(airQuality.aqi) + "/5 - " + airQuality.status, 120, 30);
  
  // UV Index
  tft.setTextColor(COLOR_TEXT, COLOR_BACKGROUND);
  tft.drawString("UV Index:", 5, 45);
  
  // UV color coding
  uint16_t uvColor = COLOR_WIND; // Green for low
  if (airQuality.uvIndex > 10) uvColor = 0x7817; // Purple for extreme
  else if (airQuality.uvIndex > 7) uvColor = 0xF800; // Red for very high
  else if (airQuality.uvIndex > 5) uvColor = 0xFD20; // Orange for high
  else if (airQuality.uvIndex > 2) uvColor = COLOR_PRESSURE; // Yellow for moderate
  
  tft.setTextColor(uvColor, COLOR_BACKGROUND);
  tft.drawString(String(airQuality.uvIndex) + " - " + airQuality.uvRisk, 70, 45);
  
  // Pollutants
  tft.setTextColor(COLOR_TEXT, COLOR_BACKGROUND);
  tft.drawString("PM2.5: " + String(airQuality.pm2_5, 1) + " μg/m³", 5, 65);
  tft.drawString("PM10:  " + String(airQuality.pm10, 1) + " μg/m³", 5, 78);
  tft.drawString("O3:    " + String(airQuality.o3, 1) + " μg/m³", 5, 91);
  
  // Visibility - shortened text to fit
  if (airQuality.visibility > 0) {
    tft.drawString("Vis: " + String(airQuality.visibility, 1) + "km", 130, 78);
  }
  
  // Last update
  drawUpdateTime(airQuality.lastUpdate);
  
  lastAirQualityUpdate = millis();
  lastAirQualityTime = currentTime;
  firstAirQualityRun = false;
  
  tft.setTextDatum(TL_DATUM);
}

// Standardized header function for all screens
void drawStandardHeader(String title) {
  extern String currentTime;
  
  // Title - large and consistent
  tft.setTextColor(COLOR_ACCENT, COLOR_BACKGROUND);
  tft.setTextSize(2);
  tft.setTextDatum(TL_DATUM);
  tft.drawString(title, 5, 5);
  
  // Current time - consistent color and position
  tft.setTextColor(COLOR_TEXT, COLOR_BACKGROUND);
  tft.setTextSize(1);
  tft.setTextDatum(TR_DATUM);
  tft.drawString(currentTime, SCREEN_WIDTH - 5, 5);
  
  // Single horizontal line below header
  tft.drawLine(0, 25, SCREEN_WIDTH, 25, COLOR_ACCENT);
  
  // Reset to default
  tft.setTextDatum(TL_DATUM);
}

// Standardized update time display at bottom
void drawUpdateTime(unsigned long lastUpdate) {
  // Draw bottom line higher up to make room for status info
  tft.drawLine(0, 115, SCREEN_WIDTH, 115, COLOR_ACCENT);
  
  // Update time in lower left
  tft.setTextColor(0x7BEF, COLOR_BACKGROUND); // Gray
  tft.setTextSize(1);
  tft.setTextDatum(TL_DATUM);
  tft.drawString("Updated: " + String((millis() - lastUpdate) / 60000) + "m ago", 5, 120);
  
  // WiFi status in lower right
  tft.setTextDatum(TR_DATUM);
  if (WiFi.status() == WL_CONNECTED) {
    tft.setTextColor(COLOR_WIND, COLOR_BACKGROUND); // Green for connected
    tft.drawString("WiFi", SCREEN_WIDTH - 5, 120); // WiFi connected
  } else {
    tft.setTextColor(0xF800, COLOR_BACKGROUND); // Red for disconnected
    tft.drawString("NO WiFi", SCREEN_WIDTH - 5, 120); // Disconnected
  }
  
  // Reset to default
  tft.setTextDatum(TL_DATUM);
}

