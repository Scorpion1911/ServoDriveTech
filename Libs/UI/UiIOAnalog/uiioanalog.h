#ifndef UIIOANALOG_H
#define UIIOANALOG_H

#include "uiioanalog_global.h"
#include "iuiwidget.h"

namespace Ui {
class UiIOAnalog;
}

class QWidget;
class UiIOAnalogPrivate;

class UIIOANALOGSHARED_EXPORT UiIOAnalog : public IUiWidget
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(UiIOAnalog)
public:
    explicit UiIOAnalog(QWidget *parent = 0);
    ~UiIOAnalog();

    void accept(QWidget*w) Q_DECL_OVERRIDE;
    void setUiActive(bool actived)Q_DECL_OVERRIDE;
    bool writePageFLASH()Q_DECL_OVERRIDE;
    bool writePageFlashToOtherAxis(int srcAxisInx, int desAxisInx, QTreeWidget *tree) Q_DECL_OVERRIDE;

    bool hasConfigFunc()Q_DECL_OVERRIDE;
    bool hasSaveFunc()Q_DECL_OVERRIDE;

protected:
    void setContextAction() Q_DECL_OVERRIDE;

private:
    QStackedWidget *getUiStackedWidget(void)Q_DECL_OVERRIDE;
    QVBoxLayout *getVBoxLayout(void)Q_DECL_OVERRIDE;
    void setDefaultUi()Q_DECL_OVERRIDE;
private:
    Ui::UiIOAnalog *ui;
};

#endif // UIIOANALOG_H
