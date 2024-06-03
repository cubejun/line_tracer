#include "line_tracer/jetsonsub.hpp"
Sub::Sub() : Node("errsub")
{
    auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10)).best_effort();
    if(!dxl.open())
    {
        RCLCPP_ERROR(this->get_logger(), "dynamixel open error");
        rclcpp::shutdown();
        return;
    } 
    std::function<void(const std_msgs::msg::Int32::SharedPtr msg)> fn;
    fn = std::bind(&Sub::mysub_callback, this, dxl, _1);
    sub_ = this->create_subscription<std_msgs::msg::Int32>("err", qos_profile,fn);
}

void Sub::mysub_callback(Dxl& mdxl, const std_msgs::msg::Int32::SharedPtr intmsg)
{
    int err = intmsg->data;
    int lvel = 200 - gain * err;//왼쪽 바퀴 속도
	int rvel = -(200 + gain * err);//오른쪽 바퀴 속도
    RCLCPP_INFO(this->get_logger(), "Received message: %d %d", lvel, rvel);
    mdxl.setVelocity(lvel, rvel);
}
