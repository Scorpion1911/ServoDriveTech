#ifndef UIHOME_H
#define UIHOME_H

#include "uihome_global.h"
#include "iuiwidget.h"

namespace Ui {
class UiHome;
}

class QWidget;
class UiHomePrivate;

class UIHOMESHARED_EXPORT UiHome : public IUiWidget
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(UiHome)

public:
    explicit UiHome(QWidget *parent = 0);
    ~UiHome();

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
    Ui::UiHome *ui;
};

#endif // UIHOME_H
