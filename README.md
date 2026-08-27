# ESP32 Captive Portal Proof of Concept

⚠️ **IMPORTANT NOTICE:** 
*This project is intended **strictly for educational purposes, security research (Red Teaming), and authorized penetration testing** with explicit consent. Using this code to harvest personal credentials without authorization is illegal. The author assumes no liability for any misuse of this software.*

This project turns an ESP32 into a Rogue Access Point. Upon startup, it broadcasts a Wi-Fi network and intercepts DNS traffic to force clients into a captive portal featuring a replica of the Google Sign-In page. Captured credentials are saved to non-volatile storage (EEPROM) and can be managed via an integrated web admin panel.

## Preview

| Captive Portal (Google) | Admin Dashboard |
| :---: | :---: |
| ![Google Login Portal](images/00.jpg) | ![Management Panel](images/01.jpg) |

## Features

* **Captive Portal:** Automatically triggers the operating system's native captive portal handler upon connection (iOS, Android, Windows).
* **DNS Spoofing:** Redirects all client web requests to the ESP32 local web server.
* **Realistic Template:** Embedded Google login page layout using PROGMEM for fast rendering.
* **Persistent Storage:** Saves up to 20 credentials in EEPROM, surviving device reboots.
* **Web Management UI:** Fully functional admin dashboard accessible directly through the browser.
* **Dynamic SSID Rotation:** Includes a built-in list of popular public Wi-Fi profiles that can be switched on the fly.

## How It Works

1. **Access Point & DNS:** The ESP32 operates in `WIFI_AP` mode. The DNS server (`DNSServer`) listens on port 53 and responds to all domain lookups with its own IP address.
2. **Web Server:** The `WebServer` listens on port 80. Unauthenticated traffic hits `handleRoot()`, serving the phishing template.
3. **Data Harvesting:** Submitting credentials triggers `/get` (`handleGet()`), which parses the parameters and writes them directly to EEPROM.
4. **Admin Panel:** Navigating to `/admin` generates a management interface to view captured logs, delete entries, or change the active SSID and password.

## Installation & Usage

1. Open the script in **Arduino IDE**.
2. Ensure you have the **ESP32 board package** installed.
3. Upload the sketch to your ESP32 board.
4. Connect to the broadcasted Wi-Fi network (**`FREE-WIFI`**).

## Accessing the Admin Panel

1. Connect to the rogue network.
2. When the login page appears, use the administrative backdoor:
   - **Email:** `admin`
   - **Password:** `admin`
3. Click "Next". The portal will automatically authenticate and redirect you to `/admin`.
4. From the admin panel, you can monitor connected stations, view harvested credentials, clear storage, or change the SSID profile.

## EEPROM Layout

* Configured to store a maximum of `20` records (`MAX_SAVED`).
* Each entry allocates 40 bytes for the username/email and 40 bytes for the password.
* Metadata counter is stored at EEPROM address 0.

## License
Provided "as is" without warranty. Use responsibly.
