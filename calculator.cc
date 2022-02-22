#include <cmath>
#include <iostream>
#include <cstdlib>
#include <string.h>

#include "window.h"
#include "button.h"
#include "GUI.h"


enum UserEventType
{
    EVENT_1,
    EVENT_2,
    EVENT_3,
    EVENT_4,
    EVENT_5,
    EVENT_6,
    EVENT_7,
    EVENT_8,
    EVENT_9,
    EVENT_0,
    EVENT_EQ,
    EVENT_PLUS,
    EVENT_MINUS,
    EVENT_CLEAR,
    EVENT_MULT,
    EVENT_SHARE
};

// дочернее окно
class ChildWindow : public Window
{
public:
    ChildWindow() {}
    ~ChildWindow() {}

    void OnDraw(Context *cr);
    bool OnLeftMouseButtonClick(const Point &position);
    bool OnRightMouseButtonClick(const Point &position);
    bool OnMouseMove(const Point &position);
private:
    uint32_t m_count;
};

// родительское окно
class MainWindow : public Window
{
public:
    MainWindow() {num1=0; num2=0; flag=0;}
    ~MainWindow() {}

    void OnCreate();
    void OnDraw(Context *cr);
    void OnSizeChanged();
    void OnNotify(Window *child, uint32_t type, const Point &position);
    void OnChildDelete(Window *pChild);
    bool OnKeyPress(uint64_t value);
    bool OnTimeout();

private:
    ChildWindow *m_pChild;
    RGB         m_color;
    int          num1; // 1 vvodimoe chislo
    int          num2; // 2 vvodimoe chislo
    int          flag; // nuzgen dlya opredelenia tipa operacii
    int          res; // resultat operacii mezdu num1 i num2
    int          EQ;
};

void MainWindow::OnDraw(Context *cr)
{
    char * a = new char[255];
    char buffer [255];
    char * a1 = new char[255];
    char buffer1 [255];
    char * a2 = new char[255];
    char buffer2 [255];
    std::cout << "MainWindow::OnDraw()" << std::endl;
	Point size = GetSize();
	Point center(size.GetX()/2,size.GetY()/2);

    // зальем прямоугольник окна серым цветом
    cr->SetColor(RGB(0.8,0.8,0.8));
    cr->FillRectangle(Point(0,0), size);

	double scaleX = 0.5*size.GetX();
	double scaleY = 0.5*size.GetY();
	Point from(size.GetX(),size.GetY()/2);

	cr->SetColor(RGB(1,1,1));
	cr->FillRectangle(Point(250,100), Point(500, 130));

    cr->SetColor(RGB(0,0,0));
    cr->Text("Press F to close calculator","Arial", 20, Point(500,50), 0x01);
    if (num1 != 0)
    {
    int n1 = num1;
    n1 = g_snprintf(buffer, sizeof(buffer), "%d", n1);
    a = buffer;
    cr->Text(a,"Arial", 20, Point(270,150), 0x04);

    std::cout << num1 << std::endl;
    }

        if (flag == 1)
        {
            cr->Text("+","Arial", 20, Point(strlen(a)*11+270+10,150), 0x10);
        }
        if (flag == 2)
        {
            cr->Text("-","Arial", 20, Point(strlen(a)*11+270+10,150), 0x10);
        }
        if (flag == 3)
        {
        cr->Text("*","Arial", 20, Point(strlen(a)*11+270+10,150), 0x10);
        }
         if (flag == 4)
        {
        cr->Text("/","Arial", 20, Point(strlen(a)*11+270+10,150), 0x10);
        }
    if ((flag > 0 && flag < 5)&&(num2 != 0))
    {
    int n2 = num2;
    n2 = g_snprintf(buffer1, sizeof(buffer1), "%d", n2);
    a1 = buffer1;
    cr->Text(a1,"Arial", 20, Point(strlen(a)*11+270+20,150), 0x04);
    std::cout << num2 << std::endl;
    }
    if (EQ == 1)
    {
    cr->Text("=","Arial", 20, Point(strlen(a)*11+strlen(a1)*11+270+30,150), 0x04);
    int n3 = res;
    n3 = g_snprintf(buffer2, sizeof(buffer2), "%d", n3);
    a2 = buffer2;
    cr->Text(a2,"Arial", 20, Point(strlen(a)*11+strlen(a1)*11+270+50,150), 0x04);
    std::cout << res << std::endl;
    }
}

void MainWindow::OnCreate()
{
    std::cout << "MainWindow::OnCreate()" << std::endl;
    AddChild(new Button(" 7 ",EVENT_7), Point(250,260), Point(100,90));
    AddChild(new Button(" 4 ",EVENT_4), Point(250,350), Point(100,90));
    AddChild(new Button(" 1 ",EVENT_1), Point(250,440), Point(100,90));
    AddChild(new Button(" 8 ",EVENT_8), Point(350,260), Point(100,90));
    AddChild(new Button(" 5 ",EVENT_5), Point(350,350), Point(100,90));
    AddChild(new Button(" 2 ",EVENT_2), Point(350,440), Point(100,90));
    AddChild(new Button(" 9 ",EVENT_9), Point(450,260), Point(100,90));
    AddChild(new Button(" 6 ",EVENT_6), Point(450,350), Point(100,90));
    AddChild(new Button(" 3 ",EVENT_3), Point(450,440), Point(100,90));
    AddChild(new Button(" + ",EVENT_PLUS), Point(550,260), Point(100,90));
    AddChild(new Button(" - ",EVENT_MINUS), Point(550,350), Point(100,90));
    AddChild(new Button(" = ",EVENT_EQ), Point(550,440), Point(100,90));
    AddChild(new Button(" CLEAR ",EVENT_CLEAR), Point(650,260), Point(100,270));
    AddChild(new Button(" 0 ",EVENT_0), Point(250,530), Point(300,90));
    AddChild(new Button(" * ",EVENT_MULT), Point(550,530), Point(100,90));
    AddChild(new Button(" / ",EVENT_SHARE), Point(650,530), Point(100,90));
}

void MainWindow::OnSizeChanged()
{
    std::cout << "MainWindow::OnSizeChanged()" << std::endl;
    if(m_pChild)
    {
        Point mysize = GetSize();
        Point childsize(500,140);
        Point childpos(250,87);
        m_pChild->SetPosition(childpos);
        m_pChild->SetSize(childsize);
    }
}

void MainWindow::OnNotify(Window *child, uint32_t type, const Point &position)
{
    std::cout << "MainWindow::OnNotify()" << std::endl;
    if(type == EVENT_CLEAR)
    {
        num1 = 0;
        num2 = 0;
        flag = 0;
        res = 0;
        EQ = 0;
        ReDraw();
    }
     if((type == EVENT_PLUS) && (EQ != 1))
    {
        flag = 1;
        ReDraw();
    }
    if ((type == EVENT_MINUS) && (EQ != 1))
    {
        flag = 2;
        ReDraw();
    }
    if ((type == EVENT_MULT) && (EQ != 1))
    {
        flag = 3;
        ReDraw();
    }
    if ((type == EVENT_SHARE) && (EQ != 1))
    {
        flag = 4;
        ReDraw();
    }

    if (flag == 0)
    {
        if (type == EVENT_0)
        {
            num1 = num1*10 + 0;
            ReDraw();
        }
        if (type == EVENT_1)
        {
            num1 = num1*10 + 1;
            ReDraw();
        }
        if(type == EVENT_2)
        {
            num1 = num1*10 + 2;
            ReDraw();
        }
        if(type == EVENT_3)
        {
            num1 = num1*10 + 3;
            ReDraw();
        }
        if(type == EVENT_4)
        {
            num1 = num1*10 + 4;
            ReDraw();
        }
        if(type == EVENT_5)
        {
            num1 = num1*10 + 5;
            ReDraw();
        }
        if(type == EVENT_6)
        {
            num1 = num1*10 + 6;
            ReDraw();
        }
        if(type == EVENT_7)
        {
            num1 = num1*10 + 7;
            ReDraw();
        }
        if(type == EVENT_8)
        {
            num1 = num1*10 + 8;
            ReDraw();
        }
        if(type == EVENT_9)
        {
            num1 = num1*10 + 9;
            ReDraw();
        }
}
     if (flag !=0)
     {
        if (type == EVENT_0)
        {
            num2 = num2*10 + 0;
            ReDraw();
        }
        if (type == EVENT_1)
        {
            num2 = num2*10 + 1;
            ReDraw();
        }
        if(type == EVENT_2)
        {
            num2 = num2*10 + 2;
            ReDraw();
        }
        if(type == EVENT_3)
        {
            num2 = num2*10 + 3;
            ReDraw();
        }
        if(type == EVENT_4)
        {
            num2 = num2*10 + 4;
            ReDraw();
        }
        if(type == EVENT_5)
        {
            num2 = num2*10 + 5;
            ReDraw();
        }
        if(type == EVENT_6)
        {
            num2 = num2*10 + 6;
            ReDraw();
        }
        if(type == EVENT_7)
        {
            num2 = num2*10 + 7;
            ReDraw();
        }
        if(type == EVENT_8)
        {
            num2 = num2*10 + 8;
            ReDraw();
        }
        if(type == EVENT_9)
        {
            num2 = num2*10 + 9;
            ReDraw();
        }
     }
     if ((type == EVENT_EQ) && (flag > 0 && flag < 5) && (num2 != 0))
     {
        if (flag == 1)
            res = num1 + num2;
        if (flag == 2)
            res = num1 - num2;
        if (flag == 3)
            res = num1 * num2;
        if (flag == 4)
            res = num1 / num2;
        EQ = 1;
        ReDraw();
     }

}
void MainWindow::OnChildDelete(Window *pChild)
{
    std::cout << "MainWindow::OnChildDelete()" << std::endl;
    if(m_pChild == pChild)
    {
        m_pChild = NULL;
    }
}

bool MainWindow::OnKeyPress(uint64_t keyval)
{
    std::cout << "MainWindow::OnKeyPress(" << keyval << ")" << std::endl;
    switch(keyval)
    {
    case '1':
        if (flag == 0)
            num1 = num1*10 + 1;
        else
            num2 = num2*10 + 1;
        break;
    case '2':
        if (flag == 0)
            num1 = num1*10 + 2;
        else
            num2 = num2*10 + 2;
        break;
    case '3':
        if (flag == 0)
            num1 = num1*10 + 3;
        else
            num2 = num2*10 + 3;
        break;
    case '4':
        if (flag == 0)
            num1 = num1*10 + 4;
        else
            num2 = num2*10 + 4;
        break;
    case '5':
        if (flag == 0)
            num1 = num1*10 + 5;
        else
            num2 = num2*10 + 5;
        break;
    case '6':
        if (flag == 0)
            num1 = num1*10 + 6;
        else
            num2 = num2*10 + 6;
        break;
    case '7':
        if (flag == 0)
            num1 = num1*10 + 7;
        else
            num2 = num2*10 + 7;
        break;
    case '8':
        if (flag == 0)
            num1 = num1*10 + 8;
        else
            num2 = num2*10 + 8;
        break;
    case '9':
        if (flag == 0)
            num1 = num1*10 + 9;
        else
            num2 = num2*10 + 9;
        break;
    case '0':
        if (flag == 0)
            num1 = num1*10 + 0;
        else
            num2 = num2*10 + 0;
        break;
    case '+':
        if (EQ != 1)
            flag = 1;
        break;
    case '-':
        if (EQ != 1)
            flag = 2;
        break;
    case '*':
        if (EQ != 1)
            flag = 3;
        break;
    case '/':
        if (EQ != 1)
            flag = 4;
        break;
    case '=':
       if ((flag > 0 && flag < 5) && (num2 != 0))
        {
        if (flag == 1)
            res = num1 + num2;
        if (flag == 2)
            res = num1 - num2;
        if (flag == 3)
            res = num1 * num2;
        if (flag == 4)
            res = num1 / num2;
        EQ = 1;
        }
        break;
    case 'c':
        num1 = 0;
        num2 = 0;
        flag = 0;
        res = 0;
        EQ = 0;
        break;
    case 'f':
        DeleteMe();
        return true;
    default:
        ;
}
        ReDraw();
        return true;
}

bool MainWindow::OnTimeout()
{
    std::cout << "MainWindow::OnTimeout()" << std::endl;
    static int k = 5;
    return --k > 0;
}

// функция main
int main(int argc, char **argv)
{
    MainWindow *pWindow = new MainWindow;

    int res = Run(argc, argv, pWindow, 1000, 700);

    delete pWindow;

    return res;
}

