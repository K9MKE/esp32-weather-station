# ESP32 T-Display Weather Station# ESP32 T-Display Weather Station# ESP32 T-Display Weather Station# ESP32 T-Display Weather Station# ESP32 T-Display Weather Station



An accurate weather station with astronomy features for the LILYGO® TTGO T-Display ESP32.



## 🌟 FeaturesAn accurate weather station with astronomy features for the LILYGO® TTGO T-Display ESP32.



- **8 Screen Weather Display**: Current conditions, hourly forecast, daily forecast, astronomy, air quality, and more

- **Accurate Moon Phases**: Manual overrides for precise lunar data

- **Customized Layouts**: Aligned astronomical data and enhanced forecast display## 🌟 FeaturesA comprehensive weather station built with ESP32 T-Display featuring real-time weather data, space weather monitoring, and aurora forecasting across 8 different display screens.

- **Multiple Data Sources**: OneCall 3.0 API for weather, RapidAPI for moon phases

- **Auto-Refresh**: Updates every 10 minutes



## 📱 Display Screens- **8 Screen Weather Display**: Current conditions, hourly forecast, daily forecast, astronomy, air quality, and more



1. **Current Weather** - Temperature, conditions, feels like- **Accurate Moon Phases**: Manual overrides for precise lunar data

2. **Hourly Forecast** - Next 24 hours with icons

3. **7-Day Forecast** - Extended weather outlook- **Customized Layouts**: Aligned astronomical data and enhanced forecast display## 🌟 FeaturesA comprehensive weather station built with ESP32 T-Display featuring real-time weather data, space weather monitoring, and aurora forecasting across 8 different display screens.A beautiful weather station project for the ESP32 T-Display board featuring real-time weather data, stunning graphics, and WiFi connectivity.

4. **Astronomy** - Sun/moon rise/set times, moon phase

5. **Air Quality** - AQI and pollutant levels- **Multiple Data Sources**: OneCall 3.0 API for weather, RapidAPI for moon phases

6. **Weather Alerts** - Severe weather warnings

7. **Detailed Current** - Humidity, pressure, UV index- **Auto-Refresh**: Updates every 10 minutes

8. **Additional Data** - Wind, visibility, dewpoint



## 🛠️ Hardware Required

## 📱 Display Screens- **8 Different Screens** - Current weather, 7-day forecast, hourly forecast, air quality, astronomy, space weather, aurora forecasts

- **LILYGO® TTGO T-Display ESP32** (ST7789 135x240 LCD)

- **WiFi Connection** for API access

- **USB Cable** for programming and power

1. **Current Weather** - Temperature, conditions, feels like- **Real-time Data** - Weather updates every 10 minutes from OpenWeatherMap and NOAA

## 🔧 Setup Instructions

2. **Hourly Forecast** - Next 24 hours with icons

### 1. Install Dependencies

3. **7-Day Forecast** - Extended weather outlook- **Aurora Monitoring** - Space weather data for aurora watching## Features## Features

This project uses PlatformIO. Install the following libraries:

- `TFT_eSPI`4. **Astronomy** - Sun/moon rise/set times, moon phase

- `WiFi`

- `HTTPClient`5. **Air Quality** - AQI and pollutant levels- **Easy Navigation** - Press right button to cycle through screens

- `ArduinoJson`

- `Time`6. **Weather Alerts** - Severe weather warnings



### 2. Configure APIs7. **Detailed Current** - Humidity, pressure, UV index- **WiFi Connected** - No manual updates needed



Edit `src/config.h` and add your credentials:8. **Additional Data** - Wind, visibility, dewpoint



```cpp

// WiFi Settings

#define WIFI_SSID "YOUR_WIFI_SSID"## 🛠️ Hardware Required

#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

## 🛠️ What You Need### 🌤️ Weather & Environmental Data- 🌤️ **Real-time Weather Data** - Current temperature, humidity, pressure, and wind information

// OpenWeatherMap API Key (Free tier available)

#define OPENWEATHER_API_KEY "YOUR_OPENWEATHERMAP_API_KEY"- **LILYGO® TTGO T-Display ESP32** (ST7789 135x240 LCD)



// RapidAPI Key for Moon Phases- **WiFi Connection** for API access

#define RAPIDAPI_KEY "YOUR_RAPIDAPI_KEY"

- **USB Cable** for programming and power

// Your Location Coordinates

#define LATITUDE 40.7128    // Your latitude- **ESP32 T-Display** board (LILYGO TTGO T-Display ESP32 with 1.14" screen)- **Current Weather** - Temperature, humidity, pressure, wind conditions- 📱 **Beautiful TFT Display** - Clean, modern UI with weather icons and color-coded data  

#define LONGITUDE -74.0060  // Your longitude

```## 🔧 Setup Instructions



### 3. Get API Keys- **USB-C Cable** for programming and power



**OpenWeatherMap (Required)**:### 1. Install Dependencies

1. Visit [OpenWeatherMap](https://openweathermap.org/api/one-call-3)

2. Sign up for free account- **WiFi Network** (2.4GHz)- **7-Day Forecast** - Extended weather outlook with precipitation chances- 📡 **WiFi Connectivity** - Fetches data from OpenWeatherMap API

3. Subscribe to "One Call API 3.0" (1000 calls/day free)

4. Copy your API keyThis project uses PlatformIO. Install the following libraries:



**RapidAPI for Moon Phases (Optional)**:- `TFT_eSPI`- **Free OpenWeatherMap Account** (for weather data)

1. Visit [Moon Phase API](https://rapidapi.com/MoonAPIcom/api/moon-phase/)

2. Subscribe to free tier- `WiFi`

3. Copy your API key

- `HTTPClient`- **Hourly Forecast** - 12-hour temperature and precipitation bar graph- ⚡ **Low Power** - Optimized for continuous operation

### 4. Find Your Coordinates

- `ArduinoJson`

Visit [LatLong.net](https://www.latlong.net/) to find your exact coordinates.

- `Time`## 🚀 Quick Setup (3 Steps!)

### 5. Build and Upload



```bash

# Using PlatformIO CLI### 2. Configure APIs- **Air Quality & UV** - AQI, pollutant levels, UV index and risk assessment- 🎨 **Visual Icons** - Weather condition icons and status indicators

pio run --target upload



# Or use VS Code with PlatformIO extension

```Edit `src/config.h` and add your credentials:### Step 1: Download the Code



## 🌙 Moon Phase Features



This weather station includes enhanced moon phase accuracy with manual overrides for specific dates where API data may be inaccurate. The system automatically corrects known discrepancies.```cpp```bash- 🔄 **Auto-refresh** - Updates weather data every 5 minutes



## 📊 Layout Enhancements// WiFi Settings



- **Astronomy Screen**: Aligned sunrise/sunset and moonrise/moonset data#define WIFI_SSID "YOUR_WIFI_SSID"git clone https://github.com/yourusername/esp32-tdisplay-weather.git

- **7-Day Forecast**: Extended weather descriptions with repositioned rain percentages

- **Improved Spacing**: Better visual hierarchy and readability#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"



## 🔄 Data Refresh```### 🌌 Space Weather & Aurora Monitoring



- **Automatic Updates**: Every 10 minutes// OpenWeatherMap API Key (Free tier available)

- **Manual Refresh**: Touch screen or reset button

- **Error Handling**: Graceful fallbacks for API failures#define OPENWEATHER_API_KEY "YOUR_OPENWEATHERMAP_API_KEY"OR download ZIP from GitHub and extract it.



## 🎛️ Configuration Options



All settings are in `src/config.h`:// RapidAPI Key for Moon Phases- **Space Weather** - Combined real-time Kp index, solar wind, magnetic field data, plus solar flux, X-ray activity, and NOAA alerts## Hardware Requirements

- Display brightness

- Refresh intervals#define RAPIDAPI_KEY "YOUR_RAPIDAPI_KEY"

- Temperature units

- Screen rotation### Step 2: Get Your Weather API Key

- Debug modes

// Your Location Coordinates

## 🐛 Troubleshooting

#define LATITUDE 40.7128    // Your latitude1. Go to **[OpenWeatherMap.org](https://openweathermap.org/api)** - **Aurora Forecasts** - Today and tomorrow aurora predictions with activity levels

### WiFi Connection Issues

- Verify SSID and password in config.h#define LONGITUDE -74.0060  // Your longitude

- Check 2.4GHz network (ESP32 doesn't support 5GHz)

- Ensure network allows device connections```2. Click **"Sign Up"** (it's free!)



### API Errors

- Verify API keys are correct

- Check subscription status (One Call 3.0 requires subscription)### 3. Get API Keys3. After signing in, go to **"My API Keys"**- **Astronomy Data** - Sunrise, sunset, moonrise, moonset times with moon phases- **ESP32 T-Display** (CH9102F 16MB)

- Monitor rate limits (1000 calls/day for free tier)



### Display Issues

- Verify TFT_eSPI library configuration**OpenWeatherMap (Required)**:4. **Copy your API key** (looks like: `abc123def456...`)

- Check wiring if using external display

- Ensure correct pin definitions1. Visit [OpenWeatherMap](https://openweathermap.org/api/one-call-3)



## 📁 Project Structure2. Sign up for free account  - Built-in 1.14" TFT display (135x240 pixels)



```3. Subscribe to "One Call API 3.0" (1000 calls/day free)

├── src/

│   ├── main.cpp          # Main application logic4. Copy your API key### Step 3: Add Your WiFi and API Key

│   ├── display.cpp       # Display rendering functions

│   ├── display.h         # Display function declarations

│   └── config.h          # Configuration settings

├── lib/                  # Additional libraries**RapidAPI for Moon Phases (Optional)**:### 📱 Display & Interface  - WiFi connectivity

├── include/              # Header files

└── platformio.ini        # PlatformIO configuration1. Visit [Moon Phase API](https://rapidapi.com/MoonAPIcom/api/moon-phase/)

```

2. Subscribe to free tier**⚠️ BEFORE UPLOADING: You MUST edit your settings! ⚠️**

## 📡 API Attribution & Credits

3. Copy your API key

This project uses the following APIs and services:

- **8 Screen Navigation** - Cycle through all data screens with right button  - USB-C programming interface

### Weather Data

- **OpenWeatherMap One Call API 3.0** - Weather data including current conditions, forecasts, and alerts### 4. Find Your Coordinates

  - Website: https://openweathermap.org/

  - License: Free tier available (1,000 calls/day)Open the file `src/main.cpp` and find these lines (around line 10-15):

  - Attribution: Weather data provided by OpenWeatherMap

  - Terms: https://openweathermap.org/termsVisit [LatLong.net](https://www.latlong.net/) to find your exact coordinates.



### Moon Phase Data  - **Consistent UI** - Standardized headers, WiFi status, and update timestamps

- **Moon Phase API by MoonAPI.com** via RapidAPI

  - Website: https://rapidapi.com/MoonAPIcom/api/moon-phase/### 5. Build and Upload

  - License: Free tier available

  - Attribution: Moon phase data provided by MoonAPI.com```cpp

  - Terms: https://rapidapi.com/terms

```bash

## ⚖️ Legal & License Information

# Using PlatformIO CLI// 🔧 REPLACE THESE WITH YOUR ACTUAL VALUES! 🔧- **Color Coding** - Intuitive color schemes for different data types and alert levels## Software Setup

- This project is for educational and personal use

- Weather data: © OpenWeatherMap contributorspio run --target upload

- Moon phase data: © MoonAPI.com  

- Hardware: LILYGO® TTGO T-Display ESP32const char* WIFI_SSID = "YOUR_WIFI_SSID";

- Display library: TFT_eSPI by Bodmer

- All API usage must comply with respective terms of service# Or use VS Code with PlatformIO extension

- Users are responsible for adhering to API rate limits and terms

```const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";  - **Auto Updates** - Refreshes weather data every 10 minutes

## 🤝 Contributing



Feel free to submit issues and enhancement requests!

## 🌙 Moon Phase Featuresconst char* API_KEY = "YOUR_OPENWEATHERMAP_API_KEY";

## 📄 License



This project is open source. See the LICENSE file for details.

This weather station includes enhanced moon phase accuracy with manual overrides for specific dates where API data may be inaccurate. The system automatically corrects known discrepancies.### Prerequisites

## 🙏 Acknowledgments



- **LILYGO** for the excellent T-Display ESP32 hardware

- **OpenWeatherMap** for comprehensive weather data API  ## 📊 Layout Enhancements// 📍 CHANGE TO YOUR LOCATION 📍

- **MoonAPI.com** for accurate lunar phase calculations

- **Bodmer** for the powerful TFT_eSPI display library

- **Arduino/ESP32 community** for extensive documentation and support

- **Astronomy Screen**: Aligned sunrise/sunset and moonrise/moonset dataconst float LATITUDE = 40.7128;   // Your latitude## Hardware Requirements1. Install [Visual Studio Code](https://code.visualstudio.com/)

---

- **7-Day Forecast**: Extended weather descriptions with repositioned rain percentages

**⚠️ Important Notice**: Before using this project, ensure you have valid API keys and comply with all terms of service for the respective APIs. This project is intended for personal, educational, and non-commercial use.
- **Improved Spacing**: Better visual hierarchy and readabilityconst float LONGITUDE = -74.0060; // Your longitude



## 🔄 Data Refreshconst char* LOCATION_NAME = "Your City";2. Install [PlatformIO Extension](https://platformio.org/install/ide?install=vscode)



- **Automatic Updates**: Every 10 minutes```

- **Manual Refresh**: Touch screen or reset button

- **Error Handling**: Graceful fallbacks for API failures- **ESP32 T-Display** (LILYGO TTGO T-Display ESP32 1.14" TFT)



## 🎛️ Configuration Options**Replace them with your actual information:**



All settings are in `src/config.h`:```cpp  - 240x135 pixel ST7789 display### Configuration

- Display brightness

- Refresh intervalsconst char* WIFI_SSID = "MyHomeWiFi";              // ← Your WiFi name

- Temperature units

- Screen rotationconst char* WIFI_PASSWORD = "mypassword123";       // ← Your WiFi password  - Built-in buttons and WiFi1. **WiFi Setup**: Edit `include/config.h` and replace:

- Debug modes

const char* API_KEY = "abc123def456ghi789...";     // ← Your API key from Step 2

## 🐛 Troubleshooting

  - CH9102F USB-C connector   ```cpp

### WiFi Connection Issues

- Verify SSID and password in config.hconst float LATITUDE = 34.0522;                    // ← Your latitude  

- Check 2.4GHz network (ESP32 doesn't support 5GHz)

- Ensure network allows device connectionsconst float LONGITUDE = -118.2437;                 // ← Your longitude- **Power Supply** - USB-C cable or battery pack   const char* WIFI_SSID = "YOUR_WIFI_SSID";



### API Errorsconst char* LOCATION_NAME = "Los Angeles";         // ← Your city name

- Verify API keys are correct

- Check subscription status (One Call 3.0 requires subscription)```   const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

- Monitor rate limits (1000 calls/day for free tier)



### Display Issues

- Verify TFT_eSPI library configuration**🗺️ How to Find Your Coordinates:**## API Services Used   ```

- Check wiring if using external display

- Ensure correct pin definitions1. Open **[Google Maps](https://maps.google.com)**



## 📁 Project Structure2. **Right-click** on your location  



```3. Click **"What's here?"**

├── src/

│   ├── main.cpp          # Main application logic4. **Copy the numbers** that appear (example: 34.0522, -118.2437)- **OpenWeatherMap OneCall 3.0** - Current weather, forecasts, air quality, UV data2. **Weather API Setup**:

│   ├── display.cpp       # Display rendering functions

│   ├── display.h         # Display function declarations

│   └── config.h          # Configuration settings

├── lib/                  # Additional libraries## 📤 Upload to Your ESP32- **NOAA Space Weather Prediction Center** - Real-time space weather data including:   - Get a free API key from [OpenWeatherMap](https://openweathermap.org/api)

├── include/              # Header files

└── platformio.ini        # PlatformIO configuration

```

### Using PlatformIO (Recommended)  - Planetary K-index   - Replace in `include/config.h`:

## 🤝 Contributing

1. Install **[VS Code](https://code.visualstudio.com/)**

Feel free to submit issues and enhancement requests!

2. Install **PlatformIO extension** in VS Code  - Solar wind plasma and magnetic field data     ```cpp

## 📄 License

3. Open this project folder in VS Code

This project is open source. See the LICENSE file for details.

4. Connect your ESP32 with USB-C cable  - Solar flux index (10.7 cm)   const char* API_KEY = "YOUR_API_KEY";

## 🙏 Acknowledgments

5. Click **"Upload"** button (→) in VS Code

- LILYGO for the T-Display hardware

- OpenWeatherMap for weather data API  - X-ray flux classification   const char* CITY_NAME = "London";  // Your city

- TFT_eSPI library for display control
### Using Arduino IDE  

1. Install **Arduino IDE**  - Geomagnetic activity indices   ```

2. Install **ESP32 board package**

3. Install libraries: TFT_eSPI, ArduinoJson  - Space weather alerts and warnings

4. Select board: **"ESP32 Dev Module"**

5. Click **Upload**### Building and Uploading



## 🖱️ How to Use## Software Requirements1. Connect your ESP32 T-Display via USB-C



**Press the RIGHT BUTTON** to cycle through these 8 screens:2. Open this project in VS Code with PlatformIO



1. **🌤️ Current Weather** - Temperature, humidity, conditions- **PlatformIO** (recommended) or Arduino IDE3. Press `Ctrl+Shift+P` and type "PlatformIO: Build" to compile

2. **📅 7-Day Forecast** - Week outlook with rain chances  

3. **📊 Hourly Forecast** - 12-hour bar graph- **Required Libraries** (auto-installed with PlatformIO):4. Press `Ctrl+Shift+P` and type "PlatformIO: Upload" to flash to your board

4. **🌬️ Air Quality & UV** - Pollution levels and UV index

5. **🌙 Astronomy** - Sunrise, sunset, moon phases  - `TFT_eSPI` - Display driver

6. **🌌 Space Weather** - Aurora conditions and solar activity

7. **🔮 Aurora Today** - Today's aurora forecast  - `ArduinoJson` v7+ - JSON parsing## Display Layout

8. **🔮 Aurora Tomorrow** - Tomorrow's aurora forecast

  - `WiFi` - ESP32 WiFi connectivity

## ⚙️ Optional: Adjust Your Timezone

  - `HTTPClient` - API requests```

If the time shows incorrectly, edit this line in `src/main.cpp`:

  - `time.h` - Time synchronization┌─────────────────────────────┐

```cpp

// Change the first number for your timezone:│ City Name           Time    │

configTime(-6 * 3600, 3600, "pool.ntp.org", "time.nist.gov");

```## Setup Instructions├─────────────────────────────┤



**Common Timezone Values:**│ ☀ Weather Description      │  

- **Eastern Time**: `-5 * 3600`  

- **Central Time**: `-6 * 3600`### 1. Clone the Repository│                             │

- **Mountain Time**: `-7 * 3600`

- **Pacific Time**: `-8 * 3600````bash│     25°C                    │

- **UTC**: `0 * 3600`

git clone https://github.com/yourusername/esp32-tdisplay-weather.git│                             │

## 🚨 Troubleshooting

cd esp32-tdisplay-weather│ 💧 65%    🔽 1013hPa       │

### "WiFi Connection Failed"

- ✅ Check your WiFi name and password are exactly correct (case sensitive!)```│ 💨 2.5m/s 180°            │

- ✅ Make sure you're using 2.4GHz WiFi (ESP32 doesn't work with 5GHz)

- ✅ Try moving closer to your router│                      WiFi   │



### "API Error" or No Weather Data### 2. Get API Keys└─────────────────────────────┘

- ✅ Verify your OpenWeatherMap API key is correct

- ✅ Make sure you subscribed to the "OneCall API 3.0" (free tier)  ```

- ✅ Check your coordinates are in decimal format (34.0522, not 34°3'8")

#### OpenWeatherMap API

### "Upload Failed" or Programming Errors

- ✅ Make sure your ESP32 T-Display is connected via USB-C1. Sign up at [OpenWeatherMap](https://openweathermap.org/api)## Customization

- ✅ Try a different USB cable or port

- ✅ Press and hold the BOOT button while uploading if needed2. Subscribe to **OneCall API 3.0** (1000 calls/day free tier)



### Display Issues3. Copy your API key### Colors

- ✅ Make sure you selected the correct ESP32 board type

- ✅ Try pressing the reset button on the ESP32Modify colors in `include/config.h`:



## 🔧 Customization### 3. Configure Settings- `COLOR_TEMP` - Temperature display color



### Change Update Frequency- `COLOR_HUMIDITY` - Humidity display color  

In `src/main.cpp`, modify this line:

```cppEdit `src/main.cpp` and update these values:- `COLOR_PRESSURE` - Pressure display color

const unsigned long WEATHER_UPDATE_INTERVAL = 600000; // 10 minutes (in milliseconds)

```- `COLOR_WIND` - Wind display color



### Change Colors  ```cpp- `COLOR_ACCENT` - Accent color for highlights

In `include/config.h`, modify the color definitions:

```cpp// WiFi Configuration

#define COLOR_TEMP 0xF81F      // Temperature color (magenta)

#define COLOR_HUMIDITY 0x07FF  // Humidity color (cyan)const char* WIFI_SSID = "YOUR_WIFI_SSID";### Update Intervals

// etc...

```const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";Change update frequency in `src/main.cpp`:



## 📊 What APIs This Usesconst char* API_KEY = "YOUR_OPENWEATHERMAP_API_KEY";```cpp



- **OpenWeatherMap OneCall 3.0** - Weather, forecasts, air quality (free: 1000 calls/day)const unsigned long WEATHER_UPDATE_INTERVAL = 300000; // 5 minutes in milliseconds

- **NOAA Space Weather** - Aurora and space weather data (free, no limits)

// Location Configuration  ```

## 📄 License

const float LATITUDE = 40.7128;   // Your latitude

This project is open source under the MIT License. Feel free to modify and share!

const float LONGITUDE = -74.0060; // Your longitude### Weather Icons

## 🙏 Credits

const char* LOCATION_NAME = "Your City"; // Display nameWeather icons are displayed as Unicode symbols. Customize in `src/display.cpp` function `drawWeatherIcon()`.

- **OpenWeatherMap** - Weather data

- **NOAA** - Space weather data```

- **LILYGO** - ESP32 T-Display hardware  

- **Bodmer** - TFT_eSPI display library## API Usage



---**Finding Your Coordinates:**



**💡 Need Help?** Check the serial monitor output at 115200 baud for debugging information!- Use [LatLong.net](https://www.latlong.net/) or Google MapsThe project uses the OpenWeatherMap Current Weather Data API:

- Right-click your location → "What's here?"- **Endpoint**: `http://api.openweathermap.org/data/2.5/weather`

- **Rate Limit**: 60 calls/minute (free tier)

### 4. Adjust Timezone (Optional)- **Update Frequency**: 5 minutes (well within limits)



In `src/main.cpp`, modify the timezone configuration:## Troubleshooting



```cpp### WiFi Connection Issues

// Use your local timezone (adjust as needed)- Verify SSID and password in `config.h`

configTime(-6 * 3600, 3600, "pool.ntp.org", "time.nist.gov");- Check 2.4GHz network compatibility

```- Monitor serial output at 115200 baud



Common timezone offsets:### Display Issues  

- **EST/EDT**: `-5 * 3600` (Eastern Time)- Ensure correct TFT_eSPI pin configuration in `platformio.ini`

- **CST/CDT**: `-6 * 3600` (Central Time)  - Check display orientation setting in `setup()`

- **MST/MDT**: `-7 * 3600` (Mountain Time)

- **PST/PDT**: `-8 * 3600` (Pacific Time)### API Issues

- Verify API key is valid and active

### 5. Upload to ESP32- Check city name spelling

- Monitor HTTP response codes in serial output

#### Using PlatformIO (Recommended)

```bash## Libraries Used

# Install PlatformIO if not already installed

pip install platformio- **TFT_eSPI** (v2.5.43) - Display control

- **ArduinoJson** (v7.0.4) - JSON parsing

# Build and upload- **WiFi** (v1.2.7) - Network connectivity

pio run --target upload

```## License



#### Using Arduino IDEThis project is open source. Feel free to modify and distribute as needed.

1. Install ESP32 board package

2. Install required libraries via Library Manager## Support

3. Select "ESP32 Dev Module" as board

4. Upload sketchFor issues and questions:

1. Check the serial monitor output for debugging info

## Screen Navigation2. Verify all configuration settings

3. Ensure stable WiFi connection and valid API credentials
Press the **right button** to cycle through screens:

1. **Current Weather** - Live conditions with large temperature display
2. **7-Day Forecast** - Weekly outlook in compact table format  
3. **Hourly Forecast** - 12-hour bar graph with temps and precipitation
4. **Air Quality & UV** - Environmental health data
5. **Astronomy** - Sun/moon times and phases
6. **Space Weather** - Aurora conditions + solar activity monitoring
7. **Aurora Today** - Current day aurora forecast
8. **Aurora Tomorrow** - Next day aurora predictions

## Screen Details

### Current Weather
- Large temperature display with weather icon
- Humidity, pressure, and wind conditions  
- City name and current time in header

### 7-Day Forecast  
- Compact 7-day table showing all days
- Low/high temperatures, conditions, rain chances
- Shortened day names to prevent text overlap

### Hourly Forecast
- 12-hour temperature bar graph
- Color-coded bars (blue=freezing, red=hot)
- Temperature values and precipitation percentages below bars

### Air Quality & UV
- Air Quality Index (AQI) with descriptive status
- Individual pollutant measurements (PM2.5, PM10, NO₂, O₃, CO)
- UV Index with risk level color coding
- Visibility distance

### Space Weather (Combined NOAA Data)
**Left Column - Aurora Conditions:**
- Kp Index (color-coded aurora activity level)
- Aurora Forecast (HIGH/MODERATE/LOW/MINIMAL)
- Bz Magnetic Field (negative values favor aurora)
- Solar Wind speed

**Right Column - Solar Activity:**  
- SFI (Solar Flux Index)
- X-ray flux class (A/B/C/M/X scale)
- A-index (daily geomagnetic activity)
- Sunspot count
- NOAA alert status

### Aurora Forecasts
- Predicted Kp values with color coding
- Activity level descriptions
- Visibility regions and peak times
- Confidence ratings

## API Rate Limits & Caching

- **Update Interval**: 10 minutes (configurable)
- **OneCall 3.0**: 1000 calls/day limit (144 calls/day at 10min intervals)
- **NOAA APIs**: No strict limits, respectful usage
- **Error Handling**: Graceful degradation if APIs are unavailable

## Troubleshooting

### WiFi Connection Issues
- Verify SSID and password are correct
- Check 2.4GHz network availability (ESP32 doesn't support 5GHz)
- Monitor serial output for connection status

### API Issues  
- Confirm OpenWeatherMap API key is valid and OneCall 3.0 is enabled
- Check daily API usage in OpenWeatherMap dashboard
- Verify coordinates are in decimal degrees format

### Display Issues
- Ensure proper ESP32 T-Display board selection
- Check TFT_eSPI library configuration
- Verify button pin definitions match your board

### Time Synchronization
- NTP servers require internet connectivity
- Timezone offset may need adjustment for your location
- Check serial output for time sync confirmation

## Customization

### Adding New Screens
1. Define new screen constant in `include/config.h`
2. Add display function in `src/display.cpp` 
3. Include in screen cycling logic in `src/main.cpp`
4. Add data fetching if needed

### Modifying Update Intervals
```cpp
// In src/main.cpp
const unsigned long WEATHER_UPDATE_INTERVAL = 600000; // 10 minutes
```

### Color Scheme Changes
```cpp
// In include/config.h
#define COLOR_TEMP 0xF81F          // Temperature color
#define COLOR_HUMIDITY 0x07FF      // Humidity color  
// ... modify other color definitions
```

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## License

This project is open source and available under the [MIT License](LICENSE).

## Acknowledgments

- **OpenWeatherMap** - Weather and air quality data
- **NOAA Space Weather Prediction Center** - Space weather and aurora data  
- **LILYGO** - ESP32 T-Display hardware
- **TFT_eSPI Library** - Display driver by Bodmer

## Changelog

### v1.0.0
- Initial release with 8 comprehensive weather and space weather screens
- OneCall 3.0 API integration
- Combined NOAA space weather monitoring  
- Standardized UI with consistent headers and status indicators
- 12-hour bar graph hourly forecasts
- Complete aurora monitoring system

---

**Note**: This project is for educational and personal use. Please respect API terms of service and rate limits.