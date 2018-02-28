#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <stdlib.h>
#include <thread>
#include <iomanip>
#include <Windows.h>
using namespace std;
using namespace cv;
double alpha;
int x, y, xx, yy, k = 0, in, c = 1, ii = 0, beta;
float fx = 1, fy = 1;
char res='n', r='n', ex='n', filename[100], curs='n', save[100];
Mat src, ext, frame, n_image;
void par_terminal();
void contrast();
void res_big();
void mouse_callback(int event, int x, int y, int flag, void *param);
int main(){
	VideoCapture cap(0);
	if (!cap.isOpened()) {
		cout << "\nImpossibile aprire la webcam!\n";
		system("PAUSE");
		Sleep(99999999);
	}
	cout << setfill('-') << setw(35) << "MICROSCOPIO 298.0" << setfill('-') << setw(20) << "\n";
	cout << "1)Cursor Normal Mode\n2)Cursor Vanish Mode\n3)Contrast Mode\n4)GrayScale Mode\nScegliere : ";
	cin >> in;
	thread t(par_terminal);
	while(true) {
		cap >> src;
		resize(src, src, Size(), fx, fy, INTER_CUBIC);
		ext = src;
		frame = ext;
		if (ii == 0) {
			ext = src;
			frame = ext;
		}
		namedWindow("camera", CV_WINDOW_AUTOSIZE);
		thread a(contrast);
		//namedWindow("camera", 1);
		/*if (in == 3) {
			for (int y = 0; y < frame.rows; y++) {
				for (int x = 0; x < frame.cols; x++) {
					for (int c = 0; c < 3; c++)
						frame.at<Vec3b>(y, x)[c] = saturate_cast<uchar>(alpha*(frame.at<Vec3b>(y, x)[c]) + beta);
				}
			}
		}*/
		if (in != 4) {
			line(frame, Point(0, frame.rows / (fx * 64)), Point(frame.cols, frame.rows / (fx * 64)), Scalar(0, 200, 0), 3);									//Linee Del Piano Cartesiano
			line(frame, Point(frame.cols - (frame.rows / (fx * 64)), 0), Point(frame.cols - (frame.rows / (fx * 64)), frame.cols), Scalar(0, 200, 0), 3);              //190px==0.28mm CIRCA!!! (68px==0.10mm)
			if (r == 'S' || r == 's') {
				for (int i = 100; i > 0; i--) {
					/*if (68 * i > frame.cols)
					break;*/
					line(frame, Point(68 * i, 0), Point(68 * i, frame.rows), Scalar(0, 200, 0), 1);
				}
				for (int i = 1; i < 100; i++) {
					if (68 * i > frame.rows)
						break;
					line(frame, Point(frame.cols, 68 * i), Point(0, 68 * i), Scalar(0, 200, 0), 1);
				}
			}
			else {
				for (int i = 100; i > 0; i--)
					line(frame, Point(68 * i, 0), Point(68 * i, 25), Scalar(0, 200, 0), 2);
				for (int i = 1; i < 100; i++) {
					if (68 * i > frame.rows)
						break;
					line(frame, Point(frame.cols, 68 * i), Point(frame.cols - 25, 68 * i), Scalar(0, 200, 0), 2);
				}
			}
			putText(frame, "0.10mm", Point((frame.cols - 90), 45), 1, 1, Scalar(0, 0, 0), 2);
		}
		else {
			cvtColor(frame, ext, CV_BGR2GRAY);
			Canny(ext, ext, 30, 60);
		}
		if (curs == 'S' || curs == 's')
			line(frame, Point(xx, yy), Point(x, y), Scalar(200, 0, 0), 2, CV_AA);
		line(frame, Point(xx, 0), Point(xx, frame.rows), Scalar(0, 0, 0), 1, CV_AA);
		line(frame, Point(0, yy), Point(frame.cols, yy), Scalar(0, 0, 0), 1, CV_AA);
		if (in == 2) {
			xx = x;
			yy = y;
		}
		//xx = x;         //per far sparire il cursore azzurro invece che lockarlo
		//yy = y;		  //per far sparire il cursore azzurro invece che lockarlo
		setMouseCallback("camera", mouse_callback);
		a.join();
		imshow("camera", ext);
		ii++;
		if (r == 'Q' || r == 'q')
			break;
		waitKey(30);
	}
	t.join();
	return 0;
}
void mouse_callback(int  event, int  x, int  y, int  flag, void *param){
	if (k % 2 == 0) {
		xx = x;
		yy = y;
	}
	if (event == EVENT_RBUTTONDOWN) {
		sprintf_s(filename, "E:/Microscopio/Scatto_%d.jpg", c); // selezione della cartella di salvataggio
		waitKey(10);
		//imshow("CAMERA 1", frame);        //per mostrarla in esecuzione
		if (in != 4)
			imwrite(filename, frame);
		else
			imwrite(filename, ext);
		cout << "Scatto_" << c << endl;
		c++;
	}
	if (event == EVENT_MOUSEWHEEL) {
		if (flag >= 1) {
			fx *= 1.03;
			fy *= 1.03;
			resize(frame, ext, Size(), fx, fy, INTER_CUBIC);
		}
		if (flag <= 0) {
			fx /= 1.03;
			fy /= 1.03;
			resize(frame, ext, Size(), fx, fy, INTER_CUBIC);
		}
	}
	if (event == EVENT_LBUTTONDOWN) {
		cout << "\n(" << x << ", " << y << ")";
		xx = x;
		yy = y;
		k++;
	}
}
void par_terminal() {
	if (in == 2) cout << "\nAvete scelto la seconda opzione, volete far mostrare il cursore secondario?(S/n) : ";
	if (in == 1) cout << "\nAvete scelto la prima opzione, volete far mostrare il cursore secondario?(S/n) : ";
	if (in == 3) cout << "\nAvete scelto la terza opzione, volete far mostrare il cursore secondario?(S/n) : ";
		cin >> curs;
	for (;;) {
		if (in != 4) cout << "\nMostrare la griglia?(S/n/Q[uit]) : ";
		cin >> r;
		if (in == 3) {
			cout << "Inserire alpha[1.0 - 3.0] : ";
			cin >> alpha;
			cout << "Inserire beta[0 - 100] : ";
			cin >> beta;
		}
	}
}
void contrast() {
	if (in == 3) {
		for (int y = 0; y < frame.rows; y++) {
			for (int x = 0; x < frame.cols; x++) {
				for (int c = 0; c < 3; c++)
					frame.at<Vec3b>(y, x)[c] = saturate_cast<uchar>(alpha*(frame.at<Vec3b>(y, x)[c]) + beta);
			}
		}
	}
}