#ifndef APPFILLMAXRANGETEST_H
#define APPFILLMAXRANGETEST_H

#include <QDialog>

class QTreeWidget;

namespace Ui {
class AppFillMaxRangeTest;
}

class AppFillMaxRangeTest : public QDialog
{
    Q_OBJECT

public:
    explicit AppFillMaxRangeTest(QWidget *parent = 0);
    ~AppFillMaxRangeTest();

private:
    Ui::AppFillMaxRangeTest *ui;
    QString m_filePath;
    QTreeWidget *m_tree;
private slots:
    void onActionToolBtnClicked();
    void onActionFillBtnClicked();
    void onActionSaveBtnClicked();
};

#endif // APPFILLMAXRANGETEST_H
