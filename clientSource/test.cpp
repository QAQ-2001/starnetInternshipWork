#include "client.h"
#include "cOperation.h"
#include <iostream>
#include <Windows.h>
#include <json/json.h>
#include <fstream>
#include <cstring>

#include <thread>
#include <future>

using namespace std;
using namespace Json;

#include "code.h"

enum Function
{
    Login, Register, ChangePrior, AddProject, DelProject
};

void hanshu(std::string strValue, string& token)
{
    Client client;
    client.sendData(strValue);
    token = client.recvData();
}

void myTime(std::string strValue)
{
    Client client;
    client.sendData(strValue);
    client.recvData();
}

std::string createString2()
{
    Json::Value root;
    Json::Value person;

    StreamWriterBuilder writerBuilder; // 新式API
    ostringstream os;
    writerBuilder.settings_["emitUTF8"] = true;

    //root.append(userArray);
    person["loginName"] = "T2021";
    person["password"] = encryptMd5("34vs");

    root["userArray"].append(person);
    root["function"] = "login";

    // 这里使用智能指针
    unique_ptr<StreamWriter> jsonWriter(writerBuilder.newStreamWriter());
    jsonWriter->write(root, &os); // json-->stringstream
    string data = os.str(); // 转为string

    // 解析Json字符串
    //string strValue = root.toStyledString();      // json对象转变为json字符串
    return data;
}

std::string createString1()
{
    Json::Value root;
    Json::Value person;

    StreamWriterBuilder writerBuilder; // 新式API
    ostringstream os;

    //root.append(userArray);
    person["loginName"] = "T2022";
    person["password"] = encryptMd5("wqe");
    //person["sex"] = "male";
    root["userArray"].append(person);
    root["function"] = "login";
    writerBuilder.settings_["emitUTF8"] = true;

    // 这里使用智能指针
    unique_ptr<StreamWriter> jsonWriter(writerBuilder.newStreamWriter());
    jsonWriter->write(root, &os); // json-->stringstream
    string data = os.str(); // 转为string

    return data;
}

std::string createString3()
{
    Json::Value root;
    Json::Value person;

    StreamWriterBuilder writerBuilder; // 新式API
    ostringstream os;
    writerBuilder.settings_["emitUTF8"] = true;

    //root.append(userArray);
    person["loginName"] = "T2023";
    person["password"] = encryptMd5("test");
    //person["sex"] = "male";
    root["userArray"].append(person);
    root["function"] = "login";

    // 这里使用智能指针
    unique_ptr<StreamWriter> jsonWriter(writerBuilder.newStreamWriter());
    jsonWriter->write(root, &os); // json-->stringstream
    string data = os.str(); // 转为string

    cout << data << endl;

    return data;
}

std::string createStringTimeTable1(string token)
{
    Json::Value root;
    Json::Value timeTable;

    StreamWriterBuilder writerBuilder; // 新式API
    ostringstream os;
    writerBuilder.settings_["emitUTF8"] = true;

    //root.append(userArray);
    root["function"] = "updateTimeTable";
    timeTable["projectName"] = "del";
    timeTable["duration"] = "10";
    timeTable["myDate"] = "2020-1-1";
    root["projects"].append(timeTable);
    timeTable["projectName"] = "io";
    timeTable["duration"] = "10";
    timeTable["myDate"] = "2020-1-1";
    root["projects"].append(timeTable);
    root["token"] = token;

    // 这里使用智能指针
    unique_ptr<StreamWriter> jsonWriter(writerBuilder.newStreamWriter());
    jsonWriter->write(root, &os); // json-->stringstream
    string data = os.str(); // 转为string

    return data;
}

std::string createStringTimeTable2(string token)
{
    Json::Value root;
    Json::Value timeTable;

    StreamWriterBuilder writerBuilder; // 新式API
    ostringstream os;
    writerBuilder.settings_["emitUTF8"] = true;

    //root.append(userArray);
    root["function"] = "updateTimeTable";
    timeTable["projectName"] = "del";
    timeTable["duration"] = "66";
    timeTable["myDate"] = "2020-1-1";
    root["projects"].append(timeTable);
    timeTable["projectName"] = "io";
    timeTable["duration"] = "66";
    timeTable["myDate"] = "2020-1-1";
    root["projects"].append(timeTable);
    root["token"] = token;

    // 这里使用智能指针
    unique_ptr<StreamWriter> jsonWriter(writerBuilder.newStreamWriter());
    jsonWriter->write(root, &os); // json-->stringstream
    string data = os.str(); // 转为string

    return data;
}

std::string createStringTimeTable3(string token)
{
    Json::Value root;
    Json::Value timeTable;

    StreamWriterBuilder writerBuilder; // 新式API
    ostringstream os;
    writerBuilder.settings_["emitUTF8"] = true;

    //root.append(userArray);
    root["function"] = "updateTimeTable";
    timeTable["projectName"] = "del";
    timeTable["duration"] = "3";
    timeTable["myDate"] = "2020-1-1";
    root["projects"].append(timeTable);
    timeTable["projectName"] = "io";
    timeTable["duration"] = "4";
    timeTable["myDate"] = "2020-1-1";
    root["projects"].append(timeTable);
    root["token"] = token;

    // 这里使用智能指针
    unique_ptr<StreamWriter> jsonWriter(writerBuilder.newStreamWriter());
    jsonWriter->write(root, &os); // json-->stringstream
    string data = os.str(); // 转为string

    return data;
}

std::string getStringTimeTable(string token, string startDate, string endDate, string range, string total)
{
    Json::Value root;
    Json::Value timeTable;

    StreamWriterBuilder writerBuilder; // 新式API
    ostringstream os;
    writerBuilder.settings_["emitUTF8"] = true;

    //root.append(userArray);
    root["function"] = "getTimeTable";
    root["token"] = token;
    timeTable["startDate"] = startDate;
    timeTable["endDate"] = endDate;
    timeTable["range"] = range;
    timeTable["total"] = total;
    root["timeTable"].append(timeTable);

    // 这里使用智能指针
    unique_ptr<StreamWriter> jsonWriter(writerBuilder.newStreamWriter());
    jsonWriter->write(root, &os); // json-->stringstream
    string data = os.str(); // 转为string

    return data;
}


int main()
{
    Json::Value userArray;
    Json::Value root;
    Json::Value timeTable;

    StreamWriterBuilder writerBuilder; // 新式API
    ostringstream os;

    timeTable["projectName"] = "del";
    timeTable["duration"] = "2";
    timeTable["time"] = "2020-1-1";
    root["projects"].append(timeTable);
    timeTable["projectName"] = "io";
    timeTable["duration"] = "3";
    timeTable["time"] = "2020-1-1";
    root["projects"].append(timeTable);
    root["userId"] = "1";

    std::string token1;
    std::string token2;
    std::string token3;

    string strValue1 = createString1();
    //string strValue2 = createString2();
    //string strValue3 = createString3();

    //std::cout << strValue3 << std::endl;

    //thread thread2(hanshu, strValue2, ref(token2));
    thread thread1(hanshu, strValue1, ref(token1));
    //thread thread3(hanshu, strValue3, ref(token3));
    thread1.join();
    //thread2.join();
    //thread3.join();

    std::vector<std::string> vtoken1 = split(token1, ",");
    //std::vector<std::string> vtoken2 = split(token2, ",");
    //std::vector<std::string> vtoken3 = split(token3, ",");

    token1 = vtoken1[0];
    //token2 = vtoken2[0];
    //token3 = vtoken3[0];

    //std::cout << token1 << std::endl;

    strValue1 = getStringTimeTable(token1,"2020-1-1","2020-1-2","all","");
    //strValue2 = createStringTimeTable2(token2);
    //strValue3 = createStringTimeTable3(token3);//测试再次提交的更改功能

    //std::cout << strValue3 << std::endl;

    thread thread4(myTime, strValue1);
    //thread thread5(myTime, strValue2);
    //thread thread6(myTime, strValue3);

    thread4.join();
    //thread5.join();
    //thread6.join();

	return 0;
}