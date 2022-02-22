//#include <iostream>
#include <assert.h>

#include "window.h"
#include "GUI.h"

CairoContext::CairoContext()
{
    m_width = 1;
}

CairoContext::~CairoContext()
{
}

void CairoContext::SetCairoContext(cairo_t *cr)
{
    m_cr = cr;
}

void CairoContext::Save()
{
    cairo_save(m_cr);
}

void CairoContext::Restore()
{
    cairo_restore(m_cr);
}

void CairoContext::SetMask(const Point &pos, const Point &size)
{
    cairo_translate(m_cr, pos.GetX(), pos.GetY());
    cairo_rectangle(m_cr, 0, 0, size.GetX(), size.GetY());
    cairo_clip(m_cr);
}

void CairoContext::SetColor(const RGB &clr)
{
    m_color = clr;
}

void CairoContext::SetLineWidth(uint16_t width)
{
    m_width = width;
}

void CairoContext::FillRectangle(const Point &from, const Point &to)
{
	cairo_set_source_rgba(m_cr, m_color.GetRed(), m_color.GetGreen(), m_color.GetBlue(), 1.0);
	cairo_set_line_width (m_cr, m_width);
	cairo_rectangle(m_cr, from.GetX(), from.GetY(), to.GetX(), to.GetY());
    cairo_fill(m_cr);
}

void CairoContext::Rectangle(const Point &from, const Point &to)
{
	cairo_set_source_rgba(m_cr, m_color.GetRed(), m_color.GetGreen(), m_color.GetBlue(), 1.0);
	cairo_set_line_width (m_cr, m_width);
	cairo_rectangle(m_cr, from.GetX(), from.GetY(), to.GetX(), to.GetY());
    cairo_stroke(m_cr);
}

void CairoContext::Line(const Point &from, const Point &to)
{
	cairo_set_source_rgba(m_cr, m_color.GetRed(), m_color.GetGreen(), m_color.GetBlue(), 1.0);
	cairo_set_line_width (m_cr, m_width);
    cairo_move_to(m_cr, from.GetX(), from.GetY());
    cairo_line_to(m_cr, to.GetX(), to.GetY());
    cairo_stroke(m_cr);
}

void CairoContext::Text(const char *text, const char *fontface,
    const uint16_t fontsize, const Point &pt, const uint32_t style)
{
    cairo_text_extents_t te;
	cairo_set_source_rgba(m_cr, m_color.GetRed(), m_color.GetGreen(), m_color.GetBlue(), 1.0);
    cairo_select_font_face (m_cr, fontface,
        style & TEXT_STYLE_ITALIC ? CAIRO_FONT_SLANT_ITALIC : CAIRO_FONT_SLANT_NORMAL,
        style & TEXT_STYLE_BOLD ? CAIRO_FONT_WEIGHT_BOLD : CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size (m_cr, fontsize);
    cairo_text_extents (m_cr, text, &te);

    double x,y;

    // выравнивание по горизонтали
    if(style & TEXT_ALIGNH_LEFT)
    {
        x = pt.GetX() + te.x_bearing;
    }
    else if(style & TEXT_ALIGNH_RIGHT)
    {
        x = pt.GetX() - te.width - te.x_bearing;
    }
    else // TEXT_ALIGNH_CENTER
    {
        x = pt.GetX() - te.width / 2 - te.x_bearing;
    }

    // выравнивание по вертикали
    if(style & TEXT_ALIGNV_TOP)
    {
        y = pt.GetY() - te.y_bearing;
    }
    else if(style & TEXT_ALIGNV_BOTTOM)
    {
        y = pt.GetY() + te.height + te.y_bearing;
    }
    else // TEXT_ALIGNV_CENTER
    {
        y = pt.GetY() - te.height / 2 - te.y_bearing;
    }

    cairo_move_to (m_cr, x, y);
    cairo_show_text (m_cr, text);
}





GtkPlus::GtkPlus()
{
}

GtkPlus::~GtkPlus()
{
}

int GtkPlus::Run(int argc, char **argv, Window *wnd, uint16_t w, uint16_t h)
{
    // инициализация GTK+
	gtk_init(&argc, &argv);

	// Создаем главное окно, устанавливваем заголовок
	m_Widget = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	m_Window = wnd;
	gtk_window_set_title(GTK_WINDOW(m_Widget), "GUI Model");
	gtk_window_set_default_size (GTK_WINDOW(m_Widget),w,h);

	wnd->Create(this);

	// Соединяем сигналы
	g_signal_connect(G_OBJECT(m_Widget), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(G_OBJECT(m_Widget), "draw", G_CALLBACK(on_draw), this);
	g_signal_connect(G_OBJECT(m_Widget), "size-allocate", G_CALLBACK(on_size_allocate), this);
    g_signal_connect (m_Widget, "button-press-event", G_CALLBACK (on_button_press_event), this);
    g_signal_connect (m_Widget, "key-press-event", G_CALLBACK (on_key_press_event), this);
    g_signal_connect (m_Widget, "motion-notify-event", G_CALLBACK (on_motion_notify_event), this);

    gtk_widget_set_events (m_Widget, gtk_widget_get_events (m_Widget)
        | GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK);

	// Показываем окно
	gtk_widget_show_all(m_Widget);

	// Приложение переходит в вечный цикл ожидания действий пользователя
	gtk_main();

	return 0;
}

bool GtkPlus::NotifyWindow(uint32_t type, const Point &p, uint64_t value)
{
    bool res = m_Window->WindowProc(type, p, value);

    if(m_Window->m_bToBeDeleted)
    {
        gtk_main_quit();
    }

    return res;
}

gboolean GtkPlus::Draw(GtkWidget *widget, cairo_t *cr)
{
    assert(m_Widget == widget);
    SetCairoContext(cr);
    m_Window->Draw(this);
    return TRUE;
}

void GtkPlus::ReDraw()
{
    gtk_widget_queue_draw(m_Widget);
}

void GtkPlus::CreateTimeout(Window *pWindow, uint32_t timeout)
{
    gdk_threads_add_timeout (timeout, &on_timeout, pWindow);
}

gboolean GtkPlus::Allocation(GtkWidget *widget, GdkRectangle *allocation)
{
    assert(m_Widget == widget);
    return NotifyWindow(EVENT_WINDOWRESIZE, Point(allocation->width,allocation->height),0);
}

gboolean GtkPlus::ButtonPressEvent(GtkWidget *widget, GdkEventButton *event)
{
    assert(m_Widget == widget);

    uint32_t type;

    switch(event->button)
    {
    case GDK_BUTTON_PRIMARY:
        type = EVENT_LEFTMOUSEBUTTONCLICK;
        break;
    case GDK_BUTTON_SECONDARY:
        type = EVENT_RIGHTMOUSEBUTTONCLICK;
        break;
    default:
        type = EVENT_UNKNOWN;
    }

    return NotifyWindow(type, Point(event->x,event->y),0);
}

gboolean GtkPlus::MotionNotifyEvent(GtkWidget *widget, GdkEventMotion *event)
{
    assert(m_Widget == widget);

    uint32_t type;

    switch(event->type)
    {
    case GDK_MOTION_NOTIFY:
        type = EVENT_MOUSEMOVE;
        break;
    default:
        type = EVENT_UNKNOWN;
    }

    return NotifyWindow(type, Point(event->x,event->y),0);
}

gboolean GtkPlus::KeyPressEvent(GtkWidget *widget, GdkEvent *event)
{
    assert(m_Widget == widget);
    return NotifyWindow(EVENT_KEYPRESS, Point(0,0), event->key.keyval);
}



int Run(int argc, char **argv, Window *wnd, uint16_t w, uint16_t h)
{
    GtkPlus *gui = new GtkPlus;
    int res = gui->Run(argc, argv, wnd, w, h);
    delete gui;
    return res;
}

gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{
    GtkPlus *gui = reinterpret_cast<GtkPlus*>(user_data);
    return gui->Draw(widget,cr);
}

gboolean on_size_allocate(GtkWidget *widget, GdkRectangle *allocation, gpointer user_data)
{
    GtkPlus *gui = reinterpret_cast<GtkPlus*>(user_data);
    return gui->Allocation(widget,allocation);
}

gboolean on_button_press_event(GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
    GtkPlus *gui = reinterpret_cast<GtkPlus*>(user_data);
    return gui->ButtonPressEvent(widget,event);
}

gboolean on_key_press_event(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    GtkPlus *gui = reinterpret_cast<GtkPlus*>(user_data);
    return gui->KeyPressEvent(widget,event);
}

gboolean on_motion_notify_event(GtkWidget *widget, GdkEventMotion *event, gpointer user_data)
{
    GtkPlus *gui = reinterpret_cast<GtkPlus*>(user_data);
    return gui->MotionNotifyEvent(widget,event);
}


gboolean on_timeout(gpointer user_data)
{
    Window *win = reinterpret_cast<Window*>(user_data);
    return win->WindowProc(EVENT_TIMEOUT, Point(0,0), 0);
}
