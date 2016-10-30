#include "MyClass.h"
#include <cstring>

#include <iostream>
#include <list>

#include <chrono>
using namespace std::chrono_literals;   // allow 1s

#include <thread>   // also link -pthread in linker
#include <mutex>

#include <future>

void Status( int id, bool starting )
{
    std::cout << (starting?"start":"done") << " t" << id << " (" << std::this_thread::get_id() << ")" << std::endl;
}

void T2Fn(int id) {
    Status(id,true);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    Status(id,false);
}

int main()
{
    std::cout << "system has " << std::thread::hardware_concurrency() << " cores" << std::endl;

    // Thread by Lambda
    auto T1Fn = [](int id){
        Status(id,true);
        std::this_thread::sleep_for(std::chrono::seconds(2));
        Status(id,false);
    };
    std::thread t1( T1Fn, 1 );  // note arg passed to CTOR rather than function

    // Thread by function pointer
    std::thread t2( T2Fn, 2 ); 
 
    // Wait for threads to finish, else "terminate called without active exception"
    t1.join();
    t2.join();

    // Detach example
    std::thread t3( [](int id){
        Status(id,true);
        for( int i=0; i<8; ++i) {
            std::this_thread::sleep_for(std::chrono::seconds(5));
            std::cout << "t" << id << ": tick " << i << std::endl;
        }
        Status(id,false);
    }, 3 );
    t3.detach();    // if main thread exists before t3 finishes, then t3 terminates
    if( t3.joinable() ) {
        std::cout << "Never happens - t3 was detached, and so not joinable" << std::endl;
    }

    // Pass by reference - std::ref() or std::cref() (to avoid thread ctor causing a copy)
    MyClass bigObject{ strdup("Humongous") };
    std::thread t4( [](int id, MyClass& bigThing){
        Status(id,true);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        Status(id,false);
    }, 4, std::ref(bigObject) );
    t4.join();

    // Returning data from Thread
    std::list<int> sharedData;
    std::mutex sharedDataLock;
    std::thread t5([&sharedData, &sharedDataLock](int id){
        Status(id,true);
        for(int i=0;i<10000;++i){
            sharedDataLock.lock();
            sharedData.push_back( i );
            sharedDataLock.unlock();
        }
        Status(id,false);
    }, 5 );
    std::thread t6([&sharedData, &sharedDataLock](int id){
        Status(id,true);
        for(int i=0;i<10000;++i){
            sharedDataLock.lock();
            sharedData.push_back( i );
            sharedDataLock.unlock();
        }
        Status(id,false);
    }, 6 );
    t5.join();
    t6.join();
    std::cout << "sharedData has " << sharedData.size() << " elements (should be 20,000)" << std::endl;

    // RAII use of mutex - std::lock_guard
    sharedData.clear();
    std::thread t7([&sharedData, &sharedDataLock](int id){
        Status(id,true);
        for(int i=0;i<10000;++i){
            std::lock_guard<std::mutex> myLock(sharedDataLock);
            sharedData.push_back( i );
        }
        Status(id,false);
    }, 7 );
    std::thread t8([&sharedData, &sharedDataLock](int id){
        Status(id,true);
        for(int i=0;i<10000;++i){
            std::lock_guard<std::mutex> myLock(sharedDataLock);
            sharedData.push_back( i );
        }
        Status(id,false);
    }, 8 );
    t7.join();
    t8.join();
    std::cout << "sharedData # 2 has " << sharedData.size() << " elements (should be 20,000)" << std::endl;

    // Higher level: Async Tasks (#include <future>)
    std::future<int> result = std::async( [](int id) {
        Status(id,true);
        std::this_thread::sleep_for(std::chrono::seconds(2));
        Status(id,false);
        return id;
    }, 9 );
    std::cout << "Main thread still running. Will now wait for task...";
    int val = result.get();   // wait for async task to complete
    std::cout << "done: " << val << std::endl;
    if( result.valid())
    {
        std::cout << "won't get here as .get() above destroys/invalidates" << std::endl;
    }

    // Launch Policy
    std::future<int> result10 = std::async( std::launch::deferred, [](int id) {
        Status(id,true);
        std::this_thread::sleep_for(std::chrono::seconds(2));
        Status(id,false);
        return id;
    }, 10 );

    std::cout << "Main thread still running, but t10 isn't untill we call .get...";
    int val10 = result10.get();
    std::cout << "t10 returned " << val10 << std::endl;

    // wait_for
    std::future<int> result11 = std::async( [](int id) {
        Status(id,true);
        std::this_thread::sleep_for(std::chrono::seconds(2));
        Status(id,false);
        return id;
    }, 11 );
    if( result11.valid() ) {
        auto status = result11.wait_for(1s);  // 1s - std::chrono_literals
        // or wait for a specific time;
        //   auto timepoint = std::chrono::system_clock::now();
        //   timepoint += 1s;
        //   auto status = result11.wait_until(timepoint);

        switch(status) {
            case std::future_status::deferred:
                std::cout << "not run" << std::endl;
                break;
            case std::future_status::ready:
                std::cout << "you can .get() now" << std::endl;
                break;
            case std::future_status::timeout:
                std::cout << "still running" << std::endl;
                break;
        }
    }
    if( result11.valid() ) {
        auto status = result11.wait_for(std::chrono::seconds(2));  // or 2s if using std::chrono_literals
        switch(status) {
            case std::future_status::deferred:
                std::cout << "not run" << std::endl;
                break;
            case std::future_status::ready:
                std::cout << "you can .get() now" << std::endl;
                break;
            case std::future_status::timeout:
                std::cout << "still running" << std::endl;
                break;
        }
    }
    int val11 = result11.get();   // wait for async task to complete
    std::cout << "done: " << val11 << std::endl;

    // Promises
    std::promise<int> mainThreadsPromise;

    std::future<int> result12 = std::async( [](int id, std::promise<int>& data) {
        Status(id,true);
        auto f12 = data.get_future();
        auto value = f12.get();     // wait for main thread (promise owner) to set data
        std::cout << "t" << id << " got " << value << std::endl;
        Status(id,false);
        return id;
    }, 12, std::ref(mainThreadsPromise) );

    std::cout << "Main sleeping on promise for 2 secs..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Main setting promise value: ";
    mainThreadsPromise.set_value(12);   // can only set once

    return 0;
}