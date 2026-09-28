from flask import Flask, request, jsonify

from baseline import PersonalBaseline
from features import extract_features
from model import VitalAnomalyModel
from risk import calculate_risk


app = Flask(__name__)


# ==========================================
# Global AI objects
# ==========================================

baseline_engine = PersonalBaseline()

anomaly_model = VitalAnomalyModel()


# ==========================================
# Demo baseline training data
# ==========================================

training_data = [

    [70, 98, 1.0, 5, 0, 0.1],
    [72, 98, 1.1, 3, 0, 0.1],
    [75, 97, 1.0, 2, 1, 0.0],
    [74, 98, 0.9, 1, 0, 0.1],
    [76, 98, 1.2, 2, 0, 0.2],
    [73, 99, 1.0, 1, 1, 0.0],
    [71, 98, 1.1, 4, 0, 0.1],
    [77, 97, 1.2, 3, 1, 0.2],
    [75, 98, 0.8, 2, 0, 0.2],
    [74, 98, 1.0, 1, 0, 0.0]

]

anomaly_model.train(training_data)


# ==========================================
# Health check
# ==========================================

@app.route("/", methods=["GET"])
def home():

    return jsonify({
        "service": "VitalSentry AI Engine",
        "status": "running"
    })


# ==========================================
# AI prediction
# ==========================================

@app.route("/analyze", methods=["POST"])
def analyze():

    try:

        data = request.get_json()

        if not data:

            return jsonify({
                "success": False,
                "message": "No data received"
            }), 400


        # ==================================
        # Extract vitals
        # ==================================

        vitals = {

            "heart_rate":
                float(data["heart_rate"]),

            "spo2":
                float(data["spo2"]),

            "motion":
                float(data.get("motion", 0))

        }


        # ==================================
        # Personal baseline
        # ==================================

        baseline = baseline_engine.baseline


        # ==================================
        # Feature extraction
        # ==================================

        features = extract_features(
            vitals,
            baseline
        )


        # ==================================
        # Anomaly detection
        # ==================================

        anomaly_result = anomaly_model.predict(
            features
        )


        # ==================================
        # Risk assessment
        # ==================================

        risk = calculate_risk(

            vitals,

            baseline,

            anomaly_result["score"]

        )


        # ==================================
        # Final response
        # ==================================

        result = {

            "success": True,

            "vitals": vitals,

            "baseline": baseline,

            "anomaly": {

                "detected": bool(anomaly_result["anomaly"]),
                "score": int(anomaly_result["score"])

            },

            "risk": risk

        }


        return jsonify(result)


    except Exception as e:

        return jsonify({

            "success": False,

            "error": str(e)

        }), 500


# ==========================================
# Run server
# ==========================================

if __name__ == "__main__":

    app.run(
        host="0.0.0.0",
        port=5000,
        debug=True
    )