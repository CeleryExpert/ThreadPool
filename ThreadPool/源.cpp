#include<iostream>
#include "threadpool.h"

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
	Mytask t;
	pool.submitTask(std::make_shared<Mytask>(t));
	while (1);
	return 0;
}