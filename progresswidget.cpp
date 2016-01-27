#include "progresswidget.h"

ProgressWidget::ProgressWidget(QWidget* parent): QWidget(parent)
{
    QHBoxLayout* current = new QHBoxLayout(this);
    bar = new QProgressBar(this) ;
    label = new QLabel(this);
    current->setAlignment(Qt::AlignRight);
    current->addWidget(label);
    current->addWidget(bar);
    setLayout(current);
}

