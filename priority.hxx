enum class ThreadPriority {
    High = 1, // ESP
    Medium = 2,
    Low = 3
};

thread_local std::stack<ThreadPriority> priority_stack;

void PushPriority(ThreadPriority priority) {
    priority_stack.push(priority);
}

void PopPriority() {
    if (!priority_stack.empty()) {
        priority_stack.pop();
    }
}

ThreadPriority GetCurrentPriority() {
    return priority_stack.empty() ? ThreadPriority::High : priority_stack.top();
}

struct ScopedPriority {
    ScopedPriority(ThreadPriority priority) { PushPriority(priority); }
    ~ScopedPriority() { PopPriority(); }
};

struct ThrottleState {
    std::mutex mtx;
    std::condition_variable cv;
    int budget_remaining = 1000; // adjustable
} throttle_state;
