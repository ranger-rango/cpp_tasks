#include <drogon/drogon.h>
#include <string>
using namespace std;
using namespace drogon;

int main() {
    app().loadConfigFile("../config.json");
    app().run();
    return 0;
}

// extern "C" void start_server()
// {
//     app().loadConfigFile("config.json").run();
// }
