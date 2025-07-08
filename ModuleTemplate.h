// ModuleTemplate.h
#pragma once
#include <QString>
#include <QIcon>
#include <QWidget>
#include <QUuid>
#include <QObject>

// 模組狀態枚舉
enum class ModuleState {
    Unloaded,
    Loading,
    Loaded,
    Active,
    Error,
    Unloading
};

class ModuleTemplate {
public:
    virtual ~ModuleTemplate() {}
    
    // 基本資訊
    virtual QString name() const = 0;           // 顯示名稱
    virtual QUuid moduleUuid() const = 0;       // 唯一識別 UUID
    virtual int priority() const = 0;           // 優先級（越大越高）
    virtual QIcon icon() const = 0;             // 64x64 icon
    virtual QWidget* widget() = 0;              // 內容顯示用
    
    // 版本和相容性
    virtual QString version() const { return "1.0.0"; }
    virtual QString minAppVersion() const { return "1.0.0"; }
    virtual bool isCompatible(const QString& appVersion) const { 
        Q_UNUSED(appVersion); 
        return true; 
    }
    
    // Content 共享機制
    virtual void setContentAccess(QObject* contentWidget) {
        Q_UNUSED(contentWidget);
        // 預設實作：不做任何事
    }
    
    // 模組狀態管理
    virtual ModuleState state() const { return ModuleState::Loaded; }
    virtual void setState(ModuleState newState) { Q_UNUSED(newState); }
    
    // 模組生命週期
    virtual bool initialize() { return true; }
    virtual bool activate() { return true; }
    virtual bool deactivate() { return true; }
    virtual void cleanup() {}
    
    // 統一檔案操作介面
    virtual void OpenFile(const QString& content, const QString& type) = 0;
    virtual QString SaveFile(const QString& content, const QString& type) = 0;
};

#define ModuleInterface_iid "org.example.Monogatari.ModuleTemplate"
Q_DECLARE_INTERFACE(ModuleTemplate, ModuleInterface_iid)
