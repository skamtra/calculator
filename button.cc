#include <string.h>
#include "window.h"
#include "button.h"

#define BS 2

Button::Button(const char *text, uint32_t command)
{
    m_backColor = RGB(0.5, 0.5, 0.5);
    m_frameColor = RGB(0.0, 0.0, 0.0);
    m_liteColor = RGB(0.99, 0.99, 0.99);
    m_darkColor = RGB(0.1, 0.1, 0.1);
    m_textColor = RGB(1.0, 1.0, 1.0);
    m_frameWidth = 3;
    m_textSize = 20;
    strcpy(m_fontFace,"Arial");

    strcpy(m_text, text);
    m_command = command;
}

Button::~Button()
{

}

void Button::OnDraw(Context *cr)
{
    // нарисуем рамку
    cr->SetColor(m_frameColor);
    cr->Rectangle(Point(0,0), GetSize());

    Point ws = GetSize();

    // рисуем рельеф
    cr->SetLineWidth(BS);
    cr->SetColor(m_liteColor);

    // сверху
    for(unsigned short i=0; i<m_frameWidth; i++)
    {
        cr->Line(Point(BS+i,BS+i),Point(ws.GetX()-BS-i+1,BS+i));
    }
    // слева
    for(unsigned short i=0; i<m_frameWidth; i++)
    {
        cr->Line(Point(BS+i,BS+i),Point(BS+i,ws.GetY()-BS-i+1));
    }

    cr->SetColor(m_darkColor);

    // снизу
    for(unsigned short i=0; i<m_frameWidth; i++)
    {
        cr->Line(Point(BS+i,ws.GetY()-BS-i),Point(ws.GetX()-BS-i+1,ws.GetY()-BS-i));
    }
    // справа
    for(unsigned short i=0; i<m_frameWidth; i++)
    {
        cr->Line(Point(ws.GetX()-BS-i,BS+i),Point(ws.GetX()-BS-i,ws.GetY()-BS-i+1));
    }

    // зальем прямоугольник окна цветом фона
    cr->SetColor(m_backColor);
    cr->FillRectangle(Point(m_frameWidth,m_frameWidth), GetSize()-Point(2*m_frameWidth,2*m_frameWidth));

    // текст
    cr->SetColor(m_textColor);
    cr->Text(m_text, m_fontFace, m_textSize, Point(ws.GetX()/2, ws.GetY()/2),
        TEXT_ALIGNH_CENTER|TEXT_ALIGNV_CENTER);
}

bool Button::OnLeftMouseButtonClick(const Point &position)
{
    NotifyParent(m_command,position);
    return true;
}

RGB  Button::GetBackColor()
{
    return m_backColor;
}

void Button::SetBackColor(RGB backColor)
{
    m_backColor = backColor;
}

RGB  Button::GetFrameColor()
{
    return m_frameColor;
}

void Button::SetFrameColor(RGB frameColor)
{
    m_frameColor = frameColor;
}

RGB  Button::GetTextColor()
{
    return m_textColor;
}

void Button::SetTextColor(RGB textColor)
{
    m_textColor = textColor;
}

uint16_t Button::GetFrameWidth()
{
    return m_frameWidth;
}

void     Button::SetFrameWidth(uint16_t frameWidth)
{
    m_frameWidth = frameWidth;
}

uint16_t Button::GetTextSize()
{
    return m_textSize;
}

void     Button::SetTextSize(uint16_t textSize)
{
    m_textSize = textSize;
}
