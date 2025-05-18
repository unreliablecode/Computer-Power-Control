#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "YOUR SSID";
const char* password = "YOUR PASSWORD";

IPAddress local_IP(192, 168, 1, 82);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

const int pin32 = 32;

WebServer server(32);

bool loggedIn = false;

const char* loginUser = "admin";
const char* loginPass = "admin";

void setup() {
  Serial.begin(115200);
  delay(100);

  pinMode(pin32, OUTPUT);
  digitalWrite(pin32, LOW);

  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("Failed to configure Static IP");
  }

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi..");
  unsigned long wifiStart = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (millis() - wifiStart > 15000) {
      Serial.println("WiFi connect timeout.");
      ESP.restart();
    }
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/login", handleLogin);
  server.on("/logout", handleLogout);
  server.on("/toggle", handleToggle);
  server.on("/check_server", handleCheckServer);
  server.begin();
  Serial.println("Web server running on port 80");
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  String html = R"rawliteral(
  <!DOCTYPE html><html>
  <head>
  <meta name='viewport' content='width=device-width, initial-scale=1'>
  <title>ESP32 Control</title>
  <style>
    body {
      margin:0;
      padding:0;
      background: linear-gradient(135deg, #1f1c2c, #928dab);
      font-family: 'Arial', sans-serif;
      display: flex;
      flex-direction: column;
      align-items: center;
      justify-content: center;
      height: 100vh;
      color: white;
    }
    .glass {
      backdrop-filter: blur(15px);
      background: rgba(255,255,255,0.05);
      border: 1px solid rgba(255,255,255,0.2);
      border-radius: 20px;
      padding: 40px;
      text-align: center;
      box-shadow: 0 8px 32px 0 rgba( 31, 38, 135, 0.37 );
    }
    .button {
      margin: 10px;
      padding: 15px 40px;
      font-size: 18px;
      color: #fff;
      background: rgba(255,255,255,0.1);
      border: 1px solid rgba(255,255,255,0.3);
      border-radius: 50px;
      cursor: pointer;
      backdrop-filter: blur(10px);
      transition: all 0.3s ease;
    }
    .button:hover {
      background: rgba(255,255,255,0.3);
      transform: scale(1.05);
    }
    input {
      padding: 12px;
      border-radius: 10px;
      border: none;
      margin: 5px;
    }
    #status {
      font-weight:bold;
      margin-top:20px;
    }
  </style>
  </head>
  <body>
  <div class='glass'>
    <h2>Server Power Control</h2>
  )rawliteral";

  if (loggedIn) {
    html += "<p>Welcome, admin!</p>";
    html += "<button class='button' onclick=\"location.href='/toggle'\">Toggle Relay</button>";
    html += "<button class='button' onclick=\"location.href='/logout'\">Logout</button>";
    html += "<br><br><button class='button' onclick='checkServer()'>Check Server 192.168.1.7</button>";
    html += "<div id='status'></div>";
  } else {
    html += R"rawliteral(
      <form action='/login' method='GET'>
        <input type='text' name='username' placeholder='Username' required><br>
        <input type='password' name='password' placeholder='Password' required><br>
        <input class='button' type='submit' value='Login'>
      </form>
    )rawliteral";
  }

  html += R"rawliteral(
  </div>
  <script>
  function checkServer(){
    document.getElementById("status").innerHTML="Checking...";
    fetch('/check_server')
      .then(res => res.text())
      .then(data => document.getElementById("status").innerHTML = data)
      .catch(err => document.getElementById("status").innerHTML = "Error contacting ESP32.");
  }
  </script>
  </body></html>
  )rawliteral";

  server.send(200, "text/html", html);
}

void handleLogin() {
  if (server.hasArg("username") && server.hasArg("password")) {
    String user = server.arg("username");
    String pass = server.arg("password");
    if (user == loginUser && pass == loginPass) {
      loggedIn = true;
      server.sendHeader("Location", "/");
      server.send(303);
      return;
    }
  }
  server.send(200, "text/html", "<p>Invalid login. <a href='/'>Try again</a></p>");
}

void handleLogout() {
  loggedIn = false;
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleToggle() {
  if (loggedIn) {
    digitalWrite(pin32, HIGH);
    delay(500);
    digitalWrite(pin32, LOW);
    server.sendHeader("Location", "/");
    server.send(303);
  } else {
    server.send(403, "text/html", "<p>Unauthorized. <a href='/'>Login</a></p>");
  }
}

void handleCheckServer() {
  if (!loggedIn) {
    server.send(403, "text/plain", "Unauthorized");
    return;
  }

  WiFiClient client;
  bool reachable = false;

  unsigned long startAttempt = millis();
  if (client.connect("192.168.1.7", 80)) {
    reachable = true;
    client.stop();
  }
  unsigned long elapsed = millis() - startAttempt;

  if (reachable) {
    server.send(200, "text/plain", "✅ Server is ONLINE (Response in " + String(elapsed) + "ms)");
  } else {
    server.send(200, "text/plain", "❌ Server is OFFLINE");
  }
}
