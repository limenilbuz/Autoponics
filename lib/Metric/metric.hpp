#pragma once
#include "CircBuf.hpp"
#include <numeric>

#define MAX_HISTORY_SIZE 10

/**
 * @brief Metric template class to track data over time periods.
 */
template <typename T>
class Metric
{
private:
    /**
     * @var Represents the history of the data as a double ended queue.
     * TODO: CHANGE DEQUE TO CIRCULAR BUFFER!!!!
     */
    CircBuf<T> history;

public:
    /**
     * @brief Adds a data point to the history, shifting over all data if the size of history exceeds the max size.
     */
    void addToHistory(T data)
    {
        history.push_front(data);
    }

    /**
     * @brief Returns the average of all the data points.
     */
    double average()
    {
        T total = history.accumulate();
        return total / MAX_HISTORY_SIZE;
    }

    /**
     * @brief Returns the most recent data point.
     */
    T mostRecent()
    {
        return history.front();
    }
};
