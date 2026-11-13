#include "includes/Hooks.h"
#include <plugin.h>

using namespace plugin;

class Home {
public:
    Home() {
        Hooks::InitHooksSA();
    }
} home;
