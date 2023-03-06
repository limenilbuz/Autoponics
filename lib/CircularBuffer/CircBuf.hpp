#pragma once

#define MAX_CAPACITY 10
/**
 * @brief Container class for unique data structure in metric.hpp
 */
template <typename T>
class CircBuf
{

private:
    /**
     * @var Static array of length 10
     */
    T buffer[MAX_CAPACITY];

    /**
     * @var Allows us to keep track of where in the "circle" we are, i.e at the oldest entry
     */
    int counter = 0;

    /**
     * @var Current size.
     */
    int sz = 0;

public:
    CircBuf() : buffer{T{0}}, counter{0}, sz{0} {}
    /**
     * @brief Returns current size, capped at 10.
     */
    int size()
    {
        return sz;
    }

    /**
     * @brief Adds a data point to the buffer, deleting the entry that was there, which was the oldest entry
     */
    void push_front(T data)
    {
        buffer[counter] = data;
        int temp = (++counter) % 10;
        counter = temp;
        if (sz < 10)
        {
            sz++;
        }
    }

    /**
     * @brief Returns sum of all data entries so far.
     */
    T accumulate()
    {
        T sum = T{0};
        for (int i = 0; i < sz; i++)
        {
            sum += buffer[i];
        }
        return sum;
    }

    /**
     * @brief Returns most recent entry.
     * Returns -1 if there is no current data.
     */
    T front()
    {
        if (sz == 0)
        {
            return T{-1};
        }
        if (counter == 0)
        {
            return buffer[MAX_CAPACITY - 1];
        }
        return buffer[(counter - 1)];
    }
};
