
#include "Base64Helper.h"
#include "StringHelper.h"
#include "HttpHelper.h"
#include "RandomHelper.h"
#include "ChineseHelper.h"
#include "GithubHelper.h"
#include "CRCHelper.h"
#include "TCPClientHelper.h"
#include "TCPServerHelper.h"
#include "TimeHelper.h"

using namespace std;
using namespace aigc;

void NewConnect(TCPServerHelper::ClientInfo info)
{
    string message = "[Connect] Socket:" + std::to_string(info.socket) +
                     " IP:" + info.ip +
                     " Port:" + std::to_string(info.port);
    std::cout << message << std::endl;
}

void DisConnect(TCPServerHelper::ClientInfo info, string error)
{
    string message = "[DisConnect] Socket:" + std::to_string(info.socket) +
                     " IP:" + info.ip +
                     " Port:" + std::to_string(info.port) +
                     " Msg:" + error;
    std::cout << message << std::endl;
}

void ReceiveData(TCPServerHelper::ClientInfo info, char *buffer, int len)
{
    string message = "[Receive] Socket:" + std::to_string(info.socket) +
                     " IP:" + info.ip +
                     " Port:" + std::to_string(info.port) +
                     " Data:" + buffer +
                     " Len:" + std::to_string(len);
    std::cout << message << std::endl;
}

class TestPtr
{
private:
    /* data */
public:
    TestPtr(/* args */);
    ~TestPtr();

    int age;
};

TestPtr::TestPtr(/* args */)
{
    age = 0;
    std::cout << "创建Tets" << std::endl;
}

TestPtr::~TestPtr()
{
    std::cout << "释放Test" << std::endl;
}
typedef shared_ptr<TestPtr> TestSmatrPtr;

TestSmatrPtr CreatPtr()
{
    TestSmatrPtr ptr(new TestPtr());
    ptr->age = 1;
    return ptr;
}

bool CreatPtr(TestSmatrPtr &inptr)
{
    TestSmatrPtr ptr(new TestPtr());
    inptr = ptr;
    return true;
}

list<TestSmatrPtr> g_lis;
void Add(TestSmatrPtr ptr)
{
    g_lis.push_back(ptr);
    long c = ptr.use_count();
    return;
}

void Get(TestSmatrPtr &ptr)
{
    long c = ptr.use_count();
    ptr = g_lis.front();
    c = ptr.use_count();
    g_lis.pop_back();
    c = ptr.use_count();
}

void CallTets()
{
    TestSmatrPtr ptr = CreatPtr();
    Add(ptr);
    TestSmatrPtr newptr;
    Get(newptr);

    long c = ptr.use_count();
    return;
}

int main()
{
    CallTets();
    string str = "hello world";
    string ret = Base64Helper::Encode(str);
    string var = Base64Helper::Decode(ret);

    str = " tess ";
    str = StringHelper::Trim(str);

    SocketHelper socket;
    str = socket.GetLocalIp();

    // TCPServerHelper server("192.168.0.141", 6801);
    // while (1)
    // {
    //     if(!server.Select(0, NewConnect, DisConnect, ReceiveData))
    //         break;
    //     break;

    //     // if (!server.Select(0, NULL, NULL, [](TCPServerHelper::ClientInfo info, char *buffer, int len) {
    //     //         string message = "[Receive] Socket:" + std::to_string(info.socket) +
    //     //                          " IP:" + info.ip +
    //     //                          " Port:" + std::to_string(info.port) +
    //     //                          " Data:" + buffer +
    //     //                          " Len:" + std::to_string(len);
    //     //         std::cout << message << std::endl;
    //     //     }))
    //     //     break;
    // }
    // server.CloseListen();
    // return 0;
    // HttpHelper::Response html2 = HttpHelper::Get("www.baidu.com");
    TCPClientHelper client;
    bool isc = client.Connect("192.168.0.141", 6800);
    // while(1)
    // {
    //     if (client.Send("test") <= 0)
    //         break;
    //     TimeHelper::SleepMS(500);
    // }

    str = client.Receive(1024, 100000);

    std::cout << "[debug] " + client.GetLastErrorMessage() << std::endl;
    return 0;

    HttpHelper::Response resp = HttpHelper::Get("www.yaronzz.top");
    GithubHelper::GetLatestVersion("yaronzz", "Tidal-Media-Downloader");
    return 0;
}
