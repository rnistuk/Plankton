#pragma once
#include "ParameterPanel.h"

class ReactorGeometry : public ParameterPanel {
    Q_OBJECT
    QLineEdit* m_depth;
    QLineEdit* m_I0;
    QLineEdit* m_k;
public:
    ReactorGeometry(QWidget* parent = nullptr);
};
