#ifndef ENCCONFIGSINHIPERFACE_H
#define ENCCONFIGSINHIPERFACE_H

#include <QObject>
#include "iencconfigitem.h"

class QSpinBox;
class QCheckBox;

class EncConfigSinHiperface : public IEncConfigItem
{
    Q_OBJECT
public:
    explicit EncConfigSinHiperface(QObject *parent = 0);
    ~EncConfigSinHiperface();
    void createAttributeUi() Q_DECL_OVERRIDE;
    void updateAttributeUi() Q_DECL_OVERRIDE;
    bool execute() Q_DECL_OVERRIDE;
    void setAbsPos(quint32 value) Q_DECL_OVERRIDE;

signals:

public slots:
private slots:
    void onSinNumChanged(int num);
private:
    QSpinBox* m_sinPerR;
    QSpinBox* m_absLineNumBox;
    QCheckBox* m_reverseBox;
    QSpinBox* m_absPos;
};

#endif // ENCCONFIGSINHIPERFACE_H
