// OpenCVApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "common.h"
#include <opencv2/core/utils/logger.hpp>
#include <set>
#include <stdio.h>
#include <conio.h>
#include <math.h>
//THIS LINE I ADDED FOR TEST

using namespace std;

wchar_t* projectPath;

struct myStruct {
	string filePath;
	int label;
};

inline bool operator<(const myStruct& lhs, const myStruct& rhs)
{
	return lhs.filePath < rhs.filePath;
} 

void testOpenImage()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat src;
		src = imread(fname);
		imshow("image",src);
		waitKey();
	}
}

int getLabel(const string& fileName) {
	if (strstr(fileName.c_str(), "pepsi") != nullptr) {
		return 0;
	}
	else if (strstr(fileName.c_str(), "cola") != nullptr) {
		return 1;
	}
	return 0;
}

void processImages(FileGetter& fg, set<myStruct>& fileSet, const char* message) {
	char fname[MAX_PATH];

	printf("%s:\n", message);
	while (fg.getNextAbsFile(fname)) {
		Mat src = imread(fname);
		if (src.empty()) {
			printf("Could not open or find the image: %s\n", fname);
			continue;
		}
		int label = getLabel(fname);

		myStruct fileData = { fname, label };
		fileSet.insert(fileData);

		if (waitKey(1) == 27) 
			break;
	}

	for (set<myStruct>::iterator i = fileSet.begin(); i != fileSet.end(); i++) {
		printf("%s, label: %d\n", i->filePath.c_str(), i->label);
	}
}

void generateLabel(set<myStruct> testSet, set<myStruct> &newSet, const char* message) {
	for (set<myStruct>::iterator i = testSet.begin(); i != testSet.end(); i++) {
		int newLabel = rand() % 2;

		myStruct fileData = { i->filePath.c_str(), newLabel};
		newSet.insert(fileData);

		if (waitKey(1) == 27)
			break;
	}

	for (set<myStruct>::iterator i = newSet.begin(); i != newSet.end(); i++) {
		printf("%s, label: %d\n", i->filePath.c_str(), i->label);
	}
}

void computeAccuracy(set<myStruct>& set1, set<myStruct>& set2) {
	int ok = 0;
	int total = 0;
	float accuracy = 0;
	if (set1.size() == set2.size()) {
		total = set1.size();
	}
	for (set<myStruct>::iterator i = set1.begin(), j = set2.begin(); i != set1.end() && j != set2.end(); ++i, ++j) {
		if (i->label == j->label) {
			ok++;
		}
	}
	accuracy = (float) ok / total;
	printf("Ok value is %d.\n", ok);
	printf("Total value is %d.\n", total);
	printf("Accuracy computed: %f.\n", accuracy);
}

void testSets(const set<myStruct>& trainSet, const set<myStruct>& testSet) {
	if (trainSet.size() != 470 || testSet.size() != 115) {
		printf("The test has failed: train contains %d files, test contains %d files.\n", trainSet.size(), testSet.size());
	}
	else
		printf("The test has passed: train contains 470 files, test contains 115 files.\n");
}

void computeColor(const Mat& image, int &sumRed, int &sumBlue) {
	sumRed = 0;
	sumBlue = 0;

	for (int y = 0; y < image.rows; y++) {
		for (int x = 0; x < image.cols; x++) {
			Vec3b intensity = image.at<Vec3b>(y, x);
			if (intensity.val[0] == 255 && intensity.val[1] == 255 && intensity.val[2] == 255) {
				continue; 
			}
			sumBlue += intensity.val[0];
			sumRed += intensity.val[2];
		}
	}
}

int readColor(set<myStruct> &testSet, set<myStruct> &newSet) {
	for (set<myStruct>::iterator it = testSet.begin(); it != testSet.end(); it++) {
		string path_to_image = it->filePath;
		Mat image = imread(path_to_image);
		if (image.empty()) {
			printf("Image could not be loaded!\n");
			return -1;
		}

		int sumRed = 0, sumBlue = 0;
		computeColor(image, sumRed, sumBlue);
		int newLabel = 0;

		printf("%s - ", it->filePath.c_str());
		if (sumRed > sumBlue) {
			newLabel = 1;
			printf("Image contains more red.");
		}
		else if (sumRed < sumBlue) {
			newLabel = 0;
			printf("Image contains more blue.");
		}
		else
			printf("Image contains same amount of blue and red.");

		myStruct fileData = { it->filePath.c_str(), newLabel };
		newSet.insert(fileData);
		printf("Label:%d\n", newLabel);
	}
	return 0;
}

void computeAccuracyByColor(set<myStruct>&testSet, set<myStruct>&newSet) {
	int mat[2][2] = { {0, 0}, {0, 0} };

	for (const myStruct& real : testSet) {
		for (const myStruct& predicted : newSet) {
			if (real.filePath == predicted.filePath) {
				mat[real.label][predicted.label]++;
				break;
			}
		}
	}

	int ok = mat[0][0] + mat[1][1];
	int total = ok + mat[0][1] + mat[1][0];
	float accuracy = (float)ok / total;

	printf("Confusion Matrix:\n");
	printf("       |-------|-------|\n");
	printf("       | Pepsi | Cola  |\n");
	printf("|------|-------|-------|\n");
	printf("| Pepsi| %5d | %5d |\n", mat[0][0], mat[0][1]);
	printf("| Cola | %5d | %5d |\n", mat[1][0], mat[1][1]);
	printf("|------|-------|-------|\n");

	printf("\nAccuracy: %f\n", accuracy);
}


void MyCallBackFunc(int event, int x, int y, int flags, void* param)
{
	//More examples: http://opencvexamples.blogspot.com/2014/01/detect-mouse-clicks-and-moves-on-image.html
	Mat* src = (Mat*)param;
	if (event == EVENT_LBUTTONDOWN)
		{
			printf("Pos(x,y): %d,%d  Color(RGB): %d,%d,%d\n",
				x, y,
				(int)(*src).at<Vec3b>(y, x)[2],
				(int)(*src).at<Vec3b>(y, x)[1],
				(int)(*src).at<Vec3b>(y, x)[0]);
		}
}

void testMouseClick()
{
	Mat src;
	// Read image from file 
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		src = imread(fname);
		//Create a window
		namedWindow("My Window", 1);

		//set the callback function for any mouse event
		setMouseCallback("My Window", MyCallBackFunc, &src);

		//show the image
		imshow("My Window", src);

		// Wait until user press some key
		waitKey(0);
	}
}

/* Histogram display function - display a histogram using bars (simlilar to L3 / Image Processing)
Input:
name - destination (output) window name
hist - pointer to the vector containing the histogram values
hist_cols - no. of bins (elements) in the histogram = histogram image width
hist_height - height of the histogram image
Call example:
showHistogram ("MyHist", hist_dir, 255, 200);
*/
void showHistogram(const std::string& name, int* hist, const int  hist_cols, const int hist_height)
{
	Mat imgHist(hist_height, hist_cols, CV_8UC3, CV_RGB(255, 255, 255)); // constructs a white image

	//computes histogram maximum
	int max_hist = 0;
	for (int i = 0; i<hist_cols; i++)
	if (hist[i] > max_hist)
		max_hist = hist[i];
	double scale = 1.0;
	scale = (double)hist_height / max_hist;
	int baseline = hist_height - 1;

	for (int x = 0; x < hist_cols; x++) {
		Point p1 = Point(x, baseline);
		Point p2 = Point(x, baseline - cvRound(hist[x] * scale));
		line(imgHist, p1, p2, CV_RGB(255, 0, 255)); // histogram bins colored in magenta
	}

	imshow(name, imgHist);
}


int main() 
{
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_FATAL);
    projectPath = _wgetcwd(0, 0);

	srand(int(time(0)));

	set<myStruct> train;
	set<myStruct> test; 
	set<myStruct> newSetRandom;
	set<myStruct> newSetByColor;
	//set<int> label;

	char folderName[] = "D:/PI/archive/";
	char trainFolderPath[] = "D:/PI/archive/train_images";
	char testFolderPath[] = "D:/PI/archive/test_images";

	FileGetter fgTrain = { trainFolderPath, "jpg" };
	FileGetter fgTest = { testFolderPath, "jpg" };


	int op;
	do
	{
		system("cls");
		destroyAllWindows();
		printf("Menu:\n");
		printf(" 1 - Open image\n");
		printf(" 2 - Open JPG images from folder\n");
		printf(" 3 - Generate random labels \n");
		printf(" 4 - Compute random accuracy\n");
		printf(" 5 - Generate label by image color\n");
		printf(" 6 - Compute accuracy by color\n");
		printf(" 12 - Mouse callback demo\n");
		printf(" 13 - Test BATCH opening\n");
		printf(" 0 - Exit\n\n");
		printf("Option: ");
		scanf("%d",&op);
		switch (op)
		{
			case 1:
				testOpenImage();
				break;
			case 2:
				processImages(fgTrain, train, "Opening images from train_images folder");
				processImages(fgTest, test, "Opening images from test_images folder");
				printf("Press 'e' to exit.\n");
				while (true) {
					if (_kbhit()) {  
						char ch = _getch();  
						if (ch == 'e') {
							break;  
						}
					}
				}
				break;
			case 3: 
				generateLabel(test, newSetRandom, "Generating labels for test images\n");
				printf("Press 'e' to exit.\n");
				while (true) {
					if (_kbhit()) {
						char ch = _getch();
						if (ch == 'e') {
							break;
						}
					}
				}
				break;
			case 4: 
				computeAccuracy(test, newSetRandom);
				printf("Press 'e' to exit.\n");
				while (true) {
					if (_kbhit()) {
						char ch = _getch();
						if (ch == 'e') {
							break;
						}
					}
				}
				break;
			case 5: 
				readColor(test, newSetByColor);
				printf("Press 'e' to exit.\n");
				while (true) {
					if (_kbhit()) {
						char ch = _getch();
						if (ch == 'e') {
							break;
						}
					}
				}
				break;
			case 6:
				computeAccuracyByColor(test, newSetByColor);
				printf("Press 'e' to exit.\n");
				while (true) {
					if (_kbhit()) {
						char ch = _getch();
						if (ch == 'e') {
							break;
						}
					}
				}
				break;
			case 12:
				testMouseClick();
				break;
			case 13:
				testSets(train, test);
				printf("Press 'e' to exit.\n");
				while (true) {
					if (_kbhit()) {
						char ch = _getch();
						if (ch == 'e') {
							break;
						}
					}
				}
		}
	}
	while (op!=0);
	return 0;
}
