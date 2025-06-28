/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action514;
    QWidget *centralwidget;
    QFrame *Dock;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QStackedWidget *SideBar;
    QWidget *page;
    QWidget *page_2;
    QFrame *line;
    QPlainTextEdit *Content;
    QFrame *ToolBar;
    QToolButton *UndoBtn;
    QToolButton *RedoBtn;
    QToolButton *CopyBtn;
    QToolButton *CutBtn;
    QToolButton *PasteBtn;
    QToolButton *FindBtn;
    QLabel *label;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(960, 540);
        MainWindow->setMinimumSize(QSize(960, 540));
        MainWindow->setMaximumSize(QSize(960, 16777215));
        action514 = new QAction(MainWindow);
        action514->setObjectName("action514");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        Dock = new QFrame(centralwidget);
        Dock->setObjectName("Dock");
        Dock->setGeometry(QRect(0, 0, 96, 480));
        Dock->setMinimumSize(QSize(96, 480));
        Dock->setFrameShape(QFrame::StyledPanel);
        Dock->setFrameShadow(QFrame::Raised);
        scrollArea = new QScrollArea(Dock);
        scrollArea->setObjectName("scrollArea");
        scrollArea->setGeometry(QRect(0, 0, 96, 81));
        scrollArea->setMinimumSize(QSize(96, 0));
        scrollArea->setMaximumSize(QSize(96, 16777215));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName("scrollAreaWidgetContents");
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 94, 79));
        scrollArea->setWidget(scrollAreaWidgetContents);
        SideBar = new QStackedWidget(centralwidget);
        SideBar->setObjectName("SideBar");
        SideBar->setGeometry(QRect(100, 0, 200, 480));
        SideBar->setMinimumSize(QSize(200, 480));
        SideBar->setMaximumSize(QSize(400, 16777215));
        page = new QWidget();
        page->setObjectName("page");
        SideBar->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName("page_2");
        SideBar->addWidget(page_2);
        line = new QFrame(centralwidget);
        line->setObjectName("line");
        line->setGeometry(QRect(290, 0, 16, 491));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);
        Content = new QPlainTextEdit(centralwidget);
        Content->setObjectName("Content");
        Content->setGeometry(QRect(300, 30, 661, 461));
        ToolBar = new QFrame(centralwidget);
        ToolBar->setObjectName("ToolBar");
        ToolBar->setGeometry(QRect(300, 0, 661, 31));
        ToolBar->setFrameShape(QFrame::StyledPanel);
        ToolBar->setFrameShadow(QFrame::Raised);
        UndoBtn = new QToolButton(ToolBar);
        UndoBtn->setObjectName("UndoBtn");
        UndoBtn->setGeometry(QRect(0, 0, 32, 32));
        QIcon icon(QIcon::fromTheme(QString::fromUtf8("QIcon::ThemeIcon::EditUndo")));
        UndoBtn->setIcon(icon);
        RedoBtn = new QToolButton(ToolBar);
        RedoBtn->setObjectName("RedoBtn");
        RedoBtn->setGeometry(QRect(32, 0, 32, 32));
        QIcon icon1(QIcon::fromTheme(QString::fromUtf8("QIcon::ThemeIcon::EditRedo")));
        RedoBtn->setIcon(icon1);
        CopyBtn = new QToolButton(ToolBar);
        CopyBtn->setObjectName("CopyBtn");
        CopyBtn->setGeometry(QRect(64, 0, 32, 32));
        QIcon icon2(QIcon::fromTheme(QString::fromUtf8("QIcon::ThemeIcon::EditCopy")));
        CopyBtn->setIcon(icon2);
        CutBtn = new QToolButton(ToolBar);
        CutBtn->setObjectName("CutBtn");
        CutBtn->setGeometry(QRect(96, 0, 32, 32));
        QIcon icon3(QIcon::fromTheme(QString::fromUtf8("QIcon::ThemeIcon::EditCut")));
        CutBtn->setIcon(icon3);
        PasteBtn = new QToolButton(ToolBar);
        PasteBtn->setObjectName("PasteBtn");
        PasteBtn->setGeometry(QRect(128, 0, 32, 32));
        QIcon icon4(QIcon::fromTheme(QString::fromUtf8("QIcon::ThemeIcon::EditPaste")));
        PasteBtn->setIcon(icon4);
        FindBtn = new QToolButton(ToolBar);
        FindBtn->setObjectName("FindBtn");
        FindBtn->setGeometry(QRect(160, 0, 32, 32));
        QIcon icon5(QIcon::fromTheme(QString::fromUtf8("QIcon::ThemeIcon::EditFind")));
        FindBtn->setIcon(icon5);
        label = new QLabel(ToolBar);
        label->setObjectName("label");
        label->setGeometry(QRect(224, 0, 256, 32));
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
        action514->setText(QCoreApplication::translate("MainWindow", "514", nullptr));
        UndoBtn->setText(QString());
        RedoBtn->setText(QString());
        CopyBtn->setText(QString());
        CutBtn->setText(QString());
        PasteBtn->setText(QString());
        FindBtn->setText(QString());
        label->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
