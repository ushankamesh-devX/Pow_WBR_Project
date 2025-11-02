#ifndef WEBCONTROLLER_H
#define WEBCONTROLLER_H

#include <WiFi.h>
#include <WebServer.h>
#include "Params.h"

/**
 * @class WebController
 * @brief WiFi Access Point with web interface for robot control
 */
class WebController {
private:
  WebServer server;
  
  // Control variables
  float h_d;      // Desired height (m)
  float phi_d;    // Desired roll angle (degrees)
  float v_d;      // Desired velocity (m/s)
  float psi_d;    // Desired yaw rate (rad/s)
  bool is_run;    // Run/Stop state
  bool is_reset;  // Reset flag

public:
  WebController() : server(webServerPort) {
    h_d = HEIGHT_MAX;
    phi_d = 0;
    v_d = 0;
    psi_d = 0;
    is_run = false;
    is_reset = false;
  }

  /**
   * @brief Initialize WiFi Access Point and web server
   */
  void begin() {
    // Create WiFi Access Point
    Serial.println("Creating WiFi Access Point...");
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);
    
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);
    Serial.println("Connect to WiFi and visit http://192.168.4.1");
    
    // Setup web server routes
    server.on("/", [this]() { handleRoot(); });
    server.on("/control", [this]() { handleControl(); });
    server.on("/status", [this]() { handleStatus(); });
    server.on("/data", [this]() { handleData(); });
    
    server.begin();
    Serial.println("Web server started");
  }

  /**
   * @brief Handle client requests (call in loop)
   */
  void handleClient() {
    server.handleClient();
  }

  // Getters
  float getDesiredHeight() const { return h_d; }
  float getDesiredRoll() const { return phi_d; }
  float getDesiredVel() const { return v_d; }
  float getDesiredYawVel() const { return psi_d; }
  bool isRun() const { return is_run; }
  bool isReset() { 
    bool temp = is_reset;
    is_reset = false;  // Auto-clear reset flag
    return temp;
  }

  // Setters (for status updates)
  void setStatus(float h, float v, float psi, float ax, float ay, float az) {
    current_h = h;
    current_v = v;
    current_psi = psi;
    current_ax = ax;
    current_ay = ay;
    current_az = az;
  }

private:
  // Status variables
  float current_h, current_v, current_psi;
  float current_ax, current_ay, current_az;

  /**
   * @brief Serve main control webpage
   */
  void handleRoot() {
    String html = R"HTML(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>WBR Robot Control</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      max-width: 600px;
      margin: 50px auto;
      padding: 20px;
      background: #f0f0f0;
    }
    .container {
      background: white;
      padding: 30px;
      border-radius: 10px;
      box-shadow: 0 2px 10px rgba(0,0,0,0.1);
    }
    h1 {
      color: #333;
      text-align: center;
    }
    .control-group {
      margin: 20px 0;
    }
    label {
      display: block;
      margin-bottom: 5px;
      font-weight: bold;
      color: #555;
    }
    input[type="range"] {
      width: 100%;
      height: 30px;
    }
    .value-display {
      text-align: center;
      font-size: 18px;
      color: #2196F3;
      margin: 5px 0;
    }
    button {
      width: 100%;
      padding: 15px;
      margin: 10px 0;
      font-size: 18px;
      border: none;
      border-radius: 5px;
      cursor: pointer;
      transition: background 0.3s;
    }
    .btn-run {
      background: #4CAF50;
      color: white;
    }
    .btn-run:hover {
      background: #45a049;
    }
    .btn-stop {
      background: #f44336;
      color: white;
    }
    .btn-stop:hover {
      background: #da190b;
    }
    .btn-reset {
      background: #ff9800;
      color: white;
    }
    .btn-reset:hover {
      background: #e68900;
    }
    .status {
      background: #e3f2fd;
      padding: 15px;
      border-radius: 5px;
      margin-top: 20px;
    }
    .status-item {
      display: flex;
      justify-content: space-between;
      margin: 5px 0;
    }
  </style>
</head>
<body>
  <div class="container">
    <h1>🤖 WBR Robot Control</h1>
    
    <div class="control-group">
      <label>Height (m)</label>
      <input type="range" id="height" min="0.07" max="0.20" step="0.01" value="0.15" oninput="updateValue('height')">
      <div class="value-display" id="height-val">0.15 m</div>
    </div>
    
    <div class="control-group">
      <label>Roll Angle (&deg;)</label>
      <input type="range" id="roll" min="-30" max="30" step="1" value="0" oninput="updateValue('roll')">
      <div class="value-display" id="roll-val">0&deg;</div>
    </div>
    
    <div class="control-group">
      <label>Velocity (m/s)</label>
      <input type="range" id="velocity" min="-0.5" max="0.5" step="0.05" value="0" oninput="updateValue('velocity')">
      <div class="value-display" id="velocity-val">0 m/s</div>
    </div>
    
    <div class="control-group">
      <label>Yaw Rate (&deg;/s)</label>
      <input type="range" id="yaw" min="-60" max="60" step="5" value="0" oninput="updateValue('yaw')">
      <div class="value-display" id="yaw-val">0&deg;/s</div>
    </div>
    
    <button class="btn-run" onclick="sendCommand('run')">&#9654; START</button>
    <button class="btn-stop" onclick="sendCommand('stop')">&#9724; STOP</button>
    <button class="btn-reset" onclick="sendCommand('reset')">&#8635; RESET</button>
    
    <div class="status">
      <h3>Status</h3>
      <div id="status-display">Connecting...</div>
    </div>
  </div>

  <script>
    function updateValue(id) {
      const slider = document.getElementById(id);
      const display = document.getElementById(id + '-val');
      let value = parseFloat(slider.value);
      
      if (id === 'height') {
        display.textContent = value.toFixed(2) + ' m';
      } else if (id === 'roll') {
        display.textContent = value + '&deg;';
      } else if (id === 'velocity') {
        display.textContent = value.toFixed(1) + ' m/s';
      } else if (id === 'yaw') {
        display.textContent = value.toFixed(1) + ' rad/s';
      }
      
      sendControlValues();
    }
    
    function sendControlValues() {
      const h = document.getElementById('height').value;
      const roll = document.getElementById('roll').value;
      const vel = document.getElementById('velocity').value;
      const yaw = document.getElementById('yaw').value;
      
      fetch('/control?h=' + h + '&roll=' + roll + '&v=' + vel + '&yaw=' + yaw)
        .catch(err => console.error('Error:', err));
    }
    
    function sendCommand(cmd) {
      fetch('/control?cmd=' + cmd)
        .then(response => response.text())
        .then(data => {
          console.log(data);
          updateStatus();
        })
        .catch(err => console.error('Error:', err));
    }
    
    function updateStatus() {
      fetch('/status')
        .then(response => response.json())
        .then(data => {
          const statusDiv = document.getElementById('status-display');
          statusDiv.innerHTML = `
            <div class="status-item"><span>State:</span><span>${data.state}</span></div>
            <div class="status-item"><span>Height:</span><span>${data.h} m</span></div>
            <div class="status-item"><span>Velocity:</span><span>${data.v} m/s</span></div>
            <div class="status-item"><span>Accel X:</span><span>${data.ax} m/s²</span></div>
          `;
        })
        .catch(err => console.error('Error:', err));
    }
    
    // Auto-update status every 500ms
    setInterval(updateStatus, 500);
    updateStatus();
  </script>
</body>
</html>
)HTML";
    server.send(200, "text/html", html);
  }

  /**
   * @brief Handle control commands
   */
  void handleControl() {
    if (server.hasArg("cmd")) {
      String cmd = server.arg("cmd");
      if (cmd == "run") {
        is_run = true;
        Serial.println("Command: RUN");
      } else if (cmd == "stop") {
        is_run = false;
        Serial.println("Command: STOP");
      } else if (cmd == "reset") {
        is_reset = true;
        Serial.println("Command: RESET");
      }
    }
    
    if (server.hasArg("h")) {
      h_d = server.arg("h").toFloat();
    }
    if (server.hasArg("roll")) {
      phi_d = server.arg("roll").toFloat();
    }
    if (server.hasArg("v")) {
      v_d = server.arg("v").toFloat();
    }
    if (server.hasArg("yaw")) {
      psi_d = server.arg("yaw").toFloat();
    }
    
    server.send(200, "text/plain", "OK");
  }

  /**
   * @brief Return JSON status
   */
  void handleStatus() {
    String json = "{";
    json += "\"state\":\"" + String(is_run ? "RUNNING" : "STOPPED") + "\",";
    json += "\"h\":" + String(current_h, 3) + ",";
    json += "\"v\":" + String(current_v, 3) + ",";
    json += "\"psi\":" + String(current_psi, 3) + ",";
    json += "\"ax\":" + String(current_ax, 3) + ",";
    json += "\"ay\":" + String(current_ay, 3) + ",";
    json += "\"az\":" + String(current_az, 3);
    json += "}";
    
    server.send(200, "application/json", json);
  }

  /**
   * @brief Handle data download request
   */
  void handleData() {
    // Placeholder for data download
    server.send(200, "text/plain", "Data download not yet implemented");
  }
};

#endif  // WEBCONTROLLER_H
