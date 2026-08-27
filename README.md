# wifi-phishing-captive-portal
ESP32 script to create a Rogue AP with a captive portal. Upon client connection, it redirects traffic to a fake Google login page and saves harvested credentials to EEPROM. Features a web admin panel (via admin/admin) for credential management and dynamic SSID switching from a built-in public Wi-Fi list. Intended for security research.
