// ModuleTemplate.h
#pragma once
#include <QString>
#include <QIcon>
#include <QWidget>
#include <QUuid>
#include <QPair>
#include <QPlainTextEdit>

// 前向聲明
class MainWindow;

class ModuleTemplate {
public:
    virtual ~ModuleTemplate() {}
    virtual QString name() const = 0;           // 顯示名稱
    virtual QUuid moduleUuid() const = 0;       // 唯一識別 UUID
    virtual int priority() const = 0;           // 優先級（越大越高）
    virtual QIcon icon() const = 0;             // 64x64 icon
    virtual QWidget* widget() = 0;              // 內容顯示用

    // Content 存取權限設定
    virtual void setContentAccess(QPlainTextEdit* content) = 0;

    // Chapter 存取權限設定
    virtual void setChapterAccess(MainWindow* mainWindow) = 0;

    // 檔案操作函式（Type由函式自行判定）
    virtual void OpenFile(const QString& content) = 0;
    virtual QString SaveFile() = 0;
};

#define ModuleInterface_iid "org.example.Monogatari.ModuleTemplate"
Q_DECLARE_INTERFACE(ModuleTemplate, ModuleInterface_iid)
