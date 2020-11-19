idf_component_register(
    SRCS    main.cpp
            bt_spp.c
            comms.cpp
            i2cpwm_controller.cpp
            spot_micro_motion_cmd.cpp
            smfsm/spot_micro_idle.cpp
            smfsm/spot_micro_stand.cpp
            smfsm/spot_micro_walk.cpp
            smfsm/spot_micro_state.cpp
            smfsm/spot_micro_transition_idle.cpp
            smfsm/spot_micro_transition_stand.cpp
    INCLUDE_DIRS ""
            smfsm 
            rate_limited_first_order_filter
)
