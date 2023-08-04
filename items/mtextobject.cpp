#include "mtextobject.h"

#include <QInputDialog>
#include <QDir>

MTextObject::MTextObject(MBaseObject *parent)
    : MBaseObject(parent)
    , mText("Text")
{
    mFont.setPixelSize(8);
}

MTextObject::MTextObject(const QString &text, MBaseObject *parent)
    : MBaseObject(parent)
    , mText(text)
{

}

MTextObject &MTextObject::operator=(const MTextObject &obj)
{
    this->mFont = obj.mFont;
    this->mText = obj.mText;
    this->IS_Edit = obj.IS_Edit;
    return *this;
}

MTextObject::~MTextObject()
{

}

QRectF MTextObject::boundingRect() const
{
    int size = mFont.pixelSize();
    int count = mText.size();
    return QRectF(-size/2*count/2,-size/2,size*count/2,size);
}

void MTextObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget); Q_UNUSED(option);

    painter->setRenderHint(QPainter::TextAntialiasing, true);
    painter->setPen(gPen);
    painter->setFont(mFont);
    painter->drawText(this->boundingRect(), mText);

    if(this->MBaseObject::isSelected())
        drawSelectedRect(painter);
}

void MTextObject::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    bool ok;
    QString text = QInputDialog::getText(nullptr, nullptr,
                                        tr("Input:"), QLineEdit::Normal,nullptr, &ok);
    if (ok && !text.isEmpty())
    {
        mText = text;
        update();
    }
}


