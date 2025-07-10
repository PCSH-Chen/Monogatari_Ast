#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QPluginLoader>
#include <QUuid>
#include <QMap>
#include <QPair>
#include "ModuleTemplate.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class QPlainTextEdit;
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

    // Chapter 存取方法（公開給模組使用）
    QPair<int, int> getChapterIdx() const { return ChapterIdx; }
    QString getChapterLabel() const { return ChapterLabel; }
    void setChapterIdx(const QPair<int, int>& idx);
    void setChapterLabel(const QString& label);

signals:
    // Chapter 變更訊號
    void chapterIdxChanged(const QPair<int, int>& newIdx);
    void chapterLabelChanged(const QString& newLabel);
    void chapterChanged(const QPair<int, int>& idx, const QString& label);

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
    QPair<int, int> ChapterIdx;     // 章節索引 (章節編號, 子章節編號)
    QString ChapterLabel;           // 章節標籤
};

#endif // MAINWINDOW_H
