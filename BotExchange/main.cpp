#include <iostream>
#include <enet/enet.h>

int main() {
    if (enet_initialize() != 0) {
        std::cout << "An error occurred while initializing ENet!\n";
        return EXIT_FAILURE;
    }

    enet_deinitialize();
}