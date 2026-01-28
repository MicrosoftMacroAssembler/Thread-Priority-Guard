void BudgetRefillerThread() {
    while (true) {
        {
            std::lock_guard<std::mutex> lock(throttle_state.mtx);
            throttle_state.budget_remaining = 200; // or whatever your cap is
        }
        throttle_state.cv.notify_all(); // resume any waiting threads
        Sleep(10);
    }
}
