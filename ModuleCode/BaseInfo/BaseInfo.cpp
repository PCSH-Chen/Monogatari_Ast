#include "BaseInfo.h"
#include <QStandardItemModel>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDomDocument>
#include <QDateTime>
#include <QDebug>

BaseInfoModule::BaseInfoModule()
    : m_widget(new QWidget)
{
    ui.setupUi(m_widget);

    // 載入贅字與修飾詞詞庫
    loadJsonToSet(":/RedundancyWords.json", m_redundancySet);
    loadJsonToSet(":/FilterWords.json", m_filterSet);

    // 初始化常用字表格
    auto* model = new QStandardItemModel(0, 4, m_widget);
    model->setHorizontalHeaderLabels({ tr("字詞"), tr("次數"), tr("贅字？"), tr("修飾詞？") });
    //ui.RecentWordsTable->setModel(model);
    ui.RecentWordsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui.RecentWordsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

BaseInfoModule::~BaseInfoModule()
{
    delete m_widget;
}

void BaseInfoModule::loadJsonToSet(const QString& filename, QSet<QString>& targetSet)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open JSON:" << filename;
        return;
    }
    QByteArray data = file.readAll();
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isArray()) {
        for (const QJsonValue& v : doc.array())
            targetSet.insert(v.toString());
    }
}

void BaseInfoModule::OpenFile(const QString& content)
{
    QDomDocument doc;
    if (!doc.setContent(content)) return;
    QDomElement root = doc.documentElement();
    if (root.tagName() != "BaseInfo") return; // 只處理 BaseInfo

    // BookName
    QDomElement bookName = root.firstChildElement("BookName");
    if (!bookName.isNull()) ui.BookNameEdit->setText(bookName.text());

    // Author
    QDomElement author = root.firstChildElement("AnotherName");
    if (!author.isNull()) ui.AnotherNameEdit->setText(author.text());

    // StoryType
    QDomElement storyType = root.firstChildElement("StoryType");
    if (!storyType.isNull()) ui.StoryTypeEdit->setText(storyType.text());

    // Tags
    QDomElement tagsElem = root.firstChildElement("Tags");
    QStringList tagList;
    for (QDomElement tag = tagsElem.firstChildElement("Tag"); !tag.isNull(); tag = tag.nextSiblingElement("Tag"))
        tagList << tag.text();
    QStandardItemModel* tagModel = new QStandardItemModel(tagList.size(), 1, m_widget);
    for (int i = 0; i < tagList.size(); ++i)
        tagModel->setItem(i, 0, new QStandardItem(tagList[i]));
    ui.Tagslist->setModel(tagModel);

    // Intro
    QDomElement intro = root.firstChildElement("Intro");
    if (!intro.isNull()) ui.IntroEdit->setPlainText(intro.text());

    // LatestSave
    QDomElement saveTime = root.firstChildElement("LatestSave");
    if (!saveTime.isNull()) ui.LatestSaveShow->setText(saveTime.text());
}

QString BaseInfoModule::SaveFile()
{
    QDomDocument doc;
    QDomElement root = doc.createElement("BaseInfo");
    doc.appendChild(root);

    // BookName
    QDomElement bookName = doc.createElement("BookName");
    bookName.appendChild(doc.createTextNode(ui.BookNameEdit->text()));
    root.appendChild(bookName);

    // Author
    QDomElement author = doc.createElement("AnotherName");
    author.appendChild(doc.createTextNode(ui.AnotherNameEdit->text()));
    root.appendChild(author);

    // StoryType
    QDomElement storyType = doc.createElement("StoryType");
    storyType.appendChild(doc.createTextNode(ui.StoryTypeEdit->text()));
    root.appendChild(storyType);

    // Tags
    QDomElement tagsElem = doc.createElement("Tags");
    QStandardItemModel* tagModel = qobject_cast<QStandardItemModel*>(ui.Tagslist->model());
    if (tagModel) {
        for (int i = 0; i < tagModel->rowCount(); ++i) {
            QDomElement tagElem = doc.createElement("Tag");
            tagElem.appendChild(doc.createTextNode(tagModel->item(i, 0)->text()));
            tagsElem.appendChild(tagElem);
        }
    }
    root.appendChild(tagsElem);

    // Intro
    QDomElement intro = doc.createElement("Intro");
    intro.appendChild(doc.createTextNode(ui.IntroEdit->toPlainText()));
    root.appendChild(intro);

    // LatestSave
    QDomElement saveTime = doc.createElement("LatestSave");
    saveTime.appendChild(doc.createTextNode(ui.LatestSaveShow->text()));
    root.appendChild(saveTime);

    return doc.toString();
}

void BaseInfoModule::ImportWords(const QVector<QString>& words, int total, double redundancyPercent, double filterPercent)
{
    m_words = QSet<QString>(words.begin(), words.end());
    updateRecentWordsTable(words);
    ui.TotalWordsVal->setText(QString::number(total));
    ui.RedundancyWordsVal->setText(QString::number(redundancyPercent, 'f', 2));
    ui.FilterWordsVal->setText(QString::number(filterPercent, 'f', 2));
}

void BaseInfoModule::updateRecentWordsTable(const QVector<QString>& words)
{
    QMap<QString, int> freq;
    for (const QString& w : words) freq[w]++;
    auto* model = qobject_cast<QStandardItemModel*>(ui.RecentWordsTable->model());
    model->removeRows(0, model->rowCount());
    for (auto it = freq.begin(); it != freq.end(); ++it) {
        QList<QStandardItem*> row;
        row << new QStandardItem(it.key());
        row << new QStandardItem(QString::number(it.value()));
        row << new QStandardItem(m_redundancySet.contains(it.key()) ? tr("是") : tr("否"));
        row << new QStandardItem(m_filterSet.contains(it.key()) ? tr("是") : tr("否"));
        model->appendRow(row);
    }
}
