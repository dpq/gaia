/********************************************************************************
** Form generated from reading ui file 'redbook.ui'
**
** Created: Sun Jun 29 15:20:36 2008
**      by: Qt User Interface Compiler version 4.4.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_REDBOOK_H
#define UI_REDBOOK_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QListView>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStackedWidget>
#include <QtGui/QStatusBar>
#include <QtGui/QTextBrowser>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QGridLayout *gridLayout1;
    QHBoxLayout *hboxLayout;
    QLabel *logoLabel;
    QLabel *ministryLabel;
    QSpacerItem *spacerItem;
    QSpacerItem *spacerItem1;
    QWidget *widget;
    QVBoxLayout *vboxLayout;
    QLabel *indexLabel;
    QListView *indexList;
    QSpacerItem *spacerItem2;
    QSpacerItem *spacerItem3;
    QWidget *page_3;
    QHBoxLayout *hboxLayout1;
    QSpacerItem *spacerItem4;
    QWidget *widget_2;
    QGridLayout *gridLayout2;
    QListWidget *indexList2;
    QListWidget *chapterList;
    QTextBrowser *docViewer;
    QSpacerItem *spacerItem5;
    QWidget *page_2;
    QVBoxLayout *vboxLayout1;
    QLabel *label_4;
    QHBoxLayout *hboxLayout2;
    QGroupBox *groupBox;
    QVBoxLayout *vboxLayout2;
    QRadioButton *radioc0;
    QRadioButton *radioc1;
    QRadioButton *radioc2;
    QRadioButton *radioc3;
    QRadioButton *radioc4;
    QRadioButton *radioc5;
    QRadioButton *radioc6;
    QRadioButton *radioc7;
    QSpacerItem *spacerItem6;
    QVBoxLayout *vboxLayout3;
    QLabel *label_2;
    QTreeWidget *treeWidget;
    QVBoxLayout *vboxLayout4;
    QLabel *label_3;
    QHBoxLayout *hboxLayout3;
    QPushButton *latButton;
    QPushButton *rusButton;
    QSpacerItem *spacerItem7;
    QListWidget *listWidget_3;
    QWidget *page_4;
    QVBoxLayout *vboxLayout5;
    QGridLayout *gridLayout3;
    QLabel *label_7;
    QListWidget *listWidget_4;
    QSpacerItem *spacerItem8;
    QLabel *label_9;
    QSpacerItem *spacerItem9;
    QHBoxLayout *hboxLayout4;
    QVBoxLayout *vboxLayout6;
    QLabel *label_10;
    QLabel *label_11;
    QSpacerItem *spacerItem10;
    QTextBrowser *textBrowser_2;
    QSpacerItem *spacerItem11;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
    if (MainWindow->objectName().isEmpty())
        MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
    MainWindow->resize(906, 777);
    centralwidget = new QWidget(MainWindow);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    gridLayout = new QGridLayout(centralwidget);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    stackedWidget = new QStackedWidget(centralwidget);
    stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
    stackedWidget->setAutoFillBackground(false);
    page = new QWidget();
    page->setObjectName(QString::fromUtf8("page"));
    gridLayout1 = new QGridLayout(page);
    gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
    hboxLayout = new QHBoxLayout();
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    logoLabel = new QLabel(page);
    logoLabel->setObjectName(QString::fromUtf8("logoLabel"));

    hboxLayout->addWidget(logoLabel);

    ministryLabel = new QLabel(page);
    ministryLabel->setObjectName(QString::fromUtf8("ministryLabel"));

    hboxLayout->addWidget(ministryLabel);


    gridLayout1->addLayout(hboxLayout, 0, 0, 1, 3);

    spacerItem = new QSpacerItem(20, 111, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout1->addItem(spacerItem, 1, 1, 1, 1);

    spacerItem1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout1->addItem(spacerItem1, 2, 0, 1, 1);

    widget = new QWidget(page);
    widget->setObjectName(QString::fromUtf8("widget"));
    vboxLayout = new QVBoxLayout(widget);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    indexLabel = new QLabel(widget);
    indexLabel->setObjectName(QString::fromUtf8("indexLabel"));

    vboxLayout->addWidget(indexLabel);

    indexList = new QListView(widget);
    indexList->setObjectName(QString::fromUtf8("indexList"));

    vboxLayout->addWidget(indexList);


    gridLayout1->addWidget(widget, 2, 1, 2, 1);

    spacerItem2 = new QSpacerItem(151, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout1->addItem(spacerItem2, 3, 2, 1, 1);

    spacerItem3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout1->addItem(spacerItem3, 4, 1, 1, 1);

    stackedWidget->addWidget(page);
    page_3 = new QWidget();
    page_3->setObjectName(QString::fromUtf8("page_3"));
    hboxLayout1 = new QHBoxLayout(page_3);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    spacerItem4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout1->addItem(spacerItem4);

    widget_2 = new QWidget(page_3);
    widget_2->setObjectName(QString::fromUtf8("widget_2"));
    gridLayout2 = new QGridLayout(widget_2);
    gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
    indexList2 = new QListWidget(widget_2);
    indexList2->setObjectName(QString::fromUtf8("indexList2"));
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(indexList2->sizePolicy().hasHeightForWidth());
    indexList2->setSizePolicy(sizePolicy);

    gridLayout2->addWidget(indexList2, 0, 0, 1, 1);

    chapterList = new QListWidget(widget_2);
    chapterList->setObjectName(QString::fromUtf8("chapterList"));
    sizePolicy.setHeightForWidth(chapterList->sizePolicy().hasHeightForWidth());
    chapterList->setSizePolicy(sizePolicy);

    gridLayout2->addWidget(chapterList, 0, 1, 1, 1);

    docViewer = new QTextBrowser(widget_2);
    docViewer->setObjectName(QString::fromUtf8("docViewer"));

    gridLayout2->addWidget(docViewer, 1, 0, 1, 2);


    hboxLayout1->addWidget(widget_2);

    spacerItem5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout1->addItem(spacerItem5);

    stackedWidget->addWidget(page_3);
    page_2 = new QWidget();
    page_2->setObjectName(QString::fromUtf8("page_2"));
    vboxLayout1 = new QVBoxLayout(page_2);
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
    label_4 = new QLabel(page_2);
    label_4->setObjectName(QString::fromUtf8("label_4"));

    vboxLayout1->addWidget(label_4);

    hboxLayout2 = new QHBoxLayout();
    hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
    groupBox = new QGroupBox(page_2);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    vboxLayout2 = new QVBoxLayout(groupBox);
    vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
    radioc0 = new QRadioButton(groupBox);
    radioc0->setObjectName(QString::fromUtf8("radioc0"));

    vboxLayout2->addWidget(radioc0);

    radioc1 = new QRadioButton(groupBox);
    radioc1->setObjectName(QString::fromUtf8("radioc1"));

    vboxLayout2->addWidget(radioc1);

    radioc2 = new QRadioButton(groupBox);
    radioc2->setObjectName(QString::fromUtf8("radioc2"));

    vboxLayout2->addWidget(radioc2);

    radioc3 = new QRadioButton(groupBox);
    radioc3->setObjectName(QString::fromUtf8("radioc3"));

    vboxLayout2->addWidget(radioc3);

    radioc4 = new QRadioButton(groupBox);
    radioc4->setObjectName(QString::fromUtf8("radioc4"));

    vboxLayout2->addWidget(radioc4);

    radioc5 = new QRadioButton(groupBox);
    radioc5->setObjectName(QString::fromUtf8("radioc5"));

    vboxLayout2->addWidget(radioc5);

    radioc6 = new QRadioButton(groupBox);
    radioc6->setObjectName(QString::fromUtf8("radioc6"));

    vboxLayout2->addWidget(radioc6);

    radioc7 = new QRadioButton(groupBox);
    radioc7->setObjectName(QString::fromUtf8("radioc7"));

    vboxLayout2->addWidget(radioc7);

    spacerItem6 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout2->addItem(spacerItem6);


    hboxLayout2->addWidget(groupBox);

    vboxLayout3 = new QVBoxLayout();
    vboxLayout3->setObjectName(QString::fromUtf8("vboxLayout3"));
    label_2 = new QLabel(page_2);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    vboxLayout3->addWidget(label_2);

    treeWidget = new QTreeWidget(page_2);
    treeWidget->setObjectName(QString::fromUtf8("treeWidget"));

    vboxLayout3->addWidget(treeWidget);


    hboxLayout2->addLayout(vboxLayout3);

    vboxLayout4 = new QVBoxLayout();
    vboxLayout4->setObjectName(QString::fromUtf8("vboxLayout4"));
    label_3 = new QLabel(page_2);
    label_3->setObjectName(QString::fromUtf8("label_3"));

    vboxLayout4->addWidget(label_3);

    hboxLayout3 = new QHBoxLayout();
    hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
    latButton = new QPushButton(page_2);
    latButton->setObjectName(QString::fromUtf8("latButton"));

    hboxLayout3->addWidget(latButton);

    rusButton = new QPushButton(page_2);
    rusButton->setObjectName(QString::fromUtf8("rusButton"));

    hboxLayout3->addWidget(rusButton);

    spacerItem7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout3->addItem(spacerItem7);


    vboxLayout4->addLayout(hboxLayout3);

    listWidget_3 = new QListWidget(page_2);
    listWidget_3->setObjectName(QString::fromUtf8("listWidget_3"));

    vboxLayout4->addWidget(listWidget_3);


    hboxLayout2->addLayout(vboxLayout4);


    vboxLayout1->addLayout(hboxLayout2);

    stackedWidget->addWidget(page_2);
    page_4 = new QWidget();
    page_4->setObjectName(QString::fromUtf8("page_4"));
    vboxLayout5 = new QVBoxLayout(page_4);
    vboxLayout5->setObjectName(QString::fromUtf8("vboxLayout5"));
    gridLayout3 = new QGridLayout();
    gridLayout3->setObjectName(QString::fromUtf8("gridLayout3"));
    label_7 = new QLabel(page_4);
    label_7->setObjectName(QString::fromUtf8("label_7"));

    gridLayout3->addWidget(label_7, 0, 0, 1, 2);

    listWidget_4 = new QListWidget(page_4);
    listWidget_4->setObjectName(QString::fromUtf8("listWidget_4"));
    QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy1.setHorizontalStretch(2);
    sizePolicy1.setVerticalStretch(2);
    sizePolicy1.setHeightForWidth(listWidget_4->sizePolicy().hasHeightForWidth());
    listWidget_4->setSizePolicy(sizePolicy1);

    gridLayout3->addWidget(listWidget_4, 0, 2, 3, 1);

    spacerItem8 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout3->addItem(spacerItem8, 1, 0, 1, 1);

    label_9 = new QLabel(page_4);
    label_9->setObjectName(QString::fromUtf8("label_9"));

    gridLayout3->addWidget(label_9, 2, 0, 1, 1);

    spacerItem9 = new QSpacerItem(513, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout3->addItem(spacerItem9, 2, 1, 1, 1);


    vboxLayout5->addLayout(gridLayout3);

    hboxLayout4 = new QHBoxLayout();
    hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
    vboxLayout6 = new QVBoxLayout();
    vboxLayout6->setObjectName(QString::fromUtf8("vboxLayout6"));
    label_10 = new QLabel(page_4);
    label_10->setObjectName(QString::fromUtf8("label_10"));
    QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(label_10->sizePolicy().hasHeightForWidth());
    label_10->setSizePolicy(sizePolicy2);
    label_10->setPixmap(QPixmap(QString::fromUtf8("gaia/images/na.png")));

    vboxLayout6->addWidget(label_10);

    label_11 = new QLabel(page_4);
    label_11->setObjectName(QString::fromUtf8("label_11"));
    sizePolicy2.setHeightForWidth(label_11->sizePolicy().hasHeightForWidth());
    label_11->setSizePolicy(sizePolicy2);
    label_11->setPixmap(QPixmap(QString::fromUtf8("gaia/images/na.png")));

    vboxLayout6->addWidget(label_11);

    spacerItem10 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout6->addItem(spacerItem10);


    hboxLayout4->addLayout(vboxLayout6);

    textBrowser_2 = new QTextBrowser(page_4);
    textBrowser_2->setObjectName(QString::fromUtf8("textBrowser_2"));
    QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy3.setHorizontalStretch(1);
    sizePolicy3.setVerticalStretch(1);
    sizePolicy3.setHeightForWidth(textBrowser_2->sizePolicy().hasHeightForWidth());
    textBrowser_2->setSizePolicy(sizePolicy3);

    hboxLayout4->addWidget(textBrowser_2);


    vboxLayout5->addLayout(hboxLayout4);

    spacerItem11 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout5->addItem(spacerItem11);

    stackedWidget->addWidget(page_4);

    gridLayout->addWidget(stackedWidget, 0, 0, 1, 1);

    MainWindow->setCentralWidget(centralwidget);
    menubar = new QMenuBar(MainWindow);
    menubar->setObjectName(QString::fromUtf8("menubar"));
    menubar->setGeometry(QRect(0, 0, 906, 31));
    MainWindow->setMenuBar(menubar);
    statusbar = new QStatusBar(MainWindow);
    statusbar->setObjectName(QString::fromUtf8("statusbar"));
    MainWindow->setStatusBar(statusbar);

    retranslateUi(MainWindow);

    stackedWidget->setCurrentIndex(0);


    QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
    MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
    stackedWidget->setStyleSheet(QString());
    logoLabel->setText(QApplication::translate("MainWindow", "TextLabel", 0, QApplication::UnicodeUTF8));
    ministryLabel->setText(QApplication::translate("MainWindow", "\320\234\320\230\320\235\320\230\320\241\320\242\320\225\320\240\320\241\320\242\320\222\320\236 \320\237\320\240\320\230\320\240\320\236\320\224\320\235\320\253\320\245 \320\240\320\225\320\241\320\243\320\240\320\241\320\236\320\222 \320\240\320\236\320\241\320\241\320\230\320\231\320\241\320\232\320\236\320\231 \320\244\320\225\320\224\320\225\320\240\320\220\320\246\320\230\320\230. \320\240\320\236\320\241\320\241\320\230\320\231\320\241\320\232\320\220\320\257 \320\220\320\232\320\220\320\224\320\225\320\234\320\230\320\257 \320\235\320\220\320\243\320\232", 0, QApplication::UnicodeUTF8));
    widget->setStyleSheet(QApplication::translate("MainWindow", "background-color:#c00000", 0, QApplication::UnicodeUTF8));
    indexLabel->setText(QApplication::translate("MainWindow", "\320\241\320\236\320\224\320\225\320\240\320\226\320\220\320\235\320\230\320\225:", 0, QApplication::UnicodeUTF8));
    indexList->setStyleSheet(QApplication::translate("MainWindow", "background-color:#ffffff", 0, QApplication::UnicodeUTF8));
    widget_2->setStyleSheet(QApplication::translate("MainWindow", "background-color: #0080c0", 0, QApplication::UnicodeUTF8));
    label_4->setText(QApplication::translate("MainWindow", "\320\222\320\270\320\264\321\213, \320\267\320\260\320\275\320\265\321\201\320\265\320\275\320\275\321\213\320\265 \320\262 \320\232\321\200\320\260\321\201\320\275\321\203\321\216 \320\232\320\275\320\270\320\263\321\203 \320\240\320\276\321\201\321\201\320\270\320\271\321\201\320\272\320\276\320\271 \320\244\320\265\320\264\320\265\321\200\320\260\321\206\320\270\320\270", 0, QApplication::UnicodeUTF8));
    groupBox->setTitle(QApplication::translate("MainWindow", "\320\240\320\260\320\267\320\264\320\265\320\273\321\213", 0, QApplication::UnicodeUTF8));
    radioc0->setText(QApplication::translate("MainWindow", "\320\234\320\273\320\265\320\272\320\276\320\277\320\270\321\202\320\260\321\216\321\211\320\270\320\265", 0, QApplication::UnicodeUTF8));
    radioc1->setText(QApplication::translate("MainWindow", "\320\237\321\202\320\270\321\206\321\213", 0, QApplication::UnicodeUTF8));
    radioc2->setText(QApplication::translate("MainWindow", "\320\227\320\265\320\274\320\275\320\276\320\262\320\276\320\264\320\275\321\213\320\265 \320\270 \320\277\321\200\320\265\321\201\320\274\321\213\320\272\320\260\321\216\321\211\320\270\320\265\321\201\321\217", 0, QApplication::UnicodeUTF8));
    radioc3->setText(QApplication::translate("MainWindow", "\320\232\321\200\321\203\320\263\320\273\320\276\321\200\320\276\321\202\321\213\320\265 \320\270 \321\200\321\213\320\261\321\213", 0, QApplication::UnicodeUTF8));
    radioc4->setText(QApplication::translate("MainWindow", "\320\247\320\273\320\265\320\275\320\270\321\201\321\202\320\276\320\275\320\276\320\263\320\270\320\265", 0, QApplication::UnicodeUTF8));
    radioc5->setText(QApplication::translate("MainWindow", "\320\234\320\276\320\273\320\273\321\216\321\201\320\272\320\270", 0, QApplication::UnicodeUTF8));
    radioc6->setText(QApplication::translate("MainWindow", "\320\234\321\210\320\260\320\275\320\272\320\270 \320\270 \320\277\320\273\320\265\321\207\320\265\320\275\320\276\320\263\320\270\320\265", 0, QApplication::UnicodeUTF8));
    radioc7->setText(QApplication::translate("MainWindow", "\320\232\320\276\320\273\321\214\321\207\320\260\321\202\321\213\320\265 \321\207\320\265\321\200\320\262\320\270", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("MainWindow", "\320\241\320\270\321\201\321\202\320\265\320\274\320\260\321\202\320\270\321\207\320\265\321\201\320\272\320\270\320\271 \321\201\320\277\320\270\321\201\320\276\320\272 \320\262\320\270\320\264\320\276\320\262:", 0, QApplication::UnicodeUTF8));
    treeWidget->headerItem()->setText(0, QApplication::translate("MainWindow", "1", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("MainWindow", "\320\220\320\273\321\204\320\260\320\262\320\270\321\202\320\275\321\213\320\271 \321\203\320\272\320\260\320\267\320\260\321\202\320\265\320\273\321\214:", 0, QApplication::UnicodeUTF8));
    latButton->setText(QApplication::translate("MainWindow", "\320\233\320\260\321\202", 0, QApplication::UnicodeUTF8));
    rusButton->setText(QApplication::translate("MainWindow", "\320\240\321\203\321\201", 0, QApplication::UnicodeUTF8));
    label_7->setText(QApplication::translate("MainWindow", "TextLabel", 0, QApplication::UnicodeUTF8));
    label_9->setText(QApplication::translate("MainWindow", "TextLabel", 0, QApplication::UnicodeUTF8));
    label_10->setText(QString());
    label_11->setText(QString());
    Q_UNUSED(MainWindow);
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REDBOOK_H
