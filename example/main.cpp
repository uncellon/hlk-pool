#include "hlk/pool/pool.h"

#include <future>
#include <string>
#include <iostream>
#include <unistd.h>

void function(int number) {
    std::cout << "Number from function: " << number << std::endl;
}

int main(int argc, char *argv[]) {
    auto tp = Hlk::Pool::getInstance();

    tp->pushTask([] (int a) {
        std::cout << a << std::endl;
    }, 2);
    for (int i = 0; i < 10000; ++i) {
        auto del = Hlk::Delegate<void()>([i] () {
            std::cout << "Task " + std::to_string(i) << "\n";
        });
        tp->pushTask(del);
    }

    auto f = tp->getFuture(Hlk::Delegate<bool(int)>([] (int a) -> bool {
        return true;
    }), 2);

    sleep(5);

    std::cout << "Future result: " << f.get() << std::endl;

    return 0;
}