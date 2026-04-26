#include "ParameterPanel.h"

#include <QLabel>

ParameterPanel::ParameterPanel(QWidget* parent) : QGroupBox(parent), m_row(0) {
    this->m_layout = new QGridLayout;
    this->setLayout(this->m_layout);
}

QLineEdit* ParameterPanel::addRow(const QString& label, const QString& value, const QString& units) {
    QLineEdit* ret = new QLineEdit(value);
    this->m_layout->addWidget( new QLabel(label), this->m_row, 0);
    this->m_layout->addWidget( ret, this->m_row, 1);
    this->m_layout->addWidget( new QLabel(units), this->m_row, 2);
    ++this->m_row;

    return ret;
}
