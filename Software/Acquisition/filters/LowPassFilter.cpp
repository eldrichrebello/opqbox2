//
// Created by tusk on 8/29/16.
//

#include "LowPassFilter.hpp"

static float filter_taps[LOWPASSFILTER_TAP_NUM] = {
        -0.000242972945201603,
        0.00931009681576234,
        0.012274256140224659,
        0.017695281178896027,
        0.02222714075752265,
        0.02453151402463442,
        0.02359360628591471,
        0.01901638351399286,
        0.011274993428201667,
        0.001728318226649868,
        -0.007612398351600763,
        -0.014501616648421175,
        -0.017112499894529325,
        -0.0145383951431215,
        -0.007217925289049719,
        0.003046469021138364,
        0.013411026461527755,
        0.020670142457790953,
        0.02213690098599959,
        0.016488858935162876,
        0.004345037579781999,
        -0.011604205168388126,
        -0.02703330842530798,
        -0.036877537881394876,
        -0.03654981187239224,
        -0.02315095372500509,
        0.003614581913363816,
        0.04108045393549397,
        0.08395023253315499,
        0.12527264516297337,
        0.15785608793765463,
        0.17580539103103388,
        0.17580539103103388,
        0.15785608793765463,
        0.12527264516297337,
        0.08395023253315499,
        0.04108045393549397,
        0.003614581913363816,
        -0.02315095372500509,
        -0.03654981187239224,
        -0.036877537881394876,
        -0.02703330842530798,
        -0.011604205168388126,
        0.004345037579781992,
        0.016488858935162876,
        0.022136900985999585,
        0.020670142457790953,
        0.013411026461527755,
        0.003046469021138364,
        -0.007217925289049719,
        -0.0145383951431215,
        -0.017112499894529325,
        -0.014501616648421175,
        -0.007612398351600763,
        0.0017283182266498685,
        0.011274993428201667,
        0.01901638351399286,
        0.02359360628591471,
        0.024531514024634417,
        0.022227140757522655,
        0.017695281178896027,
        0.012274256140224659,
        0.009310096815762343,
        -0.000242972945201603
};

void LowPassFilter_init(LowPassFilter* f) {
    int i;
    for(i = 0; i < LOWPASSFILTER_TAP_NUM; ++i)
        f->history[i] = 0;
    f->last_index = 0;
}

void LowPassFilter_put(LowPassFilter* f, float input) {
    f->history[(f->last_index++) & 63] = input;
}

float LowPassFilter_get(LowPassFilter* f) {
    float acc = 0;
    int index = f->last_index, i;
    for(i = 0; i < LOWPASSFILTER_TAP_NUM; ++i) {
        acc += f->history[(index--) & 63] * filter_taps[i];
    };
    return acc;
}