const express = require("express");
const Vital = require("../models/Vital");
const { calculateRisk } = require("../services/riskEngine");
const { sendNotification } = require("../services/notificationService");

const router = express.Router();




// =====================================
// POST /api/vitals
// Receive data from ESP32
// =====================================

router.post("/", async (req, res) => {

  try {

    const {
      patientId = "demo-patient",
      heart_rate,
      spo2,
      motion = 0
    } = req.body;

    // Validate input
    if (
      heart_rate === undefined ||
      spo2 === undefined
    ) {
      return res.status(400).json({
        success: false,
        message: "heart_rate and spo2 are required"
      });
    }

    // -------------------------
    // Risk analysis
    // -------------------------

    const analysis = calculateRisk({
      heart_rate,
      spo2,
      motion
    });

    // 🔔 ADD NOTIFICATION HERE
  if (analysis.status !== "NORMAL") {
  await sendNotification({
    heart_rate,
    spo2,
    motion,
    riskScore: analysis.riskScore,
    status: analysis.status,
    explanation: analysis.explanation
  });
}

    // -------------------------
    // Save to MongoDB
    // -------------------------

    const vital = await Vital.create({
      patientId,
      heart_rate,
      spo2,
      motion,

      status: analysis.status,
      alert: analysis.alert,

      riskScore: analysis.riskScore,

      explanation: analysis.explanation
    });

    // -------------------------
    // Response
    // -------------------------

    res.status(201).json({
      success: true,

      data: {
        heart_rate,
        spo2,
        motion,

        riskScore: analysis.riskScore,
        status: analysis.status,

        alert: analysis.alert,

        explanation: analysis.explanation,

        timestamp: vital.createdAt
      }
    });

  } catch (error) {

    console.error(error);

    res.status(500).json({
      success: false,
      message: "Failed to process vitals"
    });

  }

});


// =====================================
// GET latest vital
// =====================================

router.get("/latest", async (req, res) => {

  try {

    const latest = await Vital
      .findOne()
      .sort({ createdAt: -1 });

    res.json({
      success: true,
      data: latest
    });

  } catch (error) {

    res.status(500).json({
      success: false,
      message: "Failed to fetch latest vitals"
    });

  }

});


// =====================================
// GET patient history
// =====================================

router.get("/history/:patientId", async (req, res) => {

  try {

    const history = await Vital
      .find({
        patientId: req.params.patientId
      })
      .sort({
        createdAt: -1
      })
      .limit(100);

    res.json({
      success: true,
      data: history
    });

  } catch (error) {

    res.status(500).json({
      success: false,
      message: "Failed to fetch history"
    });

  }

});


module.exports = router;