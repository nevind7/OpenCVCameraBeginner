//#include <math.h>
//#include <string.h>
//#include <vector>
//#include <iostream>
//#include<opencv2\opencv.hpp>
//#include <opencv2\highgui\highgui.hpp>
//
//using namespace cv;
//using namespace std;
//
//
//#define window "LiveFeed"
//Mat frame;
//
//Mat source_image;
//
//Rect cropRect(0, 0, 0, 0);
//Point P1(0, 0);
//Point P2(0, 0);
//const char* winName = "Crop Image";
//bool clicked = false;
//int i = 0;
//char imgName[15];
//
//vector<Point> pts;
//
//void drawClickedPoints(Mat img) {
//	for (size_t i = 0; i < pts.size(); ++i) {
//		circle(img, pts[i], 4, Scalar(0, 0, 255), 2);
//		putText(frame, format("count%d", i), Point(pts[i].x + 5, pts[i].y + 2), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 0), 2);
//		Mat gray_image;
//		cvtColor(frame, gray_image, CV_BGRA2RGB);
//		imwrite("Images.jpg", gray_image);		
//	}
//}
//
////void cbMouse(int event, int x, int y, int flags, void* userdata) {
////	if (event == EVENT_LBUTTONDOWN) {
////		pts.push_back(Point(x, y));
////	}
////}
//
//
//Rect box;
//bool drawing_box = false;
//
//void draw_box(Mat * img, Rect rect)
//{
//	rectangle(*img, Point(box.x, box.y), Point(box.x + box.width, box.y + box.height), Scalar(0, 0, 255), 2);
//
//	Rect rect2 = Rect(box.x, box.y, box.width, box.height);
//
//}
//
//
//// Implement mouse callback
//
//void cbMouse(int event, int x, int y, int flags, void* param)
//{
//	Mat* frame = (Mat*)param;
//
//	switch (event)
//	{
//	case CV_EVENT_MOUSEMOVE:
//	{
//		if (drawing_box)
//		{
//			box.width = x - box.x;
//			box.height = y - box.y;
//		}
//	}
//	break;
//
//	case CV_EVENT_LBUTTONDOWN:
//	{   drawing_box = true;
//	box = Rect(x, y, 0, 0);
//	}
//	break;
//
//	case CV_EVENT_LBUTTONUP:
//	{   drawing_box = false;
//	if (box.width < 0)
//	{
//		box.x += box.width;
//		box.width *= -1;
//	}
//
//	if (box.height < 0)
//	{
//		box.y += box.height;
//		box.height *= -1;
//	}
//	draw_box(frame, box);
//	}
//	break;
//
//	default:
//		break;
//	}
//}
//int main()
//{
//	VideoCapture camera;
//	if (!camera.open(0))
//		return 0;
//
//	namedWindow(window);
//	setMouseCallback(window, cbMouse, &frame);
//
//	
//	box = Rect(0, 0, 1, 1);
//
//	
//	while (true)
//	{
//		camera.read(frame);
//		
//		if (drawing_box)
//			draw_box(&frame, box);
//
//		imshow(window, frame);
//
//		if (waitKey(15) == 27)
//			break;
//	}
//	//while (true) {
//	//	camera >> frame;
//	//	drawClickedPoints(frame);
//	//	imshow(window, frame);
//	//	if (waitKey(1) == 27)
//	//		break;
//	//}
//
//
//	for (auto &x : pts) {
//		cout << x.x << "  +  " << x.y << endl;
//	}
//	
//	return 0;
//}
//
////C:\\Users\\nevind7\\Desktop\\Media\\pics\\test2.jpg



#include "opencv2/opencv.hpp"
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;

Mat source_image;
Mat img;

// Current rectangle and start&end points
Rect rect(0, 0, 0, 0);
Point P1(0, 0);
Point P2(0, 0);

// Rectangles to be outputted
vector<Rect> rects;

static const char* WINDOW_NAME = "Tracking Info Generator";
static bool clicked = false;


void fixBoundaries() {
	if (rect.width > img.cols - rect.x)
		rect.width = img.cols - rect.x;

	if (rect.height > img.rows - rect.y)
		rect.height = img.rows - rect.y;

	if (rect.x < 0)
		rect.x = 0;

	if (rect.y < 0)
		rect.height = 0;
}

void draw() {
	img = source_image.clone();
	fixBoundaries();

	for (const auto& r : rects)
		rectangle(img, r, Scalar(0, 255, 0), 1, 8, 0);
	rectangle(img, rect, Scalar(0, 255, 0), 1, 8, 0);

	imshow(WINDOW_NAME, img);
}


void onMouse(int event, int x, int y, int f, void*) {
	switch (event) {
	case CV_EVENT_LBUTTONDOWN:
		clicked = true;
		P1.x = x;
		P1.y = y;
		P2.x = x;
		P2.y = y;
		break;

	case  CV_EVENT_LBUTTONUP:
		clicked = false;
		P2.x = x;
		P2.y = y;
		break;

	case CV_EVENT_MOUSEMOVE:
		if (clicked) {
			P2.x = x;
			P2.y = y;
		}
		break;

	default:
		break;
	}

	if (P1.x > P2.x) {
		rect.x = P2.x;
		rect.width = P1.x - P2.x;
	}
	else {
		rect.x = P1.x;
		rect.width = P2.x - P1.x;
	}

	if (P1.y > P2.y) {
		rect.y = P2.y;
		rect.height = P1.y - P2.y;
	}
	else {
		rect.y = P1.y;
		rect.height = P2.y - P1.y;
	}

	draw();
}

int main()
{
	//source_image = imread("../assets/face/faces.png", 1);
		VideoCapture camera;
		if (!camera.open(0))
			return 0;

	namedWindow(WINDOW_NAME, WINDOW_NORMAL);
	setMouseCallback(WINDOW_NAME, onMouse, NULL);
	

	while (1) {
		

		camera.read(source_image);
		char c = waitKey();
		imshow(WINDOW_NAME, source_image);
		switch (c) {
		case 's':
			if (rects.empty()) {
				cerr << "No rect added." << endl
					<< "Select an area and press 'a' to add!" << endl;
				continue;
			}
			for (const auto& r : rects) {
				cout << r.x + r.width / 2 << " "
					<< r.y + r.height / 2 << endl;
			}
			break;
		case 'a':
			rects.push_back(rect);
			rect = Rect(0, 0, 0, 0);
			break;
		}
	}

	return 0;
}

