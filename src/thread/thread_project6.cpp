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
            my_cond.notify_one();
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