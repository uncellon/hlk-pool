#ifndef HLK_POOL_POOL_H
#define HLK_POOL_POOL_H

#include <queue>
#include <mutex>
#include <thread>
#include <vector>
#include <future>
#include <condition_variable>
#include <hlk/events/delegate.h>

namespace Hlk {

class BaseTask {
public:
    virtual ~BaseTask() { }

    virtual void execute() = 0;
};

template<class TFunction>
class Task;

// Specialization without return and arguments
template<> 
class Task<void()> : public BaseTask {
public:
    virtual inline void execute() override {
        delegate();
    }

    Delegate<void()> delegate;
};

// Specialization without return and with arguments
template<class... TArgs>
class Task<void(TArgs...)> : public BaseTask {
public:
    virtual inline void execute() override {
        delegate(std::get<TArgs...>(args));
    }

    Delegate<void(TArgs...)> delegate;
    std::tuple<TArgs...> args;
};

// Specialization with return and without arguments
template<class TReturn>
class Task<TReturn()> : public BaseTask {
public:
    virtual inline void execute() override {
        promise.set_value(delegate());
    }

    Delegate<TReturn()> delegate;
    std::promise<TReturn> promise;
};

// Specialization with return and arguments
template<class TReturn, class... TArgs>
class Task<TReturn(TArgs...)> : public BaseTask {
public:
    virtual inline void execute() override {
        promise.set_value(delegate(std::get<TArgs...>(args)));
    }

    Delegate<TReturn(TArgs...)> delegate;
    std::tuple<TArgs...> args;
    std::promise<TReturn> promise;
};

class Pool {
public:
    /**************************************************************************
     * Public methods
     *************************************************************************/

    static Pool* getInstance();

    template<class TTask>
    void pushTask(TTask&& task) {
        m_queueMutex.lock();

        auto unit = new Task<void()>();
        unit->delegate = task;
        m_tasks.push(unit);

        m_queueMutex.unlock();
        m_cv.notify_one();
    }

    template<class TTask, class... TArgs>
    void pushTask(TTask&& task, TArgs&&... args) {
        m_queueMutex.lock();

        auto unit = new Task<void(TArgs...)>();
        unit->delegate = task;
        unit->args = std::make_tuple(std::forward<TArgs...>(args...));
        m_tasks.push(unit);

        m_queueMutex.unlock();
        m_cv.notify_one();
    }

    template<class TReturn, class... TArgs>
    std::future<TReturn> phTask(Delegate<TReturn(TArgs...)> delegate, TArgs&&... args) {
        m_queueMutex.lock();

        auto unit = new Task<TReturn(TArgs...)>();
        unit->delegate = delegate;
        unit->args = std::make_tuple(std::forward<TArgs...>(args...));
        m_tasks.push(unit);

        m_queueMutex.unlock();
        m_cv.notify_one();

        return unit->promise.get_future();
    }

protected:
    /**************************************************************************
     * Constructors / Destructors
     *************************************************************************/

    Pool(unsigned int threadCount = std::thread::hardware_concurrency());
    ~Pool();

    /**************************************************************************
     * Protected methods
     *************************************************************************/

    void threadLoop(int mutexIndex);

    /**************************************************************************
     * Protected members
     *************************************************************************/

    static Pool *m_pool;

    std::vector<std::thread *> m_threads;
    std::vector<std::mutex *> m_mutexes;
    std::queue<BaseTask *> m_tasks;
    std::condition_variable m_cv;
    std::mutex m_queueMutex;

    bool m_running = false;
};

} // namespace Hlk

#endif // HLK_POOL_POOL_H