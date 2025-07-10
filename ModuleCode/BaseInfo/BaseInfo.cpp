#include "BaseInfo.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QPlainTextEdit>
#include <QDebug>
#include <QRegularExpression>
#include <QUuid>

BaseInfoModule::BaseInfoModule(QWidget *parent)
    : QWidget(parent), ui(new Ui::BaseInfo), m_tagModel(new QStringListModel(this))
    , m_contentEditor(nullptr), m_mainWindow(nullptr)
{
    ui->setupUi(this);
    setupUI();
    loadStopwords();
}

BaseInfoModule::~BaseInfoModule()
{
    delete ui;
}

void BaseInfoModule::setupUI()
{
    // 設定 Tag 列表模型
    ui->Tagslist->setModel(m_tagModel);

    // 連接訊號與槽
    connect(ui->TagAdd, &QPushButton::clicked, this, &BaseInfoModule::onTagAdd);
    connect(ui->TagDel, &QPushButton::clicked, this, &BaseInfoModule::onTagDel);

    // 設定表格
    ui->RecentWordsTable->setColumnCount(3);
    ui->RecentWordsTable->setHorizontalHeaderLabels({"字詞", "次數", "贅字？"});
    ui->RecentWordsTable->horizontalHeader()->setStretchLastSection(true);
    ui->RecentWordsTable->setColumnWidth(0, 100);
    ui->RecentWordsTable->setColumnWidth(1, 50);
}

// --- ModuleTemplate 介面實作 ---

QString BaseInfoModule::name() const { return "基本資訊"; }

QUuid BaseInfoModule::moduleUuid() const { 
    // 為 BaseInfo 模組提供唯一的 UUID
    return QUuid("{550e8400-e29b-41d4-a716-446655440000}"); 
}

int BaseInfoModule::priority() const { 
    return 90; // 高優先級，核心功能模組
}

QIcon BaseInfoModule::icon() const { return QIcon(); /* 暫時返回空圖示 */ }
QWidget* BaseInfoModule::widget() { return this; }

void BaseInfoModule::setContentAccess(QPlainTextEdit* content)
{
    m_contentEditor = content;
    if (m_contentEditor) {
        connect(m_contentEditor, &QPlainTextEdit::textChanged, 
                this, &BaseInfoModule::onContentChanged);
        qDebug() << "BaseInfo: Content access set up successfully";
    }
}

void BaseInfoModule::setChapterAccess(MainWindow* mainWindow)
{
    m_mainWindow = mainWindow;
    if (m_mainWindow) {
        // 可以在這裡連接章節相關的信號
        qDebug() << "BaseInfo: Chapter access set up successfully";
    }
}

void BaseInfoModule::OpenFile(const QString& content)
{
    // TODO: 實作開啟檔案時的資料載入邏輯
    Q_UNUSED(content);
    qDebug() << "BaseInfo: OpenFile called";
}

QString BaseInfoModule::SaveFile()
{
    // TODO: 實作儲存檔案時的資料序列化邏輯
    qDebug() << "BaseInfo: SaveFile called";
    return QString(); // 暫時返回空字串
}



// --- 核心邏輯 ---

void BaseInfoModule::loadStopwords()
{
    QFile file(":/res/stopwords.json");
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open stopwords.json");
        return;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(data));
    QJsonArray array = doc.array();

    for (const QJsonValue &value : array) {
        m_stopwords.insert(value.toString());
    }
    file.close();
}

void BaseInfoModule::onContentChanged()
{
    // 使用新的 m_contentEditor 成員變數
    if (!m_contentEditor) {
        qWarning() << "BaseInfo: Content editor not available";
        return;
    }

    QString currentText = m_contentEditor->toPlainText();

    updateWordCount(currentText);
    updateRecentWords(currentText);
    updateRedundancy(currentText);

    // 更新最後儲存時間
    ui->LatestSaveShow->setText(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss"));
}


void BaseInfoModule::updateWordCount(const QString& text)
{
    ui->TotalWordsVal->setText(QString::number(text.length()));
}

void BaseInfoModule::updateRecentWords(const QString& text)
{
    m_vocabulary.clear();
    QMap<QString, int> wordCounts;
    QRegularExpression re("[\\s\\p{P}]"); // 分割符號：空白和標點
    QStringList words = text.split(re, Qt::SkipEmptyParts);

    for (const QString& word : words) {
        m_vocabulary.insert(word);
        wordCounts[word]++;
    }

    // 更新表格
    ui->RecentWordsTable->setRowCount(0);
    for (auto it = wordCounts.constBegin(); it != wordCounts.constEnd(); ++it) {
        int row = ui->RecentWordsTable->rowCount();
        ui->RecentWordsTable->insertRow(row);
        ui->RecentWordsTable->setItem(row, 0, new QTableWidgetItem(it.key()));
        ui->RecentWordsTable->setItem(row, 1, new QTableWidgetItem(QString::number(it.value())));

        QTableWidgetItem* isStopwordItem = new QTableWidgetItem(m_stopwords.contains(it.key()) ? "是" : "否");
        ui->RecentWordsTable->setItem(row, 2, isStopwordItem);
    }
}

void BaseInfoModule::updateRedundancy(const QString& text)
{
    if (text.isEmpty()) {
        ui->RedundancyWordsVal->setText("0.00");
        return;
    }

    int stopwordCount = 0;
    QRegularExpression re("[\\s\\p{P}]");
    QStringList words = text.split(re, Qt::SkipEmptyParts);

    for (const QString& word : words) {
        if (m_stopwords.contains(word)) {
            stopwordCount++;
        }
    }

    double totalWords = words.length();
    double redundancyRate = (totalWords > 0) ? (static_cast<double>(stopwordCount) / totalWords) * 100.0 : 0.0;
    ui->RedundancyWordsVal->setText(QString::asprintf("%.2f", redundancyRate));
}

// --- UI 事件處理 ---

void BaseInfoModule::onTagAdd()
{
    QString newTag = ui->TagName->text().trimmed();
    if (!newTag.isEmpty() && !m_tags.contains(newTag)) {
        m_tags.insert(newTag);
        QStringList tagList = QStringList(m_tags.begin(), m_tags.end());
        m_tagModel->setStringList(tagList);
        ui->TagName->clear();
    }
}

void BaseInfoModule::onTagDel()
{
    QModelIndexList selectedIndexes = ui->Tagslist->selectionModel()->selectedIndexes();
    if (selectedIndexes.isEmpty()) return;

    QString tagToRemove = selectedIndexes.first().data(Qt::DisplayRole).toString();
    m_tags.remove(tagToRemove);
    QStringList tagList = QStringList(m_tags.begin(), m_tags.end());
    m_tagModel->setStringList(tagList);
}

void BaseInfoModule::onTagSelectionChanged()
{
    // 可選：未來可用於篩選等功能
}

// --- 對外擴充函數實作 ---
void BaseInfoModule::WordsListImport(const QVector<QString>& lists)
{
    for (const QString& word : lists) {
        m_vocabulary.insert(word);
    }
    // TODO: 可以在此處觸發一次列表更新
}
