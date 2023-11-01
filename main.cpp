#include <iostream>
#include <math.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
//目前还差的地方：
//优化运算速度
class Complex
{
    double* realNum;
    double* imageNum;
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
int cals_steps(Complex c, int maxIterNum=255);
void loop(sf::RenderWindow& window);
void render();
/////////////////////////////////////////////////////////////////////////////////////////////
bool needRender = true;
int width = 600, height = 600;
double pixelToDistance = 0.0066667;
double zooms = 0.0d;
Complex center(0,0);
sf::VertexArray canvas(sf::Points, 358801);

int main()
{
    using namespace std;
    using namespace sf;
    RenderWindow window(VideoMode(width, height), "Mandelbort", Style::Titlebar | Style::Close);
    window.setFramerateLimit(60);
    View view = window.getView();
    view.move(Vector2f(-300, -300));
    window.setView(view);
    loop(window);
    return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////
Complex::Complex()
{
    realNum = new double;
    imageNum = new double;
}

Complex::Complex(const Complex& _target)
{
    realNum = new double;
    imageNum = new double;
    *realNum = *(_target.realNum);
    *imageNum = *(_target.imageNum);
}

Complex::Complex(const double& _realNum, const double& _imageNum)
{
    realNum = new double;
    imageNum = new double;
    *realNum = _realNum;
    *imageNum = _imageNum;
}

Complex::~Complex()
{
    delete realNum;
    delete imageNum;
}

Complex& Complex::operator=(const Complex& _target)
{
    *realNum = *(_target.realNum);
    *imageNum = *(_target.imageNum);
    return *this;
}

void Complex::set_realNum(const double& _realNum)
{
    *realNum = _realNum;
}

void Complex::set_imageNum(const double& _imageNum)
{
    *imageNum = _imageNum;
}

double Complex::get_realNum()
{
    return *realNum;
}

double Complex::get_imageNum()
{
    return *imageNum;
}

void Complex::print()
{
    using namespace std;
    cout<< *realNum <<"+"  << *imageNum<< "i"<< endl;
}

double Complex::abs()
{
    return (sqrt(pow(*realNum, 2) + pow(*imageNum, 2)));
}


Complex Complex::square() const
{
//    using namespace std;
    Complex result;
    result.set_realNum(pow(*realNum, 2) - pow(*imageNum, 2));
    result.set_imageNum(2*(*realNum)*(*imageNum));
//    cout<<"real"<<(pow(realNum, 2) - pow(imageNum, 2))<<endl;
//    cout<<"image"<<(2*realNum*imageNum)<<endl;
    return result;
}

Complex Complex::add(const Complex& _target) const
{
    Complex result;
    result.set_realNum(*realNum + *_target.realNum);
    result.set_imageNum(*imageNum + *_target.imageNum);
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
                    newLNum = center.get_realNum()- 0.25d*zooms;
                    center.set_realNum(newLNum);
                    needRender = true;
                    break;
                case Keyboard::Key::Right:
                    newRNum = center.get_realNum()+ 0.25d*zooms;
                    center.set_realNum(newRNum);
                    needRender = true;
                    break;
                case Keyboard::Key::Up:
                    newUNum = center.get_imageNum() + 0.25d*zooms;
                    center.set_imageNum(newUNum);
                    needRender = true;
                    break;
                case Keyboard::Key::Down:
                    newDNum = center.get_imageNum() - 0.25d*zooms;
                    center.set_imageNum(newDNum);
                    needRender = true;
                    break;
                case Keyboard::Key::Dash:
                    zooms -= 0.1;
                    needRender = true;
                    break;
                case Keyboard::Key::Equal:
                    zooms += 0.1;
                    needRender = true;
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
        if (needRender)
        {
            window.clear(Color::White);
            render();
            window.draw(canvas);
            window.display();
            window.draw(canvas);
        }
        window.display();
    }
}

void render()
{
    using namespace sf;
    double scalar = pixelToDistance*pow(10,-zooms);
    for (int x = -width/2;x <= width/2;x++)
    {
        for (int y = -height/2;y <= height/2;y++)
        {
            Complex point(center.get_realNum() + x*scalar,
                          center.get_imageNum() + y*scalar);
            canvas.append(Vertex(Vector2f(x,y),Color(0,0,cals_steps(point))));
        }
    }
    needRender = false;
}




























