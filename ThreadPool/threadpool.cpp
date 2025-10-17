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
void submitTask(std::shared_ptr<Task> task) {
	 
}

// 启动线程池
void ThreadPool::start(int initThreadSize) {
	initThreadSize_ = initThreadSize;

	// 创建线程对象 
	for (int i = 0; i < initThreadSize_; ++i) {
		threads_.push_back(new Thread(std::bind(&ThreadPool::threadFunc,this)));
	}

	// 启动所有线程
	for (int i = 0;i < initThreadSize_; ++i) {
		// 启动线程执行线程函数 
		threads_[i]->start();
	}
}

void ThreadPool::threadFunc() {
	std::cout << "func running..." << std::endl;
	std::cout << std::this_thread::get_id() << std::endl;
	std::cout << "func end..." << std::endl;
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



