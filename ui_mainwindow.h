/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *mainLayout;
    QVBoxLayout *dockLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QSplitter *mainSplitter;
    QStackedWidget *SideBar;
    QWidget *page;
    QWidget *page_2;
    QWidget *ContentContainer;
    QVBoxLayout *rightVBox;
    QHBoxLayout *toolBarLayout;
    QToolButton *SaveBtn;
    QToolButton *UndoBtn;
    QToolButton *RedoBtn;
    QToolButton *AllSelectBtn;
    QToolButton *CopyBtn;
    QToolButton *CutBtn;
    QToolButton *PasteBtn;
    QToolButton *FindBtn;
    QSpacerItem *ToolBarSpacer;
    QLabel *ChapterNameLabel;
    QFrame *hLine;
    QPlainTextEdit *Content;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(960, 540);
        MainWindow->setMinimumSize(QSize(960, 540));
        MainWindow->setTabShape(QTabWidget::Rounded);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        mainLayout = new QHBoxLayout(centralwidget);
        mainLayout->setObjectName("mainLayout");
        dockLayout = new QVBoxLayout();
        dockLayout->setSpacing(0);
        dockLayout->setObjectName("dockLayout");
        scrollArea = new QScrollArea(centralwidget);
        scrollArea->setObjectName("scrollArea");
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName("scrollAreaWidgetContents");
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 69, 463));
        scrollArea->setWidget(scrollAreaWidgetContents);

        dockLayout->addWidget(scrollArea);


        mainLayout->addLayout(dockLayout);

        mainSplitter = new QSplitter(centralwidget);
        mainSplitter->setObjectName("mainSplitter");
        mainSplitter->setOrientation(Qt::Horizontal);
        SideBar = new QStackedWidget(mainSplitter);
        SideBar->setObjectName("SideBar");
        SideBar->setMinimumSize(QSize(200, 0));
        SideBar->setMaximumSize(QSize(400, 16777215));
        page = new QWidget();
        page->setObjectName("page");
        SideBar->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName("page_2");
        SideBar->addWidget(page_2);
        mainSplitter->addWidget(SideBar);
        ContentContainer = new QWidget(mainSplitter);
        ContentContainer->setObjectName("ContentContainer");
        rightVBox = new QVBoxLayout(ContentContainer);
        rightVBox->setSpacing(0);
        rightVBox->setObjectName("rightVBox");
        rightVBox->setContentsMargins(0, 0, 0, 0);
        toolBarLayout = new QHBoxLayout();
        toolBarLayout->setObjectName("toolBarLayout");
        SaveBtn = new QToolButton(ContentContainer);
        SaveBtn->setObjectName("SaveBtn");
        SaveBtn->setStyleSheet(QString::fromUtf8("\n"
"                QToolButton {\n"
"                    background: transparent;\n"
"                    border: none;\n"
"                    border-radius: 6px;\n"
"                    padding: 2px;\n"
"                }\n"
"                QToolButton:hover {\n"
"                    background: #e0e0e0;\n"
"                }\n"
"                QToolButton:pressed {\n"
"                    background: #b0b0b0;\n"
"                }\n"
"             "));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/res/icons/Save.svg"), QSize(), QIcon::Normal, QIcon::Off);
        SaveBtn->setIcon(icon);
        SaveBtn->setIconSize(QSize(24, 24));

        toolBarLayout->addWidget(SaveBtn);

        UndoBtn = new QToolButton(ContentContainer);
        UndoBtn->setObjectName("UndoBtn");
        UndoBtn->setMinimumSize(QSize(32, 0));
        UndoBtn->setStyleSheet(QString::fromUtf8("\n"
"                QToolButton {\n"
"                    background: transparent;\n"
"                    border: none;\n"
"                    border-radius: 6px;\n"
"                    padding: 2px;\n"
"                }\n"
"                QToolButton:hover {\n"
"                    background: #e0e0e0;\n"
"                }\n"
"                QToolButton:pressed {\n"
"                    background: #b0b0b0;\n"
"                }\n"
"             "));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/res/icons/Undo.svg"), QSize(), QIcon::Normal, QIcon::Off);
        UndoBtn->setIcon(icon1);
        UndoBtn->setIconSize(QSize(24, 24));

        toolBarLayout->addWidget(UndoBtn);

        RedoBtn = new QToolButton(ContentContainer);
        RedoBtn->setObjectName("RedoBtn");
        RedoBtn->setStyleSheet(QString::fromUtf8("\n"
"                QToolButton {\n"
"                    background: transparent;\n"
"                    border: none;\n"
"                    border-radius: 6px;\n"
"                    padding: 2px;\n"
"                }\n"
"                QToolButton:hover {\n"
"                    background: #e0e0e0;\n"
"                }\n"
"                QToolButton:pressed {\n"
"                    background: #b0b0b0;\n"
"                }\n"
"             "));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/res/icons/Redo.svg"), QSize(), QIcon::Normal, QIcon::Off);
        RedoBtn->setIcon(icon2);
        RedoBtn->setIconSize(QSize(24, 24));

        toolBarLayout->addWidget(RedoBtn);

        AllSelectBtn = new QToolButton(ContentContainer);
        AllSelectBtn->setObjectName("AllSelectBtn");
        AllSelectBtn->setStyleSheet(QString::fromUtf8("\n"
"                QToolButton {\n"
"                    background: transparent;\n"
"                    border: none;\n"
"                    border-radius: 6px;\n"
"                    padding: 2px;\n"
"                }\n"
"                QToolButton:hover {\n"
"                    background: #e0e0e0;\n"
"                }\n"
"                QToolButton:pressed {\n"
"                    background: #b0b0b0;\n"
"                }\n"
"             "));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/res/icons/AllSelect.svg"), QSize(), QIcon::Normal, QIcon::Off);
        AllSelectBtn->setIcon(icon3);
        AllSelectBtn->setIconSize(QSize(24, 24));

        toolBarLayout->addWidget(AllSelectBtn);

        CopyBtn = new QToolButton(ContentContainer);
        CopyBtn->setObjectName("CopyBtn");
        CopyBtn->setStyleSheet(QString::fromUtf8("\n"
"                QToolButton {\n"
"                    background: transparent;\n"
"                    border: none;\n"
"                    border-radius: 6px;\n"
"                    padding: 2px;\n"
"                }\n"
"                QToolButton:hover {\n"
"                    background: #e0e0e0;\n"
"                }\n"
"                QToolButton:pressed {\n"
"                    background: #b0b0b0;\n"
"                }\n"
"             "));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/res/icons/Copy.svg"), QSize(), QIcon::Normal, QIcon::Off);
        CopyBtn->setIcon(icon4);
        CopyBtn->setIconSize(QSize(24, 24));

        toolBarLayout->addWidget(CopyBtn);

        CutBtn = new QToolButton(ContentContainer);
        CutBtn->setObjectName("CutBtn");
        CutBtn->setStyleSheet(QString::fromUtf8("\n"
"                QToolButton {\n"
"                    background: transparent;\n"
"                    border: none;\n"
"                    border-radius: 6px;\n"
"                    padding: 2px;\n"
"                }\n"
"                QToolButton:hover {\n"
"                    background: #e0e0e0;\n"
"                }\n"
"                QToolButton:pressed {\n"
"                    background: #b0b0b0;\n"
"                }\n"
"             "));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/res/icons/Cut.svg"), QSize(), QIcon::Normal, QIcon::Off);
        CutBtn->setIcon(icon5);
        CutBtn->setIconSize(QSize(24, 24));

        toolBarLayout->addWidget(CutBtn);

        PasteBtn = new QToolButton(ContentContainer);
        PasteBtn->setObjectName("PasteBtn");
        PasteBtn->setStyleSheet(QString::fromUtf8("\n"
"                QToolButton {\n"
"                    background: transparent;\n"
"                    border: none;\n"
"                    border-radius: 6px;\n"
"                    padding: 2px;\n"
"                }\n"
"                QToolButton:hover {\n"
"                    background: #e0e0e0;\n"
"                }\n"
"                QToolButton:pressed {\n"
"                    background: #b0b0b0;\n"
"                }\n"
"             "));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/res/icons/Paste.svg"), QSize(), QIcon::Normal, QIcon::Off);
        PasteBtn->setIcon(icon6);
        PasteBtn->setIconSize(QSize(24, 24));

        toolBarLayout->addWidget(PasteBtn);

        FindBtn = new QToolButton(ContentContainer);
        FindBtn->setObjectName("FindBtn");
        FindBtn->setStyleSheet(QString::fromUtf8("\n"
"                QToolButton {\n"
"                    background: transparent;\n"
"                    border: none;\n"
"                    border-radius: 6px;\n"
"                    padding: 2px;\n"
"                }\n"
"                QToolButton:hover {\n"
"                    background: #e0e0e0;\n"
"                }\n"
"                QToolButton:pressed {\n"
"                    background: #b0b0b0;\n"
"                }\n"
"             "));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/res/icons/Find.svg"), QSize(), QIcon::Normal, QIcon::Off);
        FindBtn->setIcon(icon7);
        FindBtn->setIconSize(QSize(24, 24));

        toolBarLayout->addWidget(FindBtn);

        ToolBarSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        toolBarLayout->addItem(ToolBarSpacer);

        ChapterNameLabel = new QLabel(ContentContainer);
        ChapterNameLabel->setObjectName("ChapterNameLabel");
        ChapterNameLabel->setMinimumSize(QSize(0, 32));

        toolBarLayout->addWidget(ChapterNameLabel);


        rightVBox->addLayout(toolBarLayout);

        hLine = new QFrame(ContentContainer);
        hLine->setObjectName("hLine");
        hLine->setFrameShape(QFrame::HLine);
        hLine->setFrameShadow(QFrame::Sunken);

        rightVBox->addWidget(hLine);

        Content = new QPlainTextEdit(ContentContainer);
        Content->setObjectName("Content");

        rightVBox->addWidget(Content);

        mainSplitter->addWidget(ContentContainer);

        mainLayout->addWidget(mainSplitter);

        mainLayout->setStretch(1, 1);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 960, 24));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        ChapterNameLabel->setText(QCoreApplication::translate("MainWindow", "ChapterName", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
