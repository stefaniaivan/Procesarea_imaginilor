#include "stdafx.h"
#include "common.h"
#include <opencv2/core/utils/logger.hpp>
#include <set>
#include <stdio.h>
#include <conio.h>
#include <math.h>

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <cstdio> // For printf
#include <fstream> // For file operations
//put them in folder, threshhold binarization, documentation
using namespace std;

wchar_t* projectPath;


struct myStruct {
	string filePath;
	int label;
<<<<<<< HEAD
	
=======
	Vec3b RGB;
	Vec3b averageRGBperLabel;
>>>>>>> 9579e6fe4847aa2e7c9d502af94f27bc08d0469d
};


inline bool operator<(const myStruct& lhs, const myStruct& rhs)
{
	return lhs.filePath < rhs.filePath;
}

void testOpenImage()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src;
		src = imread(fname);
		imshow("image", src);
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

void generateLabel(set<myStruct> testSet, set<myStruct>& newSet, const char* message) {
	for (set<myStruct>::iterator i = testSet.begin(); i != testSet.end(); i++) {
		int newLabel = rand() % 2;

		myStruct fileData = { i->filePath.c_str(), newLabel };
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
	accuracy = (float)ok / total;
	printf("Ok value is %d.\n", ok);
	printf("Total value is %d.\n", total);
	printf("Accuracy computed: %f.\n", accuracy);
}
void printIncorrectImages(set<myStruct>& set1, set<myStruct>& set2) {
	vector<string> incorrectFilePaths;

	auto i = set1.begin();
	auto j = set2.begin();
	while (i != set1.end() && j != set2.end()) {
		if (i->label != j->label) {
			incorrectFilePaths.push_back(i->filePath);
		}
		++i;
		++j;
	}

	if (incorrectFilePaths.empty()) {
		cout << "All images are correctly labeled.\n";
	}
	else {
		cout << "Incorrectly labeled images (File Paths):\n";
		for (const string& filePath : incorrectFilePaths) {
			cout << "File Path: " << filePath << endl;
		}
	}
}

void moveIncorrectImages(set<myStruct>& set1, set<myStruct>& set2, const string& destinationFolder) {
	vector<string> incorrectFilePaths;

	auto i = set1.begin();
	auto j = set2.begin();
	while (i != set1.end() && j != set2.end()) {
		if (i->label != j->label) {
			incorrectFilePaths.push_back(i->filePath);
		}
		++i;
		++j;
	}

	if (incorrectFilePaths.empty()) {
		cout << "All images are correctly labeled.\n";
	}
	else {
		cout << "Incorrectly labeled images (File Paths):\n";
		for (const string& filePath : incorrectFilePaths) {
			cout << "File Path: " << filePath << endl;
		}

		// Move incorrect images to the destination folder
		for (const string& filePath : incorrectFilePaths) {
			ifstream sourceFile(filePath, ios::binary);
			ofstream destFile(destinationFolder + "/" + filePath.substr(filePath.find_last_of("\\/") + 1), ios::binary);

			destFile << sourceFile.rdbuf(); // Copy file content

			sourceFile.close();
			destFile.close();

			// Check if the move was successful
			if (sourceFile.fail() || destFile.fail()) {
				cout << "Failed to move file " << filePath << ".\n";
			}
			else {
				cout << "Moved " << filePath << " to " << destinationFolder << ".\n";
			}
		}
	}
}


void testSets(const set<myStruct>& trainSet, const set<myStruct>& testSet) {
	if (trainSet.size() != 470 || testSet.size() != 115) {
		printf("The test has failed: train contains %d files, test contains %d files.\n", trainSet.size(), testSet.size());
	}
	else
		printf("The test has passed: train contains 470 files, test contains 115 files.\n");
}

void computeColor(const Mat& image, int& sumRed, int& sumBlue) {
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

int readColor(set<myStruct>& testSet, set<myStruct>& newSet) {
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

void computeAccuracyByColor(set<myStruct>& testSet, set<myStruct>& newSet) {
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
	printf("           |----------------|----------------|\n");
	printf("           |   Actual       |   Actual       |\n");
	printf("|----------|----------------|----------------|\n");
	printf("| Predicted|     Pepsi      |     Cola       |\n");
	printf("|----------|----------------|----------------|\n");
	printf("| Pepsi    | %14d | %14d |\n", mat[0][0], mat[0][1]);
	printf("| Cola     | %14d | %14d |\n", mat[1][0], mat[1][1]);
	printf("|----------|----------------|----------------|\n");

	printf("\nAccuracy: %f\n", accuracy);
}
// Function to calculate average RGB values of an image
//RGB calculate_average_rgb(int** image, int width, int height) {
//	int total_pixels = width * height;
//	RGB average_rgb = { 0, 0, 0 };
//
//	for (int y = 0; y < height; y++) {
//		for (int x = 0; x < width; x++) {
//			average_rgb.r += image[y][x] >> 16 & 0xFF;
//			average_rgb.g += image[y][x] >> 8 & 0xFF;
//			average_rgb.b += image[y][x] & 0xFF;
//		}
//	}
//
//	average_rgb.r /= total_pixels;
//	average_rgb.g /= total_pixels;
//	average_rgb.b /= total_pixels;
//
//	return average_rgb;
//}

Vec3b compute_rgb_per_image(const Mat& image) {
	/*Vec3b average_rgb = (0, 0, 0);

	for (int y = 0; y < image.rows; y++) {
		for (int x = 0; x < image.cols; x++) {
			Vec3b intensity = image.at<Vec3b>(y, x);
			if (intensity.val[0] == 255 && intensity.val[1] == 255 && intensity.val[2] == 255) {
				continue;
			}
			average_rgb[0] += intensity.val[0];
			average_rgb[1] += intensity.val[1];
			average_rgb[2] += intensity.val[2];
		}
	}

	return average_rgb;*/

	//Mat floatImage;
	//image.convertTo(floatImage, CV_32FC3); 

	Scalar meanScalar = mean(image); 
	Vec3b average_rgb = Vec3b(meanScalar[0], meanScalar[1], meanScalar[2]); 

	return average_rgb;
	
}

pair<Vec3b, Vec3b> generateRGB(set<myStruct> trainSet, set<myStruct>& newSetRGB) {
	Vec3i sumRGBLabel0(0, 0, 0);
	Vec3i sumRGBLabel1(0, 0, 0);
	int countLabel0 = 0;
	int countLabel1 = 0;
	for (set<myStruct>::iterator i = trainSet.begin(); i != trainSet.end(); i++) {
		Mat image = imread(i->filePath);
		Vec3b RGB = compute_rgb_per_image(image);

		if (i->label == 0) {
			sumRGBLabel0 += RGB;
			countLabel0++;
		}
		else if (i->label == 1) {
			sumRGBLabel1 += RGB;
			countLabel1++;
		}

		myStruct fileData = { i->filePath.c_str(), i->label, RGB };
		newSetRGB.insert(fileData);

		if (waitKey(1) == 27)
			break;
	}

	Vec3b avgRGBLabel0(0, 0, 0);
	Vec3b avgRGBLabel1(0, 0, 0);

	if (countLabel0 > 0) {
		avgRGBLabel0 = Vec3b(sumRGBLabel0[0] / countLabel0, sumRGBLabel0[1] / countLabel0, sumRGBLabel0[2] / countLabel0);
	}

	if (countLabel1 > 0) {
		avgRGBLabel1 = Vec3b(sumRGBLabel1[0] / countLabel1, sumRGBLabel1[1] / countLabel1, sumRGBLabel1[2] / countLabel1);
	}

	printf("Average RGB for label 0: (%d, %d, %d)\n", avgRGBLabel0[0], avgRGBLabel0[1], avgRGBLabel0[2]);
	printf("Average RGB for label 1: (%d, %d, %d)\n", avgRGBLabel1[0], avgRGBLabel1[1], avgRGBLabel1[2]);


	for (set<myStruct>::iterator i = newSetRGB.begin(); i != newSetRGB.end(); i++) {
		myStruct& current = const_cast<myStruct&>(*i);
		if (current.label == 0) {
			current.averageRGBperLabel = avgRGBLabel0;
		}
		else if (current.label == 1) {
			current.averageRGBperLabel = avgRGBLabel1;
		}
		printf("%s, RGB: (%d, %d, %d), Average RGB by label: (%d, %d, %d)\n",i->filePath.c_str(), i->RGB[0], i->RGB[1], i->RGB[2], i->averageRGBperLabel[0], i->averageRGBperLabel[1], i->averageRGBperLabel[2]);
	}

	return make_pair(avgRGBLabel0, avgRGBLabel1);
}

// Function to calculate Euclidean distance between two RGB values
//float calculate_euclidean_distance(RGB rgb1, RGB rgb2) {
//	float distance = sqrt(pow(rgb1.r - rgb2.r, 2) + pow(rgb1.g - rgb2.g, 2) + pow(rgb1.b - rgb2.b, 2));
//	return distance;
//}

float calculate_euclidean_distance(const Vec3b& v1, const Vec3b& v2) {
	float distance = sqrt(pow(v1[2] - v2[2], 2) + pow(v1[1] - v2[1], 2) + pow(v1[0] - v2[0], 2));
	return distance;
}

// Function to classify a test image based on average RGB values of training images
//char* classify_image(RGB test_rgb, RGB* train_rgbs, char** train_labels, int train_size) {
//	float min_distance = INFINITY;
//	char* predicted_label = NULL;
//
//	for (int i = 0; i < train_size; i++) {
//		float distance = calculate_euclidean_distance(test_rgb, train_rgbs[i]);
//
//		if (distance < min_distance) {
//			min_distance = distance;
//			predicted_label = train_labels[i];
//		}
//	}
//
//	return predicted_label;
//}

void assignLabels(set<myStruct>& testSet, set<myStruct>& newTestSetRGB, Vec3b& avgRGBLabel0,Vec3b& avgRGBLabel1) {
	int predictedLabel = 0;
	for (set<myStruct>::iterator i = testSet.begin(); i != testSet.end(); i++) {
		Mat image = imread(i->filePath);
		Vec3b RGB = compute_rgb_per_image(image);
		double distToLabel0 = calculate_euclidean_distance(RGB, avgRGBLabel0);
		double distToLabel1 = calculate_euclidean_distance(RGB, avgRGBLabel1);

		/*printf("%d, %d, %d\n", avgRGBLabel0[0], avgRGBLabel0[1], avgRGBLabel0[2]);
		printf("%d, %d, %d\n", avgRGBLabel1[0], avgRGBLabel1[1], avgRGBLabel1[2]);*/

		if (distToLabel0 < distToLabel1) {
			predictedLabel = 0;
		}
		else {
			predictedLabel = 1;
		}

		myStruct fileData = { i->filePath.c_str(), predictedLabel};
		newTestSetRGB.insert(fileData);

	}

	for (set<myStruct>::iterator i = newTestSetRGB.begin(); i != newTestSetRGB.end(); i++) {
		printf("%s, label: %d\n", i->filePath.c_str(), i->label);
	}
}
void correctPredictions(set<myStruct>& testSet, set<myStruct>& newSet) {
	// Iterate through each image in the test set
	for (const myStruct& real : testSet) {
		// Find the corresponding image in the new set
		for (auto it = newSet.begin(); it != newSet.end(); ++it) {
			if (real.filePath == it->filePath) {
				// Load the image
				Mat image = imread(real.filePath, IMREAD_GRAYSCALE);
				if (image.empty()) {
					cerr << "Error: Unable to load image " << real.filePath << endl;
					continue;
				}

				// Apply threshold binarization
				Mat binaryImage;
				threshold(image, binaryImage, 128, 255, THRESH_BINARY);

				// Apply Canny edge detection
				Mat edges;
				Canny(binaryImage, edges, 100, 200);

				// Visual inspection and manual correction
				imshow("Edges", edges);
				char key = waitKey(0); // Wait for user input
				if (key == 'c') { // 'c' for correct, update label
					myStruct temp = *it; // Copy the element to a temporary variable
					temp.label = real.label; // Update the label of the temporary variable
					newSet.erase(it); // Remove the old element from the set
					newSet.insert(temp); // Insert the modified element into the set
				}

				break; // Move to the next image in the test set
			}
		}
	}


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
	for (int i = 0; i < hist_cols; i++)
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
	set<myStruct> newSetRGB;
	set<myStruct> newTestSetRGB;

	pair<Vec3b, Vec3b> result;
	Vec3b avgRGBLabel0, avgRGBLabel1;

	//set<int> label;

	/*char folderName[] = "C:/Users/shiri/Desktop/Anul3_Sem2/PI/archive/";
	char trainFolderPath[] = "C:/Users/shiri/Desktop/Anul3_Sem2/PI/archive/train_images";
	char testFolderPath[] = "C:/Users/shiri/Desktop/Anul3_Sem2/PI/archive/test_images";*/
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
		printf(" 5 - Compute rgb and euclidian \n");
		printf(" 6 - Generate label by image color\n");
		printf(" 7 - Compute accuracy by color\n");
<<<<<<< HEAD
		printf(" 8 - Show inaccurately calculated images\n");
		printf(" 9 - put incorrect images in folder\n");

=======
		printf(" 8 - Compute RGB per image for train set\n");
		printf(" 9 - Generate label using euclidian distance\n");
		printf(" 10 - Compute accuracy by euclidian distance\n");
>>>>>>> 9579e6fe4847aa2e7c9d502af94f27bc08d0469d
		printf(" 12 - Mouse callback demo\n");
		printf(" 13 - Test BATCH opening\n");
		printf(" 0 - Exit\n\n");
		printf("Option: ");
		scanf("%d", &op);
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
					
		case 6:
			readColor(test, newSetByColor); // Assuming test and newSetByColor are already defined
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
		case 7:
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
<<<<<<< HEAD
		case 8:
			computeAccuracy(test, newSetRandom);
			printIncorrectImages(test, newSetRandom);
			cout << "Press 'e' to exit.\n";
=======

		case 8: 
			result = generateRGB(train, newSetRGB);
			printf("Press 'e' to exit.\n");
>>>>>>> 9579e6fe4847aa2e7c9d502af94f27bc08d0469d
			while (true) {
				if (_kbhit()) {
					char ch = _getch();
					if (ch == 'e') {
						break;
					}
				}
			}
			break;
<<<<<<< HEAD
		case 9:
			moveIncorrectImages(test, newSetRandom, "C:/Users/shiri/Desktop/Anul3_Sem2/PI/archive/incorrect_predicted_images");
			//here put your file path
			//moveIncorrectImages(test, newSetRandom, "C:/Users/shiri/Desktop/Anul3_Sem2/PI/archive/incorrect_predicted_images");
			cout << "Press 'e' to exit.\n";
=======

		case 9:
			
			avgRGBLabel0 = result.first;
			avgRGBLabel1 = result.second;
			assignLabels(test, newTestSetRGB, avgRGBLabel0, avgRGBLabel1);
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

		case 10:
			computeAccuracyByColor(test, newTestSetRGB);
			printf("Press 'e' to exit.\n");
>>>>>>> 9579e6fe4847aa2e7c9d502af94f27bc08d0469d
			while (true) {
				if (_kbhit()) {
					char ch = _getch();
					if (ch == 'e') {
						break;
					}
				}
			}
			break;
<<<<<<< HEAD
			
=======
>>>>>>> 9579e6fe4847aa2e7c9d502af94f27bc08d0469d
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
	} while (op != 0);
	return 0;
}