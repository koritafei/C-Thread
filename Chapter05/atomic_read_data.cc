/**
 * @file atomic_read_data.cc
 * @author koritafei (koritafei@gmail.com)
 * @brief 使用原子操作从队列中读取数据
 * @version 0.1
 * @date 2020-07-27
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <atomic>
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

class atomic_queue {
public:
    void populte_queue() {
        unsigned const number_of_items = 20;
        _queue.clear();
        for (unsigned i = 0; i < number_of_items; i++) {
            _queue.push_back(i);
        }
        _count.store(number_of_items, std::memory_order_release);
    }

    void consume_queue_items() {
        while (true) {
            int item_index;
            if (0 >=
                (item_index = _count.fetch_sub(1, std::memory_order_acquire))) {
                wait_for_more_item();
                continue;
            }
            process(_queue[item_index - 1]);
        }
    }

private:
    void wait_for_more_item() {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "Wait For More Data" << std::endl;
    }

    void process(int item) {
        std::cout << item << std::endl;
    }

    std::vector<int> _queue;
    std::atomic<int> _count;
};

int main(int argc, char **argv) {
    std::shared_ptr<atomic_queue> ptr = std::make_shared<atomic_queue>();
    std::thread                   t1{&atomic_queue::populte_queue, ptr};
    std::thread                   t2{&atomic_queue::consume_queue_items, ptr};
    std::thread                   t3{&atomic_queue::consume_queue_items, ptr};

    t3.join();
    t2.join();
    t1.join();
}
