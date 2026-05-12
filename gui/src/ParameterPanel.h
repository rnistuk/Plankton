#pragma once
#include <QGroupBox>
#include <QGridLayout>
#include <QLineEdit>
#include <QString>


class ParameterPanel : public QGroupBox {
    Q_OBJECT
    QGridLayout* m_layout;
    int m_row = 0;
public:
    explicit ParameterPanel(QWidget* parent = nullptr);
    QLineEdit* addRow(const QString& name, const QString &label, const QString &value, const QString &units);
signals:
    void parametersChanged();
};
