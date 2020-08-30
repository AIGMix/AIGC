// AIGC.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <iostream>

#include "StringHelper.h"
#include "HttpHelper.h"
#include "JsonHelper.h"
#include "TidalClient.h"

using namespace std;
using namespace AIGC;

class Animal
{
public:
    virtual void eat()
    {
        cout << "Animal eat" << endl;
    }
    virtual int sss() = 0;

};

class Dog : public Animal
{
private:
    void eat()
    {
        cout << "dog eat" << endl;
    }
    int sss()
    {

    }
};

int main(char arg[100])
{
    string chinese = "中文字符";
    string english = "english";
    string number = "11035";

    string sub = StringHelper::Substring(english, "n", "1", true);

    string english2 = StringHelper::ToUpper(english);

    bool bflag;
    bflag = StringHelper::IsChinese(chinese);
    bflag = StringHelper::IsEnglish(english);
    bflag = StringHelper::IsNumeric(number);

    Animal *A = new Dog;
    A->eat();

    char* p = new char[10];
    int size = 0;
    char ss[] = "hello";
    size = sizeof(HttpHelper);
    // size = sizeof(p);
    // size = sizeof(size);


    std::cout
        << "hello" << std::endl;
    string strJson = "{\"name\":\"MenAngel\",\"age\":23,\"hobbys\":{\"age\":23}}";


    string value = JsonHelper::GetValue(strJson, {"name"}, string(""));

    return 0;


    return 0;
}

