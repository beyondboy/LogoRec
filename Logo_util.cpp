#include "stdafx.h"
#include "Logo_base.h"

#include "Logo_util.h"
#include "Logo_log.h"
namespace LogRec
{
	/*

	*/
	bool GetTriCode(std::vector<LogRec::LogoPoint> &vlp, LogRec::LogoTriangle &tris)
	{
		bool bp1 = false;
		bool bp2 = false;
		bool bp3 = false;
		for (std::vector<LogRec::LogoPoint>::iterator itr = vlp.begin(); itr != vlp.end(); itr++)
		{
			if (*itr == tris.m_A)
			{
				tris.m_A.m_uVisualWord = itr->m_uVisualWord;
				bp1 = true;
			}
			if (*itr == tris.m_B)
			{
				tris.m_B.m_uVisualWord = itr->m_uVisualWord;
				bp2 = true;
			}
			if (*itr == tris.m_C)
			{
				tris.m_C.m_uVisualWord = itr->m_uVisualWord;
				bp3 = true;
			}
			if (bp1 && bp2 && bp3)
			{
				return true;
			}
		}
		return false;
	}
	//point wrapper
	LogoSLRPoint logoSLRPoint(CvSURFPoint &surfp,unsigned short visualWord)
	{
		LogoSLRPoint slrp;
		slrp = surfp;
		slrp.m_uVisualWord = visualWord;
		return slrp;
	}

	LogoSLRPoint logoSLRPoint(KeyPoint &keypt,unsigned short visualWord)
	{
		LogoSLRPoint slrp;
		slrp = keypt;
		slrp.m_uVisualWord = visualWord;
		return slrp;
	}
	CvSURFPoint  KeyPointToSurfPoint(KeyPoint &keypt)
	{
		CvSURFPoint cvSurfpt;
		cvSurfpt.pt.x = keypt.pt.x;
		cvSurfpt.pt.y = keypt.pt.y;
		cvSurfpt.dir = keypt.angle;
		cvSurfpt.size = keypt.size;
		cvSurfpt.hessian = 0;
		cvSurfpt.laplacian = 0;
		return cvSurfpt;
	}

	bool IsAlreadyInTriangles(std::vector<LogRec::LogoTriangle> &vLtri, LogRec::LogoTriangle &tris)
	{

		for (int i = 0; i < vLtri.size (); i ++)
		{
			if (vLtri[i] == tris)
			{
				return true;
			}
		}
		return false;
	}

	bool TriangleIsInRect(CvRect &rect,CvPoint *tri)
	{
		for (int i = 0; i < 3; i++)
		{
			if ((tri[i].x < 0) || (tri[i].x > rect.width) || (tri[i].y < 0) || (tri[i].y > rect.height))
			{
				return false;
			}
		}
		return true;
	}

	void LogoReadTrainFileList(std::string filePath, std::string fileName, std::vector<LogRec::LogoTrainFileListFormat> &vTrainList)
	{
		ifstream fin((filePath+fileName).c_str());//("flickr_logos_27_dataset_training_set_annotation.txt");

		if (!fin)
		{
			cout << "file open error!" << endl;//use log in the future!
			return;
		}

		string line;
		string lastLine;
		while (getline(fin,line))
		{

			LogRec::LogoTrainFileListFormat ltf;
			istringstream eleExtractor;
			eleExtractor.str(line.c_str());

			eleExtractor >> ltf.m_sFileName;//
			eleExtractor >> ltf.m_sClasName;
			eleExtractor >> ltf.m_iSubClassType;
			eleExtractor >> ltf.m_rBoundBox.m_iLeft;
			eleExtractor >> ltf.m_rBoundBox.m_iTop;
			eleExtractor >> ltf.m_rBoundBox.m_iRight;
			eleExtractor >> ltf.m_rBoundBox.m_iBottom;

			vTrainList.push_back(ltf);		
		}
	}

	bool LogoLoadCodeBook(std::string filePath, std::string fileName, std::vector<LogRec::LogoCodeBookFormat> &vCodeBook)
	{//读取visual words vocabulary
		ifstream fin((filePath+fileName).c_str());//("flickr_logos_27_dataset_training_set_annotation.txt");

		if (!fin)
		{
			cout << "file open error!" << __FILE__ << __LINE__ << endl;//use log in the future!
			return false;
		}

		string line;
		int iSig = 1;
		while (getline(fin,line))
		{
			LogRec::LogoCodeBookFormat lcb;
			istringstream eleExtractor;
			eleExtractor.str(line.c_str());
			for(int i = 0; i < 128; i++)
			{
				eleExtractor >> lcb.m_fDescriptor[i];
			}
			lcb.m_iSig = iSig;
			iSig++;
			vCodeBook.push_back(lcb);
		}
		return true;
	}

	bool LogoEdgeIsRepeated(std::vector<unsigned int> &edges, unsigned int e)
	{
		for (std::vector<unsigned int>::iterator first = edges.begin(); first != edges.end(); first++)
		{
			if (*first == e)
			{
				return false;
			}
		}
		return true;
	}

	bool LogoVectorIsNotInFormerTriangles(const vector<vector<int> >& one_tri, 
												const vector<vector<vector<int> > > &tris)
	{
		set<vector<int> > tTriangle;
		set<vector<int> > sTriangle;

		for (int i = 0; i < tris.size (); i ++)
		{
			tTriangle.clear();
			sTriangle.clear();
			for (int j = 0; j < 3; j++ )
			{	
				tTriangle.insert(tris[i][j]);
				sTriangle.insert(one_tri[j]);
			}
			if (tTriangle == sTriangle)
			{
				return false;
			}
		}
		return true;
	}

	void Logo_draw_subdiv_facet(IplImage* img,CvSubdiv2DEdge edge)
	{
		CvSubdiv2DEdge t = edge;
		int i, count = 0;
		CvPoint * buf = 0;

		do
		{
			count++;
			t = cvSubdiv2DGetEdge(t,CV_NEXT_AROUND_LEFT);
		}while ( t != edge );

		buf = (CvPoint*)malloc(count * sizeof(buf[0]));
		t = edge;

		for (i = 0; i < count; i++)
		{
			CvSubdiv2DPoint* pt = cvSubdiv2DEdgeOrg( t );
			if ( !pt )
			{
				break;
			}
			buf[i] = cvPoint( cvRound(pt->pt.x), cvRound(pt->pt.y) );
			t = cvSubdiv2DGetEdge( t, CV_NEXT_AROUND_LEFT );
		}

		if ( i == count )
		{
			CvSubdiv2DPoint* pt = cvSubdiv2DEdgeDst( cvSubdiv2DRotateEdge(edge,1));
			cvFillConvexPoly( img, buf, count, CV_RGB(255,255,255), CV_AA, 0 );
			cvPolyLine( img, &buf, &count, 1, 1, CV_RGB(0,0,0), 1, CV_AA, 0);
			//draw_subdiv_point( img, pt->pt, CV_RGB(0,0,0));
		}
		free( buf );
	}
	void Logo_paint_voronoi(CvSubdiv2D* subdiv, IplImage* img)
	{
		CvSeqReader reader;
		int i, total = subdiv->edges->total;
		int elem_size = subdiv->edges->elem_size;
		cvCalcSubdivVoronoi2D( subdiv );

		cvStartReadSeq( (CvSeq*)(subdiv->edges), &reader, 0 );

		for ( i = 0; i < total; i++)
		{
			CvQuadEdge2D* edge = (CvQuadEdge2D*)(reader.ptr);

			if ( CV_IS_SET_ELEM( edge ))
			{
				CvSubdiv2DEdge e = (CvSubdiv2DEdge)edge;

				Logo_draw_subdiv_facet( img, cvSubdiv2DRotateEdge( e, 0 ) );

				//draw_subdiv_facet( img, cvSubdiv2DRotateEdge( e, 3 ) );
			}

			CV_NEXT_SEQ_ELEM( elem_size, reader );
		}
	}

	void Logo_draw_subdiv_point( IplImage* img, CvPoint2D32f fp, CvScalar color )
	{
		cvCircle( img, cvPoint(cvRound(fp.x), cvRound(fp.y)), 3, color, CV_FILLED, 8, 0 );
	}

	int Logo_get_visual_vocabulary(std::string dstVocabFile, int thresh_size, int k, std::string filePath, std::string fileList)
	{
	
		ifstream fin((filePath+fileList).c_str());

		if (!fin)
		{
			cerr << "open file failed!" << endl;
			return -1;
		}
		string path = filePath;

		//read images from given path
		string line;

		//用来聚类求visual vocabulary 的矩阵
		cv::Mat m_Features(thresh_size, 128, CV_32F);

		int picNum = 0;
		int i_totalF = 0;
		float* p_imgPtr = m_Features.ptr<float>(0);

		while (getline(fin,line))
		{
			istringstream filenameExtractor;
			//string str;
			filenameExtractor.str(line);
			filenameExtractor >> line;// >> str;
			string fullNamePath = path+line;

			//根据路径抽取图片特征点
			const char* image_filename = fullNamePath.c_str();
			CvMemStorage* storage = cvCreateMemStorage(0);
			IplImage* image = cvLoadImage( image_filename, CV_LOAD_IMAGE_GRAYSCALE );//CV_LOAD_IMAGE_GRAYSCALE
 
			if(!image )
			{
				fprintf( stderr, "Can not load %s \n",image_filename );
				continue;
				//goto l1;
				//exit(-1);
			}

			CvSeq *imageKeypoints = 0, *imageDescriptors = 0;

			CvSURFParams params = cvSURFParams(500, 1);
			cvExtractSURF( image, 0, &imageKeypoints, &imageDescriptors, storage, params );

			int length = (int)(imageDescriptors->elem_size/sizeof(float));
			//cv::Mat m_image(imageDescriptors->total, length, CV_32F);
		
			//提取到矩阵里
			CvSeqReader img_reader;
			//float* img_ptr = m_image.ptr<float>(0);
			cvStartReadSeq( imageDescriptors, &img_reader );
			for(int i = 0; i < imageDescriptors->total; i++ )
			{
				const float* descriptor = (const float*)img_reader.ptr;
				CV_NEXT_SEQ_ELEM( img_reader.seq->elem_size, img_reader );
				memcpy(p_imgPtr, descriptor, length*sizeof(float));
				p_imgPtr += length;

				i_totalF++;
				if (i_totalF > thresh_size)
				{
					break;
				}
			}

			if (i_totalF > thresh_size)
			{
				break;
			}
			picNum++;
			cout << "name:" << line << "  itotalf:" << i_totalF << "  picNum:" << picNum << endl;
			cvReleaseImage(&image);
			//add by 2011.12.14
			cvReleaseMemStorage(&storage);
			image = NULL;
		}
		cout << "toal features:" << i_totalF << endl;
		BOWKMeansTrainer bowKms(k);
		//bowKms.add(m_Features);
		double tCount = (double)cvGetTickCount();

		cout << "begin kmeans------------>>>>>>>>>>>>>>" << endl;
		cv::Mat m_vocab = bowKms.cluster(m_Features);
		cout << "kmeans over ---------->>>>>>>>>>>>" << endl;
		tCount = (double)cvGetTickCount() - tCount;

		ofstream ofin((filePath+dstVocabFile).c_str());
		//float farr[2][3] = {{4.233,0.987,4.32},{0.44,0.666,0.34}};
		printf("The Clustering result!\n");

		for (int j = 0; j < m_vocab.rows; j++)
		{
			string sline;
			for (int k = 0; k < m_vocab.cols; k++)
			{
				string tstr;
				ostringstream ostr;
				ostr << m_vocab.at<float>(j,k) << " ";
				sline += ostr.str();
				//istringstream istr(ostr.str());
				//istr >> tstr;
			}
			ofin << sline << endl;
			//printf("\n");
		}
		ostringstream lline;

		lline << "cost time:" << tCount;
		ofin << lline.str() << endl;
	}

	int Logo_get_visual_vocabularyRWI(std::string dstVocabFile, int thresh_size, int k, std::string filePath, std::string fileList)
	{
	
		ifstream fin((filePath+fileList).c_str());

		if (!fin)
		{
			//cerr << "open file failed!" << endl;
			LogRec::LOG_LEVEL_ERROR;
			LogRec::PRINT_LOG_MESSAGE("failed to open file!");
			return -1;
		}
		string path = filePath;

		//read images from given path
		string line;

		//用来聚类求visual vocabulary 的矩阵
		cv::Mat m_Features(thresh_size, 128, CV_32F);

		int picNum = 0;
		int i_totalF = 0;
		float* p_imgPtr = m_Features.ptr<float>(0);

		LogRec::LOG_SET_OPTION(LogRec::LOG_OPTION_INFO);

		while (getline(fin,line))
		{
			istringstream filenameExtractor;

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
 
			if(!image )
			{
				//fprintf( stderr, "Can not load %s \n",image_filename );
			LogRec::LOG_LEVEL_WARN;
			LogRec::PRINT_LOG_MESSAGE("failed to open fiel!");				
				continue;
				//goto l1;
				//exit(-1);
			}

			//cvExtractSIFT();
			CvSeq *imageKeypoints = 0, *imageDescriptors = 0;
			//cvGetMat()
			CvSURFParams params = cvSURFParams(500, 0);
			cvExtractSURF( image, 0, &imageKeypoints, &imageDescriptors, storage, params );
			int length = (int)(imageDescriptors->elem_size/sizeof(float));
			//cv::Mat m_image(imageDescriptors->total, length, CV_32F);
		
			//提取到矩阵里
			CvSeqReader img_reader;
			//float* img_ptr = m_image.ptr<float>(0);

			cvStartReadSeq( imageDescriptors, &img_reader );
			for(int i = 0; i < imageDescriptors->total; i++ )
			{
				const float* descriptor = (const float*)img_reader.ptr;
				CV_NEXT_SEQ_ELEM( img_reader.seq->elem_size, img_reader );
				memcpy(p_imgPtr, descriptor, length*sizeof(float));
				p_imgPtr += length;

				i_totalF++;
				if (i_totalF > thresh_size)
				{
					break;
				}
			}

			if (i_totalF > thresh_size)
			{
				break;
			}
			picNum++;

			LogRec::LOG_LEVEL_INFO;
			LogRec::PRINT_LOG_MESSAGE("PIC_NAME: %s TOTAL_FEATURES: %d PIC_NUM: %d \n",
				line.c_str(),i_totalF,picNum);
			//cout << "name:" << line << "  itotalf:" << i_totalF << "  picNum:" << picNum << endl;
			cvReleaseImage(&image);
			////add by 2011.12.14
			cvReleaseMemStorage(&storage);
			image = NULL;
		}
		//cout << "toal features:" << i_totalF << endl;
		LogRec::LOG_LEVEL_INFO;
		LogRec::PRINT_LOG_MESSAGE("TOTAL_FEATURES: %d",i_totalF);
		
		BOWKMeansTrainer bowKms(k);
		//bowKms.add(m_Features);
		double tCount = (double)cvGetTickCount();
		LogRec::LOG_LEVEL_INFO;
		LogRec::PRINT_LOG_MESSAGE("<-----------------------KMEANS BEGIN----------------------->");
		//cout << "begin kmeans------------>>>>>>>>>>>>>>" << endl;
		cv::Mat m_vocab = bowKms.cluster(m_Features);
		LogRec::LOG_LEVEL_INFO;
		LogRec::PRINT_LOG_MESSAGE("<-----------------------KMEANS OVER----------------------->");
		tCount = (double)cvGetTickCount() - tCount;

		ofstream ofin((filePath+dstVocabFile).c_str());
		//float farr[2][3] = {{4.233,0.987,4.32},{0.44,0.666,0.34}};
		//printf("The Clustering result!\n");
		LogRec::LOG_LEVEL_INFO;
		LogRec::PRINT_LOG_MESSAGE("<-----------------------CLUSTERING RESULT----------------------->");
		
		for (int j = 0; j < m_vocab.rows; j++)
		{
			string sline;
			for (int k = 0; k < m_vocab.cols; k++)
			{
				string tstr;
				ostringstream ostr;
				ostr << m_vocab.at<float>(j,k) << " ";
				sline += ostr.str();
				//istringstream istr(ostr.str());
				//istr >> tstr;
			}
			ofin << sline << endl;
			//printf("\n");
		}
		ostringstream lline;

		lline << "cost time:" << tCount;
		ofin << lline.str() << endl;
	LogRec::LOG_SET_OPTION(LogRec::LOG_OPTION_ALL);
	}

	int Logo_get_visual_vocabularySIFT(std::string dstVocabFile, int thresh_size, int k, std::string filePath, std::string fileList)
	{
	
		ifstream fin((filePath+fileList).c_str());

		if (!fin)
		{
			//cerr << "open file failed!" << endl;
			LogRec::LOG_LEVEL_ERROR;
			LogRec::PRINT_LOG_MESSAGE("--failed to open file!--");
			return -1;
		}
		string path = filePath;

		//read images from given path
		string line;

		//用来聚类求visual vocabulary 的矩阵
		cv::Mat m_Features;
		int length = 128;
		m_Features.create(thresh_size, 128, CV_32FC1);
		
		int picNum = 0;
		int i_totalF = 0;
		float* p_imgPtr = m_Features.ptr<float>(0);


		LogRec::LOG_SET_OPTION(LogRec::LOG_OPTION_INFO);

		while ( getline(fin,line) )
		{
			istringstream filenameExtractor;

			string fileDir;
			filenameExtractor.str(line.c_str());
			filenameExtractor >> fileDir;// >> str;

			string fileName;
			filenameExtractor >> fileName;
			string fullNamePath = path+fileDir+"\\"+fileName;
			//根据路径抽取图片特征点

			cv::Mat image = cv::imread(fullNamePath,CV_LOAD_IMAGE_COLOR);
			if( !image.data )
			{
				LogRec::LOG_LEVEL_WARN;
				LogRec::PRINT_LOG_MESSAGE("---failed to open fiel!---");				
				continue;
			}

//#define _SURF_

#ifdef	_SURF_
			int hessian = 500;
			SurfFeatureDetector detector( hessian );
			SurfDescriptorExtractor extractor(4,2,1);
			Mat descriptors;
			LogRec::LOG_LEVEL_INFO;
			LogRec::PRINT_LOG_MESSAGE("--Use Surf");
#else
			SiftFeatureDetector detector;
			SiftDescriptorExtractor extractor;
			Mat descriptors;
			LogRec::LOG_LEVEL_INFO;
			LogRec::PRINT_LOG_MESSAGE("--Use Sift");
#endif			
			std::vector<cv::KeyPoint> keypoints;

			detector.detect(image,keypoints);
		
			extractor.compute(image,keypoints,descriptors);


			for(int i = 0; i < descriptors.rows; i++ )
			{
				//const float* descriptor = (const float*)descriptors.ptr<float>(i);
				const float* descriptor = (const float*)(descriptors.data + length*sizeof(float)*i);
				//descriptors
				memcpy(p_imgPtr, descriptor, length*sizeof(float));
				p_imgPtr += length;
				
				i_totalF++;
				/**/if (i_totalF > thresh_size)
				{
					break;
				}
			}

			if (i_totalF > thresh_size)
			{
				break;
			}
			picNum++;

			LogRec::LOG_LEVEL_INFO;
			LogRec::PRINT_LOG_MESSAGE("PIC_NAME: %s TOTAL_FEATURES: %d PIC_NUM: %d \n",
				line.c_str(),i_totalF,picNum);
			image = NULL;
		}

		LogRec::LOG_LEVEL_INFO;
		LogRec::PRINT_LOG_MESSAGE("TOTAL_FEATURES: %d",i_totalF);
		
		BOWKMeansTrainer bowKms(k);
		//bowKms.add(m_Features);
		double tCount = (double)cvGetTickCount();
		LogRec::LOG_LEVEL_INFO;
		LogRec::PRINT_LOG_MESSAGE("\n<-----------------------KMEANS BEGIN----------------------->\n");
		//cout << "begin kmeans------------>>>>>>>>>>>>>>" << endl;
		cv::Mat m_vocab = bowKms.cluster(m_Features);
		LogRec::LOG_LEVEL_INFO;
		LogRec::PRINT_LOG_MESSAGE("<-----------------------KMEANS OVER----------------------->");
		tCount = (double)cvGetTickCount() - tCount;

		ofstream ofin((filePath+dstVocabFile).c_str());
		//float farr[2][3] = {{4.233,0.987,4.32},{0.44,0.666,0.34}};
		//printf("The Clustering result!\n");
		LogRec::LOG_LEVEL_INFO;
		LogRec::PRINT_LOG_MESSAGE("<-----------------------CLUSTERING RESULT----------------------->");
		
		for (int j = 0; j < m_vocab.rows; j++)
		{
			string sline;
			for (int k = 0; k < m_vocab.cols; k++)
			{
				string tstr;
				ostringstream ostr;
				ostr << m_vocab.at<float>(j,k) << " ";
				sline += ostr.str();
				//istringstream istr(ostr.str());
				//istr >> tstr;
			}
			ofin << sline << endl;
			//printf("\n");
		}
		ostringstream lline;

		lline << "cost time:" << tCount;
		ofin << lline.str() << endl;
	LogRec::LOG_SET_OPTION(LogRec::LOG_OPTION_ALL);
	}
}
