#include "fm_algorism.h"
#include "playing_command.h"
#include <string.h>

#define defaultTempo 120
#define whole_note 2000000
#define half_note 1000000
#define quoater_note 500000
#define eighth_note 250000


// struct note_param {
//     algolism_4op playing_algorism;
//     struct algorism_param_4op param1, param2, param3, param4;
//
//     bool        note_on_off;
//     uint16_t    tempo;
//     uint64_t    note_length;
//
//     bool        is_current;
//     struct note_param* prev;
//     struct note_param* next;
// };





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
    uint64_t note_length = note *  defaultTempo / tempo;
    param1->internal.atack_start_time = param1->internal.decay_start_time = param1->internal.release_start_time = 0;
    param2->internal.atack_start_time = param2->internal.decay_start_time = param2->internal.release_start_time = 0;
    param3->internal.atack_start_time = param3->internal.decay_start_time = param3->internal.release_start_time = 0;
    param4->internal.atack_start_time = param4->internal.decay_start_time = param4->internal.release_start_time = 0;

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


/*------------------------------------------------------------------------------
*
* ここからシリアル通信のやつ
*
*/

#define command_param1      0x01
#define command_param2      0x02
#define command_param3      0x03
#define command_param4      0x04

#define command_note_on     0x10
#define command_note_off    0x11


#define RW_LENGTH 34               /*!< Data length for r/w test, [0,DATA_LENGTH] */

struct ocirator_param_i2c {
    uint16_t    amp100;
    uint16_t    mul;
    uint32_t    helz;

    uint64_t    attack;
    uint64_t    decay;
    uint16_t    sus_level100;
    uint16_t    release_level100;
};

struct note_param_i2c {
    uint16_t    on_off;
    uint16_t    algorism;
    uint16_t    tempo;
    uint16_t    helz;
    uint64_t    note_type;
    uint64_t    reserved[2];
};

#define ECHO_TEST_TXD  (GPIO_NUM_15)
#define ECHO_TEST_RXD  (GPIO_NUM_2)
#define ECHO_TEST_RTS  (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS  (UART_PIN_NO_CHANGE)

#define BUF_SIZE (1024)

struct ocirator_param_i2c param1,param2,param3,param4;


void make_algorism_param(struct note_param_i2c i2c_note, struct ocirator_param_i2c i2c_param, struct algorism_param_4op* alg_param) {
    alg_param->amp = (float)((float)i2c_param.amp100 / 100.);
    alg_param->helz_mul = i2c_param.mul;
    alg_param->helz = i2c_note.helz;
    alg_param->attack = i2c_param.attack;
    alg_param->decay = i2c_param.decay;
    alg_param->sustain_level = (float)((float)i2c_param.sus_level100 / 100.);
    alg_param->release = (float)((float)i2c_param.release_level100 / 100.);
    alg_param->muled_helz = alg_param->helz_mul * alg_param->helz ;
}

void music_data_receive(void* arg)
{
    char ACK = 1;
    char NACK = 0;

    struct algorism_param_4op alg_param1, alg_param2, alg_param3, alg_param4;
    struct note_param_i2c note_i2c;

    uint8_t *data = (uint8_t *) malloc(BUF_SIZE);
    while(1) {
        // onkai(NULL);
        int len = uart_read_bytes(UART_NUM_1, data, RW_LENGTH, 1000);

        if(len <= 0) continue;

        switch(data[0]) {
            case command_param1:
                // printf("\n");
                // printf("Received1, ");
                memcpy(&param1, &data[2], sizeof(struct ocirator_param_i2c));
                // printf("%d, ", param1.amp100);
                // printf("%d, ", param1.mul);
                // printf("%d, ", param1.helz);
                // printf("%lld, ", param1.attack);
                // printf("%lld, ", param1.decay);
                // printf("%d, ", param1.sus_level100);
                // printf("%d, ", param1.release_level100);

                uart_write_bytes(UART_NUM_1, &ACK, 1);
                break;

            case command_param2:
                // printf("\n");
                // printf("Received2");
                memcpy(&param2, &data[2], sizeof(struct ocirator_param_i2c));
                uart_write_bytes(UART_NUM_1, &ACK, 1);
                break;

            case command_param3:
                // printf("\n");
                // printf("Received3");
                memcpy(&param3, &data[2], sizeof(struct ocirator_param_i2c));
                uart_write_bytes(UART_NUM_1, &ACK, 1);
                break;

            case command_param4:
                // printf("\n");
                // printf("Received4");
                memcpy(&param4, &data[2], sizeof(struct ocirator_param_i2c));
                uart_write_bytes(UART_NUM_1, &ACK, 1);
                break;

            case command_note_on:
            case command_note_off:
                // printf("\n");
                // printf("Received16, ");
                memcpy(&note_i2c, &data[2], sizeof(struct note_param_i2c));
                // printf("\n");
                // printf("%d, ", note_i2c.on_off);
                // printf("%d, ", note_i2c.algorism);
                // printf("%d, ", note_i2c.tempo);
                // printf("%d, ", note_i2c.helz);
                // printf("%lld, ", note_i2c.note_type);
                // printf("\n");

                make_algorism_param(note_i2c, param1, &alg_param1);
                make_algorism_param(note_i2c, param2, &alg_param2);
                make_algorism_param(note_i2c, param3, &alg_param3);
                make_algorism_param(note_i2c, param4, &alg_param4);

                algolism_4op algorism;
                switch(note_i2c.algorism) {
                    case 0:
                        algorism = YM2203_algolism0;
                        break;
                    case 1:
                        algorism = YM2203_algolism1;
                        break;
                    case 2:
                        algorism = YM2203_algolism2;
                        break;
                    case 3:
                        algorism = YM2203_algolism3;
                        break;
                    case 4:
                        algorism = YM2203_algolism4;
                        break;
                    case 5:
                        algorism = YM2203_algolism5;
                        break;
                    case 6:
                        algorism = YM2203_algolism6;
                        break;
                    case 7:
                        algorism = YM2203_algolism7;
                        break;
                    default:
                        algorism = YM2203_algolism0;
                        break;
                }

                note_4algorism2(algorism, note_i2c.on_off, note_i2c.tempo, note_i2c.note_type, &alg_param1, &alg_param2, &alg_param3, &alg_param4);
                uart_write_bytes(UART_NUM_1, &ACK, 1);
                break;
        }
    }
}




void app_main(void)
{

    mcpwm_sound_soure_init();
    // xTaskCreate(onkai, "onkai", 4096, NULL, 5, NULL);

    // spi_initialize();
    // xTaskCreatePinnedToCore(spi_receive_task, "spi_receive_task", 4096, NULL, 5, NULL, 1);


    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    uart_driver_install(UART_NUM_1, BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, ECHO_TEST_TXD, ECHO_TEST_RXD, ECHO_TEST_RTS, ECHO_TEST_CTS);

    xTaskCreatePinnedToCore(music_data_receive, "music_data_receive", 4096, NULL, 10, NULL, 0);

    printf("end");
}
