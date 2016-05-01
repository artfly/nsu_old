#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <cmath>

void showImage(cv::Mat image, std::string desc) {
	cv::namedWindow(desc, CV_WINDOW_AUTOSIZE);
	cv::imshow(desc, image);
	cv::waitKey(0);
}

void showHist(cv::Mat grayImage) {
	int histSize = 256;
	float range[] = {0, 256};
	const float * histRange = {range};
	bool uniform = true;
	bool accumulate = false;
	cv::Mat hist(512, 512, CV_8UC3, cv::Scalar(255, 255, 255));
	cv::calcHist(& grayImage, 1, 0, cv::Mat(), hist, 1, & histSize, & histRange, uniform, accumulate);

	int histW = 512;
	int histH = 512;
	int binW = cvRound( (double) histW/histSize );
	cv::Mat histImage(histH, histW, CV_8UC3, cv::Scalar(0, 0, 0));
	cv::normalize(hist, hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());

	for (int i = 1; i < histSize; i++) {
		cv::line(histImage, cv::Point(binW * (i - 1), histH - cvRound(hist.at<float>(i - 1)) ),
					cv::Point(binW * i, histH - cvRound(hist.at<float>(i))),
					cv::Scalar(255, 255, 255), 2, 8, 0);
	}
	showImage(histImage, "calcHist Gray");
}

void addNoiseAndBlur(cv::Mat grayImage) {
	cv::Mat gaussianNoise = grayImage.clone();
	cv::Mat copy = grayImage.clone();
	cv::Mat blurredImage;
	cv::randn(gaussianNoise, 128, 30);
	copy += gaussianNoise;
	cv::blur(copy, blurredImage, cv::Size(5, 5));

	showImage(copy, "gaussianNoise Test");
	showImage(blurredImage, "blur Test");
}

void calcDerivativesAndGradient(cv::Mat grayImage) {
	cv::Mat gradX, gradY;
	cv::Mat absGradX, absGradY;
	cv::Mat grad;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;

	cv::Sobel(grayImage, gradX, ddepth, 1, 0, 3, scale, delta, cv::BORDER_DEFAULT);
	cv::convertScaleAbs(gradX, absGradX);

	cv::Sobel(grayImage, gradY, ddepth, 1, 0, 3, scale, delta, cv::BORDER_DEFAULT);
	cv::convertScaleAbs(gradY, absGradY);
	cv::addWeighted(absGradX, 0.5, absGradY, 0.5, 0, grad);

	//calc orientation
	cv::Mat orientation = cv::Mat(gradY.rows, gradY.cols, CV_32F);
	for (int i = 0; i < gradX.rows; i++) {
		for (int j = 0; j < gradX.cols; j++) {
			float valueX = gradX.at<float>(i, j);
			float valueY = gradY.at<float>(i, j);
			float result = std::atan2(valueX, valueY);
			orientation.at<float>(i, j) = result; 
		}
	}

	showImage(orientation, "gradOrientation Test");
	showImage(grad, "gradMagnitude Test");
}

int main (int argc, char ** argv) {
	if( argc != 2) {
     std::cout <<" Usage: display_image ImageToLoadAndDisplay" << std::endl;
     return -1;
    }

    cv::Mat image, grayImage;
    image = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);   // Read the file

    if(!image.data) {
        std::cout << "Could not open or find the image" << std::endl;
    }
    cv::cvtColor(image, grayImage, CV_BGR2GRAY);

    showHist(grayImage);
    addNoiseAndBlur(grayImage);
    calcDerivativesAndGradient(grayImage);

    cv::namedWindow( "Display window", cv::WINDOW_AUTOSIZE);// Create a window for display.
    cv::imshow("Display window", grayImage);                   // Show our image inside it.

    cv::waitKey(0);                                          // Wait for a keystroke in the window
    return 0;
}