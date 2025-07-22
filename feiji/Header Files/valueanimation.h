#ifndef VALUEANIMATION_H
#define VALUEANIMATION_H

struct ValueAnimation {
    long long startValue;
    long long targetValue;
    long long currentValue;
    int timer;
    int duration;
};

#endif // VALUEANIMATION_H