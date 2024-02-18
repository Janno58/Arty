#pragma once

////////////////////////////////////////////////////////////////////////////////
template<typename T>
struct WrappingInt{

    ////////////////////////////////////////////////////////////////////////////
    WrappingInt(T max) : maxValue(max) {};

    ////////////////////////////////////////////////////////////////////////////
    operator T() {
        return value;
    }

    ////////////////////////////////////////////////////////////////////////////
    T operator++(int) {
        if(value + 1 >= maxValue) {
            return value = 0;
        }

        return ++value;
    }

private:

    ////////////////////////////////////////////////////////////////////////////
    T value{0};

    ////////////////////////////////////////////////////////////////////////////
    const T maxValue;

};