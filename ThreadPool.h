#pragma once

#include <queue>
#include <functional>
#include "Thread.h"
#include "CSLock.h"
#include "Condition.h"
#include "StringHelper.h"

class ThreadPool
{
public:
    typedef std::function<void()> Task;

public:
    ThreadPool()
        : lock_()
        , notEmpty_(lock_)
        , notFull_(lock_)
        , maxTaskCount_(20)
        , tasks_()
        , threads_()
        , running_(false)
    {
    }

    ~ThreadPool()
    {
        stop();
    }

public:
    void start(int threadNum)
    {
        CSGuard(lock_);
        if(running_)
        {
            return;
        }

        running_ = true;
        for(int i = 0; i < threadNum; i++)
        {
            std::shared_ptr<Thread> taskThread = 
                std::make_shared<Thread>(
                    std::bind(&ThreadPool::runInThread, this),
                    StringHelper::Format("thread%d", i)
                );

            threads_.push_back(taskThread);
            taskThread->start();
        } 
    }

    void stop()
    {
        {
            //必须将lock_范围限定在这里，不能包括join，否则take中的wait会返回不了（wait release lock之后被触发，但是获取不到lock）。
            CSGuard(lock_);
            running_ = false;
            notEmpty_.notifyAll();
        }

        for(size_t i = 0; i < threads_.size(); i++)
        {
            threads_[i]->join();
        }

        // 可能会有任务没有执行完
    }


    void add(const Task& task)
    {
        CSGuard(lock_);
        while(maxTaskCount_ > 0 && tasks_.size() >= maxTaskCount_)
        {
            notFull_.wait();
        }
        tasks_.push(task);
        notEmpty_.notify();
    }

private:
    Task take()
    {
        //此时lock_必须是锁定状态
        while(tasks_.empty() && running_)
        {
            notEmpty_.wait();
        }

        Task task;
        if(!tasks_.empty())
        {
            task = tasks_.front();
            tasks_.pop();

            if(maxTaskCount_ > 0)
            {
                notFull_.notify();
            }
        }
               
        return task;
    }

    void runInThread()
    {
        CSGuard(lock_);
        while(running_)
        {
            Task task = take();
            if(task)
            {
                task();
            }
        }
    }

private:
    CSLock  lock_;
    Condition notEmpty_;
    Condition notFull_;
    size_t maxTaskCount_;
    std::queue<Task> tasks_;
    std::vector<std::shared_ptr<Thread>> threads_;
    bool running_;
};

