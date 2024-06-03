#include "line_tracer/jetsonsub.hpp"
int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<Sub>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    node->dxl.close();
    return 0;
}