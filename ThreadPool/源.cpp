#include<iostream>
#include "threadpool.h"
#include "any.h"

class Mytask : public Task {
public:
	Any run() {
		int sum = 0;
		for (int i = 0;i < 100;i++) {
			sum += i;
		}
		return sum;
	}
};



int main() {
	ThreadPool pool;
	pool.start(1);
	std::shared_ptr<Task> task1 = std::make_shared<Mytask>();
	Result res = pool.submitTask(task1);
	std::cout << res.get().cast_<int>() << std::endl;
	return 0;
}