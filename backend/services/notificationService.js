const https = require("https");

const NTFY_TOPIC = process.env.NTFY_TOPIC || "vitalsentry";
const NTFY_HOST = "ntfy.sh";

function sendNotification({
  heart_rate,
  spo2,
  motion,
  riskScore,
  status,
  explanation = []
}) {
  return new Promise((resolve, reject) => {
    // No notification for normal readings
    if (status === "NORMAL") {
      return resolve();
    }

    const priority =
      status === "HIGH_PRIORITY" ? "urgent" : "high";

    // IMPORTANT:
    // No emoji in HTTP headers
    const title =
      status === "HIGH_PRIORITY"
        ? "VitalSentry Critical Alert"
        : "VitalSentry Attention Alert";

    // Emojis are safe inside the message body
    const emoji =
      status === "HIGH_PRIORITY" ? "🚨" : "⚠️";

    const reasons =
      explanation.length > 0
        ? explanation.map(reason => `• ${reason}`).join("\n")
        : "Abnormal vital signs detected";

    const message = `${emoji} ${title}

Heart Rate: ${heart_rate} BPM
SpO2: ${spo2}%
Motion: ${motion}
Risk Score: ${riskScore}/100
Status: ${status}

Reasons:
${reasons}`;

    const options = {
      hostname: NTFY_HOST,
      port: 443,
      path: `/${encodeURIComponent(NTFY_TOPIC)}`,
      method: "POST",

      headers: {
        "Content-Type": "text/plain; charset=utf-8",
        "Content-Length": Buffer.byteLength(message),

        // ASCII only
        "Title": title,
        "Priority": priority
      }
    };

    const request = https.request(options, (response) => {
      let responseData = "";

      response.on("data", (chunk) => {
        responseData += chunk;
      });

      response.on("end", () => {
        if (
          response.statusCode >= 200 &&
          response.statusCode < 300
        ) {
          console.log("✅ Notification sent successfully");
          resolve();
        } else {
          console.error(
            "❌ Notification failed:",
            response.statusCode,
            responseData
          );

          reject(
            new Error(
              `ntfy returned status ${response.statusCode}`
            )
          );
        }
      });
    });

    request.on("error", (error) => {
      console.error(
        "❌ Notification request failed:",
        error.message
      );

      reject(error);
    });

    request.write(message);
    request.end();
  });
}

module.exports = {
  sendNotification
};