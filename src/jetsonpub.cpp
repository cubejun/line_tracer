#include "line_tracer/jetsonpub.hpp"
Pub::Pub() : Node("campub")
{
    // auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10)).best_effort();
    auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10));
    pub_ = this->create_publisher<sensor_msgs::msg::CompressedImage>("image/compressed", qos_profile );
    timer_ = this->create_wall_timer(25ms, std::bind(&Pub::publish_msg, this));
    cap.open("/home/jetson/ros2_ws/src/line_tracer/src/8_lt_cw_100rpm_in.mp4");
    //cap.open(src, cv::CAP_GSTREAMER);
    // cv::VideoCapture cap(src, cv::CAP_GSTREAMER);
    if (!cap.isOpened()) {
        RCLCPP_ERROR(this->get_logger(), "Could not open video!");
        rclcpp::shutdown();
        return;
    }
}
void Pub::publish_msg()
{
    static int i=0;
    cap >> frame;
    if (frame.empty()) { RCLCPP_ERROR(this->get_logger(), "frame empty"); return;}
    msg = cv_bridge::CvImage(hdr, "bgr8", frame).toCompressedImageMsg();
    RCLCPP_INFO(this->get_logger(), "%d", i++);
    pub_->publish(*msg);
}
