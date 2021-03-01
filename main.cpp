#include    <iostream>
#include    "Request.h"

int main()
{
    Request req = { "150.107.72.204", 80, "/" };
    req.Connect();

    std::string response = req.Send();
    req.Terminate();

    std::cout << response << std::endl;
    std::cout << "Response time: " << req.getTime() << " seconds";

    return 0;
}