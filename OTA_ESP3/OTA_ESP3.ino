#include <WiFi.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>
#include <Update.h>
#include <Wire.h>
#include <RTClib.h>
#include <WebServer.h>

RTC_DS3231 rtc;
const char* host = "esp32";
const char* ssid = "OCEAN";
const char* password = "cerdas2023";
bool isLogin = false;

// Static IP address configuration
IPAddress local_IP(192, 168, 1, 184);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);   // Optional
IPAddress secondaryDNS(8, 8, 4, 4); 

WebServer server(80);


String pumpTime = ""; // Variabel global untuk menyimpan waktu pompa yang telah diatur
String pumpTimeOff = "";
const int relay1 = 16;
const int relay2 = 17;
const int relay3 = 18;
const int relay4 = 19;
const int btn1 = 32;
const int btn2 = 34;
const int led1 = 12;
const int led2 = 27; 
const int voltage_sensor = 4;
/*
 * Login page
 */

 const char* dashboard = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>RTC Sensor and Pump Setting</title>
  <style>
    * { font-family: "Poppins", sans-serif; margin: 0; padding: 0; box-sizing: border-box; }
    body { display: flex; justify-content: center; align-items: center; min-height: 100vh; background-color: #dde5f4; }
    .container { background-color: #f1f7fe; padding: 2em; border-radius: 20px; box-shadow: 0 0 10px rgba(0, 0, 0, 0.1); display: flex; width: 800px; justify-content: space-between; }
    .vertical-line { border-left: 1px solid #3e4684; height: auto; margin: 0 20px; }
    .section { width: 45%; text-align: center; }
    .sensor-value, .time-setting { background: white; box-shadow: 0 0 5px rgba(0, 0, 0, 0.1); padding: 1em; margin-bottom: 1em; border-radius: 20px; display: flex; flex-direction: column; align-items: center; font-size: 1em; color: #3e4684; }
    .refresh-btn, .login-btn, .set-time-btn { background-color: #3e4684; color: white; border: none; padding: 1em 2em; font-size: 1em; border-radius: 20px; cursor: pointer; transition: background-color 0.3s ease; margin: 0.5em 0; }
    .refresh-btn:hover, .login-btn:hover, .set-time-btn:hover { background-color: #2c325f; }
    .time-setting input { width: 80%; border: none; padding: 0.5em; margin: 0.5em 0; font-size: 1em; border-radius: 10px; box-shadow: 0 0 5px rgba(0, 0, 0, 0.1); outline: none; }
    .time-setting label { margin-bottom: 0.5em; color: #3e4684; font-size: 1em; }
  </style>
</head>
<body>
<div class="container">
  <div class="section">
    <H2>Time Now</H2>
    <br>
    <div class="sensor-value" id="sensorValue">00:00:00</div>
    <button class="refresh-btn" onclick="refreshSensorValue()">Refresh</button>
    <br><br>
    <H2>Voltage PLN</H2>
    <br>
    <div class="sensor-value" id="voltageValue">loading</div>
    <button class="refresh-btn" onclick="refreshSensorValue()">Refresh</button>
    <br><br>
    <H2>Update Firmware</H2>
    <br>
    <button class="login-btn" onclick="navigateToLogin()">Login</button>
  </div>
  <div class="vertical-line"></div>
  <div class="section">
    <H2>Setting PLTS Time</H2>
    <br>
    <div class="time-setting">
      <label for="pumpTime">Set PLTS Time On (HH:MM):</label>
      <input type="time" id="pumpTime" name="pumpTime">
      <button class="set-time-btn" onclick="setPumpTime()">Set Time</button>
    </div>
    <div class="time-setting">
      <label for="pumpTime">Set PLTS Time Off (HH:MM):</label>
      <input type="time" id="pumpTimeOff" name="pumpTime">
      <button class="set-time-btn" onclick="setPumpTimeOff()">Set Time</button>
    </div>
  </div>
</div>
<script>
  function refreshSensorValue() {
    fetch('/time')
      .then(response => response.json())
      .then(data => {
        document.getElementById('sensorValue').innerText = data.time;
      })
      .catch(error => console.error('Error:', error));

      fetch('/voltage')
      .then(response => response.json())
      .then(data => {
        document.getElementById('voltageValue').innerText = data.voltage;
      })
      .catch(error => console.error('Error:', error));
  }

  function navigateToLogin() {
    window.location.href = '/loginIndex';
  }

  function setPumpTime() {
  var pumpTime = document.getElementById('pumpTime').value;
  if (pumpTime) {
    fetch('/setPumpTime', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/x-www-form-urlencoded',
      },
      body: 'pumpTime=' + encodeURIComponent(pumpTime)
    })
    .then(response => response.text())
    .then(data => {
      alert(data);
    })
    .catch(error => console.error('Error:', error));
  } else {
    alert('Please select a valid time.');
  }
}
function setPumpTimeOff() {
  var pumpTimeOff = document.getElementById('pumpTimeOff').value;
  if (pumpTimeOff) {
    fetch('/setPumpTimeOff', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/x-www-form-urlencoded',
      },
      body: 'pumpTimeOff=' + encodeURIComponent(pumpTimeOff)
    })
    .then(response => response.text())
    .then(data => {
      alert(data);
    })
    .catch(error => console.error('Error:', error));
  } else {
    alert('Please select a valid time.');
  }
}


  setInterval(refreshSensorValue, 1000);
  window.onload = refreshSensorValue;
</script>
</body>
</html>
)=====";

const char* loginIndex =
  "<!DOCTYPE html>"
  "<html lang=\"en\">"
  "<head>"
  "  <meta charset=\"UTF-8\">"
  "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
  "  <title>Login Form</title>"
  "  <style>"
  "    * {"
  "      font-family: \"Poppins\", sans-serif;"
  "      margin: 0;"
  "      padding: 0;"
  "      box-sizing: border-box;"
  "    }"
  ""
  "    body {"
  "      display: flex;"
  "      justify-content: center;"
  "      align-items: center;"
  "      min-height: 100vh;"
  "      background-color: #dde5f4;"
  "    }"
  ""
  "    .login-container {"
  "      background-color: #f1f7fe;"
  "      padding: 2em;"
  "      border-radius: 20px;"
  "      box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);"
  "      width: 300px;"
  "      text-align: center;"
  "    }"
  ""
  "    .logo {"
  "      margin-top: -3em;"
  "    }"
  ""
  "    .email, .password {"
  "      background: white;"
  "      box-shadow: 0 0 5px rgba(0, 0, 0, 0.1);"
  "      padding: 1em;"
  "      margin-bottom: 1em;"
  "      border-radius: 20px;"
  "      display: flex;"
  "      align-items: center;"
  "    }"
  ""
  "    .email input, .password input {"
  "      flex: 1;"
  "      border: none;"
  "      padding: 0.5em;"
  "      margin-left: 0.5em;"
  "      font-size: 1em;"
  "      outline: none;"
  "    }"
  ""
  "    .login-btn {"
  "      background-color: #3e4684;"
  "      color: white;"
  "      border: none;"
  "      padding: 1em 2em;"
  "      font-size: 1em;"
  "      border-radius: 20px;"
  "      cursor: pointer;"
  "      transition: background-color 0.3s ease;"
  "    }"
  ""
  "    .login-btn:hover {"
  "      background-color: #2c325f;"
  "    }"
  ""
  "    .footer {"
  "      font-size: 0.8em;"
  "      color: #5e5e5e;"
  "      margin-top: 1em;"
  "    }"
  ""
  "    .footer span {"
  "      cursor: pointer;"
  "      margin: 0 0.5em;"
  "    }"
  "  </style>"
  "</head>"
  "<body>"
  ""
  "<div class=\"login-container\">"
  "  <H1>Login</H1>"
  "  <br>"
  "  <form id=\"loginForm\" onsubmit=\"return checkForm()\">"
  "    <div class=\"email\">"
  "      <ion-icon name=\"mail-outline\"></ion-icon>"
  "      <input type=\"username\" id=\"email\" name=\"email\" placeholder=\"Username\" required>"
  "    </div>"
  "    <div class=\"password\">"
  "      <ion-icon name=\"lock-closed-outline\"></ion-icon>"
  "      <input type=\"password\" id=\"password\" name=\"password\" placeholder=\"Password\" required>"
  "    </div>"
  "    <button type=\"submit\" class=\"login-btn\">Login</button>"
  "  </form>"
  "</div>"
  ""
  "<script>"
  "  function checkForm() {"
  "    var email = document.getElementById('email').value.trim();"
  "    var password = document.getElementById('password').value.trim();"
  ""
  "    if (email === 'admin' && password === 'admin') {"
  "      window.location.href = '/serverIndex';"
      
  "      return false;"
  "    } else {"
  "      alert('Error: Incorrect Username or Password');"
  "      return false;"
  "    }"
  "  }"
  "</script>"
  ""
  "</body>"
  "</html>";


/*
 * Server Index Page
 */

const char* serverIndex =
  "<!DOCTYPE html>"
  "<html lang=\"en\">"
  "<head>"
  "  <meta charset=\"UTF-8\">"
  "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
  "  <title>Server Index</title>"
  "  <style>"
  "    * {"
  "      font-family: \"Poppins\", sans-serif;"
  "      margin: 0;"
  "      padding: 0;"
  "      box-sizing: border-box;"
  
  "    }"
  ""
  "    body {"
  "      display: flex;"
  "      justify-content: center;"
  "      align-items: center;"
  "      min-height: 100vh;"
  "      background-color: #dde5f4;"
  "    }"
  ""
  "    .server-container {"
  "      background-color: #f1f7fe;"
  "      padding: 2em;"
  "      border-radius: 20px;"
  "      box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);"
  "      width: 300px;"
  "      text-align: center;"
  "    }"
  ""
  "    .upload-form {"
  "      margin-bottom: 1em;"
  "    }"
  ""
  "    .upload-btn {"
  "      background-color: #3e4684;"
  "      color: white;"
  "      border: none;"
  "      padding: 1em 2em;"
  "      font-size: 1em;"
  "      border-radius: 20px;"
  "      cursor: pointer;"
  "      transition: background-color 0.3s ease;"
  "    }"
  ""
  "    .upload-btn:hover {"
  "      background-color: #2c325f;"
  "    }"
  ""
  "    .file-input {"
  "      margin-bottom: 0.5em;" // Penambahan jarak di sini
  "    }"
  ""
  "    .progress {"
  "      font-size: 0.8em;"
  "      color: #5e5e5e;"
  "      margin-top: 1em;"
  "    }"
  ""
  "    #prg {"
  "      margin-top: 0.5em;"
  "    }"
  "  </style>"
  "</head>"
  "<body>"
  ""
  "<div class=\"server-container\">"
  "  <H2>Update Firmware</H2>"
  "  <br>"
  "  <form method=\"POST\" action=\"#\" enctype=\"multipart/form-data\" class=\"upload-form\" id=\"upload_form\">"
  "    <input type=\"file\" name=\"update\" class=\"file-input\">"
  "    <button type=\"submit\" class=\"upload-btn\">Update</button>"
  "  </form>"
  "  <div class=\"progress\">"
  "    <div id=\"prg\">progress: 0%</div>"
  "  </div>"
      "<div class=\"progress\">"
       " <div id=\"prg\"><u><a href=\"/\">back to dashboard</a></u> </div>"
     "</div>"
  "</div>"
  ""
  "<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js\"></script>"
  "<script>"
  "  $('form').submit(function(e) {"
  "    e.preventDefault();"
  "    var form = $('#upload_form')[0];"
  "    var data = new FormData(form);"
  "    $.ajax({"
  "      url: '/update',"
  "      type: 'POST',"
  "      data: data,"
  "      contentType: false,"
  "      processData: false,"
  "      xhr: function() {"
  "        var xhr = new window.XMLHttpRequest();"
  "        xhr.upload.addEventListener('progress', function(evt) {"
  "          if (evt.lengthComputable) {"
  "            var per = evt.loaded / evt.total;"
  "            $('#prg').html('progress: ' + Math.round(per * 100) + '%');"
  "          }"
  "        }, false);"
  "        return xhr;"
  "      },"
  "      success: function(d, s) {"
  "        console.log('success!');"
  "      },"
  "      error: function(a, b, c) {"
  "        console.error('error!');"
  "      }"
  "    });"
  "  });"
  "</script>"
  ""
  "</body>"
  "</html>";



/*
 * setup function
 */
void setup(void) {
  Serial.begin(115200);

  

  // Configuring the static IP address
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    // while (1);
  }
  //  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(voltage_sensor,INPUT);
  pinMode(2,OUTPUT);

  
  /*use mdns for host name resolution*/
  if (!MDNS.begin(host)) { //http://esp32.local
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");
  /*return index page which is stored in serverIndex */
  server.on("/", HTTP_GET, []() {
    isLogin = false;
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", dashboard);
  });
  server.on("/setPumpTime", HTTP_POST, []() {
  if (server.hasArg("pumpTime")) {
    pumpTime = server.arg("pumpTime");
    server.send(200, "text/plain", "Pump time set to: " + pumpTime);
  } else {
    server.send(400, "text/plain", "Invalid request");
  }
});
  server.on("/setPumpTimeOff", HTTP_POST, []() {
  if (server.hasArg("pumpTimeOff")) {
    pumpTimeOff = server.arg("pumpTimeOff");
    server.send(200, "text/plain", "Pump time off set to: " + pumpTimeOff);
  } else {
    server.send(400, "text/plain", "Invalid request");
  }
});
  server.on("/time", HTTP_GET, []() {
    DateTime now = rtc.now();
    String currentTime = String(now.year()) + "-" + String(now.month()) + "-" + String(now.day()) + " " + String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
    String json = "{\"time\": \"" + currentTime + "\"}";
    server.send(200, "application/json", json);
  });
  server.on("/voltage", HTTP_GET, []() {
    int value_sensor = digitalRead(voltage_sensor);
    String value;
    if(value_sensor == LOW){
      value = "ON";
    } else {
      value = "OFF";
    }
     String json = "{\"voltage\":\"" + value + "\"}";
     server.send(200, "application/json", json);
  });
  server.on("/serverIndex", HTTP_GET, []() {

    if(isLogin == true){
      server.sendHeader("Connection", "close");
      server.send(200, "text/html", serverIndex);
    } else{
      server.sendHeader("Connection", "close");
      server.send(200, "text/html", dashboard);
    }
    
  });
  server.on("/loginIndex", HTTP_GET, []() {
    isLogin = true;
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", loginIndex);
  });
  /*handling uploading firmware file */
  server.on("/update", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      /* flashing firmware to ESP*/
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { //true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
    }
  });
  server.begin();
}

void loop(void) {
  server.handleClient();
  delay(1);
  DateTime now = rtc.now();
  // String currentTime = String(now.hour()) + ":" + String(now.minute());
  int currentHour = now.hour();
  int currentMinute = now.minute();
  int currentTime = currentHour * 100 + currentMinute; 
  int pumpOnTimeInt = parseTimeString(pumpTime); // Ubah string waktu menjadi integer
  int pumpOffTimeInt = parseTimeString(pumpTimeOff);
  
    //ponpes
    if (currentTime >= pumpOnTimeInt && currentTime < pumpOffTimeInt) {
      
      digitalWrite(relay1, HIGH); // PLTS Fasa
      digitalWrite(relay2, HIGH); // PLTS Netral
      digitalWrite(relay3, HIGH); // PLN Fasa
      digitalWrite(relay4, HIGH); // PLN Netral 

  } else{
      
      digitalWrite(relay1, LOW); // PLTS Fasa
      digitalWrite(relay2, LOW); // PLTS Netral
      digitalWrite(relay3, LOW); // PLN Fasa
      digitalWrite(relay4, LOW); // PLN Netral  

  }
  //klink 
  // if(currentTime >= pumpOnTimeInt && currentTime < pumpOffTimeInt){
  //   if(value_sensor == LOW){
  //     digitalWrite(2, HIGH); 
  //     digitalWrite(relay1, LOW);
  //   } else {
  //     digitalWrite(2, LOW); 
  //     digitalWrite(relay1, HIGH);
  //   }
  // } 
  


  if (pumpTime != "") {
    Serial.println("Current pump time ON setting: " + pumpTime);
  }
  if (pumpTimeOff != "") {
    Serial.println("Current pump time OFF setting: " + pumpTimeOff);
  }
  // Serial.println("Current pump time setting: " + pumpTime);
}

int parseTimeString(const String& timeString) {
  int separatorIndex = timeString.indexOf(':');
  if (separatorIndex == -1) return -1; // Jika format waktu tidak valid, kembalikan -1
  int hour = timeString.substring(0, separatorIndex).toInt();
  int minute = timeString.substring(separatorIndex + 1).toInt();
  return hour * 100 + minute;
}
