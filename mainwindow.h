#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QPushButton>
#include <QTextStream>
#include <QInputDialog>
#include <QFileDialog>
#include <QGraphicsItem>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QDomNode>
#include <QPrinter>

#include "mbaseview.h"
#include "mbasescene.h"

#include "mtextobject.h"
#include "mlineobject.h"
#include "mrectobject.h"
#include "mellipseobject.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void itemDockWidgetInit(void);
    void tabWidgetInit(void);
    void xmlFileInit(QDomDocument *doc);
    bool appendObj2Xml(QDomDocument &doc, const QString &cls, const QDomElement &new_node);
    bool xmlIsExist(const QString &name);

protected:

    void closeEvent(QCloseEvent *event);

private:

    void savePage(const QString &file);
    void openPage();

private slots:
    void slotBtnText(void);
    void slotBtnLine(void);
    void slotBtnRect(void);
    void slotBtnEllipse(void);

    void slotActNew(void);
    void slotActOpen(void);
    void slotActSave(void);
    void slotActSaveAs(void);
    void slotActPrintImage(void);
    void slotActPrintPDF(void);

    void slotActLight(void);
    void slotActDark(void);

    /* Grid */
    void slotAct1Pixel(void);
    void slotAct2Pixel(void);
    void slotAct5Pixel(void);
    void slotAct10Pixel(void);

private:
    Ui::MainWindow *ui;
    MBaseView *curView;

    const QString defaultXmlPath = "./xml/";



};

#endif // MAINWINDOW_H
