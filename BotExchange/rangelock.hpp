#pragma once

#include <vector>
#include <shared_mutex>
#include <atomic>
#include <condition_variable>

class RangeLock {
public:
	void LockRange(double start, double end);
	void UnlockRange(double start, double end);
private:
	using IntervalEndpoint = std::pair<double, std::shared_mutex>;

	std::vector<IntervalEndpoint> m_intervals;
	std::shared_mutex m_allIntervals;
	std::atomic_int m_consumers;
	std::mutex m_clearMutex;
};