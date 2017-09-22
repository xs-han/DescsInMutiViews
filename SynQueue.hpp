//
// Created by xushen on 9/14/17.
//

#ifndef DESCSINMUTIVIEWS_SYNQUEUE_H
#define DESCSINMUTIVIEWS_SYNQUEUE_H

#include <mutex>
#include <thread>
#include <condition_variable>
#include <iostream>
#include <deque>

template<typename T>
class SynQueue
{
public:
    //构造函数
    explicit SynQueue(int MaxSize):
            m_maxsize(MaxSize), finish(false) { }

    //将T类型对象放入队列
    void put(const T &x);

    //将T类型对象从队列取出
    void take(T &x);

    //判断队列是否为空
    bool empty();

    //判断队列是否为满
    bool full();

    //返回队列大小
    size_t size();

    void clear();

    void setFinish(bool fin);

private:
    //判断空或满,内部使用不需要加锁
    bool isFull() const
    {
        return m_queue.size() == m_maxsize;
    }
    bool isEmpty() const
    {
        return m_queue.empty();
    }
    bool isFinish() const
    {
        return finish;
    }

private:
    //队列
    std::deque<T>m_queue;
    bool finish;
    //互斥锁
    std::mutex m_mutex;
    //不为空时的条件变量
    std::condition_variable_any m_notEmpty;
    //不为满时的条件变量
    std::condition_variable_any m_notFull;
    //队列最大长度
    //std::condition_variable_any m_notFinish;
    int m_maxsize;
};

template<class T>
void SynQueue<T>::put(const T &x) {
    std::lock_guard<std::mutex>locker(m_mutex);
    while(isFull() && (!isFinish()))
    {
        //如果满了，等待
        m_notFull.wait(m_mutex);
    }
    if(isFinish()){
        return;
    }
    m_queue.push_back(x);
    //通过条件变量唤醒一个线程，也可以所有线程
    m_notEmpty.notify_one();
}

template<class T>
void SynQueue<T>::take(T &x) {
    std::lock_guard<std::mutex> locker(m_mutex);
    while(isEmpty() && (!isFinish()))
    {
#ifdef Debug
        std::cout << "no resource... please wait" << std::endl;
#endif
        m_notEmpty.wait(m_mutex);
    }
    if(isFinish() && isEmpty()){
#ifdef Debug
        cout << "This queue is marked as finished." << endl;
#endif
        return;
    }
    x = m_queue.front();
    m_queue.pop_front();
    m_notFull.notify_one();
}

template<class T>
bool SynQueue<T>::empty() {
    std::lock_guard<std::mutex> locker(m_mutex);
    return m_queue.empty();
}

template<class T>
bool SynQueue<T>::full() {
    std::lock_guard<std::mutex> locker(m_mutex);
    return m_queue.size() == m_maxsize;
}

template<class T>
size_t SynQueue<T>::size() {
    std::lock_guard<std::mutex> locker(m_mutex);
    return m_queue.size();
}

template<class T>
void SynQueue<T>::clear() {
    std::lock_guard<std::mutex> locker(m_mutex);
    m_queue.clear();
    m_notFull.notify_one();
}

template<class T>
void SynQueue<T>::setFinish(bool fin) {
    std::lock_guard<std::mutex> locker(m_mutex);
    finish = fin;
    if(fin) {
        m_notEmpty.notify_one();
        m_notFull.notify_one();
    }
}

#endif //DESCSINMUTIVIEWS_SYNQUEUE_H
