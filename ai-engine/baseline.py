import numpy as np


class PersonalBaseline:

    def __init__(self):

        self.baseline = {
            "heart_rate": 75,
            "spo2": 98,
            "motion": 1.0
        }


    def update(self, history):

        if not history:
            return self.baseline

        heart_rates = [
            x["heart_rate"]
            for x in history
            if x.get("heart_rate") is not None
        ]

        spo2_values = [
            x["spo2"]
            for x in history
            if x.get("spo2") is not None
        ]

        motion_values = [
            x["motion"]
            for x in history
            if x.get("motion") is not None
        ]

        if heart_rates:
            self.baseline["heart_rate"] = float(
                np.mean(heart_rates)
            )

        if spo2_values:
            self.baseline["spo2"] = float(
                np.mean(spo2_values)
            )

        if motion_values:
            self.baseline["motion"] = float(
                np.mean(motion_values)
            )

        return self.baseline


    def deviation(self, vitals):

        return {
            "heart_rate": abs(
                vitals["heart_rate"]
                - self.baseline["heart_rate"]
            ),

            "spo2": abs(
                vitals["spo2"]
                - self.baseline["spo2"]
            ),

            "motion": abs(
                vitals["motion"]
                - self.baseline["motion"]
            )
        }