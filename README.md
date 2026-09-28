<div align="center">
🩺 ArogyaDrishti AI
See the Signal. Understand the Risk. Act in Time.

An IoT + AI-assisted remote health monitoring and early-warning system with explainable risk scoring, real-time alerts, and a live dashboard.

Show Image Show Image Show Image Show Image Show Image Show Image

</div>
📖 Overview

ArogyaDrishti AI continuously monitors vital-health signals, identifies abnormal conditions, generates explainable risk levels, and delivers timely alerts to caregivers or monitoring personnel.

It brings together IoT hardware, intelligent risk analysis, cloud data storage, real-time notifications, and a live monitoring dashboard in one connected healthcare ecosystem.

🌟 Why ArogyaDrishti AI?

Healthcare monitoring should not stop at displaying numbers. ArogyaDrishti AI turns raw physiological signals into an actionable pipeline:

text
SENSE → COLLECT → ANALYZE → UNDERSTAND → ALERT → RESPOND

Instead of simply showing:

text
❤️ Heart Rate: 145 BPM
🫁 SpO₂: 87%

ArogyaDrishti AI converts the readings into an explainable warning:

text
🚨 HIGH PRIORITY

Risk Score: 90/100

Reasons:
- Heart rate is elevated
- SpO₂ is below the configured threshold

📱 Notification Sent
🖥️ Dashboard Updated
🔴 ESP32 Alert Activated
🎯 Problem Statement

Remote health monitoring is hard when caregivers must continuously watch raw sensor readings.

Continuous monitoring is difficult.
Raw physiological values are hard to interpret quickly.
Abnormal readings may require timely attention.
Multiple signals need to be considered together.
Historical readings need to be stored for review.
Alerts need to reach the responsible person quickly.

Our approach: one unified pipeline

text
Monitoring → Analysis → Explanation → Notification → Review
💡 Our Solution

ArogyaDrishti AI processes health-related signals and converts them into understandable risk information.

Signals currently supported

❤️ Heart Rate
🫁 SpO₂
🏃 Motion / Activity

Planned for future versions

ECG
Temperature
Respiration
🧠 Core Intelligence: Explainable Risk Engine

Rather than simply producing an alert, the engine provides:

Risk Score
Risk Status
Alert State
Explanation / Reasons
Risk Levels
Status	Meaning
🟢 NORMAL	No configured abnormality detected
🟡 ATTENTION	Readings require attention
🔴 HIGH_PRIORITY	Significant configured risk detected
Example
text
Heart Rate = 145 BPM
SpO₂       = 87%
Motion     = 0.2

        ↓

Risk Score = 90/100
Status     = HIGH_PRIORITY

Reasons:
- Heart rate is elevated
- SpO₂ is below the configured threshold

Note: ArogyaDrishti AI is an early-warning and decision-support prototype. It is not a medical diagnostic system.

📱 Smart Notification System

When the backend detects an abnormal state, it sends a real-time notification via ntfy.

text
🚨 VitalSentry Critical Alert

Heart Rate: 145 BPM
SpO₂: 87%
Risk Score: 90/100
Status: HIGH_PRIORITY

Reasons:
- Heart rate is elevated
- SpO₂ is below the configured threshold

Monitoring personnel receive important alerts without continuously watching the dashboard.

🖥️ Real-Time Monitoring Dashboard

The React dashboard provides a centralized monitoring interface.

Capability	
Current heart rate	❤️
Current SpO₂	🫁
Motion / activity	🏃
Risk score	📊
Current status	🚦
Alert state	🚨
Historical readings	📜

The backend acts as the single source of truth, so the dashboard, notification system, and ESP32 all receive consistent processed data.

📡 IoT Hardware Interface

The edge interface is built on an ESP32.

Hardware

ESP32
OLED SSD1306 display
Motion sensor (MPU6050)
Green, Yellow, and Red LEDs
Wi-Fi

LED status

LED	Status
🟢 Green	NORMAL
🟡 Yellow	ATTENTION
🔴 Red	HIGH PRIORITY

The ESP32 retrieves the latest processed status from the backend rather than independently calculating the final risk state.

🏗️ System Architecture
text
                    ┌─────────────────────┐
                    │   Health Data Input │
                    │ Sensors / API Input │
                    └──────────┬──────────┘
                               │
                               ▼
                    ┌─────────────────────┐
                    │   Node.js + Express │
                    │       REST API      │
                    └──────────┬──────────┘
                               │
                               ▼
                    ┌─────────────────────┐
                    │   Explainable Risk  │
                    │       Engine        │
                    └──────────┬──────────┘
                               │
                     ┌─────────┴─────────┐
                     │                   │
                     ▼                   ▼
             ┌───────────────┐   ┌────────────────┐
             │    MongoDB    │   │  Notification  │
             │ Vital History │   │    Service     │
             └───────┬───────┘   └───────┬────────┘
                     │                   │
                     ▼                   ▼
             ┌───────────────┐     ┌────────────┐
             │     React     │     │    ntfy    │
             │   Dashboard   │     │   Alerts   │
             └───────┬───────┘     └─────┬──────┘
                     │                   │
                     │                   ▼
                     │                📱 Phone
                     ▼
             ┌────────────────┐
             │     ESP32      │
             │  OLED + LEDs   │
             └────────────────┘
🔑 Backend as the Source of Truth

Keeping the backend as the central authority prevents different devices from independently calculating different health states.

text
                    Backend
                       │
              ┌────────┼────────┐
              ▼        ▼        ▼
           MongoDB   React    ESP32
                       │
                       ▼
                     ntfy
                       │
                       ▼
                    📱 User
🔄 End-to-End Data Flow

Step 1: Data Input. Readings enter the backend through the REST API.

http
POST /api/vitals
json
{
  "patientId": "demo-patient",
  "heart_rate": 145,
  "spo2": 87,
  "motion": 0.2
}

Step 2: Risk Analysis. The Node.js risk engine evaluates each signal.

text
Heart Rate → Elevated
SpO₂       → Low
Motion     → Normal

Step 3: Risk Calculation. The system generates a score, status, alert flag, and explanation.

text
Risk Score → 90
Status     → HIGH_PRIORITY
Alert      → TRUE

Step 4: Data Storage. The processed event is stored in MongoDB for later review.

Step 5: Notification. If the status requires attention, the notification service sends an ntfy alert.

text
Backend → Notification Service → ntfy → 📱 Phone

Step 6: Dashboard Update. The React dashboard retrieves the latest state from the backend.

Step 7: IoT Response. The ESP32 retrieves the latest state and reflects it through the OLED display and the green, yellow, or red LED.

🧩 Technology Stack
Layer	Technologies
Frontend	React, Vite, Recharts, HTML5, CSS3
Backend	Node.js, Express.js, REST API
Database	MongoDB, Mongoose
IoT	ESP32, OLED SSD1306, MPU6050, LEDs, Wi-Fi
Notifications	ntfy
Dev & Testing	Postman, ngrok, PlatformIO, Wokwi
📁 Project Structure
text
ArogyaDrishti-AI/
│
├── backend/
│   ├── server.js
│   ├── .env
│   │
│   ├── models/
│   │   └── Vital.js
│   │
│   ├── routes/
│   │   └── vitals.js
│   │
│   └── services/
│       ├── riskEngine.js
│       └── notificationService.js
│
├── frontend/
│   ├── src/
│   ├── package.json
│   └── ...
│
├── esp32/
│   ├── src/
│   │   └── main.cpp
│   └── platformio.ini
│
└── README.md
🚀 Getting Started
1. Clone the repository
bash
git clone <your-repository-url>
cd ArogyaDrishti-AI
2. Backend setup
bash
cd backend
npm install

Create a .env file:

env
MONGO_URI=your_mongodb_connection_string
NTFY_TOPIC=vitalsentry
PORT=3000

Start the server:

bash
node server.js

Expected output:

text
MongoDB connected
ArogyaDrishti AI backend running on port 3000
3. Frontend setup
bash
cd frontend
npm install
npm run dev
4. ESP32 setup

Open the esp32/ folder in PlatformIO, set your Wi-Fi credentials and backend URL in main.cpp, then build and upload to the board (or simulate in Wokwi).

🔌 API Endpoints
Method	Endpoint	Description
POST	/api/vitals	Submit a vitals reading
GET	/api/vitals/latest	Get the latest processed reading
GET	/api/vitals/history/:patientId	Get history for a patient

Example request

json
{
  "patientId": "demo-patient",
  "heart_rate": 145,
  "spo2": 87,
  "motion": 0.2
}

Example history call

http
GET /api/vitals/history/demo-patient
🧪 Hackathon Demo

Three states demonstrate the whole system.

🟢 Normal
text
Heart Rate: 72 BPM
SpO₂: 98%
Motion: 0.4

Expected: NORMAL, Risk Score 0, 🟢 Green LED.

🟡 Attention
text
Heart Rate: 115 BPM
SpO₂: 93%
Motion: 0.8

Expected: ATTENTION, 🟡 Yellow LED, 📱 notification.

🔴 High Priority
text
Heart Rate: 145 BPM
SpO₂: 87%
Motion: 0.2

Expected: HIGH_PRIORITY, Risk Score 90, 🔴 Red LED, 📱 critical notification.

🏆 What Makes ArogyaDrishti AI Different?

Most basic IoT healthcare prototypes stop at:

text
Sensor → Data → Dashboard

ArogyaDrishti AI goes further:

text
Sensor
   ↓
Data Collection
   ↓
Risk Analysis
   ↓
Explainable Decision
   ↓
Persistent Storage
   ↓
Real-Time Notification
   ↓
Physical IoT Feedback
   ↓
Human Response

The question is not merely "Can we measure a vital?" It is "Can we turn a vital signal into understandable, timely action?"

🌍 Real-World Impact

Designed for remote monitoring where continuous human observation is not always practical.

🏠 Remote patient monitoring
👴 Elder-care monitoring
🏥 Hospital observation workflows
🏡 Home healthcare
🚑 Remote health-support scenarios
🧑‍⚕️ Caregiver monitoring systems

The prototype shows how low-cost IoT hardware and software intelligence can create a more responsive monitoring workflow.

🔮 Future Roadmap
Phase 1: Current MVP
 Heart-rate monitoring
 SpO₂ monitoring
 Motion monitoring
 Risk scoring
 Explainable alerts
 MongoDB persistence
 React dashboard
 ESP32 interface
 OLED display
 LED-based status indication
 ntfy notifications
Phase 2: Intelligent Monitoring
 Personalized baselines
 Trend-based risk detection
 Adaptive alert escalation
 ML-based anomaly detection
 False-alert reduction
 Activity-aware interpretation
Phase 3: Expanded Platform
 ECG integration
 Temperature monitoring
 Respiration monitoring
 Multi-patient monitoring
 Caregiver accounts
 Secure authentication
 Encrypted health-data pipeline
 Clinical validation
🔐 Safety & Responsible Use

ArogyaDrishti AI is a prototype built for educational and hackathon purposes. It is not intended to:

Diagnose medical conditions
Replace healthcare professionals
Replace clinical monitoring equipment
Provide treatment recommendations

Real-world clinical deployment would require appropriate clinical validation, sensor accuracy validation, data privacy, cybersecurity, regulatory compliance, and professional medical oversight.

💭 Our Vision

From raw signals to meaningful action.

ArogyaDrishti AI demonstrates how IoT, intelligent software, and real-time communication can turn health monitoring from passive data collection into an actionable early-warning workflow.

We believe technology should not only see the signal. It should help people understand what changed and respond in time.

👥 Team

The OGs

<div align="center">
❤️ ArogyaDrishti AI

See the Signal. Understand the Risk. Act in Time.

Built with IoT • AI-assisted Analytics • Cloud Data • Real-Time Alerts

</div>

⚠️ Disclaimer: ArogyaDrishti AI is a prototype for educational and hackathon purposes. Its outputs are not medical diagnoses and should not be used as a substitute for professi
