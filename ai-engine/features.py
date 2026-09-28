def extract_features(vitals, baseline):

    hr = vitals["heart_rate"]
    spo2 = vitals["spo2"]
    motion = vitals["motion"]

    hr_baseline = baseline["heart_rate"]
    spo2_baseline = baseline["spo2"]
    motion_baseline = baseline["motion"]

    hr_deviation = abs(hr - hr_baseline)

    spo2_deviation = abs(spo2 - spo2_baseline)

    motion_deviation = abs(motion - motion_baseline)

    return [
        hr,
        spo2,
        motion,
        hr_deviation,
        spo2_deviation,
        motion_deviation
    ]