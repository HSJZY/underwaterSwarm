#include "utils.h"

template<typename T>
utils<T>::utils()
{
}

template<typename T>
T utils<T>::max(T val1, T val2)
{
    if(val1>val2){
        return val1;
    }
    else{
        return val2;
    }
}
template<typename T>
T utils<T>::min(T val1, T val2)
{
    if(val1>val2){
        return val2;
    }
    else
        return val1;
}
