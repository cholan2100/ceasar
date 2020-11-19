#include "ik_tests.h"
#include "spot_micro_kinematics/utils.h"
#include "spot_micro_kinematics/spot_micro_kinematics.h"
#include "math.h"

#include <esp_log.h>
static char tag[] = "IKTEST";

using namespace smk;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
void print_matrix(float * matrix, int n, int m, char* name) {
    printf("Matrix %s:\n", name);
    for (int i=0;i<n;i++)
    {
        for (int j=0;j<m;j++)
            printf("%.2f ", matrix[i*m + j]);
        printf("\n");
    }
}
#pragma GCC diagnostic pop

#define EXPECT_EQ(lhs,rhs) if(lhs != rhs) test_fail=1;//printf("NOT EQUAL\n");

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
static void testsmk_testSetJointAngles()
{
    float d2r = M_PI/180.0f; 

    // Create a spot micro kinematics object, set the joint angles, make sure they
    // are set for the legs
    SpotMicroConfig smc = {0.1, 0.2, 0.3, 0.4, 0.5};

    SpotMicroKinematics sm = SpotMicroKinematics(0.0f, 0.0f, 0.0f, smc);

    JointAngles desired = {10*d2r, 20*d2r, -15*d2r};

    LegsJointAngles four_legs_desired_angs = {desired, desired, desired, desired};

    sm.setLegJointAngles(four_legs_desired_angs);

    LegsJointAngles test_legs_joint_angs = sm.getLegsJointAngles();

    // Test equality
    int test_fail = 0;
    EXPECT_EQ(four_legs_desired_angs.right_back.ang1, test_legs_joint_angs.right_back.ang1); 
    EXPECT_EQ(four_legs_desired_angs.right_back.ang2, test_legs_joint_angs.right_back.ang2); 
    EXPECT_EQ(four_legs_desired_angs.right_back.ang3, test_legs_joint_angs.right_back.ang3); 

    EXPECT_EQ(four_legs_desired_angs.right_front.ang1, test_legs_joint_angs.right_front.ang1); 
    EXPECT_EQ(four_legs_desired_angs.right_front.ang2, test_legs_joint_angs.right_front.ang2); 
    EXPECT_EQ(four_legs_desired_angs.right_front.ang3, test_legs_joint_angs.right_front.ang3); 

    EXPECT_EQ(four_legs_desired_angs.left_front.ang1, test_legs_joint_angs.left_front.ang1); 
    EXPECT_EQ(four_legs_desired_angs.left_front.ang2, test_legs_joint_angs.left_front.ang2); 
    EXPECT_EQ(four_legs_desired_angs.left_front.ang3, test_legs_joint_angs.left_front.ang3); 

    EXPECT_EQ(four_legs_desired_angs.left_back.ang1, test_legs_joint_angs.left_back.ang1); 
    EXPECT_EQ(four_legs_desired_angs.left_back.ang2, test_legs_joint_angs.left_back.ang2); 
    EXPECT_EQ(four_legs_desired_angs.left_back.ang3, test_legs_joint_angs.left_back.ang3);
    if(test_fail)
    ESP_LOGE(tag, "testsmk_testSetJointAngles: Failed");
}
#pragma GCC diagnostic pop

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
static void testsmk_testReturnBodyState()
{
    float d2r = M_PI/180.0; 
    float truth_x = 0.1; float truth_y = 0.12; float truth_z = 0.13;
    float truth_phi = 0.1; float truth_theta = 0.05f; float truth_psi = -0.02f;

    // Create a spot micro kinematics object
    SpotMicroConfig smc = {0.1, 0.2, 0.3, 0.4, 0.5};

    SpotMicroKinematics sm = SpotMicroKinematics(truth_x, truth_y, truth_z, smc);

    JointAngles desired = {10*d2r, 20*d2r, -15*d2r};

    LegsJointAngles four_legs_desired_angs = {desired, desired, desired, desired};

    sm.setLegJointAngles(four_legs_desired_angs);

    sm.setBodyAngles(truth_phi, truth_theta, truth_psi);

    BodyState test_body_state = sm.getBodyState();

    // Test equality
    int test_fail = 0;
    EXPECT_EQ(truth_x, test_body_state.xyz_pos.x); 
    EXPECT_EQ(truth_y, test_body_state.xyz_pos.y); 
    EXPECT_EQ(truth_z, test_body_state.xyz_pos.z); 

    EXPECT_EQ(truth_phi, test_body_state.euler_angs.phi); 
    EXPECT_EQ(truth_theta, test_body_state.euler_angs.theta); 
    EXPECT_EQ(truth_psi, test_body_state.euler_angs.psi); 
    if(test_fail)
    ESP_LOGE(tag, "testsmk_testReturnBodyState: Failed");
}
#pragma GCC diagnostic pop

extern "C" int ik_tests()
{
    testsmk_testSetJointAngles();
    testsmk_testReturnBodyState();
    return 0;
}