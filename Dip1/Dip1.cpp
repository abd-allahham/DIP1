//============================================================================
// Name        : Dip1.cpp
// Author      : Ronny Haensch
// Version     : 2.0
// Copyright   : -
// Description : 
//============================================================================

#include "Dip1.h"
#include <stdio.h>

// function that performs some kind of (simple) image processing
/*
img	input image
return	output image
*/
Mat Dip1::doSomethingThatMyTutorIsGonnaLike(Mat& img){
  
	Mat Gx, Gy, G, theta;
	try
	{
		cvtColor(img, img, CV_BGR2GRAY);
		GaussianBlur(img, img, Size(5, 5), 1.4, 1.4);
		Mat xKernel = (Mat_<int>(3, 3) << -1, 0, 1, -2, 0, 2, -1, 0, 1);
		Mat yKernel = xKernel.t();
		Mat intImage;
		img.convertTo(intImage, CV_32F);
		filter2D(intImage, Gx, -1, xKernel);
		filter2D(intImage, Gy, -1, yKernel);

/*		Mat r,x,y;
		Gx.convertTo(x, CV_16SC1);
		Gy.convertTo(y, CV_16SC1);
		Canny(x, y,r, 100, 80);
		return r*/;
		//double min2, max2, overAllMax2;
		//minMaxLoc(Gx, &min2, &max2);
		//Mat ff = (Gx - min2) / (max2 - min2);
		//return ff;
		
		//Mat x, y;
		//Gx.convertTo(x, CV_32F);
		//Gy.convertTo(y, CV_32F);
		
		cartToPolar(Gx, Gy, G, theta, true);
		
		//double min1, max1;
		//minMaxLoc(G, &min1, &max1);
		
		//Mat G1 = (G - min1) / (max1-min1);
		//return G1;

		for (int i = 0; i < G.rows; i++) {
			for (int j = 0; j < G.cols; j++) {
				int orientation = Dip1::roundOrientation(theta.at<float>(i, j));
				int i1, i2, j1, j2;
				switch (orientation) {
				case 0 :
					j1 = j - 1;
					j2 = j + 1;
					i1 = i2 = i;
					break;
				case 45:
					i1 = i + 1;
					j1 = j + 1;
					i2 = i - 1;
					j2 = j - 1;
					break;
				case 90:
					i1 = i - 1;
					i2 = i + 1;
					j1 = j2 = j;
					break;
				default:
					i1 = i + 1;
					j1 = j - 1;
					i2 = i - 1;
					j2 = j + 1;
				}
				float p1, p2;
				if (i1 < 0 || i1 == G.rows || j1 < 0 || j1 == G.cols) {
					p1 = -1;
				}
				else {
					p1 = G.at<float>(i1, j1);
				}
				if (i2 < 0 || i2 == G.rows || j2 < 0 || j2 == G.cols) {
					p2 = -1;
				}
				else {
					p2 = G.at<float>(i2, j2);
				}
				float p = G.at<float>(i, j);
				if (p < p1 || p < p2) {
					G.at<float>(i, j) = 0;
				}
			}
		}

		//Mat G1 = (G - min1) / (max1 - min1);
		//return G1;

		double min, max;
		minMaxLoc(G, &min, &max);
		//overAllMax = max;
		Mat result = Mat::zeros(G.rows, G.cols, CV_8U);
		G = G / max;

		minMaxLoc(G, &min, &max);
		//return G;

		max = 0.25;
		min = 0.08;

		//max = 0.25;
		//min = 0.08;

		//max = 0.25;
		//min = 0.10;

		//min = 40;
		for (int i = 0; i < G.rows; i++) {
			for (int j = 0; j < G.cols; j++) {
				float p = G.at<float>(i, j);
				if (p < min) {
					G.at<float>(i, j) = 0;
				}
				else if (p > max) {
					result.at<uchar>(i, j) = 255;
					G.at<float>(i, j) = 1;
				}
			}
		}

		//Mat G1 = G / overAllMax;
		//return G1;

		int flippedPixels = 0;
		Mat tempG;
		G.copyTo(tempG);
		do {
			flippedPixels = 0;
			tempG.copyTo(G);
			for (int i = 0; i < G.rows; i++) {
				for (int j = 0; j < G.cols; j++) {
					if (G.at<float>(i, j) == 1 || G.at<float>(i, j) == 0) {
						continue;
					}
					int iMaskStart = i - 1;
					int iMaskend = i + 1;
					int jMaskStart = j - 1;
					int jMaskend = j + 1;
					if (iMaskStart < 0) {
						iMaskStart = 0;
					}
					else if (iMaskend == G.rows) {
						iMaskend = G.rows - 1;
					}
					if (jMaskStart < 0) {
						jMaskStart = 0;
					}
					else if (jMaskend == G.cols) {
						jMaskend = G.cols - 1;
					}

					for (int iMask = iMaskStart; iMask <= iMaskend; iMask++) {
						for (int jMask = jMaskStart; jMask <= jMaskend; jMask++) {
							if (G.at<float>(iMask, jMask) == 1) {
								result.at<uchar>(i, j) = 255;
								tempG.at<float>(i, j) = 1;
								iMask = iMaskend + 1;
								jMask = jMaskend + 1;
								flippedPixels += 1;
							}
						}
					}
				}
			}
		} while (flippedPixels > 50);

		//Mat G1 = G / overAllMax;
		//return G1;

		G = result;
	}
	catch (cv::Exception & e)
	{
		cerr << e.msg << endl; // output exception message
	}
	
	// TO DO !!!
	imwrite("../Lenna_result.jpg", G);
	return G;

}

int Dip1::roundOrientation(float o)
{	
	if (o > 135) {
		o -= 135;
	}

	if (o >= 0 && o <= 22.5) {
		return 0;
	}
	if (o > 22.5 && o <= 67.5) {
		return 45;
	}
	if (o > 67.5 && o <= 112.5) {
		return 90;
	}
	int f = 0;
	f += 1;
	return 135;
}
/* *****************************
  GIVEN FUNCTIONS
***************************** */

// function loads input image, calls processing function, and saves result
/*
fname	path to input image
*/
void Dip1::run(string fname){

	// window names
	string win1 = string ("Original image");
	string win2 = string ("Result");
  
	// some images
	Mat inputImage, outputImage;
  
	// load image
	cout << "load image" << endl;
	inputImage = imread( fname );
	cout << "done" << endl;
	
	// check if image can be loaded
	if (!inputImage.data){
	    cout << "ERROR: Cannot read file " << fname << endl;
	    cout << "Press enter to continue..." << endl;
	    cin.get();
	    exit(-1);
	}

	// show input image
	namedWindow( win1.c_str() );
	imshow( win1.c_str(), inputImage );
	
	// do something (reasonable!)
	outputImage = doSomethingThatMyTutorIsGonnaLike( inputImage );
	
	// show result
	namedWindow( win2.c_str() );
	imshow( win2.c_str(), outputImage );
	
	// save result
	imwrite("result.jpg", outputImage);
	
	// wait a bit
	waitKey(0);

}

// function loads input image and calls the processing functions
// output is tested on "correctness" 
/*
fname	path to input image
*/
void Dip1::test(string fname){

	// some image variables
	Mat inputImage, outputImage;
  
	// load image
	inputImage = imread( fname );

	// check if image can be loaded
	if (!inputImage.data){
	    cout << "ERROR: Cannot read file " << fname << endl;
	    cout << "Continue with pressing enter..." << endl;
	    cin.get();
	    exit(-1);
	}

	// create output
	outputImage = doSomethingThatMyTutorIsGonnaLike( inputImage );
	// test output
	test_doSomethingThatMyTutorIsGonnaLike(inputImage, outputImage);

}

// function loads input image and calls processing function

// output is tested on "correctness" 
/*
inputImage	input image as used by doSomethingThatMyTutorIsGonnaLike()
outputImage	output image as created by doSomethingThatMyTutorIsGonnaLike()
*/
void Dip1::test_doSomethingThatMyTutorIsGonnaLike(Mat& inputImage, Mat& outputImage){

	// ensure that input and output have equal number of channels
	if ( (inputImage.channels() == 3) && (outputImage.channels() == 1) )
		cvtColor(inputImage, inputImage, CV_BGR2GRAY);

	// split (multi-channel) image into planes
	vector<Mat> inputPlanes, outputPlanes;
	split( inputImage, inputPlanes );
	split( outputImage, outputPlanes );

	// number of planes (1=grayscale, 3=color)
	int numOfPlanes = inputPlanes.size();

	// calculate and compare image histograms for each plane
	Mat inputHist, outputHist;
	// number of bins
	int histSize = 100;
	float range[] = { 0, 256 } ;
	const float* histRange = { range };
	bool uniform = true; bool accumulate = false;
	double sim = 0;
	for(int p = 0; p < numOfPlanes; p++){
		// calculate histogram
		calcHist( &inputPlanes[p], 1, 0, Mat(), inputHist, 1, &histSize, &histRange, uniform, accumulate );
		calcHist( &outputPlanes[p], 1, 0, Mat(), outputHist, 1, &histSize, &histRange, uniform, accumulate );
		// normalize
		inputHist = inputHist / sum(inputHist).val[0];
		outputHist = outputHist / sum(outputHist).val[0];
		// similarity as histogram intersection
		sim += compareHist(inputHist, outputHist, CV_COMP_INTERSECT);
	}
	sim /= numOfPlanes;

	// check whether images are to similar after transformation
	if (sim >= 0.8)
		cout << "Warning: The input and output image seem to be quite similar (similarity = " << sim << " ). Are you sure your tutor is gonna like your work?" << endl;

}
