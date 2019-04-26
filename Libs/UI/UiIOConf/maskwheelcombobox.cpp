#include "maskwheelcombobox.h"

MaskWheelComboBox::MaskWheelComboBox(QWidget *parent):
    QComboBox(parent)
{

}

MaskWheelComboBox::~MaskWheelComboBox()
{

}

void MaskWheelComboBox::wheelEvent(QWheelEvent *e)
{
    Q_UNUSED(e);
    return ;
}

