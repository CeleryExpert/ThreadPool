#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <vector>
#include <queue>
#include <memory>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <functional>

 
enum class PoolMode{
	MODE_FIXED,     // �̶��߳���
	MODE_CACHED     // ��̬�߳���
};


class Task {
public:
	virtual void run() = 0;
};

// �߳�����
class Thread {
public:
	// �����߳�
	using ThreadFunc = std::function<void()>;
	void start();
	Thread(ThreadFunc func);
	~Thread();
private:
	ThreadFunc func_;
};

class ThreadPool {
public:
	ThreadPool(); // �̳߳ع���
	~ThreadPool(); // �̳߳�����2
	void setMode(PoolMode mode); // �����̳߳�ģʽ
	void setTaskQueMaxSize(int size); // ������������������
	void setInitThreadSize(int size); // ���ó�ʼ�߳���
	void submitTask(std::shared_ptr<Task> task); // �ύ����
	void start(int initThreadSize = 4); // �����̳߳� 
	void stop(); // ֹͣ�̳߳�
	 
	ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;   
private:
	// �����̴߳�����
	void threadFunc();
private:
	std::vector<Thread*> threads_; // �߳��б�
	size_t initThreadSize_; // ��ʼ�߳���
	std::queue<std::shared_ptr<Task>> taskQue_; // ������� 
	std::atomic_int taskSize_; // ��������
	int taskQueMaxSize_; // ��������������;
	std::mutex taskQueMutex; // ������л�����
	std::condition_variable taskQueNotEmpty_; // ������в�Ϊ����������
	std::condition_variable taskQueNotFull_; // ������в�Ϊ����������
	PoolMode poolMode_;
};

#endif // !THREADPOOL_H

