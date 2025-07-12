/*
    Written by: Oktay and Okke(Me)
*/

#pragma once
#include "Core/Logger.h"

template <class T>
class ArrayList
{
public:
    ArrayList();


    ArrayList(const ArrayList& other) {
        count = other.count;
        capacity = other.capacity;
        if (capacity > 0) {
            buffer = new T[capacity];
            for (uint i = 0; i < count; ++i) 
                buffer[i] = other.buffer[i];
        }
    }

    ArrayList& operator=(ArrayList&& other) noexcept {
        if (this == &other)
            return *this;

        delete[] buffer;

        buffer = other.buffer;
        count = other.count;
        capacity = other.capacity;

        other.buffer = nullptr;
        other.count = 0;
        other.capacity = 0;

        return *this;
    }

    ArrayList& operator=(const ArrayList& other) {
        if (this == &other)
            return *this;

        T* newBuffer = nullptr;
        if (other.capacity > 0) {
            newBuffer = new T[other.capacity];
            for (uint i = 0; i < other.count; ++i) {
                newBuffer[i] = other.buffer[i];
            }
        }

        delete[] buffer;

        buffer = newBuffer;
        count = other.count;
        capacity = other.capacity;

        return *this;
    }
 

    ~ArrayList()
    {
        delete[] buffer;
        capacity = 0;
        count = 0;
    }



    T& at(uint index) const;

    void append(T item);

    T remove_at(uint index);

    T remove(const T& item)
    {
        int i = find(item);
        if (i!=-1)
            return remove_at(i);
        return nullptr;
    }

    int find(const T& item) const
    {
        for (uint i = 0; i < count; i++)
        {
            if (buffer[i] == item)
            {
                return i;
            }
        }
        return -1;

    }

    void clear() { count = 0; }

    uint size() const { return count; }
    uint get_capacity() const { return capacity; }

    T* data() const { return buffer; }

    void reserve(uint new_cap);

    template<typename Comparator>
    void quickSort(Comparator comp);
private:
    template <typename Comparator>
    int partition(ArrayList& vec, int low, int high, Comparator comp);

    template<typename Comparator>
    void quickSort(ArrayList& vec, int low, int high, Comparator comp);

    
    uint count{ 0 };
    uint capacity{ 0 };

    T* buffer{ nullptr };
};

template<class T>
inline ArrayList<T>::ArrayList() : buffer(new T[capacity]) {}




template<class T>
inline T& ArrayList<T>::at(uint index) const
{
    if (index >= count)
    {
        SKEL_CRITICAL("Tried accessing outside List");

        assert(0);
    }
    return buffer[index];
}

template<class T>
inline void ArrayList<T>::append(T item)
{
    if (count >= capacity) reserve(capacity == 0 ? 1 : capacity * 2);

    buffer[count++] = item;
}

template<class T>
inline T ArrayList<T>::remove_at(uint index)
{
    if (index >= count)
    {
        SKEL_CRITICAL("Tried accessing outside ArrayList");
        assert(0);
    }

    T temp = buffer[index];
    count--;
    for (uint i = index; i < count; i++) buffer[i] = buffer[i + 1];

    return temp;
}


template<class T>
inline void ArrayList<T>::reserve(uint new_cap)
{
    if (new_cap <= capacity) return;

    T* newData = new T[new_cap];

    for (uint i = 0; i < count; i++)
    {
        newData[i] = buffer[i];
    }



    delete[] buffer;
    buffer = newData;
    capacity = new_cap;
}

template<class T>
template<typename Comparator>
inline int ArrayList<T>::partition(ArrayList& vec, int low, int high, Comparator comp)
{
    // Got this from https://www.geeksforgeeks.org/cpp-program-for-quicksort/
    // but changed it to handle array classes isntead of vectors

    // Selecting last element as the pivot
    T pivot = vec.at(high);

    // Index of elemment just before the last element
    // It is used for swapping
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {

        // If current element is smaller than or
        // equal to pivot
        if (comp(vec.at(j), pivot)) {
            i++;
            
            T old = vec.at(i);
            vec.at(i) = vec.at(j);
            vec.at(j) = old;
        }
    }

    // Put pivot to its position
    T old = vec.at(i + 1);
    vec.at(i+1) = vec.at(high);
    vec.at(high) = old;

    // Return the point of partition
    return (i + 1);
}

template<class T>
template<typename Comparator>
inline void ArrayList<T>::quickSort(ArrayList& vec, int low, int high, Comparator comp)
{
    // Got this from https://www.geeksforgeeks.org/cpp-program-for-quicksort/
    // but changed it to handle array classes isntead of vectors
    // 
    // Base case: This part will be executed till the starting
    // index low is lesser than the ending index high
    if (low < high) {

        // pi is Partitioning Index, arr[p] is now at
        // right place
        int pi = partition(vec, low, high, comp);

        // Separately sort elements before and after the
        // Partition Index pi
        quickSort(vec, low, pi - 1, comp);
        quickSort(vec, pi + 1, high, comp);
    }
}

template<class T>
template<typename Comparator>
inline void ArrayList<T>::quickSort(Comparator comp)
{
    if (count > 1)
    {
        quickSort(*this, 0, count - 1, comp);
    }
}
