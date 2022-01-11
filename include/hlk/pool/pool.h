#ifndef HLK_POOL_POOL_H
#define HLK_POOL_POOL_H

#include "task.h"

#include <queue>
#include <mutex>
#include <thread>
#include <vector>
#include <condition_variable>

namespace Hlk {

class Pool {
public:
    /**************************************************************************
     * Methods
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
    std::future<TReturn> getFuture(Delegate<TReturn(TArgs...)> delegate, TArgs&&... args) {
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
     * Constructors / Destructors (Protected)
     *************************************************************************/

    Pool(unsigned int threadCount = std::thread::hardware_concurrency());
    ~Pool();

    /**************************************************************************
     * Methods (Protected)
     *************************************************************************/

    void workerLoop(int mutexIndex);

    /**************************************************************************
     * Members
     *************************************************************************/

    static Pool *m_pool;
    static std::mutex m_singletonMutex;

    std::vector<std::thread *> m_threads;
    std::vector<std::mutex *> m_lockMutexes;
    std::queue<AbstractTask *> m_tasks;
    std::condition_variable m_cv;
    std::mutex m_queueMutex;

    bool m_running = false;
};

} // namespace Hlk

#endif // HLK_POOL_POOL_H