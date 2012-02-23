// Logo1221.cpp : Defines the entry point for the console application.
//

// logo_regonition_sys.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "Logo_base.h"
#include "Logo_util.h"
#include "Logo_funcTest.h"
#include "Logo_multiDelaunay.h"
#include "Logo_common.h"
#include "Logo_core.h"
#include "Logo_log.h"
#include "Logo_surf.h"
#include <CppUnit\TestCase.h>
#include "Logo_unitTest.h"
#include <hash_map>
#include <hash_set>

using namespace LogRec;
//全家变量，用来做参数设置
LogRec::LogParams gLogoParams; 
int getVisualWordCode(std::vector<LogRec::LogoCodeBookFormat> &vCodeBook,float fdes[])
{
	float fmin = 0.0;
	int sig = 0;
	for (std::vector<LogRec::LogoCodeBookFormat>::iterator itr = vCodeBook.begin();
		itr != vCodeBook.end(); itr++)
	{
		float ft = LogRec::LogoEuclidean(fdes,itr->m_fDescriptor,128);
		if (fmin < ft)
		{
			fmin = ft;
			sig = itr->m_iSig;
		}
	}
	return sig;
}

struct SSig
{
	SSig()
	{
		fdis = 0;
		sig = 0;
	}
	float fdis;
	int sig;
};


//void multi_scale_denaulay(string path, string name,std::vector<LogRec::LogoCodeBookFormat> &vCodeBook,
//	std::vector<LogRec::LogoTriangle> &vLtri, LogRec::LogoRect &lrect)
//{
//	string fullNamePath = path+name;
//
//	//根据路径抽取图片特征点
//	const char* image_filename = fullNamePath.c_str();
//	CvMemStorage* storage_k = cvCreateMemStorage(0);
//
/////####################
/////使用bounding box 来限定训练区域，即在图片的某个指定矩形区域
//
//	IplImage* src = cvLoadImage( image_filename, CV_LOAD_IMAGE_GRAYSCALE );//CV_LOAD_IMAGE_GRAYSCALE
// 
//	if(!src)
//	{
//		fprintf( stderr, "Can not load %s \n",image_filename );
//		return;
//	}	
//
//	//src = cvLoadImage("C:\\Users\\beyondboy\\Desktop\\logo recognition\\flickr_logos_27_dataset.tar\\flickr_logos_27_dataset\\flickr_logos_27_dataset_images\\flickr_logos_27_dataset_images\\3294282629.jpg",1);
//	CvMat *dst;
//	dst = cvCreateMat(lrect.m_iRight-lrect.m_iLeft,lrect.m_iBottom-lrect.m_iTop,CV_8UC1);
//	CvRect a = cvRect(lrect.m_iLeft ,lrect.m_iTop ,lrect.m_iRight-lrect.m_iLeft,lrect.m_iBottom-lrect.m_iTop);
//	IplImage *image;
//
//	cvGetSubRect(src,dst,a);
//
//	image = cvCreateImage(cvSize(lrect.m_iRight-lrect.m_iLeft,lrect.m_iBottom-lrect.m_iTop),IPL_DEPTH_8U,1);
//	
//	cvGetImage(dst,image);
//
////######################
//
//	CvSeq *imageKeypoints = 0, *imageDescriptors = 0;
//
//	CvSURFParams params = cvSURFParams(500, 1);
//	cvExtractSURF( image, 0, &imageKeypoints, &imageDescriptors, storage_k, params );
//
//	int length = (int)(imageDescriptors->elem_size/sizeof(float));
//	
//	CvRect rect = {0,0,image->width,image->height};
//
//	cvReleaseMat(&dst);
//	cvReleaseImage(&src);
//	src = NULL;
//
//#if 0
//	//
//#endif
//	printf("Denaulay triangulation process will begin!!!\n");
//	float minF = 0;
//	float maxF = 0;
//	if (imageKeypoints->total > 0)
//	{
//		minF = (float)((CvSURFPoint*)cvGetSeqElem(imageKeypoints,0))->size;
//		maxF = (float)((CvSURFPoint*)cvGetSeqElem(imageKeypoints,0))->size;
//	}
//
//	std::vector<LogRec::LogoPoint> vlp;
//	std::vector<LogRec::LogoCodeBookFormat> vDecriptors;
//		//提取到矩阵里
//	CvSeqReader img_reader;
//	cvStartReadSeq( imageDescriptors, &img_reader );
//	float* p_imgPtr = (float*)malloc(length*sizeof(float));
//	for (int i = 0; i < imageKeypoints->total; i++)
//	{
//		const float* descriptor = (const float*)img_reader.ptr;
//		CV_NEXT_SEQ_ELEM( img_reader.seq->elem_size, img_reader );
//
//		//memcpy(p_imgPtr, descriptor, length*sizeof(float));
//		LogRec::LogoCodeBookFormat lbf;
//		memcpy(lbf.m_fDescriptor, descriptor, length*sizeof(float));
//		vDecriptors.push_back(lbf);
//		if (minF > ((CvSURFPoint*)cvGetSeqElem(imageKeypoints,i))->size)
//		{
//			minF = ((CvSURFPoint*)cvGetSeqElem(imageKeypoints,i))->size;
//		}
//		if (maxF < ((CvSURFPoint*)cvGetSeqElem(imageKeypoints,i))->size)
//		{
//			maxF = ((CvSURFPoint*)cvGetSeqElem(imageKeypoints,i))->size;
//		}
//		LogRec::LogoPoint lpp(cvRound(((CvSURFPoint*)cvGetSeqElem(imageKeypoints,i))->pt.x),cvRound(((CvSURFPoint*)cvGetSeqElem(imageKeypoints,i))->pt.y));
//		//lpp.m_uVisualWord = getVisualWordCode(vCodeBook, p_imgPtr);
//		if (i%50 == 0)
//		{
//			std::cout << "the " << i << " round" << std::endl;
//		}	
//
//		vlp.push_back(lpp);
//		//cvSubdivDelaunay2DInsert(subdiv,((CvSURFPoint*)cvGetSeqElem(imageKeypoints,i))->pt);
//	}
//	free(p_imgPtr);
//
//	getTotalVisualWordCode(vCodeBook,vDecriptors);
//	if (vlp.size() != vDecriptors.size())
//	{
//		cout << "TTTTTTTTTTTTT getcode error!" << endl;
//	}
//	else
//	{
//		for (int i = 0; i < vlp.size(); i++)
//		{
//			vlp[i].m_uVisualWord = vDecriptors[i].m_iSig;
//		}
//	}
//
//	float sigma = minF;
//	float w = 2.5;
//	float t = 2;
//
//	//#######################
//	while (sigma < maxF)
//	{
//
//		CvMemStorage* storage;
//		CvSubdiv2D* subdiv;
//
//		storage = cvCreateMemStorage(0);
//
//		subdiv = cvCreateSubdiv2D( CV_SEQ_KIND_SUBDIV2D, sizeof(*subdiv),
//								   sizeof(CvSubdiv2DPoint),
//								   sizeof(CvQuadEdge2D),
//								   storage );
//		//cvReleaseSubdiv2D();
//		cvInitSubdivDelaunay2D( subdiv, rect );
//		for (int i = 0; i < imageKeypoints->total; i++)
//		{
//			if ((float)((CvSURFPoint*)cvGetSeqElem(imageKeypoints,i))->size >= sigma 
//				 && (float)((CvSURFPoint*)cvGetSeqElem(imageKeypoints,i))->size < (sigma+w))
//			{
//				cvSubdivDelaunay2DInsert(subdiv,((CvSURFPoint*)cvGetSeqElem(imageKeypoints,i))->pt);
//			}
//		}
//		
//		logo_single_denaulay(rect,subdiv,vLtri,vlp, 10.0);
//		//######################
//
//		sigma += t;
//		cvReleaseMemStorage(&storage);
//	}
//	//cvReleaseImage(&image);
//	cvReleaseImageHeader(&image);
//	cvReleaseMemStorage(&storage_k);
//}

//void param_multi_scale_denaulay(string path, string name,std::vector<LogRec::LogoCodeBookFormat> &vCodeBook,
//	std::vector<LogRec::LogoTriangle> &vLtri, LogRec::LogoRect &lrect, float winSize, float step, float triArea)
//{
//	string fullNamePath = path+name;
//
//	//根据路径抽取图片特征点
//	const char* image_filename = fullNamePath.c_str();
//	CvMemStorage* storage_k = cvCreateMemStorage(0);
//
/////####################
/////使用bounding box 来限定训练区域，即在图片的某个指定矩形区域
//	IplImage* src = cvLoadImage( image_filename, CV_LOAD_IMAGE_GRAYSCALE );//CV_LOAD_IMAGE_GRAYSCALE
// 
//	if(!src)
//	{
//		fprintf( stderr, "Can not load %s \n",image_filename );
//		return;
//	}	
//
//	//src = cvLoadImage("C:\\Users\\beyondboy\\Desktop\\logo recognition\\flickr_logos_27_dataset.tar\\flickr_logos_27_dataset\\flickr_logos_27_dataset_images\\flickr_logos_27_dataset_images\\3294282629.jpg",1);
//	CvMat *dst;
//	dst = cvCreateMat(lrect.m_iRight-lrect.m_iLeft,lrect.m_iBottom-lrect.m_iTop,CV_8UC1);
//	CvRect a = cvRect(lrect.m_iLeft ,lrect.m_iTop ,lrect.m_iRight-lrect.m_iLeft,lrect.m_iBottom-lrect.m_iTop);
//	IplImage *image;
//
//	cvGetSubRect(src,dst,a);
//
//	image = cvCreateImage(cvSize(lrect.m_iRight-lrect.m_iLeft,lrect.m_iBottom-lrect.m_iTop),IPL_DEPTH_8U,1);
//	
//	cvGetImage(dst,image);
//
////######################
//
//	CvSeq *imageKeypoints = 0, *imageDescriptors = 0;
//
//	CvSURFParams params = cvSURFParams(500, 1);
//	cvExtractSURF( image, 0, &imageKeypoints, &imageDescriptors, storage_k, params );
//
//	int length = (int)(imageDescriptors->elem_size/sizeof(float));
//	
//	CvRect rect = {0,0,image->width,image->height};
//
//	cvReleaseMat(&dst);
//	cvReleaseImage(&src);
//	src = NULL;
//
//#if 0
//	//
//#endif
//	printf("Denaulay triangulation process will begin!!!\n");
//	////##########在特征值区间内进行多尺度三角剖分
//	float minF = 0;
//	float maxF = 0;
//	if (imageKeypoints->total > 0)
//	{
//		minF = (float)((CvSURFPoint*)cvGetSeqElem(imageKeypoints,0))->dir;
//		maxF = (float)((CvSURFPoint*)cvGetSeqElem(imageKeypoints,0))->dir;
//	}
//
//	std::vector<LogRec::LogoPoint> vlp;
//	std::vector<LogRec::LogoCodeBookFormat> vDecriptors;
//		//提取到矩阵里
//	CvSeqReader img_reader;
//	cvStartReadSeq( imageDescriptors, &img_reader );
//	float* p_imgPtr = (float*)malloc(length*sizeof(float));
//	for (int i = 0; i < imageKeypoints->total; i++)
//	{
//		const float* descriptor = (const float*)img_reader.ptr;
//		CV_NEXT_SEQ_ELEM( img_reader.seq->elem_size, img_reader );
//
//		//memcpy(p_imgPtr, descriptor, length*sizeof(float));
//		LogRec::LogoCodeBookFormat lbf;
//		memcpy(lbf.m_fDescriptor, descriptor, length*sizeof(float));
//		vDecriptors.push_back(lbf);
//		if (minF > ((CvSURFPoint*)cvGetSeqElem(imageKeypoints,i))->dir)
//		{
//			minF = ((CvSURFPoint*)cvGetSeqElem(imageKeypoints,i))->dir;
//		}
//		if (maxF < ((CvSURFPoint*)cvGetSeqElem(imageKeypoints,i))->dir)
//		{
//			maxF = ((CvSURFPoint*)cvGetSeqElem(imageKeypoints,i))->dir;
//		}
//		LogRec::LogoPoint lpp(cvRound(((CvSURFPoint*)cvGetSeqElem(imageKeypoints,i))->pt.x),cvRound(((CvSURFPoint*)cvGetSeqElem(imageKeypoints,i))->pt.y));
//		//lpp.m_uVisualWord = getVisualWordCode(vCodeBook, p_imgPtr);
//		if (i%50 == 0)
//		{
//			std::cout << "the " << i << " round" << std::endl;
//		}	
//
//		vlp.push_back(lpp);
//		//cvSubdivDelaunay2DInsert(subdiv,((CvSURFPoint*)cvGetSeqElem(imageKeypoints,i))->pt);
//	}
//	free(p_imgPtr);
//	//得到编码
//	getTotalVisualWordCode(vCodeBook,vDecriptors);
//	if (vlp.size() != vDecriptors.size())
//	{
//		cout << "TTTTTTTTTTTTT getcode error!" << endl;
//		return;
//	}
//	else
//	{
//		for (int i = 0; i < vlp.size(); i++)
//		{
//			vlp[i].m_uVisualWord = vDecriptors[i].m_iSig;
//		}
//	}
//
//
//	float sigma = minF;
//	float w = winSize;
//	float t = step;
//
//	//#######################
//	while (sigma < maxF)
//	{
//		//cout << "hwllo " << sigma << endl;
//		/*IplImage* img = cvCreateImage( cvSize(rect.width,rect.height), 8, 3 );
//		CvScalar bkgnd_color = CV_RGB(255,255,255);
//		cvSet( img, bkgnd_color, 0 )*/;
//		CvMemStorage* storage;
//		CvSubdiv2D* subdiv;
//
//		storage = cvCreateMemStorage(0);
//
//		subdiv = cvCreateSubdiv2D( CV_SEQ_KIND_SUBDIV2D, sizeof(*subdiv),
//								   sizeof(CvSubdiv2DPoint),
//								   sizeof(CvQuadEdge2D),
//								   storage );
//
//		cvInitSubdivDelaunay2D( subdiv, rect );
//		for (int i = 0; i < imageKeypoints->total; i++)
//		{
//			if ((float)((CvSURFPoint*)cvGetSeqElem(imageKeypoints,i))->dir >= sigma 
//				&& (float)((CvSURFPoint*)cvGetSeqElem(imageKeypoints,i))->dir < (sigma+w))
//			{
//				cvSubdivDelaunay2DInsert(subdiv,((CvSURFPoint*)cvGetSeqElem(imageKeypoints,i))->pt);
//			}
//		}
//		//单步三角剖分
//		logo_single_denaulay(rect,subdiv,vLtri,vlp,triArea);
//		//######################
//		cvReleaseMemStorage(&storage);
//		sigma += t;
//	}
//	//cvReleaseImage(&image);
//	cvReleaseMemStorage(&storage_k);
//	cvReleaseImageHeader(&image);
//}

template <typename T>
extern std::string LogoParamToString(T e)
{
	std::ostringstream os;
	os << e;
	return std::string(os.str());
}

//void singleQuery(std::vector<LogRec::LogoCodeBookFormat> &vCodeBook,std::map<std::string,std::set<long long> > mSig, 
//	std::string sPath, std::string sQ1, std::string sQ2,int w, int s, int a)
//{
//	std::vector<LogRec::LogoTriangle> vLtri1;
//	std::vector<LogRec::LogoTriangle> vLtri2;
//
//	std::vector<LogRec::LogoRanker> lrank1;
//	std::vector<LogRec::LogoRanker> lrank2;
//
//
//	IplImage* src = cvLoadImage( (sPath+sQ1).c_str(), CV_LOAD_IMAGE_GRAYSCALE );//CV_LOAD_IMAGE_GRAYSCALE
//	multi_scale_denaulay(sPath,sQ1,vCodeBook,vLtri1,LogRec::LogoRect(0,0,src->height,src->width));
//
//	src = cvLoadImage( (sPath+sQ2).c_str(), CV_LOAD_IMAGE_GRAYSCALE );//CV_LOAD_IMAGE_GRAYSCALE
//	multi_scale_denaulay(sPath,sQ2,vCodeBook,vLtri2,LogRec::LogoRect(0,0,src->height,src->width));
//
//	for (std::vector<LogRec::LogoTriangle>::iterator itr1 = vLtri1.begin(); 
//		itr1 != vLtri1.end(); itr1++)
//	{
//		itr1->Logo_ToSignature();
//	}
//	for (std::vector<LogRec::LogoTriangle>::iterator itr2 = vLtri2.begin(); 
//		itr2 != vLtri2.end(); itr2++)
//	{
//		itr2->Logo_ToSignature();	
//	}
//
//	std::string s2 = sQ1.substr(0,sQ1.find_first_of("."))+"1res"+LogoParamToString(w)+LogoParamToString(s)+LogoParamToString(a)+".txt";
//	ofstream ofres1(s2.c_str());
//	std::string s1 = sQ2.substr(0,sQ2.find_first_of("."))+LogoParamToString(w)+LogoParamToString(s)+LogoParamToString(a)+".txt";
//	ofstream ofres2(s1.c_str());
//
//	for (std::map<std::string,std::set<long long> >::iterator mitr = mSig.begin(); 
//		mitr != mSig.end(); mitr++)
//	{
//
//		LogRec::LogoRanker lr1;
//		LogRec::LogoRanker lr2;
//		lr1.m_sClassName = mitr->first;
//		lr2.m_sClassName = mitr->first;
//
//		lr1.m_iCount = 0;
//		lr2.m_iCount = 0;
//		//int ic1 = 0;	
//		for (std::vector<LogRec::LogoTriangle>::iterator itr1 = vLtri1.begin(); 
//			itr1 != vLtri1.end(); itr1++)
//		{
//			if ((mitr->second.find(itr1->m_llSignature)) != mitr->second.end())
//			{
//				lr1.m_iCount++;
//			}
//		}
//		for (std::vector<LogRec::LogoTriangle>::iterator itr2 = vLtri2.begin(); 
//			itr2 != vLtri2.end(); itr2++)
//		{
//			if ((mitr->second.find(itr2->m_llSignature)) != mitr->second.end())
//			{
//				lr2.m_iCount++;
//			}			
//		}
//
//		ofres1  << "clasName: " << lr1.m_sClassName << "  count:" << lr1.m_iCount << std::endl;
//		ofres2  << "clasName: " << lr2.m_sClassName << "  count:" << lr2.m_iCount << std::endl;
//
//		lrank1.push_back(lr1);
//		lrank2.push_back(lr2);
//	}
//}
//void logoParamRecProcess(string sPath,string vabFile)
//{
//
//	//总的索引和模型
//	std::map<std::string,std::set<long long> > mSig;
//	//统计排序的结构
//	std::vector<LogRec::LogoRanker> lrank;
//	
//	//读取码书
//	std::vector<LogRec::LogoCodeBookFormat> vCodeBook;
//	LogRec::LogoLoadCodeBook(sPath, vabFile, vCodeBook);
//
//	//std::string sPath = "C:\\Users\\beyondboy\\Desktop\\logo recognition\\flickr_logos_27_dataset.tar\\flickr_logos_27_dataset\\flickr_logos_27_dataset_images\\flickr_logos_27_dataset_images\\";
//	//读训练集
//	std::vector<LogRec::LogoTrainFileListFormat> vTrainList;
//
//	LogRec::LogoReadTrainFileList(sPath,"train.txt",vTrainList);
//	//开始训练,
//	std::string qf1 = "4159635668.jpg";
//	std::string qf2 = "3907703753.jpg";
//	std::string qr1 = "4498492208.jpg"; 
//	std::string qr2 = "3035356063.jpg";
//
//	for (int step = 3; step <= 5; step++)
//		for(int winSize = step; winSize <= 5; winSize++)		
//			for(int triArea = 8; triArea < 9; triArea++)
//			{
//				int tround = 0;
//				bool bFirst = true;
//				std::string lastName;	
//				string fileNa = "model_"+LogoParamToString(winSize)+"_"+LogoParamToString(step)+"_"+LogoParamToString(triArea)+"_"+"_.txt";
//				ofstream ofin(fileNa.c_str());
//
//				for (std::vector<LogRec::LogoTrainFileListFormat>::iterator itr = vTrainList.begin();
//					itr != vTrainList.end(); itr++)
//				{
//					if (bFirst)
//					{
//						lastName = itr->m_sClasName;
//						bFirst = false;
//					}
//
//					if (lastName != itr->m_sClasName)
//					{
//						string sline = lastName + " ";
//						if (mSig.size() > 0)
//						{
//							for (std::set<long long>::iterator itt = (mSig.begin())->second.begin(); 
//								itt != (mSig.begin())->second.end(); itt++)
//							{
//								string tstr;
//								ostringstream ostr;
//								ostr << *itt << " ";
//								sline += ostr.str();
//								//istringstream istr(ostr.str());
//								//istr >> tstr;
//							}
//							lastName = itr->m_sClasName;
//							ofin << sline << endl;
//							//mSig.clear();
//						}
//					}
//					std::vector<LogRec::LogoTriangle> vLtri;
//
//
//			#if 1
//					param_multi_scale_denaulay(sPath,itr->m_sFileName,vCodeBook,vLtri,itr->m_rBoundBox,winSize,step,triArea);
//			#else
//					multi_scale_denaulay(sPath,itr->m_sFileName,vCodeBook,vLtri,itr->m_rBoundBox);
//			#endif
//
//					for (std::vector<LogRec::LogoTriangle>::iterator sitr = vLtri.begin(); 
//						sitr != vLtri.end(); sitr++)
//					{
//						sitr->Logo_ToSignature();
//						mSig[itr->m_sClasName].insert(sitr->m_llSignature);
//			
//					}
//					std::cout << tround << " round" << std::endl;
//					tround++;
//				}
//				singleQuery(vCodeBook,mSig,sPath,qf1,qf2,winSize,step,triArea);
//				singleQuery(vCodeBook,mSig,sPath,qr1,qr2,winSize,step,triArea);
//			}
//}


//##########

void readme();

/** @function main */
int run( int argc, char** argv )
{
  if( argc != 3 )
  { readme(); return -1; }

  Mat img_1 = imread( argv[1], CV_LOAD_IMAGE_GRAYSCALE );
  Mat img_2 = imread( argv[2], CV_LOAD_IMAGE_GRAYSCALE );

  if( !img_1.data || !img_2.data )
  { std::cout<< " --(!) Error reading images " << std::endl; return -1; }

  //-- Step 1: Detect the keypoints using SURF Detector
  int minHessian = 400;

  SurfFeatureDetector detector( minHessian );

  std::vector<KeyPoint> keypoints_1, keypoints_2;

  detector.detect( img_1, keypoints_1 );
  detector.detect( img_2, keypoints_2 );

  //-- Step 2: Calculate descriptors (feature vectors)
  SurfDescriptorExtractor extractor;

  Mat descriptors_1, descriptors_2;

  /*for (int i = 0; i < descriptors_1.rows; i++)
  {
	  for (int j = 0; j < descriptors_1.cols; i++)
		{
			cout << descriptors_1[i][j] << endl;
		}
  }*/
  extractor.compute( img_1, keypoints_1, descriptors_1 );
  extractor.compute( img_2, keypoints_2, descriptors_2 );

  //-- Step 3: Matching descriptor vectors using FLANN matcher
  FlannBasedMatcher matcher;
  std::vector< DMatch > matches;
  matcher.match( descriptors_1, descriptors_2, matches );

  double max_dist = 0; double min_dist = 100;

  //-- Quick calculation of max and min distances between keypoints
  for( int i = 0; i < descriptors_1.rows; i++ )
  { double dist = matches[i].distance;
    if( dist < min_dist ) min_dist = dist;
    if( dist > max_dist ) max_dist = dist;
  }

  printf("-- Max dist : %f \n", max_dist );
  printf("-- Min dist : %f \n", min_dist );

  //-- Draw only "good" matches (i.e. whose distance is less than 2*min_dist )
  //-- PS.- radiusMatch can also be used here.
  std::vector< DMatch > good_matches;

  for( int i = 0; i < descriptors_1.rows; i++ )
  { if( matches[i].distance < 2*min_dist )
    { good_matches.push_back( matches[i]); }
  }

  //-- Draw only "good" matches
  Mat img_matches;
  drawMatches( img_1, keypoints_1, img_2, keypoints_2,
               good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
               vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

  //-- Show detected matches
  imshow( "Good Matches", img_matches );

  for( int i = 0; i < good_matches.size(); i++ )
  {
	  printf( "-- Good Match [%d] Keypoint 1: %d  -- Keypoint 2: %d  \n", i, good_matches[i].queryIdx, good_matches[i].trainIdx );
  }

  waitKey(0);

  return 0;
 }

 /** @function readme */
 void readme()
 { std::cout << " Usage: ./SURF_FlannMatcher <img1> <img2>" << std::endl; }

//##########


int totalFeatures(string filePath, string fileList)
{
	ifstream fin((filePath+fileList).c_str());

	if (!fin)
	{
		cerr << "open file failed!" << endl;
		return -1;
	}
	string path = filePath;

	//read images from given path
	int total = 0;
	string line;
	while (getline(fin,line))
	{
		istringstream filenameExtractor;
		//string str;
		string fileDir;
		filenameExtractor.str(line.c_str());
		filenameExtractor >> fileDir;// >> str;

		string fileName;
		filenameExtractor >> fileName;
		string fullNamePath = path+fileDir+"\\"+fileName;

		//根据路径抽取图片特征点
		const char* image_filename = fullNamePath.c_str();
		CvMemStorage* storage = cvCreateMemStorage(0);
		IplImage* image = cvLoadImage( image_filename, CV_LOAD_IMAGE_GRAYSCALE );//CV_LOAD_IMAGE_GRAYSCALE
 
		if( !image )
		{
			fprintf( stderr, "Can not load %s \n",image_filename );
			continue;
			//goto l1;
			//exit(-1);
		}
		cout << "now read " << fileDir << "  " << fileName << endl;
		CvSeq *imageKeypoints = 0, *imageDescriptors = 0;

		CvSURFParams params = cvSURFParams(500, 1);
		cvExtractSURF( image, 0, &imageKeypoints, &imageDescriptors, storage, params );
		total += imageDescriptors->total;
		cvReleaseMemStorage(&storage);
	}

	cout << "the total features are:" << endl;
	ofstream ofin("tres.txt");
	ofin << total << endl;
	return total;
}

int lastmain(int argc, char* argv[])
{
#if 0
	int thresh_size = 500000;
	int k = 1000;
	string filename = "";
	std::string filePath;
	std::string fileList;
	if (argc == 6)
	{
		//char* p = argv[1];
		filename = argv[1];
		thresh_size = atoi(argv[2]);
		k = atoi(argv[3]);
		filePath = argv[4];
		fileList = argv[5];
	}
	else
	{
		printf("Usage: %s [resultFile] [thresh_size] [k] [filePath] [fileList]",argv[0]);
		return -1;
	}
	get_visual_vocabulary(filename,thresh_size,k,filePath, fileList);
////
#endif

#if 0
	std::string filePath;
	std::string vabfile;
	std::string trainfile;
	std::string modefile;
	
	if (argc == 5)
	{
		filePath = argv[1];
		vabfile  = argv[2];
		trainfile = argv[3];
		modefile = argv[4];
	}
	else
	{
		printf("Usage: %s [filePath] [vabFile] [modelFile]",argv[0]);
		return -1;
	}
#endif

#if 0
	std::string filePath;
	std::string vabfile;
	std::string modelFile;
	std::string queryFile;
	std::string queryPath;
	if (argc == 6)
	{
		filePath = argv[1];
		vabfile  = argv[2];
		modelFile = argv[3];
		queryFile = argv[4];
		queryPath = argv[5];
	}
	else
	{
		printf("Usage: %s [filePath] [vabFile] [model] [logoQuery] [qPath]",argv[0]);
		return -1;
	}
#endif
	//get_visual_vocabulary("C:\\Users\\beyondboy\\Desktop\\logo recognition\\flickr_logos_27_dataset.tar\\flickr_logos_27_dataset\\flickr_logos_27_dataset_images\\flickr_logos_27_dataset_images\\",\
	//	"flickr_logos_27_dataset_training_set_annotation.txt","vocal.txt");
	////cout << sizeof(float) << endl;	

	//logoRecProcess("C:\\Users\\beyondboy\\Desktop\\logo recognition\\flickr_logos_27_dataset.tar\\flickr_logos_27_dataset\\flickr_logos_27_dataset_images\\flickr_logos_27_dataset_images\\","vocal.txt");
	////LogRec::Logo_TrainFileListReadTest();
	//LogRec::Logo_EuclideanTest();
	//LogRec::Logo_LoadCodeBookTest();
	//LogRec::Logo_SignatureTest();
	////ogRec::Logo_MultiDelaunayTest();
	//logoRecProcess(filePath, vabfile,trainfile,modefile);
	//logoRecProcess("C:\\Users\\beyondboy\\Desktop\\LogoRecSys\\trainset\\train\\", "val4000.txt", "t1.txt", "model.txt");
	////logoParamRecProcess(filePath, vabfile);
	/*std::map<std::string,std::set<long long> > mModel;
	LogoLoadModel("./", "model.txt",mModel);*/
	//LogoQuery(filePath, vabfile, modelFile, queryFile,queryPath);
	//LogoQuery("C:\\Users\\beyondboy\\Desktop\\LogoRecSys\\", "vocal.txt", "model.txt", "4605630935.jpg","C:\\Users\\beyondboy\\Desktop\\LogoRecSys\\flickr_logos_27_dataset_images\\");
	//run(argc, argv);

	//LogRec::Logo_MemLeakTest();
	//totalFeatures("E:\\FlickrLogos32\\FlickrLogos\\classes\\","trainingset.txt");
	long long ll = 0xfe00000000009988;

	cout << std::tanf((90*2*3.1415926)/360) << " T:" << std::atan(-1.0)*((360)/(2*3.1415926))<< endl;

//##########################################
	//第一步，跑词汇 visual vocabulary
	/*for (int i = 0; i < 100000000; i++)
	{
		for (int j = 0; j < 10000; j++)
		{
			LogRec::Logo_AngleTest();
		}
		if (i%1000000 == 0)
		{
			cout << i << "round!" << endl;
		}
	}
	cout << "over!" << endl;*/

	return 0;
}

//the following is core code of realization of <scalable Logo recognition in real-world images>
bool IsPointAlreadyIn(std::vector<LogRec::LogoSLRPoint> &VSet,LogRec::LogoSLRPoint &Pt)
{
	for (std::vector<LogRec::LogoSLRPoint>::iterator itr = VSet.begin(); itr != VSet.end(); itr++)
	{
		if (Pt == *itr)
		{
			return true;
		}
	}
	return false;
}

bool IsEdgeAlreadyIn(std::vector<LogRec::LogoSLREdge> &EdgeSet,LogRec::LogoSLREdge &E)
{
	for (std::vector<LogRec::LogoSLREdge>::iterator itr = EdgeSet.begin(); itr != EdgeSet.end(); itr++)
	{
		if (E == *itr)
		{
			return true;
		}
	}
	return false;
}

//在两张图片中得到有相同label的特征点子集


void imageTest(std::string sfileA, std::string sfileB)
{
	
	/*std::string sfileA = filePath+className+"\\"+fitr->first;
	std::string sfileB = filePath+className+"\\"+sitr->first;*/
	const char* fileA = sfileA.c_str();
	const char* fileB = sfileB.c_str();

	IplImage* ImgA = cvLoadImage( fileA, CV_LOAD_IMAGE_GRAYSCALE );
	IplImage* ImgB = cvLoadImage( fileB, CV_LOAD_IMAGE_GRAYSCALE );

	if ( !ImgA || !ImgB )
	{
		fprintf(stderr,"load img file failed!\n");
		;
	}

	IplImage* CImgA = cvCreateImage(cvGetSize(ImgA),8,1);
	IplImage* CImgB = cvCreateImage(cvGetSize(ImgB),8,1);

	cvSetImageROI(CImgA,cvRect(0,0,ImgA->width,ImgA->height));
	cvCopy(ImgA,CImgA);
	cvResetImageROI(CImgA);
	cvSetImageROI(CImgB,cvRect(0,0,ImgB->width,ImgB->height));
	cvCopy(ImgB,CImgB);
	cvResetImageROI(CImgB);

	cvShowImage("ImgA",CImgA);
	cvShowImage("ImgB",CImgB);
	cvWaitKey(0);
	cvDestroyWindow("ImgA");
	cvDestroyWindow("ImgB");
}


//bool Logo
//用蓄水池方法抽取出m_iMontCarloDensity个样本EDGE

bool LogoBatchQuery()
{

	return true;
}

bool LoadConf(std::string confFile)
{
	std::ifstream ifconf(confFile.c_str());

	if ( !ifconf )
	{
		std::cerr << "open conf File failed! " << __FILE__ << __LINE__ << std::endl;
		return false;
	}
	std::map<std::string, std::string> m_confList;	
	std::string key;
	std::string value;
	std::string sline;

	while(std::getline(ifconf,sline))
	{
		if ( sline.find('#') == 0 )
		{
			m_confList["comments"] += sline;
		}
		else if ( ( sline.find('#') != std::string::npos ) )
		{
			std::string line = sline.substr( 0,sline.find_first_of('#') );
			key = line.substr(0,line.find_first_of('='));
			value = line.substr(line.find_first_of('=')+1);
			std::cout << "key:" << key << " value:" << value << std::endl;
			m_confList[key] = value;
			m_confList["comments"] += sline.substr(sline.find_first_of('#'));
		}
		else
		{
			std::string line = sline;
			key = line.substr(0,line.find_first_of('='));
			value = line.substr(line.find_first_of('=')+1);
			std::cout << "key:" << key << " value:" << value << std::endl;
			m_confList[key] = value;
		}

	}

	return true;
}


string copy(int m, string & str1)
{
	string str2 = "";
	if (str1.length() < m)
	{
		return str2;
	}
	for (int i = m; i < str1.length(); i++)
	{
		str2 += str1.at(i);
	}
	return str2;
}
int vfun(const char* fmt, ...)
{
	va_list argp;
	int cnt;
	va_start(argp,fmt);
	cnt = vfprintf(stderr,fmt,argp);
	va_end(argp);
	return cnt;
}

#define LALL(logInfo)	vfun(logInfo);

bool some_func(int i, double d)
{
	return i > d;
}

void boostTest()
{
	boost::function<bool (int,double)> f;
	f = &some_func;
	if ( f(10,1.2) )
	{
		cout << "inter is larger!" << endl;
	}
}
void hello()
{
	std::cout << 
	"Hello world, I'm a thread!"
	<< std::endl;
}
void boostThreadTest()
{
	boost::thread thrd(&hello);
	thrd.join();
}

boost::mutex io_mutex;
struct dcount
{
	dcount(int id) : id(id){}
	void operator()()
	{
		for (int i = 0; i < 500; ++i)
		{
			boost::mutex::scoped_lock
			lock(io_mutex);
			std::cout << id << ": "
			<< i << std::endl;
		}
	}
	int id;
};



void scoped_lock_test()
{
	boost::thread thrd1(dcount(1));
	boost::thread thrd2(dcount(2));
	thrd1.join();
	thrd2.join();
}

void count1(int id)
{
	for (int i = 0; i < 50; ++i)
	{
		boost::mutex::scoped_lock
		lock(io_mutex);
		std::cout << id << ": "
		<< i << std::endl;
	}
}

void thread_bind_test()
{
	boost::thread thrd1(boost::bind(&count1, 1));
	boost::thread thrd2(boost::bind(&count1, 2));
	thrd1.join();
	thrd2.join();
}

const int BUF_SIZE = 10;
const int ITERS = 100;

class buffer
{
public:
	typedef boost::mutex::scoped_lock scoped_lock;
	buffer() : p(0), c(0), full(0)
	{
	}
	void put(int m)
	{
		scoped_lock lock(mutex);
		if (full == BUF_SIZE)
		{
			{
				boost::mutex::scoped_lock lock(io_mutex);
				std::cout << "Buffer is full. Waiting " << std::endl;
			}
			while (full == BUF_SIZE)
				cond.wait(lock);
		}
		buf[p] = m;
		p = (p+1) % BUF_SIZE;
		++full;
		cond.notify_one();
	}

	int get()
	{
		scoped_lock lk(mutex);
		if (full == 0)
		{
			{
				boost::mutex::scoped_lock lock(io_mutex);
				std::cout << "Buff is empty. Waiting " << std::endl;
			}
			while (full == 0)
			{
				cond.wait(lk);
			}
		}
		int i = buf[c];
		c = (c+1) % BUF_SIZE;
		cond.notify_one();
		full--;
		return i;
	}
private:
	boost::mutex mutex;
	boost::condition cond;
	unsigned int p,c,full;
	int buf[BUF_SIZE];
};

buffer buf;

void writer()
{
	for (int n = 0; n < ITERS; ++n)
	{
		{
			boost::mutex::scoped_lock lock(io_mutex);
			std::cout << "sending: " << n << std::endl;
		}
		buf.put(n);
	}
}

void reader()
{
	for (int x = 0; x < ITERS; ++x)
	{
		int n = buf.get();
		{
			boost::mutex::scoped_lock lock(io_mutex);
			std::cout << "received: " << n << std::endl;
		}
	}
}

void thread_condition_test()
{
	boost::thread thrd1(&reader);
	boost::thread thrd2(&writer);
	thrd1.join();
	thrd2.join();
}


boost::thread_specific_ptr<int> ptr;

struct countPtr
{
	countPtr(int id) : id(id) {}

	void operator()()
	{
		if (ptr.get() == 0)
		{
			ptr.reset(new int(0));
		}

		for (int i = 0; i < 100; ++i)
		{
			(*ptr)++;
			boost::mutex::scoped_lock lock(io_mutex);
			std::cout << id << ": " << *ptr << std::endl;
		}
	}

	int id;
};

void thread_pointer_test()
{
	boost::thread thrd1(countPtr(1));
	boost::thread thrd2(countPtr(2));
	thrd1.join();
	thrd2.join();
}

int main(int argc, char* argv[])
{

//##########################################

	//LogRec::Logo_EuclideanTest();
	//LogRec::Logo_get_visual_vocabularyRWI("newVocal.txt", 438000, 1600,"E:\\FlickrLogos32\\FlickrLogos\\classes\\","trainingset.txt");
	//训练参数
#if 0
	std::string filePath;
	std::string vabfile;
	std::string trainList;
	if (argc == 4)
	{
		filePath = argv[1];
		vabfile  = argv[2];
		trainList = argv[3];
	}
	else
	{
		printf("Usage: %s [filePath] [vabFile] [trainList]",argv[0]);
		return -1;
	}
#endif
	//测试参数
#if 0
	std::string sFilePath;
	std::string sQuery;
	std::string valDicFile;
	std::string sModelFileList;

	if (argc == 5)
	{
		sFilePath		 = argv[1];
		sQuery			 = argv[2];
		valDicFile		 = argv[3];
		sModelFileList	 = argv[4];
	}
	else
	{
		printf("Usage: %s [sFilePath] [sQuery] [valDicFile] [sModelFileList]",argv[0]);
		return -1;
	}

#endif
	////第二步，训练，结对训练
	/*boostTest();
	boostThreadTest();
	scoped_lock_test();
	thread_bind_test();
	thread_condition_test();
	thread_pointer_test();*/
	gLogoParams.m_iMaxComNum = 10;
	//ReadTrainSRLFileList("F:\\logo\\FlickrLogos32\\FlickrLogos32\\FlickrLogos\\classes\\","newVocal.txt","test.txt");
	//F:\logo\FlickrLogos32\FlickrLogos32\FlickrLogos\classes
	//LogoSingleQuery( "D:\\LOGO\\Logo1223\\", "2429624705.jpg", "newVocal.txt", "lg.txt" );
	//ReadTrainSRLFileList( filePath,vabfile,trainList);

	//LoadConf("conf.txt");
	LogRec::LOG_SET_MIN_LEVEL(LogRec::LEVEL_ALL);
	LogRec::LOG_SET_OPTION(LogRec::LOG_OPTION_ALL);

	//test of surf to find keypoints

	LogRec::CLogoSURF_Wrapper surfWrapper;
	//surfWrapper.matchOfTwoImgs("1.jpg","2.jpg");
	//surfWrapper.findKeyPoints("1.jpg");
	//unit test
	//LogRec::startUnitTest();
	/*
	*
	*@测试
	*@用基于三角形来描述局部特征，并用边和三角形的级联索引方式进行查询
	*@
	*
	*/
	//Logo_get_visual_vocabularyRWI()

	//跑视觉词汇表
	//Logo_BOFTest();

	LogRec::CTriTrain ctrlogo;
	ctrlogo.LoadConf("conf.txt");
	ctrlogo.Training();
	/*LogRec::CTriAngleQuery ctrlq;
	ctrlq.LoadConf("conf.txt");
	ctrlq.Query("2429624705.jpg");*/

	/*
	*
	*@测试
	*@基于多尺度delaunay三角剖分的方法做logo识别
	*
	*/
	
/*	int yum = 45;
	LogRec::CDelaunayQuery dqr;
	string str = "welcome home!";
	char cstr[] = "welcome home!";
	LOG_LEVEL_ALL
	PRINT_LOG_MESSAGE("111Load conf over! %d %s\n", yum, cstr);
	LOG_LEVEL_ALL
	PRINT_LOG_MESSAGE("2Load conf over! %d %s\n",yum,cstr);
	LOG_LEVEL_ALL
	PRINT_LOG_MESSAGE("Load conf over! %d %s\n", yum, cstr);

	dqr.LoadConf("conf.txt");


	dqr.LogoQuery("4737345823.jpg");*/
	
	//##########################################
	//LogRec::Logo_SingletonTest();
	system( "pause" );
	return 0;
}
