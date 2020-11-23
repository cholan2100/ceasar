#include <nvs.h>
#include <nvs_flash.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <sys/time.h>

#include "config.h"
#include "comms.h"
#include "servocontrol.h"
#include "spot_micro_motion_cmd.h"

#include <esp_log.h>
static char tag[] = "ceasar";

void control_loop(void *ignore)
{
    ESP_LOGI(tag, "Locking to CPU: %d", xPortGetCoreID());

    // initialize controller state machine
    static SpotMicroMotionCmd motion;
    if (!motion.init())
    {
        ESP_LOGE(tag, "Error initializing motion gait");
        vTaskDelete(NULL);
        return;
    }

    //TODO: optimize with vTaskDelayUntil
    // task frequency to match GAIT update frequency
    float dt = motion.getNodeConfig().dt;
    TickType_t xDelay = pdMS_TO_TICKS((int)(dt * 1000.0f));
#ifdef SIMULATED_COMMANDS
    bool test_stand_initiated = false;
    bool test_walk_initiated = false;
    bool test_walkspeed_initiated = false;
    struct timeval start_time;
    gettimeofday(&start_time, NULL);
#endif // SIMULATED_COMMANDS

    // struct timeval prof_start;
    // struct timeval prof_end;

    // Controller loop
    while (1)
    {
        // ESP_LOGI(tag, "begin");
        motion.runOnce();
        // ESP_LOGI(tag, "----end");

        //TODO: establish the loop rate
        vTaskDelay(xDelay);

        // simulate commands for tests
#ifdef SIMULATED_COMMANDS
        // initiate stand
        if (!test_stand_initiated)
        {
            struct timeval current_time;
            gettimeofday(&current_time, NULL);
            if ((current_time.tv_sec - start_time.tv_sec) >= 1)
            {
                motion.command_stand();
                test_stand_initiated = true;
                gettimeofday(&start_time, NULL);
                ESP_LOGI(tag, " >>> STAND");
            }
        }

        // initiate walk
        if (test_stand_initiated & !test_walk_initiated)
        {
            struct timeval current_time;
            gettimeofday(&current_time, NULL);
            if ((current_time.tv_sec - start_time.tv_sec) >= 10)
            {
                motion.command_walk();
                test_walk_initiated = true;
                gettimeofday(&start_time, NULL);
                ESP_LOGI(tag, " >>> WALK");
            }
        }

        // add walk speed
        if (test_walk_initiated & !test_walkspeed_initiated)
        {
            struct timeval current_time;
            gettimeofday(&current_time, NULL);
            if ((current_time.tv_sec - start_time.tv_sec) >= 3)
            {
                float speed = 0.1f; //NOTE: this is max walking speed with MG996R, > risk of breaking robot
                motion.command_speed(speed, 0.0f, 0.0f); // forward speed
                test_walkspeed_initiated = true;
                gettimeofday(&start_time, NULL);
                ESP_LOGI(tag, " >>> Forware Speed: %f m/s", speed);
            }
        }
#endif // SIMULATED_COMMANDS
    }
    vTaskDelete(NULL);
}

extern "C" void app_main()
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    ESP_LOGI(tag, "Spot Micro ESP32 controller");

    //TODO: check fails and update NeoPixel status
    servocontrol::init();
    comms::init();

    xTaskCreate(control_loop, "controller", 1024 * 32, (void *)0, 10, NULL);
}