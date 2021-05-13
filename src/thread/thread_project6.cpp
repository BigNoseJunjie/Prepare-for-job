#include <iostream>
#include <string>
#include <list>
#include <thread>
#include <mutex>
#include <condition_variable>
using namespace std;

class Msg
{

public:
    Msg() = default;
    Msg(Msg &A) = delete;
    Msg &operator=(const Msg &A) = delete;

    void InputMsg(int n)
    {
        for (int i = 0; i < n; i++)
        {
            unique_lock<mutex> in_uniquelock(this->my_mutex);
            msgQueue.push_back(i);
            cout << "the thread id :" << this_thread::get_id() << " inputs a value " << i << endl;
            //假如OutputMsg_withCV正在处理一个事务，需要一段时间，而不是正卡在wait()那里等待你的唤醒，
            //那么此时这个notify_one（）这个调用也许就没效果；
            my_cond.notify_one(); //我们尝试把wait()线程唤醒,执行完这行，那么outMsgRecvQueue()里面的wait()就会被唤醒
                                  //唤醒之后的事情后续研究；
                                  //my_cond.notify_all(); //通知所有线程
                                  //...
        }
        started_flag = false;
    }

    void OutputMsg_withoutCV()
    {
        while (true)
        {
            if (!this->msgQueue.empty())
            {
                unique_lock<mutex> out_uniquelock(this->my_mutex);
                if (!this->msgQueue.empty())
                {
                    int value = msgQueue.front();
                    msgQueue.pop_front();
                    cout << "this thread id : " << this_thread::get_id() << " outputs a value " << value << endl;
                }
            }
            if (!started_flag && msgQueue.empty())
                break;
        }
        cout << "this thread id :" << this_thread::get_id() << " over!" << endl;
    }

    void OutputMsg_withCV()
    {
        while (true)
        {
            unique_lock<mutex> out_uniquelock(my_mutex);
            //wait()用来等一个东西
            //如果第二个参数lambda表达式返回值是true,那么wait()直接返回；
            //如果第二个参数lambda表达式返回值是false,那么wait()将解锁互斥量，并且堵塞到本行
            //那堵到什么时候为止？堵塞到其他线程调用notify_one()成员函数为止。
            //如果wait()没有第二个参数：my_cond.wait(sbguard）；那么就跟第二个参数返回false效果一样，
            //那么wait()将解锁互斥量，并且堵塞到本行，堵塞到其他线程调用notify_one()成员函数为止。

            //但其他线程用notify_one()将本wait(原来是睡着/堵塞)的状态唤醒后，wait()就开始恢复干活了，那恢复后的
            //wait()干什么活？
            //a)wait()不断的尝试重新获取互斥量锁，如果获取不到，那么流程就卡在wait()这里等着获取，如果获取到了互斥锁，
            //那么wait()就继续执行b
            //b)上锁（实际上获取锁了就等于上了锁）
            //b.1)如果wait有第二个参数（lamdba）,就判断这个lamdba表达式，如果表达式为false,
            //那么wait()又对互斥量解锁然后又休眠，这里继续等待再次被notify_one()唤醒
            //b.2)如果lamdba表达式为true,则wait()返回，流程走下来（此时互斥锁被锁着）
            //b.3)如果wait()没有第二个参数,则wait()返回，流程走下来
            //为防止虚假唤醒：wait()中要有第二个参数（lambda）并且这个lambda中要正确处理公共数
            // 据是否存在
            my_cond.wait(out_uniquelock, [this]() -> bool
                         {
                             if (this->msgQueue.empty())
                                 return false;
                             else
                                 return true;
                         });
            int value = msgQueue.front();
            msgQueue.pop_front();
            out_uniquelock.unlock();
            cout << "this thread id : " << this_thread::get_id() << " outputs a value " << value << endl;

            if (!started_flag && msgQueue.empty())
                break;
        }
    }

private:
    list<int> msgQueue;
    mutex my_mutex;
    condition_variable my_cond;
    bool started_flag = true;
};

int main()
{

    Msg A;
    thread consumer(&Msg::OutputMsg_withCV, &A);
    thread producter(&Msg::InputMsg, &A, 100);

    consumer.join();
    producter.join();
    cout << "main thread end!" << endl;
}