#include <WiFi.h>
#include <WebServer.h>

// --- WiFi Credentials ---
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";


WebServer server(80);

const int plantPin = 34;
const int oversampleCount = 1000;

// This function creates the webpage you see in the browser
void handleRoot() {
  // Brute Force Sampling (1000 samples for accuracy)
  long sum = 0;
  for(int i = 0; i < 1000; i++) {
    sum += analogRead(plantPin);
    delayMicroseconds(50);
  }
  int val = sum / 1000;

  // Convert Raw (0-4095) to mV (0-3300mV)
  float mV = (val / 4095.0) * 3300.0;

  String html = "<html><head><meta http-equiv='refresh' content='1'>";
  html += "<style>body{font-family:sans-serif; text-align:center; background:#1a1a1a; color:white; padding-top:50px;}";
  html += ".box{display:inline-block; padding:20px; margin:10px; border-radius:15px; background:#333; min-width:150px;}";
  html += "#current{font-size:60px; color:#00ff88;}</style></head><body>";
  
  html += "<h1>Lily Bio-Monitor</h1>";
  html += "<div class='box'><h3>Current</h3><div id='current'>" + String(mV, 1) + " mV</div></div><br>";
  html += "<div class='box'><h3>Session Highest</h3><div id='hi'>--</div></div>";
  html += "<div class='box'><h3>Session Lowest</h3><div id='lo'>--</div></div>";
  html += "<br><button onclick='localStorage.clear(); location.reload();' style='margin-top:20px; padding:10px;'>Reset Records</button>";

  // The Magic Script: Compares current value to stored Max/Min
  html += "<script>";
  html += "let cur = " + String(mV, 1) + ";";
  html += "let hi = localStorage.getItem('lilyHi') || -9999;";
  html += "let lo = localStorage.getItem('lilyLo') || 9999;";
  html += "if(cur > hi) { localStorage.setItem('lilyHi', cur); hi = cur; }";
  html += "if(cur < lo) { localStorage.setItem('lilyLo', cur); lo = cur; }";
  html += "document.getElementById('hi').innerText = hi + ' mV';";
  html += "document.getElementById('lo').innerText = lo + ' mV';";
  html += "</script></body></html>";

  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);

  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nWiFi Connected!");
  Serial.print("Go to this address in your browser: http://");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient(); // Keeps the website running
}
