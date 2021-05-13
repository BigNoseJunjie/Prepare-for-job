#include <iostream>
#include <thread>
#include <memory>
using namespace std;

void myprint2(unique_ptr<int> pzn)
{
    ;
}

void myprint3(shared_ptr<int> pzn)
{
    cout<<"sub thread end!"<<endl;
}

int main()
{
    unique_ptr<int> myp(new int(100));
    //注意unique_ptr不能拷贝或者赋值，要通过move来传递所有权
    shared_ptr<int> mys(new int(100));
    thread mytobj(myprint3, mys);
    mytobj.join();
    // mytobj.detach();
    return 0;
}