#include "token.h"

std::string generateToken(std::string user)
{
	auto token = jwt::create()
		.set_issuer(user)
		.set_issued_at(std::chrono::system_clock::now())
		.set_expires_at(std::chrono::system_clock::now() + std::chrono::seconds{ 5 })
		.sign(jwt::algorithm::hs256{ "secret" });

	return token;
}

bool verifyToken(std::string token, std::string user)
{
	auto decoded = jwt::decode(token);
	//Get all payload claims
	//for (auto& e1 : decoded.get_payload_claims())
	//	std::cout << e1.first << " = " << e1.second.to_json() << std::endl;
	////Get all header claims
	//for (auto& e2 : decoded.get_header_claims())
	//	std::cout << e2.first << " = " << e2.second.to_json() << std::endl;
	//Create a verifier using the default clock then return verifier instance
	auto verifier = jwt::verify().allow_algorithm(jwt::algorithm::hs256{ "secret" }).with_issuer(user);
	try
	{
		verifier.verify(decoded);
	}
	catch (std::system_error e)
	{
		std::cout << e.what() << std::endl;
		return false;
	}
	return true;
}