class Button : public Window
{
public:
    Button(const char *text, uint32_t command);
    ~Button();

    void OnDraw(Context *cr);
    bool OnLeftMouseButtonClick(const Point &position);

    RGB  GetBackColor();
    void SetBackColor(RGB backColor);
    RGB  GetFrameColor();
    void SetFrameColor(RGB frameColor);
    RGB  GetTextColor();
    void SetTextColor(RGB textColor);

    uint16_t GetFrameWidth();
    void     SetFrameWidth(uint16_t frameWidth);
    uint16_t GetTextSize();
    void     SetTextSize(uint16_t textSize);

private:
    char m_text[100];
    uint32_t    m_command;

    RGB  m_backColor;
    RGB  m_frameColor;
    RGB  m_liteColor;   // add Get/Set
    RGB  m_darkColor;   // add Get/Set
    RGB  m_textColor;
    uint16_t m_frameWidth;
    uint16_t m_textSize;
    char m_fontFace[100];   // add Get/Set
};
