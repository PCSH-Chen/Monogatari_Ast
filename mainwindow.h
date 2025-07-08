#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QPluginLoader>
#include <QUuid>
#include <QMap>
#include <QTimer>
#include "ModuleTemplate.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct ModuleInfo {
    ModuleTemplate* module;
    QPluginLoader* loader;
    int priority;
    QUuid uuid;
    ModuleState state;
    QString filePath;
    QString errorMessage;

    ModuleInfo(ModuleTemplate* mod, QPluginLoader* load, const QString& path = "")
        : module(mod), loader(load), state(ModuleState::Unloaded), filePath(path) {
        if (module) {
            priority = module->priority();
            uuid = module->moduleUuid();
            state = module->state();
        }
    }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // 現有編輯功能槽
    void onUndoClicked();
    void onRedoClicked();
    void onAllSelectClicked();
    void onCopyClicked();
    void onCutClicked();
    void onPasteClicked();
    
    // 新增模組管理槽
    void onModuleActivated(int index);
    void onModuleError(const QString& moduleName, const QString& error);
    void checkModuleStates();

signals:
    // 模組管理相關信號
    void moduleLoaded(const QString& moduleName);
    void moduleUnloaded(const QString& moduleName);
    void moduleActivated(const QString& moduleName);
    void moduleError(const QString& moduleName, const QString& error);
    void contentChanged();

private:
    // 模塊相關函式
    void loadModules();
    void loadModule(const QString& filePath);
    void unloadModules();
    void sortModulesByPriority();
    void addModulesToSideBar();
    bool isModuleUuidExists(const QUuid& uuid);
    
    // 新增模組管理函式
    void setupContentSharing();
    void updateModuleState(const QUuid& uuid, ModuleState newState);
    ModuleInfo* findModuleByUuid(const QUuid& uuid);
    void handleModuleLifecycle(ModuleInfo& info);
    void logModuleOperation(const QString& operation, const QString& moduleName, bool success = true);

    // 成員變數
    Ui::MainWindow *ui;
    QList<ModuleInfo> moduleInfos;  // 模塊資訊列表（包含優先級和狀態）
    QTimer* moduleStateTimer;       // 定期檢查模組狀態
    QMap<QUuid, int> moduleIndexMap; // UUID 到 SideBar 索引的映射
};

#endif // MAINWINDOW_H
