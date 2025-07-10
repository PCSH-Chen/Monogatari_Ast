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

## Chapter 資訊格式

新架構支援章節資訊的共享機制，格式如下：

### ChapterIdx 格式
```cpp
ChapterIdx = QPair<int, int>    // (章節編號, 子章節編號)
```

範例：
- `ChapterIdx(1, 0)` 代表第一章
- `ChapterIdx(1, 3)` 代表第一章第三節
- `ChapterIdx(5, 1)` 代表第五章第一節

### ChapterLabel 格式
```cpp
ChapterLabel = QString          // 章節標籤文字
```

範例：
- `"第一章 開始"`
- `"第三章第二節 轉折"`
- `"尾聲"`

### XML 中的 Chapter 資訊
```xml
<Type>
    <ChapterInfo>
        <Chapter idx="1,0" label="第一章 開始">
            <Content>第一章的內容...</Content>
        </Chapter>
        <Chapter idx="1,1" label="第一章第一節 起始">
            <Content>第一章第一節的內容...</Content>
        </Chapter>
        <Chapter idx="2,0" label="第二章 發展">
            <Content>第二章的內容...</Content>
        </Chapter>
    </ChapterInfo>
</Type>
```

## 完整 XML 結構範例

```xml
<?xml version="1.0" encoding="UTF-8"?>
<Document>
    <Metadata>
        <Title>作品標題</Title>
        <Author>作者名稱</Author>
        <CreatedDate>2024-01-01</CreatedDate>
        <ModifiedDate>2024-01-15</ModifiedDate>
    </Metadata>
    
    <ChapterInfo>
        <Chapter idx="1,0" label="第一章 序幕">
            <Content>序幕的內容...</Content>
            <Tags>
                <Tag>開場</Tag>
                <Tag>介紹</Tag>
            </Tags>
        </Chapter>
        <Chapter idx="1,1" label="第一章第一節 初遇">
            <Content>初遇的內容...</Content>
            <Tags>
                <Tag>人物介紹</Tag>
            </Tags>
        </Chapter>
    </ChapterInfo>
    
    <CharacterInfo>
        <Character>
            <Name>主角名稱</Name>
            <Description>主角描述</Description>
            <Tags>
                <Tag>主角</Tag>
                <Tag>男性</Tag>
            </Tags>
        </Character>
    </CharacterInfo>
    
    <CustomData>
        <UserDefinedType>
            <CustomField>自定義內容</CustomField>
        </UserDefinedType>
    </CustomData>
</Document>
```

## API 函式說明

請統一使用以下函式進行資料操作：

### 檔案操作函式

* `void OpenFile(const QString& content)`
  載入資料。參數 `content` 包含完整的檔案內容字串。模組應自行解析 XML 結構並提取所需資料。

* `QString SaveFile()`
  儲存資料。回傳序列化後的 XML 內容字串，將由主程式寫入檔案。

### 內容存取函式

* `void setContentAccess(QPlainTextEdit* content)`
  設定主視窗內容編輯器的存取權限。模組可透過此方法取得主視窗的文字編輯器，實現統一的內容編輯功能。

* `void setChapterAccess(MainWindow* mainWindow)`
  設定主視窗的存取權限。模組可透過此方法取得主視窗實例，用於存取章節資訊和使用訊號槽通知機制。

### 訊號槽通知機制

模組可透過主視窗的訊號槽機制接收以下通知：
- 章節切換通知
- 內容變更通知
- 檔案載入完成通知
- 檔案儲存完成通知

## 最佳實踐

1. **資料驗證**：在 `OpenFile()` 中應驗證 XML 格式是否正確
2. **錯誤處理**：檔案操作應包含適當的錯誤處理機制
3. **效能考量**：大型檔案應考慮分段載入和延遲處理
4. **編碼處理**：確保 XML 使用 UTF-8 編碼
5. **向後相容**：新版本應能讀取舊版本的資料格式
