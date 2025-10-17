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
void submitTask(std::shared_ptr<Task> task) {
	 
}

// �����̳߳�
void ThreadPool::start(int initThreadSize) {
	initThreadSize_ = initThreadSize;

	// �����̶߳��� 
	for (int i = 0; i < initThreadSize_; ++i) {
		threads_.push_back(new Thread(std::bind(&ThreadPool::threadFunc,this)));
	}

	// ���������߳�
	for (int i = 0;i < initThreadSize_; ++i) {
		// �����߳�ִ���̺߳��� 
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

// �����߳� 
void Thread::start() {
	// ����һ���߳�;
	std::thread t(func_);
	t.detach(); // �����߳� pthread_detach
}



