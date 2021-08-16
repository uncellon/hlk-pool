#include "hlk/pool/pool.h"

#include <iostream>
#include <unistd.h>

void function(int number) {
    std::cout << "Number from function: " << number << std::endl;
}

int main(int argc, char *argv[]) {
    Hlk::Pool tp;

    Hlk::WorkBase wb;
    Hlk::Delegate<void, int> del([] (int number) {
        std::cout << "Number from delegate: " << number << std::endl;
    });
    wb.set(std::move(del), 1);

    sleep(3);

    return 0;
}