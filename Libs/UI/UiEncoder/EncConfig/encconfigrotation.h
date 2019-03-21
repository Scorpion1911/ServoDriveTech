#ifndef ENCCONFIGROTATION_H
#define ENCCONFIGROTATION_H

#include <QObject>
#include "iencconfigitem.h"

class QSpinBox;

class EncConfigRotation : public IEncConfigItem
{
    Q_OBJECT
public:
    explicit EncConfigRotation(QObject *parent = 0);
    ~EncConfigRotation();
    void createAttributeUi() Q_DECL_OVERRIDE;
    void updateAttributeUi() Q_DECL_OVERRIDE;
    bool execute() Q_DECL_OVERRIDE;

signals:

public slots:
private slots:
    void onPolesNumChanged(int num);
private:
    QSpinBox* m_polesNum;
};

#endif // ENCCONFIGROTATION_H
