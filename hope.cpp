#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <X11/Xlib.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <time.h>

#define SPACE 32
#define ESC 27
#define W_WIDTH 500
#define W_HEIGHT 500

int help (std::string pr_name) {
	std::cout << pr_name << " FILE" << std::endl;
	std::cout << pr_name << " - get a snapshot" << std::endl;
	return 1;
}



int main (int argc, char **argv) {
	if (argc != 2) {
		return help (std::string(argv[0]));
	}
	char *imageName = argv[1];
	struct timespec start, end;
	
	cv::Mat image;
	cv::Mat hope = cv::imread ("hopeless.png");

	clock_gettime (CLOCK_MONOTONIC_RAW, &start);
	cv::VideoCapture capture(0);
	clock_gettime (CLOCK_MONOTONIC_RAW, &end);
	std::cout <<  "Time taken to get capture : " 
						<< end.tv_sec-start.tv_sec + 0.000000001* (end.tv_nsec-start.tv_nsec)
						<< " sec \n";
	
	cv::Mat hope_resize;
	cv::Mat result;
	
	Display* disp = XOpenDisplay (NULL);
	Screen *scrn = DefaultScreenOfDisplay (disp);

	if (!capture.isOpened()) {
		std::cout << "Coudn't open camera" << std::endl;
		return -1;
	}
	while (1) {
		cv::Mat frame;
		capture >> frame;
		cv::imshow ("Camera", frame);
		cv::resizeWindow ("Camera", W_WIDTH, W_HEIGHT);
		cv::moveWindow ("Camera", (scrn->width-W_WIDTH)/2, (scrn->height-W_HEIGHT)/2);
		char c = cv::waitKey (33);
		if (c == SPACE) {

			clock_gettime (CLOCK_MONOTONIC_RAW, &start);
			cv::cvtColor (frame, image, CV_BGR2GRAY);
			cv::imwrite (imageName, image);
			image  = cv::imread (imageName);
			cv::resize (hope, hope_resize, cv::Size2i (image.cols, image.rows));
			cv::addWeighted (image, 1, hope_resize, 0.5, 0.0, result);
			cv::imwrite (imageName, result);
			clock_gettime (CLOCK_MONOTONIC_RAW, &end);
			std::cout <<  "Time taken to transform image : " 
							<< end.tv_sec-start.tv_sec + 0.000000001* (end.tv_nsec-start.tv_nsec)
							<< " sec \n";

			clock_gettime (CLOCK_MONOTONIC_RAW, &start);
			cv::imshow ("Camera", result);
			clock_gettime (CLOCK_MONOTONIC_RAW, &end);
			std::cout <<  "Time taken to show image : " 
							<< end.tv_sec-start.tv_sec + 0.000000001* (end.tv_nsec-start.tv_nsec)
							<< " sec \n";

			cv::waitKey (2500);
		}	
		if (c == ESC) {
			break;
		}
	}

	return 0;
}

