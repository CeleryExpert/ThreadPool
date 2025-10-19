#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <vector>
#include <queue>
#include <memory>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <functional>
#include "any.h"
#include "semaphore.h"

 
enum class PoolMode{
	MODE_FIXED,     // 固定线程数
	MODE_CACHED     // 动态线程数
};

class Task;

class Result {
public:
	Result(std::shared_ptr<Task>, bool);	
	~Result() = default;
	Any get();
	Result(Result&& s) = default;
	Result& operator = (Result&&) = default;
	void setVal(Any);
public:
	Semaphore sem_;
	bool isValid_;
	Any any_;
	std::shared_ptr<Task> task_;
};


class Task {
public:
	virtual Any run() = 0;
	void execute();
	Task();
	void setResult(Result* res);
private:
	Result* result_;
};

// 线程类型
class Thread {
public:
	// 启动线程
	using ThreadFunc = std::function<void()>;
	void start();
	Thread(ThreadFunc func);
	~Thread();
private:
	ThreadFunc func_;
};

class ThreadPool {
public:
	ThreadPool(); // 线程池构造
	~ThreadPool(); // 线程池析构2
	void setMode(PoolMode mode); // 设置线程池模式
	void setTaskQueMaxSize(int size); // 设置任务队列最大容量
	void setInitThreadSize(int size); // 设置初始线程数
	Result submitTask(std::shared_ptr<Task> task); // 提交任务
	void start(int initThreadSize = 4); // 启动线程池 
	void stop(); // 停止线程池
	 
	ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;   
private:
	// 定义线程处理函数
	void threadFunc();
private:
	std::vector<std::unique_ptr<Thread>> threads_; // 线程列表
	size_t initThreadSize_; // 初始线程数
	std::queue<std::shared_ptr<Task>> taskQue_; // 任务队列 
	std::atomic_int taskSize_; // 任务数量
	int taskQueMaxSize_; // 任务队列最大容量;
	std::mutex taskQueMutex_; // 任务队列互斥锁
	std::condition_variable taskQueNotEmpty_; // 任务队列不为空条件变量
	std::condition_variable taskQueNotFull_; // 任务队列不为满条件变量
	PoolMode poolMode_;
};

#endif // !THREADPOOL_H

