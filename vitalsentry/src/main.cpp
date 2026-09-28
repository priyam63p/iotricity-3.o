#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <ArduinoJson.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


// ============================================================
// WIFI
// ============================================================

const char* WIFI_SSID = "Wokwi-GUEST";
const char* WIFI_PASSWORD = "";


// ============================================================
// BACKEND / NGROK
// ============================================================

// IMPORTANT:
// Replace this with your CURRENT ngrok URL.
// Do NOT add /api/vitals here.

const char* BACKEND_URL =
  "https://police-carwash-outthink.ngrok-free.dev";

const char* LATEST_ENDPOINT =
  "/api/vitals/latest";


// ============================================================
// OLED
// ============================================================

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  -1
);


// ============================================================
// LED + BUZZER PINS
// ============================================================

#define LED_GREEN   16
#define LED_YELLOW  17
#define LED_RED     18
#define BUZZER_PIN  19


// ============================================================
// BACKEND POLLING
// ============================================================

const unsigned long BACKEND_INTERVAL = 2000;

unsigned long lastBackendCheck = 0;


// ============================================================
// CURRENT BACKEND DATA
// ============================================================

int currentHR = 0;
int currentSpO2 = 0;
float currentMotion = 0.0;

int currentRiskScore = 0;

bool currentAlert = false;

String currentStatus = "NO_DATA";


// ============================================================
// WIFI CONNECTION
// ============================================================

void connectWiFi() {

  Serial.println();
  Serial.println("================================");
  Serial.println("Connecting to WiFi...");
  Serial.println("================================");

  WiFi.mode(WIFI_STA);

  WiFi.begin(
    WIFI_SSID,
    WIFI_PASSWORD
  );

  int attempts = 0;

  while (
    WiFi.status() != WL_CONNECTED &&
    attempts < 30
  ) {

    delay(500);

    Serial.print(".");

    attempts++;
  }

  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {

    Serial.println("WiFi connected!");

    Serial.print("ESP32 IP: ");
    Serial.println(WiFi.localIP());

  } else {

    Serial.println("WiFi connection FAILED");
  }
}


// ============================================================
// OLED - SHOW CONNECTION STATUS
// ============================================================

void showConnectionScreen() {

  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(1);

  display.setCursor(0, 0);
  display.println("VitalSentry AI");

  display.setCursor(0, 18);
  display.println("Connecting WiFi...");

  display.display();
}


// ============================================================
// OLED - SHOW NO DATA
// ============================================================

void showNoDataScreen() {

  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(1);

  display.setCursor(0, 0);
  display.println("VitalSentry AI");

  display.setCursor(0, 18);
  display.println("Backend:");

  display.setCursor(0, 32);
  display.println("NO DATA");

  display.setCursor(0, 48);
  display.println("Waiting...");

  display.display();
}


// ============================================================
// OLED - SHOW VITALS
// ============================================================

void updateOLED() {

  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);


  // ----------------------------------------------------------
  // STATUS
  // ----------------------------------------------------------

  display.setTextSize(1);

  display.setCursor(0, 0);

  display.print("STATUS: ");

  if (currentStatus == "HIGH_PRIORITY") {

    display.println("HIGH");

  }
  else if (currentStatus == "ATTENTION") {

    display.println("ATTENTION");

  }
  else if (currentStatus == "NORMAL") {

    display.println("NORMAL");

  }
  else {

    display.println("NO DATA");
  }


  // ----------------------------------------------------------
  // HEART RATE
  // ----------------------------------------------------------

  display.setTextSize(2);

  display.setCursor(0, 17);

  display.print("HR ");

  display.print(currentHR);


  // ----------------------------------------------------------
  // SPO2
  // ----------------------------------------------------------

  display.setCursor(0, 40);

  display.print("O2 ");

  display.print(currentSpO2);

  display.print("%");


  // ----------------------------------------------------------
  // RISK SCORE
  // ----------------------------------------------------------

  display.setTextSize(1);

  display.setCursor(88, 20);

  display.print("Risk");

  display.setCursor(88, 32);

  display.print(currentRiskScore);


  display.display();
}


// ============================================================
// NORMAL ALARM
// ============================================================

void setNormalAlarm() {

  digitalWrite(
    LED_GREEN,
    HIGH
  );

  digitalWrite(
    LED_YELLOW,
    LOW
  );

  digitalWrite(
    LED_RED,
    LOW
  );

  noTone(
    BUZZER_PIN
  );

  Serial.println("GREEN LED = ON");
  Serial.println("YELLOW LED = OFF");
  Serial.println("RED LED = OFF");
  Serial.println("BUZZER = OFF");
}


// ============================================================
// ATTENTION ALARM
// ============================================================

void setAttentionAlarm() {

  digitalWrite(
    LED_GREEN,
    LOW
  );

  digitalWrite(
    LED_YELLOW,
    HIGH
  );

  digitalWrite(
    LED_RED,
    LOW
  );

  // Short warning beep
  tone(
    BUZZER_PIN,
    1000,
    300
  );

  Serial.println("YELLOW LED = ON");
  Serial.println("RED LED = OFF");
  Serial.println("BUZZER = SHORT WARNING");
}


// ============================================================
// HIGH PRIORITY ALARM
// ============================================================

void setHighPriorityAlarm() {

  digitalWrite(
    LED_GREEN,
    LOW
  );

  digitalWrite(
    LED_YELLOW,
    LOW
  );

  digitalWrite(
    LED_RED,
    HIGH
  );

  // Continuous high-priority alarm
  tone(
    BUZZER_PIN,
    2000
  );

  Serial.println();
  Serial.println("********************************");
  Serial.println("      HIGH PRIORITY ALERT");
  Serial.println("********************************");
  Serial.println("RED LED = ON");
  Serial.println("BUZZER = ON");
}


// ============================================================
// APPLY BACKEND ALARM
// ============================================================

void applyBackendAlarm(
  String status,
  bool alert,
  int riskScore
) {

  Serial.println();
  Serial.println("========== ALARM CHECK ==========");

  Serial.print("Status : ");
  Serial.println(status);

  Serial.print("Alert  : ");
  Serial.println(
    alert ? "TRUE" : "FALSE"
  );

  Serial.print("Risk   : ");
  Serial.println(riskScore);


  // ----------------------------------------------------------
  // HIGH PRIORITY
  // ----------------------------------------------------------

  if (status == "HIGH_PRIORITY") {

    setHighPriorityAlarm();

  }


  // ----------------------------------------------------------
  // ATTENTION
  // ----------------------------------------------------------

  else if (status == "ATTENTION") {

    setAttentionAlarm();

  }


  // ----------------------------------------------------------
  // NORMAL
  // ----------------------------------------------------------

  else if (status == "NORMAL") {

    setNormalAlarm();

  }


  // ----------------------------------------------------------
  // UNKNOWN STATUS
  // ----------------------------------------------------------

  else {

    digitalWrite(
      LED_GREEN,
      LOW
    );

    digitalWrite(
      LED_YELLOW,
      LOW
    );

    digitalWrite(
      LED_RED,
      LOW
    );

    noTone(
      BUZZER_PIN
    );

    Serial.println("UNKNOWN STATUS");
  }

  Serial.println("=================================");
}


// ============================================================
// GET DATA FROM BACKEND
// ============================================================

void fetchBackendVitals() {

  // ----------------------------------------------------------
  // CHECK WIFI
  // ----------------------------------------------------------

  if (
    WiFi.status() != WL_CONNECTED
  ) {

    Serial.println(
      "WiFi disconnected"
    );

    connectWiFi();

    return;
  }


  // ----------------------------------------------------------
  // HTTPS CLIENT
  // ----------------------------------------------------------

  WiFiClientSecure client;

  // Prototype/demo only.
  // This allows ESP32 to connect to ngrok HTTPS.
  client.setInsecure();


  HTTPClient http;


  // ----------------------------------------------------------
  // CREATE URL
  // ----------------------------------------------------------

  String url =
    String(BACKEND_URL) +
    LATEST_ENDPOINT;


  Serial.println();
  Serial.println("================================");
  Serial.println("GET LATEST VITALS");
  Serial.println("================================");

  Serial.println(url);


  // ----------------------------------------------------------
  // START HTTP
  // ----------------------------------------------------------

  if (
    !http.begin(
      client,
      url
    )
  ) {

    Serial.println(
      "HTTP begin failed"
    );

    return;
  }


  // ----------------------------------------------------------
  // SEND GET
  // ----------------------------------------------------------

  int httpCode =
    http.GET();


  Serial.print(
    "HTTP Code: "
  );

  Serial.println(
    httpCode
  );


  // ==========================================================
  // SUCCESS
  // ==========================================================

  if (
    httpCode == 200
  ) {

    String response =
      http.getString();


    Serial.println();
    Serial.println(
      "Backend response:"
    );

    Serial.println(
      response
    );


    // --------------------------------------------------------
    // JSON PARSING
    // --------------------------------------------------------

    DynamicJsonDocument doc(
      4096
    );


    DeserializationError error =
      deserializeJson(
        doc,
        response
      );


    if (error) {

      Serial.print(
        "JSON parse error: "
      );

      Serial.println(
        error.c_str()
      );

      http.end();

      return;
    }


    // --------------------------------------------------------
    // GET DATA OBJECT
    // --------------------------------------------------------

    JsonObject data =
      doc["data"];


    if (
      data.isNull()
    ) {

      Serial.println(
        "ERROR: data object missing"
      );

      http.end();

      return;
    }


    // --------------------------------------------------------
    // READ BACKEND VALUES
    // --------------------------------------------------------

    currentHR =
      data["heart_rate"] |
      0;


    currentSpO2 =
      data["spo2"] |
      0;


    currentMotion =
      data["motion"] |
      0.0;


    currentRiskScore =
      data["riskScore"] |
      0;


    currentAlert =
      data["alert"] |
      false;


    currentStatus =
      data["status"] |
      "NO_DATA";


    // --------------------------------------------------------
    // NORMALIZE STATUS
    // --------------------------------------------------------

    currentStatus.replace(
      " ",
      "_"
    );

    currentStatus.replace(
      "-",
      "_"
    );

    currentStatus.toUpperCase();


    // --------------------------------------------------------
    // PRINT DATA
    // --------------------------------------------------------

    Serial.println();
    Serial.println(
      "========= BACKEND DATA ========="
    );

    Serial.print(
      "Heart Rate : "
    );

    Serial.println(
      currentHR
    );


    Serial.print(
      "SpO2       : "
    );

    Serial.println(
      currentSpO2
    );


    Serial.print(
      "Motion     : "
    );

    Serial.println(
      currentMotion
    );


    Serial.print(
      "Risk Score : "
    );

    Serial.println(
      currentRiskScore
    );


    Serial.print(
      "Alert      : "
    );

    Serial.println(
      currentAlert ?
      "TRUE" :
      "FALSE"
    );


    Serial.print(
      "Status     : "
    );

    Serial.println(
      currentStatus
    );


    Serial.println(
      "================================"
    );


    // --------------------------------------------------------
    // UPDATE OLED
    // --------------------------------------------------------

    updateOLED();


    // --------------------------------------------------------
    // APPLY BACKEND ALARM
    // --------------------------------------------------------

    applyBackendAlarm(
      currentStatus,
      currentAlert,
      currentRiskScore
    );
  }


  // ==========================================================
  // HTTP ERROR
  // ==========================================================

  else {

    Serial.print(
      "GET failed. HTTP code: "
    );

    Serial.println(
      httpCode
    );

    // Don't leave alarm active
    // when backend cannot be reached.

    digitalWrite(
      LED_GREEN,
      LOW
    );

    digitalWrite(
      LED_YELLOW,
      LOW
    );

    digitalWrite(
      LED_RED,
      LOW
    );

    noTone(
      BUZZER_PIN
    );


    showNoDataScreen();
  }


  // ----------------------------------------------------------
  // CLOSE HTTP
  // ----------------------------------------------------------

  http.end();
}


// ============================================================
// SETUP
// ============================================================

void setup() {

  // ----------------------------------------------------------
  // SERIAL
  // ----------------------------------------------------------

  Serial.begin(
    115200
  );

  delay(1000);


  Serial.println();
  Serial.println(
    "================================"
  );

  Serial.println(
    "      VitalSentry AI"
  );

  Serial.println(
    "Backend-Driven ESP32"
  );

  Serial.println(
    "================================"
  );


  // ----------------------------------------------------------
  // LED + BUZZER PINS
  // ----------------------------------------------------------

  pinMode(
    LED_GREEN,
    OUTPUT
  );

  pinMode(
    LED_YELLOW,
    OUTPUT
  );

  pinMode(
    LED_RED,
    OUTPUT
  );

  pinMode(
    BUZZER_PIN,
    OUTPUT
  );


  // Start everything OFF

  digitalWrite(
    LED_GREEN,
    LOW
  );

  digitalWrite(
    LED_YELLOW,
    LOW
  );

  digitalWrite(
    LED_RED,
    LOW
  );

  noTone(
    BUZZER_PIN
  );


  // ----------------------------------------------------------
  // OLED
  // ----------------------------------------------------------

  Wire.begin();

  if (
    !display.begin(
      SSD1306_SWITCHCAPVCC,
      0x3C
    )
  ) {

    Serial.println(
      "OLED initialization failed"
    );

  } else {

    Serial.println(
      "OLED initialized"
    );

    display.clearDisplay();

    display.setTextColor(
      SSD1306_WHITE
    );

    display.setTextSize(1);

    display.setCursor(
      0,
      0
    );

    display.println(
      "VitalSentry AI"
    );

    display.setCursor(
      0,
      20
    );

    display.println(
      "Starting..."
    );

    display.display();
  }


  // ----------------------------------------------------------
  // WIFI
  // ----------------------------------------------------------

  showConnectionScreen();

  connectWiFi();


  // ----------------------------------------------------------
  // FIRST BACKEND REQUEST
  // ----------------------------------------------------------

  if (
    WiFi.status() == WL_CONNECTED
  ) {

    delay(1000);

    fetchBackendVitals();
  }
}


// ============================================================
// LOOP
// ============================================================

void loop() {

  // ----------------------------------------------------------
  // CHECK BACKEND EVERY 2 SECONDS
  // ----------------------------------------------------------

  if (
    millis() -
    lastBackendCheck >=
    BACKEND_INTERVAL
  ) {

    lastBackendCheck =
      millis();

    fetchBackendVitals();
  }


  // ----------------------------------------------------------
  // KEEP WIFI ALIVE
  // ----------------------------------------------------------

  if (
    WiFi.status() != WL_CONNECTED
  ) {

    Serial.println(
      "WiFi lost. Reconnecting..."
    );

    connectWiFi();
  }


  delay(10);
}