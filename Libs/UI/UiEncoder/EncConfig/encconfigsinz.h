#ifndef ENCCONFIGSINZ_H
#define ENCCONFIGSINZ_H

#include <QObject>
#include <iencconfigitem.h>

class QSpinBox;

class EncConfigSinZ : public IEncConfigItem
{
    Q_OBJECT
public:
    explicit EncConfigSinZ(QObject *parent = 0);
    ~EncConfigSinZ();
    void createAttributeUi() Q_DECL_OVERRIDE;
    void updateAttributeUi() Q_DECL_OVERRIDE;
    bool execute() Q_DECL_OVERRIDE;
signals:

public slots:
private slots:
    void onSinNumChanged(int num);
private:
    QSpinBox* m_sinPerR;
};

#endif // ENCCONFIGSINZ_H
