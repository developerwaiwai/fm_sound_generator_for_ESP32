#include "fm_algorism.h"


void mcpwm_sound_soure_init()
{

    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, 18);    //Set GPIO 18 as PWM0A, to which servo is connected

    //2. initial mcpwm configuration
    printf("Configuring Initial Parameters of mcpwm......\n");
    mcpwm_config_t pwm_config;
    pwm_config.frequency = FREQ;    //frequency = 50Hz, i.e. for every servo motor time period should be 20ms
    pwm_config.cmpr_a = 0;    //duty cycle of PWMxA = 0
    pwm_config.cmpr_b = 0;    //duty cycle of PWMxb = 0
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);    //Configure PWM0A & PWM0B with above settings

}



void mcpwm_example_servo_control(algorism algor, bool noteOn, uint32_t helz, uint64_t time)
{
    time -= 10000;

    uint32_t angle;

    uint64_t start = esp_timer_get_time();
    uint64_t stop = esp_timer_get_time();
    while (1) {
        stop = esp_timer_get_time();
        if(stop - start >= time) {
            uint64_t start2 = esp_timer_get_time();
            uint64_t stop2 = esp_timer_get_time();
            while(1){
                stop2 = esp_timer_get_time();
                if(stop2 - start2 >= 10000) {
                    // mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, 0);
                    // dac_output_voltage(DAC_CHANNEL_1, 0);
                    break;
                }
            }
            break;
        }

        if(noteOn == false) {
            // mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, 0);
            // dac_output_voltage(DAC_CHANNEL_1, 0);
            return;
        }
        else
        {
            float Y = algor(helz);
            angle = herz_to_duty(Y);
            mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, angle);
            // dac_output_voltage(DAC_CHANNEL_1, 255 * Y);
        }
    }
}


void mcpwm_example_servo_control_4op(algolism_4op algor, bool noteOn, uint64_t time, struct algorism_param_4op *param1, struct algorism_param_4op *param2, struct algorism_param_4op *param3, struct algorism_param_4op *param4)
{
    time -= 10000;

    uint32_t angle;

    uint64_t start = esp_timer_get_time();
    uint64_t stop = esp_timer_get_time();
    while (1) {
        stop = esp_timer_get_time();
        if(stop - start >= time) {
            uint64_t start2 = esp_timer_get_time();
            uint64_t stop2 = esp_timer_get_time();
            while(1){
                stop2 = esp_timer_get_time();
                if(stop2 - start2 >= 10000) {
                    // mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, 0);
                    // dac_output_voltage(DAC_CHANNEL_1, 0);
                    break;
                }
            }
            break;
        }

        if(noteOn == false) {
            // mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, 0);
            // dac_output_voltage(DAC_CHANNEL_1, 0);
            return;
        }
        else
        {
            float Y = algor(param1, param2, param3, param4);
            angle = herz_to_duty(Y);
            mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, angle);
            // dac_output_voltage(DAC_CHANNEL_1, 255 * Y);
        }
    }
}
