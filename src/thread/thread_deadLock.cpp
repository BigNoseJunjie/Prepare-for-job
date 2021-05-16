#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <chrono>
using namespace std;

void worker1()
{
    cout << "thread-1, thread id :" << std::this_thread::get_id()<<" run begin!" << endl;
    std::chrono::milliseconds wait_time(2000);
    std::this_thread::sleep_for(wait_time);
    cout << "thread-1 2秒唤醒，run end!" << endl;
}

void worker2(int val, string info)
{

    cout << "thread-2, thread id :" << std::this_thread::get_id()<< "run begin!" << endl;
    cout << "thread-2 args[val:" << val << ",info:" << info << "]" << endl;
    // 线程2睡眠4秒
    std::this_thread::sleep_for(std::chrono::seconds(4));
    cout << "thread-2 4秒唤醒，run end!" << endl;
}

int main()
{

    thread t1(worker1);
    thread t2(worker2, 20, "hello, world!");

    t1.join();
    t2.join();

    cout << "main thread end!" << endl;
    return 0;
}
