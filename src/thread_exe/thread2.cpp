#include <iostream>
#include <thread>

int main()
{

    auto mylamda = [] {
        std::cout << "use lamda!" << std::endl;
        std::cout << "sub thread end!" << std::endl;
    };

    std::thread t(mylamda);

    if (t.joinable())
    {
        std::cout << "use t.join()!" << std::endl;
        t.join();
    }
    else
    {
        std::cout << "t.joinable() is false" << std::endl;
    }

    std::cout << "main thread end!" << std::endl;
}