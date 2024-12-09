#include <WiFi.h>
#include <Adafruit_AHTX0.h>

Adafruit_AHTX0 aht;
float suhu, kelembaban;

const unsigned long eventInterval = 400;
unsigned long previousTime2 = 0;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

// Replace with your network credentials
const char* ssid = "Hai";
const char* password = "hai12345";

// Set web server port number to 80
WiFiServer server(80);  

// Variable to store the HTTP request
String header;

void setup() {
  Serial.begin(115200);
 
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  aht.begin();
  server.begin();
}

void loop() {
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) { // loop while the client's connected
          
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          if (currentLine.length() == 0) {
            if (header.indexOf("GET /data") >= 0) {
              // Handle the /data endpoint
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:application/json");
              client.println("Connection: close");
              client.println();
              
              String json = "{\"temperature\":" + String(suhu) + ",\"humidity\":" + String(kelembaban) + "}";
              client.println(json);
            } else {
              // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
              // and a content-type so the client knows what's coming, then a blank line:
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();
          
              // Display the HTML web page
              client.println("<!DOCTYPE html><html>");
              client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
              client.println("<link rel=\"icon\" href=\"data:,\">");
              // CSS to style the on/off buttons 
              // Feel free to change the background-color and font-size attributes to fit your preferences
              client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
              client.println(".button { background-color: #7D7A76; border: none; color: white; padding: 16px 40px;");
              client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
              client.println(".button2 {background-color: #195B6A;}</style></head>");
            
              // Web Page Heading
              client.println("<body><h1>ESP32 Web Server</h1>");
              
              client.println("<br>");   
              client.println("<h3>Data Sensor</h3>");
              client.println("<h3> Suhu: <span id=\"temperature\">" + (String)suhu + " C </span></h3>");
              client.println("<h3> Kelembaban: <span id=\"humidity\">" + (String)kelembaban + "% rH</span></h3>");
            
              client.println("<script>");
              client.println("setInterval(function() {");
              client.println("fetch('/data').then(response => response.json()).then(data => {");
              client.println("document.getElementById('temperature').innerText = data.temperature + ' C';");
              client.println("document.getElementById('humidity').innerText = data.humidity + '% rH';");
              client.println("});");
              client.println("}, 2000);");
              client.println("</script>");
              
              client.println("</body></html>");
            
              // The HTTP response ends with another blank line
              client.println();
            }
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }

  if (millis() - previousTime2 >= eventInterval) {
    previousTime2 = millis();
    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp); // populate temp and humidity objects with fresh data
    suhu = temp.temperature;
    kelembaban = humidity.relative_humidity;
  }
}