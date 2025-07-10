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
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
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
    connect(ui->SaveBtn, &QToolButton::clicked, this, &MainWindow::onSaveFile);
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
            // 獲取模塊的 widget
            QWidget* moduleWidget = info.module->widget();
            if (!moduleWidget) {
                qWarning() << "Module" << info.module->name() << "returned null widget";
                continue;
            }

            // 將模塊 widget 新增到 SideBar (QStackedWidget)
            int index = ui->SideBar->addWidget(moduleWidget);

            // 設定共享資源存取
            if (ui->Content) {
                info.module->setContentAccess(ui->Content);
                qDebug() << "Set content access for module" << info.module->name();
            }

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

    // 更新章節存取資訊
    updateChapterAccess();
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

void MainWindow::onOpenFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, 
        tr("開啟檔案"), 
        "",
        tr("物語檔案 (*.mgf);;所有檔案 (*.*)"));
    
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            QString content = in.readAll();
            openFileToModules(content);
            
            // 同時載入到主編輯器
            if (ui->Content) {
                ui->Content->setPlainText(content);
            }
            
            qDebug() << "Opened file:" << fileName;
        } else {
            QMessageBox::warning(this, tr("錯誤"), tr("無法開啟檔案：") + fileName);
        }
    }
}

void MainWindow::onSaveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("儲存檔案"),
        "",
        tr("物語檔案 (*.mgf);;所有檔案 (*.*)"));
    
    if (!fileName.isEmpty()) {
        QString content = saveFileFromModules();
        
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << content;
            qDebug() << "Saved file:" << fileName;
        } else {
            QMessageBox::warning(this, tr("錯誤"), tr("無法儲存檔案：") + fileName);
        }
    }
}

void MainWindow::openFileToModules(const QString& content)
{
    for (const ModuleInfo& info : moduleInfos) {
        try {
            info.module->OpenFile(content);
            qDebug() << "Opened file content to module:" << info.module->name();
        } catch (const std::exception& e) {
            qWarning() << "Exception while opening file to module" 
                       << info.module->name() << ":" << e.what();
        } catch (...) {
            qWarning() << "Unknown exception while opening file to module" 
                       << info.module->name();
        }
    }
}

QString MainWindow::saveFileFromModules()
{
    QString result = "<Document>\n";
    
    // 儲存主要內容
    if (ui->Content) {
        result += "    <MainContent>\n";
        result += "        " + ui->Content->toPlainText().replace("\n", "\n        ") + "\n";
        result += "    </MainContent>\n";
    }
    
    // 儲存各模組資料
    for (const ModuleInfo& info : moduleInfos) {
        try {
            QString moduleData = info.module->SaveFile();
            if (!moduleData.isEmpty()) {
                result += "    <Module name=\"" + info.module->name() + "\" uuid=\"" + info.uuid.toString() + "\">\n";
                result += "        " + moduleData.replace("\n", "\n        ") + "\n";
                result += "    </Module>\n";
                qDebug() << "Saved data from module:" << info.module->name();
            }
        } catch (const std::exception& e) {
            qWarning() << "Exception while saving from module" 
                       << info.module->name() << ":" << e.what();
        } catch (...) {
            qWarning() << "Unknown exception while saving from module" 
                       << info.module->name();
        }
    }
    
    result += "</Document>";
    return result;
}

void MainWindow::updateChapterAccess()
{
    // 這裡可以實作從文件內容中自動解析章節的邏輯
    // 目前提供示例資料
    QStringList chapterIdx;
    QStringList chapterLabel;
    
    if (ui->Content) {
        QString content = ui->Content->toPlainText();
        // 簡單的章節檢測邏輯（可以根據實際需求改進）
        QStringList lines = content.split('\n');
        int chapterCount = 0;
        
        for (const QString& line : lines) {
            if (line.contains("第") && line.contains("章")) {
                chapterCount++;
                chapterIdx.append(QString::number(chapterCount));
                chapterLabel.append(line.trimmed());
            }
        }
    }
    
    // 如果沒有檢測到章節，提供預設值
    if (chapterIdx.isEmpty()) {
        chapterIdx = {"1", "2", "3"};
        chapterLabel = {"第一章 開始", "第二章 冒險", "第三章 結局"};
    }
    
    // 更新所有模組的章節存取
    for (const ModuleInfo& info : moduleInfos) {
        try {
            info.module->setChapterAccess(chapterIdx, chapterLabel);
        } catch (const std::exception& e) {
            qWarning() << "Exception while updating chapter access for module"
                       << info.module->name() << ":" << e.what();
        } catch (...) {
            qWarning() << "Unknown exception while updating chapter access for module"
                       << info.module->name();
        }
    }
}
