#pragma once
#include "MonodParameters.h"
#include "ParameterPanel.h"

class KineticParametersPanel : public ParameterPanel {
    Q_OBJECT
    QLineEdit *m_Ks;
    QLineEdit *m_muMax;
    QLineEdit *m_Yxs;
    QLineEdit *m_Ki;
    QLineEdit *m_kd;

public:
    KineticParametersPanel(QWidget *parent = nullptr, double Ks = 1.0, double muMax = 1.5, double Yxs = 6.6, double Ki = 100.0, double kd = 0.3);
    MonodParameters toParameters() const;
};
