#include "semaphore.h"

void Semaphore::wait() {
	std::unique_lock<std::mutex> lock(mutex_);
	cond_.wait(lock, [&]() ->bool{
		return count_ > 0; 
		}
	);
	--count_;
}

void Semaphore::post() {
	std::unique_lock<std::mutex> lock(mutex_);
	++count_;
	cond_.notify_all();
}


