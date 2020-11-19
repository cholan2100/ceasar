#include <nvs.h>
#include <nvs_flash.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <sys/time.h>

#include "comms.h"
#include "servocontrol.h"
#include "spot_micro_motion_cmd.h"

#include <esp_log.h>
static char tag[] = "ceasar";

void task_controller(void *ignore)
{
    ESP_LOGI(tag, "Locking to CPU: %d", xPortGetCoreID());

    // initialize controller state machine
    static SpotMicroMotionCmd motion;
    // bool debug_mode = motion.getNodeConfig().debug_mode; 
    if(!motion.init())
    {
        ESP_LOGE(tag, "Error initializing motion gait");
        vTaskDelete(NULL);
        return;
    }

    bool test_initiated = false;
    struct timeval start_time;
    gettimeofday(&start_time, NULL);

    // Controller loop
    while(1)
    {
        motion.runOnce();
        
        //TODO: establish the loop rate
        vTaskDelay(100 / portTICK_RATE_MS);

        // simulate commands for tests
        //TODO: use push button instead
        if(!test_initiated)
        {
            struct timeval current_time;
            gettimeofday(&current_time, NULL);
            if((current_time.tv_sec - start_time.tv_sec) >= 5)
            {
                motion.command_stand();
                test_initiated = true;
            }
        }

    }
    vTaskDelete(NULL);
}

extern "C" void app_main()
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );
    ESP_LOGI(tag, "Spot Micro ESP32 controller");

    //TODO: check fails and update NeoPixel status
    servocontrol::init();
    comms::init();

    xTaskCreate(task_controller, "task_controller", 1025 * 2, (void* ) 0, 10, NULL);
}