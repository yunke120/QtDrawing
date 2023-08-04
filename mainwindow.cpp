#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QtMath>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Qt Drawing");

    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::slotActNew);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::slotActOpen);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::slotActSave);
    connect(ui->actionSaveAs, &QAction::triggered, this, &MainWindow::slotActSaveAs);
    connect(ui->actionExImage, &QAction::triggered, this, &MainWindow::slotActPrintImage);
    connect(ui->actionExPDF, &QAction::triggered, this, &MainWindow::slotActPrintPDF);

    connect(ui->actionLight, &QAction::triggered, this, &MainWindow::slotActLight);
    connect(ui->actionDark, &QAction::triggered, this, &MainWindow::slotActDark);

    connect(ui->action1_pixel, &QAction::triggered, this, &MainWindow::slotAct1Pixel);
    connect(ui->action2_pixel, &QAction::triggered, this, &MainWindow::slotAct2Pixel);
    connect(ui->action5_pixel, &QAction::triggered, this, &MainWindow::slotAct5Pixel);
    connect(ui->action10_pixel, &QAction::triggered, this, &MainWindow::slotAct10Pixel);

    itemDockWidgetInit();
    tabWidgetInit();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::itemDockWidgetInit()
{
    QFile file(":/Resources/qss/style.qss");
    if(!file.open(QFile::ReadOnly)) return;
    QTextStream textStream(&file);
    QString stylesheet = textStream.readAll();
    this->setStyleSheet(stylesheet);
    file.close();

    connect(ui->btnText, &QPushButton::clicked, this, &MainWindow::slotBtnText);
    connect(ui->btnLine, &QPushButton::clicked, this, &MainWindow::slotBtnLine);
    connect(ui->btnRect, &QPushButton::clicked, this, &MainWindow::slotBtnRect);
    connect(ui->btnEllipse, &QPushButton::clicked, this, &MainWindow::slotBtnEllipse);
}

void MainWindow::tabWidgetInit()
{
    ui->tabWidget->setTabsClosable(true);

    connect(ui->tabWidget, &QTabWidget::currentChanged, this, [=](int index)
    {
        Q_UNUSED(index);
        curView = dynamic_cast<MBaseView *>(ui->tabWidget->currentWidget());
    });

    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, this, [=](int index){
        QMessageBox msgBox;
        msgBox.setText("The document has changed. Whether to save the changes?");

        QPushButton *save = msgBox.addButton("Save", QMessageBox::ActionRole);
        QPushButton *unsave = msgBox.addButton("Unsave", QMessageBox::ActionRole);
        QPushButton *cancel = msgBox.addButton("Cancel", QMessageBox::ActionRole);
        Q_UNUSED(cancel);

        msgBox.exec();

        if(msgBox.clickedButton() == save)
        {
            slotActSave();
            ui->tabWidget->removeTab(index);
        }
        else if (msgBox.clickedButton() == unsave) {
            ui->tabWidget->removeTab(index);
        }
    });


}

void MainWindow::xmlFileInit(QDomDocument *doc)
{
    /* 创建并添加文档声明 */
    QDomProcessingInstruction  docStatement =
    doc->createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    doc->appendChild(docStatement);

    QDomElement root;   /* 根节点 */
    QDomElement header; /* 标题 */
    QDomElement _class; /* 根节点下的class属性 */
    QDomComment comment;/* 注释 */

    root = doc->createElement("root");
    doc->appendChild(root);

    header = doc->createElement("header"); /* 添加窗口标题 */
    header.appendChild(doc->createTextNode(ui->tabWidget->tabText(ui->tabWidget->currentIndex())));
    root.appendChild(header);

    comment = doc->createComment("The following are rect primitives");
    root.appendChild(comment);
    _class = doc->createElementNS("rect", "class");
    root.appendChild(_class);

    comment = doc->createComment("The following are line primitives");
    root.appendChild(comment);
    _class = doc->createElementNS("line", "class");
    root.appendChild(_class);

    comment = doc->createComment("The following are text primitives");
    root.appendChild(comment);
    _class = doc->createElementNS("text", "class");
    root.appendChild(_class);

    comment = doc->createComment("The following are ellipse primitives");
    root.appendChild(comment);
    _class = doc->createElementNS("ellipse", "class");
    root.appendChild(_class);


}

bool MainWindow::appendObj2Xml(QDomDocument &doc, const QString &cls, const QDomElement &new_node)
{
    QString nsURL = cls;
    QDomNodeList nodelist = doc.elementsByTagNameNS(nsURL.toLower(), "class");
    QDomNode node = nodelist.at(0);

    QDomElement classElement;
    if(node.isElement())
        classElement = node.toElement();

    if(classElement.appendChild(new_node).isNull())
        return false;
    return true;
}

bool MainWindow::xmlIsExist(const QString &name)
{
    QDir dir(defaultXmlPath);
    QStringList nameFilters;
    nameFilters << "*.xml";
    QStringList files = dir.entryList(nameFilters, QDir::Files | QDir::Readable, QDir::Time);
    return files.contains(name);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(ui->tabWidget->count() == 0)
    {
        qApp->quit();
        return;
    }

    QMessageBox msgBox;
    msgBox.setIconPixmap(QPixmap(":/Resources/images/close.png"));
    QPushButton *sBtn = msgBox.addButton("Save and Close(S)",QMessageBox::ActionRole);
    QPushButton *yBtn = msgBox.addButton("Close(Y)",QMessageBox::ActionRole);
    QPushButton *nBtn = msgBox.addButton("Cancel(F)",QMessageBox::ActionRole);
    Q_UNUSED(nBtn);
    msgBox.setText("Whether you want to close the current tag or all tags？");

    msgBox.exec();
    if(msgBox.clickedButton() == sBtn)
    {
        slotActSave();
        qApp->quit();
    }
    if(msgBox.clickedButton() == yBtn)
    {
        qApp->quit();
    }
    else {
        event->ignore();
    }

}

void MainWindow::savePage(const QString &filename)
{
    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        qDebug() << "Save failed";
        return;
    }
    QDomDocument domDoc;
    xmlFileInit(&domDoc);

    /* 图元的保存
     * 获取当前Scene中所有的items
     */
    QList<QGraphicsItem *> items = curView->scene()->items();

    int size = items.size();
    MBaseObject *obj = nullptr;

    for (int i = 0; i < size; i++) {
        obj = qgraphicsitem_cast<MBaseObject*>(items[i]);
        QPointF pos = obj->scenePos();

        QString scale = QString::number(obj->scale());
        QString rotation = QString::number(obj->rotation());
        QString penwidth = QString::number(obj->gPen.widthF());

        QDomElement newNode = domDoc.createElement("object");

        QDomElement scaleNode = domDoc.createElement("scale");
        QDomText scaleText = domDoc.createTextNode(scale);
        scaleNode.appendChild(scaleText);

        QDomElement xNode = domDoc.createElement("x");
        QDomText xText = domDoc.createTextNode(QString::number(pos.x()));
        xNode.appendChild(xText);

        QDomElement yNode = domDoc.createElement("y");
        QDomText yText = domDoc.createTextNode(QString::number(pos.y()));
        yNode.appendChild(yText);

        QDomElement rotationNode = domDoc.createElement("rotation");
        QDomText rotationText = domDoc.createTextNode(rotation);
        rotationNode.appendChild(rotationText);

        QDomElement penwidthNode = domDoc.createElement("penwidth");
        QDomText penwidthText = domDoc.createTextNode(penwidth);
        penwidthNode.appendChild(penwidthText);

        newNode.appendChild(xNode);
        newNode.appendChild(yNode);
        newNode.appendChild(scaleNode);
        newNode.appendChild(rotationNode);
        newNode.appendChild(penwidthNode);

        switch (obj->objType()) {
        case MBaseObject::Text:
        {
            MTextObject *textObj = qgraphicsitem_cast<MTextObject*>(obj);
            QDomElement textNode = domDoc.createElement("text");
            QDomText textText = domDoc.createTextNode(textObj->text());
            textNode.appendChild(textText);
            newNode.appendChild(textNode);
        }break;
        case MBaseObject::Line:
        {
            MLineObject *lineObj = qgraphicsitem_cast<MLineObject*>(obj);

            QDomElement sxNode = domDoc.createElement("sx");
            QDomText sxText = domDoc.createTextNode(QString::number(lineObj->startPoint().x()));
            sxNode.appendChild(sxText);

            QDomElement syNode = domDoc.createElement("sy");
            QDomText syText = domDoc.createTextNode(QString::number(lineObj->startPoint().y()));
            syNode.appendChild(syText);

            QDomElement exNode = domDoc.createElement("ex");
            QDomText exText = domDoc.createTextNode(QString::number(lineObj->endPoint().x()));
            exNode.appendChild(exText);

            QDomElement eyNode = domDoc.createElement("ey");
            QDomText eyText = domDoc.createTextNode(QString::number(lineObj->endPoint().y()));
            eyNode.appendChild(eyText);

            newNode.appendChild(sxNode);
            newNode.appendChild(syNode);
            newNode.appendChild(exNode);
            newNode.appendChild(eyNode);
        }break;
        default:break;
        }

        appendObj2Xml(domDoc, obj->nameString(), newNode);
    }

    QTextStream stream(&file);
    domDoc.save(stream, 4);
    file.close();
}

void MainWindow::openPage()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open A XML File"
                                                        , "./xml", "Xml(*.xml)");

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Open failed";
        return;
    }

    QXmlStreamReader reader(&file);
    QXmlStreamReader::TokenType token;
    QString element, nsUrl, textStr;
    /* 以下是xml文件中的属性 */
    qreal x, y, penwidth, scale, rotation;
    qreal sx,sy,ex,ey;
    QString text;
    x = 0;
    y = 0;
    penwidth = 0;
    scale = 0;
    rotation = 0;
    sx = 0;
    sy = 0;
    ex = 0;
    ey = 0;

    MBaseObject::OBJTYPE type = MBaseObject::Base; /* class类型 */
    bool bStartDrawObjFlag = false; /* 开始绘制一个object标志 */

    while (!reader.atEnd()) {
        token = reader.readNext();
        if(token == QXmlStreamReader::StartElement)
        {
            element = reader.name().toString();
            if(QString::compare(element, "header") == 0) /* 创建一个View窗口 */
            {
                textStr = reader.readElementText();
                MBaseScene *pScene = new MBaseScene(this);
                pScene->setSceneRect(-297*3/2, -210*3/2, 297*3, 210*3);
                MBaseView *view = new MBaseView(pScene);
                ui->tabWidget->addTab(view, textStr);
                curView = view;
            }
            else if(QString::compare(element, "class") == 0) /* 获取类 */
            {
                nsUrl = reader.namespaceUri().toString();
                if(nsUrl == "rect")
                    type = MBaseObject::Rect;
                else if (nsUrl == "line")
                    type = MBaseObject::Line;
                else if (nsUrl == "text")
                    type = MBaseObject::Text;
                else if (nsUrl == "ellipse")
                    type = MBaseObject::Ellipse;
            }
            else if(QString::compare(element, "object") == 0) /* 获取对象 */
            {
                bStartDrawObjFlag = true;
            }
            else if(QString::compare(element, "x") == 0) /* 对象的x属性 */
            {
                textStr = reader.readElementText();
                x = textStr.toDouble();
            }
            else if(QString::compare(element, "y") == 0) /* 对象的y属性 */
            {
                textStr = reader.readElementText();
                y = textStr.toDouble();
            }
            else if(QString::compare(element, "scale") == 0) /* 对象的scale属性 */
            {
                textStr = reader.readElementText();
                scale = textStr.toDouble();
            }
            else if(QString::compare(element, "rotation") == 0) /* 对象的rotation属性 */
            {
                textStr = reader.readElementText();
                rotation = textStr.toDouble();
            }
            else if(QString::compare(element, "penwidth") == 0) /* 对象的penwidth属性 */
            {
                textStr = reader.readElementText();
                penwidth = textStr.toDouble();
            }
            else if(QString::compare(element, "text") == 0) /* 对象的text属性 */
            {
                textStr = reader.readElementText();
                text = textStr;
            }
            else if(QString::compare(element, "sx") == 0) /* 对象的sx属性 */
            {
                textStr = reader.readElementText();
                sx = textStr.toDouble();
            }
            else if(QString::compare(element, "sy") == 0) /* 对象的sy属性 */
            {
                textStr = reader.readElementText();
                sy = textStr.toDouble();
            }
            else if(QString::compare(element, "ex") == 0) /* 对象的ex属性 */
            {
                textStr = reader.readElementText();
                ex = textStr.toDouble();
            }
            else if(QString::compare(element, "ey") == 0) /* 对象的ey属性 */
            {
                textStr = reader.readElementText();
                ey = textStr.toDouble();
            }
        }
        else if(token == QXmlStreamReader::EndElement)
        {
            if(bStartDrawObjFlag)
            {
                bStartDrawObjFlag = false;
                switch (type) {
                case MBaseObject::Rect:
                {
                    MRectObject *rectObj = new MRectObject();
                    rectObj->setScale(scale);
                    rectObj->setRotation(rotation);
                    rectObj->gPen.setWidthF(penwidth);
                    rectObj->setPos(x, y);
                    curView->scene()->addItem(rectObj);
                }break;
                case MBaseObject::Line:
                {
                    MLineObject *lineObj = new MLineObject();
                    lineObj->setScale(scale);
                    lineObj->setRotation(rotation);
                    lineObj->gPen.setWidthF(penwidth);
                    lineObj->setStartPoint(QPointF(sx, sy));
                    lineObj->setEndPoint(QPointF(ex, ey));
                    lineObj->setPos(x, y);
                    curView->scene()->addItem(lineObj);
                }break;
                case MBaseObject::Text:
                {
                    MTextObject *textObj = new MTextObject();
                    textObj->setScale(scale);
                    textObj->setRotation(rotation);
                    textObj->gPen.setWidthF(penwidth);
                    textObj->setText(text);
                    textObj->setPos(x, y);
                    curView->scene()->addItem(textObj);
                }break;
                case MBaseObject::Ellipse:
                {
                    MEllipseObject *ellipseObj = new MEllipseObject();
                    ellipseObj->setScale(scale);
                    ellipseObj->setRotation(rotation);
                    ellipseObj->gPen.setWidthF(penwidth);
                    ellipseObj->setPos(x, y);
                    curView->scene()->addItem(ellipseObj);
                }break;
                default:break;
                }
            }
        }
    }
}

void MainWindow::slotBtnText()
{
    if(ui->tabWidget->count() == 0) return;
    MTextObject *item = new MTextObject();
    item->setPos(curView->gEndPoint);
    curView->scene()->addItem(item);
}

void MainWindow::slotBtnLine()
{
    if(ui->tabWidget->count() == 0) return;
    curView->enbaleDrawLine(true);
}

void MainWindow::slotBtnRect()
{
    if(ui->tabWidget->count() == 0) return;
    MBaseObject *item = new MRectObject();
    item->setPos(curView->gEndPoint);
    curView->scene()->addItem(item);
}

void MainWindow::slotBtnEllipse()
{
    if(ui->tabWidget->count() == 0) return;
    MBaseObject *item = new MEllipseObject();
    item->setPos(curView->gEndPoint);
    curView->scene()->addItem(item);
}

void MainWindow::slotActNew()
{
    static int count = 0;
    QString text = "untitled_" + QString::number(count);
    count ++;

    MBaseScene *pScene = new MBaseScene(this);
    pScene->setSceneRect(-297*3/2, -210*3/2, 297*3, 210*3);
    MBaseView *view = new MBaseView(pScene);
    ui->tabWidget->addTab(view, text);
    ui->tabWidget->setCurrentWidget(view);
    curView = view;
}

void MainWindow::slotActOpen()
{
    openPage();
}

void MainWindow::slotActSave()
{
    if(ui->tabWidget->count() == 0) return;
    int index = ui->tabWidget->currentIndex();
    QString name = ui->tabWidget->tabText(index) + ".xml";
    QString pathname = defaultXmlPath + name;
    QString fileName;
    if(!xmlIsExist(name))
    {
        fileName = QFileDialog::getSaveFileName(this, "Save File",
                                                        pathname,"Xml(*.xml)");
        qDebug() << fileName;
        QString title = fileName.split('/').last().split('.').first();
        ui->tabWidget->setTabText(index, title);
    }
    else {
        fileName = pathname;
    }

    savePage(fileName);
}

void MainWindow::slotActSaveAs()
{
    if(ui->tabWidget->count() == 0) return;
    int index = ui->tabWidget->currentIndex();
    QString name = ui->tabWidget->tabText(index) + ".xml";
    QString pathname = defaultXmlPath + name;
    QString fileName;

    fileName = QFileDialog::getSaveFileName(this, "Save File",
                                                        pathname,"Xml(*.xml)");

    QString title = fileName.split('/').last().split('.').first();
    ui->tabWidget->setTabText(index, title);

    savePage(fileName);
}

void MainWindow::slotActPrintImage()
{
    if(ui->tabWidget->count() == 0) return;
    int index = ui->tabWidget->currentIndex();
    QString name = ui->tabWidget->tabText(index) + ".png";
    QString pathname = defaultXmlPath + name;
    QString fileName;

    fileName = QFileDialog::getSaveFileName(this,
                                            "Save File",
                                             pathname,
                                            "PNG (*.png);;JPEG (*.jpg *.jpeg);;BMP (*.bmp);;All files (*.*)");



    QPixmap pixmap(qCeil(curView->scene()->sceneRect().width()),
                       qCeil(curView->scene()->sceneRect().height()));
    pixmap.fill(Qt::white);
    QPainter painter(&pixmap);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing
                         | QPainter::HighQualityAntialiasing | QPainter::LosslessImageRendering);
    curView->scene()->render(&painter);
    painter.end();

    if (fileName == "" && pixmap.isNull())
    {
        qDebug() << "Error!";
        return;
    }

    pixmap.save(fileName);
    QMessageBox::information(this, nullptr, "Export image success");
}

void MainWindow::slotActPrintPDF()
{
    if(ui->tabWidget->count() == 0) return;
    int index = ui->tabWidget->currentIndex();
    QString name = ui->tabWidget->tabText(index) + ".pdf";
    QString pathname = defaultXmlPath + name;
    QString fileName;

    fileName = QFileDialog::getSaveFileName(this,
                                            "Save File",
                                             pathname,
                                            "PDF (*.pdf);");
    QPrinter printer(QPrinter::HighResolution);
    printer.setPageSize(QPrinter::A4);
    printer.setPageOrientation(QPageLayout::Landscape);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    QPainter painter;
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing
                           | QPainter::HighQualityAntialiasing | QPainter::LosslessImageRendering);
    if(!painter.begin(&printer))
    {
        qDebug() << "Error!";
        return;
    }
    curView->scene()->render(&painter);
    painter.end();

    QMessageBox::information(this, nullptr, "Export PDF success");

}

void MainWindow::slotActLight()
{
     /* Scene的背景色 */
    MBaseScene::gBackgroundColor = QColor("#f2f4f6");
    MBaseScene::gForegroundColorDark = QColor("#ceefe4");
    MBaseScene::gForegroundColorLight = QColor("#f4f0e6");
    /* 图元绘制的颜色 */
    MBaseObject::gObjectColor = Qt::black;
    MBaseObject::gBorderColor = QColor("#260033");

    MBaseView::gXYLineColor = QColor("#FE8402");

}

void MainWindow::slotActDark()
{
    /* Scene的背景色 */
    MBaseScene::gBackgroundColor = QColor("#393939");
    MBaseScene::gForegroundColorDark = QColor("#292929");
    MBaseScene::gForegroundColorLight = QColor("#2f2f2f");
    /* 图元绘制的颜色 */
    MBaseObject::gObjectColor = Qt::white;
    MBaseObject::gBorderColor = QColor(8,255,200);

    MBaseView::gXYLineColor = QColor(0,255,255);
}

/* Grid */
void MainWindow::slotAct1Pixel()
{
    MBaseScene::gGridSize = 20;
    MBaseScene::gGridSquares = 5;
    MBaseObject::gMoveOffset = 20;
}

void MainWindow::slotAct2Pixel()
{
    MBaseScene::gGridSize = 2;
    MBaseScene::gGridSquares = 5;
    MBaseObject::gMoveOffset = 2;
}

void MainWindow::slotAct5Pixel()
{
    MBaseScene::gGridSize = 5;
    MBaseScene::gGridSquares = 5;
    MBaseObject::gMoveOffset = 5;
}

void MainWindow::slotAct10Pixel()
{
    MBaseScene::gGridSize = 10;
    MBaseScene::gGridSquares = 5;
    MBaseObject::gMoveOffset = 10;
}

