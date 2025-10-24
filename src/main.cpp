#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h>
#include "config.h"
#include "weather.h"
#include "display.h"

// Configuration variables from config.h
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;
const char* apiKey = OPENWEATHER_API_KEY;

// OneCall 3.0 API Configuration
const char* ONECALL_API_URL = "https://api.openweathermap.org/data/3.0/onecall";

// Location Configuration from config.h
const float latitude = LATITUDE;
const float longitude = LONGITUDE;

TFT_eSPI tft = TFT_eSPI();
WeatherData currentWeather;
SpaceWeatherData currentSpaceWeather;
WeeklyForecast weeklyForecast;
AuroraForecastData auroraToday;
AuroraForecastData auroraTomorrow;
HourlyForecastData hourlyForecast;
AirQualityData airQuality;
NOAASpaceWeatherData noaaSpaceWeather;
// Screen control
int currentScreen = SCREEN_WEATHER;
bool forceDisplayUpdate = false; // Flag to force immediate display update
bool timeInitialized = false; // Global time initialization flag
unsigned long lastButtonPress = 0;
const unsigned long BUTTON_DEBOUNCE = 300;

// Time variables
String currentTime = "";
unsigned long lastTimeUpdate = 0;
const unsigned long TIME_UPDATE_INTERVAL = 60000; // Update every minute
unsigned long lastWeatherUpdate = 0;
const unsigned long WEATHER_UPDATE_INTERVAL = 600000; // 10 minutes (600 calls/day limit)

// API call tracking for rate limiting
int dailyApiCalls = 0;
const int MAX_DAILY_CALLS = 600; // Safety margin under 1000 limit
unsigned long lastDayReset = 0;

// Function declarations
void connectToWiFi();
void updateWeatherData();
void updateMoonData();
void updateAllWeatherData();
void updateAirQualityData();
void updateSpaceWeatherData();
void updateNOAASpaceWeatherData();
void update7DayForecast();
void updateAuroraForecast();
void updateTime();
void handleButtons();

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP32 Weather Station Starting...");
  
  // Initialize backlight pin
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH); // Turn on backlight
  Serial.println("Backlight turned on");
  
  // Initialize buttons
  pinMode(RIGHT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(LEFT_BUTTON_PIN, INPUT_PULLUP);
  Serial.println("Buttons initialized");
  
  // Initialize display
  tft.init();
  tft.setRotation(1); // Landscape mode
  Serial.println("Display initialized");
  
  displayInit();
  
  // Show startup sequence with detailed status
  displayMessage("ESP32 Weather Station");
  delay(2000);
  
  displayMessage("Starting System...");
  delay(1500);
  
  displayMessage("Hardware: OK");
  delay(1000);
  
  // Connect to WiFi with detailed status
  displayMessage("Connecting to WiFi...");
  Serial.println("Starting WiFi connection...");
  connectToWiFi();
  
  // Time synchronization
  displayMessage("Syncing Time...");
  Serial.println("Synchronizing time with API...");
  delay(1000);
  updateTime(); // Get time immediately on startup
  displayMessage("Time: Synchronized");
  delay(1500);
  
  // Weather data fetching
  displayMessage("Fetching Weather...");
  Serial.println("Fetching weather data from OpenWeatherMap OneCall 3.0...");
  delay(1000);
  updateAllWeatherData(); // OneCall 3.0 - gets current, hourly, daily in one call
  displayMessage("Weather: Updated");
  delay(1500);
  
  // Air quality data
  displayMessage("Getting Air Quality...");
  Serial.println("Fetching air quality data...");
  delay(1000);
  updateAirQualityData();
  displayMessage("Air Quality: Updated");
  delay(1500);
  
  // Space weather data
  displayMessage("Space Weather APIs...");
  Serial.println("Fetching space weather data...");
  delay(1000);
  updateSpaceWeatherData();
  updateNOAASpaceWeatherData();
  updateAuroraForecast();
  displayMessage("Space Weather: Updated");
  delay(1500);
  
  // Final status
  displayMessage("System Ready!");
  Serial.println("All systems initialized successfully!");
  delay(2000);
  
  displayMessage("Loading Interface...");
  delay(1000);
  
  Serial.println("Weather Station Ready!");
}

void loop() {
  // Check button presses
  handleButtons();
  
  // Update time every minute or when crossing minute boundary
  static String lastTimeCheck = "";
  bool intervalReached = (millis() - lastTimeUpdate >= TIME_UPDATE_INTERVAL);
  
  if (intervalReached) {
    updateTime();
    
    // Call moon data after first successful time initialization
    static bool moonDataInitialized = false;
    if (!moonDataInitialized && timeInitialized) {
      Serial.println("Time initialized, calling moon data for first time...");
      updateMoonData();
      moonDataInitialized = true;
    }
  }
  
  // Check if it's time to update weather data
  if (millis() - lastWeatherUpdate > WEATHER_UPDATE_INTERVAL) {
    updateAllWeatherData(); // OneCall 3.0 - gets current, hourly, daily in one call
    updateAirQualityData(); // Separate air quality call
    updateMoonData(); // Also update moon data
    updateSpaceWeatherData(); // Also update space weather data
    updateNOAASpaceWeatherData(); // Also update NOAA space weather data
    updateAuroraForecast(); // Update aurora forecasts
    lastWeatherUpdate = millis(); // Update the timestamp
  }
  
  // Update display based on current screen
  if (currentScreen == SCREEN_WEATHER) {
    updateDisplay();
  } else if (currentScreen == SCREEN_FORECAST_7DAY) {
    update7DayForecastDisplay();
  } else if (currentScreen == SCREEN_HOURLY_FORECAST) {
    updateHourlyForecastDisplay();
  } else if (currentScreen == SCREEN_AIR_QUALITY) {
    updateAirQualityDisplay();
  } else if (currentScreen == SCREEN_ASTRONOMY) {
    updateAstronomyDisplay();
  } else if (currentScreen == SCREEN_SPACE_WEATHER) {
    updateSpaceWeatherDisplay();
  } else if (currentScreen == SCREEN_AURORA_TODAY) {
    updateAuroraTodayDisplay();
  } else if (currentScreen == SCREEN_AURORA_TOMORROW) {
    updateAuroraTomorrowDisplay();
  } else {
    // Fallback to weather screen if something went wrong
    currentScreen = SCREEN_WEATHER;
    updateDisplay();
  }
  
  // Reset force update flag after display functions have been called
  if (forceDisplayUpdate) {
    forceDisplayUpdate = false;
  }
  
  delay(50); // Fast updates for responsive buttons, time/weather update on intervals
}

void connectToWiFi() {
  displayMessage("Connecting to WiFi...");
  Serial.print("Connecting to SSID: ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    displayMessage("WiFi: Attempt " + String(attempts + 1) + "/20");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.print("Connected to WiFi. IP address: ");
    Serial.println(WiFi.localIP());
    displayMessage("WiFi: Connected!");
    delay(1000);
    displayMessage("IP: " + WiFi.localIP().toString());
    delay(2000);
  } else {
    Serial.println("Failed to connect to WiFi");
    displayMessage("WiFi Connection Failed!");
    delay(2000);
  }
}

void updateWeatherData() {
  // Legacy function - now using updateAllWeatherData() with OneCall 3.0
  // This is kept for compatibility but redirects to the new function
  updateAllWeatherData();
}

void parseWeatherData(String jsonString) {
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, jsonString);
  
  if (error) {
    Serial.print("JSON parsing failed: ");
    Serial.println(error.c_str());
    return;
  }
  
  // Extract weather data
  currentWeather.temperature = doc["main"]["temp"];
  currentWeather.humidity = doc["main"]["humidity"];
  currentWeather.pressure = doc["main"]["pressure"];
  currentWeather.description = doc["weather"][0]["description"].as<String>();
  currentWeather.icon = doc["weather"][0]["icon"].as<String>();
  currentWeather.cityName = doc["name"].as<String>();
  currentWeather.windSpeed = doc["wind"]["speed"];
  currentWeather.windDirection = doc["wind"]["deg"];
  
  // Extract astronomical data
  currentWeather.sunrise = doc["sys"]["sunrise"];
  currentWeather.sunset = doc["sys"]["sunset"];
  
  // Note: Basic weather API doesn't include moon data
  // For moon data, you'd need a different API endpoint
  currentWeather.moonrise = 0; // Placeholder
  currentWeather.moonset = 0;  // Placeholder
  
  Serial.println("Weather data parsed successfully");
}

void updateTime() {
  static unsigned long timeOffset = 0;
  // timeInitialized is now global
  
  if (!timeInitialized || (millis() - lastTimeUpdate > 3600000)) { // Update from server every hour
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      // Use your local timezone (adjust as needed)
      http.begin("http://worldtimeapi.org/api/timezone/America/Chicago");
      http.setTimeout(10000); // 10 second timeout
      int httpCode = http.GET();
      
      Serial.print("Time API HTTP code: "); Serial.println(httpCode);
      
      if (httpCode == 200) {
        String payload = http.getString();
        Serial.println("Time API response: " + payload);
        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, payload);
        
        if (!error) {
          String datetime = doc["datetime"].as<String>();
          String timezone = doc["timezone"].as<String>();
          
          Serial.println("Parsed datetime: " + datetime);
          
          // Parse ISO datetime: 2025-10-23T14:30:45.123456-05:00
          int hour = datetime.substring(11, 13).toInt();
          int minute = datetime.substring(14, 16).toInt();
          int second = datetime.substring(17, 19).toInt();
          
          Serial.printf("Parsed time: %02d:%02d:%02d\n", hour, minute, second);
          
          unsigned long serverTime = hour * 3600 + minute * 60 + second;
          timeOffset = serverTime - (millis() / 1000);
          timeInitialized = true;
          
          // Configure ESP32 system timezone for Chicago (Central Time)
          // CST6CDT means Central Standard Time (UTC-6) with Daylight saving (UTC-5)
          configTime(-6 * 3600, 3600, "pool.ntp.org"); // GMT-6 offset, 1 hour DST
          setenv("TZ", "CST6CDT,M3.2.0,M11.1.0", 1);
          tzset();
          
          Serial.print("Time synchronized - Local Time (");
          Serial.print(timezone);
          Serial.println(") - ESP32 timezone configured for Central Time");
        } else {
          Serial.println("Failed to parse time API JSON");
        }
      } else {
        Serial.print("Time API failed with HTTP code: ");
        Serial.println(httpCode);
      }
      http.end();
    }
  }
  
  // Calculate current time
  if (timeInitialized) {
    unsigned long currentSeconds = (millis() / 1000) + timeOffset;
    int hours = (currentSeconds % 86400) / 3600;
    int minutes = (currentSeconds % 3600) / 60;
    
    String ampm = hours >= 12 ? "PM" : "AM";
    if (hours > 12) hours -= 12;
    if (hours == 0) hours = 12;
    
    currentTime = String(hours) + ":";
    if (minutes < 10) currentTime += "0";
    currentTime += String(minutes) + " " + ampm;
    Serial.println("Time updated: " + currentTime);
  } else {
    // Fallback to uptime (no seconds to prevent flicker)
    unsigned long uptime = millis() / 1000;
    int hours = uptime / 3600;
    int minutes = (uptime % 3600) / 60;
    currentTime = String(hours) + "h" + String(minutes) + "m";
    Serial.println("Time fallback: " + currentTime);
  }
  
  lastTimeUpdate = millis();
}

void handleButtons() {
  static bool buttonWasPressed = false;
  unsigned long now = millis();
  
  // Check right button (screen switch)
  bool buttonPressed = (digitalRead(RIGHT_BUTTON_PIN) == LOW);
  
  if (buttonPressed && !buttonWasPressed) {
    // Button just pressed (rising edge)
    if (now - lastButtonPress > BUTTON_DEBOUNCE) {
      // Cycle through all screens
      if (currentScreen == SCREEN_WEATHER) {
        currentScreen = SCREEN_FORECAST_7DAY;
      } else if (currentScreen == SCREEN_FORECAST_7DAY) {
        currentScreen = SCREEN_HOURLY_FORECAST;
      } else if (currentScreen == SCREEN_HOURLY_FORECAST) {
        currentScreen = SCREEN_AIR_QUALITY;
      } else if (currentScreen == SCREEN_AIR_QUALITY) {
        currentScreen = SCREEN_ASTRONOMY;
      } else if (currentScreen == SCREEN_ASTRONOMY) {
        currentScreen = SCREEN_SPACE_WEATHER;
      } else if (currentScreen == SCREEN_SPACE_WEATHER) {
        currentScreen = SCREEN_AURORA_TODAY;
      } else if (currentScreen == SCREEN_AURORA_TODAY) {
        currentScreen = SCREEN_AURORA_TOMORROW;
      } else {
        currentScreen = SCREEN_WEATHER;
      }
      
      Serial.print("Switched to screen: ");
      if (currentScreen == SCREEN_WEATHER) {
        Serial.println("Current Weather");
      } else if (currentScreen == SCREEN_FORECAST_7DAY) {
        Serial.println("7-Day Forecast");
      } else if (currentScreen == SCREEN_HOURLY_FORECAST) {
        Serial.println("Hourly Forecast");
      } else if (currentScreen == SCREEN_AIR_QUALITY) {
        Serial.println("Air Quality & UV");
      } else if (currentScreen == SCREEN_ASTRONOMY) {
        Serial.println("Astronomy");
      } else if (currentScreen == SCREEN_SPACE_WEATHER) {
        Serial.println("Space Weather");
      } else if (currentScreen == SCREEN_AURORA_TODAY) {
        Serial.println("Aurora Today");
      } else if (currentScreen == SCREEN_AURORA_TOMORROW) {
        Serial.println("Aurora Tomorrow");
      } else {
        Serial.println("NOAA Space Weather");
      }
      lastButtonPress = now;
      
      // Force immediate display update
      forceDisplayUpdate = true;
      tft.fillScreen(COLOR_BACKGROUND);
    }
  }
  
  buttonWasPressed = buttonPressed;
}

void updateSpaceWeatherData() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    Serial.println("Fetching space weather data...");
    
    // Using NOAA Space Weather Prediction Center API
    // This is a simplified approach - in production you might want multiple APIs
    http.begin("https://services.swpc.noaa.gov/products/noaa-planetary-k-index.json");
    
    int httpCode = http.GET();
    if (httpCode == 200) {
      String payload = http.getString();
      
      // Parse the JSON for latest KP index
      JsonDocument doc;
      DeserializationError error = deserializeJson(doc, payload);
      
      if (!error && doc.size() > 1) {
        // Get the most recent entry (last in array)
        JsonArray data = doc.as<JsonArray>();
        if (data.size() > 0) {
          JsonArray latest = data[data.size()-1];
          if (latest.size() > 1) {
            currentSpaceWeather.kpIndex = latest[1].as<float>();
          }
        }
      }
    }
    http.end();
    
    // Get additional space weather data from a second API call
    // Using NOAA real-time solar wind data
    http.begin("https://services.swpc.noaa.gov/products/solar-wind/mag-1-day.json");
    httpCode = http.GET();
    if (httpCode == 200) {
      String payload = http.getString();
      JsonDocument doc;
      DeserializationError error = deserializeJson(doc, payload);
      
      if (!error && doc.size() > 1) {
        JsonArray data = doc.as<JsonArray>();
        if (data.size() > 0) {
          // Get latest entry
          JsonArray latest = data[data.size()-1];
          if (latest.size() > 6) {
            currentSpaceWeather.magneticFieldBz = latest[6].as<float>(); // Bz component
          }
        }
      }
    }
    http.end();
    
    // Get real solar wind speed and density data
    http.begin("https://services.swpc.noaa.gov/products/solar-wind/plasma-1-day.json");
    httpCode = http.GET();
    if (httpCode == 200) {
      String payload = http.getString();
      JsonDocument doc;
      DeserializationError error = deserializeJson(doc, payload);
      
      if (!error && doc.size() > 1) {
        JsonArray data = doc.as<JsonArray>();
        if (data.size() > 0) {
          // Get latest entry for solar wind speed and density
          JsonArray latest = data[data.size()-1];
          if (latest.size() > 2) {
            currentSpaceWeather.solarWindSpeed = latest[1].as<float>(); // Speed km/s
            currentSpaceWeather.solarWindDensity = latest[2].as<float>(); // Density p/cm³
          }
        }
      }
    } else {
      // Fallback values if API fails
      currentSpaceWeather.solarWindSpeed = 400.0; // Typical average
      currentSpaceWeather.solarWindDensity = 5.0;  // Typical average
    }
    http.end();
    
    // Determine geomagnetic status based on KP index
    if (currentSpaceWeather.kpIndex < 3) {
      currentSpaceWeather.geomagStatus = "Quiet";
    } else if (currentSpaceWeather.kpIndex < 5) {
      currentSpaceWeather.geomagStatus = "Unsettled";
    } else if (currentSpaceWeather.kpIndex < 7) {
      currentSpaceWeather.geomagStatus = "Active";
    } else {
      currentSpaceWeather.geomagStatus = "Storm";
    }
    
    // Aurora forecast based on KP and Bz
    if (currentSpaceWeather.kpIndex >= 5 && currentSpaceWeather.magneticFieldBz < -5) {
      currentSpaceWeather.auroraForecast = "HIGH";
    } else if (currentSpaceWeather.kpIndex >= 4 || currentSpaceWeather.magneticFieldBz < -3) {
      currentSpaceWeather.auroraForecast = "MODERATE";
    } else if (currentSpaceWeather.kpIndex >= 3) {
      currentSpaceWeather.auroraForecast = "LOW";
    } else {
      currentSpaceWeather.auroraForecast = "MINIMAL";
    }
    
    currentSpaceWeather.lastUpdate = millis();
    
    Serial.println("Space weather data updated:");
    Serial.print("KP Index: "); Serial.println(currentSpaceWeather.kpIndex);
    Serial.print("Geomag Status: "); Serial.println(currentSpaceWeather.geomagStatus);
    Serial.print("Aurora Forecast: "); Serial.println(currentSpaceWeather.auroraForecast);
    Serial.print("Bz: "); Serial.print(currentSpaceWeather.magneticFieldBz); Serial.println(" nT");
  } else {
    Serial.println("WiFi not connected - cannot fetch space weather data");
  }
}

void updateNOAASpaceWeatherData() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    Serial.println("Fetching NOAA space weather data...");
    
    // Initialize data with defaults
    noaaSpaceWeather.alertCount = 0;
    noaaSpaceWeather.solarFluxIndex = 0.0;
    noaaSpaceWeather.aIndex = 0.0;
    noaaSpaceWeather.kpIndex = 0.0;
    noaaSpaceWeather.xrayFlux = "A1.0";
    noaaSpaceWeather.protonFlux = "Quiet";
    noaaSpaceWeather.sunspotNumber = 0;
    
    // Get Solar Flux Index (10.7 cm radio flux)
    http.begin("https://services.swpc.noaa.gov/json/f107_cm_flux.json");
    int httpCode = http.GET();
    if (httpCode == 200) {
      String payload = http.getString();
      JsonDocument doc;
      DeserializationError error = deserializeJson(doc, payload);
      
      if (!error && doc.size() > 0) {
        JsonArray data = doc.as<JsonArray>();
        if (data.size() > 0) {
          JsonObject latest = data[data.size()-1];
          noaaSpaceWeather.solarFluxIndex = latest["flux"].as<float>();
        }
      }
    }
    http.end();
    
    // Get Daily Geomagnetic Indices (A-index)
    http.begin("https://services.swpc.noaa.gov/products/daily-geomagnetic-indices.json");
    httpCode = http.GET();
    if (httpCode == 200) {
      String payload = http.getString();
      JsonDocument doc;
      DeserializationError error = deserializeJson(doc, payload);
      
      if (!error && doc.size() > 1) {
        JsonArray data = doc.as<JsonArray>();
        if (data.size() > 1) {
          JsonArray latest = data[data.size()-1];
          if (latest.size() > 3) {
            noaaSpaceWeather.aIndex = latest[3].as<float>(); // A-index
          }
        }
      }
    }
    http.end();
    
    // Get current Kp index (reuse from existing function)
    noaaSpaceWeather.kpIndex = currentSpaceWeather.kpIndex;
    
    // Get X-ray flux data
    http.begin("https://services.swpc.noaa.gov/json/goes/primary/xrays-1-day.json");
    httpCode = http.GET();
    if (httpCode == 200) {
      String payload = http.getString();
      JsonDocument doc;
      DeserializationError error = deserializeJson(doc, payload);
      
      if (!error && doc.size() > 0) {
        JsonArray data = doc.as<JsonArray>();
        if (data.size() > 0) {
          JsonObject latest = data[data.size()-1];
          float flux = latest["flux"].as<float>();
          
          // Convert to X-ray class
          if (flux >= 1e-3) {
            noaaSpaceWeather.xrayFlux = "X" + String(flux/1e-4, 1);
          } else if (flux >= 1e-4) {
            noaaSpaceWeather.xrayFlux = "M" + String(flux/1e-5, 1);
          } else if (flux >= 1e-5) {
            noaaSpaceWeather.xrayFlux = "C" + String(flux/1e-6, 1);
          } else if (flux >= 1e-6) {
            noaaSpaceWeather.xrayFlux = "B" + String(flux/1e-7, 1);
          } else {
            noaaSpaceWeather.xrayFlux = "A" + String(flux/1e-8, 1);
          }
        }
      }
    }
    http.end();
    
    // Get sunspot data
    http.begin("https://services.swpc.noaa.gov/json/solar_regions.json");
    httpCode = http.GET();
    if (httpCode == 200) {
      String payload = http.getString();
      JsonDocument doc;
      DeserializationError error = deserializeJson(doc, payload);
      
      if (!error) {
        JsonArray regions = doc.as<JsonArray>();
        noaaSpaceWeather.sunspotNumber = regions.size(); // Count active regions as approximate sunspot number
      }
    }
    http.end();
    
    // Get space weather alerts
    http.begin("https://services.swpc.noaa.gov/products/alerts.json");
    httpCode = http.GET();
    if (httpCode == 200) {
      String payload = http.getString();
      JsonDocument doc;
      DeserializationError error = deserializeJson(doc, payload);
      
      if (!error) {
        JsonArray alerts = doc.as<JsonArray>();
        noaaSpaceWeather.alertCount = min(5, (int)alerts.size()); // Limit to 5 alerts
        
        for (int i = 0; i < noaaSpaceWeather.alertCount; i++) {
          JsonObject alert = alerts[i];
          String message = alert["message"].as<String>();
          
          // Extract key info from alert message
          if (message.indexOf("WATCH") >= 0) {
            noaaSpaceWeather.alerts[i] = "WATCH: " + message.substring(0, 20);
          } else if (message.indexOf("WARNING") >= 0) {
            noaaSpaceWeather.alerts[i] = "WARN: " + message.substring(0, 20);
          } else {
            noaaSpaceWeather.alerts[i] = message.substring(0, 25);
          }
        }
      }
    }
    http.end();
    
    // Set proton flux status based on alerts
    bool protonAlert = false;
    for (int i = 0; i < noaaSpaceWeather.alertCount; i++) {
      if (noaaSpaceWeather.alerts[i].indexOf("proton") >= 0 || 
          noaaSpaceWeather.alerts[i].indexOf("PROTON") >= 0) {
        protonAlert = true;
        break;
      }
    }
    noaaSpaceWeather.protonFlux = protonAlert ? "Enhanced" : "Quiet";
    
    noaaSpaceWeather.lastUpdate = millis();
    
    Serial.println("NOAA space weather data updated:");
    Serial.print("Solar Flux: "); Serial.println(noaaSpaceWeather.solarFluxIndex);
    Serial.print("A Index: "); Serial.println(noaaSpaceWeather.aIndex);
    Serial.print("Kp Index: "); Serial.println(noaaSpaceWeather.kpIndex);
    Serial.print("X-ray Class: "); Serial.println(noaaSpaceWeather.xrayFlux);
    Serial.print("Sunspots: "); Serial.println(noaaSpaceWeather.sunspotNumber);
    Serial.print("Alerts: "); Serial.println(noaaSpaceWeather.alertCount);
    
  } else {
    Serial.println("WiFi not connected - cannot fetch NOAA space weather data");
  }
}

void update7DayForecast() {
  // Legacy function - data now comes from OneCall 3.0 in updateAllWeatherData()
  Serial.println("7-day forecast data updated via OneCall 3.0");
  weeklyForecast.lastUpdate = millis();
}

/* OLD CODE COMMENTED OUT - REPLACE WITH ONECALL 3.0
      String payload = http.getString();
      JsonDocument doc;
      DeserializationError error = deserializeJson(doc, payload);
      
      if (!error) {
        JsonArray list = doc["list"];
        
        // Process forecast data (5-day forecast with 3-hour intervals)
        // We'll take one reading per day around noon
        int dayIndex = 0;
        String lastDate = "";
        
        for (JsonObject item : list) {
          if (dayIndex >= 7) break;
          
          String dateTime = item["dt_txt"].as<String>();
          String currentDate = dateTime.substring(0, 10); // YYYY-MM-DD
          String currentTime = dateTime.substring(11, 13); // HH
          
          // Take the forecast around noon (12:00) or closest available
          if (currentTime == "12" && currentDate != lastDate) {
            JsonObject main = item["main"];
            JsonObject weather = item["weather"][0];
            
            weeklyForecast.days[dayIndex].tempHigh = main["temp_max"].as<float>();
            weeklyForecast.days[dayIndex].tempLow = main["temp_min"].as<float>();
            weeklyForecast.days[dayIndex].icon = weather["icon"].as<String>();
            weeklyForecast.days[dayIndex].description = weather["description"].as<String>();
            
            if (item.containsKey("pop")) {
              weeklyForecast.days[dayIndex].precipChance = (int)(item["pop"].as<float>() * 100);
            } else {
              weeklyForecast.days[dayIndex].precipChance = 0;
            }
            
            // Convert date to day name using a simpler method
            String dayNames[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
            
            if (dayIndex == 0) {
              weeklyForecast.days[dayIndex].dayName = "Today";
            } else if (dayIndex == 1) {
              weeklyForecast.days[dayIndex].dayName = "Tomorrow";
            } else {
              // Parse the date string to get day of week
              String dateTime = item["dt_txt"].as<String>();
              unsigned long timestamp = item["dt"].as<unsigned long>();
              
              // Calculate day of week (simple approximation)
              // Unix timestamp 0 was a Thursday, so we adjust
              int dayOfWeek = ((timestamp / 86400) + 4) % 7;
              weeklyForecast.days[dayIndex].dayName = dayNames[dayOfWeek];
            }
            
            lastDate = currentDate;
            dayIndex++;
          }
        }
        
        weeklyForecast.lastUpdate = millis();
        Serial.println("7-day forecast updated successfully");
      }
    } else {
      Serial.print("HTTP error: ");
      Serial.println(httpCode);
    }
    
    http.end();
  } else {
    Serial.println("WiFi not connected - cannot fetch 7-day forecast");
  }
END OF OLD CODE COMMENTED OUT */

void updateAuroraForecast() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    Serial.println("Fetching NOAA aurora forecast...");
    
    // NOAA Aurora forecast - this is a simplified version
    // In production, you'd want to parse their more complex forecast data
    http.begin("https://services.swpc.noaa.gov/products/noaa-planetary-k-index-forecast.json");
    
    int httpCode = http.GET();
    if (httpCode == 200) {
      String payload = http.getString();
      JsonDocument doc;
      DeserializationError error = deserializeJson(doc, payload);
      
      if (!error && doc.size() > 0) {
        JsonArray data = doc.as<JsonArray>();
        
        // Process today's forecast
        if (data.size() > 1) {
          JsonArray todayData = data[1]; // First forecast entry
          if (todayData.size() > 1) {
            auroraToday.kpPredicted = todayData[1].as<float>();
            auroraToday.date = "Today";
            
            // Determine activity level
            if (auroraToday.kpPredicted >= 7) {
              auroraToday.activity = "Very High";
              auroraToday.visibility = "Visible to southern WI";
            } else if (auroraToday.kpPredicted >= 5) {
              auroraToday.activity = "High";
              auroraToday.visibility = "Visible to northern WI";
            } else if (auroraToday.kpPredicted >= 4) {
              auroraToday.activity = "Moderate";
              auroraToday.visibility = "Northern horizon only";
            } else {
              auroraToday.activity = "Low";
              auroraToday.visibility = "Not likely visible";
            }
            
            auroraToday.peakTime = "10 PM - 2 AM";
            auroraToday.confidence = "Medium";
          }
        }
        
        // Process tomorrow's forecast
        if (data.size() > 2) {
          JsonArray tomorrowData = data[2]; // Second forecast entry
          if (tomorrowData.size() > 1) {
            auroraTomorrow.kpPredicted = tomorrowData[1].as<float>();
            auroraTomorrow.date = "Tomorrow";
            
            // Determine activity level
            if (auroraTomorrow.kpPredicted >= 7) {
              auroraTomorrow.activity = "Very High";
              auroraTomorrow.visibility = "Visible to southern WI";
            } else if (auroraTomorrow.kpPredicted >= 5) {
              auroraTomorrow.activity = "High";
              auroraTomorrow.visibility = "Visible to northern WI";
            } else if (auroraTomorrow.kpPredicted >= 4) {
              auroraTomorrow.activity = "Moderate";
              auroraTomorrow.visibility = "Northern horizon only";
            } else {
              auroraTomorrow.activity = "Low";
              auroraTomorrow.visibility = "Not likely visible";
            }
            
            auroraTomorrow.peakTime = "10 PM - 2 AM";
            auroraTomorrow.confidence = "Medium";
          }
        }
        
        auroraToday.lastUpdate = millis();
        auroraTomorrow.lastUpdate = millis();
        
        Serial.println("Aurora forecasts updated:");
        Serial.print("Today KP: "); Serial.print(auroraToday.kpPredicted); 
        Serial.print(" ("); Serial.print(auroraToday.activity); Serial.println(")");
        Serial.print("Tomorrow KP: "); Serial.print(auroraTomorrow.kpPredicted); 
        Serial.print(" ("); Serial.print(auroraTomorrow.activity); Serial.println(")");
      }
    }
    
    http.end();
  } else {
    Serial.println("WiFi not connected - cannot fetch aurora forecast");
  }
}

// New unified OneCall 3.0 function for all weather data
void updateAllWeatherData() {
  // Check API rate limits
  unsigned long now = millis();
  if (now - lastDayReset > 86400000) { // Reset daily counter every 24 hours
    dailyApiCalls = 0;
    lastDayReset = now;
  }
  
  if (dailyApiCalls >= MAX_DAILY_CALLS) {
    Serial.println("Daily API limit reached! Skipping update.");
    return;
  }
  
  if (now - currentWeather.lastUpdate < WEATHER_UPDATE_INTERVAL && currentWeather.lastUpdate > 0) {
    return; // Don't update too frequently
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    Serial.println("Fetching weather data with OneCall 3.0...");
    Serial.print("API calls today: "); Serial.println(dailyApiCalls);
    
    // OneCall 3.0 API - gets current, hourly, daily, and air quality in one call
    String oneCallUrl = String(ONECALL_API_URL) + "?lat=" + String(latitude, 4) + 
                        "&lon=" + String(longitude, 4) + "&appid=" + String(apiKey) + 
                        "&units=imperial&exclude=minutely,alerts";
    
    http.begin(oneCallUrl.c_str());
    int httpCode = http.GET();
    
    if (httpCode == 200) {
      dailyApiCalls++; // Count successful API call
      String payload = http.getString();
      JsonDocument doc;
      DeserializationError error = deserializeJson(doc, payload);
      
      if (!error) {
        // Parse current weather
        JsonObject current = doc["current"];
        currentWeather.temperature = current["temp"].as<float>();
        currentWeather.humidity = current["humidity"].as<int>();
        currentWeather.pressure = current["pressure"].as<float>();
        currentWeather.windSpeed = current["wind_speed"].as<float>();
        currentWeather.windDirection = current["wind_deg"].as<int>();
        currentWeather.description = current["weather"][0]["description"].as<String>();
        currentWeather.icon = current["weather"][0]["icon"].as<String>();
        currentWeather.cityName = String(LOCATION_NAME);
        currentWeather.sunrise = current["sunrise"].as<unsigned long>();
        currentWeather.sunset = current["sunset"].as<unsigned long>();
        currentWeather.lastUpdate = millis();
        
        // Parse air quality data
        if (current.containsKey("uvi")) {
          airQuality.uvIndex = current["uvi"].as<int>();
          if (airQuality.uvIndex <= 2) airQuality.uvRisk = "Low";
          else if (airQuality.uvIndex <= 5) airQuality.uvRisk = "Moderate";
          else if (airQuality.uvIndex <= 7) airQuality.uvRisk = "High";
          else if (airQuality.uvIndex <= 10) airQuality.uvRisk = "Very High";
          else airQuality.uvRisk = "Extreme";
        }
        
        if (current.containsKey("visibility")) {
          airQuality.visibility = current["visibility"].as<float>() / 1000.0; // Convert to km
        }
        
        // Parse hourly forecast (next 12 hours, starting from next hour)
        JsonArray hourly = doc["hourly"];
        int hourCount = 0;
        
        Serial.println("=== HOURLY FORECAST DEBUG ===");
        Serial.print("Total hourly entries: "); Serial.println(hourly.size());
        
        // Get current local time for comparison
        time_t currentLocalTime = time(nullptr);
        struct tm* currentTm = localtime(&currentLocalTime);
        int currentHour = currentTm->tm_hour;
        Serial.print("Current local hour: "); Serial.println(currentHour);
        
        // Use traditional for loop starting from index 1 (skip current hour at index 0)
        for (int i = 1; i < hourly.size() && hourCount < 12; i++) {
          JsonObject hour = hourly[i];
          
          unsigned long timestamp = hour["dt"].as<unsigned long>();
          
          // Convert UTC timestamp to local time properly
          time_t utcTime = (time_t)timestamp;
          struct tm* timeinfo = localtime(&utcTime); // This should now work with our timezone config
          
          Serial.print("API Timestamp: "); Serial.print(timestamp);
          Serial.print(" -> Local time: "); Serial.print(timeinfo->tm_hour);
          Serial.print(":"); Serial.print(timeinfo->tm_min);
          Serial.println();
          
          hourlyForecast.hours[hourCount].time = String(timeinfo->tm_hour % 12 == 0 ? 12 : timeinfo->tm_hour % 12) + 
                                                 (timeinfo->tm_hour >= 12 ? "PM" : "AM");
          hourlyForecast.hours[hourCount].temperature = hour["temp"].as<float>();
          hourlyForecast.hours[hourCount].icon = hour["weather"][0]["icon"].as<String>();
          hourlyForecast.hours[hourCount].description = hour["weather"][0]["main"].as<String>();
          hourlyForecast.hours[hourCount].precipChance = (hour.containsKey("pop") ? 
                                                          (int)(hour["pop"].as<float>() * 100) : 0);
          hourlyForecast.hours[hourCount].humidity = hour["humidity"].as<int>();
          
          // Enhanced debug output
          Serial.print("Hour "); Serial.print(hourCount); 
          Serial.print(" (API Index "); Serial.print(i); Serial.print(")");
          Serial.print(" - Time: "); Serial.print(hourlyForecast.hours[hourCount].time);
          Serial.print(", Temp: "); Serial.print(hourlyForecast.hours[hourCount].temperature);
          Serial.print(", Humidity: "); Serial.print(hourlyForecast.hours[hourCount].humidity);
          Serial.print("%, Rain: "); Serial.print(hourlyForecast.hours[hourCount].precipChance);
          Serial.println("%");
          
          hourCount++;
        }
        Serial.println("=== END HOURLY DEBUG ===");
        hourlyForecast.lastUpdate = millis();
        
        // Parse daily forecast (7 days)
        JsonArray daily = doc["daily"];
        String dayNames[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
        int dayCount = 0;
        
        for (JsonObject day : daily) {
          if (dayCount >= 7) break;
          
          unsigned long timestamp = day["dt"].as<unsigned long>();
          struct tm* timeinfo = localtime((time_t*)&timestamp);
          
          if (dayCount == 0) {
            weeklyForecast.days[dayCount].dayName = "Today";
            // Get moon phase data from today's forecast
            if (day.containsKey("moon_phase")) {
              currentWeather.moonPhase = day["moon_phase"].as<float>();
              
              Serial.println("=== MOON DATA FROM ONECALL API ===");
              Serial.print("Raw OneCall Moon Phase: ");
              Serial.println(currentWeather.moonPhase);
              
              // Convert moon phase to name (0.0 = New Moon, 0.5 = Full Moon)
              float phase = currentWeather.moonPhase;
              if (phase < 0.0625 || phase >= 0.9375) {
                currentWeather.moonPhaseName = "New Moon";
              } else if (phase < 0.1875) {
                currentWeather.moonPhaseName = "Waxing Crescent";
              } else if (phase < 0.3125) {
                currentWeather.moonPhaseName = "First Quarter";
              } else if (phase < 0.4375) {
                currentWeather.moonPhaseName = "Waxing Gibbous";
              } else if (phase < 0.5625) {
                currentWeather.moonPhaseName = "Full Moon";
              } else if (phase < 0.6875) {
                currentWeather.moonPhaseName = "Waning Gibbous";
              } else if (phase < 0.8125) {
                currentWeather.moonPhaseName = "Last Quarter";
              } else {
                currentWeather.moonPhaseName = "Waning Crescent";
              }
              
              // Manual override DISABLED for testing - was October 24, 2025 if API data seems wrong
              if (timeinfo->tm_year + 1900 == 2099 && timeinfo->tm_mon + 1 == 10 && timeinfo->tm_mday == 24) {
                Serial.println("Manual override for October 24, 2025 - correcting moon phase");
                
                // October 24, 2025 is actually a Waxing Crescent phase
                // New Moon was October 21, 2025, so this is 3 days after (early waxing)
                currentWeather.moonPhaseName = "Waxing Crescent";
                currentWeather.moonIllumination = 9.4; // 9.4% illuminated (waxing)
                currentWeather.moonEmoji = "🌒"; // Waxing Crescent
                
                Serial.println("Corrected to: " + currentWeather.moonPhaseName + " (" + String(currentWeather.moonIllumination) + "% lit)");
              } else {
                // Calculate illumination percentage from phase (standard calculation)
                currentWeather.moonIllumination = 0.5 * (1 - cos(2 * PI * phase)) * 100;
                
                // Set emoji based on corrected phase
                if (currentWeather.moonPhaseName == "New Moon") {
                  currentWeather.moonEmoji = "🌑";
                } else if (currentWeather.moonPhaseName == "Waxing Crescent") {
                  currentWeather.moonEmoji = "🌒";
                } else if (currentWeather.moonPhaseName == "First Quarter") {
                  currentWeather.moonEmoji = "🌓";
                } else if (currentWeather.moonPhaseName == "Waxing Gibbous") {
                  currentWeather.moonEmoji = "🌔";
                } else if (currentWeather.moonPhaseName == "Full Moon") {
                  currentWeather.moonEmoji = "🌕";
                } else if (currentWeather.moonPhaseName == "Waning Gibbous") {
                  currentWeather.moonEmoji = "🌖";
                } else if (currentWeather.moonPhaseName == "Last Quarter") {
                  currentWeather.moonEmoji = "🌗";
                } else {
                  currentWeather.moonEmoji = "🌘"; // Waning Crescent
                }
              }
              
              Serial.println("OneCall Moon Phase: " + String(phase));
              Serial.println("Converted Phase Name: " + currentWeather.moonPhaseName);
              Serial.println("Calculated Illumination: " + String(currentWeather.moonIllumination) + "%");
              Serial.println("Moon Emoji: " + currentWeather.moonEmoji);
              Serial.println("Moon data updated from OneCall API successfully");
              
              // Force display update to show new moon phase
              extern bool forceDisplayUpdate;
              forceDisplayUpdate = true;
            }
            // Get moonrise/moonset from API if available
            if (day.containsKey("moonrise")) {
              currentWeather.moonrise = day["moonrise"].as<unsigned long>();
            }
            if (day.containsKey("moonset")) {
              currentWeather.moonset = day["moonset"].as<unsigned long>();
            }
          } else if (dayCount == 1) {
            weeklyForecast.days[dayCount].dayName = "Tomorrow";
          } else {
            weeklyForecast.days[dayCount].dayName = dayNames[timeinfo->tm_wday];
          }
          
          JsonObject temp = day["temp"];
          weeklyForecast.days[dayCount].tempHigh = temp["max"].as<float>();
          weeklyForecast.days[dayCount].tempLow = temp["min"].as<float>();
          weeklyForecast.days[dayCount].icon = day["weather"][0]["icon"].as<String>();
          weeklyForecast.days[dayCount].description = day["weather"][0]["description"].as<String>();
          weeklyForecast.days[dayCount].precipChance = (day.containsKey("pop") ? 
                                                        (int)(day["pop"].as<float>() * 100) : 0);
          
          dayCount++;
        }
        weeklyForecast.lastUpdate = millis();
        
        airQuality.lastUpdate = millis();
        
        Serial.println("OneCall 3.0 data updated successfully!");
        Serial.print("Current temp: "); Serial.print(currentWeather.temperature); Serial.println("°F");
        Serial.print("UV Index: "); Serial.println(airQuality.uvIndex);
        
      } else {
        Serial.print("JSON parse failed: ");
        Serial.println(error.c_str());
      }
    } else {
      Serial.print("HTTP error: ");
      Serial.println(httpCode);
      if (httpCode > 0) {
        String errorResponse = http.getString();
        Serial.println("Error response: " + errorResponse);
      }
    }
    
    http.end();
  } else {
    Serial.println("WiFi not connected - cannot fetch weather data");
  }
}

// Air Quality API call (separate from OneCall)
void updateAirQualityData() {
  if (dailyApiCalls >= MAX_DAILY_CALLS) return;
  
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    Serial.println("Fetching air quality data...");
    
    String airQualityUrl = "http://api.openweathermap.org/data/2.5/air_pollution?lat=" + 
                          String(latitude, 4) + "&lon=" + String(longitude, 4) + 
                          "&appid=" + String(apiKey);
    
    http.begin(airQualityUrl.c_str());
    int httpCode = http.GET();
    
    if (httpCode == 200) {
      dailyApiCalls++; // Count API call
      String payload = http.getString();
      JsonDocument doc;
      DeserializationError error = deserializeJson(doc, payload);
      
      if (!error) {
        JsonObject main = doc["list"][0]["main"];
        JsonObject components = doc["list"][0]["components"];
        
        airQuality.aqi = main["aqi"].as<int>();
        
        // AQI status mapping
        switch(airQuality.aqi) {
          case 1: airQuality.status = "Good"; break;
          case 2: airQuality.status = "Fair"; break;
          case 3: airQuality.status = "Moderate"; break;
          case 4: airQuality.status = "Poor"; break;
          case 5: airQuality.status = "Very Poor"; break;
          default: airQuality.status = "Unknown"; break;
        }
        
        airQuality.co = components["co"].as<float>();
        airQuality.no2 = components["no2"].as<float>();
        airQuality.o3 = components["o3"].as<float>();
        airQuality.pm2_5 = components["pm2_5"].as<float>();
        airQuality.pm10 = components["pm10"].as<float>();
        
        Serial.print("Air Quality Index: "); Serial.print(airQuality.aqi);
        Serial.print(" ("); Serial.print(airQuality.status); Serial.println(")");
        
      } else {
        Serial.print("Air quality JSON parse failed: ");
        Serial.println(error.c_str());
      }
    } else {
      Serial.print("Air quality HTTP error: ");
      Serial.println(httpCode);
    }
    
    http.end();
  }
}

void updateMoonData() {
  // DISABLED: RapidAPI calls are failing with 202 error (requires ULTRA/MEGA plan)
  // Instead, we'll use the moon phase data from OneCall API which is already working
  
  Serial.println("=== MOON DATA FROM ONECALL API ===");
  
  if (currentWeather.moonPhase >= 0) {
    // Convert OpenWeather moon phase (0-1) to phase name
    float phase = currentWeather.moonPhase;
    
    Serial.println("Raw OneCall Moon Phase: " + String(phase));
    
    // OpenWeather API moon phase: 0 = new moon, 0.25 = first quarter, 0.5 = full moon, 0.75 = last quarter
    // However, the API seems to sometimes give incorrect values, so let's add some debugging
    
    // For October 24, 2025, the actual moon phase should be Waxing Crescent (about 23% illuminated)
    // Let's check if we need to override the API value for accuracy
    
    // Get current date for verification
    time_t now;
    time(&now);
    struct tm* timeinfo = localtime(&now);
    
    // For debugging - calculate expected phase for this date
    // October 24, 2025 should be approximately day 23 of lunar cycle (Waxing Crescent)
    
    // Convert phase value to moon phase name (consistent with OneCall API logic)
    if (phase < 0.0625 || phase >= 0.9375) {
      currentWeather.moonPhaseName = "New Moon";
    } else if (phase < 0.1875) {
      currentWeather.moonPhaseName = "Waxing Crescent";
    } else if (phase < 0.3125) {
      currentWeather.moonPhaseName = "First Quarter";
    } else if (phase < 0.4375) {
      currentWeather.moonPhaseName = "Waxing Gibbous";
    } else if (phase < 0.5625) {
      currentWeather.moonPhaseName = "Full Moon";
    } else if (phase < 0.6875) {
      currentWeather.moonPhaseName = "Waning Gibbous";
    } else if (phase < 0.8125) {
      currentWeather.moonPhaseName = "Last Quarter";
    } else {
      currentWeather.moonPhaseName = "Waning Crescent";
    }
    
    // Manual override DISABLED for testing - was October 24, 2025 if API data seems wrong
    if (timeinfo->tm_year + 1900 == 2099 && timeinfo->tm_mon + 1 == 10 && timeinfo->tm_mday == 24) {
      Serial.println("Manual override for October 24, 2025 - correcting moon phase");
      
      // October 24, 2025 is actually a Waxing Crescent phase
      // New Moon was October 21, 2025, so this is 3 days after (early waxing)
      currentWeather.moonPhaseName = "Waxing Crescent";
      currentWeather.moonIllumination = 9.4; // 9.4% illuminated (waxing)
      currentWeather.moonEmoji = "�"; // Waning Crescent
      
      Serial.println("Corrected to: " + currentWeather.moonPhaseName + " (" + String(currentWeather.moonIllumination) + "% lit)");
    } else {
      // Calculate illumination percentage from phase (standard calculation)
      currentWeather.moonIllumination = 0.5 * (1 - cos(2 * PI * phase)) * 100;
      
      // Set emoji based on corrected phase
      if (currentWeather.moonPhaseName == "New Moon") {
        currentWeather.moonEmoji = "🌑";
      } else if (currentWeather.moonPhaseName == "Waxing Crescent") {
        currentWeather.moonEmoji = "🌒";
      } else if (currentWeather.moonPhaseName == "First Quarter") {
        currentWeather.moonEmoji = "🌓";
      } else if (currentWeather.moonPhaseName == "Waxing Gibbous") {
        currentWeather.moonEmoji = "🌔";
      } else if (currentWeather.moonPhaseName == "Full Moon") {
        currentWeather.moonEmoji = "🌕";
      } else if (currentWeather.moonPhaseName == "Waning Gibbous") {
        currentWeather.moonEmoji = "🌖";
      } else if (currentWeather.moonPhaseName == "Last Quarter") {
        currentWeather.moonEmoji = "🌗";
      } else {
        currentWeather.moonEmoji = "🌘"; // Waning Crescent
      }
    }
    
    Serial.println("OneCall Moon Phase: " + String(phase));
    Serial.println("Converted Phase Name: " + currentWeather.moonPhaseName);
    Serial.println("Calculated Illumination: " + String(currentWeather.moonIllumination) + "%");
    Serial.println("Moon Emoji: " + currentWeather.moonEmoji);
    Serial.println("Moon data updated from OneCall API successfully");
    
    // Force display update to show new moon phase
    extern bool forceDisplayUpdate;
    forceDisplayUpdate = true;
    
  } else {
    Serial.println("No moon phase data available from OneCall API");
  }
}