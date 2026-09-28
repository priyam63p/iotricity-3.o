const mongoose = require("mongoose");

const vitalSchema = new mongoose.Schema(
  {
    patientId: {
      type: String,
      default: "demo-patient"
    },

    heart_rate: {
      type: Number,
      required: true
    },

    spo2: {
      type: Number,
      required: true
    },

    motion: {
      type: Number,
      default: 0
    },

    status: {
      type: String,
      enum: ["NORMAL", "ATTENTION", "HIGH_PRIORITY"],
      default: "NORMAL"
    },

    alert: {
      type: Boolean,
      default: false
    },

    riskScore: {
      type: Number,
      default: 0
    },

    explanation: {
      type: [String],
      default: []
    }
  },
  {
    timestamps: true
  }
);

module.exports = mongoose.model("Vital", vitalSchema);