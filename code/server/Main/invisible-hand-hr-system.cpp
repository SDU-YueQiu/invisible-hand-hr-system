#include "invisible-hand-hr-system.h"
using namespace std;

int main()
{
    crow::SimpleApp app;
    CROW_ROUTE(app, "/")([]() { return "Hello world"; });
    cout<<"yes"<<endl;
    app.port(6970).multithreaded().run();
    return 0;
}
