#include <stdio.h>
#include <string.h>
#include "driver/i2c.h"
#include "fm_algorism.h"
#include "sdkconfig.h"

#define command_param1      0x01
#define command_param2      0x02
#define command_param3      0x03
#define command_param4      0x04

#define command_note_on     0x10
#define command_note_off    0x11


struct ocirator_param_i2c {
    uint16_t    amp100;
    uint16_t    padding;
    uint32_t    mul;
    uint32_t    helz;

    uint64_t    attack;
    uint64_t    decay;
    uint16_t    sus_level100;
    uint16_t    release_level100;
};

struct note_param_i2c {
    char        on_off;
    char        algorism;
    char        padding1[2];
    uint16_t    tempo;
    uint16_t    padding2;
    uint64_t    note_type;

    uint32_t    padding3[4];
};


#define _I2C_NUMBER(num) I2C_NUM_##num
#define I2C_NUMBER(num) _I2C_NUMBER(num)

#define DATA_LENGTH 512                  /*!< Data buffer length of test buffer */
#define RW_LENGTH 33               /*!< Data length for r/w test, [0,DATA_LENGTH] */

#define I2C_SLAVE_SCL_IO CONFIG_I2C_SLAVE_SCL               /*!< gpio number for i2c slave clock */
#define I2C_SLAVE_SDA_IO CONFIG_I2C_SLAVE_SDA               /*!< gpio number for i2c slave data */
#define I2C_SLAVE_NUM I2C_NUMBER(CONFIG_I2C_SLAVE_PORT_NUM) /*!< I2C port number for slave dev */
#define I2C_SLAVE_TX_BUF_LEN (2 * DATA_LENGTH)              /*!< I2C slave tx buffer size */
#define I2C_SLAVE_RX_BUF_LEN (2 * DATA_LENGTH)              /*!< I2C slave rx buffer size */

#define ESP_SLAVE_ADDR CONFIG_I2C_SLAVE_ADDRESS /*!< ESP32 slave address, you can set any 7bit value */


struct ocirator_param_i2c param1,param2,param3,param4;


esp_err_t i2c_slave_init(void)
{
    int i2c_slave_port = I2C_SLAVE_NUM;
    i2c_config_t conf_slave;
    conf_slave.sda_io_num = I2C_SLAVE_SDA_IO;
    conf_slave.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf_slave.scl_io_num = I2C_SLAVE_SCL_IO;
    conf_slave.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf_slave.mode = I2C_MODE_SLAVE;
    conf_slave.slave.addr_10bit_en = 0;
    conf_slave.slave.slave_addr = ESP_SLAVE_ADDR;
    conf_slave.master.clk_speed = 400000;
    i2c_param_config(i2c_slave_port, &conf_slave);
    return i2c_driver_install(i2c_slave_port, conf_slave.mode, I2C_SLAVE_RX_BUF_LEN, I2C_SLAVE_TX_BUF_LEN, 0);
}


void make_algorism_param(struct ocirator_param_i2c i2c_param, struct algorism_param_4op* alg_param) {
    alg_param->amp = i2c_param.amp100 / 100;
    alg_param->helz_mul = i2c_param.mul;
    alg_param->helz = i2c_param.helz;
    alg_param->attack = i2c_param.attack;
    alg_param->decay = i2c_param.decay;
    alg_param->sustain_level = i2c_param.sus_level100 / 100;
    alg_param->release = i2c_param.release_level100 / 100;
    alg_param->muled_helz = alg_param->helz_mul * alg_param->helz ;
}


void i2c_slave_task(void *arg)
{
    struct algorism_param_4op alg_param1, alg_param2, alg_param3, alg_param4;
    struct note_param_i2c note_i2c;
    int i = 0;
    uint32_t task_idx = (uint32_t)arg;
    uint8_t *data = (uint8_t *)malloc(DATA_LENGTH);
    int cnt = 0;
    while (1) {
        i2c_slave_read_buffer(I2C_SLAVE_NUM, data, RW_LENGTH, portMAX_DELAY);
        switch(data[0]) {
            case command_param1:
                memcpy(&param1, &data[1], sizeof(struct ocirator_param_i2c));
                printf("%d,", param1.amp100);
                printf("%d,", param1.mul);
                printf("%d,", param1.helz);
                printf("%lld,", param1.attack);
                printf("%lld,", param1.decay);
                printf("%d,", param1.sus_level100);
                printf("%d,", param1.release_level100);
                break;

            case command_param2:
                memcpy(&param2, &data[1], sizeof(struct ocirator_param_i2c));
                break;

            case command_param3:
                memcpy(&param3, &data[1], sizeof(struct ocirator_param_i2c));
                break;

            case command_param4:
                memcpy(&param4, &data[1], sizeof(struct ocirator_param_i2c));
                break;

            case command_note_on:
            case command_note_off:
                memcpy(&note_i2c, &data[1], sizeof(struct note_param_i2c));
                make_algorism_param(param1, &alg_param1);
                make_algorism_param(param2, &alg_param2);
                make_algorism_param(param3, &alg_param3);
                make_algorism_param(param4, &alg_param4);

                note_4algorism2(note_i2c.algorism, note_i2c.on_off, note_i2c.tempo, note_i2c.note_type, &alg_param1, &alg_param2, &alg_param3, &alg_param4);
                break;
        }
    }
}
