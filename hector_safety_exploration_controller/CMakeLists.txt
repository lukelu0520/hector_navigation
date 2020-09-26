cmake_minimum_required(VERSION 2.8.3)
project(hector_safety_exploration_controller)
find_package(catkin REQUIRED COMPONENTS roscpp nodelet pluginlib std_msgs geometry_msgs kobuki_msgs yocs_controllers ecl_threads rospy hector_path_follower hector_nav_msgs)

catkin_package(
   INCLUDE_DIRS include
   LIBRARIES hector_safety_exploration_controller_nodelet
   CATKIN_DEPENDS roscpp nodelet pluginlib std_msgs geometry_msgs kobuki_msgs yocs_controllers ecl_threads rospy hector_path_follower hector_nav_msgs)

include_directories(include
                    ${catkin_INCLUDE_DIRS})

add_library(hector_safety_exploration_controller_nodelet src/nodelet.cpp)
add_dependencies(hector_safety_exploration_controller_nodelet ${catkin_EXPORTED_TARGETS})
target_link_libraries(hector_safety_exploration_controller_nodelet ${catkin_LIBRARIES})

install(TARGETS hector_safety_exploration_controller_nodelet
        DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
)
install(DIRECTORY include/${PROJECT_NAME}/
        DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
)
install(DIRECTORY plugins
        DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
install(DIRECTORY launch
        DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
