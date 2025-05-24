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
	bool initialize(std::string ipFile, unsigned int numberOfDevices, unsigned int deviceLifetimeSeconds, unsigned int deviceLifeTimeCheckSeconds);
protected:
	Dispatcher& operator = (Dispatcher&);

	Dispatcher() {}
	Dispatcher(const Dispatcher&);
	~Dispatcher(){}
	friend class DispatcherDestructor;
private:
	std::string ipFile;
	unsigned int numberOfDevices;
	unsigned int deviceLifetimeSeconds;
	unsigned int deviceLifeTimeCheckSeconds;
	void registerDevice();
};