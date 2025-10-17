#include<iostream>
#include "threadpool.h"
int main() {
	ThreadPool pool;
	pool.start(20);
	std::this_thread::sleep_for(std::chrono::seconds(20));
	return 0;
}