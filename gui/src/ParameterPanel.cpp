#include "ParameterPanel.h"

#include <QLabel>

ParameterPanel::ParameterPanel(QWidget *parent) : QGroupBox(parent), m_row(0) {
    this->m_layout = new QGridLayout;
    this->setLayout(this->m_layout);
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    this->setStyleSheet("QGroupBox { font-size: 16px; font-weight: bold; padding-top: 20px; }"
        "QGroupBox::title { background-color: transparent; }");
}

QLineEdit *ParameterPanel::addRow(const QString &name, const QString &label, const QString &value,
                                  const QString &units, QValidator *validator) {
    QLineEdit *ret = new QLineEdit(value);
    ret->setObjectName(name);
    this->m_layout->addWidget(new QLabel(label), this->m_row, 0);
    this->m_layout->addWidget(ret, this->m_row, 1);
    this->m_layout->addWidget(new QLabel(units), this->m_row, 2);
    ++this->m_row;

    connect(ret, &QLineEdit::editingFinished, this, &ParameterPanel::parametersChanged);
    ret->setValidator(validator);

    return ret;
}
