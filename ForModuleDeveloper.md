# 物語アシスタント｜模組開發文件

本文件說明如何撰寫一個可整合於物語アシスタント框架的模組（Module）。模組需遵循以下介面規範，確保其可以與主應用程式動態載入與互動。

---

## 目標

本開發介面旨在提供模組化擴充能力，使開發者可獨立構建功能性元件，並於執行期間整合至物語アシスタント的使用者介面中。

---

## 模組介面說明

所有模組皆需實作以下類別：

```cpp
// ModuleTemplate.h
#pragma once
#include <QString>
#include <QIcon>
#include <QWidget>
#include <QUuid>
#include <QObject>

// 模組狀態枚舉
enum class ModuleState {
    Unloaded,    // 未載入
    Loading,     // 載入中
    Loaded,      // 已載入
    Active,      // 活躍中
    Error,       // 錯誤狀態
    Unloading    // 卸載中
};

class ModuleTemplate {
public:
    virtual ~ModuleTemplate() {}
    
    // === 必須實作的純虛函式 ===
    virtual QString name() const = 0;           // 顯示名稱
    virtual QUuid moduleUuid() const = 0;       // 唯一識別 UUID
    virtual int priority() const = 0;           // 優先級（越大越高）
    virtual QIcon icon() const = 0;             // 64x64 icon
    virtual QWidget* widget() = 0;              // 內容顯示用
    virtual void OpenFile(const QString& content, const QString& type) = 0;
    virtual QString SaveFile(const QString& content, const QString& type) = 0;
    
    // === 可選實作的虛函式（已有預設實作） ===
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
};

#define ModuleInterface_iid "org.example.Monogatari.ModuleTemplate"
Q_DECLARE_INTERFACE(ModuleTemplate, ModuleInterface_iid)
```

---

## 成員說明

### 必須實作的純虛函式

* `QString name() const`：
  回傳模組在 UI 中顯示的名稱。

* `QUuid moduleUuid() const`：
  回傳模組的唯一識別 UUID。請使用固定的 UUID 以確保模組身份一致性。

* `int priority() const`：
  回傳模組的優先級（數值越大優先級越高）。決定模組在側邊欄中的顯示順序。

* `QIcon icon() const`：
  回傳 64x64 的圖示，供模組圖示顯示使用。

* `QWidget* widget()`：
  提供模組的主要畫面元件（會嵌入於主視窗中）。

* `void OpenFile(const QString& content, const QString& type)`：
  處理檔案載入。`content` 為檔案內容，`type` 為檔案類型。

* `QString SaveFile(const QString& content, const QString& type)`：
  處理檔案儲存。`content` 為當前內容，`type` 為儲存類型，回傳序列化後的內容。

### 可選實作的虛函式

* `QString version() const`：
  回傳模組版本，預設為 "1.0.0"。

* `QString minAppVersion() const`：
  回傳模組所需的最低應用程式版本，預設為 "1.0.0"。

* `bool isCompatible(const QString& appVersion) const`：
  檢查模組是否與指定應用程式版本相容，預設返回 true。

* `void setContentAccess(QObject* contentWidget)`：
  設定 Content 共享存取權限。由主程式呼叫，傳入主編輯區域的 widget。

* `ModuleState state() const` / `void setState(ModuleState newState)`：
  取得/設定模組狀態。

* `bool initialize()` / `bool activate()` / `bool deactivate()` / `void cleanup()`：
  模組生命週期管理函式。分別處理初始化、啟用、停用和清理。

---

## Content 共享機制

新的 `setContentAccess()` 方法提供了與主編輯區域的直接存取能力：

```cpp
void MyModule::setContentAccess(QObject* contentWidget) {
    QPlainTextEdit* textEdit = qobject_cast<QPlainTextEdit*>(contentWidget);
    if (textEdit) {
        // 連接信號以監聽內容變化
        connect(textEdit, &QPlainTextEdit::textChanged, 
                this, &MyModule::onContentChanged);
    }
}
```

---

## 實作提示

* 請以 `QObject` 派生類別包裝您的模組，以支援 Qt Meta-Object System。
* 若使用 Qt Designer，建議將 `QWidget* widget()` 回傳為自訂的 UI 表單類別。
* 請確保您的模組使用唯一 UUID，避免與其他模組衝突。
* 實作生命週期方法以確保模組能正確載入和卸載。
* 透過 `setContentAccess()` 與主程式的編輯區域互動。

---

## 模組註冊

模組須透過 Qt 插件系統進行註冊。
範例如下：
```cpp
Q_PLUGIN_METADATA(IID ModuleInterface_iid)
Q_INTERFACES(ModuleTemplate)
```

---

## 完整實作範例

以下是一個完整的模組實作範例：

```cpp
// MyModule.h
#include <QWidget>
#include "ModuleTemplate.h"

class MyModule : public QWidget, public ModuleTemplate {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID ModuleInterface_iid)
    Q_INTERFACES(ModuleTemplate)

public:
    explicit MyModule(QWidget *parent = nullptr);
    ~MyModule() override;

    // ModuleTemplate 必須實作
    QString name() const override { return "我的模組"; }
    QUuid moduleUuid() const override { 
        return QUuid("{12345678-abcd-1234-ef00-123456789abc}"); 
    }
    int priority() const override { return 50; }
    QIcon icon() const override { return QIcon(":/my_icon.png"); }
    QWidget* widget() override { return this; }
    void OpenFile(const QString& content, const QString& type) override;
    QString SaveFile(const QString& content, const QString& type) override;
    
    // 可選實作
    void setContentAccess(QObject* contentWidget) override;
    bool initialize() override;
    
private slots:
    void onContentChanged();
    
private:
    QPlainTextEdit* m_contentAccess;
};

// MyModule.cpp
MyModule::MyModule(QWidget *parent) : QWidget(parent), m_contentAccess(nullptr) {
    // 初始化 UI
}

void MyModule::setContentAccess(QObject* contentWidget) {
    m_contentAccess = qobject_cast<QPlainTextEdit*>(contentWidget);
    if (m_contentAccess) {
        connect(m_contentAccess, &QPlainTextEdit::textChanged,
                this, &MyModule::onContentChanged);
    }
}

bool MyModule::initialize() {
    // 執行初始化邏輯
    return true;
}

void MyModule::onContentChanged() {
    if (m_contentAccess) {
        QString text = m_contentAccess->toPlainText();
        // 處理內容變化
    }
}

void MyModule::OpenFile(const QString& content, const QString& type) {
    // 處理檔案載入
}

QString MyModule::SaveFile(const QString& content, const QString& type) {
    // 處理檔案儲存
    return content;
}
```

---
