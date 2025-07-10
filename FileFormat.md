# 物語アシスタント｜資料儲存格式指南

本文件說明物語アシスタント模組的資料儲存格式，並規範資料的讀寫方式。所有資料應遵循指定的 XML 結構，確保能夠順利解析自 `*.mgf` 檔案。

## 資料結構格式

每筆資料應以 `<Type>` 作為根節點，並於其下依據類別名稱進行分類儲存，例如：

```xml
<Type>
    <ClassName1>
        <Content>內容</Content>
        <Content>內容</Content>
    </ClassName1>
    <ClassName2>
        <Content>內容</Content>
        <Content>內容</Content>
    </ClassName2>
    ...
</Type>
```

* `<Type>` 必須為獨立的一行，以利程式正確解析分類。
* `<ClassName>` 代表任意類別名稱，用於分組。
* `<Content>` 可為任意自定名稱的子節點。

## 自定義內容結構

`<Content>` 節點名稱不固定，可以根據需求自定義，例如：

```xml
<ClassName1>
    <Title>標題</Title>
    <Text>文字內容</Text>
</ClassName1>
```

也支援巢狀結構，例如：

```xml
<List>
    <Item>第一項</Item>
    <Item>第二項</Item>
</List>
```

或更進階的表格形式：

```xml
<Table>
    <List>
        <Item>0,0</Item>
        <Item>0,1</Item>
    </List>
    <List>
        <Item>1,0</Item>
        <Item>1,1</Item>
    </List>
</Table>
```

## API 函式說明

請統一使用以下函式進行資料操作：

### `void OpenFile(const QString& content)`

**功能說明**：載入檔案內容到模組中。

**Type 判定機制**：函式會自動分析 `content` 內容來判斷資料類型，不需要額外的 type 參數。

**實作建議**：
```cpp
void YourModule::OpenFile(const QString& content) {
    // 自動判斷內容格式
    if (content.trimmed().startsWith("<?xml") || content.contains("<Type>")) {
        // 處理 XML 格式的 MGF 檔案
        parseXmlContent(content);
    } else if (content.trimmed().startsWith("{") && content.trimmed().endsWith("}")) {
        // 處理 JSON 格式
        parseJsonContent(content);
    } else {
        // 處理純文字格式
        parseTextContent(content);
    }
}
```

**格式檢查範例**：
- **XML 檢查**：`content.contains("<Type>")` 或 `content.startsWith("<?xml")`
- **JSON 檢查**：`content.trimmed().startsWith("{")` 
- **純文字**：其他格式則視為純文字處理

### `QString SaveFile()`

**功能說明**：將模組資料序列化為字串格式以便儲存。

**Type 判定機制**：函式根據模組內部狀態和設定，自動決定輸出的資料格式。

**實作建議**：
```cpp
QString YourModule::SaveFile() {
    // 根據模組設定決定輸出格式
    if (m_outputFormat == XmlFormat) {
        return generateXmlOutput();
    } else if (m_outputFormat == JsonFormat) {
        return generateJsonOutput();
    } else {
        return generateTextOutput();
    }
}

QString YourModule::generateXmlOutput() {
    QString result = "<Type>\n";
    result += "    <" + getModuleClassName() + ">\n";
    
    // 加入模組特定的資料
    for (const auto& item : m_dataItems) {
        result += "        <Content>" + item + "</Content>\n";
    }
    
    result += "    </" + getModuleClassName() + ">\n";
    result += "</Type>";
    return result;
}
```

## 資料格式範例

### 基本資訊模組格式
```xml
<Type>
    <BaseInfo>
        <Title>小說標題</Title>
        <Author>作者名稱</Author>
        <WordCount>12345</WordCount>
        <Tags>
            <Tag>奇幻</Tag>
            <Tag>冒險</Tag>
        </Tags>
    </BaseInfo>
</Type>
```

### 章節資訊格式
```xml
<Type>
    <ChapterInfo>
        <Chapter>
            <Index>1</Index>
            <Label>第一章 開始</Label>
            <WordCount>2500</WordCount>
        </Chapter>
        <Chapter>
            <Index>2</Index>
            <Label>第二章 冒險</Label>
            <WordCount>3200</WordCount>
        </Chapter>
    </ChapterInfo>
</Type>
```

### 角色資訊格式
```xml
<Type>
    <CharacterInfo>
        <Character>
            <Name>主角</Name>
            <Description>勇敢的冒險者</Description>
            <Attributes>
                <Attribute name="年齡">20</Attribute>
                <Attribute name="職業">劍士</Attribute>
            </Attributes>
        </Character>
    </CharacterInfo>
</Type>
```

## 實作注意事項

### 讀取檔案時
1. **格式驗證**：先檢查內容格式是否符合預期
2. **錯誤處理**：對於格式錯誤的內容，應提供適當的錯誤訊息
3. **相容性**：支援多種格式以提高相容性

### 儲存檔案時
1. **格式一致性**：確保輸出格式與應用程式期望相符
2. **資料完整性**：檢查所有必要資料都已包含
3. **編碼正確**：使用 UTF-8 編碼確保中文字元正確顯示

### Type 判定最佳實務
1. **優先使用標準格式**：XML 格式具有最佳相容性
2. **備用格式支援**：提供 JSON 或純文字格式作為備選
3. **格式標記**：在檔案開頭加入格式識別標記

```cpp
// 格式判定函式範例
QString YourModule::detectFormat(const QString& content) {
    QString trimmed = content.trimmed();
    
    if (trimmed.startsWith("<?xml") || trimmed.contains("<Type>")) {
        return "XML";
    } else if (trimmed.startsWith("{") && trimmed.contains("\"Type\"")) {
        return "JSON";
    } else {
        return "TEXT";
    }
}
```

## 檔案擴展名約定

* **`.mgf`**：標準的物語アシスタント檔案格式（通常為 XML）
* **`.mgf.json`**：JSON 格式的備用檔案格式  
* **`.mgf.txt`**：純文字格式的備用檔案格式

透過這樣的約定，可以讓不同模組更容易識別和處理適當的檔案格式。
