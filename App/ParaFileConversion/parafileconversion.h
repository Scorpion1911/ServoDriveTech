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
//    bool findAxisParameter(QTreeWidget *treeWidget);
    void printStructMemberToText(QTreeWidgetItem *treeWidgetItem, QTextStream &out);
    void setEnabledExprotPushBtn();
    void findOutIncreaseParameter(QTreeWidgetItem *treeWidgetItem);

private slots:
    void onPushBtnOpenFileClicked();
//    void onComboBoxIndexChanged(int index);
    void onPushBtnExprotFileClicked();
    void onCheckBoxUpgradeToggled(bool checked);
    void onPushBtnOpenFileUpgradeClicked();

private:
    Ui::ParaFileConversion *ui;
    QTreeWidgetItem *m_treeWidgetItem;
//    QStringList m_axisList;
    QString m_savePath;
    quint32 m_memberSerialNum;
    QStringList m_memberType;
    QStringList m_oldParaFileAddr;
    bool m_xmlPrepare;
    bool m_headPrepare;
    QTreeWidgetItem m_increaseItem;
    QString m_headFilePath;
    QString m_xmlVersion;
    QString m_headFileUpPath;
    QString m_headFileNameCom;

//    quint32 m_oldParaNum;
//    quint8 m_axisNum;
//    bool m_isParaFile;
};

#endif // PARAFILECONVERSION_H
