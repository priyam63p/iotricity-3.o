import numpy as np
from sklearn.ensemble import IsolationForest


class VitalAnomalyModel:

    def __init__(self):

        self.model = IsolationForest(
            n_estimators=100,
            contamination=0.05,
            random_state=42
        )

        self.is_trained = False


    def train(self, data):

        X = np.array(data)

        self.model.fit(X)

        self.is_trained = True


    def predict(self, features):
        if not self.is_trained:
            return {
            "anomaly": False,
            "score": 0
        }

        X = np.array([features])

        prediction = self.model.predict(X)[0]
        anomaly_score = self.model.decision_function(X)[0]

        normalized_score = max(
        0,
        min(
            100,
            int((0.5 - float(anomaly_score)) * 100)
        )
    )

        return {
            "anomaly": bool(prediction == -1),
            "score": int(normalized_score)
    }