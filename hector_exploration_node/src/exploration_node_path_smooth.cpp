//=================================================================================================
// Copyright (c) 2012, Stefan Kohlbrecher, TU Darmstadt
// All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the Simulation, Systems Optimization and Robotics
//       group, TU Darmstadt nor the names of its contributors may be used to
//       endorse or promote products derived from this software without
//       specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//=================================================================================================


#include <ros/ros.h>
#include <hector_exploration_planner/hector_exploration_planner.h>
#include <costmap_2d/costmap_2d_ros.h>
#include <hector_nav_msgs/GetRobotTrajectory.h>
#include <tf2_ros/buffer.h>
#include <tf2_ros/transform_listener.h>

class SimpleExplorationPlannerPathSmooth
{
public:
  SimpleExplorationPlannerPathSmooth() : listener_(buffer_)
  {
    buffer_.setUsingDedicatedThread(true);
    costmap_2d_ros_ = new costmap_2d::Costmap2DROS("global_costmap", buffer_);

    planner_ = new hector_exploration_planner::HectorExplorationPlanner();
    planner_->initialize("hector_exploration_planner",costmap_2d_ros_);

    exploration_plan_service_server_ = nh_.advertiseService("get_exploration_path", &SimpleExplorationPlannerPathSmooth::explorationServiceCallback, this);

    exploration_plan_pub_ = nh_.advertise<nav_msgs::Path>("exploration_path",2);
  }

  bool explorationServiceCallback(hector_nav_msgs::GetRobotTrajectory::Request  &req,
                                  hector_nav_msgs::GetRobotTrajectory::Response &res )
    {
      ROS_INFO("Exploration Service called");

      geometry_msgs::PoseStamped robot_pose;
      std::vector<geometry_msgs::PoseStamped> raw_plan;
      costmap_2d_ros_->getRobotPose(robot_pose);
      
      planner_->doExploration(robot_pose, raw_plan);
      planner_->planSmoother(raw_plan, res.trajectory.poses);
      res.trajectory.header.frame_id = "map";
      res.trajectory.header.stamp = ros::Time::now();
      if (exploration_plan_pub_.getNumSubscribers() > 0)
      {
        exploration_plan_pub_.publish(res.trajectory);
      }

      return true;
    }

protected:
  hector_exploration_planner::HectorExplorationPlanner* planner_;
  ros::NodeHandle nh_;
  ros::ServiceServer exploration_plan_service_server_;
  ros::Publisher exploration_plan_pub_;
  costmap_2d::Costmap2DROS* costmap_2d_ros_;
  tf2_ros::Buffer buffer_;
  tf2_ros::TransformListener listener_;
};

int main(int argc, char **argv) {
  ros::init(argc, argv, ROS_PACKAGE_NAME);

  SimpleExplorationPlannerPathSmooth ep;

  ros::spin();

  return 0;
}
