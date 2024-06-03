# line_tracer

주행 로봇의 카메라 영상을 vm 웨어로 publish하는 실행파일인 jpub와, 카메라 영상을 토픽으로 subscribe받아 에러값을 계산해 다시 주행 로봇으로 에러값을 토픽 publish 하는 vm 실행파일 그리고 vm웨어에서 publish한 에러값을 subscribe 해 다이나믹셀을 제어하는 실행파일인 vm을 하나의 패키지로 작성했다. 

라인 트레이싱 알고리즘은 작년에 만들어둔 https://github.com/cubejun/linetracer1/blob/main/main.cpp 를 사용했다. 

카메라 영상을 토픽 통신 할 때 Qos를 UDP 통신 모드인 .best_effort()로 설정해두었더니 프레임 드랍이 자주 발생했다. .best_effort() 를 지우고 TCP 통신 방식으로 바꾸어 프레임 드랍 문제를 해결했다.

카메라 영상을 주고 받는 인터페이스는 sensor_msgs::msg::CompressedImage를 사용했다. 

# 시뮬레이션 영상 
https://youtu.be/Uk-e7_1u2QE
# 실습 영상
https://youtu.be/32A8xbPnTBk
