#pragma once
#include <QString>
#include <QIcon>
#include <QWidget>

class ModuleTemplate {
public:
    virtual ~ModuleTemplate() = default;
    virtual QString name() const = 0;           // 顯示名稱
    virtual QString moduleUuid() const = 0;     // 唯一識別名稱
    virtual QIcon icon() const = 0;             // 64x64 圖示
    virtual QWidget* widget() = 0;              // 提供內容顯示的 QWidget

    // 只傳內容字串，型別由模組內自行判斷
    virtual void OpenFile(const QString& content) = 0;
    virtual QString SaveFile() = 0;
};

#define ModuleInterface_iid "org.example.Monogatari.ModuleTemplate"
Q_DECLARE_INTERFACE(ModuleTemplate, ModuleInterface_iid)
