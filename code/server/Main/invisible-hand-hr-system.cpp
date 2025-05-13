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
    auto &dbm = DAL::DatabaseManager::getInstance();
    DAL::IndividualUserDAO iudao;
    int id=iudao.create(Model::IndividualUser(-1, "testuser", "123", "", "123@123.mail", time(nullptr), time(nullptr), "testing", ""));
    auto user = iudao.findByUsername("123");
    auto user2=iudao.findById(id);
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
