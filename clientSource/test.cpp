#include "client.h"
#include <iostream>
#include <Windows.h>
#include <json/json.h>
#include <fstream>
#include <cstring>
#include <cryptopp/cryptlib.h>
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <cryptopp/md5.h>
#include <cryptopp/files.h>
#include <cryptopp/hex.h>

#include <iostream>
#include <cstddef> 

std::string encryptMd5(std::string str)
{
    std::string strMd5;
    CryptoPP::Weak1::MD5 hash;
    hash.Update((const unsigned char*)str.c_str(), str.length());
    strMd5.resize(hash.DigestSize());
    hash.Final((unsigned char*)&strMd5[0]);
    return strMd5;
}

using namespace std;
using namespace Json;

enum Function
{
    Login, Register, ChangePrior, AddProject, DelProject
};

void createJson()
{
    string jsonStr;
    Value root, userInfo, userArray;
    StreamWriterBuilder writerBuilder; // 新式API
    ostringstream os;
    Value arrayObj;

    try
    {
        // 设置默认无格式化的输出
        //writerBuilder.settings_["indentation"] = "";

        userInfo["username"] = "wangbingzhi";
        userInfo["password"] = "girlfriend";
        root.append(userInfo);

        userInfo["username"] = "yellowmorn";
        userInfo["password"] = "boy";
        root.append(userInfo);

        userArray["userInfo"] = Json::Value(root);

        root["function"] = "Login";

        // 这里使用智能指针
        unique_ptr<StreamWriter> jsonWriter(writerBuilder.newStreamWriter());
        jsonWriter->write(root, &os); // json-->stringstream
        jsonStr = os.str(); // 转为string

        /*Json::Value aim;
        Json::String errs;
        Json::CharReaderBuilder readBuilder;
        std::unique_ptr<Json::CharReader> jsonRead(readBuilder.newCharReader());
        if (nullptr == jsonRead) {
            std::cerr << "jsonRead is null" << std::endl;
            return;
        }*/

        //std::string username = root["userInfo"].asString();
        //std::string password = root["userInfo"][i]["password"].asString();
        //std::cout << ":" << username << " " << std::endl;
        //Client client;
        //client.sendData(jsonStr);
        //// 无格式化的输出
        std::cout << "Json-none:\n" << jsonStr << std::endl;
        //// 格式化的输出
        std::cout << "Json-formatted:\n" << root.toStyledString() << std::endl;
    }
    catch (const std::exception e)
    {
        std::cout << e.what() << std::endl;
    }
}

int main()
{
    //writeJson();
	//char buf2[10] = "world";
    //readJson();
    //createJson();
	//CreateThread(NULL, 0, thread1, (void*)buf1, 0, NULL);
	//CreateThread(NULL, 0, thread1, (void*)buf2, 0, NULL);
	/*Sleep(10000);*/
    // 构建json数组
    Json::Value userArray;
    Json::Value root;
    Json::Value person;

    StreamWriterBuilder writerBuilder; // 新式API
    ostringstream os;

    //root.append(userArray);
    person["username"] = "yellow";
    person["password"] = "we";
    //person["sex"] = "male";
    root["userArray"].append(person);
    root["function"] = "login";

    /*person["name"] = "keiv";
    person["age"] = 20;
    person["sex"] = "female";
    root.append(person);

    person["name"] = "lihua";
    person["age"] = 10;
    person["sex"] = "female";
    root.append(person);*/

    // 添加数组格式
    //userArray["userArray"].append(root);

    // 子节点挂到根节点上
    //userArray["userArray"] = Json::Value(root);

    // 这里使用智能指针
    unique_ptr<StreamWriter> jsonWriter(writerBuilder.newStreamWriter());
    jsonWriter->write(root, &os); // json-->stringstream
    string data = os.str(); // 转为string

    cout<<data<<endl;  
    //cout<<userArray.toStyledString()<<endl;


    // 解析Json字符串
    string strValue = root.toStyledString();      // json对象转变为json字符串
    cout << strValue << endl;

    /*Json::Value value;
    Json::String errs;
    Json::CharReaderBuilder readBuilder;

    std::unique_ptr<Json::CharReader> jsonRead(readBuilder.newCharReader());
    if (nullptr == jsonRead) {
        std::cerr << "jsonRead is null" << std::endl;
        return 0;
    }*/

    // reader将Json字符串解析到root，root将包含Json里所有子元素
    //bool ret = jsonRead->parse(strValue.c_str(), strValue.c_str() + strValue.length(), &value, &errs);
    //if (!ret || !errs.empty())// json字符串转为json对象
    //{
    //    std::cout << "parseJsonFromString error!" << errs << std::endl;
    //    return 0;
    //}
    Client client;
    client.sendData(strValue);

    /*for (unsigned int i = 0; i < value["userArray"].size(); i++)
    {
        string name = value["userArray"][i]["name"].asString();
        int     age = value["userArray"][i]["age"].asInt();
        string sex = value["userArray"][i]["sex"].asString();

        cout << name << " " << age << " " << sex << endl;
    }*/

	return 0;
}