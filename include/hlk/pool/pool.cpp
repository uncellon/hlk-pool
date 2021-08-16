#include "pool.h"

namespace Hlk {

/******************************************************************************
 * Constructors / Destructors
 *****************************************************************************/

// Pool::Pool(unsigned int threadCount) {
//     m_running = true;
//     for (unsigned int i = 0; i < threadCount; ++i) {
//         m_threads.emplace_back(new std::thread([this] () {
//             while (m_running) {

//             }
//         }));
//     }
// }

// Pool::~Pool() {
//     m_running = false;
//     for (size_t i = 0; i < m_threads.size(); ++i) {
//         m_threads[i]->join();
//     }
// }

/******************************************************************************
 * Public methods
 *****************************************************************************/

// template<class... TParams>
// void Pool::work(Hlk::Delegate<void, TParams...> delegate, TParams... params) {
//     delegate(params...);
// }

} // namespace Hlk