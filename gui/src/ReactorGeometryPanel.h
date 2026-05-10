#pragma once
#include "ParameterPanel.h"
#include "ReactorGeometry.h"

class ReactorGeometryPanel : public ParameterPanel {
    Q_OBJECT
    QLineEdit* m_depth;
    QLineEdit* m_I0;
    QLineEdit* m_k;
public:
    ReactorGeometryPanel(QWidget* parent = nullptr
        , double depth = 0.05, double I0 = 200.0, double k = 0.2);
    ReactorGeometry toGeometry() const;
};
