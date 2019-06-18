#ifndef RECOVERDIALOG_H
#define RECOVERDIALOG_H

#include <QDialog>

class QCheckBox;
class SevDevice;

namespace Ui {
class RecoverDialog;
}

class RecoverDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RecoverDialog(QWidget *parent = 0);
    ~RecoverDialog();

    void uiInit(const QList<SevDevice *> &devList, QList<int> &indexList);
private slots:
    void onSingleBoxClicked(bool checked);
    void onOkButtonClicked();
    void onAllBoxClicked(bool checked);
private:
    Ui::RecoverDialog *ui;

    int m_devNum;
    QList<int> *m_indexList;
    QList<QCheckBox*> m_boxList;
};

#endif // RECOVERDIALOG_H
