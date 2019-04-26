#ifndef CONVERTXML_H
#define CONVERTXML_H

#include <QWidget>

namespace Ui {
class ConvertXml;
}

class ConvertXml : public QWidget
{
    Q_OBJECT

public:
    explicit ConvertXml(QWidget *parent = 0);
    ~ConvertXml();
private slots:
    void onConvertBtnClicked();

private:
    Ui::ConvertXml *ui;
    QString m_openFilePath;
};

#endif // CONVERTXML_H
