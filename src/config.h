/*
 * ESP32 Weather Station Configuration
 * 
 * INSTRUCTIONS: Create a file called 'config_local.h' in this same folder
 * with your personal settings. See README.md for details.
 */

#ifndef CONFIG_H
#define CONFIG_H

// Try to include personal config first
#ifdef __has_include
    #if __has_include("config_local.h")
        #include "config_local.h"
        #define HAS_LOCAL_CONFIG
    #endif
#endif

// If no local config, use these default values
#ifndef HAS_LOCAL_CONFIG
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
    
    // ============================================================================
    // Your Location - CHANGE THESE!
    // ============================================================================
    // Find your coordinates at: https://www.latlong.net/
    #define LATITUDE 40.7128    // Example: New York City
    #define LONGITUDE -74.0060  // Example: New York City
    #define LOCATION_NAME "New York, NY"    // Your city and state for display
#endif

// Common configuration that doesn't need to be secret
#define RAPIDAPI_HOST "moon-phase.p.rapidapi.com"

// ============================================================================
// System Configuration (DO NOT CHANGE)
// ============================================================================
// Display Configuration
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 135
#define ROTATION 1

// Debug Configuration
#define DEBUG_SERIAL true
#define SERIAL_BAUD 115200

// Network Configuration
#define WIFI_TIMEOUT 20000              // 20 seconds
#define HTTP_TIMEOUT 10000              // 10 seconds

// Screen Configuration
#define TOTAL_SCREENS 8

// Screen constants (for main.cpp compatibility)
#define SCREEN_WEATHER 0
#define SCREEN_FORECAST_7DAY 1
#define SCREEN_HOURLY_FORECAST 2
#define SCREEN_AIR_QUALITY 3
#define SCREEN_ASTRONOMY 4
#define SCREEN_SPACE_WEATHER 5
#define SCREEN_AURORA_TODAY 6
#define SCREEN_AURORA_TOMORROW 7

// Alternative screen names
#define CURRENT_WEATHER_SCREEN 0
#define HOURLY_FORECAST_SCREEN 1
#define WEEKLY_FORECAST_SCREEN 2
#define ASTRONOMY_SCREEN 3
#define AIR_QUALITY_SCREEN 4
#define ALERTS_SCREEN 5
#define DETAILED_SCREEN 6
#define ADDITIONAL_SCREEN 7

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

#endif // CONFIG_H