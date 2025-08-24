#pragma once

#include <utility>
#include <mutex>
#include <condition_variable>
#include <deque>

/**
 * @brief A thread-safe queue for concurrent producer/consumer scenarios.
 * 
 * @tparam T The type of item stored in the queue.
 */
template <typename T>
class ConcurrentQueue {
public:
    /**
     * @brief Pushes an item into the queue.
     * @param item The item to push (rvalue reference).
     */
    void push(T&& item) {
        {
            std::lock_guard<std::mutex> lk(m_);
            q_.push_back(std::move(item));
        }
        cv_.notify_one();
    }

    /**
     * @brief Pops an item from the queue. Blocks if the queue is empty.
     * @param out Reference to store the popped item.
     * @return False if the queue is closed and empty, true otherwise.
     */
    bool pop(T& out) {
        std::unique_lock<std::mutex> lk(m_);
        cv_.wait(lk, [&] { return closed_ || !q_.empty(); });
        if (q_.empty()) return false; // closed_ must be true here
        out = std::move(q_.front());
        q_.pop_front();
        return true;
    }

    /**
     * @brief Closes the queue. Wakes up all waiting threads.
     */
    void close() {
        {
            std::lock_guard<std::mutex> lk(m_);
            closed_ = true;
        }
        cv_.notify_all();
    }

    int size() {
        return q_.size();
    }

private:
    std::deque<T> q_; ///< Underlying queue storage
    std::mutex m_; ///< Mutex for thread safety
    std::condition_variable cv_; ///< Condition variable for blocking pop
    bool closed_ = false; ///< Indicates if the queue is closed
};
