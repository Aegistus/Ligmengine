#include <iostream>
#include <Engine.h>
#include <InputCode.h>
using namespace Ligmengine;

int main(int argc, const char* argv[]) {
    std::cout << "Hello, World!\n";
    Ligmengine::gEngine.Startup([&]()
        {
            if (gEngine.input.GetKey(InputCode::KEY_A))
            {
                std::cout << "TEST\n";
            }
            if (gEngine.input.GetKey(InputCode::KEY_ESCAPE))
            {
                gEngine.quit = true;
            }
        });
    return 0;
}