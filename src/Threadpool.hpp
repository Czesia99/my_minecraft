#pragma once
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

#include <exception>

using namespace std;

class TaskQueue {
    public:
        void enqueue(function<void()> task) {
            const lock_guard<std::mutex> lock(queue_mutex_);
            tasks_.emplace_back(move(task));
        }

        void execute()
        {
            const lock_guard<std::mutex> lock(queue_mutex_);
            for (auto &t : tasks_) {
                t();
            }
            tasks_.clear();
        }

    private:
        deque<function<void()>> tasks_;
        mutex queue_mutex_;
};

class ThreadPool {
    public:
        ThreadPool(size_t num_threads = thread::hardware_concurrency())
        {
            std::cout << "NUM THREADS == " << num_threads << std::endl;
            for (size_t i = 0; i < num_threads; ++i) {
                threads_.emplace_back([this] {
                    while (true) {
                        function<void()> task;
                        {
                            unique_lock<mutex> lock(queue_mutex_);

                            cv_.wait(lock, [this] {
                                return !tasks_.empty() || stop_;
                            });
                            if (stop_ && tasks_.empty()) {
                                return;
                            }

                            task = move(tasks_.front());
                            tasks_.pop();
                        }
                        task();
                    }
                });
            }
        }

        void stop() {
            {
                unique_lock<mutex> lock(queue_mutex_);
                stop_ = true;
            }

            cv_.notify_all();

            for (auto& thread : threads_) {
                if (thread.joinable()) {
                    thread.join();
                }
            }
        }

        void enqueue(function<void()> task)
        {
            {
                unique_lock<std::mutex> lock(queue_mutex_);
                tasks_.emplace(move(task));
            }
            cv_.notify_one();
        }

    private:
        vector<thread> threads_;

        queue<function<void()>> tasks_;
        mutex queue_mutex_;
        condition_variable cv_;
        bool stop_ = false;
};