#include <stdio.h>
#include <math.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_attr.h"

#include "driver/periph_ctrl.h"
#include "driver/timer.h"

#include "driver/mcpwm.h"
#include "driver/uart.h"
#include "driver/gpio.h"

#include "soc/mcpwm_periph.h"


#define FREQ           20000
#define MIN_PULSEWIDTH 0     //Minimum pulse width in microsecond
#define MAX_PULSEWIDTH 50    //Maximum pulse width in microsecond


struct internal_params {
    uint64_t atack_start_time;
    uint64_t decay_start_time;
    uint64_t release_start_time;
};

struct algorism_param_4op {
    float       amp;
    uint32_t    helz_mul;
    uint32_t    helz;

    uint64_t    attack;
    uint64_t    decay;
    float       sustain_level;
    float       release;

    struct internal_params internal;
};

typedef float (*algorism)(uint32_t helz);
typedef float (*algolism_4op)(struct algorism_param_4op *param1, struct algorism_param_4op *param2, struct algorism_param_4op *param3, struct algorism_param_4op *param4);


float calc_sin_value(uint32_t helz);

float YM2203_algolism0(struct algorism_param_4op *param1, struct algorism_param_4op *param2, struct algorism_param_4op *param3, struct algorism_param_4op *param4);
float YM2203_algolism1(struct algorism_param_4op *param1, struct algorism_param_4op *param2, struct algorism_param_4op *param3, struct algorism_param_4op *param4);
float YM2203_algolism2(struct algorism_param_4op *param1, struct algorism_param_4op *param2, struct algorism_param_4op *param3, struct algorism_param_4op *param4);
float YM2203_algolism3(struct algorism_param_4op *param1, struct algorism_param_4op *param2, struct algorism_param_4op *param3, struct algorism_param_4op *param4);
float YM2203_algolism4(struct algorism_param_4op *param1, struct algorism_param_4op *param2, struct algorism_param_4op *param3, struct algorism_param_4op *param4);
float YM2203_algolism5(struct algorism_param_4op *param1, struct algorism_param_4op *param2, struct algorism_param_4op *param3, struct algorism_param_4op *param4);
float YM2203_algolism6(struct algorism_param_4op *param1, struct algorism_param_4op *param2, struct algorism_param_4op *param3, struct algorism_param_4op *param4);
float YM2203_algolism7(struct algorism_param_4op *param1, struct algorism_param_4op *param2, struct algorism_param_4op *param3, struct algorism_param_4op *param4);


float calc_sin(uint32_t helz);
float calc_sin_float(float helz, float mod, uint64_t now_time_in_us);
uint32_t herz_to_duty(float Y);


void mcpwm_sound_soure_init();
void mcpwm_example_servo_control(algorism algor, bool noteOn, uint32_t helz, uint64_t time);
void mcpwm_example_servo_control_4op(algolism_4op algor, bool noteOn, uint64_t time, struct algorism_param_4op *param1, struct algorism_param_4op *param2, struct algorism_param_4op *param3, struct algorism_param_4op *param4);
void mcpwm_example_servo_control_4op2(algolism_4op algor, bool noteOn, uint64_t time, struct algorism_param_4op *param1, struct algorism_param_4op *param2, struct algorism_param_4op *param3, struct algorism_param_4op *param4);
