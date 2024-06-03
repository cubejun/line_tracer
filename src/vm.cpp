#include "line_tracer/vm.hpp"
void Sub::mysub_callback(const sensor_msgs::msg::CompressedImage::SharedPtr msg)
{
    frame = cv::imdecode(cv::Mat(msg->data), cv::IMREAD_COLOR);
    //gettimeofday(&start, NULL);
	if (frame.empty()) { std::cerr << "frame empty!" << std::endl; return; }
	ROI = frame(cv::Rect(0, 270, 640, 90));
	cv::cvtColor(ROI, gray, cv::COLOR_BGR2GRAY);
	meangray = gray + (cv::Scalar(100) - mean(gray));
	cv::threshold(meangray, bin, 120, 255, cv::THRESH_BINARY);
	//라인 160
	//레인 불 켬 120, 130
	cnt = cv::connectedComponentsWithStats(bin, labels, stats, centroids);
	cv::cvtColor(bin, color, cv::COLOR_GRAY2BGR);
	centermin = abs(centerq.back() - centroids.at<double>(1, 0));
	centerymin = abs(centeryq.back() - centroids.at<double>(1, 1));
	for (int i = 1; i < cnt; i++) {
		p = stats.ptr<int>(i);
		c = centroids.ptr<double>(i);
		if (p[4] > 100) {
			cv::rectangle(color, cv::Rect(p[0], p[1], p[2], p[3]), cv::Scalar(255, 0, 0), 2);
			cv::circle(color, cv::Point(c[0], c[1]), 2, cv::Scalar(255, 0, 0), 2);
			if ((centermin >= abs(centerq.back() - c[0])) || (centerymin >= abs(centeryq.back() - c[1]))) {
				if ((abs(centerq.back() - c[0]) < 200) && (abs(centeryq.back() - c[1]) < 50)) {
					cntmin = i;
					centermin = abs(centerq.back() - c[0]);
					centerymin = abs(centeryq.back() - c[1]);
					// nearx = centroids.at<double>(cntmin, 0);
					// neary = centroids.at<double>(cntmin, 1);
					cx = stats.at<int>(cntmin, 0);
					cy = stats.at<int>(cntmin, 1);
					cw = stats.at<int>(cntmin, 2);
					ch = stats.at<int>(cntmin, 3);
				}
			}
		}
	}
	//1
	centerq.push(centroids.at<double>(cntmin, 0));
	centeryq.push(centroids.at<double>(cntmin, 1));
	if (abs(centerq.back() - centerq.front()) > 200) {
		centerq.push(centerq.front());
		centerq.pop();
		centerq.push(centerq.back());
		centerq.pop();
		//cx = 0; cy = 0; cw = 0; ch = 0;
	}
	if (abs(centeryq.back() - centeryq.front()) > 60) {
		centeryq.push(centeryq.front());
		centeryq.pop();
		centeryq.push(centeryq.back());
		centeryq.pop();
	}
	if(abs(err - (cameracentroidsx - centerq.back()))>100){
		centerq.push(centerq.front());
		centerq.pop();
		centerq.push(centerq.back());
		centerq.pop();
	}
	centerqbacksave = centerq.back();
	centerysave = centeryq.back();

	cv::circle(color, cv::Point(centerqbacksave, centerysave), 2, cv::Scalar(0, 0, 255), 2);
	cv::rectangle(color, cv::Rect(cx, cy, cw, ch), cv::Scalar(0, 0, 255), 2);
		
	err = cameracentroidsx - centerq.back();
	

	centerq.pop();
	centeryq.pop();

	writer1 << frame;
	writer2 << color;
    cv::imshow("frame", frame);
    cv::imshow("color", color);
    cv::waitKey(1);
    RCLCPP_INFO(this->get_logger(), "Received Image : %s,%d,%d", msg->format.c_str(),frame.rows,frame.cols);
}
void Sub::publish_msg()
{
    intmsg.data = err;
	RCLCPP_INFO(this->get_logger(), "Publish: %d", intmsg.data);
	pub_->publish(intmsg);
}
Sub::Sub() : Node("camsub_wsl")
{
    centerq.push(320);
    centeryq.push(45);
	writer1.open("output1.mp4", cv::VideoWriter::fourcc('m', 'p', '4', 'v'), 30, cv::Size(640, 360));
	writer2.open("output2.mp4", cv::VideoWriter::fourcc('m', 'p', '4', 'v'), 30, cv::Size(640, 90));
    auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10)).best_effort();
    sub_ = this->create_subscription<sensor_msgs::msg::CompressedImage>("image/compressed", qos_profile, std::bind(&Sub::mysub_callback, this, _1));
    pub_ = this->create_publisher<std_msgs::msg::Int32>("err", qos_profile);
    timer_ = this->create_wall_timer(50ms, std::bind(&Sub::publish_msg, this));
}
  
