#include "aigc/AIGCDef.h"
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

// void getSecret()
// {
//     HttpHelper::Get()
// }

struct User
{
    int id;
    string email;
    string display_name;
    string country_code;
    string avatar;
    
    string offer;
    string user_auth_token;

    string username;
    string password;
    string appId;

    AIGC_JSON_HELPER(id, email, display_name, country_code, avatar)
};


void testLogin()
{
    string username = "Yaron202101@icloud.com";
    string password = "ede4477fe3b8ba54cde94b9049cba838";
    string appId = "950096963";

    string url = "https://www.qobuz.com/api.json/0.2/user/login";
    url += "?username=" + username + "&email=" + username + "&password=" + password + "&extra=partner" + "&app_id=" + appId;
    vector<string> headers = {
        "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:67.0) Gecko/20100101 Firefox/67.0",
        "Authorization:Basic WlRKbE9XTmhaR1V0TnpsbVpTMDBaR1UyTFRrd1lqTXRaRGsxT0RSbE1Ea3dPRE01Ok1UUmpaVFZqTTJFdE9HVmxaaTAwT1RVM0xXRm1Oamt0TlRsbE9ERmhObVl5TnpJNQ==",
        "Content-Type: application/x-www-form-urlencoded",};
    HttpHelper::Response resp = HttpHelper::Get(url, {}, headers);



    return;
}


int main()
{
    testLogin();
    bool check= PathHelper::Mkdirs("./111/222/333");
    HttpHelper::Response html2 = HttpHelper::Get("www.baidu.com");

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
