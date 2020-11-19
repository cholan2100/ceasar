#ifndef ESP_PLATFORM
#include "std_msgs/Float32.h"
#include "std_msgs/Bool.h"
#include "std_msgs/String.h"
#include "std_msgs/Float32MultiArray.h"
#include "geometry_msgs/Vector3.h"
#else // ESP_PLATFORM
#include <memory>
#endif // ESP_PLATFORM

#include "spot_micro_motion_cmd.h"
#include "spot_micro_kinematics/spot_micro_kinematics.h"
#ifndef ESP_PLATFORM
#include "i2cpwm_board/Servo.h"
#include "i2cpwm_board/ServoArray.h"
#include "i2cpwm_board/ServoConfig.h"
#include "i2cpwm_board/ServosConfig.h"
#else // ESP_PLATFORM
// #include "i2cpwm_controller.h"
#endif // ESP_PLATFORM
#include "spot_micro_idle.h"

#include <esp_log.h>
static char tag[] = "spot_mmcmd";


using namespace smk;

// Constructor
SpotMicroMotionCmd::SpotMicroMotionCmd()
{
#if 1
  if (smnc_.debug_mode) {
    std::cout<<"from Constructor \n";
  }

  // Initialize Command 
  cmd_ = Command();

  // Initialize state to Idle state
  state_ = std::make_unique<SpotMicroIdleState>();

  // Read in config parameters into smnc_
  readInConfigParameters();


  // Initialize spot micro kinematics object of this class
  sm_ = smk::SpotMicroKinematics(0.0f, 0.0f, 0.0f, smnc_.smc);

  // Set an initial body height and stance cmd for idle mode
  body_state_cmd_.euler_angs = {.phi = 0.0f, .theta = 0.0f, .psi = 0.0f};
  body_state_cmd_.xyz_pos = {.x = 0.0f, .y = smnc_.lie_down_height, .z = 0.0f};
  body_state_cmd_.leg_feet_pos = getLieDownStance();

  // Set the spot micro kinematics object to this initial command
  sm_.setBodyState(body_state_cmd_);

  // Initialize servo array message with 12 servo objects
  for (int i = 1; i <= smnc_.num_servos; i++) {
    i2cpwm_board::Servo temp_servo;
    temp_servo.servo = i;
    temp_servo.value = 0;
    servo_array_[i] = temp_servo;
  }

  // Initialize servo array absolute message with 12 servo object with a value of
  // zero, just copy servo_array_msg since it's already correct 
  servo_array_absolute_ = servo_array_;
#endif // 0
}

// Destructor method
SpotMicroMotionCmd::~SpotMicroMotionCmd() {

  if (smnc_.debug_mode) {
    std::cout<<"from Destructor \n";
  }
  // Free up the memory assigned from heap
}

void SpotMicroMotionCmd::readInConfigParameters() {
    //TODO: Read in from configuration file instead
    //# Robot structure parameters
    smnc_.smc.hip_link_length = 0.0605;
    smnc_.smc.upper_leg_link_length = 0.11126;
    smnc_.smc.lower_leg_link_length = 0.1185; //TODO: include foot length
    smnc_.smc.body_width = 0.078;
    smnc_.smc.body_length = 0.2075;

    //#FIXME: Stance parameters, 
    smnc_.default_stand_height = 0.155;
    smnc_.stand_front_x_offset = 0.015;
    smnc_.stand_back_x_offset = 0.0 ;
    smnc_.lie_down_height = 0.083;
    smnc_.lie_down_feet_x_offset = 0.065;

    // Servo parameters
    smnc_.num_servos = 12;
    smnc_.servo_max_angle_deg = 82.5;

    //# Control Parameters
    smnc_.transit_tau = 0.3;
    smnc_.transit_rl = 0.06;
    smnc_.transit_angle_rl = 0.35;

    // # Node parameters
    smnc_.dt = 0.02; //# 50hz

    smnc_.debug_mode = false;
    smnc_.plot_mode = false; //FIXME

    // # Gait parameters
    smnc_.max_fwd_velocity = 0.4;
    smnc_.max_side_velocity = 0.4;
    smnc_.max_yaw_rate = 0.35;
    smnc_.z_clearance = 0.045;
    smnc_.alpha = 0.5;
    smnc_.beta = 0.5;
    smnc_.num_phases = 8; // 8 phse Gait
    smnc_.rb_contact_phases = {1, 0, 1, 1, 1, 1, 1, 1};
    smnc_.rf_contact_phases = {1, 1, 1, 0, 1, 1, 1, 1};
    smnc_.lf_contact_phases = {1, 1, 1, 1, 1, 1, 1, 0};
    smnc_.lb_contact_phases = {1, 1, 1, 1, 1, 0, 1, 1};
    smnc_.overlap_time = 0.0;
    smnc_.swing_time = 0.20;
    smnc_.foot_height_time_constant = 0.02;
    smnc_.body_shift_phases = {1, 2, 3, 4, 5, 6, 7, 8};
    smnc_.fwd_body_balance_shift = 0.025;
    smnc_.back_body_balance_shift = 0.005;
    smnc_.side_body_balance_shift = 0.015;

  // Derived parameters
  // Round result of division of floats
  smnc_.overlap_ticks = round(smnc_.overlap_time / smnc_.dt);
  smnc_.swing_ticks = round(smnc_.swing_time / smnc_.dt);
  smnc_.stance_ticks = 7 * smnc_.swing_ticks;
  smnc_.overlap_ticks = round(smnc_.overlap_time / smnc_.dt);
  smnc_.phase_ticks = std::vector<int>
      {smnc_.swing_ticks, smnc_.swing_ticks, smnc_.swing_ticks, smnc_.swing_ticks,
       smnc_.swing_ticks, smnc_.swing_ticks, smnc_.swing_ticks, smnc_.swing_ticks};
  smnc_.phase_length = smnc_.num_phases * smnc_.swing_ticks;

  // load servo config map
  smnc_.servo_config["RF_3"] = {{"num", 10}, {"center", 306}, {"range", 385}, {"direction", 1}, {"center_angle_deg", 84.0f}};
  smnc_.servo_config["RF_2"] = {{"num", 9}, {"center", 306}, {"range", 385}, {"direction", 1}, {"center_angle_deg", -27.9f}};
  smnc_.servo_config["RF_1"] = {{"num", 8}, {"center", 306}, {"range", 396}, {"direction", -1}, {"center_angle_deg", -5.4f}};
  smnc_.servo_config["RB_3"] = {{"num", 2}, {"center", 306}, {"range", 394}, {"direction", 1}, {"center_angle_deg", 90.4f}};
  smnc_.servo_config["RB_2"] = {{"num", 1}, {"center", 306}, {"range", 396}, {"direction", 1}, {"center_angle_deg", -35.2f}};
  smnc_.servo_config["RB_1"] = {{"num", 0}, {"center", 306}, {"range", 410}, {"direction", 1}, {"center_angle_deg", -4.4f}};
  smnc_.servo_config["LB_3"] = {{"num", 6}, {"center", 306}, {"range", 390}, {"direction", 1}, {"center_angle_deg", -84.1f}};
  smnc_.servo_config["LB_2"] = {{"num", 5}, {"center", 306}, {"range", 392}, {"direction", 1}, {"center_angle_deg", 37.4f}};
  smnc_.servo_config["LB_1"] = {{"num", 4}, {"center", 306}, {"range", 374}, {"direction", -1}, {"center_angle_deg", -0.4f}};
  smnc_.servo_config["LF_3"] = {{"num", 14}, {"center", 306}, {"range", 394}, {"direction", 1}, {"center_angle_deg", -80.0f}};
  smnc_.servo_config["LF_2"] = {{"num", 13}, {"center", 306}, {"range", 392}, {"direction", 1}, {"center_angle_deg", 37.9f}};
  smnc_.servo_config["LF_1"] = {{"num", 12}, {"center", 306}, {"range", 403}, {"direction", 1}, {"center_angle_deg", -9.0f}};
}

bool SpotMicroMotionCmd::publishServoConfiguration()
{
  // Create a temporary servo config
  i2cpwm_board::ServoConfig temp_servo_config;
  std::map<int, i2cpwm_board::ServoConfig> servo_config;

  // Loop through servo configuration dictionary in smnc_, append servo to
  for (std::map<std::string, std::map<std::string, float>>::iterator
       iter = smnc_.servo_config.begin();
       iter != smnc_.servo_config.end();
       ++iter) {

    std::map<std::string, float> servo_config_params = iter->second;
    temp_servo_config.center = servo_config_params["center"];
    temp_servo_config.range = servo_config_params["range"];
    temp_servo_config.servo = servo_config_params["num"];
    temp_servo_config.direction = servo_config_params["direction"];

    // Append to temp_servo_config_array
    servo_config[temp_servo_config.servo] = temp_servo_config;
  }
  i2cpwm_board::config(servo_config);
  return true;
}

SpotMicroNodeConfig SpotMicroMotionCmd::getNodeConfig() {
  return smnc_;
}

LegsFootPos SpotMicroMotionCmd::getNeutralStance() {
  float len = smnc_.smc.body_length; // body length
  float width = smnc_.smc.body_width; // body width
  float l1 = smnc_.smc.hip_link_length; // liength of the hip link
  float f_offset = smnc_.stand_front_x_offset; // x offset for front feet in neutral stance
  float b_offset = smnc_.stand_back_x_offset; // y offset for back feet in neutral stance

  LegsFootPos neutral_stance;
  neutral_stance.right_back  = {.x = -len/2 + b_offset, .y = 0.0f, .z =  width/2 + l1};
  neutral_stance.right_front = {.x =  len/2 + f_offset, .y = 0.0f, .z =  width/2 + l1};
  neutral_stance.left_front  = {.x =  len/2 + f_offset, .y = 0.0f, .z = -width/2 - l1};
  neutral_stance.left_back   = {.x = -len/2 + b_offset, .y = 0.0f, .z = -width/2 - l1};

  return neutral_stance;
}

LegsFootPos SpotMicroMotionCmd::getLieDownStance() {
  float len = smnc_.smc.body_length; // body length
  float width = smnc_.smc.body_width; // body width
  float l1 = smnc_.smc.hip_link_length; // length of the hip link
  float x_off = smnc_.lie_down_feet_x_offset;

  LegsFootPos lie_down_stance;
  lie_down_stance.right_back  = {.x = -len/2 + x_off, .y = 0.0f, .z =  width/2 + l1};
  lie_down_stance.right_front = {.x =  len/2 + x_off, .y = 0.0f, .z =  width/2 + l1};
  lie_down_stance.left_front  = {.x =  len/2 + x_off, .y = 0.0f, .z = -width/2 - l1};
  lie_down_stance.left_back   = {.x = -len/2 + x_off, .y = 0.0f, .z = -width/2 - l1};

  return lie_down_stance;
}

void SpotMicroMotionCmd::publishZeroServoAbsoluteCommand() {
  i2cpwm_board::servos_absolute(servo_array_absolute_);
}

void SpotMicroMotionCmd::handleInputCommands() {
  // Delegate input handling to state
  state_->handleInputCommands(sm_.getBodyState(), smnc_, cmd_, this, &body_state_cmd_);
}


void SpotMicroMotionCmd::changeState(std::unique_ptr<SpotMicroState> sms) {
  // Change the active state
  state_ = std::move(sms);

  // Call init method of new state
  state_->init(sm_.getBodyState(), smnc_, cmd_, this);

  // Reset all command values
  cmd_.resetAllCommands();
}

void SpotMicroMotionCmd::setServoCommandMessageData() {

  // Set the state of the spot micro kinematics object by setting the foot
  // positions, body position, and body orientation. Retrieve joint angles and
  // set the servo cmd message data
  sm_.setBodyState(body_state_cmd_);
  LegsJointAngles joint_angs = sm_.getLegsJointAngles();

  // Set the angles for the servo command message
  servo_cmds_rad_["RF_1"] = joint_angs.right_front.ang1;
  servo_cmds_rad_["RF_2"] = joint_angs.right_front.ang2;
  servo_cmds_rad_["RF_3"] = joint_angs.right_front.ang3;
 
  servo_cmds_rad_["RB_1"] = joint_angs.right_back.ang1;
  servo_cmds_rad_["RB_2"] = joint_angs.right_back.ang2;
  servo_cmds_rad_["RB_3"] = joint_angs.right_back.ang3;
 
  servo_cmds_rad_["LF_1"] = joint_angs.left_front.ang1;
  servo_cmds_rad_["LF_2"] = joint_angs.left_front.ang2;
  servo_cmds_rad_["LF_3"] = joint_angs.left_front.ang3;
 
  servo_cmds_rad_["LB_1"] = joint_angs.left_back.ang1;
  servo_cmds_rad_["LB_2"] = joint_angs.left_back.ang2;
  servo_cmds_rad_["LB_3"] = joint_angs.left_back.ang3;
}

void SpotMicroMotionCmd::publishServoProportionalCommand() {
  for (std::map<std::string, std::map<std::string, float>>::iterator
       iter = smnc_.servo_config.begin();
       iter != smnc_.servo_config.end();
       ++iter) {
 
    std::string servo_name = iter->first;
    std::map<std::string, float> servo_config_params = iter->second;
    
    int servo_num = servo_config_params["num"];
    float cmd_ang_rad = servo_cmds_rad_[servo_name];
    float center_ang_rad = servo_config_params["center_angle_deg"]*M_PI/180.0f;
    float servo_proportional_cmd = (cmd_ang_rad - center_ang_rad) /
                                   (smnc_.servo_max_angle_deg*M_PI/180.0f);
 
    if (servo_proportional_cmd > 1.0f) {
      servo_proportional_cmd = 1.0f;
      ESP_LOGW(tag, "Proportional Command above +1.0 was computed, clipped to 1.0");
      ESP_LOGW(tag, "Joint %s, Angle: %1.2f", servo_name.c_str(), cmd_ang_rad*180.0/M_PI);
 
    } else if (servo_proportional_cmd < -1.0f) {
      servo_proportional_cmd = -1.0f;
      ESP_LOGW(tag, "Proportional Command below -1.0 was computed, clipped to -1.0");
      ESP_LOGW(tag, "Joint %s, Angle: %1.2f", servo_name.c_str(), cmd_ang_rad*180.0/M_PI);
    }

    servo_array_[servo_num-1].servo = servo_num;
    servo_array_[servo_num-1].value = servo_proportional_cmd; 

    i2cpwm_board::servos_proportional (servo_array_);
 }
}

void SpotMicroMotionCmd::standCommandCallback() {
  cmd_.stand_cmd_ = true;
}

void SpotMicroMotionCmd::idleCommandCallback() {
  cmd_.idle_cmd_ = true;
}


void SpotMicroMotionCmd::walkCommandCallback() {
  cmd_.walk_cmd_ = true;
}

void SpotMicroMotionCmd::speedCommandCallback(float x, float y, float z) {
  cmd_.x_vel_cmd_mps_ = x;
  cmd_.y_vel_cmd_mps_ = y;
  cmd_.yaw_rate_cmd_rps_ = z;
}


void SpotMicroMotionCmd::angleCommandCallback(float x, float y, float z) {
  cmd_.phi_cmd_ = x;
  cmd_.theta_cmd_ = y;
  cmd_.psi_cmd_ = z;
}