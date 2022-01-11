#ifndef HLK_TASK_H
#define HLK_TASK_H

#include "abstracttask.h"

#include <future>
#include <hlk/events/delegate.h>

namespace Hlk {

template<class TFunction>
class Task;

// Specialization without return and arguments
template<> 
class Task<void()> : public AbstractTask {
public:
    /**************************************************************************
     * Methods
     *************************************************************************/

    virtual void execute() override {
        delegate();
    }

    /**************************************************************************
     * Members
     *************************************************************************/

    Delegate<void()> delegate;
};

// Specialization without return and with arguments
template<class... TArgs>
class Task<void(TArgs...)> : public AbstractTask {
public:
    /**************************************************************************
     * Methods
     *************************************************************************/

    virtual void execute() override {
        delegate(std::get<TArgs...>(args));
    }

    /**************************************************************************
     * Members
     *************************************************************************/

    Delegate<void(TArgs...)> delegate;
    std::tuple<TArgs...> args;
};

// Specialization with return and without arguments
template<class TReturn>
class Task<TReturn()> : public AbstractTask {
public:
    /**************************************************************************
     * Methods
     *************************************************************************/

    virtual void execute() override {
        promise.set_value(delegate());
    }

    /**************************************************************************
     * Members
     *************************************************************************/

    Delegate<TReturn()> delegate;
    std::promise<TReturn> promise;
};

// Specialization with return and arguments
template<class TReturn, class... TArgs>
class Task<TReturn(TArgs...)> : public AbstractTask {
public:
    /**************************************************************************
     * Methods
     *************************************************************************/

    virtual void execute() override {
        promise.set_value(delegate(std::get<TArgs...>(args)));
    }

    /**************************************************************************
     * Members
     *************************************************************************/

    Delegate<TReturn(TArgs...)> delegate;
    std::tuple<TArgs...> args;
    std::promise<TReturn> promise;
};

} // namespace Hlk

#endif // HLK_TASK_H