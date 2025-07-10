#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QPluginLoader>
#include <QUuid>
#include <QMap>
#include <QPair>
#include <QPlainTextEdit>
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

    // 存取權限介面（新增）
    QPlainTextEdit* getContentEditor() const;
    void setContentAccess(QPlainTextEdit* content);
    
    // 章節管理介面（新增）
    using ChapterIdx = QPair<int, int>;
    QList<ChapterIdx> getChapterList() const;
    QString getChapterLabel(const ChapterIdx& idx) const;
    void setCurrentChapter(const ChapterIdx& idx);
    ChapterIdx getCurrentChapter() const;

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
    
    // 存取權限設定（新增）
    void setupModuleAccess(ModuleTemplate* module);

    // 成員變數
    Ui::MainWindow *ui;
    QList<ModuleInfo> moduleInfos;  // 模塊資訊列表（包含優先級）
    
    // 章節管理（新增）
    QList<ChapterIdx> m_chapterList;
    QMap<ChapterIdx, QString> m_chapterLabels;
    ChapterIdx m_currentChapter;
};

#endif // MAINWINDOW_H
