// ModuleTemplate.h
#pragma once
#include <QString>
#include <QIcon>
#include <QWidget>
#include <QUuid>
#include <QObject>

class ModuleTemplate {
public:
    virtual ~ModuleTemplate() {}
    virtual QString name() const = 0;           // 顯示名稱
    virtual QUuid moduleUuid() const = 0;       // 唯一識別 UUID
    virtual int priority() const = 0;           // 優先級（越大越高）
    virtual QIcon icon() const = 0;             // 64x64 icon
    virtual QWidget* widget() = 0;              // 內容顯示用

    // 檔案操作介面 - Type 由函式自行判定
    virtual void OpenFile(const QString& content) = 0;
    virtual QString SaveFile() = 0;

    // 共享資源存取介面
    virtual void setContentAccess(QObject* contentWidget) = 0;    // 設定 Content 存取
    virtual void setChapterAccess(const QStringList& chapterIdx, const QStringList& chapterLabel) = 0; // 設定章節存取
};

#define ModuleInterface_iid "org.example.Monogatari.ModuleTemplate"
Q_DECLARE_INTERFACE(ModuleTemplate, ModuleInterface_iid)
