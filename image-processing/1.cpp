#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

void countPixels (cv::Mat image) {
	std::cout << "Rows : " << image.rows << " Cols : " << image.cols << std::endl;
	for (int y = 0; y < image.rows; y++) {
		for (int x = 0; x < image.cols; x++) {
			if (x == y) {
				image.at<cv::Vec3b>(x, y)[0] = 0;
				image.at<cv::Vec3b>(x, y)[1] = 255;
				image.at<cv::Vec3b>(x, y)[2] = 0;
			}			// cv::Vec3b color = image.at<cv::Vec3b>(cv::Point(x, y));
		}
	}
}

void drawPrimitive (cv::Mat image) {
	cv::RotatedRect ellipseRect = cv::RotatedRect(cv::Point2f(50, 255), cv::Size2f(100, 50), 30);
	cv::Scalar ellipsColor = cv::Scalar(0, 0, 255);
	cv::ellipse(image, ellipseRect, ellipsColor, -1);
	cv::circle(image, cv::Point2f(480, 460), 10, cv::Scalar(10, 155, 155));
	cv::line(image, cv::Point2f(250, 250), cv::Point2f(120, 220), cv::Scalar(255, 255, 255));
	cv::rectangle(image, cv::Point2f(12.4, 76.4), cv::Point2f(66.6, 177.77), cv::Scalar(0, 0, 0), -1);
}

int main( int argc, char** argv ) {
    if( argc != 2) {
     std::cout <<" Usage: display_image ImageToLoadAndDisplay" << std::endl;
     return -1;
    }

    cv::Mat image;
    image = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);   // Read the file

    if(!image.data ) {
        std::cout << "Could not open or find the image" << std::endl;
    }

    countPixels(image);
    drawPrimitive(image);

    cv::namedWindow( "Display window", cv::WINDOW_AUTOSIZE );// Create a window for display.
    cv::imshow( "Display window", image );                   // Show our image inside it.

    cv::waitKey(0);                                          // Wait for a keystroke in the window
    return 0;
}