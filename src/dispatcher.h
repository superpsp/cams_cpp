#include <string>

class Dispatcher;

class DispatcherDestructor {
public:
	~DispatcherDestructor();
	void initialize(Dispatcher* p);
private:
	Dispatcher* dispatcherInstance;
};

class Dispatcher {
public:
	static Dispatcher& getInstance();
	void setIpFile(std::string ipFile);
protected:
	Dispatcher& operator = (Dispatcher&);

	Dispatcher() {}
	Dispatcher(const Dispatcher&);
	~Dispatcher(){}
	friend class DispatcherDestructor;
private:
	std::string ipFile;
};