#ifndef HLK_POOL_POOL_H
#define HLK_POOL_POOL_H

#include <queue>
#include <thread>
#include <vector>
#include <hlk/events/delegate.h>

namespace Hlk {

class WorkBase {
public:
    virtual ~WorkBase() { }

    template<class... TArgs>
    void set(Delegate<void, TArgs...>&& delegate, TArgs&&... args);
};

template <class TDelegate, class... TArgs>
class Work : public WorkBase {
public:
    void set(TDelegate&& delegate, TArgs&&... args) { 
        m_delegate = std::move(delegate);
        m_args = std::move(args...);
    }

protected:
    TDelegate m_delegate;
    std::tuple<TArgs...> m_args;
};

template<class... TArgs>
void WorkBase::set(Delegate<void, TArgs...>&& delegate, TArgs&&... args) {
    dynamic_cast <Work<Delegate<void, TArgs...>> &>(*this).set(std::move(delegate), std::move(args...));
}

class Pool {
public:
    /**************************************************************************
     * Constructors / Destructors
     *************************************************************************/

    Pool(unsigned int threadCount = std::thread::hardware_concurrency()) {
        m_running = true;
        for (unsigned int i = 0; i < threadCount; ++i) {
            m_threads.emplace_back(new std::thread([this] () {
                while (m_running) {

                }
            }));
        }
    }

    ~Pool() {
        m_running = false;
        for (size_t i = 0; i < m_threads.size(); ++i) {
            m_threads[i]->join();
        }
    }

    /**************************************************************************
     * Public methods
     *************************************************************************/

protected:
    /**************************************************************************
     * Protected members
     *************************************************************************/

    std::vector<std::thread *> m_threads;

    bool m_running = false;
};

} // namespace Hlk

#endif // HLK_POOL_POOL_H