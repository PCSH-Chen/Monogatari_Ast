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
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_chapterIdx(0)
    , m_chapterLabel("未命名章節")
{
    ui->setupUi(this);

    // 儲存、搜尋按鈕預留（不連接功能）
    connect(ui->UndoBtn, &QToolButton::clicked, this, &MainWindow::onUndoClicked);
    connect(ui->RedoBtn, &QToolButton::clicked, this, &MainWindow::onRedoClicked);
    connect(ui->AllSelectBtn, &QToolButton::clicked, this, &MainWindow::onAllSelectClicked);
    connect(ui->CopyBtn, &QToolButton::clicked, this, &MainWindow::onCopyClicked);
    connect(ui->CutBtn, &QToolButton::clicked, this, &MainWindow::onCutClicked);
    connect(ui->PasteBtn, &QToolButton::clicked, this, &MainWindow::onPasteClicked);

    // 預留 SaveBtn, FindBtn
    // connect(ui->SaveBtn, &QToolButton::clicked, ...);
    // connect(ui->FindBtn, &QToolButton::clicked, ...);

    // 載入模塊
    loadModules();
}

MainWindow::~MainWindow()
{
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

    // 成功載入模塊，創建模塊資訊
    ModuleInfo moduleInfo(module, loader);
    moduleInfos.append(moduleInfo);

    qDebug() << "Successfully loaded module:"
             << module->name()
             << "UUID:" << moduleUuid.toString()
             << "Priority:" << module->priority();
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
        return;
    }

    for (const ModuleInfo& info : moduleInfos) {
        try {
            // 設定 Chapter 存取權限
            info.module->setChapterAccess(
                [this]() { return getChapterIdx(); },
                [this](int idx) { setChapterIdx(idx); },
                [this]() { return getChapterLabel(); },
                [this](const QString& label) { setChapterLabel(label); }
            );

            // 獲取模塊的 widget
            QWidget* moduleWidget = info.module->widget();
            if (!moduleWidget) {
                qWarning() << "Module" << info.module->name() << "returned null widget";
                continue;
            }

            // 將模塊 widget 新增到 SideBar (QStackedWidget)
            int index = ui->SideBar->addWidget(moduleWidget);

            qDebug() << "Added module" << info.module->name()
                     << "to sidebar at index" << index
                     << "(Priority:" << info.priority << ")";

        } catch (const std::exception& e) {
            qWarning() << "Exception while adding module"
                       << info.module->name() << "to sidebar:" << e.what();
        } catch (...) {
            qWarning() << "Unknown exception while adding module"
                       << info.module->name() << "to sidebar";
        }
    }

    // 如果有模塊，設定第一個（最高優先級）為當前頁面
    if (!moduleInfos.isEmpty() && ui->SideBar->count() > 0) {
        ui->SideBar->setCurrentIndex(0);
        qDebug() << "Set highest priority module as current:"
                 << moduleInfos.first().module->name();
    }
}

void MainWindow::unloadModules()
{
    // 卸載所有模塊
    for (const ModuleInfo& info : moduleInfos) {
        if (info.loader) {
            info.loader->unload();
            delete info.loader;
        }
    }
    moduleInfos.clear();

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

// Chapter 相關方法實作
int MainWindow::getChapterIdx() const
{
    return m_chapterIdx;
}

void MainWindow::setChapterIdx(int idx)
{
    if (m_chapterIdx != idx) {
        m_chapterIdx = idx;
        emit chapterIdxChanged(idx);
    }
}

QString MainWindow::getChapterLabel() const
{
    return m_chapterLabel;
}

void MainWindow::setChapterLabel(const QString& label)
{
    if (m_chapterLabel != label) {
        m_chapterLabel = label;
        emit chapterLabelChanged(label);
    }
}
