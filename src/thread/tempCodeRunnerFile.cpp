#include <iostream>
#include <thread>
#include <string>
using namespace std;

class A
{
public:
    mutable int m_i;
    //类型转换构造函数，可以把一个int转换成一个类A对象。
    A(int a) : m_i(a)
    {
        cout << "A::A(int a)构造函数执行！" << this << "threadid:" << std::this_thread::get_id() << endl;
    }
    A(const A &a) : m_i(a.m_i)
    {
        cout << "A::A(A &a)复制构造函数执行！" << this << "threadid:" << std::this_thread::get_id() << endl;
    }

    ~A()
    {
        cout << "A::~A()析构函数执行！" << this << "threadid:" << std::this_thread::get_id() << endl;
    }
};

void myprint2(const A &pmybuf)
{
    pmybuf.m_i = 199;                                                                                   //我们修改该值不会影响main()函数
    cout << "子对象myprint的参数地址是" << &pmybuf << "threadid" << std::this_thread::get_id() << endl; // 打印的是pmybuf对象的地址
}

int main()
{
    A myobj(10); //生成一个类对象
    thread mytobj(myprint2, std::ref(myobj));
    mytobj.join();

    cout << "main thread end!" << endl;
    return 0;
}