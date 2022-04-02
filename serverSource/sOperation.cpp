#include"sOperation.h"

std::string serverLogin(Json::Value& value, AdoAccess& dataBase)
{
    std::string username = value["userArray"][0]["username"].asString();
    std::string password = value["userArray"][0]["password"].asString();
    
    std::string result = dataBase.login(username, password);

    return result;
}

std::string encryptMd5(std::string str)
{
    std::string strMd5;
    CryptoPP::Weak1::MD5 hash;
    hash.Update((const unsigned char*)str.c_str(), str.length());
    strMd5.resize(hash.DigestSize());
    hash.Final((unsigned char*)&strMd5[0]);
    return strMd5;
}
