//#include "opencv2/videoio.hpp"
//#include "opencv2/highgui.hpp"
//#include "opencv2/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <ctype.h>
#include <stdio.h>
#include <iostream>

using namespace cv;
using namespace std;

enum { GAUSSIAN, BLUR, MEDIAN };

int sigma = 3;
int smoothType = GAUSSIAN;

int main(int argc, char** argv)
{
	VideoCapture cap(1);
	if (cap.isOpened()) cout << ": width=" << cap.get(CAP_PROP_FRAME_WIDTH) << ", height=" << cap.get(CAP_PROP_FRAME_HEIGHT) << ", nframes=" << cap.get(CAP_PROP_FRAME_COUNT) << endl;
	if (!cap.isOpened())
	{
		cout << "Could not initialize capturing...\n";
		return -1;
	}

	namedWindow("Laplacian", 0);
	//createTrackbar("Sigma", "Laplacian", &sigma, 15, 0);
	//namedWindow("Webcam", 0);
	Mat smoothed, smoothed_gray, laplace, result, dx, dy;
	for (;;)
	{
		Mat frame;
		cap >> frame;
		if (frame.empty()) break;
		int ksize = 3;
		if (smoothType == GAUSSIAN) GaussianBlur(frame, smoothed, Size(ksize, ksize), 0, 0, BORDER_DEFAULT);
		else if (smoothType == BLUR) blur(frame, smoothed, Size(ksize, ksize));
		else medianBlur(frame, smoothed, ksize);
		cvtColor(smoothed, smoothed_gray, COLOR_BGR2GRAY);
		Laplacian(smoothed_gray, laplace, CV_16S, 5);
		convertScaleAbs(laplace, result);
		imshow("Laplacian", result);
		//imshow("Webcam", frame);
		Sobel(smoothed_gray, dx, CV_32F, 1, 0, 3);
		Sobel(smoothed_gray, dy, CV_32F, 0, 1, 3);
		magnitude(dx, dy, dx);
		cout << "Messa a Fuoco in percentuale: " << sum(dx)[0] / 100000 << endl;

		int c = waitKey(30);
		if (c == ' ')
			smoothType = smoothType == GAUSSIAN ? BLUR : smoothType == BLUR ? MEDIAN : GAUSSIAN;
		if (c == 'q' || c == 'Q' || (c & 255) == 27)
			break;
	}

	return 0;
}