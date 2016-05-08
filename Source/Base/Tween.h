//
//  Tween
//  OctoBoom
//
//  Created by teivaz on 2/14/16.
//  Copyright © 2016 teivaz. All rights reserved.
//

#pragma once
#include "Common.h"
#include <functional>
#include <math.h>

struct LinearInterpolation{
    float Interpolate(float a) {return a;}
};
struct EaseSinInOut{
    float Interpolate(float a) {return 0.5 + 0.5*sinf( M_PI*(a-0.5) );}
};
struct EaseSinIn{
    float Interpolate(float a) {return 1 + sinf( M_PI*(a+3)/2 );}
};
struct EaseSinOut{
    float Interpolate(float a) {return sinf( M_PI*(a)/2 );}
};
struct EaseSquareIn{
    float Interpolate(float a) {return a * a;}
};

// T should be copyable, have defaault constructor, and have function lerp(T, T, float)
template<class T, class F = LinearInterpolation>
class Tween : private F
{
public:
    typedef std::function<void(const T&)> Setter_t;
    
    Tween() : m_from(), m_to(), m_progress(1), m_speed(1), m_setter(nullptr) {}
    Tween(const Setter_t& setter, const T& from, const T& to, float ms)
    : m_from(from), m_to(to), m_progress(0), m_speed(1.0/ms), m_setter(setter) {}
    void Reset(const Setter_t& setter, const T& from, const T& to, float ms)
    {
        m_setter = setter;
        m_from = from;
        m_to = to;
        m_progress = 0;
        m_speed = 1.0/ms;
    }
    void Tick(float dt)
    {
        if(!m_setter) return;
        if(IsFinished()) return;
        m_progress = dt * m_speed + m_progress;
        m_progress = fmin(1, m_progress);
        float alpha = this->F::Interpolate(m_progress);
        m_setter(lerp(m_from, m_to, alpha));
    }
    bool IsFinished() const {return m_progress == 1;}
    
private:
    T m_from;
    T m_to;
    float m_progress;
    float m_speed;
    Setter_t m_setter;
};
