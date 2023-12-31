#version 430

struct Complex
{
    float real;
    float imag;
};

out vec4 color;
uniform Complex center;
uniform float scalar;
float maxIterTimes = 1000;

Complex square(Complex x)
{
    Complex res;
    res.real = pow(x.real, 2)-pow(x.imag, 2);
    res.imag = 2*x.real*x.imag;
    return res;
}

Complex add(Complex a, Complex b)
{
    Complex res;
    res.real = a.real + b.real;
    res.imag = a.imag + b.imag;
    return res;
}

//Complex iter_func(Complex x, Complex c)
//{
//    return add(square(x), c);
//}

float len(Complex x)
{

    return sqrt(pow(x.real, 2) + pow(x.imag, 2));
}

float cals_steps(Complex c)
{
    Complex z = {0,0};
    Complex temp = {0,0};
    float iterTimes = 0;
    while ((len(z)<2) && (iterTimes<maxIterTimes))
    {
        z = add(square(z), c);
        iterTimes += 1;
    }
    return iterTimes;
}

void main(void)
{
    Complex point;
    point.real = center.real + gl_FragCoord.x*scalar;
    point.imag = center.imag + gl_FragCoord.y*scalar;
//    color = vec4(0,0,test(point), 1);
	color = vec4(0.0, 0.0, cals_steps(point)/maxIterTimes, 1.0);
}



//float centerR = 0.1;
//float centerI = 0.1;
//float tempR;
//float tempI;
//float zreal;
//float zimag;

//void iter_func(float real, float imag, float _zreal, float _zimag)
//{
//    tempR = pow(real, 2) - pow(imag, 2);
//    tempI = 2*real*imag;
//    tempR = tempR + _zreal;
//    tempI = tempI + _zimag;
//    zreal = tempR;
//    zimag = tempI;
//}
//
//float len()
//{
//    return pow(zreal*zreal+zimag*zimag, 0.5);
//}
//
//float cals_steps(float real, float imag)
//{
//    zreal = 0;
//    zimag = 0;
//    float iterTimes = 0;
//    while (true)
//    {
//        iter_func(real, imag, zreal, zimag);
//        iterTimes = iterTimes + 1;
//        if (len() > 2 && iterTimes <maxIterTimes)
//        {
//            break;
//        }
//    }
//    return iterTimes;
//}











