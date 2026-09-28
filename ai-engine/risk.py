def calculate_risk(vitals, baseline, anomaly_score):

    hr = vitals["heart_rate"]
    spo2 = vitals["spo2"]
    motion = vitals["motion"]

    reasons = []

    risk_score = 0


    # ==================================
    # Heart rate analysis
    # ==================================

    hr_difference = hr - baseline["heart_rate"]

    if hr > 130:

        risk_score += 35

        reasons.append(
            "Heart rate is significantly elevated"
        )

    elif hr_difference > 25:

        risk_score += 20

        reasons.append(
            "Heart rate is above the personal baseline"
        )


    # ==================================
    # SpO2 analysis
    # ==================================

    if spo2 < 90:

        risk_score += 40

        reasons.append(
            "SpO₂ is below the configured threshold"
        )

    elif spo2 < 94:

        risk_score += 20

        reasons.append(
            "SpO₂ is below the configured normal range"
        )


    # ==================================
    # Motion analysis
    # ==================================

    motion_difference = abs(
        motion - baseline["motion"]
    )

    if motion_difference > 2:

        risk_score += 10

        reasons.append(
            "Motion pattern differs from baseline"
        )


    # ==================================
    # ML anomaly
    # ==================================

    if anomaly_score > 60:

        risk_score += 20

        reasons.append(
            "Multiple signals show an unusual pattern"
        )


    risk_score = min(
        100,
        risk_score
    )


    # ==================================
    # Final status
    # ==================================

    if risk_score >= 60:

        status = "HIGH_PRIORITY"

    elif risk_score >= 25:

        status = "ATTENTION"

    else:

        status = "NORMAL"


    return {
        "risk_score": risk_score,
        "status": status,
        "reasons": reasons
    }