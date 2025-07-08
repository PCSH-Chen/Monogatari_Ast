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

class ModuleTemplate {
public:
    virtual ~ModuleTemplate() = default;
    virtual QString name() const = 0;           // 顯示名稱
    virtual QString moduleUuid() const = 0;     // 唯一識別名稱
    virtual QIcon icon() const = 0;             // 64x64 圖示
    virtual QWidget* widget() = 0;              // 提供內容顯示的 QWidget

    // 若模組需處理檔案內容，需實作以下方法
    virtual void OpenFile(const QString& content, const QString& type) = 0;
    virtual QString SaveFile(const QString& content, const QString& type) = 0;
};

#define ModuleInterface_iid "org.example.Monogatari.ModuleTemplate"
Q_DECLARE_INTERFACE(ModuleTemplate, ModuleInterface_iid)
```

---

## 成員說明

* `QString name() const`：
  回傳模組在 UI 中顯示的名稱。

* `QString moduleUuid() const`：
  回傳模組的唯一識別UUID。

* `QIcon icon() const`：
  回傳 64x64 的圖示，供模組圖示顯示使用。

* `QWidget* widget()`：
  提供模組的主要畫面元件（會嵌入於主視窗中）。

* `void OpenFile(const QString& content, const QString& type)`：
  可選實作。由主程式呼叫以提供載入的檔案內容及格式。

* `QString SaveFile(const QString& content, const QString& type)`：
  可選實作。由主程式呼叫以取得儲存後的內容資料。

---

## 實作提示
* 請以 `QObject` 派生類別包裝您的模組，以支援 Qt Meta-Object System。
* 若使用 Qt Designer，建議將 `QWidget* widget()` 回傳為自訂的 UI 表單類別。
* 請確保您的模組使用唯一 `moduleName`、`Type`，避免與其他模組衝突。
---

## 模組註冊

模組須透過 Qt 插件系統進行註冊。
範例如下：
```cpp
Q_PLUGIN_METADATA(IID ModuleInterface_iid)
Q_INTERFACES(ModuleTemplate)
```
---
