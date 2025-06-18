#ifdef _WIN32
#include <SDL.h>
#endif

#include "core/application.h"

int main() {
    core::Application app(1200, 800, "akuma");
    app.run();
    return 0;
}