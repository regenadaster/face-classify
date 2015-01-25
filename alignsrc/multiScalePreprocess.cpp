#include "multiScalePreprocess.h"
/** enum with all landmarks (0-7)--> flandmarks and 8,9 added */
enum landmark_pos {
	FACE_CENTER = 0,
	LEFT_EYE_INNER = 1,
	RIGHT_EYE_INNER = 2,
	MOUTH_LEFT = 3,
	MOUTH_RIGHT = 4,
	LEFT_EYE_OUTER = 5,
	RIGHT_EYE_OUTER = 6,
	NOSE_CENTER = 7,
	LEFT_EYE_ALIGN = 8,
	RIGHT_EYE_ALIGN = 9
};

bool getMultiScaleImage(Mat aligned_image, vector<cv::Point2d> aligned_landmarks, vector<Mat> &patches)
{
	if (aligned_image.empty() || aligned_landmarks.empty())
	{
		return false;
	}
	Mat processedImage;
	Mat resizeImage;
	int a[] = {300};
	vector<int> scales (a, a + sizeof(a) / sizeof(int));
	processedImage = aligned_image;
	for (int s = 0; s < scales.size(); s++)
	{
		int currentScale = scales[s];
		vector<cv::Point2d> newPoints;
		for(int i = 0; i < aligned_landmarks.size()-3; i++)//aligned_landmarks.size()-2,because the last two landmarks are added to align picture
		{
			Point2d point;
			point.x = aligned_landmarks[i].x * double(currentScale) / aligned_image.cols;
			point.y = aligned_landmarks[i].y * double(currentScale) / aligned_image.rows;
			newPoints.push_back(point);
		}

		Point2d twoEyeCenter;
		twoEyeCenter.x = (aligned_landmarks[LEFT_EYE_INNER].x + aligned_landmarks[RIGHT_EYE_INNER].x) / 2;
		twoEyeCenter.y = (aligned_landmarks[LEFT_EYE_INNER].y + aligned_landmarks[RIGHT_EYE_INNER].y) / 2;
		newPoints.push_back(twoEyeCenter);

		resize(processedImage, resizeImage, Size(currentScale, currentScale));
		for(int i = 0; i<newPoints.size(); i++)
		{
			Mat patch;
			getRectSubPix(resizeImage, Size(currentScale/2, currentScale/2), newPoints[i], patch);
			patches.push_back(patch);
		}
	}
	return true;
}
