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
    // myprint2(myobj);
    //如果不用std::ref(myobj),则会在子线程中先拷贝构造一个临时A对象，然后再拷贝给myprint2的第二个参数
    //如果使用std::move，相对上面会少一次析构
    //使用std::ref是传入真正的引用，对象的地址都是一样的。
    thread mytobj(myprint2, myobj); 

    mytobj.join();

    cout << "main thread end!" << endl;
    return 0;
}