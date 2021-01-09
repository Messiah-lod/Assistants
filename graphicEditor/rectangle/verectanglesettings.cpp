#include "verectanglesettings.h"
#include <QColorDialog>
#include <QBrush>
#include <QPen>
#include <QDebug>
#include "verectangle.h"

#define BOOL_STR(b) (b?"true":"false")

VERectangleSettings::VERectangleSettings(QWidget *parent) :
    QWidget(parent),
    currentRectangle(nullptr)
{
	centralWidget = new QGridLayout();
	centralWidget->setContentsMargins(1, 1, 1, 1);

	comboBox = new QComboBox();
	comboBox->addItem(QString());
	comboBox->addItem(QString());

	fillingWidget = new QWidget();
	fillingWidgetLayout = new QGridLayout(fillingWidget);
	verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

	centralWidget->addWidget(comboBox, 0, 0, 1, 1);
	centralWidget->addWidget(fillingWidget, 1, 0, 1, 1);
	centralWidget->addItem(verticalSpacer, 2, 0, 1, 1);

	labelColor_1 = new QLabel();
	labelColor_2 = new QLabel();
	label_2 = new QLabel();
	label_4 = new QLabel();

	clBorderColor = new ColorLabel();
	clColor_1 = new ColorLabel();
	clColor_2 = new ColorLabel();
	cbBorderWidth = new QSpinBox();
	cbBorderWidth->setValue(1);

	currentRectangle = new VERectangle();

	fillingWidgetLayout->addWidget(labelColor_1, 0, 0, 1, 1);
	fillingWidgetLayout->addWidget(clColor_1, 0, 1, 1, 1);
	fillingWidgetLayout->addWidget(labelColor_2, 1, 0, 1, 1);
	fillingWidgetLayout->addWidget(clColor_2, 1, 1, 1, 1);
	fillingWidgetLayout->addWidget(label_2, 2, 0, 1, 1);
	fillingWidgetLayout->addWidget(clBorderColor, 2, 1, 1, 1);
	fillingWidgetLayout->addWidget(label_4, 3, 0, 1, 1);
	fillingWidgetLayout->addWidget(cbBorderWidth, 3, 1, 1, 1);

	clColor_2->setVisible(false);
    labelColor_2->setVisible(false);

    setColor_1(Qt::black);
    setColor_2(Qt::black);
    setBorderColor(Qt::black);
    setBorderWidth(1);

    connect(clColor_1, &ColorLabel::clicked,
            [=](){
        QColorDialog dialog;
        connect(&dialog, &QColorDialog::colorSelected, this, &VERectangleSettings::setColor_1);
        dialog.exec();
    });
    connect(clColor_2, &ColorLabel::clicked,
            [=](){
        QColorDialog dialog;
        connect(&dialog, &QColorDialog::colorSelected, this, &VERectangleSettings::setColor_2);
        dialog.exec();
    });
    connect(clBorderColor, &ColorLabel::clicked,
            [=](){
        QColorDialog dialog;
        connect(&dialog, &QColorDialog::colorSelected, this, &VERectangleSettings::setBorderColor);
        dialog.exec();
    });

    connect(comboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &VERectangleSettings::comboBoxIndexChanged);
    connect(cbBorderWidth, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &VERectangleSettings::setBorderWidth);

	setLayout(centralWidget);
	retranslateUi();
}

VERectangleSettings::~VERectangleSettings()
{
    delete currentRectangle;
}

void VERectangleSettings::retranslateUi()
{
	labelColor_1->setText(tr("Background"));
	labelColor_2->setText(tr("Color 2"));
	label_2->setText(tr("Border color"));
	label_4->setText(tr("Border width"));
	comboBox->setItemText(0, tr("filling"));
	comboBox->setItemText(1, tr("gradient"));
}

QColor VERectangleSettings::color_1() const
{
    return m_color_1;
}

QColor VERectangleSettings::color_2() const
{
    return m_color_2;
}

QColor VERectangleSettings::borderColor() const
{
    return m_borderColor;
}

int VERectangleSettings::borderWidth() const
{
    return m_borderWidth;
}

void VERectangleSettings::setColor_1(const QColor &color)
{
    m_color_1 = color;
	clColor_1->setColor(color);
    if(currentRectangle != nullptr){
        if(comboBox->currentIndex() == 0){
            currentRectangle->setBrush(QBrush(m_color_1));
        } else {
             setGradient(currentRectangle);
        }
    }
    emit color_1Changed(m_color_1);
}

void VERectangleSettings::setColor_2(const QColor &color)
{
    m_color_2 = color;
	clColor_2->setColor(color);
    if(currentRectangle != nullptr){
         setGradient(currentRectangle);
    }
    emit color_2Changed(m_color_2);
}

void VERectangleSettings::setBorderColor(const QColor &color)
{
    m_borderColor = color;
	clBorderColor->setColor(color);
    if(currentRectangle != nullptr){
        QPen pen(color,currentRectangle->pen().width());
        currentRectangle->setPen(pen);
    }
    emit borderColorChanged(m_borderColor);
}

void VERectangleSettings::setBorderWidth(const int &width)
{
    m_borderWidth = width;
    if(currentRectangle != nullptr){
        if(width == 0){
            currentRectangle->setPen(Qt::NoPen);
        } else {
            QPen pen(currentRectangle->pen().color(), width);
            currentRectangle->setPen(pen);
        }
    }
    emit borderWidthChanged(m_borderWidth);
}

void VERectangleSettings::newRectangle(VERectangle *rect)
{
    if(comboBox->currentIndex() == 0){
        rect->setBrush(QBrush(m_color_1));
    } else {
        setGradient(rect);
    }
    if(m_borderWidth == 0){
        rect->setPen(Qt::NoPen);
    } else {
        rect->setPen(QPen(m_borderColor, m_borderWidth));
    }
}

void VERectangleSettings::loadRectangle(VERectangle *rect)
{
    currentRectangle = rect;

    m_color_1 = currentRectangle->brush().color();
    m_borderColor = currentRectangle->pen().color();
    if(currentRectangle->pen().style() == Qt::NoPen){
        m_borderWidth = 0;
    } else {
        m_borderWidth = currentRectangle->pen().width();
    }
	clColor_1->setColor(m_color_1);
	clBorderColor->setColor(m_borderColor);
	cbBorderWidth->setValue(m_borderWidth);
}

void VERectangleSettings::deselect()
{
    currentRectangle = nullptr;
}

void VERectangleSettings::setVisible(bool visible)
{
	//QMessageBox *a;
	//a = new QMessageBox();
	//if (visible)
	//{
	//	a->setText("visible true");
	//}
	//else
	//{
	//	a->setText("visible false");
	//}
	//a->exec();

	//QWidget::setVisible(visible);

    if(!visible && currentRectangle != nullptr){
        QWidget::setVisible(true);
    } else {
        QWidget::setVisible(visible);
    }
}

void VERectangleSettings::comboBoxIndexChanged(int index)
{
    switch (index) {
    case 1:
		clColor_2->setVisible(true);
        labelColor_2->setVisible(true);
 //      labelColor_1->setText(trUtf8("color 1"));
        if(currentRectangle != nullptr){
            setGradient(currentRectangle);
        }
        break;
    default:
		clColor_2->setVisible(false);
        labelColor_2->setVisible(false);
//        labelColor_1->setText(trUtf8("background"));
        if(currentRectangle != nullptr){
            currentRectangle->setBrush(QBrush(m_color_1));
        }
        break;
    }
}

void VERectangleSettings::setGradient(VERectangle *rectangle)
{
    auto tmpRect = rectangle->rect();
    QLinearGradient gradient(tmpRect.left() + tmpRect.width()/2,
                             tmpRect.top(),
                             tmpRect.left() + tmpRect.width()/2,
                             tmpRect.bottom());
    gradient.setColorAt(0.0, m_color_1);
    gradient.setColorAt(1.0, m_color_2);
    rectangle->setBrush(QBrush(gradient));
}
