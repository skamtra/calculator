#include "window.h"

Window::Window()
{
    m_pMyFirstChild = 0;
    m_pNextChild = 0;
    m_bToBeDeleted = false;
    m_bCreated = false;
}

Window::~Window()
{
}

bool Window::WindowProc(uint32_t type, const Point &position, uint64_t value)
{
    bool result = false;

    // событие - щелчок кнопки или перемещение мыши ?
    if(type == EVENT_LEFTMOUSEBUTTONCLICK || type == EVENT_RIGHTMOUSEBUTTONCLICK || type == EVENT_MOUSEMOVE)
    {
        // если событие относится к дочернему окну, делегируем его обработку процедуре дочернего окна
        Window **pPrevNext = &m_pMyFirstChild;
        for(Window *pChild = m_pMyFirstChild; pChild; pChild = pChild->m_pNextChild)
        {
            Point from = pChild->GetPosition();
            Point to = from + pChild->GetSize();

            if(position.GetX() >= from.GetX() && position.GetX() <= to.GetX()
                && position.GetY() >= from.GetY() && position.GetY() <= to.GetY())
            {
                // вызываем процедуру дочернего окна
                result = pChild->WindowProc(type, position-from, value);

                // дочернее окно запросило удаление?
                if(pChild->m_bToBeDeleted)
                {
                    Window *next = pChild->m_pNextChild;
                    OnChildDelete(pChild);
                    ReDraw();
                    pChild->Destroy();
                    delete pChild;
                    *pPrevNext = next;
                }
                break;
            }

            pPrevNext = &pChild->m_pNextChild;
        }

        // если обработка завершена, уходим
        if(result)
        {
            return result;
        }

        // стандартные события
        switch(type)
        {
        case EVENT_LEFTMOUSEBUTTONCLICK:
            return OnLeftMouseButtonClick(position);
        case EVENT_RIGHTMOUSEBUTTONCLICK:
            return OnRightMouseButtonClick(position);
        case EVENT_MOUSEMOVE:
            return OnMouseMove(position);
        default:
            return false;
        }
    }
    // событие - изменение размера окна ?
    else if(type == EVENT_WINDOWRESIZE)
    {
        SetSize(position);
        return true;
    }
    // событие - истечение интервала таймера ?
    else if(type == EVENT_TIMEOUT)
    {
        return OnTimeout();
    }
    // событие - нажатие клавиши ?
    else if(type == EVENT_KEYPRESS)
    {
        return OnKeyPress(value);
    }
    // других событий мы не знаем
    else
    {
        return false;
    }
}

void Window::Draw(Context *cr)
{
    if(!m_bCreated)
    {
        return;
    }

    // положение и размер окна
    Point position(0,0);
    Point size = GetSize();

    cr->Save();

    // рисуем во всем окне
    cr->SetMask(position, size);

    // вызываем метод отрисовки содержимого
    OnDraw(cr);

    cr->Restore();

    // отрисовка дочерних окон

    for(Window *pChild = m_pMyFirstChild; pChild; pChild = pChild->m_pNextChild)
    {
        position = pChild->GetPosition();
        size = pChild->GetSize();

        cr->Save();

	    cr->SetMask(position, size);
        pChild->Draw(cr);

        cr->Restore();
    }
}

void Window::OnDraw(Context *cr)
{
}

void Window::ReDraw()
{
    m_pParent->ReDraw();
}

void Window::Create(Window *parent)
{
    m_pParent = parent;

    // вызываем виртуальный метод OnCreate() для создания дочерних окон классов-наследников
    OnCreate();

    m_bCreated = true;
}

void Window::OnCreate()
{
}

void Window::AddChild(Window *child, const Point &position, const Point &size)
{
    child->m_pNextChild = m_pMyFirstChild;
    m_pMyFirstChild = child;

    child->SetPosition(position);
    child->SetSize(size);
    child->Create(this);
}

void Window::Destroy()
{
    // удаление дочерних окон
	while(m_pMyFirstChild)
	{
		Window *next = m_pMyFirstChild->m_pNextChild;
        m_pMyFirstChild->Destroy();
        delete m_pMyFirstChild;
        m_pMyFirstChild = next;
	}
}

void Window::OnChildDelete(Window *pChild)
{
}

void Window::NotifyParent(uint32_t type, const Point &position)
{
    m_pParent->Notify(this, type, position);
}

void Window::Notify(Window *child, uint32_t type, const Point &position)
{
    Point pos = child->GetPosition() + position;
    OnNotify(child, type, position);
}

void Window::OnNotify(Window *child, uint32_t type, const Point &position)
{
}

Point &Window::GetSize()
{
    return m_size;
}

void Window::SetSize(const Point &size)
{
    m_size = size;
    if(m_bCreated)
    {
        OnSizeChanged();
    }
}

void Window::OnSizeChanged()
{
}

Point &Window::GetPosition()
{
    return m_position;
}

void Window::SetPosition(const Point &position)
{
    m_position = position;
}

void Window::CreateTimeout(uint32_t timeout)
{
    m_pParent->CreateTimeout(this, timeout);
}

void Window::CreateTimeout(Window *pWindow, uint32_t timeout)
{
    m_pParent->CreateTimeout(pWindow, timeout);
}
