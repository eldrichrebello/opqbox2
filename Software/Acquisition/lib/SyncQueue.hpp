//
// Created by tusk on 8/15/16.
//

#ifndef ACQUISITION_LOCKFREEQUEUE_H
#define ACQUISITION_LOCKFREEQUEUE_H

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

template <typename T>
class SyncQueue
{
public:

    SyncQueue(){
        _count = 0;
    }

    T pop()
    {
        std::unique_lock<std::mutex> mlock(_mutex);
        while (_queue.empty())
        {
            cond_.wait(mlock);
        }
        auto item = _queue.front();
        _queue.pop();
        _count--;
        return item;
    }

    void pop(T& item)
    {
        std::unique_lock<std::mutex> mlock(_mutex);
        while (_queue.empty())
        {
            cond_.wait(mlock);
        }
        item = _queue.front();
        _queue.pop();
        _count--;
    }

    void push(const T& item)
    {
        std::unique_lock<std::mutex> mlock(_mutex);
        _queue.push(item);
        _count++;
        mlock.unlock();
        cond_.notify_one();

    }

    void push(T&& item)
    {
        std::unique_lock<std::mutex> mlock(_mutex);
        _queue.push(std::move(item));
        _count++;
        mlock.unlock();
        cond_.notify_one();

    }

    void clear(){
        std::unique_lock<std::mutex> mlock(_mutex);
        std::queue<T>().swap(_queue);
        _count = 0;


    }

    size_t size(){
        return _count;
    }

private:
    std::queue<T> _queue;
    std::mutex _mutex;
    std::condition_variable cond_;
    size_t _count;
};

#endif //ACQUISITION_LOCKFREEQUEUE_H
