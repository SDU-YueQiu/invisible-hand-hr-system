#include "invisible-hand-hr-system.h"
using namespace std;

void init()
{
    Config::loadConfig();
    DAL::initDatabase();
}

int main()
{
    init();

    crow::SimpleApp app;
    app.loglevel(crow::LogLevel::Debug);
    CROW_ROUTE(app, "/")([]() { return "Hello world"; });
    app.port(6970).multithreaded().run();
    return 0;
}
