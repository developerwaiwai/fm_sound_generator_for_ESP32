#include "fm_algorism.h"

float calc_sin_value(uint32_t helz)
{
    float Y = (calc_sin(helz) + 1 ) / 2;
    return Y;
}

float envelope(struct algorism_param_4op *param1, uint64_t t){

    float amp1 = 1.;

    if(param1->internal.atack_start_time == 0) {
        param1->internal.atack_start_time = t;
        amp1 = param1->attack ? 0. : 1.;
    }
    else {
        if(param1->attack != 0) {
            amp1 = ((float)(1. / (float)param1->attack) * (float)(t - param1->internal.atack_start_time));
        }
        if (amp1 >= 1.) {
            if(param1->internal.decay_start_time == 0) {
                param1->internal.decay_start_time = t;
                amp1 = param1->decay ? 1. : param1->sustain_level;
            }
            else {
                if(param1->decay != 0) {
                    amp1 = (1. - (float)((float)(1. / (float)param1->decay) * (float)(t - param1->internal.decay_start_time)));
                }
                if (amp1 <= param1->sustain_level) {
                    amp1 = param1->sustain_level;
                }
            }
        }
    }

    // for test
    // printf("%f", amp1);
    return amp1;
}


float YM2203_algolism0(struct algorism_param_4op *param1, struct algorism_param_4op *param2, struct algorism_param_4op *param3, struct algorism_param_4op *param4)
{
    uint64_t t = esp_timer_get_time();

    uint32_t helz1 = param1->helz_mul * param1->helz;
    uint32_t helz2 = param2->helz_mul * param2->helz;
    uint32_t helz3 = param3->helz_mul * param3->helz;
    uint32_t helz4 = param4->helz_mul * param4->helz;

    float amp1 = param1->amp * envelope(param1, t);
    float amp2 = param2->amp * envelope(param2, t);
    float amp3 = param3->amp * envelope(param3, t);
    float amp4 = param4->amp * envelope(param4, t);

    float main_helz = (float)helz4;
    float Y1 = amp1 * calc_sin_float((float)helz1, 0, t);
    float Y = amp4 * calc_sin_float((float)main_helz , amp3 * calc_sin_float((float)helz3, amp2 * calc_sin_float((float)helz2 , amp1 * calc_sin_float((float)helz1 ,Y1, t), t), t), t);

    // for test
    // float Y = param4->amp * calc_sin_float((float)helz4, param2->amp * calc_sin_float((float)helz2, 0, t), t);

    // for test
    // uint64_t t2 = esp_timer_get_time();
    // printf("%lld\n", t2-t);
    Y = (Y + param4->amp) / (param4->amp*2);
    return Y;
}


float YM2203_algolism1(struct algorism_param_4op *param1, struct algorism_param_4op *param2, struct algorism_param_4op *param3, struct algorism_param_4op *param4)
{
    uint64_t t = esp_timer_get_time();

    uint32_t helz1 = param1->helz_mul * param1->helz;
    uint32_t helz2 = param2->helz_mul * param2->helz;
    uint32_t helz3 = param3->helz_mul * param3->helz;
    uint32_t helz4 = param4->helz_mul * param4->helz;

    float amp1 = param1->amp * envelope(param1, t);
    float amp2 = param2->amp * envelope(param2, t);
    float amp3 = param3->amp * envelope(param3, t);
    float amp4 = param4->amp * envelope(param4, t);


    float Y1 = amp1 * calc_sin_float((float)helz1, 0, t);
    float Y2 = (amp1 * calc_sin_float((float)helz1, Y1, t)) + (amp2 * calc_sin_float((float)helz2, 0, t));
    float Y  = amp4 * calc_sin_float((float)helz4, amp3 * calc_sin_float((float)helz3, Y2, t), t);

    Y = (Y + param4->amp) / (param4->amp*2);
    return Y;
}


float YM2203_algolism2(struct algorism_param_4op *param1, struct algorism_param_4op *param2, struct algorism_param_4op *param3, struct algorism_param_4op *param4)
{
    uint64_t t = esp_timer_get_time();

    uint32_t helz1 = param1->helz_mul * param1->helz;
    uint32_t helz2 = param2->helz_mul * param2->helz;
    uint32_t helz3 = param3->helz_mul * param3->helz;
    uint32_t helz4 = param4->helz_mul * param4->helz;

    float amp1 = param1->amp * envelope(param1, t);
    float amp2 = param2->amp * envelope(param2, t);
    float amp3 = param3->amp * envelope(param3, t);
    float amp4 = param4->amp * envelope(param4, t);

    float Y1 = amp1 * calc_sin_float((float)helz1, amp1 * calc_sin_float((float)helz1, 0, t), t);
    float Y2 = amp3 * calc_sin_float((float)helz3, amp2 * calc_sin_float((float)helz2, 0, t), t);
    float Y  = amp4 * calc_sin_float((float)helz4, Y1 + Y2, t);

    Y = (Y + param4->amp) / (param4->amp*2);
    return Y;
}


float YM2203_algolism3(struct algorism_param_4op *param1, struct algorism_param_4op *param2, struct algorism_param_4op *param3, struct algorism_param_4op *param4)
{
    uint64_t t = esp_timer_get_time();

    uint32_t helz1 = param1->helz_mul * param1->helz;
    uint32_t helz2 = param2->helz_mul * param2->helz;
    uint32_t helz3 = param3->helz_mul * param3->helz;
    uint32_t helz4 = param4->helz_mul * param4->helz;

    float amp1 = param1->amp * envelope(param1, t);
    float amp2 = param2->amp * envelope(param2, t);
    float amp3 = param3->amp * envelope(param3, t);
    float amp4 = param4->amp * envelope(param4, t);

    float Y1 = amp1 * calc_sin_float((float)helz1, amp1 * calc_sin_float((float)helz1, 0, t), t);
    Y1 = amp2 * calc_sin_float((float)helz2, Y1, t);
    float Y2 = amp3 * calc_sin_float((float)helz3, 0, t);
    float Y = amp4 * calc_sin_float((float)helz4, Y1 + Y2, t);

    Y = (Y + param4->amp) / (param4->amp*2);
    return Y;
}


float YM2203_algolism4(struct algorism_param_4op *param1, struct algorism_param_4op *param2, struct algorism_param_4op *param3, struct algorism_param_4op *param4)
{
    uint64_t t = esp_timer_get_time();

    uint32_t helz1 = param1->helz_mul * param1->helz;
    uint32_t helz2 = param2->helz_mul * param2->helz;
    uint32_t helz3 = param3->helz_mul * param3->helz;
    uint32_t helz4 = param4->helz_mul * param4->helz;

    float amp1 = param1->amp * envelope(param1, t);
    float amp2 = param2->amp * envelope(param2, t);
    float amp3 = param3->amp * envelope(param3, t);
    float amp4 = param4->amp * envelope(param4, t);

    float Y1 = amp1 * calc_sin_float((float)helz1, amp1 * calc_sin_float((float)helz1, 0, t), t);
    float Y2 = amp2 * calc_sin_float((float)helz2, Y1, t);
    float Y3 = amp4 * calc_sin_float((float)helz4, amp3 * calc_sin_float((float)helz3, 0, t), t);

    float Y  = Y2 + Y3;

    float amp_total = param2->amp + param4->amp;
    Y = (Y + amp_total) / (amp_total * 2);
    return Y;
}


float YM2203_algolism5(struct algorism_param_4op *param1, struct algorism_param_4op *param2, struct algorism_param_4op *param3, struct algorism_param_4op *param4)
{
    uint64_t t = esp_timer_get_time();

    uint32_t helz1 = param1->helz_mul * param1->helz;
    uint32_t helz2 = param2->helz_mul * param2->helz;
    uint32_t helz3 = param3->helz_mul * param3->helz;
    uint32_t helz4 = param4->helz_mul * param4->helz;

    float amp1 = param1->amp * envelope(param1, t);
    float amp2 = param2->amp * envelope(param2, t);
    float amp3 = param3->amp * envelope(param3, t);
    float amp4 = param4->amp * envelope(param4, t);

    float Y1 = amp1 * calc_sin_float((float)helz1, amp1 * calc_sin_float((float)helz1, 0, t), t);
    float Y2 = amp2 * calc_sin_float((float)helz2, Y1, t);
    float Y3 = amp3 * calc_sin_float((float)helz3, Y1, t);
    float Y4 = amp4 * calc_sin_float((float)helz4, Y1, t);

    float Y = Y2+Y3+Y4;

    float amp_total = param2->amp + param3->amp + param4->amp;
    Y = (Y + amp_total) / (amp_total * 2);
    return Y;
}


float YM2203_algolism6(struct algorism_param_4op *param1, struct algorism_param_4op *param2, struct algorism_param_4op *param3, struct algorism_param_4op *param4)
{
    uint64_t t = esp_timer_get_time();

    uint32_t helz1 = param1->helz_mul * param1->helz;
    uint32_t helz2 = param2->helz_mul * param2->helz;
    uint32_t helz3 = param3->helz_mul * param3->helz;
    uint32_t helz4 = param4->helz_mul * param4->helz;

    float amp1 = param1->amp * envelope(param1, t);
    float amp2 = param2->amp * envelope(param2, t);
    float amp3 = param3->amp * envelope(param3, t);
    float amp4 = param4->amp * envelope(param4, t);

    float Y1 = amp1 * calc_sin_float((float)helz1, amp1 * calc_sin_float((float)helz1, 0, t), t);
    float Y2 = amp2 * calc_sin_float((float)helz2, Y1, t);
    float Y3 = amp3 * calc_sin_float((float)helz3, 0, t);
    float Y4 = amp4 * calc_sin_float((float)helz4, 0, t);

    float Y = Y2+Y3+Y4;

    float amp_total = param2->amp + param3->amp + param4->amp;
    Y = (Y + amp_total) / (amp_total * 2);
    return Y;
}


float YM2203_algolism7(struct algorism_param_4op *param1, struct algorism_param_4op *param2, struct algorism_param_4op *param3, struct algorism_param_4op *param4)
{
    uint64_t t = esp_timer_get_time();

    uint32_t helz1 = param1->helz_mul * param1->helz;
    uint32_t helz2 = param2->helz_mul * param2->helz;
    uint32_t helz3 = param3->helz_mul * param3->helz;
    uint32_t helz4 = param4->helz_mul * param4->helz;

    float amp1 = param1->amp * envelope(param1, t);
    float amp2 = param2->amp * envelope(param2, t);
    float amp3 = param3->amp * envelope(param3, t);
    float amp4 = param4->amp * envelope(param4, t);

    float Y1 = amp1 * calc_sin_float((float)helz1, amp1 * calc_sin_float((float)helz1, 0, t), t);
    float Y2 = amp2 * calc_sin_float((float)helz2, 0, t);
    float Y3 = amp3 * calc_sin_float((float)helz3, 0, t);
    float Y4 = amp4 * calc_sin_float((float)helz4, 0, t);
    float Y = Y1+Y2+Y3+Y4;

    float amp_total = param1->amp + param2->amp + param3->amp + param4->amp;
    Y = (Y + amp_total) / (amp_total*2);
    return Y;
}
