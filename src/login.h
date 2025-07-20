#pragma once
#include <string>
class Login {
	public:
		Login(unsigned int id, std::string user, std::string password);
	private:
		std::string
			user
			, password;
		unsigned int id;
};

