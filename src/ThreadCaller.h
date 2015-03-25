#pragma once

template <typename WORKER_PTR>
class ThreadCaller{
public:
	ThreadCaller(const WORKER_PTR& worker):m_worker(worker){}
	~ThreadCaller(){}

	void operator()() {
		m_worker->execute();
	}

private:
	WORKER_PTR m_worker;
};