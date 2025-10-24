/*
 * ESP32 Weather Station Configuration
 * 
 * INSTRUCTIONS: Replace the values below with your own settings
 */

#ifndef CONFIG_H
#define CONFIG_H

// ============================================================================
// WiFi Configuration - CHANGE THESE!
// ============================================================================
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

// ============================================================================
// OpenWeatherMap API Key - CHANGE THIS!
// ============================================================================
// Get your free API key from: https://openweathermap.org/api/one-call-3
#define OPENWEATHER_API_KEY "YOUR_OPENWEATHERMAP_API_KEY"

// ============================================================================
// RapidAPI Moon Phase API - CHANGE THIS!
// ============================================================================
// Get your API key from: https://rapidapi.com/MoonAPIcom/api/moon-phase/
#define RAPIDAPI_KEY "YOUR_RAPIDAPI_KEY"
#define RAPIDAPI_HOST "moon-phase.p.rapidapi.com"

// ============================================================================
// Your Location - CHANGE THESE!
// ============================================================================
// Find your coordinates at: https://www.latlong.net/
#define LATITUDE 40.7128    // Example: New York City
#define LONGITUDE -74.0060  // Example: New York City

// ============================================================================
// System Configuration (DO NOT CHANGE)
// ============================================================================
// Display Configuration
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 135

// Colors (RGB565 format)
#define COLOR_BACKGROUND 0x0000    // Black
#define COLOR_TEXT 0xFFFF          // White
#define COLOR_TEMP 0xF81F          // Magenta
#define COLOR_HUMIDITY 0x07FF      // Cyan
#define COLOR_PRESSURE 0xFFE0      // Yellow
#define COLOR_WIND 0x07E0          // Green
#define COLOR_ACCENT 0x841F        // Purple

// Button Configuration (T-Display)
#define RIGHT_BUTTON_PIN 35
#define LEFT_BUTTON_PIN 0

// Screen modes
#define SCREEN_WEATHER 0
#define SCREEN_FORECAST_7DAY 1
#define SCREEN_HOURLY_FORECAST 2
#define SCREEN_AIR_QUALITY 3
#define SCREEN_ASTRONOMY 4
#define SCREEN_SPACE_WEATHER 5
#define SCREEN_AURORA_TODAY 6
#define SCREEN_AURORA_TOMORROW 7

#endif // CONFIG_H