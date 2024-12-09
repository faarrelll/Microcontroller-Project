#include <WiFi.h>

// Ganti dengan kredensial jaringan Anda
const char* ssid = "Hai";
const char* password = "hai12345";

// Tetapkan nomor port server web ke 80
WiFiServer server(80);

// Variabel untuk menyimpan permintaan HTTP
String header;

// Variabel pembantu untuk menyimpan status output saat ini
String output26State = "off";
String output27State = "off";

// Tetapkan variabel output ke pin GPIO
const int output26 = 26;
const int output27 = 27;

// Waktu saat ini
unsigned long currentTime = millis();
// Waktu sebelumnya
unsigned long previousTime = 0; 
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  pinMode(output26, OUTPUT);
  pinMode(output27, OUTPUT);
  digitalWrite(output26, LOW);
  digitalWrite(output27, LOW);

  // Hubungkan ke jaringan Wi-Fi dengan SSID dan password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Cetak alamat IP lokal dan mulai server web
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   
  if (client) {                             
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          
    String currentLine = "";                
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  
      currentTime = millis();
      if (client.available()) {             // jika ada byte yang dapat dibaca dari klien,
        ent.read();char c = cli             // baca satu byte, lalu
        Serial.write(c);                    // cetak ke serial monitor
        header += c;
        if (c == '\n') {                    // jika byte adalah karakter newline
          // jika baris saat ini kosong, Anda mendapatkan dua karakter newline berturut-turut.
          // itu adalah akhir dari permintaan HTTP klien, jadi kirimkan tanggapan:
          if (currentLine.length() == 0) {
            // Header HTTP selalu dimulai dengan kode respons (misalnya HTTP/1.1 200 OK)
            // dan tipe konten sehingga klien tahu apa yang akan datang, lalu baris kosong:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // menghidupkan dan mematikan GPIO
            if (header.indexOf("GET /26/on") >= 0) {
              Serial.println("GPIO 26 on");
              output26State = "on";
              digitalWrite(output26, HIGH);
            } else if (header.indexOf("GET /26/off") >= 0) {
              Serial.println("GPIO 26 off");
              output26State = "off";
              digitalWrite(output26, LOW);
            } else if (header.indexOf("GET /27/on") >= 0) {
              Serial.println("GPIO 27 on");
              output27State = "on";
              digitalWrite(output27, HIGH);
            } else if (header.indexOf("GET /27/off") >= 0) {
              Serial.println("GPIO 27 off");
              output27State = "off";
              digitalWrite(output27, LOW);
            }
            
            // Tampilkan halaman web HTML
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS untuk menata tombol on/off 
            // Silakan ubah atribut background-color dan font-size sesuai preferensi Anda
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // Judul Halaman Web
            client.println("<body><h1>ESP32 Web Server</h1>");
            
            // Tampilkan status saat ini, dan tombol ON/OFF untuk GPIO 26  
            client.println("<p>GPIO 26 - State " + output26State + "</p>");
            // Jika output26State adalah off, tampilkan tombol ON       
            if (output26State=="off") {
              client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            // Tampilkan status saat ini, dan tombol ON/OFF untuk GPIO 27  
            client.println("<p>GPIO 27 - State " + output27State + "</p>");
            // Jika output27State adalah off, tampilkan tombol ON       
            if (output27State=="off") {
              client.println("<p><a href=\"/27/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/27/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");
            
            // Tanggapan HTTP diakhiri dengan baris kosong lainnya
            client.println();
            // Keluar dari loop while
            break;
          } else { // jika mendapatkan newline, bersihkan currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // jika mendapatkan karakter selain carriage return,
          currentLine += c;      // tambahkan ke akhir currentLine
        }
      }
    }
    // Bersihkan variabel header
    header = "";
    // Tutup koneksi
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
