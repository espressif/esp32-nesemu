#include "sdkconfig.h"
#include "i2c_gpcontroller.h"

#ifdef CONFIG_HW_CONTROLLER_I2C_GP

#define I2C_GAMEPAD_SCL_IO          22               /*!< gpio number for I2C master clock */
#define I2C_GAMEPAD_SDA_IO          21               /*!< gpio number for I2C master data  */
#define I2C_GAMEPAD_NUM             I2C_NUM_0        /*!< I2C port number for master dev */
#define I2C_GAMEPAD_TX_BUF_DISABLE  0                /*!< I2C master do not need buffer */
#define I2C_GAMEPAD_RX_BUF_DISABLE  0                /*!< I2C master do not need buffer */
#define I2C_GAMEPAD_FREQ_HZ         50000            /*!< I2C master clock frequency */
#define I2C_GAMEPAD_ADDR            0x08

#define READ_BIT                           I2C_MASTER_READ  /*!< I2C master read */
#define ACK_CHECK_EN                       0x1              /*!< I2C master will check ack from slave*/
#define NACK_VAL                           0x1              /*!< I2C nack value */

void i2c_gpcontrollerInit()
{
    int i2c_master_port = I2C_GAMEPAD_NUM;
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_GAMEPAD_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = I2C_GAMEPAD_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_GAMEPAD_FREQ_HZ;
    i2c_param_config(i2c_master_port, &conf);
    i2c_driver_install(i2c_master_port, conf.mode,
                       I2C_GAMEPAD_RX_BUF_DISABLE,
                       I2C_GAMEPAD_TX_BUF_DISABLE, 0);
}

/**
 * @brief test code to write esp-i2c-slave
 *
 * 1. set mode
 * _________________________________________________________________
 * | start | slave_addr + wr_bit + ack | write 1 byte + ack  | stop |
 * --------|---------------------------|---------------------|------|
 * 2. wait more than 24 ms
 * 3. read data
 * ______________________________________________________________________________________
 * | start | slave_addr + rd_bit + ack | read 1 byte + ack  | read 1 byte + nack | stop |
 * --------|---------------------------|--------------------|--------------------|------|
 */
int i2c_gpReadInput()
{
    uint8_t data;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (I2C_GAMEPAD_ADDR << 1) | READ_BIT, ACK_CHECK_EN);
    i2c_master_read_byte(cmd, &data, NACK_VAL);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_GAMEPAD_NUM, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    if (ret == 0) {
        printf("I2C read %d, return %d\n", data, ret);

        return 0xFFFF ^ (
        (((0x01 & data) > 0) << 4)
        | (((0x02 & data) > 0) << 6)
        | (((0x04 & data) > 0) << 7)
        | (((0x08 & data) > 0) << 5)
        | (((0x10 & data) > 0) << 0)
        | (((0x20 & data) > 0) << 3)
        | (((0x40 & data) > 0) << 13)
        | (((0x80 & data) > 0) << 14)
      );
    } else {
        return 0xFFFF;
    }
}

#else

void i2c_gpcontrollerInit() {
	printf("I2C Gamepad disabled in menuconfig; no input enabled.\n");
}

int i2c_gpReadInput() {
	return 0xFFFF;
}

#endif
