#include "stdafx.h"
#include "Logo_common.h"
#include "Logo_base.h"
#include "Logo_util.h"
#include "Logo_multiDelaunay.h"
#include "Logo_funcTest.h"
#include "Logo_log.h"



namespace LogRec
{
	void Logo_MultiDelaunayTest()
	{
		LogRec::LogoMultiScaleDelaunay lmd;

		LogRec::LogoPoint A;
		A.m_ix = 3;
		A.m_iy = 6;
		A.m_uVisualWord = 2312;

		LogRec::LogoPoint B;
		B.m_ix = 4;
		B.m_iy = 6;
		B.m_uVisualWord = 5312;

		LogRec::LogoPoint C;
		C.m_ix = 3;
		C.m_iy = 6;
		C.m_uVisualWord = 8312;

		LogRec::LogoTriangle lt(A,B,C);
		lmd.Logo_InsertCode(A);
		lmd.Logo_InsertCode(B);
		lt.Logo_ToSignature();

		if (lmd.Logo_IsCodeAlreadyIn(C))
		{
			cout << "code already in! code: " << lmd.Logo_GetCode(C) << endl;
		}
		lmd.Logo_InsertTri(lt);
		LogRec::LogoTriangle ltt = lt;
		if (lmd.Logo_IsTriAlreadyIn(ltt))
		{
			cout << "tri already in! sig is :" << lmd.Logo_GetSignature(ltt) << endl;
		}
	}
	
	void Logo_EuclideanTest()
	{
		float x[4] = {1,1,1,1};
		float y[4] = {2,2,2,2};
		std::string f = "34.5";
		//float t = LogoFromString(f,x[0]);
		std::cout << LogRec::LogoEuclidean(x,y,4) << std::endl;
	}

	void Logo_TrainFileListReadTest()
	{
		std::vector<LogoTrainFileListFormat> vTrainList;
		LogoReadTrainFileList("C:\\Users\\beyondboy\\Desktop\\logo recognition\\flickr_logos_27_dataset.tar\\flickr_logos_27_dataset\\flickr_logos_27_dataset_images\\flickr_logos_27_dataset_images\\",\
			"flickr_logos_27_dataset_training_set_annotation.txt",vTrainList);
		for (std::vector<LogRec::LogoTrainFileListFormat>::iterator itr = vTrainList.begin();
			itr != vTrainList.end(); itr++)
		{
			std::cout << itr->m_sFileName << " ";
			std::cout << itr->m_sClasName << " ";
			std::cout << itr->m_iSubClassType << " ";
			std::cout << itr->m_rBoundBox.m_iLeft << " ";
			std::cout << itr->m_rBoundBox.m_iTop << " ";
			std::cout << itr->m_rBoundBox.m_iRight << " ";
			std::cout << itr->m_rBoundBox.m_iBottom << " ";
			std::cout << std::endl;
		}
	}

	void Logo_MemLeakTest()
	{
		std::vector<LogoTrainFileListFormat> vTrainList;
		std::string sPath = "C:\\Users\\beyondboy\\Desktop\\logorecognition\\flickr_logos_27_dataset.tar\\flickr_logos_27_dataset\\flickr_logos_27_dataset_images\\flickr_logos_27_dataset_images\\";
		std::string sFileList = "flickr_logos_27_dataset_training_set_annotation.txt";
		LogoReadTrainFileList(sPath,\
			sFileList,vTrainList);
		for (std::vector<LogRec::LogoTrainFileListFormat>::iterator itr = vTrainList.begin();
			itr != vTrainList.end(); itr++)
		{
			std::cout << itr->m_sFileName << " ";
			//IplImage* src = cvLoadImage( (const char*)itr->m_sFileName.c_str(), CV_LOAD_IMAGE_GRAYSCALE );//CV_LOAD_IMAGE_GRAYSCALE
			IplImage* src = cvLoadImage( (itr->m_sFileName.c_str()), CV_LOAD_IMAGE_GRAYSCALE );
			CvSeq *imageKeypoints = 0, *imageDescriptors = 0;
			CvMemStorage* storage_k = cvCreateMemStorage(0);
			CvSURFParams params = cvSURFParams(500, 1);
			cvExtractSURF( src, 0, &imageKeypoints, &imageDescriptors, storage_k, params );
			cvReleaseImage(&src);
			cvReleaseMemStorage(&storage_k);
		}
	}

	void Logo_LoadCodeBookTest()
	{
		std::vector<LogRec::LogoCodeBookFormat> vCodeBook;
		LogoLoadCodeBook("./", "vocal.txt",vCodeBook);
		for (int i = 0; i < 128; i++)
		{
			cout << vCodeBook[0].m_fDescriptor[i] << " ";
		}
		cout << vCodeBook[0].m_iSig << endl;
		for (int i = 0; i < 128; i++)
		{
			cout << vCodeBook[1999].m_fDescriptor[i] << " ";
		}
		cout << vCodeBook[1999].m_iSig << endl;
	}

	void Logo_SignatureTest()
	{
		LogRec::LogoPoint A;
		A.m_uVisualWord = 2312;
		LogRec::LogoPoint B;
		B.m_uVisualWord = 5312;
		LogRec::LogoPoint C;
		C.m_uVisualWord = 8312;

		LogRec::LogoTriangle lt(A,B,C);
		lt.Logo_ToSignature();
		std::cout << lt.m_llSignature << std::endl;
	}

	void Logo_AngleTest()
	{
		float dirI = 135;
		float dirJ = 315;
		LogoSLRPoint ptI(3,75);
		ptI.m_dir = 135;
		LogoSLRPoint ptJ(4,76);
		ptJ.m_dir = 315;
		float ralpha;
		float rbeta;
		LogoGetEdgeRelativeAngles(ptI, ptJ, ralpha, rbeta);
		//std::cout << "alpha:" << ralpha << "beta:" << rbeta << std::endl;
	}
	void Logo_BitPackTest()
	{
		int theta1 = 1;
		int theta2 = 1;
		int ori1 = 1;
		int ori2 = 1;
		int ori3 = 1;
		long long ll = 0x00;
	
		int v1 = 4000;
		int v2 = 3990;
		int v3 = 3543;
		char s[32];

		long l1 = 0;
		long l2 = 0;
		long long x = v1;

		l1 = 0;
		l2 = 0;
		ll = ll | (x<<52); 
		cout << ll << endl;
		itoa(v1,s,2);

		printf("v1:%s\n",s);
		l1 = l1 | (ll >> 32);
		l2 = l2 | ll;
		ltoa(l1,s,2);
		printf("ll:%s  ",s);
		ltoa(l2,s,2);
		printf("%s\n",s);

		x = v2;
		l1 = 0;
		l2 = 0;
		ll = ll | (x<<40); 
		cout << ll << endl;
		itoa(v2,s,2);
		printf("v2:%s\n",s);
		l1 = l1 | (ll >> 32);
		l2 = l2 | ll;
		ltoa(l1,s,2);
		printf("ll:%s  ",s);
		ltoa(l2,s,2);
		printf("%s\n",s);

		x = v3;
		l1 = 0;
		l2 = 0;
		ll = ll | (x<<28); 
		cout << ll << endl;
		itoa(v3,s,2);
		printf("v3:%s\n",s);
		l1 = l1 | (ll >> 32);
		l2 = l2 | ll;
		ltoa(l1,s,2);
		printf("ll:%s  ",s);
		ltoa(l2,s,2);
		printf("%s\n",s);
	}

	void Logo_SingletonTest()
	{
		//测试用c++模板方式复用单例已经相应的日志类
		Singleton<LogBase>::getInstance()->Loginfo();
		Singleton<LogBase>::getInstance()->setMinLogLevel(LEVEL_ALL);
		Singleton<LogBase>::getInstance()->setLogInfo(LEVEL_ALL,__FILE__,__LINE__);
		Singleton<LogBase>::getInstance()->writeLog("hello motel");
		//测试用宏实现的单件模式
		LOG_SET_MIN_LEVEL(LEVEL_ALL);
		
	}
	void Logo_BOFTest()
	{		
		Logo_get_visual_vocabularySIFT("visual_vocabulary.txt", 20000, 200, "E:\\FlickrLogos32\\FlickrLogos\\classes\\", "trainingset.txt");
	}
}
