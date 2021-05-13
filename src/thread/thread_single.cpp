#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <list>
#include <mutex>

using namespace std;

std::mutex resource_mutex;

class MyCAS
{
private:
    MyCAS() {} //私有化了的构造函数

private:
    static MyCAS *m_instance; //静态成员变量

public:
    static MyCAS *GetInstance()
    {
        //提高效率
        //a)如果if (m_instance != NULL) 条件成立，则肯定表示m_instance已经被new过了；
        //b)如果if (m_instance == NULL)，不代表m_instance一定没被new过；

        if (m_instance == NULL) //双重锁定（双程检查）
        {
            std::unique_lock<std::mutex> mymutex(resource_mutex); //自动加锁，但效率非常低
            if (m_instance == NULL)
            {
                m_instance == new MyCAS();
                static CGarhuishou cl;
            }
        }
        return m_instance;
    }

    class CGarhuishou //类中套类，用来释放对象
    {
    public:
        ~CGarhuishou() //类的析构函数中
        {
            if (MyCAS::m_instance)
            {
                delete MyCAS::m_instance;
                MyCAS::m_instance = NULL;
            }
        }
    };

    void func()
    {
        cout << "测试" << endl;
    }
};

//类静态变量初始化
MyCAS *MyCAS::m_instance = NULL;

//线程入口函数
void mythread()
{
    cout << "我的线程开始执行了" << endl;
    MyCAS *p_a = MyCAS::GetInstance(); //这里可能就有问题了
    cout << "我的线程执行完毕了" << endl;
}
int main()
{
    std::thread myobj1(mythread);
    std::thread myobj2(mythread);

    myobj1.join();
    myobj2.join();

    return 0;
}