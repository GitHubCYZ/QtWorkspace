#include "DrawItems.h"
#include <QPainter>
#include <QLinearGradient>
#include <QRadialGradient>
#include <QCursor>
#include <QDebug>
#include <QGraphicsEffect>


static QPainterPath qt_graphicsItem_shapeFromPath(const QPainterPath &path, const QPen &pen)
{
    // We unfortunately need this hack as QPainterPathStroker will set a width of 1.0
    // if we pass a value of 0.0 to QPainterPathStroker::setWidth()
    const qreal penWidthZero = qreal(0.00000001);

    if (path == QPainterPath() || pen == Qt::NoPen)
        return path;
    QPainterPathStroker ps;
    ps.setCapStyle(pen.capStyle());
    if (pen.widthF() <= 0.0)
        ps.setWidth(penWidthZero);
    else
        ps.setWidth(pen.widthF());
    ps.setJoinStyle(pen.joinStyle());
    ps.setMiterLimit(pen.miterLimit());
    QPainterPath p = ps.createStroke(path);
    p.addPath(path);
    return p;
}

//创建手柄的过程，和改变手柄状态的方法。
//创建手柄的过程可以在每个具体对象中实现，
//比如矩形有8个控制点，线有2个控制点，多边形控制点就是实际的点数。
GraphicsItem::GraphicsItem(QGraphicsItem *parent)
    :QAbstractGraphicsShapeItem(parent)
{
   // QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
   // effect->setBlurRadius(8);
   // setGraphicsEffect(effect);
}

//移动手柄位置。
void GraphicsItem::updateGeometry()
{
    const QRectF &geom = this->boundingRect();

    const int w = SELECTION_HANDLE_SIZE;
    const int h = SELECTION_HANDLE_SIZE;

    const Handles::iterator hend =  m_handles.end();
    for (Handles::iterator it = m_handles.begin(); it != hend; ++it) {
        SizeHandleRect *hndl = *it;;
        switch (hndl->Quadrant()) {
        case LeftTop:
            hndl->move(geom.x() - w / 2, geom.y() - h / 2);
            break;
        case Top:
            hndl->move(geom.x() + geom.width() / 2 - w / 2, geom.y() - h / 2);
            break;
        case RightTop:
            hndl->move(geom.x() + geom.width() - w / 2, geom.y() - h / 2);
            break;
        case Right:
            hndl->move(geom.x() + geom.width() - w / 2, geom.y() + geom.height() / 2 - h / 2);
            break;
        case RightBottom:
            hndl->move(geom.x() + geom.width() - w / 2, geom.y() + geom.height() - h / 2);
            break;
        case Bottom:
            hndl->move(geom.x() + geom.width() / 2 - w / 2, geom.y() + geom.height() - h / 2);
            break;
        case LeftBottom:
            hndl->move(geom.x() - w / 2, geom.y() + geom.height() - h / 2);
            break;
        case Left:
            hndl->move(geom.x() - w / 2, geom.y() + geom.height() / 2 - h / 2);
            break;
        case Center:
            hndl->move(geom.center().x()  - w / 2 , geom.center().y() - h / 2);
            break;
        default:
            break;
        }
    }

}

void GraphicsItem::setState(SelectionHandleState st)
{
    const Handles::iterator hend =  m_handles.end();
    for (Handles::iterator it = m_handles.begin(); it != hend; ++it)
        (*it)->SetState(st);
}

enumQuadrant GraphicsItem::hitTest(const QPointF &point) const
{
    const Handles::const_iterator hend =  m_handles.end();
    for (Handles::const_iterator it = m_handles.begin(); it != hend; ++it)
    {
        if ((*it)->hitTest(point) ){
            return (*it)->Quadrant();
        }
    }
    return None;
}

Qt::CursorShape GraphicsItem::getCursor(enumQuadrant qua)
{
    switch (qua) {
    case Right:
        return Qt::SizeHorCursor;
    case RightTop:
        return Qt::SizeBDiagCursor;
    case RightBottom:
        return Qt::SizeFDiagCursor;
    case LeftBottom:
        return Qt::SizeBDiagCursor;
    case Bottom:
        return Qt::SizeVerCursor;
    case LeftTop:
        return Qt::SizeFDiagCursor;
    case Left:
        return Qt::SizeHorCursor;
    case Top:
        return Qt::SizeVerCursor;
    default:
        break;
    }
    return Qt::ArrowCursor;
}

void GraphicsItem::resizeTo(enumQuadrant qua, const QPointF &point)
{
    Q_UNUSED(qua);
    Q_UNUSED(point);
}

void GraphicsItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    Q_UNUSED(event);
}

QVariant GraphicsItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if ( change == QGraphicsItem::ItemSelectedHasChanged ) {
        qDebug()<<" Item Selected : " << value.toString();
        setState(value.toBool() ? SelectionHandleActive : SelectionHandleOff);
    }else if ( change == QGraphicsItem::ItemRotationHasChanged ){
        qDebug()<<"Item Rotation Changed:" << value.toString();
    }else if ( change == QGraphicsItem::ItemTransformOriginPointHasChanged ){
        qDebug()<<"ItemTransformOriginPointHasChanged:" << value.toPointF();
    }
    return value;
}


GraphicsRectItem::GraphicsRectItem(const QRect & rect ,QGraphicsItem *parent)
    :GraphicsItem(parent)
    ,m_width(rect.width())
    ,m_height(rect.height())
{
    // handles
    m_handles.reserve(None);
    for (int i = LeftTop; i <= Left; ++i) {
        SizeHandleRect *shr = new SizeHandleRect(this, static_cast<enumQuadrant>(i), this);
        m_handles.push_back(shr);
    }
    updateGeometry();
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setAcceptHoverEvents(true);
}

QRectF GraphicsRectItem::boundingRect() const
{
    QRectF bounding ( -(m_width) / 2 ,
                      -(m_height) / 2 ,
                      m_width,m_height);
    return bounding;
}

QPainterPath GraphicsRectItem::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return qt_graphicsItem_shapeFromPath(path,pen());
}
//这个类里有两个方法比较重要，一个就是resizeTo，另外一个就是changeSize(),
//resizeTo就是改变对象的大小，为啥还要有个changeSize方法呢。这个问题后面说。
//先看看这两个函数的具体实现。
void GraphicsRectItem::resizeTo(enumQuadrant qua, const QPointF &point)
{
    Q_UNUSED(point);
    //将场景坐标映射为本地坐标。
    QPointF local = mapFromScene(point);
    QString dirName;

    //const QRectF &geom = this->boundingRect();
    //临时对象，记录改变后的大小。
    QRect delta = this->rect().toRect();
    switch (qua) {
    case Right:
        dirName = "Rigth";
        delta.setRight(local.x());
        break;
    case RightTop:
        dirName = "RightTop";
        delta.setTopRight(local.toPoint());
        break;
    case RightBottom:
        dirName = "RightBottom";
        delta.setBottomRight(local.toPoint());
        break;
    case LeftBottom:
        dirName = "LeftBottom";
        delta.setBottomLeft(local.toPoint());
        break;
    case Bottom:
        dirName = "Bottom";
        delta.setBottom(local.y());
        break;
    case LeftTop:
        dirName = "LeftTop";
        delta.setTopLeft(local.toPoint());
        break;
    case Left:
        dirName = "Left";
        delta.setLeft(local.x());
        break;
    case Top:
        dirName = "Top";
        delta.setTop(local.y());
        break;
   default:
        break;
    }

    //改变矩形的大小。
    prepareGeometryChange();

    m_width = delta.width();
    m_height = delta.height();
    //改变本地坐标系，这里本地坐标系已经被破坏了，
    //如果进行旋转操作等其他变换后，坐标系会出问题。
    //还需要调用changeSize来重建本地坐标。
    qDebug()<<"resizeTo :"<<dirName<<"move:"<<local<< "newSize :" <<QSize(m_width,m_height);
    updateGeometry();
}

void GraphicsRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QColor c = QColor(Qt::red);
    c.setAlpha(160);
    QLinearGradient result(rect().topLeft(), rect().topRight());
    result.setColorAt(0, c.dark(150));
    result.setColorAt(0.5, c.light(200));
    result.setColorAt(1, c.dark(150));
    painter->setPen(pen());
    painter->setBrush(result);
    painter->drawRect(rect());
    /*
    QColor c1 = QColor(Qt::blue);
    c1.setAlpha(180);
    painter->setBrush(c1);
    painter->drawEllipse(rect.topLeft().x() - 3 , rect.topLeft().y() - 3 ,6,6);
   */
}


GraphicsEllipseItem::GraphicsEllipseItem(const QRect &rect, QGraphicsItem *parent)
    :GraphicsRectItem(rect,parent)
{

}

QPainterPath GraphicsEllipseItem::shape() const
{
    QPainterPath path;
    path.addEllipse(boundingRect());
    return qt_graphicsItem_shapeFromPath(path,pen());
}

void GraphicsEllipseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QColor c = QColor(Qt::red);
    c.setAlpha(160);
    QRectF rc = rect();
    qreal radius = qMax(rc.width(),rc.height());
    QRadialGradient result(rc.center(),radius);
    result.setColorAt(0, c.light(200));
    result.setColorAt(0.5, c.dark(150));
    result.setColorAt(1, c);
    painter->setPen(pen());
    painter->setBrush(result);
    painter->drawEllipse(rc);
}




GraphicsLineItem::GraphicsLineItem(QGraphicsItem *parent):GraphicsRectItem(QRect(0,0,0,0),parent)
{
    // handles
    m_handles.reserve(None);
    Handles::iterator hend =  m_handles.end();
    for (Handles::iterator it = m_handles.begin(); it != hend; ++it)
        delete (*it);
    m_handles.clear();

    SizeHandleRect *shr = new SizeHandleRect(this,LeftTop, this);
    m_handles.push_back(shr);
    shr = new SizeHandleRect(this,RightBottom, this);
    m_handles.push_back(shr);

    updateGeometry();
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setAcceptHoverEvents(true);
}



QPainterPath GraphicsLineItem::shape() const
{
    QPainterPath path;
    path.moveTo(rect().topLeft());
    path.lineTo(rect().bottomRight());
    return qt_graphicsItem_shapeFromPath(path,pen());
}

void GraphicsLineItem::resizeTo(enumQuadrant qua, const QPointF &point)
{
    QPointF local = mapFromScene(point);
    prepareGeometryChange();

    //const QRectF &geom = this->boundingRect();
    QRect delta = this->rect().toRect();
    switch(qua)
    {
    case LeftTop:
        delta.setTopLeft(local.toPoint());
        break;
    case RightBottom:
        delta.setBottomRight(local.toPoint());
        break;
    default:
        break;
    }
    m_width = delta.width();
    m_height = delta.height();

    updateGeometry();
}


void GraphicsLineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    //QColor c = QColor(Qt::red);
    painter->setPen(pen());
    painter->drawLine(rect().topLeft(),rect().bottomRight());
}

GraphicsItemGroup::GraphicsItemGroup(QGraphicsItemGroup *group, QGraphicsItem *parent)
    :GraphicsRectItem(QRect(0,0,0,0),parent )
    ,m_group(group)
{
    if ( m_group ){
        m_group->setParentItem(this);
        QRectF bound = m_group->boundingRect();

        qDebug()<<" group rect :" << m_group->boundingRect() << " new Rect:" << bound;
        bound.normalized();
        prepareGeometryChange();
        m_width = bound.width();
        m_height = bound.height();

        QPointF delta = pos() - bound.center() ;

        setTransform(transform().translate(delta.x(),delta.y()));
        setTransformOriginPoint( bound.center());
        moveBy(-delta.x(),-delta.y());

        m_group->setTransformOriginPoint(bound.center());

        qDebug()<<"pos :" << delta;
        updateGeometry();
    }
}

QRectF GraphicsItemGroup::boundingRect() const
{

    QRectF bounding = m_group->mapRectToParent(m_group->boundingRect());

    return bounding;
}

QPainterPath GraphicsItemGroup::shape() const
{
    return m_group->shape() ;
}

void GraphicsItemGroup::resizeTo(enumQuadrant qua, const QPointF &point)
{
    Q_UNUSED(qua);
    Q_UNUSED(point);
}

void GraphicsItemGroup::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QPointF origin = mapFromScene(pos());
    QPointF origin1 = m_group->transformOriginPoint();

    QColor c1 = QColor(Qt::blue);
  //  c1.setAlpha(180);
    painter->setBrush(c1);
    painter->drawEllipse(origin.x() - 3 , origin.y() - 3 ,6,6);


    QColor c2 = QColor(Qt::green);
  //  c2.setAlpha(180);
    painter->setBrush(c2);
    painter->drawEllipse(origin1.x() - 3 , origin1.y() - 3 ,6,6);

}
