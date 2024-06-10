#pragma once

class IDsp
{
public:
    virtual double process(double sample, int channel) = 0;
};