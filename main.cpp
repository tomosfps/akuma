#ifdef _WIN32
#include <SDL.h>
#endif

#include "core/application.h"

int main() {
    core::Application app(800, 600, "akuma");
    app.run();
    return 0;
}