#include"threadpool.h"
#include<iostream>
#include<thread>

const int TASK_MAX = 1024;

// 线程池构造函数
ThreadPool::ThreadPool()
	:initThreadSize_(4)
	,taskSize_(0)
	,taskQueMaxSize_(TASK_MAX)
	, poolMode_(PoolMode::MODE_FIXED)
{ }


// 线程池析构函数
ThreadPool::~ThreadPool()
{
}

// 设置线程池模式
void ThreadPool::setMode(PoolMode mode) {
	poolMode_ = mode;
}

// 设置任务队列最大容量
void ThreadPool::setTaskQueMaxSize(int size) {
	taskQueMaxSize_ = size;
}

// 设置初始线程数
void ThreadPool::setInitThreadSize(int size) {
	initThreadSize_ = size;
}

// 给线程池提交任务
void ThreadPool::submitTask(std::shared_ptr<Task> task) {
	// 获取锁
	std::unique_lock<std::mutex> lock(taskQueMutex_);

	// 线程通信
	//while (taskQue_.size() == taskQueMaxSize_) {
	//	taskQueNotFull_.wait(lock); 
	//}

	// 如果任务队列满，等待
	// 最长等待时间为1秒
	if (!taskQueNotFull_.wait_for(lock, std::chrono::seconds(1), [&]() {
		return taskQue_.size() < taskQueMaxSize_;
		})) {
		std::cerr << "submit task timeout!" << std::endl;
		return;
	}
	taskQue_.push(task);
	taskSize_++;
	taskQueNotEmpty_.notify_all(); // 通知消费者线程
}

// 启动线程池
void ThreadPool::start(int initThreadSize) {
	initThreadSize_ = initThreadSize;

	// 创建线程对象 
	for (int i = 0; i < initThreadSize_; ++i) {
		auto ptr = std::make_unique<Thread>(std::bind(&ThreadPool::threadFunc, this));
		threads_.push_back(std::move(ptr));
	}

	// 启动所有线程
	for (int i = 0;i < initThreadSize_; ++i) {
		// 启动线程执行线程函数 
		threads_[i]->start();
	}
}

void ThreadPool::threadFunc() {
	while (true) {
		// 避免执行完任务后才交出锁
		std::shared_ptr<Task> item; 
		{
			std::unique_lock<std::mutex> lock(taskQueMutex_);
			taskQueNotEmpty_.wait(lock, [&]() {
				return !taskQue_.empty();
				});
			item = taskQue_.front();
			taskSize_--;
			taskQue_.pop();
			taskQueNotFull_.notify_all();
		}
		if (item != nullptr) {
			item->run();
		}
	}
}



Thread::Thread(ThreadFunc func) {
	func_ = func;
}

Thread::~Thread() {

}

// 启动线程 
void Thread::start() {
	// 创建一个线程;
	std::thread t(func_);
	t.detach(); // 分离线程 pthread_detach
}



