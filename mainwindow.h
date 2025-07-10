#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QPluginLoader>
#include <QUuid>
#include <QMap>
#include "ModuleTemplate.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct ModuleInfo {
    ModuleTemplate* module;
    QPluginLoader* loader;
    int priority;
    QUuid uuid;

    ModuleInfo(ModuleTemplate* mod, QPluginLoader* load)
        : module(mod), loader(load) {
        if (module) {
            priority = module->priority();
            uuid = module->moduleUuid();
        }
    }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Chapter 相關的存取方法
    int getChapterIdx() const;
    void setChapterIdx(int idx);
    QString getChapterLabel() const;
    void setChapterLabel(const QString& label);

signals:
    void chapterIdxChanged(int idx);
    void chapterLabelChanged(const QString& label);

private slots:
    void onUndoClicked();
    void onRedoClicked();
    void onAllSelectClicked();
    void onCopyClicked();
    void onCutClicked();
    void onPasteClicked();

private:
    // 模塊相關函式
    void loadModules();
    void loadModule(const QString& filePath);
    void unloadModules();
    void sortModulesByPriority();
    void addModulesToSideBar();
    bool isModuleUuidExists(const QUuid& uuid);

    // 成員變數
    Ui::MainWindow *ui;
    QList<ModuleInfo> moduleInfos;  // 模塊資訊列表（包含優先級）
    
    // Chapter 相關變數
    int m_chapterIdx;
    QString m_chapterLabel;
};

#endif // MAINWINDOW_H
