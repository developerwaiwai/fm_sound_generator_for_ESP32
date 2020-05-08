#include "fm_algorism.h"
#include <string.h>
#include "esp32/rom/ets_sys.h"


struct note_param {
    algolism_4op playing_algorism;
    struct algorism_param_4op param1, param2, param3, param4;

    bool        note_on_off;
    uint64_t    note_length;

    uint64_t    start;
    bool        start_flag;
};

volatile struct note_param param_g;


typedef struct {
    int type;  // the type of timer's event
    int timer_group;
    int timer_idx;
    uint64_t timer_counter_value;
} timer_event_t;

xQueueHandle timer_queue;
xQueueHandle note_queue;


void fm_algorism_timer_init();


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

    fm_algorism_timer_init();
}


void IRAM_ATTR timer_group0_isr(void *para)
{
    // timer_spinlock_take(TIMER_GROUP_0);
    int timer_idx = (int) para;

    uint32_t timer_intr = timer_group_get_intr_status_in_isr(TIMER_GROUP_0);
    // uint64_t timer_counter_value = timer_group_get_counter_value_in_isr(TIMER_GROUP_0, timer_idx);

    timer_event_t evt;
    evt.timer_group = 0;
    evt.timer_idx = timer_idx;
    evt.timer_counter_value = esp_timer_get_time();

    timer_group_clr_intr_status_in_isr(TIMER_GROUP_0, timer_idx);
    timer_group_enable_alarm_in_isr(TIMER_GROUP_0, timer_idx);

    if(param_g.note_length > 0 && evt.timer_counter_value - param_g.start >= param_g.note_length) {
        BaseType_t received = xQueueReceiveFromISR(note_queue, &param_g, NULL);
        if(received == pdTRUE) {
            param_g.start = esp_timer_get_time();
            param_g.start_flag = true;
        }
        else{
            param_g.start = 0;
            param_g.start_flag = false;
        }
    }

    xQueueOverwriteFromISR(timer_queue, &evt, NULL);
    // timer_spinlock_give(TIMER_GROUP_0);
}


void IRAM_ATTR timer_example_evt_task(void *arg)
{
    // uint32_t angle;

    uint64_t start = 0;
    uint64_t stop = 0;
    while (1) {
        timer_event_t evt;
        xQueueReceive(timer_queue, &evt, portMAX_DELAY);

        uint64_t t1 = stop = esp_timer_get_time();
        // uint64_t t1 = stop = evt.timer_counter_value;

        if(param_g.start == 0 && param_g.start_flag == false) {
            mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, 0);
            continue;
        }

        float Y = param_g.playing_algorism(&param_g.param1, &param_g.param2, &param_g.param3, &param_g.param4, t1);
        mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, MAX_PULSEWIDTH * Y);
        // uint64_t t2 = esp_timer_get_time();
        // printf("time = %lld\n", t2 - t1);
    }
}


void fm_algorism_timer_init()
{
    memset(&param_g, 0 , sizeof(struct note_param));
    param_g.param1.amp = 1.;
    param_g.param1.helz_mul = 1;
    param_g.param1.helz = 440;
    param_g.param1.muled_helz = 440;
    param_g.param1.attack = 0;
    param_g.param1.decay = 0;
    param_g.param1.sustain_level = 1.;
    param_g.param1.release = 0.;
    memcpy (&(param_g.param2), &(param_g.param1), sizeof(struct algorism_param_4op));
    memcpy (&(param_g.param3), &(param_g.param1), sizeof(struct algorism_param_4op));
    memcpy (&(param_g.param4), &(param_g.param1), sizeof(struct algorism_param_4op));

    param_g.playing_algorism = YM2203_algolism0;
    param_g.note_on_off = false;
    param_g.note_length = 0;
    param_g.start = 0;
    param_g.start_flag = 0;

    timer_queue = xQueueCreate(1, sizeof(timer_event_t));
    xTaskCreatePinnedToCore(timer_example_evt_task, "timer_evt_task", 2048, NULL, 5, NULL, 0);

    note_queue = xQueueCreate(10, sizeof(struct note_param));

    int timer_idx = TIMER_0;

    timer_config_t config;
    config.divider = 80;
    config.counter_dir = TIMER_COUNT_UP;
    config.counter_en = TIMER_PAUSE;
    config.alarm_en = TIMER_ALARM_EN;
    config.intr_type = TIMER_INTR_LEVEL;
    config.auto_reload = 1;
#ifdef TIMER_GROUP_SUPPORTS_XTAL_CLOCK
    config.clk_src = TIMER_SRC_CLK_APB;
#endif
    timer_init(TIMER_GROUP_0, 0, &config);

    timer_init(TIMER_GROUP_0, 0, &config);

    /* Timer's counter will initially start from value below.
       Also, if auto_reload is set, this value will be automatically reload on alarm */
    timer_set_counter_value(TIMER_GROUP_0, 0, 0x00000000ULL);

    /* Configure the alarm value and the interrupt on alarm. */
    timer_set_alarm_value(TIMER_GROUP_0, 0, 25);
    timer_enable_intr(TIMER_GROUP_0, 0);
    timer_isr_register(TIMER_GROUP_0, 0, timer_group0_isr,
        (void *) timer_idx, ESP_INTR_FLAG_IRAM, NULL);

    timer_start(TIMER_GROUP_0, timer_idx);
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


bool mcpwm_example_servo_control_4op2(algolism_4op algor, bool noteOn, uint64_t time, struct algorism_param_4op *param1, struct algorism_param_4op *param2, struct algorism_param_4op *param3, struct algorism_param_4op *param4)
{

    struct note_param param_local;

    param1->muled_helz = param1->helz_mul * param1->helz;
    param2->muled_helz = param2->helz_mul * param2->helz;
    param3->muled_helz = param3->helz_mul * param3->helz;
    param4->muled_helz = param4->helz_mul * param4->helz;

    param_local.playing_algorism = algor;
    param_local.note_on_off = noteOn;
    param_local.note_length = time;

    memcpy(&param_local.param1, param1, sizeof(struct algorism_param_4op));
    memcpy(&param_local.param2, param2, sizeof(struct algorism_param_4op));
    memcpy(&param_local.param3, param3, sizeof(struct algorism_param_4op));
    memcpy(&param_local.param4, param4, sizeof(struct algorism_param_4op));

    // printf("go");
    if(param_g.start != 0 && param_g.start_flag == true) {
        xQueueSend(note_queue, &param_local, portMAX_DELAY);
        // printf("queue\n");
        return true;
    }

    // BaseType_t num = uxQueueMessagesWaiting(note_queue);
    // printf("q num = %d\n", num);
    if(uxQueueMessagesWaiting(note_queue) == 0) {
        memcpy(&param_g, &param_local, sizeof(struct note_param));
        param_g.start = esp_timer_get_time();
        param_g.start_flag = true;
        // printf("direct\n");
    }
    return true;
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
            float Y = algor(param1, param2, param3, param4, start);
            angle = herz_to_duty(Y);
            mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, angle);
            // dac_output_voltage(DAC_CHANNEL_1, 255 * Y);
        }
    }
}
