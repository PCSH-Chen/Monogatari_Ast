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

* `void OpenFile(const QString& content)`
  載入資料。請於內部實作中自行解析並處理提供的內容。模組應根據內容格式進行適當的解析。

* `QString SaveFile()`
  儲存資料。由主程式呼叫以取得序列化後的內容字串。模組應將內部資料轉換為符合格式規範的 XML 字串。
