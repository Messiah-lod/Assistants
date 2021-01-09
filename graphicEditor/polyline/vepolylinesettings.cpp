#include "vepolylinesettings.h"

#include <QColorDialog>
#include <QBrush>
#include <QPen>
#include <QDebug>
#include "vepolyline.h"

VEPolyLineSettings::VEPolyLineSettings(QWidget *parent) :
    QWidget(parent),
    currentPolyline(nullptr)
{
	centralWidget = new QGridLayout();
	centralWidget->setContentsMargins(1, 1, 1, 1);

	label = new QLabel();
	label_2 = new QLabel();
	clLineColor = new ColorLabel();
	spLineWidth = new QSpinBox();
	verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

	centralWidget->addWidget(label, 0, 0, 1, 1);
	centralWidget->addWidget(label_2, 1, 0, 1, 1);
	centralWidget->addWidget(clLineColor, 0, 1, 1, 1);
	centralWidget->addWidget(spLineWidth, 1, 1, 1, 1);
	centralWidget->addItem(verticalSpacer, 3, 0, 1, 2);

	setLineColor(QColor(Qt::black));
    setLineWidth(1);

    connect(clLineColor, &ColorLabel::clicked,
            [=](){
        QColorDialog dialog;
        connect(&dialog, &QColorDialog::colorSelected, this, &VEPolyLineSettings::setLineColor);
        dialog.exec();
    });

    connect(spLineWidth, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &VEPolyLineSettings::setLineWidth);


	setLayout(centralWidget);
	retranslateUi();
}

void VEPolyLineSettings::retranslateUi()
{
	label->setText(tr("Color"));
	label_2->setText(tr("Width"));
}

VEPolyLineSettings::~VEPolyLineSettings()
{
    delete currentPolyline;
}

QColor VEPolyLineSettings::lineColor() const
{
    return m_lineColor;
}

int VEPolyLineSettings::lineWidth() const
{
    return m_lineWidth;
}

void VEPolyLineSettings::setLineColor(const QColor &color)
{
    m_lineColor = color;
	clLineColor->setColor(m_lineColor);
    if(currentPolyline != nullptr){
        QPen pen(color,currentPolyline->pen().width());
        currentPolyline->setPen(pen);
    }
    emit lineColorChanged(m_lineColor);
}

void VEPolyLineSettings::setLineWidth(const int &width)
{
    m_lineWidth = width;
	spLineWidth->setValue(m_lineWidth);
    if(currentPolyline != nullptr){
        QPen pen(currentPolyline->pen().color(),width);
        currentPolyline->setPen(pen);
    }
    emit lineWidthChanged(m_lineWidth);
}

void VEPolyLineSettings::newPolyline(VEPolyline *polyline)
{
    polyline->setBrush(QBrush(Qt::transparent));
    polyline->setPen(QPen(m_lineColor, m_lineWidth));
}

void VEPolyLineSettings::loadPolyline(VEPolyline *polyline)
{
    currentPolyline = polyline;
    m_lineColor = currentPolyline->pen().color();
    m_lineWidth = currentPolyline->pen().width();
	clLineColor->setColor(m_lineColor);
	spLineWidth->setValue(m_lineWidth);
}

void VEPolyLineSettings::deselect()
{
    currentPolyline = nullptr;
}

void VEPolyLineSettings::setVisible(bool visible)
{
//	QWidget::setVisible(visible);
    if(!visible && currentPolyline != nullptr)
	{
        QWidget::setVisible(true);
    } else {
        QWidget::setVisible(visible);
    }
}


