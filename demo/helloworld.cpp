#include <iostream>
#include <Engine.h>
using namespace Ligmengine;

int main(int argc, const char* argv[]) {
    std::cout << "Hello, World!\n";
    Ligmengine::gEngine.Startup();
    return 0;
}