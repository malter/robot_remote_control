#pragma once

#include <string>

#include <robot_remote_control/Types/RobotRemoteControl.pb.h>
#include <geometry_msgs/PoseStamped.h>

#include "Point.hpp"
#include "Quaternion.hpp"

namespace robot_remote_control {
namespace RosConversion {

    inline static void convert(const robot_remote_control::Pose &from, geometry_msgs::Pose *to ) {
        convert(from.position(), &to->position);
        convert(from.orientation(), &to->orientation);
    }
    
    inline static void convert(const robot_remote_control::Pose &from, geometry_msgs::PoseStamped *to, const ros::Time &stamp = ros::Time::now(), const std::string &frame_id = "") {
        to->header.stamp = stamp;
        to->header.frame_id = frame_id;
        convert(from, &to->pose);
    }

    inline static void convert(const geometry_msgs::Pose &from, robot_remote_control::Pose *to ) {
        convert(from.position, to->mutable_position());
        convert(from.orientation, to->mutable_orientation());
    }
    
    inline static void convert(const geometry_msgs::PoseStamped &from, robot_remote_control::Pose *to ) {
        convert(from.pose, to);
    }
}  // namespace RosConversion
}  // namespace robot_remote_control
