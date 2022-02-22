class Point
{
public:
    Point() : m_x(0), m_y(0) {}
    Point(uint16_t x, uint16_t y) : m_x(x), m_y(y) {}
    Point(const Point &pt) : m_x(pt.GetX()), m_y(pt.GetY()) {}
    ~Point() {}

    Point operator = (const Point &pt)
    {
        m_x = pt.GetX();
        m_y = pt.GetY();
        return *this;
    }
    Point operator + (const Point &pt) const
    {
        Point res(GetX()+pt.GetX(), GetY()+pt.GetY());
        return res;
    }
    Point operator - (const Point &pt) const
    {
        Point res(GetX()-pt.GetX(), GetY()-pt.GetY());
        return res;
    }

    uint16_t GetX() const { return m_x; }
    uint16_t GetY() const { return m_y; }

private:
    uint16_t m_x;
    uint16_t m_y;
};

class RGB
{
public:
    RGB() : m_red(0), m_green(0), m_blue(0) {}
    RGB(double red, double green, double blue)
    {
        m_red = red;
        m_green = green;
        m_blue = blue;
    }
    RGB(RGB &clr) : m_red(clr.GetRed()), m_green(clr.GetGreen()), m_blue(clr.GetBlue()) {}

    double GetRed() const { return m_red; }
    double GetGreen() const { return m_green; }
    double GetBlue() const { return m_blue; }

private:
    double m_red, m_green, m_blue;
};

// стили текста
#define TEXT_STYLE_BOLD         0x01
#define TEXT_STYLE_ITALIC       0x02

#define TEXT_ALIGNH_LEFT        0x04
#define TEXT_ALIGNH_RIGHT       0x08
#define TEXT_ALIGNH_CENTER      0x10

#define TEXT_ALIGNV_TOP         0x20
#define TEXT_ALIGNV_BOTTOM      0x40
#define TEXT_ALIGNV_CENTER      0x80

class Context
{
public:
    Context() {}
    virtual ~Context() {}

    virtual void Save() = 0;
    virtual void Restore() = 0;
    virtual void SetMask(const Point &pos, const Point &size) = 0;

    virtual void SetColor(const RGB &clr) = 0;
    virtual void SetLineWidth(uint16_t width) = 0;

    virtual void FillRectangle(const Point &from, const Point &to) = 0;
    virtual void Rectangle(const Point &from, const Point &to) = 0;
    virtual void Line(const Point &from, const Point &to) = 0;
    virtual void Text(const char *text, const char *fontface,
        const uint16_t fontsize, const Point &pt, const uint32_t style) = 0;
};
