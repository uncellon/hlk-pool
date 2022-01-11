#ifndef HLK_ABSTRACT_TASK_H
#define HLK_ABSTRACT_TASK_H

namespace Hlk {

class AbstractTask {
public:
    /**************************************************************************
     * Constructors / Destructors
     *************************************************************************/

    virtual ~AbstractTask() = default;

    /**************************************************************************
     * Methods
     *************************************************************************/

    virtual void execute() = 0;
};

} // namespace Hlk

#endif // HLK_ABSTRACT_TASK_H