/********************************************************************************
** Form generated from reading UI file 'BaseInfo.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BASEINFO_H
#define UI_BASEINFO_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QVBoxLayout *FormLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout;
    QLabel *Title;
    QSpacerItem *verticalSpacer_2;
    QLabel *BookNameLabel;
    QLineEdit *BookNameEdit;
    QLabel *AnotherNameLabel;
    QLineEdit *AnotherNameEdit;
    QLabel *StorTypeLabel;
    QLineEdit *StoryTypeEdit;
    QLabel *TagsLabel;
    QListView *Tagslist;
    QHBoxLayout *TagEdit;
    QLineEdit *TagName;
    QPushButton *TagAdd;
    QPushButton *TagDel;
    QLabel *IntroLabel;
    QTextEdit *IntroEdit;
    QLabel *LatestSaveLabel;
    QLabel *LatestSaveShow;
    QSpacerItem *verticalSpacer_3;
    QLabel *RecentWordsLabel;
    QTableWidget *RecentWordsTable;
    QGridLayout *gridLayout;
    QLabel *RedundancyWordsLabel;
    QLabel *TotalWordsUnit;
    QLabel *TotalWordsLabel;
    QLabel *RedundancyWordsUnit;
    QLabel *TotalWordsVal;
    QLabel *RedundancyWordsVal;
    QLabel *FilterWordsLabel;
    QLabel *FilterWordsVal;
    QLabel *FilterWordsUnit;
    QLabel *InfoLabel;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName("Form");
        Form->resize(293, 402);
        FormLayout = new QVBoxLayout(Form);
        FormLayout->setObjectName("FormLayout");
        scrollArea = new QScrollArea(Form);
        scrollArea->setObjectName("scrollArea");
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName("scrollAreaWidgetContents");
        scrollAreaWidgetContents->setGeometry(QRect(0, -449, 259, 831));
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(scrollAreaWidgetContents->sizePolicy().hasHeightForWidth());
        scrollAreaWidgetContents->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(4, 4, 4, 4);
        Title = new QLabel(scrollAreaWidgetContents);
        Title->setObjectName("Title");
        QFont font;
        font.setPointSize(20);
        Title->setFont(font);

        verticalLayout->addWidget(Title);

        verticalSpacer_2 = new QSpacerItem(20, 4, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);

        verticalLayout->addItem(verticalSpacer_2);

        BookNameLabel = new QLabel(scrollAreaWidgetContents);
        BookNameLabel->setObjectName("BookNameLabel");
        QFont font1;
        font1.setPointSize(12);
        BookNameLabel->setFont(font1);

        verticalLayout->addWidget(BookNameLabel);

        BookNameEdit = new QLineEdit(scrollAreaWidgetContents);
        BookNameEdit->setObjectName("BookNameEdit");

        verticalLayout->addWidget(BookNameEdit);

        AnotherNameLabel = new QLabel(scrollAreaWidgetContents);
        AnotherNameLabel->setObjectName("AnotherNameLabel");
        AnotherNameLabel->setFont(font1);

        verticalLayout->addWidget(AnotherNameLabel);

        AnotherNameEdit = new QLineEdit(scrollAreaWidgetContents);
        AnotherNameEdit->setObjectName("AnotherNameEdit");

        verticalLayout->addWidget(AnotherNameEdit);

        StorTypeLabel = new QLabel(scrollAreaWidgetContents);
        StorTypeLabel->setObjectName("StorTypeLabel");
        StorTypeLabel->setFont(font1);

        verticalLayout->addWidget(StorTypeLabel);

        StoryTypeEdit = new QLineEdit(scrollAreaWidgetContents);
        StoryTypeEdit->setObjectName("StoryTypeEdit");

        verticalLayout->addWidget(StoryTypeEdit);

        TagsLabel = new QLabel(scrollAreaWidgetContents);
        TagsLabel->setObjectName("TagsLabel");
        TagsLabel->setFont(font1);

        verticalLayout->addWidget(TagsLabel);

        Tagslist = new QListView(scrollAreaWidgetContents);
        Tagslist->setObjectName("Tagslist");

        verticalLayout->addWidget(Tagslist);

        TagEdit = new QHBoxLayout();
        TagEdit->setObjectName("TagEdit");
        TagName = new QLineEdit(scrollAreaWidgetContents);
        TagName->setObjectName("TagName");

        TagEdit->addWidget(TagName);

        TagAdd = new QPushButton(scrollAreaWidgetContents);
        TagAdd->setObjectName("TagAdd");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(TagAdd->sizePolicy().hasHeightForWidth());
        TagAdd->setSizePolicy(sizePolicy1);
        TagAdd->setMaximumSize(QSize(24, 16777215));

        TagEdit->addWidget(TagAdd);

        TagDel = new QPushButton(scrollAreaWidgetContents);
        TagDel->setObjectName("TagDel");
        TagDel->setMaximumSize(QSize(24, 16777215));

        TagEdit->addWidget(TagDel);


        verticalLayout->addLayout(TagEdit);

        IntroLabel = new QLabel(scrollAreaWidgetContents);
        IntroLabel->setObjectName("IntroLabel");
        IntroLabel->setFont(font1);

        verticalLayout->addWidget(IntroLabel);

        IntroEdit = new QTextEdit(scrollAreaWidgetContents);
        IntroEdit->setObjectName("IntroEdit");

        verticalLayout->addWidget(IntroEdit);

        LatestSaveLabel = new QLabel(scrollAreaWidgetContents);
        LatestSaveLabel->setObjectName("LatestSaveLabel");
        LatestSaveLabel->setFont(font1);

        verticalLayout->addWidget(LatestSaveLabel);

        LatestSaveShow = new QLabel(scrollAreaWidgetContents);
        LatestSaveShow->setObjectName("LatestSaveShow");
        QFont font2;
        font2.setPointSize(16);
        LatestSaveShow->setFont(font2);

        verticalLayout->addWidget(LatestSaveShow);

        verticalSpacer_3 = new QSpacerItem(20, 32, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);

        verticalLayout->addItem(verticalSpacer_3);

        RecentWordsLabel = new QLabel(scrollAreaWidgetContents);
        RecentWordsLabel->setObjectName("RecentWordsLabel");
        RecentWordsLabel->setFont(font1);

        verticalLayout->addWidget(RecentWordsLabel);

        RecentWordsTable = new QTableWidget(scrollAreaWidgetContents);
        if (RecentWordsTable->columnCount() < 4)
            RecentWordsTable->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        RecentWordsTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        RecentWordsTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        RecentWordsTable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        RecentWordsTable->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        RecentWordsTable->setObjectName("RecentWordsTable");
        RecentWordsTable->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        RecentWordsTable->setAlternatingRowColors(true);
        RecentWordsTable->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        RecentWordsTable->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        RecentWordsTable->setVerticalScrollMode(QAbstractItemView::ScrollMode::ScrollPerPixel);
        RecentWordsTable->setHorizontalScrollMode(QAbstractItemView::ScrollMode::ScrollPerPixel);
        RecentWordsTable->setShowGrid(true);
        RecentWordsTable->setCornerButtonEnabled(false);
        RecentWordsTable->setRowCount(0);
        RecentWordsTable->setColumnCount(4);
        RecentWordsTable->horizontalHeader()->setVisible(true);
        RecentWordsTable->verticalHeader()->setVisible(false);

        verticalLayout->addWidget(RecentWordsTable);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        RedundancyWordsLabel = new QLabel(scrollAreaWidgetContents);
        RedundancyWordsLabel->setObjectName("RedundancyWordsLabel");

        gridLayout->addWidget(RedundancyWordsLabel, 1, 0, 1, 1);

        TotalWordsUnit = new QLabel(scrollAreaWidgetContents);
        TotalWordsUnit->setObjectName("TotalWordsUnit");

        gridLayout->addWidget(TotalWordsUnit, 0, 2, 1, 1);

        TotalWordsLabel = new QLabel(scrollAreaWidgetContents);
        TotalWordsLabel->setObjectName("TotalWordsLabel");

        gridLayout->addWidget(TotalWordsLabel, 0, 0, 1, 1);

        RedundancyWordsUnit = new QLabel(scrollAreaWidgetContents);
        RedundancyWordsUnit->setObjectName("RedundancyWordsUnit");

        gridLayout->addWidget(RedundancyWordsUnit, 1, 2, 1, 1);

        TotalWordsVal = new QLabel(scrollAreaWidgetContents);
        TotalWordsVal->setObjectName("TotalWordsVal");

        gridLayout->addWidget(TotalWordsVal, 0, 1, 1, 1);

        RedundancyWordsVal = new QLabel(scrollAreaWidgetContents);
        RedundancyWordsVal->setObjectName("RedundancyWordsVal");

        gridLayout->addWidget(RedundancyWordsVal, 1, 1, 1, 1);

        FilterWordsLabel = new QLabel(scrollAreaWidgetContents);
        FilterWordsLabel->setObjectName("FilterWordsLabel");

        gridLayout->addWidget(FilterWordsLabel, 2, 0, 1, 1);

        FilterWordsVal = new QLabel(scrollAreaWidgetContents);
        FilterWordsVal->setObjectName("FilterWordsVal");

        gridLayout->addWidget(FilterWordsVal, 2, 1, 1, 1);

        FilterWordsUnit = new QLabel(scrollAreaWidgetContents);
        FilterWordsUnit->setObjectName("FilterWordsUnit");

        gridLayout->addWidget(FilterWordsUnit, 2, 2, 1, 1);


        verticalLayout->addLayout(gridLayout);

        InfoLabel = new QLabel(scrollAreaWidgetContents);
        InfoLabel->setObjectName("InfoLabel");
        InfoLabel->setTextFormat(Qt::TextFormat::MarkdownText);
        InfoLabel->setScaledContents(true);
        InfoLabel->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignTop);

        verticalLayout->addWidget(InfoLabel);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        scrollArea->setWidget(scrollAreaWidgetContents);

        FormLayout->addWidget(scrollArea);


        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QCoreApplication::translate("Form", "Form", nullptr));
        Title->setText(QCoreApplication::translate("Form", "\345\237\272\346\234\254\350\263\207\350\250\212", nullptr));
        BookNameLabel->setText(QCoreApplication::translate("Form", "\346\233\270\345\220\215\357\274\232", nullptr));
        AnotherNameLabel->setText(QCoreApplication::translate("Form", "\344\275\234\350\200\205\357\274\232", nullptr));
        StorTypeLabel->setText(QCoreApplication::translate("Form", "\351\241\236\345\236\213\357\274\232", nullptr));
        TagsLabel->setText(QCoreApplication::translate("Form", "Tags\357\274\232", nullptr));
        TagAdd->setText(QCoreApplication::translate("Form", "+", nullptr));
        TagDel->setText(QCoreApplication::translate("Form", "-", nullptr));
        IntroLabel->setText(QCoreApplication::translate("Form", "\347\260\241\344\273\213\357\274\232", nullptr));
        LatestSaveLabel->setText(QCoreApplication::translate("Form", "\346\234\200\345\276\214\345\204\262\345\255\230\346\231\202\351\226\223", nullptr));
        LatestSaveShow->setText(QCoreApplication::translate("Form", "YYYY.MM.DD hh.mm.ss", nullptr));
        RecentWordsLabel->setText(QCoreApplication::translate("Form", "\345\270\270\347\224\250\345\255\227\345\210\227\350\241\250\357\274\232", nullptr));
        QTableWidgetItem *___qtablewidgetitem = RecentWordsTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("Form", "\345\255\227\350\251\236", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = RecentWordsTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("Form", "\346\254\241\346\225\270", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = RecentWordsTable->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("Form", "\350\264\205\345\255\227\357\274\237", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = RecentWordsTable->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("Form", "\344\277\256\351\243\276\350\251\236\357\274\237", nullptr));
        RedundancyWordsLabel->setText(QCoreApplication::translate("Form", "\350\264\205\345\255\227\345\200\274", nullptr));
        TotalWordsUnit->setText(QCoreApplication::translate("Form", "\345\255\227", nullptr));
        TotalWordsLabel->setText(QCoreApplication::translate("Form", "\347\270\275\345\255\227\346\225\270", nullptr));
        RedundancyWordsUnit->setText(QCoreApplication::translate("Form", "%", nullptr));
        TotalWordsVal->setText(QCoreApplication::translate("Form", "%d", nullptr));
        RedundancyWordsVal->setText(QCoreApplication::translate("Form", "%.2f", nullptr));
        FilterWordsLabel->setText(QCoreApplication::translate("Form", "\344\277\256\351\243\276\345\200\274", nullptr));
        FilterWordsVal->setText(QCoreApplication::translate("Form", "%.2f", nullptr));
        FilterWordsUnit->setText(QCoreApplication::translate("Form", "%", nullptr));
        InfoLabel->setText(QCoreApplication::translate("Form", "\350\264\205\345\255\227\345\200\274\357\274\2326.16%\344\273\245\344\270\213\347\202\272\345\201\245\345\272\267\347\257\204\345\234\215\n"
"\n"
"\344\277\256\351\243\276\345\200\274\357\274\232\n"
" * \347\254\254\344\270\200\344\272\272\347\250\261\344\270\2136.16%\344\273\245\344\270\213\347\202\272\345\201\245\345\272\267\347\257\204\345\234\215\n"
" * \347\254\254\344\270\211\344\272\272\347\250\261\344\270\2133.08%\344\273\245\344\270\213\347\202\272\345\201\245\345\272\267\347\257\204\345\234\215", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BASEINFO_H
