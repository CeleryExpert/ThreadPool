#ifndef SEMAPHORE_H
#define SEMAPHORE_H
#include<mutex>
#include<condition_variable>

class Semaphore {
public:
	Semaphore(int count = 0) : count_(count) {}
	~Semaphore() = default;
	Semaphore(const Semaphore&) = delete;
	/*Semaphore& operator=(const Semaphore&) =delete;*/
	void wait();
	void post();
private:
	std::mutex mutex_;
	std::condition_variable cond_;
	int count_;
};



#endif // !SEMAPHORE_H

