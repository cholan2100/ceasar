
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


static std::map<int, ServoConfig> servo_configs;
int config(std::map<int, ServoConfig> _servo_config)
{
    // int n = 0;
    // for (std::map<int, servocontrol::ServoConfig>::iterator
	// 		 iter = _servo_config.begin();
	// 	 iter != _servo_config.end();
	// 	 ++iter)
	// {
    //     ServoConfig s = iter->second;
    //     servo_channel[n] = _servo_config.;
    //     n++;
    // }
    // ESP_LOGI(tag, "Servo controller configured with %d servos", n);

//TODO: redundant configuration as motion cmd sends them all the time anyway
    servo_configs = _servo_config;
    ESP_LOGI(tag, "Servo controller configured");
    for (std::map<int, ServoConfig>::iterator
		    iter = servo_configs.begin();
		    iter != servo_configs.end();
		    ++iter)
	{
        int servo_name = iter->first;
		ServoConfig servo_config_params = iter->second;

		ESP_LOGI(tag, "Servo[%d]: channel: %d center: %d range: %d direction: %d", 
                        servo_name,
                        servo_config_params.servo-1,
                        servo_config_params.center,
                        servo_config_params.range,
                        servo_config_params.direction);
    }

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

/**
 * \private method to set a value for a PWM channel on the active board
 *
 *The pulse defined by start/stop will be active on the specified servo channel until any subsequent call changes it.
 *@param servo an int value (1..16) indicating which channel to change power
 *@param start an int value (0..4096) indicating when the pulse will go high sending power to each channel.
 *@param end an int value (0..4096) indicating when the pulse will go low stoping power to each channel.
 *Example _set_pwm_interval (3, 0, 350)    // set servo #3 (fourth position on the hardware board) with a pulse of 350
 */
static void _set_pwm_interval (int servo, int start, int end)
{
	// ESP_LOGI(tag, "servo[%d] = %d", servo, end);

    // if ((servo<1) || (servo>(MAX_SERVOS))) {
    //     ROS_ERROR("Invalid servo number %d :: servo numbers must be between 1 and %d", servo, MAX_BOARDS);
    //     return;
    // }
    setPWM(servo, start, end);
}

void servos_absolute (ServoArray& servos)
{
    // TODO: command PCA9685
    for(std::vector<Servo>::const_iterator sp = servos.begin(); sp != servos.end(); ++sp)
    {
        int servo = sp->servo;
        int value = sp->value;
        _set_pwm_interval (servo, 0, value);
    }
}

/**
 * @brief method to set a value for a PWM channel, based on a range of ±1.0, on the active board
 *
 *The pulse defined by start/stop will be active on the specified servo channel until any subsequent call changes it.
 *@param servo an int value (1..16) indicating which channel to change power
 *@param value an int value (±1.0) indicating when the size of the pulse for the channel.
 *Example _set_pwm_interval (3, 0, 350)    // set servo #3 (fourth position on the hardware board) with a pulse of 350
 */
static void _set_pwm_interval_proportional (int servo, float value)
{
	// need a little wiggle room to allow for accuracy of a floating point value
	if ((value < -1.0001) || (value > 1.0001)) {
		ESP_LOGE(tag, "Invalid proportion value %f :: proportion values must be between -1.0 and 1.0", value);
		return;
	}

	// ServoConfig* configp = &(servo_configs[servo-1]);
    ServoConfig* configp = &(servo_configs[servo]);
	
	if ((configp->center < 0) ||(configp->range < 0)) {
		ESP_LOGE(tag, "Missing servo configuration for servo[%d]", servo);
		return;
	}

	int pos = (configp->direction * (((float)(configp->range) / 2) * value)) + configp->center;
        
	if ((pos < 0) || (pos > 4096)) {
		ESP_LOGE(tag, "Invalid computed position servo[%d] = (direction(%d) * ((range(%d) / 2) * value(%6.4f))) + %d = %d", servo, configp->direction, configp->range, value, configp->center, pos);
		return;
	}
    _set_pwm_interval (servo-1, 0, pos);
	// ESP_LOGI(tag, "servo[%d] = (direction(%d) * ((range(%d) / 2) * value(%6.4f))) + %d = %d", servo, configp->direction, configp->range, value, configp->center, pos);
}

void servos_proportional(int servo, float value)
{
    _set_pwm_interval_proportional (servo, value);
}

}