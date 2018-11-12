#pragma once

template <typename T>
class span {
public:
    span() : data(nullptr), size(0) {};
    template <size_t num>
    span(const T (&data)[num]) : data((T*) data), size(num) {}
    span(T *data, size_t size) : data(data), size(size) {};

    using element_type = T;
    using iterator = T*;
    
    iterator begin() { return data; }
    iterator end() { return data + size; }

    T operator[](size_t i) { return data[i]; }
private:
    T *data;
    const size_t size;
};
