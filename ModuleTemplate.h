// ModuleTemplate.h
#pragma once
#include <QString>
#include <QIcon>
#include <QWidget>

class ModuleTemplate {
public:
    virtual ~ModuleTemplate() {}
    virtual QString name() const = 0;           // 顯示名稱
    virtual QString moduleName() const = 0;     // 唯一識別
    virtual QIcon icon() const = 0;             // 64x64 icon
    virtual QWidget* widget() = 0;              // 內容顯示用
    // 如需支援文件操作，下列為純虛函式定義
    virtual void OpenFile(const QString& content, const QString& type) = 0;
    virtual QString SaveFile(const QString& content, const QString& type) = 0;
};
#define ModuleInterface_iid "org.example.Monogatari.ModuleTemplate"
Q_DECLARE_INTERFACE(ModuleTemplate, ModuleInterface_iid)
