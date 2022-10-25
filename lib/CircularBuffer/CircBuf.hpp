#ifndef CircBuf_HPP
#define CircBuf_HPP

/**
 * @brief Container class for unique data structure in metric.hpp
*/
template <typename T> class CircBuf {

    private:
        /**
        * @brief Define static array
        */
        T history[10];

        /**
        * @brief Allows us to keep track of where in the "circle" we are, i.e at the oldest entry
        */
        int counter = 0;

    public:
        /**
        * @brief returns size, always at 10
        */
        int size (){
            return 10;
        }

        /**
        * @brief Adds a data point to the history, deleting the entry that was there, which was the oldest entry
        */
        void push_front(T data){
            history[counter%10] = data;
            counter++;
        }

        /**
        * @brief returns sum of all data entries so far
        */
        T accumulate(){
            T sum = T{0};
            for(int i = 0; i < 10; i++){
                sum += history[i];
            }
            return sum;
        }

        /**
        * @brief returns most recent entry
        */
        T front(){
            return history[(counter - 1) % 10];
        }

};

#endif