#include<iostream>
#include<thread>
#include<string>
#include<vector>
#include<list>
 
using namespace std;
 
//准备用成员函数作为线程函数的方法写线程
class A
{
public:
	//把收到的消息入到一个队列的线程
	void inMsgRecvQueue() 
	{
		for (int i = 0; i < 100;i++)
		{
			cout << "inMsgRecvQueue()执行，插入一个元素"<< i<<endl;
			msgRecvQueue.push_back(i); //假设这个数字i就是收到的命令，直接弄到消息队列里边来；
		}
	}
 
	//把数据从消息队列取出的线程
	void outMsgRecvQueue()
	{
		for (int i = 0; i < 100; i++)
		{
			if (!msgRecvQueue.empty())
			{
				//消息不为空
				int command = msgRecvQueue.front();//返回第一个元素，但不检查元素是否存在
				msgRecvQueue.pop_front();//移除第一个元素。但不返回；
				//这里就考虑处理数据
			}
			else
			{
				//消息队列为空
				cout << "inMsgRecvQueue()执行，但目前消息队列中为空！" << i << endl;
			}
		}
		cout <<"end!" << endl;
	}
 
private:
	std::list<int> msgRecvQueue;//容器（消息队列），代表玩家发送过来的命令。
};
 
int main()
{
	A myobja;
 
	std::thread myOutMsgObj(&A::outMsgRecvQueue, &myobja);//第二个参数，引用，才能保证线程里用的是同一个对象
	std::thread myInMsgObj(&A::inMsgRecvQueue, &myobja);
 
	myOutMsgObj.join();
	myInMsgObj.join();
 
	cout << "主线程执行！" << endl;
 
	return 0;
}