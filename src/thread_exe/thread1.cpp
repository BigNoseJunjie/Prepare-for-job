#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <list>
#include <mutex>

using namespace std;

class A
{
public:
    void inMsgRecvQueue()
    {
        for (int i = 0; i < 100; i++)
        {
            cout << "inMsgRecvQueue()执行，插入一个元素" << i << endl;
            {
                std::unique_lock<std::mutex> sbguard(my_mutex, std::try_to_lock);
                if (sbguard.owns_lock())
                {
                    //拿到了锁
                    msgRecvQueue.push_back(i);
                    //...
                    //其他处理代码
                }
                else
                {
                    //没拿到锁
                    cout << "inMsgRecvQueue()执行，但没拿到锁头，只能干点别的事" << i << endl;
                }
            }
        }
    }

    bool outMsgLULProc(int &command)
    {

        my_mutex.lock(); //要先lock(),后续才能用unique_lock的std::adopt_lock参数
        std::unique_lock<std::mutex> sbguard(my_mutex, std::adopt_lock);

        if (!msgRecvQueue.empty())
        {
            //消息不为空
            int command = msgRecvQueue.front(); //返回第一个元素，但不检查元素是否存在
            msgRecvQueue.pop_front();           //移除第一个元素。但不返回；

            return true;
        }
        return false;
    }
    //把数据从消息队列取出的线程
    void outMsgRecvQueue()
    {
        int command = 0;
        std::chrono::milliseconds dura(5000);
        std::this_thread::sleep_for(dura); //休息20s
        for (int i = 0; i < 100; i++)
        {
            bool result = outMsgLULProc(command);

            if (result == true)
            {
                cout << "outMsgRecvQueue()执行，取出一个元素" << endl;
                //处理数据
            }
            else
            {
                //消息队列为空
                cout << "inMsgRecvQueue()执行，但目前消息队列中为空！" << i << endl;
            }
        }
        cout << "end!" << endl;
    }

private:
    std::list<int> msgRecvQueue; //容器（消息队列），代表玩家发送过来的命令。
    std::mutex my_mutex;         //创建一个互斥量（一把锁）
};

int main()
{
    A myobja;

    std::thread myOutMsgObj(&A::outMsgRecvQueue, &myobja);
    std::thread myInMsgObj(&A::inMsgRecvQueue, &myobja);

    myOutMsgObj.join();
    myInMsgObj.join();

    cout << "主线程执行！" << endl;

    return 0;
}