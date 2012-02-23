#include "stdafx.h"
#include "Logo_surf.h"
#include "Logo_log.h"

namespace LogRec
{
	bool CLogoSURF_Wrapper::findKeyPoints(const char* imgFile)
	{
		cv::Mat img = cv::imread(std::string(imgFile));
		if( !img.data )
		{
			LogRec::LOG_LEVEL_ERROR;
			LogRec::PRINT_LOG_MESSAGE("---FAIL TO OPEN IMAGE :%s ",imgFile);
		}
		cv::Mat featureImg;
		std::vector<cv::KeyPoint> keypoints;
		
		/*
		cv::SiftFeatureDetector sift(400);
		sift.detect(img,keypoints);
		*/

		cv::SurfFeatureDetector surf(2500);
		surf.detect(img,keypoints);
		cv::drawKeypoints(img,keypoints,featureImg,cv::Scalar(255,255,255),cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
		cv::namedWindow("surf keypoints");
		cv::imshow("surf keypoints",featureImg);
		cv::waitKey();
		return true;
	}

	bool CLogoSURF_Wrapper::matchOfTwoImgs(const char* firstImg, const char* secondImg)
	{
		cv::Mat fImg = cv::imread(std::string(firstImg),CV_LOAD_IMAGE_ANYCOLOR);//CV_LOAD_IMAGE_ANYCOLOR
		cv::Mat sImg = cv::imread(std::string(secondImg),CV_LOAD_IMAGE_ANYCOLOR);//CV_LOAD_IMAGE_COLOR
		if( !fImg.data || !sImg.data)
		{
			LogRec::LOG_LEVEL_ERROR;
			LogRec::PRINT_LOG_MESSAGE("---FAIL TO OPEN IMAGE :%s AND %s ---",firstImg,secondImg);
		}

//#define	_SURF_H
#ifdef	_SURF_H
		cv::SurfFeatureDetector fDetector(2500),sDetector(2500);
		cv::SurfDescriptorExtractor fExtractor,sExtractor;
#else
		cv::SiftFeatureDetector fDetector,sDetector;
		cv::SiftDescriptorExtractor fExtractor,sExtractor;
#endif
		std::vector<cv::KeyPoint> sKeyPoints, fKeyPoints;
		fDetector.detect(fImg,fKeyPoints);
		sDetector.detect(sImg,sKeyPoints);

		cv::Mat fResultImg, sResultImg;

		
		fExtractor.compute(fImg,fKeyPoints,fResultImg);
		sExtractor.compute(sImg,sKeyPoints,sResultImg);
		
		cv::BruteForceMatcher<cv::L2<float>> matcher;
		std::vector<cv::DMatch> matches;
		matcher.match(fResultImg,sResultImg,matches);

		std::nth_element(matches.begin(),matches.begin()+25,matches.end());
		matches.erase(matches.begin() + 25, matches.end());
		cv::Mat imgMatches;
		cv::drawMatches(fImg,fKeyPoints,sImg,sKeyPoints,matches,imgMatches,cv::Scalar(255,255,255));

		cv::namedWindow("matcheResult");
		cv::imshow("matchResult",imgMatches);
		cv::waitKey();

		return true;
	}
}