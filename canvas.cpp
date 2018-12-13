/*
 * canvas.cpp
 *
 *  Created on: 12 déc. 2018
 *      Author: tux
 */

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/tracking.hpp>
#include "samples_utility.hpp"

using namespace std;
using namespace cv;

int main()
{
	Mat frame;

	MultiTracker trackers;
	String trackingAlg = "MEDIAN_FLOW";
	vector<Rect> ROIs;
	vector<Rect2d> objects;
	vector<Ptr<Tracker> > algorithms;

	string video = "hik2.mp4";
	VideoCapture cap(video);

	cap >> frame;

	selectROIs("tracker", frame, ROIs, 0, 0);

	for(size_t i=0; i<ROIs.size(); i++)
	{
		algorithms.push_back(createTrackerByName(trackingAlg));
		objects.push_back(ROIs[i]);
	}

	trackers.add(algorithms, frame, objects);

	while(true)
	{
		cap >> frame;
		Mat canvas(frame.rows + 150, frame.cols + 50, CV_8UC3, Scalar(127, 127, 127));

		Rect r(10, 10, frame.cols, frame.rows);
		frame.copyTo(canvas(r));

		trackers.update(frame);

		for(unsigned i=0; i<trackers.getObjects().size(); i++)
		{
			rectangle(canvas, trackers.getObjects()[i], Scalar::all(255), 2);
		}

		Size sz(100,100);
		Mat rois;
		vector<Rect2d> rects = trackers.getObjects();

		for(unsigned i=0; i<rects.size(); i++)
		{
			Mat patch;

			resize(frame(rects[i]), patch, sz);

			if (rois.empty())
			{
				rois = patch.clone();
			}
			else
			{
				hconcat(rois, patch, rois);
			}
		}

		Rect selection1(10, 10, 100, 100);
		Rect showSelection1(10, frame.rows + 20, 100, 100);
		frame(selection1).copyTo(canvas(showSelection1));

		imshow("Frame", frame);
		imshow("Canvas", canvas);
		imshow("tracks",rois);
		waitKey(3);
	}
	destroyAllWindows();
	return 0;
}


