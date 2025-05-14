#include "invisible-hand-hr-system.h"

#include "../Database/individualUserDAO.h"
#include "../Model/individualUser.h"

using namespace std;

void init()
{
    Config::loadConfig();
    DAL::initDatabase();
}

void quickTest()
{
    return;
}

int main()
{
    init();

    quickTest();//开发中测试用

    crow::SimpleApp app;
    app.loglevel(crow::LogLevel::Debug);
    CROW_ROUTE(app, "/")([]() { return "Hello world"; });
    app.port(6970).multithreaded().run();
    return 0;
}
