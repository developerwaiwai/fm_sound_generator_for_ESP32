#include "fm_algorism.h"
#include "playing_command.h"


#define defaultTempo 120
#define whole_note 2000000
#define half_note 1000000
#define quoater_note 500000
#define eighth_note 250000


struct note_param {
    algolism_4op playing_algorism;
    struct algorism_param_4op param1, param2, param3, param4;

    bool        note_on_off;
    uint16_t    tempo;
    uint64_t    note_length;

    bool        is_current;
    struct note_param* prev;
    struct note_param* next;
};





// volatile struct note_param*  playing_queue       = NULL;
// volatile struct note_param*  now_playing         = NULL;
//
// void prepare_note(note_param* param){
//     param->next = NULL;
//     if(playing_queue == NULL) {
//         param->prev = NULL;
//         playing_queue = param;
//     }
//     else {
//         note_param q = playing_queue;
//         do {
//             q = q->next;
//         } while( q->next == NULL);
//         param->prev = q;
//         q->next = param;
//     }
// }


void note_4algorism2(algolism_4op algorism, bool note_on, uint16_t tempo, uint64_t note, struct algorism_param_4op *param1, struct algorism_param_4op *param2, struct algorism_param_4op *param3, struct algorism_param_4op *param4)
{
    uint64_t note_length = note * tempo / defaultTempo;

    mcpwm_example_servo_control_4op2(algorism, note_on, note_length, param1, param2, param3, param4);
}


void note_4algorism(algolism_4op algorism, bool note_on, uint16_t tempo, uint64_t note, struct algorism_param_4op *param1, struct algorism_param_4op *param2, struct algorism_param_4op *param3, struct algorism_param_4op *param4)
{
    uint64_t note_length = note * tempo / defaultTempo;

    mcpwm_example_servo_control_4op(algorism, note_on, note_length, param1, param2, param3, param4);
}


void note(algorism algor, bool note_on, uint16_t helz, uint16_t tempo, uint64_t note)
{

    uint64_t note_length = note * tempo / defaultTempo;
    mcpwm_example_servo_control(algor, note_on, helz, note_length);

}

/*------------------------------------------------------------------------------
*/


void clear_param(struct algorism_param_4op *param1){

    param1->internal.atack_start_time = param1->internal.decay_start_time = param1->internal.release_start_time = 0;
    param1->attack = 300; param1->decay = 1000000; param1->sustain_level = 0.0;

}

void onkai(void* arg)
{
    vTaskDelay(10);
    while(1)
    {

        struct algorism_param_4op param1;
        struct algorism_param_4op param2;
        struct algorism_param_4op param3;
        struct algorism_param_4op param4;

        param1.amp = 1.0;
        param2.amp = 0.5;
        param3.amp = 3.0;
        param4.amp = 1.0;

        param1.helz_mul = 1;
        param2.helz_mul = 2;
        param3.helz_mul = 3;
        param4.helz_mul = 4;

        algolism_4op algorism_[8];
        algorism_[0] = YM2203_algolism0;
        algorism_[1] = YM2203_algolism1;
        algorism_[2] = YM2203_algolism2;
        algorism_[3] = YM2203_algolism3;
        algorism_[4] = YM2203_algolism4;
        algorism_[5] = YM2203_algolism5;
        algorism_[6] = YM2203_algolism6;
        algorism_[7] = YM2203_algolism7;

        // param1.helz = param2.helz = param3.helz = param4.helz = 392;
        // clear_param(&param1); clear_param(&param2); clear_param(&param3); clear_param(&param4);
        // note_4algorism2(YM2203_algolism7, true, defaultTempo, quoater_note,  &param1, &param2, &param3, &param4);
        // vTaskDelay(100);


        for (int i = 0 ; i < 8 ; ++i) {
            algolism_4op algorism = algorism_[i];


            param1.helz = param2.helz = param3.helz = param4.helz = 392;
            clear_param(&param1); clear_param(&param2); clear_param(&param3); clear_param(&param4);
            note_4algorism2(algorism, true, defaultTempo, quoater_note,  &param1, &param2, &param3, &param4);
            param1.helz = param2.helz = param3.helz = param4.helz = 330;
            clear_param(&param1); clear_param(&param2); clear_param(&param3); clear_param(&param4);
            note_4algorism2(algorism, true, defaultTempo, quoater_note,  &param1, &param2, &param3, &param4);
            clear_param(&param1); clear_param(&param2); clear_param(&param3); clear_param(&param4);
            note_4algorism2(algorism, true, defaultTempo, half_note,     &param1, &param2, &param3, &param4);

            param1.helz = param2.helz = param3.helz = param4.helz = 349;
            clear_param(&param1); clear_param(&param2); clear_param(&param3); clear_param(&param4);
            note_4algorism2(algorism, true, defaultTempo, quoater_note,  &param1, &param2, &param3, &param4);
            param1.helz = param2.helz = param3.helz = param4.helz = 294;
            clear_param(&param1); clear_param(&param2); clear_param(&param3); clear_param(&param4);
            note_4algorism2(algorism, true, defaultTempo, quoater_note,  &param1, &param2, &param3, &param4);
            clear_param(&param1); clear_param(&param2); clear_param(&param3); clear_param(&param4);
            note_4algorism2(algorism, true, defaultTempo, half_note,     &param1, &param2, &param3, &param4);

            param1.helz = param2.helz = param3.helz = param4.helz = 262;
            clear_param(&param1); clear_param(&param2); clear_param(&param3); clear_param(&param4);
            note_4algorism2(algorism, true, defaultTempo, quoater_note,  &param1, &param2, &param3, &param4);
            param1.helz = param2.helz = param3.helz = param4.helz = 294;
            clear_param(&param1); clear_param(&param2); clear_param(&param3); clear_param(&param4);
            note_4algorism2(algorism, true, defaultTempo, quoater_note,  &param1, &param2, &param3, &param4);
            param1.helz = param2.helz = param3.helz = param4.helz = 330;
            clear_param(&param1); clear_param(&param2); clear_param(&param3); clear_param(&param4);
            note_4algorism2(algorism, true, defaultTempo, quoater_note,  &param1, &param2, &param3, &param4);
            param1.helz = param2.helz = param3.helz = param4.helz = 349;
            clear_param(&param1); clear_param(&param2); clear_param(&param3); clear_param(&param4);
            note_4algorism2(algorism, true, defaultTempo, quoater_note,  &param1, &param2, &param3, &param4);

            param1.helz = param2.helz = param3.helz = param4.helz = 392;
            clear_param(&param1); clear_param(&param2); clear_param(&param3); clear_param(&param4);
            note_4algorism2(algorism, true, defaultTempo, quoater_note,  &param1, &param2, &param3, &param4);
            clear_param(&param1); clear_param(&param2); clear_param(&param3); clear_param(&param4);
            note_4algorism2(algorism, true, defaultTempo, quoater_note,  &param1, &param2, &param3, &param4);
            clear_param(&param1); clear_param(&param2); clear_param(&param3); clear_param(&param4);
            note_4algorism2(algorism, true, defaultTempo, half_note,     &param1, &param2, &param3, &param4);

        }


        param1.amp = 0.0;
        param2.amp = 0.0;
        param3.amp = 0.0;
        param4.amp = 1.0;

        param4.helz_mul = 1;


        param1.helz = param2.helz = param3.helz = param4.helz = 392;
        clear_param(&param1); clear_param(&param2); clear_param(&param3); clear_param(&param4);
        note_4algorism2(YM2203_algolism7, true, defaultTempo, quoater_note,  &param1, &param2, &param3, &param4);
        param1.helz = param2.helz = param3.helz = param4.helz = 330;
        clear_param(&param1); clear_param(&param2); clear_param(&param3); clear_param(&param4);
        note_4algorism2(YM2203_algolism7, true, defaultTempo, quoater_note,  &param1, &param2, &param3, &param4);
        clear_param(&param1); clear_param(&param2); clear_param(&param3); clear_param(&param4);
        note_4algorism2(YM2203_algolism7, true, defaultTempo, half_note,     &param1, &param2, &param3, &param4);

        param1.helz = param2.helz = param3.helz = param4.helz = 349;
        clear_param(&param1); clear_param(&param2); clear_param(&param3); clear_param(&param4);
        note_4algorism2(YM2203_algolism7, true, defaultTempo, quoater_note,  &param1, &param2, &param3, &param4);
        param1.helz = param2.helz = param3.helz = param4.helz = 294;
        clear_param(&param1); clear_param(&param2); clear_param(&param3); clear_param(&param4);
        note_4algorism2(YM2203_algolism7, true, defaultTempo, quoater_note,  &param1, &param2, &param3, &param4);
        clear_param(&param1); clear_param(&param2); clear_param(&param3); clear_param(&param4);
        note_4algorism2(YM2203_algolism7, true, defaultTempo, half_note,     &param1, &param2, &param3, &param4);

        param1.helz = param2.helz = param3.helz = param4.helz = 262;
        clear_param(&param1); clear_param(&param2); clear_param(&param3); clear_param(&param4);
        note_4algorism2(YM2203_algolism7, true, defaultTempo, quoater_note,  &param1, &param2, &param3, &param4);
        param1.helz = param2.helz = param3.helz = param4.helz = 294;
        clear_param(&param1); clear_param(&param2); clear_param(&param3); clear_param(&param4);
        note_4algorism2(YM2203_algolism7, true, defaultTempo, quoater_note,  &param1, &param2, &param3, &param4);
        param1.helz = param2.helz = param3.helz = param4.helz = 330;
        clear_param(&param1); clear_param(&param2); clear_param(&param3); clear_param(&param4);
        note_4algorism2(YM2203_algolism7, true, defaultTempo, quoater_note,  &param1, &param2, &param3, &param4);
        param1.helz = param2.helz = param3.helz = param4.helz = 349;
        clear_param(&param1); clear_param(&param2); clear_param(&param3); clear_param(&param4);
        note_4algorism2(YM2203_algolism7, true, defaultTempo, quoater_note,  &param1, &param2, &param3, &param4);

        param1.helz = param2.helz = param3.helz = param4.helz = 392;
        clear_param(&param1); clear_param(&param2); clear_param(&param3); clear_param(&param4);
        note_4algorism2(YM2203_algolism7, true, defaultTempo, quoater_note,  &param1, &param2, &param3, &param4);
        clear_param(&param1); clear_param(&param2); clear_param(&param3); clear_param(&param4);
        note_4algorism2(YM2203_algolism7, true, defaultTempo, quoater_note,  &param1, &param2, &param3, &param4);
        clear_param(&param1); clear_param(&param2); clear_param(&param3); clear_param(&param4);
        note_4algorism2(YM2203_algolism7, true, defaultTempo, half_note,     &param1, &param2, &param3, &param4);



        //
        // note(calc_sin_value, true, 392, defaultTempo, quoater_note);
        // note(calc_sin_value, true, 330, defaultTempo, quoater_note);
        // note(calc_sin_value, true, 330, defaultTempo, half_note);
        // note(calc_sin_value, false, 330, defaultTempo, half_note);
        //
        // note(calc_sin_value, true, 349, defaultTempo, quoater_note);
        // note(calc_sin_value, true, 294, defaultTempo, quoater_note);
        // note(calc_sin_value, true, 294, defaultTempo, half_note);
        // note(calc_sin_value, false, 330, defaultTempo, half_note);
        //
        // note(calc_sin_value, true, 262, defaultTempo, quoater_note);
        // note(calc_sin_value, true, 294, defaultTempo, quoater_note);
        // note(calc_sin_value, true, 330, defaultTempo, quoater_note);
        // note(calc_sin_value, true, 349, defaultTempo, quoater_note);
        // note(calc_sin_value, true, 392, defaultTempo, quoater_note);
        //
        // note(calc_sin_value, true, 392, defaultTempo, quoater_note);
        //
        // note(calc_sin_value, true, 392, defaultTempo, half_note);
        // note(calc_sin_value, false, 392, defaultTempo, half_note);
        //
        // vTaskDelay(100);
    }
}


#define ECHO_TEST_TXD  (GPIO_NUM_4)
#define ECHO_TEST_RXD  (GPIO_NUM_5)
#define ECHO_TEST_RTS  (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS  (UART_PIN_NO_CHANGE)

#define BUF_SIZE (1024)


static xQueueHandle command_queue = NULL;


// void music_data_receive(void* arg)
// {
//     uint8_t *data = (uint8_t *) malloc(BUF_SIZE);
//     while(1) {
//         int len = uart_read_bytes(UART_NUM_1, data, sizeof(struct packet), 20 / portTICK_RATE_MS);
//
//         struct packet *p = data;
//         print("received command = %d", p->command);
//
//         xQueueSend(command_queue, p, 0);
//     }
// }

void app_main(void)
{

    mcpwm_sound_soure_init();
    xTaskCreate(onkai, "onkai", 4096, NULL, 5, NULL);

    // command_queue = xQueueCreate(10, sizeof(struct packet));
    //
    // uart_config_t uart_config = {
    //     .baud_rate = 115200,
    //     .data_bits = UART_DATA_8_BITS,
    //     .parity    = UART_PARITY_DISABLE,
    //     .stop_bits = UART_STOP_BITS_1,
    //     .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    //     .source_clk = UART_SCLK_APB,
    // };
    // uart_driver_install(UART_NUM_1, BUF_SIZE * 2, 0, 0, NULL, 0);
    // uart_param_config(UART_NUM_1, &uart_config);
    // uart_set_pin(UART_NUM_1, ECHO_TEST_TXD, ECHO_TEST_RXD, ECHO_TEST_RTS, ECHO_TEST_CTS);
    //
    // xTaskCreatePinnedToCore(echo_task, "music_data_receive", 1024, NULL, 10, NULL, 0);

    printf("end");
}
