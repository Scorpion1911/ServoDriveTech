#ifndef MASKWHEELCOMBOBOX_H
#define MASKWHEELCOMBOBOX_H

#include <QComboBox>
#include "uiioconf_global.h"

class MaskWheelComboBox : public QComboBox
{
public:
    explicit MaskWheelComboBox(QWidget *parent = 0);
    ~MaskWheelComboBox();

protected:
    void wheelEvent(QWheelEvent *e) Q_DECL_OVERRIDE;

};

#endif // MASKWHEELCOMBOBOX_H
