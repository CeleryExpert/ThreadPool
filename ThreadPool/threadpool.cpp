#include"threadpool.h"
#include<iostream>
#include<thread>

const int TASK_MAX = 1024;

// �̳߳ع��캯��
ThreadPool::ThreadPool()
	:initThreadSize_(4)
	,taskSize_(0)
	,taskQueMaxSize_(TASK_MAX)
	, poolMode_(PoolMode::MODE_FIXED)
{ }


// �̳߳���������
ThreadPool::~ThreadPool()
{
}

// �����̳߳�ģʽ
void ThreadPool::setMode(PoolMode mode) {
	poolMode_ = mode;
}

// ������������������
void ThreadPool::setTaskQueMaxSize(int size) {
	taskQueMaxSize_ = size;
}

// ���ó�ʼ�߳���
void ThreadPool::setInitThreadSize(int size) {
	initThreadSize_ = size;
}

// ���̳߳��ύ����
Result ThreadPool::submitTask(std::shared_ptr<Task> task) {
	// ��ȡ��
	std::unique_lock<std::mutex> lock(taskQueMutex_);

	// �߳�ͨ��
	//while (taskQue_.size() == taskQueMaxSize_) {
	//	taskQueNotFull_.wait(lock); 
	//}

	// ���������������ȴ�
	// ��ȴ�ʱ��Ϊ1��
	if (!taskQueNotFull_.wait_for(lock, std::chrono::seconds(1), [&]() {
		return taskQue_.size() < taskQueMaxSize_;
		})) {
		std::cerr << "submit task timeout!" << std::endl;
		return Result(std::move(task), false);
	}
	taskQue_.push(task);
	taskSize_++;
	taskQueNotEmpty_.notify_all(); // ֪ͨ�������߳�
	return Result(std::move(task), true);
}

// �����̳߳�
void ThreadPool::start(int initThreadSize) {
	initThreadSize_ = initThreadSize;

	// �����̶߳��� 
	for (int i = 0; i < initThreadSize_; ++i) {
		auto ptr = std::make_unique<Thread>(std::bind(&ThreadPool::threadFunc, this));
		threads_.push_back(std::move(ptr));
	}

	// ���������߳�
	for (int i = 0;i < initThreadSize_; ++i) {
		// �����߳�ִ���̺߳��� 
		threads_[i]->start();
	}
}

void ThreadPool::threadFunc() {
	while (true) {
		// ����ִ���������Ž�����
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
			item->execute();
		}
	}
}


Thread::Thread(ThreadFunc func) {
	func_ = func;
}

Thread::~Thread() {

}

// �����߳� 
void Thread::start() {
	// ����һ���߳�;
	std::thread t(func_);
	t.detach(); // �����߳� pthread_detach
}


// Result���ʵ��
Result::Result(std::shared_ptr<Task> task, bool isValid)
	:task_(task), isValid_(isValid) {
	task->setResult(this);
}


Any Result::get() {
	if (this->isValid_ == false) {
		throw "result is invalid";
	}
	sem_.wait();
	return std::move(any_);
}

void Result::setVal(Any any) {
	this->any_ = std::move(any);
	sem_.post();
}

//Task���ʵ��

Task::Task()
	:result_(nullptr)
{ }

void Task::setResult(Result* res) {
	result_ = res;
}

void Task::execute() {
	this->result_->setVal(run());
}






