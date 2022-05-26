/**
 * @file packaged_task.cc
 * @author koritafei (koritafei@gmail.com)
 * @brief 模拟gui线程之间传递线程任务
 * @version 0.1
 * @date 2020-06-26
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <algorithm>
#include <condition_variable>
#include <deque>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <thread>
#include <unistd.h>
#include <utility>
#include <vector>

class gui_thread_task {
  public:
    bool gui_shutdown_message_received() {
        sleep(4);

        return false;
    }

    void get_and_process_gui_message() {
        std::cout << "GUI Thread id is " << std::this_thread::get_id()
                  << std::endl;
    }

    void gui_thread() {
        while (!gui_shutdown_message_received()) {
            get_and_process_gui_message();
            std::packaged_task<void()> task;
            {
                std::unique_lock<std::mutex> uk {_mutex};
                _conn.wait(uk, [&] {
                    return !_tasks.empty();
                });
                task = std::move(_tasks.front());
                _tasks.pop_front();
            }
            task();
        }
    }

    template <typename Func> std::future<void> post_task_for_thread(Func f) {
        std::packaged_task<void()>  task {f};
        std::future<void>           res = task.get_future();
        std::lock_guard<std::mutex> lk {_mutex};
        _tasks.push_back(std::move(task));
        _conn.notify_one();
        return res;
    }

  private:
    std::mutex                             _mutex;
    std::deque<std::packaged_task<void()>> _tasks;
    std::condition_variable                _conn;
};

void task_value() {
    std::cout << "Thread id is " << std::this_thread::get_id() << std::endl;
}

int main(int argc, char **argv) {
    std::shared_ptr<gui_thread_task> gui_thread_ptr =
        std::make_shared<gui_thread_task>();
    const int                thread_num = 3;
    std::vector<std::thread> custom_threads;
    for (size_t i = 0; i < thread_num; i++) {
        custom_threads.push_back(
            std::thread {&gui_thread_task::gui_thread, gui_thread_ptr});
        gui_thread_ptr->post_task_for_thread(task_value);
    }

    gui_thread_ptr->gui_shutdown_message_received();

    std::for_each(custom_threads.begin(), custom_threads.end(),
                  std::mem_fn(&std::thread::join));

    return 0;
}
