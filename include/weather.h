#ifndef WEATHER_H
#define WEATHER_H

#include <Arduino.h>

struct WeatherData {
  float temperature;
  int humidity;
  float pressure;
  String description;
  String icon;
  String cityName;
  float windSpeed;
  int windDirection;
  unsigned long lastUpdate;
  // Astronomical data (Unix timestamps)
  unsigned long sunrise;
  unsigned long sunset;
  unsigned long moonrise;
  unsigned long moonset;
  // Moon phase data
  float moonPhase;         // 0-1 (0=new moon, 0.25=first quarter, 0.5=full, 0.75=last quarter)
  String moonPhaseName;    // "New Moon", "First Quarter", etc.
  float moonIllumination;  // Percentage illuminated (0-100)
  float moonAge;           // Age in days since new moon
  String moonEmoji;        // Unicode emoji representing current moon phase
};

struct SpaceWeatherData {
  float kpIndex;           // Planetary K-index (0-9, aurora activity)
  float solarWindSpeed;    // km/s (typical: 300-800)
  float solarWindDensity;  // protons/cm³
  float magneticFieldBz;   // nT (southward = negative, aurora favorable)
  String auroraForecast;   // Text forecast
  String geomagStatus;     // Quiet/Unsettled/Active/Storm
  unsigned long lastUpdate;
};

struct DayForecast {
  String dayName;          // "Mon", "Tue", etc.
  float tempHigh;          // High temperature
  float tempLow;           // Low temperature
  String icon;             // Weather icon code
  String description;      // Brief description
  int precipChance;        // Precipitation chance %
};

struct WeeklyForecast {
  DayForecast days[7];     // 7 day forecast
  unsigned long lastUpdate;
};

struct AuroraForecastData {
  String date;             // "Today" or "Tomorrow" or date
  float kpPredicted;       // Predicted KP index
  String activity;         // "Low", "Moderate", "High", "Very High"
  String visibility;       // Geographic visibility info
  String peakTime;         // Expected peak activity time
  String confidence;       // "Low", "Medium", "High"
  unsigned long lastUpdate;
};

struct HourlyForecast {
  String time;             // "2PM", "3PM", etc.
  float temperature;       // Temperature
  String icon;             // Weather icon
  String description;      // Short description
  int precipChance;        // Precipitation chance %
  int humidity;            // Humidity %
};

struct HourlyForecastData {
  HourlyForecast hours[12]; // Next 12 hours
  unsigned long lastUpdate;
};

struct AirQualityData {
  int aqi;                 // Air Quality Index (1-5)
  String status;           // "Good", "Fair", "Moderate", etc.
  float co;                // Carbon monoxide μg/m³
  float no2;               // Nitrogen dioxide μg/m³
  float o3;                // Ozone μg/m³
  float pm2_5;             // PM2.5 μg/m³
  float pm10;              // PM10 μg/m³
  int uvIndex;             // UV Index (0-11+)
  String uvRisk;           // "Low", "Moderate", "High", "Very High", "Extreme"
  float visibility;        // Visibility in km
  unsigned long lastUpdate;
};

struct NOAASpaceWeatherData {
  float solarFluxIndex;    // SFI (Solar Flux Index) - 10.7 cm radio flux
  float aIndex;            // A-index (daily geomagnetic activity)
  float kpIndex;           // Kp index (3-hour geomagnetic activity)
  String xrayFlux;         // Current X-ray flux level (A, B, C, M, X class)
  String protonFlux;       // Proton flux level
  String electronFlux;     // Electron flux level
  int sunspotNumber;       // Daily sunspot number
  String alerts[5];        // Space weather alerts/warnings
  int alertCount;          // Number of active alerts
  String summary;          // Current conditions summary
  unsigned long lastUpdate;
};

extern WeatherData currentWeather;
extern SpaceWeatherData currentSpaceWeather;
extern WeeklyForecast weeklyForecast;
extern AuroraForecastData auroraToday;
extern AuroraForecastData auroraTomorrow;
extern HourlyForecastData hourlyForecast;
extern AirQualityData airQuality;
extern NOAASpaceWeatherData noaaSpaceWeather;

// Function declarations
void parseWeatherData(String jsonString);
void updateWeatherData();
void updateSpaceWeatherData();
void update7DayForecast();
void updateAuroraForecast();
void updateNOAASpaceWeatherData();

#endif