
#include "servocontrol.h"
extern "C" {
#include "pca9685.h"
#include "config.h"
}

static char tag[] = "servo";

namespace servocontrol
{

#define I2C_MASTER_FREQ_HZ  100000     /*!< I2C master clock frequency */
#define I2C_MASTER_NUM      I2C_NUM_0   /*!< I2C port number for master dev */
#define I2C_MASTER_TX_BUF_DISABLE   0   /*!< I2C master do not need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0   /*!< I2C master do not need buffer */


#define ACK_CHECK_EN    0x1     /*!< I2C master will check ack from slave */
#define ACK_CHECK_DIS   0x0     /*!< I2C master will not check ack from slave */
#define ACK_VAL         0x0     /*!< I2C ack value */
#define NACK_VAL        0x1     /*!< I2C nack value */

/**
 * @brief i2c master initialization
 */
static void i2c_master_init(void)
{
    ESP_LOGD(tag, ">> I2C Init");
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_MASTER_SDA_IO;
    conf.scl_io_num = I2C_MASTER_SCL_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    
    int i2c_master_port = I2C_MASTER_NUM;
    ESP_ERROR_CHECK(i2c_param_config(i2c_master_port, &conf));
    ESP_ERROR_CHECK(i2c_driver_install(i2c_master_port, conf.mode,
                       I2C_MASTER_RX_BUF_DISABLE,
                       I2C_MASTER_TX_BUF_DISABLE, 0));
}

/**
 * @brief PCA9685 initialization
 */
static void init_pca9685() {
    i2c_master_init();
    set_pca9685_adress(PCA9685_I2C_ADDRESS);
    resetPCA9685();
    setFrequencyPCA9685(50);
}

/**
 * @brief Initialize the servo control board
 */
int init()
{
    init_pca9685();
    return 0;
}

int config(std::map<int, ServoConfig> servo_config)
{
    ESP_LOGI(tag, "Servo controller configured");
    //TODO: cache the config
    return 0;
}

#if 0
static esp_err_t set_servo(uint8_t id, uint16_t angle) {
  esp_err_t ret;
  uint16_t pulse = (uint16_t) (0.5 + servo_min[id] + (angle * servo_conversion[id]));
  ESP_LOGD(tag, "setPWM of servo %d, %d degrees -> Pulse %d", id, angle, pulse);
  ret = setPWM(pwm_channel[id], 0, pulse);

  if (ret == ESP_OK) return ESP_OK;
  else return ESP_FAIL;
}
#endif // 0

void servos_absolute (const ServoArray servos)
{
    //TODO: command PCA9685
}

void servos_proportional (const ServoArray servos)
{
    //TODO: command PCA9685
}
}