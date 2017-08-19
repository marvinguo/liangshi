#include "shapeFactory.h"
#include "shapeType.h"
#include <QRect>
#include <QPainterPath>

class Point: public Shape
{
public:
    Point()
        : Shape(point), m_bPressed(false)
    {}
    ~Point(){}

    bool isEmpty(){ return m_bPressed; }

    void paint(QPainter *p)
    {
        p->drawPoint(m_pos);
    }

    bool mousePress(QMouseEvent* e)
    {
        m_pos = e->pos();
        m_bPressed = true;
        return false;
    }

    bool mouseMove(QMouseEvent* e)
    {
        m_pos = e->pos();
        return true;
    }

    bool mouseRelease(QMouseEvent* e)
    {
        return mouseMove(e);
    }

    bool m_bPressed;
    QPoint m_pos;
};

class Line: public Shape
{
public:
    Line(): Shape(line)
    {}
    ~Line(){}

    bool isEmpty()
    {
        return m_startPos == m_endPos;
    }

    void paint(QPainter *p)
    {
        p->drawLine(m_startPos, m_endPos);
    }

    bool mousePress(QMouseEvent *e)
    {
        m_startPos = e->pos();
        return false;
    }

    bool mouseMove(QMouseEvent *e)
    {
        m_endPos = e->pos();
        return true;
    }

    bool mouseRelease(QMouseEvent *e)
    {
        return mouseMove(e);
    }

    QPoint m_startPos;
    QPoint m_endPos;
};

class Rectangle: public Shape
{
public:
    Rectangle(int shapeType = rectangle): Shape(shapeType)
    {}
    ~Rectangle(){}

    bool isEmpty()
    {
        return m_point1 == m_point2;
    }

    void paint(QPainter *p)
    {
        p->drawRect(m_rect);
    }

    bool mousePress(QMouseEvent *e)
    {
        m_point1 = e->pos();
        return false;
    }

    bool mouseMove(QMouseEvent *e)
    {
        m_point2 = e->pos();

        int pt1x = m_point1.rx();
        int pt1y = m_point1.ry();
        int pt2x = m_point2.rx();
        int pt2y = m_point2.ry();
        if(pt1x <= pt2x)
        {
            if(pt1y <= pt2y)
            {
                m_rect.setTopLeft(m_point1);
                m_rect.setBottomRight(m_point2);
            }
            else
            {
                m_rect.setBottomLeft(m_point1);
                m_rect.setTopRight(m_point2);
            }
        }
        else
        {
            if(pt1y <= pt2y)
            {
                m_rect.setTopRight(m_point1);
                m_rect.setBottomLeft(m_point2);
            }
            else
            {
                m_rect.setTopLeft(m_point2);
                m_rect.setBottomRight(m_point1);
            }
        }

        return true;
    }

    bool mouseRelease(QMouseEvent *e)
    {
        return mouseMove(e);
    }

    QPoint m_point1;
    QPoint m_point2;
    QRect m_rect;
};

class RoundedRectangle: public Rectangle
{
public:
    RoundedRectangle(int xRadius, int yRadius)
        : Rectangle(roundedRect), m_xRadius(xRadius)
        , m_yRadius(yRadius)
    {}
    ~RoundedRectangle(){}

    void paint(QPainter *p)
    {
        p->drawRoundedRect(m_rect, m_xRadius, m_yRadius);
    }

    int m_xRadius;
    int m_yRadius;
};

class Ellipse: public Rectangle
{
public:
    Ellipse(): Rectangle(ellipse)
    {}
    ~Ellipse(){}

    void paint(QPainter *p)
    {
        p->drawEllipse(m_rect);
    }
};

class Path: public Shape
{
public:
    Path(): Shape(path)
    {}
    ~Path(){}

    bool isEmpty()
    {
        return m_path.isEmpty();
    }

    void paint(QPainter *p)
    {
        p->drawPath(m_path);
    }

    bool mousePress(QMouseEvent *e)
    {
        m_path.moveTo(e->pos());
        return false;
    }

    bool mouseMove(QMouseEvent *e)
    {
        m_path.lineTo(e->pos());
        return true;
    }

    bool mouseRelease(QMouseEvent *e)
    {
        m_path.lineTo(e->pos());
        return true;
    }

    QPainterPath m_path;
};

Shape *createShape(int type)
{
    switch(type)
    {
    case point:
        return new Point();
    case line:
        return new Line();
    case rectangle:
        return new Rectangle();
    case roundedRect:
        return new RoundedRectangle(12, 12);
    case ellipse:
        return new Ellipse();
    case path:
        return new Path();
    default:
        break;
    }

    return 0;
}
