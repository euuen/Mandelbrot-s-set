#include <iostream>
#include <Windows.h>
#include <thread>
#include <math.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
//目前还差的地方：
//优化运算速度
class Complex
{
    double realNum;
    double imageNum;
public:
    Complex();
    Complex(const Complex& _target);
    Complex(const double& _realNum, const double& _imageNum);
    ~Complex();

    Complex&  operator=(const Complex& _target);
    void set_realNum(const double& _realNum);
    void set_imageNum(const double& _imageNum);
    double get_realNum();
    double get_imageNum();
    void print();
    double abs();
    Complex square() const;
    Complex add(const Complex& _target) const;
};

Complex iter_func(const Complex& x, const Complex& c);
int cals_steps(Complex c, int maxIterNum=300);
void loop(sf::RenderWindow& window);
void render_up();
void render_down();
/////////////////////////////////////////////////////////////////////////////////////////////
bool needRenderUp = true;
bool needRenderDown = true;
int width = 600, height = 600;
double pixelToDistance = 0.0066667;
double zooms = 0.0d;
Complex center(0,0);
sf::VertexArray canvasUp(sf::Points, 180000);
sf::VertexArray canvasDown(sf::Points, 180000);

int main()
{
    using namespace std;
    using namespace sf;
    thread t1(render_down), t2(render_up);
    RenderWindow window(VideoMode(width, height), "Mandelbort", Style::Titlebar | Style::Close);
    window.setFramerateLimit(60);
    View view = window.getView();
    view.setCenter(Vector2f(0,0));
    window.setView(view);
    loop(window);
    return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////
Complex::Complex()
{

}

Complex::Complex(const Complex& _target)
{
    realNum = _target.realNum;
    imageNum = _target.imageNum;
}

Complex::Complex(const double& _realNum, const double& _imageNum)
{
    realNum = _realNum;
    imageNum = _imageNum;
}

Complex::~Complex()
{

}

Complex& Complex::operator=(const Complex& _target)
{
    realNum = _target.realNum;
    imageNum = _target.imageNum;
    return *this;
}

void Complex::set_realNum(const double& _realNum)
{
    realNum = _realNum;
}

void Complex::set_imageNum(const double& _imageNum)
{
    imageNum = _imageNum;
}

double Complex::get_realNum()
{
    return realNum;
}

double Complex::get_imageNum()
{
    return imageNum;
}

void Complex::print()
{
    using namespace std;
    cout<< realNum <<"+"  << imageNum<< "i"<< endl;
}

double Complex::abs()
{
    return (sqrt(pow(realNum, 2) + pow(imageNum, 2)));
}


Complex Complex::square() const
{
//    using namespace std;
    Complex result;
    result.set_realNum(pow(realNum, 2) - pow(imageNum, 2));
    result.set_imageNum(2*(realNum)*(imageNum));
//    cout<<"real"<<(pow(realNum, 2) - pow(imageNum, 2))<<endl;
//    cout<<"image"<<(2*realNum*imageNum)<<endl;
    return result;
}

Complex Complex::add(const Complex& _target) const
{
    Complex result;
    result.set_realNum(realNum + _target.realNum);
    result.set_imageNum(imageNum + _target.imageNum);
    return result;
}

Complex iter_func(const Complex& x, const Complex& c)
{
    return x.square().add(c);
}

int cals_steps(Complex c, int maxIterNum)
{
    Complex z = Complex(0,0);
    int iterNum = 0;
    while (z.abs()<2 && iterNum<maxIterNum)
    {
        z = iter_func(z, c);
        iterNum++;
    }
    return iterNum;
}

void loop(sf::RenderWindow& window)
{
    using namespace sf;
    using namespace std;
    double newLNum, newRNum, newUNum, newDNum;
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
//            处理事件
            switch (event.type)
            {
            case Event::EventType::Closed:
                window.close();
                break;
            case Event::EventType::KeyPressed:
                switch (event.key.code)
                {
                case Keyboard::Key::Left:
                    newLNum = center.get_realNum() - 0.25d*pow(10,-zooms);
                    center.set_realNum(newLNum);
                    needRenderUp = true;
                    needRenderDown = true;
                    break;
                case Keyboard::Key::Right:
                    newRNum = center.get_realNum() + 0.25d*pow(10,-zooms);
                    center.set_realNum(newRNum);
                   needRenderUp = true;
                    needRenderDown = true;
                    break;
                case Keyboard::Key::Up:
                    newUNum = center.get_imageNum() - 0.25d*pow(10,-zooms);
                    center.set_imageNum(newUNum);
                    needRenderUp = true;
                    needRenderDown = true;
                    break;
                case Keyboard::Key::Down:
                    newDNum = center.get_imageNum() + 0.25d*pow(10,-zooms);
                    center.set_imageNum(newDNum);
                    needRenderUp = true;
                    needRenderDown = true;
                    break;
                case Keyboard::Key::Dash:
                    zooms -= 0.1;
                    needRenderUp = true;
                    needRenderDown = true;
                    break;
                case Keyboard::Key::Equal:
                    zooms += 0.1;
                    needRenderUp = true;
                    needRenderDown = true;
                    break;
                default:
                    break;
                }
            case Event::EventType::KeyReleased:
                switch (event.key.code)
                {
                default:
                    break;
                }
            default:
                break;
            }
        }
        if (!needRenderUp && !needRenderDown)
        {
            window.clear(Color::White);
            window.draw(canvasUp);
            window.draw(canvasDown);
            window.display();
            window.draw(canvasUp);
            window.draw(canvasDown);

        }
        window.display();
    }
}

void render_down()
{
    using namespace sf;
    while (true)
    {
        while (!needRenderDown) Sleep(10);
        double scalar = pixelToDistance*pow(10,-zooms);
        canvasDown.clear();
        for (int x = -width/2;x <= width/2;x++)
        {
            for (int y = -height/2;y <= 0;y++)
            {
                Complex point(center.get_realNum() + x*scalar,
                              center.get_imageNum() + y*scalar);
                canvasDown.append(Vertex(Vector2f(x,y),Color(0,0,cals_steps(point)*20)));
            }
        }
        needRenderDown = false;
    }

}

void render_up()
{
    using namespace sf;
    while (true)
    {
        while (!needRenderUp) Sleep(10);
        double scalar = pixelToDistance*pow(10,-zooms);
        canvasUp.clear();
        for (int x = -width/2;x <= width/2;x++)
        {
            for (int y = 0;y <= height/2;y++)
            {
                Complex point(center.get_realNum() + x*scalar,
                              center.get_imageNum() + y*scalar);
                canvasUp.append(Vertex(Vector2f(x,y),Color(0,0,cals_steps(point)*20)));
            }
        }
        needRenderUp = false;
    }

}




























