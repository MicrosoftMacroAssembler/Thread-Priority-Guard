# Thread-Priority-Guard
Thread Priority based memory access controller that throttles kernel requests. <br />
It uses priority stacks and budget-based rate limiting to control request frequency. <br />
High-priority threads bypass throttling while medium/low priority threads wait for available budget. <br />

# What Is Memory Throttling?
Memory throttling controls request frequency:
- Priority - High, Medium, Low thread classes
- Budget - Refillable token bucket for rate limiting
- Stacks - Per-thread priority management
- Scoped - RAII-based priority scope guards

# Throttling Usage
The tool provides automatic throttling integration: <br />
```cpp
// Start budget refiller thread
std::thread refiller(BudgetRefillerThread);
refiller.detach();

// High priority (no throttle)
{
    ScopedPriority prio(ThreadPriority::High);
    auto esp = ReadMemory<uint64_t>(player_base);
}

// Low priority (throttled)
{
    ScopedPriority prio(ThreadPriority::Low);
    auto health = ReadMemory<float>(health_addr); // Waits if budget empty
}
```

Incorporate this code into your memory requests: <br />
```cpp
static constexpr ThreadPriority DEFAULT_PRIORITY = ThreadPriority::High;

// Throttling
ThreadPriority current_priority = GetCurrentPriority();
if (current_priority != ThreadPriority::High) {
    std::unique_lock<std::mutex> lock(throttle_state.mtx);
    throttle_state.cv.wait(lock, [&] {
      return throttle_state.budget_remaining > 0;
    });
    --throttle_state.budget_remaining;
    lock.unlock();
}
```
