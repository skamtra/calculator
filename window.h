#include <inttypes.h>
#include "aux.h"

enum WindowEventTypes
{
    EVENT_UNKNOWN = 0,
    EVENT_LEFTMOUSEBUTTONCLICK,
    EVENT_RIGHTMOUSEBUTTONCLICK,
    EVENT_TIMEOUT,
    EVENT_MOUSEMOVE,
    EVENT_KEYPRESS,
    EVENT_WINDOWRESIZE,
//    ...
    EVENT_LASTNUMBER
};

class Window
{
public:
    Window();
    virtual ~Window();

    bool        WindowProc(uint32_t type, const Point &position, uint64_t value); // процедура окна; вызывается из ОС для обработки событий

    void        Draw(Context *cr);                                  // отрисовка окна
    virtual void OnDraw(Context *cr);                               // виртуальный метод отрисовки; должен быть переопеределен в наследующих классах
    virtual void ReDraw();                                          // запрос на перерисовывание окна

    void        Create(Window *parent);                             // создание окна; вызывается из ОС для обработки создания окна
    virtual void OnCreate();                                        // виртуальный метод создания; может быть переопределен в наследующих классах

    void        AddChild(Window *child, const Point &position, const Point &size); // создание дочернего окна

    void        DeleteMe() { m_bToBeDeleted = true; }               // обращение окна к родителю: запрос об удалении
    void        Destroy();                                          // уничтожение окна: удаление потомков
    virtual void OnChildDelete(Window *pChild);                     // оповещение родитля об уничтожении потомка

    void        NotifyParent(uint32_t type, const Point &position); // уведомление родителя о событии дочернего окна
    void        Notify(Window *child, uint32_t type, const Point &position); // обработка уведомления от дочернего окна
    virtual void OnNotify(Window *child, uint32_t type, const Point &position); // виртуальный метод обработки уведомления от дочернего окна

    Point       &GetPosition();                                     // возвращает положение окна
    void        SetPosition(const Point &position);                 // устанавливает положение окна в родительском окне
    Point       &GetSize();                                         // возвращает размер окна
    void        SetSize(const Point &newsize);                      // вызывается из ОС; устанавливает/изменяет размер окна
    virtual void OnSizeChanged();                                   // виртуальный метод обработки изменения размера окна
    void CreateTimeout(uint32_t timeout);                           // создание оповещения о таймауте
    virtual void CreateTimeout(Window *pWindow, uint32_t timeout);  // создание оповещения о таймауте - ретрансляция ородителю

    // пустые виртуальные обработчики событий
    virtual bool OnLeftMouseButtonClick(const Point &position)
    {
        return false;
    }
    virtual bool OnRightMouseButtonClick(const Point &position)
    {
        return false;
    }
    virtual bool OnMouseMove(const Point &position)
    {
        return false;
    }
    virtual bool OnKeyPress(uint64_t)
    {
        return false;
    }
    virtual bool OnTimeout()
    {
        return false;
    }
//    ...

private:
    Point   m_size;                                                 // размер окна
    Point   m_position;                                             // положение окна в родительском окне
    Window  *m_pParent;                                             // родительское окно
    Window  *m_pMyFirstChild;                                       // начало цепочки дочерних окон
    Window  *m_pNextChild;                                          // следующее дочернее окно в цепочке дочерних окон родителя
    bool    m_bCreated;                                             // окно создано
public:
    bool    m_bToBeDeleted;                                         // запрос удаления окна
};

