#ifndef PARAFILECONVERSION_H
#define PARAFILECONVERSION_H

#include <QMainWindow>
#include <qglobal.h>
#include <QTreeWidget>
#include <QStringList>
#include <QTreeWidgetItem>

namespace Ui {
class ParaFileConversion;
}

class ParaFileConversion : public QMainWindow
{
    Q_OBJECT

public:
    explicit ParaFileConversion(QWidget *parent = 0);
    ~ParaFileConversion();

signals:

protected:
    void init();
    void printStructMemberToText(QTreeWidgetItem *treeWidgetItem, QTextStream &out);
    void setEnabledExprotPushBtn();
    void findOutIncreaseParameter(QTreeWidgetItem *treeWidgetItem);
    void addUpdateInfo(QTreeWidgetItem *treeWidgetItem);
    void addParameterToParaItem(QTreeWidgetItem *treeWidgetItem, QTreeWidgetItem *paraItem);

private slots:
    void onPushBtnOpenFileClicked();
    void onPushBtnExprotFileClicked();
    void onCheckBoxUpgradeToggled(bool checked);
    void onCheckBoxOutputHeadFileToggled(bool checked);
    void onPushBtnOpenFileUpgradeClicked();

private:
    Ui::ParaFileConversion *ui;
    QTreeWidgetItem *m_treeWidgetItem;
    QString m_savePath;
    quint32 m_memberSerialNum;
    quint32 m_insertNum;
    quint32 m_paraSum;
    QStringList m_memberType;
    QStringList m_oldParaFileAddr;
    bool m_openFilePrepare;
    bool m_upgradeFilePrepare;
    QTreeWidgetItem m_increaseItem;
    QString m_upgradeFilePath;
    QString m_xmlVersion;
    QString m_upgradeFileUpPath;
    QString m_upgradeFileNameCom;
    QTreeWidget *m_saveTreeWidget;
    quint8 m_upgradeFileType;
};

#endif // PARAFILECONVERSION_H
