# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 14)

list(APPEND EXTRA_COMPONENT_DIRS "components/pca9685" "components/esp-dsp" "components/spot_micro_kinematics" )
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(ceasar)