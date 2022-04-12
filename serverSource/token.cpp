#include "token.h"

std::string generateToken(std::string& username)
{
	auto token = jwt::create()
		.set_issuer(username)
		.set_issued_at(std::chrono::system_clock::now())
		.set_expires_at(std::chrono::system_clock::now() + std::chrono::seconds{ 3600 })
		.sign(jwt::algorithm::hs256{ "secret" });

	return token;
}

std::string verifyToken(std::string& token)
{
	std::string username;
	try
	{
		auto decoded = jwt::decode(token);
		//Get all payload claims
		for (auto& e : decoded.get_payload_claims())
		{
			if (e.first == "iss")
			{
				username = e.second.as_string();//取出username
			}
		}
		auto verifier = jwt::verify().allow_algorithm(jwt::algorithm::hs256{ "secret" }).with_issuer(username);
		verifier.verify(decoded);
	}
	catch (std::exception e)
	{
		return e.what();
	}
	return "verify success";
}

std::string getUsername(std::string& token)
{
	std::string username;
	auto decoded = jwt::decode(token);
	//Get all payload claims
	for (auto& e : decoded.get_payload_claims())
	{
		if (e.first == "iss")
		{
			username = e.second.as_string();//取出username
		}
	}
	return username;
}

std::string encryptMd5(std::string str)
{
	std::string digest;
	CryptoPP::Weak1::MD5 md5;
	CryptoPP::HashFilter hashfilter(md5);
	hashfilter.Attach(new CryptoPP::HexEncoder(new CryptoPP::StringSink(digest), false));
	hashfilter.Put(reinterpret_cast<const unsigned char*>(str.c_str()), str.length());
	hashfilter.MessageEnd();
	return digest;
}

std::vector<std::string> split(const std::string str, const std::string part)
{
	std::vector<std::string> resVec;

	if ("" == str)//若为空，则返回原字符串
	{
		return resVec;
	}
	//方便截取最后一段数据
	std::string strs = str + part;

	size_t pos = strs.find(part);//find函数的返回值，若找到分隔符返回分隔符第一次出现的位置，
								 //否则返回npos
								 //此处用size_t类型是为了返回位置
	size_t size = strs.size();

	while (pos != std::string::npos)
	{
		std::string x = strs.substr(0, pos);//substr函数，获得子字符串
		resVec.push_back(x);
		strs = strs.substr(pos + 1, size);
		pos = strs.find(part);
	}
	return resVec;
}


