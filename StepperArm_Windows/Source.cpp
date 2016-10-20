/**
* @file HoughCircle_Demo.cpp
* @brief Demo code for Hough Transform
* @author OpenCV team
*/
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace std;
using namespace cv;

namespace
{
	// windows and trackbars name
	const std::string windowName = "Hough Circle Detection Demo";
	const std::string cannyThresholdTrackbarName = "Canny threshold";
	const std::string accumulatorThresholdTrackbarName = "Accumulator Threshold";
	const std::string usage = "Usage : tutorial_HoughCircle_Demo <path_to_input_image>\n";

	// initial and max values of the parameters of interests.
	const int cannyThresholdInitialValue = 200;
	const int accumulatorThresholdInitialValue = 50;
	const int maxAccumulatorThreshold = 200;
	const int maxCannyThreshold = 255;

	void HoughDetection(const Mat& src_gray, const Mat& src_display, int cannyThreshold, int accumulatorThreshold)
	{
		// will hold the results of the detection
		std::vector<Vec3f> circles;
		// runs the actual detection
		HoughCircles(src_gray, circles, HOUGH_GRADIENT, 1, src_gray.rows / 8, cannyThreshold, accumulatorThreshold, 0, 0);

		// clone the colour, input image for displaying purposes
		Mat display = src_display.clone();
		for (size_t i = 0; i < circles.size(); i++)
		{
			Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			int radius = cvRound(circles[i][2]);
			// circle center
			circle(display, center, 3, Scalar(0, 255, 0), -1, 8, 0);
			// circle outline
			circle(display, center, radius, Scalar(0, 0, 255), 3, 8, 0);
		}

		// shows the results
		imshow(windowName, display);
	}
}


int main(int argc, char** argv)
{
	VideoCapture cap(1);
	if (!cap.isOpened()) {
		cap.release();
		cap.open(0);
		if (!cap.isOpened())
			return -1;
	}

	Mat src, src_gray;

	cap >> src;
	
	/*if (argc < 2)
	{
		std::cerr << "No input image specified\n";
		std::cout << usage;
		return -1;
	}

	// Read the image
	src = imread(argv[1], 1);

	if (src.empty())
	{
		std::cerr << "Invalid input image\n";
		std::cout << usage;
		return -1;
	}*/
	

	// Convert it to gray
	cvtColor(src, src_gray, COLOR_BGR2GRAY);

	// Reduce the noise so we avoid false circle detection
	GaussianBlur(src_gray, src_gray, Size(9, 9), 2, 2);

	//declare and initialize both parameters that are subjects to change
	int cannyThreshold = cannyThresholdInitialValue;
	int accumulatorThreshold = accumulatorThresholdInitialValue;

	// create the main window, and attach the trackbars
	namedWindow(windowName, WINDOW_AUTOSIZE);
	createTrackbar(cannyThresholdTrackbarName, windowName, &cannyThreshold, maxCannyThreshold);
	createTrackbar(accumulatorThresholdTrackbarName, windowName, &accumulatorThreshold, maxAccumulatorThreshold);

	// infinite loop to display
	// and refresh the content of the output image
	// until the user presses q or Q
	int key = 0;
	while (key != 'q' && key != 'Q')
	{
		cap >> src;
		cvtColor(src, src_gray, CV_BGR2GRAY);
		GaussianBlur(src_gray, src_gray, Size(9, 9), 2, 2);
		// those paramaters cannot be =0
		// so we must check here
		cannyThreshold = std::max(cannyThreshold, 1);
		accumulatorThreshold = std::max(accumulatorThreshold, 1);

		//runs the detection, and update the display
		HoughDetection(src_gray, src, cannyThreshold, accumulatorThreshold);

		// get user key
		key = waitKey(10);
	}

	return 0;
}

/*
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;


int main() {
	VideoCapture cap(0);
	if (!cap.isOpened())
		return -1;

	//WINDOWS
	namedWindow("Edges Image", WINDOW_AUTOSIZE);
	namedWindow("Original Image", WINDOW_AUTOSIZE);

	Mat bgrImage;
	Mat edges;

	int keyPress = -1;

	while (keyPress == -1) {
		Mat frame;
		cap >> frame;
		bgrImage = frame.clone();
	
		cvtColor(frame, edges, CV_BGR2GRAY);

		GaussianBlur(edges, edges, Size(9, 9), 1.5, 1.5);
		Canny(edges, edges, 0, 30, 3);

		vector<Vec3f> circles;

		HoughCircles(edges, circles, CV_HOUGH_GRADIENT, 1, edges.rows / 8, 200, 100);
		

		for (size_t i = 0; i < circles.size(); i++) {
			Point center(round(circles[i][0]), round(circles[i][1]));
			int radius = round(circles[i][2]);
			circle(bgrImage, center, radius, Scalar(0, 255, 0), 1, 4);
		}
		imshow("Original Image", bgrImage);
		imshow("Edges Image", edges);
		keyPress = waitKey(10);

	}
	return 0;
}

//Using color and shape - Does not work
/*int main() {
	VideoCapture cap(0);
	//cap.set....
	//cap.set(CAP_PROP_FPS, 20);
	//cap.set(CV_CAP_PROP_BUFFERSIZE, 3);
	if (!cap.isOpened())
		return -1;

	Mat bgr_image;

	//WINDOWS
	namedWindow("HSV Image", WINDOW_AUTOSIZE);
	namedWindow("Orignal Image", WINDOW_AUTOSIZE);

	int pressedKey = -1;

	while (pressedKey == -1) {
		cap >> bgr_image;

		medianBlur(bgr_image, bgr_image, 3);
		Mat newImage = bgr_image.clone();

		//GaussianBlur(newImage, newImage, Size(9, 9), 2, 2, 4);

		Point cent((newImage.cols)/2, (newImage.rows)/2);
		circle(newImage, cent, 30, Scalar(255, 0, 0), 2);

		Mat hsvImage;
		cvtColor(bgr_image, hsvImage, COLOR_BGR2HSV);

		Mat pennyHueRange;
		inRange(hsvImage, Scalar(20, 255, 255), Scalar(40, 255, 255), pennyHueRange);

		GaussianBlur(pennyHueRange, pennyHueRange, Size(9, 9), 2, 2, 4);

		vector<Vec3f> circles;
		HoughCircles(pennyHueRange, circles, CV_HOUGH_GRADIENT, 1, pennyHueRange.rows / 8, 100, 20, 0, 0);
		
		for (size_t currentCircle = 0; currentCircle < circles.size(); ++currentCircle) {
			Point center(round(circles[currentCircle][0]), round(circles[currentCircle][1]));
			int radius = round(circles[currentCircle][2]);
			circle(newImage, center, radius, Scalar(0, 255, 0), 5);
			//circle(pennyHueRange, center, radius, Scalar(0, 0, 255), 5);

		}
		imshow("pennyHueRange", pennyHueRange);

		imshow("HSV Image", hsvImage);
		imshow("Orignal Image", newImage);
		pressedKey = waitKey(5);
	}
	return 0;
}
*/
