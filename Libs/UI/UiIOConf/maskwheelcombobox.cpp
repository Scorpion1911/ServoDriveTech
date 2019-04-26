#include "maskwheelcombobox.h"

#define UN_USED(x) (void)x

MaskWheelComboBox::MaskWheelComboBox(QWidget *parent):
    QComboBox(parent)
{

}

MaskWheelComboBox::~MaskWheelComboBox()
{

}

void MaskWheelComboBox::wheelEvent(QWheelEvent *e)
{
    UN_USED(e);
    return ;
}

