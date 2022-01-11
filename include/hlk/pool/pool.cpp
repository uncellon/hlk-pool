#include "pool.h"

namespace Hlk {

Pool *Pool::m_pool = nullptr;
std::mutex Pool::m_singletonMutex;

/******************************************************************************
 * Methods
 *****************************************************************************/

Pool* Pool::getInstance() {
    std::unique_lock lock(m_singletonMutex);
    if (!m_pool) {
        m_pool = new Pool();
    }
    return m_pool;
}

/******************************************************************************
 * Constructors / Destructors (Protected)
 *****************************************************************************/

Pool::Pool(unsigned int threadCount) {
    m_running = true;
    for (unsigned int i = 0; i < threadCount; ++i) {
        m_lockMutexes.emplace_back(new std::mutex());
        m_threads.emplace_back(new std::thread(&Pool::workerLoop, this, i));
    }
}

Pool::~Pool() {
    m_running = false;
    for (size_t i = 0; i < m_threads.size(); ++i) {
        m_cv.notify_one();
        m_threads[i]->detach();
        delete m_threads[i];
        delete m_lockMutexes[i];
    }
}

/******************************************************************************
 * Methods (Protected)
 *****************************************************************************/

void Pool::workerLoop(int mutexIndex) {
    std::unique_lock lock(*m_lockMutexes[mutexIndex], std::defer_lock);
    while (m_running) {
        m_queueMutex.lock();
        if (m_tasks.empty()) {
            m_queueMutex.unlock();
            m_cv.wait(lock);
            continue;
        }

        auto task = m_tasks.front();
        m_tasks.pop();

        m_queueMutex.unlock();

        task->execute();
        delete task;
    }
}

} // namespace Hlk