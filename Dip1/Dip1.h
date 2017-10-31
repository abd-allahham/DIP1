//============================================================================
// Name        : Dip1.h
// Author      : Ronny Haensch
// Version     : 2.0
// Copyright   : -
// Description : header file for first DIP assignment
//============================================================================

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class Dip1{

	public:
		// constructor
		Dip1(void){};
		// destructor
		~Dip1(void){};
		
		// processing routine
		void run(string);
		// testing routine
		void test(string);

	private:
		// function that performs some kind of (simple) image processing
		// --> please edit ONLY these functions!
		Mat doSomethingThatMyTutorIsGonnaLike(Mat&);
		void transformImageToPolarForm(Mat& img, Mat& G, Mat& theta);
		void nonMaximumSuppression(Mat& img, Mat& theta);
		int roundOrientation(float o);
		void normalizeGradientImage(Mat& img);
		Mat minMaxThresholding(Mat& img, double min, double max);
		void edgesTrackingBetweenThresholds(Mat& gradiant, Mat& result, double min, double max, int minCahngedPixelsLimit = 50);

		// test function
		void test_doSomethingThatMyTutorIsGonnaLike(Mat&, Mat&);
};
