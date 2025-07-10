# 物語アシスタント｜模組開發文件

本文件說明如何撰寫一個可整合於物語アシスタント框架的模組（Module）。模組需遵循以下介面規範，確保其可以與主應用程式動態載入與互動。

---

## 目標

本開發介面旨在提供模組化擴充能力，使開發者可獨立構建功能性元件，並於執行期間整合至物語アシスタント的使用者介面中。同時提供共享資源存取機制，讓模組間可以協作處理內容與章節資訊。

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
```

---

## 核心方法說明

### 基本資訊方法

* **`QString name() const`**：
  回傳模組在 UI 中顯示的名稱。

* **`QUuid moduleUuid() const`**：
  回傳模組的唯一識別UUID。**必須是有效的UUID格式，且與其他模組不重複**。
  建議使用線上 UUID 產生器或程式碼產生唯一值。

* **`int priority() const`**：
  回傳模組的優先級數值。數值越大，優先級越高，在側邊欄中排序越前面。
  建議優先級範圍：
  - 100+ : 核心功能模組（如基本資訊、文字編輯）
  - 50-99 : 輔助功能模組（如語法檢查、統計分析）
  - 1-49 : 擴充功能模組（如匯出工具、外部整合）

* **`QIcon icon() const`**：
  回傳 64x64 的圖示，供模組圖示顯示使用。

* **`QWidget* widget()`**：
  提供模組的主要畫面元件（會嵌入於主視窗的側邊欄中）。

### 檔案操作方法

* **`void OpenFile(const QString& content)`**：
  當主程式載入檔案時呼叫，提供檔案內容給模組處理。
  **Type 由函式自行判定** - 模組需要自行分析 content 內容格式並決定如何處理。

* **`QString SaveFile()`**：
  當主程式儲存檔案時呼叫，模組需要回傳要儲存的資料內容。
  **Type 由函式自行判定** - 模組自行決定輸出的資料格式。

### 共享資源存取方法

* **`void setContentAccess(QObject* contentWidget)`**：
  主程式會傳入主要內容編輯器的參考，讓模組可以監聽內容變化或進行操作。
  通常 contentWidget 是 QPlainTextEdit 類型。

* **`void setChapterAccess(const QStringList& chapterIdx, const QStringList& chapterLabel)`**：
  主程式會傳入章節索引和標籤列表，讓模組可以存取章節資訊進行分析或顯示。

---

## 共享資源機制

### Content 存取機制

透過 `setContentAccess()` 方法，模組可以：

1. **監聽內容變化**：
```cpp
void YourModule::setContentAccess(QObject* contentWidget) {
    QPlainTextEdit* editor = qobject_cast<QPlainTextEdit*>(contentWidget);
    if (editor) {
        connect(editor, &QPlainTextEdit::textChanged, this, &YourModule::onContentChanged);
    }
}
```

2. **讀取當前內容**：
```cpp
void YourModule::onContentChanged() {
    QPlainTextEdit* editor = qobject_cast<QPlainTextEdit*>(sender());
    if (editor) {
        QString currentText = editor->toPlainText();
        // 處理文字內容...
    }
}
```

3. **修改內容**（謹慎使用）：
```cpp
void YourModule::modifyContent() {
    if (m_contentWidget) {
        QPlainTextEdit* editor = qobject_cast<QPlainTextEdit*>(m_contentWidget);
        if (editor) {
            editor->setPlainText("新的內容");
        }
    }
}
```

### ChapterIdx 和 ChapterLabel 存取機制

透過 `setChapterAccess()` 方法，模組可以：

1. **章節資訊分析**：
```cpp
void YourModule::setChapterAccess(const QStringList& chapterIdx, const QStringList& chapterLabel) {
    m_chapterIdx = chapterIdx;
    m_chapterLabel = chapterLabel;
    
    // 更新章節相關的 UI
    updateChapterDisplay();
}
```

2. **章節統計**：
```cpp
void YourModule::analyzeChapters() {
    for (int i = 0; i < m_chapterIdx.size() && i < m_chapterLabel.size(); ++i) {
        QString idx = m_chapterIdx[i];
        QString label = m_chapterLabel[i];
        // 進行章節分析...
    }
}
```

---

## 完整實作範例

```cpp
// ExampleModule.h
#include "ModuleTemplate.h"
#include <QWidget>
#include <QPlainTextEdit>

class ExampleModule : public QWidget, public ModuleTemplate
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID ModuleInterface_iid)
    Q_INTERFACES(ModuleTemplate)

public:
    explicit ExampleModule(QWidget *parent = nullptr);
    
    // ModuleTemplate 介面實作
    QString name() const override { return "範例模組"; }
    QUuid moduleUuid() const override { return QUuid("12345678-1234-5678-9abc-123456789abc"); }
    int priority() const override { return 75; }
    QIcon icon() const override { return QIcon(":/icons/example.png"); }
    QWidget* widget() override { return this; }
    
    void OpenFile(const QString& content) override;
    QString SaveFile() override;
    void setContentAccess(QObject* contentWidget) override;
    void setChapterAccess(const QStringList& chapterIdx, const QStringList& chapterLabel) override;

private slots:
    void onContentChanged();

private:
    QObject* m_contentWidget;
    QStringList m_chapterIdx;
    QStringList m_chapterLabel;
};

// ExampleModule.cpp
#include "ExampleModule.h"

ExampleModule::ExampleModule(QWidget *parent) 
    : QWidget(parent), m_contentWidget(nullptr) {
    // 初始化 UI...
}

void ExampleModule::OpenFile(const QString& content) {
    // 自動判斷內容格式
    if (content.startsWith("<?xml")) {
        // 處理 XML 格式
    } else if (content.contains("{")) {
        // 處理 JSON 格式  
    } else {
        // 處理純文字格式
    }
}

QString ExampleModule::SaveFile() {
    // 根據模組狀態決定輸出格式
    return generateModuleData();
}

void ExampleModule::setContentAccess(QObject* contentWidget) {
    m_contentWidget = contentWidget;
    QPlainTextEdit* editor = qobject_cast<QPlainTextEdit*>(contentWidget);
    if (editor) {
        connect(editor, &QPlainTextEdit::textChanged, this, &ExampleModule::onContentChanged);
    }
}

void ExampleModule::setChapterAccess(const QStringList& chapterIdx, const QStringList& chapterLabel) {
    m_chapterIdx = chapterIdx;
    m_chapterLabel = chapterLabel;
    // 更新章節相關的顯示...
}

void ExampleModule::onContentChanged() {
    // 處理內容變化事件...
}
```

---

## 模組註冊

模組須透過 Qt 插件系統進行註冊：

```cpp
Q_PLUGIN_METADATA(IID ModuleInterface_iid)
Q_INTERFACES(ModuleTemplate)
```

---

## 實作提示

### 基本要求
* 必須以 `QObject` 派生類別實作您的模組，以支援 Qt Meta-Object System
* 若使用 Qt Designer，建議將 `QWidget* widget()` 回傳為自訂的 UI 表單類別
* 請確保您的模組使用唯一 `moduleUuid`，避免與其他模組衝突

### 優先級設定建議
* **高優先級 (100+)**：核心功能，如文字編輯、基本資訊顯示
* **中優先級 (50-99)**：輔助功能，如語法檢查、統計分析
* **低優先級 (1-49)**：擴充功能，如匯出工具、第三方整合

### 共享資源使用注意事項
* **Content 存取**：避免頻繁修改主要內容，優先使用監聽模式
* **Chapter 存取**：章節資訊可能為空，請檢查資料有效性
* **訊號槽連接**：記得在模組卸載時正確斷開連接

### 檔案操作最佳實務
* **OpenFile**：先檢查內容格式，再決定處理方式
* **SaveFile**：確保輸出格式與應用程式期望的格式相容
* **錯誤處理**：加入適當的錯誤處理機制

---
