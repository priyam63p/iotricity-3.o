require("dotenv").config();

const express = require("express");
const mongoose = require("mongoose");
const cors = require("cors");

const vitalsRoutes = require("./routes/vitals");

const app = express();


// =====================================
// Middleware
// =====================================

app.use(cors());
app.use(express.json());


// =====================================
// Health check
// =====================================

app.get("/", (req, res) => {

  res.json({
    project: "VitalSentry AI",
    status: "Backend running"
  });

});


// =====================================
// API routes
// =====================================

app.use("/api/vitals", vitalsRoutes);


// =====================================
// MongoDB
// =====================================

mongoose
  .connect(process.env.MONGO_URI)
  .then(() => {

    console.log("MongoDB connected");

    const PORT = process.env.PORT || 3000;

    app.listen(PORT, "0.0.0.0", () => {

      console.log(
        `VitalSentry backend running on port ${PORT}`
      );

    });

  })
  .catch((error) => {

    console.error(
      "MongoDB connection failed:",
      error.message
    );

  });