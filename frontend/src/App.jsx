import { useEffect, useState } from "react";
import {
  LineChart,
  Line,
  XAxis,
  YAxis,
  CartesianGrid,
  Tooltip,
  ResponsiveContainer,
} from "recharts";

import "./App.css";

function App() {
  const [vitals, setVitals] = useState({
    heart_rate: 0,
    spo2: 0,
    motion: 0,
    status: "NORMAL",
    riskScore: 0,
    explanation: [],
    timestamp: null,
  });

  const [history, setHistory] = useState([]);

  // Fetch latest vitals from Node.js backend
  const fetchVitals = async () => {
    try {
      const response = await fetch(
        "http://localhost:3000/api/vitals/latest"
      );

      const result = await response.json();

      const data = result.data || result;

      setVitals({
        heart_rate: Number(data.heart_rate) || 0,
        spo2: Number(data.spo2) || 0,
        motion: Number(data.motion) || 0,
        status: data.status || "NORMAL",
        riskScore: Number(data.riskScore) || 0,
        explanation: data.explanation || [],
        timestamp: data.timestamp || data.createdAt || null,
      });

      // Add latest reading to chart history
      setHistory((previous) => {
        const newPoint = {
          time: new Date().toLocaleTimeString(),
          heart_rate: Number(data.heart_rate) || 0,
          spo2: Number(data.spo2) || 0,
        };

        return [...previous, newPoint].slice(-20);
      });
    } catch (error) {
      console.error("Dashboard API error:", error);
    }
  };

  // Poll backend every 2 seconds
  useEffect(() => {
    fetchVitals();

    const interval = setInterval(fetchVitals, 2000);

    return () => clearInterval(interval);
  }, []);

  // Determine risk class
  const getRiskClass = () => {
    switch (vitals.status) {
      case "HIGH_PRIORITY":
        return "high";

      case "ATTENTION":
        return "attention";

      default:
        return "normal";
    }
  };

  // Risk icon
  const getRiskIcon = () => {
    switch (vitals.status) {
      case "HIGH_PRIORITY":
        return "🔴";

      case "ATTENTION":
        return "🟡";

      default:
        return "🟢";
    }
  };

  // Format status for display
  const getReadableStatus = () => {
    switch (vitals.status) {
      case "HIGH_PRIORITY":
        return "High Priority";

      case "ATTENTION":
        return "Attention";

      default:
        return "Normal";
    }
  };

  return (
    <div className="app">

      {/* ================= HEADER ================= */}

      <header className="header">

        <div className="brand">
          <h1>ArogyaDrishti AI </h1>

          <p>
            AI-Assisted Multi-Signal Health Monitoring
          </p>
        </div>

      </header>


      {/* ================= PATIENT INFORMATION ================= */}

      <section className="patient-bar neumorphic">

        <div className="patient-info">

          <span>Patient</span>

          <strong>
            Demo Patient
          </strong>

        </div>


        <div className="patient-info">

          <span>Last Update</span>

          <strong>

            {vitals.timestamp
              ? new Date(vitals.timestamp).toLocaleTimeString()
              : "--"}

          </strong>

        </div>

      </section>


      {/* ================= VITAL CARDS ================= */}

      <section className="vitals-grid">


        {/* HEART RATE */}

        <div className="vital-card neumorphic">

          <div className="card-icon heart-icon">
            ♥
          </div>

          <div className="card-title">
            HEART RATE
          </div>

          <div className="vital-value">

            {vitals.heart_rate}

            <span>
              BPM
            </span>

          </div>

          <div className="card-subtitle">
            Beats per minute
          </div>

        </div>


        {/* SPO2 */}

        <div className="vital-card neumorphic">

          <div className="card-icon oxygen-icon">
            O₂
          </div>

          <div className="card-title">
            SpO₂
          </div>

          <div className="vital-value">

            {vitals.spo2}

            <span>
              %
            </span>

          </div>

          <div className="card-subtitle">
            Oxygen saturation
          </div>

        </div>


        {/* MOTION */}

        <div className="vital-card neumorphic">

          <div className="card-icon motion-icon">
            ◈
          </div>

          <div className="card-title">
            MOTION
          </div>

          <div className="vital-value">

            {Number(vitals.motion).toFixed(2)}

          </div>

          <div className="card-subtitle">
            Motion intensity
          </div>

        </div>

      </section>


      {/* ================= CHART + RISK ================= */}

      <section className="dashboard-grid">


        {/* VITAL TRENDS */}

        <div className="panel neumorphic chart-panel">

          <div className="panel-header">

            <div>

              <h2>
                Vital Trends
              </h2>

              <p>
                Real-time physiological signals
              </p>

            </div>

          </div>


          <div className="chart-container">

            <ResponsiveContainer
              width="100%"
              height="100%"
            >

              <LineChart data={history}>

                <CartesianGrid
                  strokeDasharray="3 3"
                  strokeOpacity={0.15}
                />

                <XAxis
                  dataKey="time"
                  tick={{ fontSize: 11 }}
                />

                <YAxis
                  tick={{ fontSize: 11 }}
                />

                <Tooltip />

                <Line
                  type="monotone"
                  dataKey="heart_rate"
                  name="Heart Rate"
                  stroke="#8D37FF"
                  strokeWidth={3}
                  dot={false}
                  activeDot={{ r: 5 }}
                />

                <Line
                  type="monotone"
                  dataKey="spo2"
                  name="SpO₂"
                  stroke="#16A34A"
                  strokeWidth={3}
                  dot={false}
                  activeDot={{ r: 5 }}
                />

              </LineChart>

            </ResponsiveContainer>

          </div>

        </div>


        {/* AI RISK */}

        <div
          className={`panel neumorphic risk-panel ${getRiskClass()}`}
        >

          <div className="risk-header">

            <h2>
              AI Risk Status
            </h2>

            <span className="ai-badge">
              AI
            </span>

          </div>


          <div className="risk-status">

            <span className="risk-icon">
              {getRiskIcon()}
            </span>

            <span>
              {getReadableStatus()}
            </span>

          </div>


          <div className="risk-score">

            <span>
              Risk Score
            </span>

            <strong>

              {vitals.riskScore}

              <small>
                /100
              </small>

            </strong>

          </div>


          <div className="risk-description">

            {vitals.status === "HIGH_PRIORITY"
              ? "Immediate review of the detected pattern is recommended."
              : vitals.status === "ATTENTION"
              ? "Some monitored signals differ from the expected pattern."
              : "Current monitored signals are within the configured range."}

          </div>

        </div>

      </section>


      {/* ================= AI EXPLANATION ================= */}

      <section className="panel neumorphic explanation-panel">

        <div className="panel-header">

          <div>

            <h2>
              AI Explanation
            </h2>

            <p>
              Why the current risk status was generated
            </p>

          </div>

        </div>


        {vitals.explanation &&
        vitals.explanation.length > 0 ? (

          <ul className="explanation-list">

            {vitals.explanation.map(
              (reason, index) => (

                <li key={index}>

                  <span className="warning-icon">
                    ⚠
                  </span>

                  <span>
                    {reason}
                  </span>

                </li>

              )
            )}

          </ul>

        ) : (

          <div className="normal-message">

            <span>
              ✓
            </span>

            <span>
              No significant abnormal pattern detected.
            </span>

          </div>

        )}

      </section>

    </div>
  );
}

export default App;