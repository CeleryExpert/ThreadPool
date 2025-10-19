#include<iostream>
#include "threadpool.h"
#include "any.h"

class Mytask : public Task {
public:
	void run() {
		for (int i = 0;i < 100;i++) {
			std::cout << i << '\n';
		}
	}
};



int main() {
	ThreadPool pool;
	pool.start(20);
	std::unique_ptr<int>res;
	//Mytask t;
	//pool.submitTask(std::make_shared<Mytask>(t));
	Any a(std::string("123213"));
	Any b("123");
	std::cout << a.cast_<std::string>() ;
	const char c[4] = b.cast_<const char [4]>();
	while (1);
	return 0;
}