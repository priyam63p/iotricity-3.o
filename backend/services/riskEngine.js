function calculateRisk(vitals) {
  const {
    heart_rate,
    spo2,
    motion
  } = vitals;

  let riskScore = 0;
  const reasons = [];

  // -------------------------
  // Heart rate
  // -------------------------

  if (heart_rate > 130) {
    riskScore += 40;
    reasons.push("Heart rate is elevated");
  } else if (heart_rate > 110) {
    riskScore += 20;
    reasons.push("Heart rate is above the configured baseline range");
  }

  if (heart_rate < 50) {
    riskScore += 30;
    reasons.push("Heart rate is unusually low");
  }

  // -------------------------
  // SpO2
  // -------------------------

  if (spo2 < 90) {
    riskScore += 50;
    reasons.push("SpO₂ is below the configured threshold");
  } else if (spo2 < 94) {
    riskScore += 25;
    reasons.push("SpO₂ is lower than the configured normal range");
  }

  // -------------------------
  // Motion
  // -------------------------

  if (motion > 2.5) {
    riskScore += 10;
    reasons.push("Unusual movement detected");
  }

  // Don't exceed 100
  riskScore = Math.min(riskScore, 100);

  // -------------------------
  // Risk status
  // -------------------------

  let status;

  if (riskScore >= 60) {
    status = "HIGH_PRIORITY";
  } else if (riskScore >= 25) {
    status = "ATTENTION";
  } else {
    status = "NORMAL";
  }

  return {
    riskScore,
    status,
    alert: status !== "NORMAL",
    explanation: reasons
  };
}

module.exports = {
  calculateRisk
};