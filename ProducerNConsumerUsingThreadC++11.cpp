/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>
#include <thread>
#include <mutex>
#include <deque>
#include <condition_variable>

using namespace std;

std::mutex mu;
std::condition_variable cond;
deque<int> buffer;

const unsigned int maxBufferSize = 50;

void producer(int val) {
    int tmp = val;
    while(val) {
        std::unique_lock<std::mutex> locker(mu);
        //std::lock_guard<std::mutex> locker(mu);
        cond.wait(locker, [](){return buffer.size() < maxBufferSize;});
        buffer.push_back(val);
        cout<< "Produced : " << val << endl;
        val--;
        locker.unlock();
        cond.notify_one();
        
        if(val == 0) {
            cout << " Count Reached 0, resetting to 100 \n";
            val = tmp;
        }
    }
}

void consumer () {
    while(true) {
        std::unique_lock<std::mutex> locker(mu);
        //std::lock_guard<std::mutex> locker(mu);
        cond.wait(locker, [](){return buffer.size() > 0;});
        int val = buffer.back();
        buffer.pop_back();
        cout << "Consumed : " << val << endl;
        locker.unlock();
        cond.notify_one();
    }
}

int main()
{
    cout<<"Hello World \n";

    std::thread t1(producer, 100);
    std::thread t2(consumer);
    
    t1.join();
    t2.join();
    return 0;
}
