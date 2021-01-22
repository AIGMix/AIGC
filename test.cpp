#include "aigc/AIGCDef.h"

using namespace aigc;
using namespace std;

#define TEST_HOMG(...) \
    printf(#__VA_ARGS__)



class base
{
public:
    int baseA;
    string baseB;
    AIGC_JSON_HELPER(baseA, baseB)
};

class classA
{
public:
    int a;
    int b;
    AIGC_JSON_HELPER(a,b)
};

class classB : public base
{
public:
    int a;
    int b;

    classA claA;
    AIGC_JSON_HELPER(a, b, claA)
    AIGC_JSON_HELPER_BASE((base*)this)
};



int main()
{
    string url = "http://44318.xc.05cg.com/xiaz/Navicat%20for%20SQLite%2012%E4%B8%AD%E6%96%87%E7%A0%B4%E8%A7%A3%E7%89%88%20@314_38234.exe";
    bool ccc = DownloadHelper::Download(url, "e:\\test.exe");
    //string msg = HttpHelper::GetString(url);
    int filesize = DownloadHelper::GetFileSize(url);
    std::string ip = NetHelper::GetIP("www.baidu.com", 0);

    //GithubHelper::GetLatestVersion("yaronzz", "Tidal-Media-Downloader-PRO");

    classB recordB;
    string buffer = R"({"a":0,
                        "b":1,
                        "baseA":3,
                        "baseB":4,
                        "claA":{"a":"11",
                                "b":22}})";
    string message;
    bool ret = JsonHelper::JsonToObject(recordB, buffer, {}, &message);

    string a = JsonHelper::Get<string>(buffer, "", { "claA", "a" });

    //int check = StringHelper::Count("fff.", "f\\.");
    //StringHelper::IsPhoneNumber("14750960874");
    TEST_HOMG("ff"; 0; 0);

    string path = "E:/OneDrive/code/AIGC/test.txt";
    FileHelper::GetSize(path);
    return 0;
}



