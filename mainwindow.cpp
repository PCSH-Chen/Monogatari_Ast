#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QDir>
#include <QPluginLoader>
#include <QDebug>
#include <QApplication>
#include <QStackedWidget>
#include <QUuid>
#include <QTimer>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , moduleStateTimer(new QTimer(this))
{
    ui->setupUi(this);

    // 儲存、搜尋按鈕預留（不連接功能）
    connect(ui->UndoBtn, &QToolButton::clicked, this, &MainWindow::onUndoClicked);
    connect(ui->RedoBtn, &QToolButton::clicked, this, &MainWindow::onRedoClicked);
    connect(ui->AllSelectBtn, &QToolButton::clicked, this, &MainWindow::onAllSelectClicked);
    connect(ui->CopyBtn, &QToolButton::clicked, this, &MainWindow::onCopyClicked);
    connect(ui->CutBtn, &QToolButton::clicked, this, &MainWindow::onCutClicked);
    connect(ui->PasteBtn, &QToolButton::clicked, this, &MainWindow::onPasteClicked);

    // 模組切換控制
    if (ui->SideBar) {
        connect(ui->SideBar, QOverload<int>::of(&QStackedWidget::currentChanged),
                this, &MainWindow::onModuleActivated);
    }
    
    // 設定模組狀態檢查定時器
    moduleStateTimer->setInterval(5000); // 每5秒檢查一次
    connect(moduleStateTimer, &QTimer::timeout, this, &MainWindow::checkModuleStates);
    
    // Content 變更信號
    if (ui->Content) {
        connect(ui->Content, &QPlainTextEdit::textChanged, this, &MainWindow::contentChanged);
    }

    // 預留 SaveBtn, FindBtn
    // connect(ui->SaveBtn, &QToolButton::clicked, ...);
    // connect(ui->FindBtn, &QToolButton::clicked, ...);

    // 設定 Content 共享
    setupContentSharing();
    
    // 載入模塊
    loadModules();
    
    // 開始模組狀態監控
    moduleStateTimer->start();
}

MainWindow::~MainWindow()
{
    moduleStateTimer->stop();
    unloadModules();
    delete ui;
}

void MainWindow::loadModules()
{
    // 獲取模塊目錄路徑
    QString modulesPath = QApplication::applicationDirPath() + "/Modules";
    QDir modulesDir(modulesPath);

    if (!modulesDir.exists()) {
        qDebug() << "Modules directory does not exist:" << modulesPath;
        return;
    }

    // 設定支援的模塊檔案過濾器
    QStringList filters;
    #ifdef Q_OS_WIN
        filters << "*.dll";
    #elif defined(Q_OS_MAC)
        filters << "*.dylib";
    #else
        filters << "*.so";
    #endif

    modulesDir.setNameFilters(filters);
    QStringList moduleFiles = modulesDir.entryList(QDir::Files);

    qDebug() << "Found" << moduleFiles.size() << "potential modules in" << modulesPath;

    // 載入每個模塊檔案
    for (const QString& fileName : moduleFiles) {
        QString filePath = modulesDir.absoluteFilePath(fileName);
        loadModule(filePath);
    }

    // 按優先級排序模塊
    sortModulesByPriority();

    // 將排序後的模塊新增到側邊欄
    addModulesToSideBar();

    qDebug() << "Successfully loaded" << moduleInfos.size() << "modules";
}

void MainWindow::loadModule(const QString& filePath)
{
    qDebug() << "Attempting to load module:" << filePath;

    QPluginLoader* loader = new QPluginLoader(filePath, this);

    if (!loader->load()) {
        qWarning() << "Failed to load plugin:" << filePath
                   << "Error:" << loader->errorString();
        delete loader;
        return;
    }

    QObject* pluginObject = loader->instance();
    if (!pluginObject) {
        qWarning() << "Failed to get plugin instance:" << filePath;
        loader->unload();
        delete loader;
        return;
    }

    ModuleTemplate* module = qobject_cast<ModuleTemplate*>(pluginObject);
    if (!module) {
        qWarning() << "Plugin does not implement ModuleTemplate interface:" << filePath;
        loader->unload();
        delete loader;
        return;
    }

    // 檢查 UUID 格式和衝突
    QUuid moduleUuid = module->moduleUuid();
    if (moduleUuid.isNull()) {
        qWarning() << "Module has invalid UUID:" << filePath;
        loader->unload();
        delete loader;
        return;
    }

    if (isModuleUuidExists(moduleUuid)) {
        qWarning() << "Module with same UUID already loaded:"
                   << moduleUuid.toString() << "from" << filePath;
        loader->unload();
        delete loader;
        return;
    }
    
    // 檢查版本相容性
    if (!module->isCompatible(QApplication::applicationVersion())) {
        qWarning() << "Module" << module->name() << "is not compatible with app version"
                   << QApplication::applicationVersion()
                   << "(module requires:" << module->minAppVersion() << ")";
        loader->unload();
        delete loader;
        return;
    }

    // 成功載入模塊，創建模塊資訊
    ModuleInfo moduleInfo(module, loader, filePath);
    moduleInfo.state = ModuleState::Loading;
    moduleInfos.append(moduleInfo);
    
    // 處理模組生命週期
    handleModuleLifecycle(moduleInfos.last());

    logModuleOperation("Load", module->name(), true);
    qDebug() << "Successfully loaded module:"
             << module->name()
             << "UUID:" << moduleUuid.toString()
             << "Priority:" << module->priority()
             << "Version:" << module->version();
    
    emit moduleLoaded(module->name());
}

bool MainWindow::isModuleUuidExists(const QUuid& uuid)
{
    for (const ModuleInfo& info : moduleInfos) {
        if (info.uuid == uuid) {
            return true;
        }
    }
    return false;
}

void MainWindow::sortModulesByPriority()
{
    // 使用 lambda 表達式按優先級降序排序（優先級越大越前面）
    std::sort(moduleInfos.begin(), moduleInfos.end(),
              [](const ModuleInfo& a, const ModuleInfo& b) {
                  // 首先按優先級排序（降序）
                  if (a.priority != b.priority) {
                      return a.priority > b.priority;
                  }
                  // 如果優先級相同，按名稱排序（升序）
                  return a.module->name() < b.module->name();
              });

    qDebug() << "Modules sorted by priority:";
    for (int i = 0; i < moduleInfos.size(); ++i) {
        const ModuleInfo& info = moduleInfos[i];
        qDebug() << QString("  %1. %2 (Priority: %3, UUID: %4)")
                        .arg(i + 1)
                        .arg(info.module->name())
                        .arg(info.priority)
                        .arg(info.uuid.toString());
    }
}

void MainWindow::addModulesToSideBar()
{
    if (!ui->SideBar) {
        qWarning() << "SideBar widget is null";
        return;
    }

    moduleIndexMap.clear();

    for (int i = 0; i < moduleInfos.size(); ++i) {
        ModuleInfo& info = moduleInfos[i];
        try {
            // 獲取模塊的 widget
            QWidget* moduleWidget = info.module->widget();
            if (!moduleWidget) {
                qWarning() << "Module" << info.module->name() << "returned null widget";
                info.state = ModuleState::Error;
                info.errorMessage = "Module returned null widget";
                continue;
            }

            // 設定 Content 存取權限
            info.module->setContentAccess(ui->Content);
            
            // 將模塊 widget 新增到 SideBar (QStackedWidget)
            int index = ui->SideBar->addWidget(moduleWidget);
            
            // 建立 UUID 到索引的映射
            moduleIndexMap[info.uuid] = index;
            
            // 更新模組狀態
            info.state = ModuleState::Loaded;
            info.module->setState(ModuleState::Loaded);

            logModuleOperation("AddToSideBar", info.module->name(), true);
            qDebug() << "Added module" << info.module->name()
                     << "to sidebar at index" << index
                     << "(Priority:" << info.priority << ")";

        } catch (const std::exception& e) {
            qWarning() << "Exception while adding module"
                       << info.module->name() << "to sidebar:" << e.what();
            info.state = ModuleState::Error;
            info.errorMessage = QString("Exception: %1").arg(e.what());
            emit moduleError(info.module->name(), info.errorMessage);
        } catch (...) {
            qWarning() << "Unknown exception while adding module"
                       << info.module->name() << "to sidebar";
            info.state = ModuleState::Error;
            info.errorMessage = "Unknown exception occurred";
            emit moduleError(info.module->name(), info.errorMessage);
        }
    }

    // 如果有模塊，設定第一個（最高優先級）為當前頁面
    if (!moduleInfos.isEmpty() && ui->SideBar->count() > 0) {
        ui->SideBar->setCurrentIndex(0);
        if (moduleInfos.first().state == ModuleState::Loaded) {
            moduleInfos.first().state = ModuleState::Active;
            moduleInfos.first().module->setState(ModuleState::Active);
            moduleInfos.first().module->activate();
            emit moduleActivated(moduleInfos.first().module->name());
        }
        qDebug() << "Set highest priority module as current:"
                 << moduleInfos.first().module->name();
    }
}

void MainWindow::unloadModules()
{
    // 停止模組狀態監控
    moduleStateTimer->stop();
    
    // 卸載所有模塊
    for (ModuleInfo& info : moduleInfos) {
        if (info.module) {
            // 處理模組生命週期
            if (info.state == ModuleState::Active) {
                info.module->deactivate();
            }
            info.module->cleanup();
            info.state = ModuleState::Unloading;
            info.module->setState(ModuleState::Unloading);
        }
        
        if (info.loader) {
            info.loader->unload();
            delete info.loader;
        }
        
        logModuleOperation("Unload", info.module ? info.module->name() : "Unknown", true);
        emit moduleUnloaded(info.module ? info.module->name() : "Unknown");
    }
    
    moduleInfos.clear();
    moduleIndexMap.clear();

    qDebug() << "All modules unloaded";
}

// 現有的槽函數保持不變
void MainWindow::onUndoClicked()
{
    if (ui->Content)
        ui->Content->undo();
}

void MainWindow::onRedoClicked()
{
    if (ui->Content)
        ui->Content->redo();
}

void MainWindow::onAllSelectClicked()
{
    if (ui->Content)
        ui->Content->selectAll();
}

void MainWindow::onCopyClicked()
{
    if (ui->Content)
        ui->Content->copy();
}

void MainWindow::onCutClicked()
{
    if (ui->Content)
        ui->Content->cut();
}

void MainWindow::onPasteClicked()
{
    if (ui->Content)
        ui->Content->paste();
}

// 新增模組管理槽函數
void MainWindow::onModuleActivated(int index)
{
    if (index < 0 || index >= moduleInfos.size()) {
        return;
    }

    // 停用當前活躍模組
    for (ModuleInfo& info : moduleInfos) {
        if (info.state == ModuleState::Active) {
            info.module->deactivate();
            info.state = ModuleState::Loaded;
            info.module->setState(ModuleState::Loaded);
        }
    }

    // 根據 SideBar 索引找到對應的模組
    QUuid targetUuid;
    for (auto it = moduleIndexMap.begin(); it != moduleIndexMap.end(); ++it) {
        if (it.value() == index) {
            targetUuid = it.key();
            break;
        }
    }

    if (targetUuid.isNull()) {
        qWarning() << "Cannot find module for sidebar index:" << index;
        return;
    }

    ModuleInfo* info = findModuleByUuid(targetUuid);
    if (info && info->module && info->state == ModuleState::Loaded) {
        info->module->activate();
        info->state = ModuleState::Active;
        info->module->setState(ModuleState::Active);
        emit moduleActivated(info->module->name());
        qDebug() << "Activated module:" << info->module->name();
    }
}

void MainWindow::onModuleError(const QString& moduleName, const QString& error)
{
    qWarning() << "Module error in" << moduleName << ":" << error;
    // 可以在這裡顯示錯誤對話框或狀態列消息
}

void MainWindow::checkModuleStates()
{
    for (ModuleInfo& info : moduleInfos) {
        if (info.module) {
            ModuleState currentState = info.module->state();
            if (currentState != info.state) {
                qDebug() << "Module" << info.module->name() 
                         << "state changed from" << static_cast<int>(info.state)
                         << "to" << static_cast<int>(currentState);
                info.state = currentState;
                
                if (currentState == ModuleState::Error) {
                    emit moduleError(info.module->name(), "Module reported error state");
                }
            }
        }
    }
}

// 新增模組管理輔助函數
void MainWindow::setupContentSharing()
{
    // 這個函數在模組載入之前被呼叫，目前只記錄日誌
    qDebug() << "Setting up Content sharing mechanism";
}

void MainWindow::updateModuleState(const QUuid& uuid, ModuleState newState)
{
    ModuleInfo* info = findModuleByUuid(uuid);
    if (info) {
        info->state = newState;
        if (info->module) {
            info->module->setState(newState);
        }
        qDebug() << "Updated module state for" << (info->module ? info->module->name() : "Unknown")
                 << "to" << static_cast<int>(newState);
    }
}

ModuleInfo* MainWindow::findModuleByUuid(const QUuid& uuid)
{
    for (ModuleInfo& info : moduleInfos) {
        if (info.uuid == uuid) {
            return &info;
        }
    }
    return nullptr;
}

void MainWindow::handleModuleLifecycle(ModuleInfo& info)
{
    if (!info.module) {
        return;
    }
    
    try {
        // 初始化模組
        if (info.module->initialize()) {
            info.state = ModuleState::Loaded;
            info.module->setState(ModuleState::Loaded);
            logModuleOperation("Initialize", info.module->name(), true);
        } else {
            info.state = ModuleState::Error;
            info.errorMessage = "Module initialization failed";
            info.module->setState(ModuleState::Error);
            logModuleOperation("Initialize", info.module->name(), false);
            emit moduleError(info.module->name(), info.errorMessage);
        }
    } catch (const std::exception& e) {
        info.state = ModuleState::Error;
        info.errorMessage = QString("Exception during initialization: %1").arg(e.what());
        if (info.module) {
            info.module->setState(ModuleState::Error);
        }
        logModuleOperation("Initialize", info.module->name(), false);
        emit moduleError(info.module->name(), info.errorMessage);
    }
}

void MainWindow::logModuleOperation(const QString& operation, const QString& moduleName, bool success)
{
    QString status = success ? "SUCCESS" : "FAILED";
    qDebug() << "[MODULE]" << operation << moduleName << "-" << status;
}
