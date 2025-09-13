#include "login.h"
#include "logger.h"

#define LOGGER Logger::getInstance()

Login::Login(unsigned int id, std::string user, std::string password) {
	LOGGER->logDebug("Login::Login: id = " + std::to_string(id) + ", user = " + user + ", password = " + password);
	this->id = id;
	this->user = user;
	this->password = password;
}

unsigned int Login::getId() {
	return id;
}

std::string Login::getPassword() {
	return password;
}

std::string Login::getUser() {
	return user;
}

Login::~Login() {
	LOGGER->logDebug("Login::Login: id = " + std::to_string(id) + ", user = " + user + ", password = " + password + " was deleted");
}
