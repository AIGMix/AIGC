// AIGC.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "AIGCBase.h"
#include "StringHelper.h"
#include "HttpHelper.h"
#include "JsonHelper.h"

#include "TidalClient.h"

using namespace std;
using namespace AIGC;

#pragma comment(lib, "ws2_32.lib")


int main()
{
    string strJson = "{\"name\":\"MenAngel\",\"age\":23,\"hobbys\":{\"age\":23}}";
    JsonHelper::GetValue(strJson, { "hobbys","age"});

    LoginKey key = TidalClient::Login("tidalhuangus2007@qq.com", "bitchjolin", "wc8j_yBJd20zOmx0");

    map<string, string> parameters = { 
        {"username", "tidalhuangus2007@qq.com"}, 
        {"password", "bitchjolin"},
        {"token", "wc8j_yBJd20zOmx0"},
        {"clientUniqueKey", "834e749c80337a6d"},
        {"clientVersion", "1.9.1"},
    };

    vector<string> headers = {
        "Content-Type: application/x-www-form-urlencoded",
    };

    HttpHelper::Result result = HttpHelper::Post("https://api.tidalhifi.com/v1/login/username", parameters, headers);
    return 0;
}

