#ifndef METRIC_HPP
#define METRIC_HPP
#include <deque>
#include <numeric>

#define MAX_HISTORY_SIZE 10

/**
 * @brief Metric template class to track data over time periods.
*/
template <typename T> class Metric {
    private:
    /**
     * @var Represents the history of the data as a double ended queue.
     * TODO: CHANGE DEQUE TO CIRCULAR BUFFER!!!!
    */
    std::deque<T> history;

    public:
    /**
     * @brief Adds a data point to the history, shifting over all data if the size of history exceeds the max size.
    */
    void addToHistory(T data) {
        if (history.size() == MAX_HISTORY_SIZE) {
            history.pop_back();
        }
    
        history.push_front(data);
    }
    
    /**
     * @brief Returns the average of all the data points.
    */
    double average() {
        T total = std::accumulate(history.begin(), history.end(), T{0});
        return total / MAX_HISTORY_SIZE;
    }

    /** 
     * @brief Returns the most recent data point.
    */
    T mostRecent() {
        return history.front();
    }


};


#endif