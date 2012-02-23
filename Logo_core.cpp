#include "stdafx.h"
#include "Logo_core.h"
#include "Logo_common.h"
#include "Logo_util.h"
#include "Logo_log.h"

namespace LogRec
{


	//在两种图片上显示找到的三角形的匹配情况
	void ShowMatchResultOfSig(std::string sfileA,std::string sfileB,std::vector<LogRec::LogoTri> &lhitA,std::vector<LogRec::LogoTri> &lhitB)
	{
		const char* fileA = sfileA.c_str();
		const char* fileB = sfileB.c_str();

		IplImage* ImgA = cvLoadImage( fileA, CV_LOAD_IMAGE_COLOR );
		IplImage* ImgB = cvLoadImage( fileB, CV_LOAD_IMAGE_COLOR );

		if ( !ImgA || !ImgB )
		{
			//fprintf(stderr,"load img file failed!\n");
			LogRec::LOG_LEVEL_ERROR;
			LogRec::PRINT_LOG_MESSAGE("LOAD IMG %s AND %s FAIL", ImgA, ImgB);
			return;
		}

		IplImage* CImgA = cvCreateImage(cvGetSize(ImgA),8,3);
		IplImage* CImgB = cvCreateImage(cvGetSize(ImgB),8,3);

		cvSetImageROI(CImgA,cvRect(0,0,ImgA->width,ImgA->height));
		cvCopy(ImgA,CImgA);
		cvResetImageROI(CImgA);
		cvSetImageROI(CImgB,cvRect(0,0,ImgB->width,ImgB->height));
		cvCopy(ImgB,CImgB);
		cvResetImageROI(CImgB);
		static CvScalar colors[] = 
		{
			{{0,0,255}},
			{{0,128,255}},
			{{0,255,255}},
			{{0,255,0}},
			{{255,128,0}},
			{{255,255,0}},
			{{255,0,0}},
			{{255,0,255}},
			{{255,255,255}}
		};

			
		for (int i = 0; i < lhitA.size(); i++)
		{
			cvShowImage("ImgA",CImgA);
			cvShowImage("ImgB",CImgB);
			CvPoint2D32f PA1 = cvPoint2D32f( lhitA[i].m_sEdge.m_slrPointA.m_ix,lhitA[i].m_sEdge.m_slrPointA.m_iy );
			CvPoint2D32f PA2 = cvPoint2D32f( lhitA[i].m_sEdge.m_slrPointB.m_ix,lhitA[i].m_sEdge.m_slrPointB.m_iy );
			CvPoint2D32f PA3 = cvPoint2D32f( lhitA[i].m_sPoint.m_ix,lhitA[i].m_sPoint.m_iy );

			CvPoint2D32f PB1 = cvPoint2D32f( lhitB[i].m_sEdge.m_slrPointA.m_ix, lhitB[i].m_sEdge.m_slrPointA.m_iy );
			CvPoint2D32f PB2 = cvPoint2D32f( lhitB[i].m_sEdge.m_slrPointB.m_ix, lhitB[i].m_sEdge.m_slrPointB.m_iy );
			CvPoint2D32f PB3 = cvPoint2D32f( lhitB[i].m_sPoint.m_ix, lhitB[i].m_sPoint.m_iy );

			double tria = std::abs(cvTriangleArea(PA1,PA2,PA3));
			double trib = std::abs(cvTriangleArea(PB1,PB2,PB3));
			if((tria < 42000.0) && (trib < 42000.0))
			{
				//画A图形
				cvLine(CImgA,cvPoint(lhitA[i].m_sEdge.m_slrPointA.m_ix,lhitA[i].m_sEdge.m_slrPointA.m_iy),
					cvPoint(lhitA[i].m_sEdge.m_slrPointB.m_ix,lhitA[i].m_sEdge.m_slrPointB.m_iy),colors[i%8]);
				cvLine(CImgA,cvPoint(lhitA[i].m_sEdge.m_slrPointA.m_ix,lhitA[i].m_sEdge.m_slrPointA.m_iy),
					cvPoint(lhitA[i].m_sPoint.m_ix,lhitA[i].m_sPoint.m_iy),colors[i%8]);
				cvLine(CImgA,cvPoint(lhitA[i].m_sEdge.m_slrPointB.m_ix,lhitA[i].m_sEdge.m_slrPointB.m_iy),
					cvPoint(lhitA[i].m_sPoint.m_ix,lhitA[i].m_sPoint.m_iy),colors[i%8]);

				//画B图像
				cvLine(CImgB,cvPoint(lhitB[i].m_sEdge.m_slrPointA.m_ix,lhitB[i].m_sEdge.m_slrPointA.m_iy),
					cvPoint(lhitB[i].m_sEdge.m_slrPointB.m_ix,lhitB[i].m_sEdge.m_slrPointB.m_iy),colors[i%8]);
				cvLine(CImgB,cvPoint(lhitB[i].m_sEdge.m_slrPointA.m_ix,lhitB[i].m_sEdge.m_slrPointA.m_iy),
					cvPoint(lhitB[i].m_sPoint.m_ix,lhitB[i].m_sPoint.m_iy),colors[i%8]);
				cvLine(CImgB,cvPoint(lhitB[i].m_sEdge.m_slrPointB.m_ix,lhitB[i].m_sEdge.m_slrPointB.m_iy),
					cvPoint(lhitB[i].m_sPoint.m_ix,lhitB[i].m_sPoint.m_iy),colors[i%8]);
			}
			cvShowImage("ImgA",CImgA);
			cvShowImage("ImgB",CImgB);

			if( cvWaitKey( 2000 ) >= 0 )
					break;
		}

		cvWaitKey(1);
		cvDestroyWindow("ImgA");
		cvDestroyWindow("ImgB");
	}


	void CTriTrain::GetCandidateSignature( std::list<LogRec::LogoSLREdge> &AEdgeSet,std::list<LogRec::LogoSLREdge> &BEdgeSet,std::list<LogRec::LogoSLRPoint> &VSetA,std::list<LogRec::LogoSLRPoint> &VSetB, 
		std::map<long long, long long> &llmap, std::map<long long, LogRec::LogoTri> &llTTmap,std::list<LogRec::LogoTri> &ltriA,std::list<LogRec::LogoTri> &ltriB)
	{
		//找在图像A特征点上所有符合约束条件的三角形，并生成签名
		for (std::list<LogRec::LogoSLRPoint>::iterator vitr = VSetA.begin();
				vitr != VSetA.end(); vitr++)
		{
			for	(std::list<LogRec::LogoSLREdge>::iterator eitr = AEdgeSet.begin();
				eitr != AEdgeSet.end(); eitr++)
			{			
				LogRec::LogoTri lsltri(*eitr,*vitr);
				//****约束条件1：组成三角形的三个特征点不能有相同的label;
				//****约束条件2：三条边的比例范围在[1/3,3]	
				//****约束条件3: 最短边长大于5 pixel
				//****约束条件4: 最小角度大于20°
				//****约束条件5：最大面积不超过20000 pixel²
				if (!LogRec::IsTriHasSameLabel(lsltri) && LogRec::IsInProportion(lsltri)
					&& LogRec::IsLargerThanShortestLen(lsltri,m_gParams.m_eMin))					
				{
					LogRec::LogoGetTtriAngles(lsltri);
					if (IsLargeThanSmallestAngle(lsltri,m_gParams.m_triMinAngle) && (LogRec::LogoGetTriangleArea(lsltri) < m_gParams.m_triMaxArea))
					{
						LogRec::LogoGetTtiReOrientation(lsltri);
						//三角形三个内角和三个特征点相对角度的矢量化的bin size分别为5和10
						//每个三角形的六个签名
						//ABC
						LogRec::LogoPackTriInfoToSignature(lsltri,m_gParams.m_binSizeOfTriDir,m_gParams.m_binSizeOfOriDir);
						ltriA.push_back(lsltri);

						LogRec::LogoSLREdge eBC(lsltri.m_sEdge.m_slrPointB,lsltri.m_sPoint);
						LogRec::LogoTri ltr1(eBC,lsltri.m_sEdge.m_slrPointA);
						LogRec::LogoGetTtriAngles(ltr1);
						LogRec::LogoGetTtiReOrientation(ltr1);
						LogRec::LogoPackTriInfoToSignature(ltr1,m_gParams.m_binSizeOfTriDir,m_gParams.m_binSizeOfOriDir);
						ltriA.push_back(ltr1);

						LogRec::LogoSLREdge eCA(lsltri.m_sPoint,lsltri.m_sEdge.m_slrPointA);
						LogRec::LogoTri ltr2(eCA,lsltri.m_sEdge.m_slrPointB);
						LogRec::LogoGetTtriAngles(ltr2);
						LogRec::LogoGetTtiReOrientation(ltr2);
						LogRec::LogoPackTriInfoToSignature(ltr2,m_gParams.m_binSizeOfTriDir,m_gParams.m_binSizeOfOriDir);
						ltriA.push_back(ltr2);

						LogRec::LogoSLREdge eBA(lsltri.m_sEdge.m_slrPointB,lsltri.m_sEdge.m_slrPointA);
						LogRec::LogoTri ltr3(eBA,lsltri.m_sPoint);
						LogRec::LogoGetTtriAngles(ltr3);
						LogRec::LogoGetTtiReOrientation(ltr3);
						LogRec::LogoPackTriInfoToSignature(ltr3,m_gParams.m_binSizeOfTriDir,m_gParams.m_binSizeOfOriDir);
						ltriA.push_back(ltr3);

						LogRec::LogoSLREdge eAC(lsltri.m_sEdge.m_slrPointA,lsltri.m_sPoint);
						LogRec::LogoTri ltr4(eAC,lsltri.m_sEdge.m_slrPointB);
						LogRec::LogoGetTtriAngles(ltr4);
						LogRec::LogoGetTtiReOrientation(ltr4);
						LogRec::LogoPackTriInfoToSignature(ltr4,m_gParams.m_binSizeOfTriDir,m_gParams.m_binSizeOfOriDir);
						ltriA.push_back(ltr4);

						LogRec::LogoSLREdge eCB(lsltri.m_sPoint,lsltri.m_sEdge.m_slrPointB);
						LogRec::LogoTri ltr5(eCB,lsltri.m_sEdge.m_slrPointA);
						LogRec::LogoGetTtriAngles(ltr5);
						LogRec::LogoGetTtiReOrientation(ltr5);
						LogRec::LogoPackTriInfoToSignature(ltr5,m_gParams.m_binSizeOfTriDir,m_gParams.m_binSizeOfOriDir);
						ltriA.push_back(ltr5);										
					}
				}
			}
		}

		//把签名拷贝到hash_set里面
		for (std::list<LogRec::LogoTri>::iterator ppltra = ltriA.begin();
			ppltra != ltriA.end(); ppltra++)
		{
			if (llmap.find(ppltra->m_signature) == llmap.end())
			{
				llmap[ppltra->m_signature] = ppltra->m_signature;
				llTTmap[ppltra->m_signature] = *ppltra;
			} 
			//llset.insert(ppltra->m_signature);
		}

		//处理图像B
		for (std::list<LogRec::LogoSLRPoint>::iterator vitrb = VSetB.begin();
			vitrb != VSetB.end(); vitrb++)
		{
			for	(std::list<LogRec::LogoSLREdge>::iterator eitrb = BEdgeSet.begin();
				eitrb != BEdgeSet.end(); eitrb++)
			{

				LogRec::LogoTri lsltri(*eitrb,*vitrb);
				if (!LogRec::IsTriHasSameLabel(lsltri) && LogRec::IsInProportion(lsltri)
					&& LogRec::IsLargerThanShortestLen(lsltri,m_gParams.m_eMin)) 	
				{
					LogRec::LogoGetTtriAngles(lsltri);
					if (IsLargeThanSmallestAngle(lsltri,m_gParams.m_triMinAngle) && (LogRec::LogoGetTriangleArea(lsltri) < m_gParams.m_triMaxArea))
					{
						LogRec::LogoGetTtiReOrientation(lsltri);
						LogRec::LogoPackTriInfoToSignature(lsltri,m_gParams.m_binSizeOfTriDir,m_gParams.m_binSizeOfOriDir);
						ltriB.push_back(lsltri);

						LogRec::LogoSLREdge eBC(lsltri.m_sEdge.m_slrPointB,lsltri.m_sPoint);
						LogRec::LogoTri ltr1(eBC,lsltri.m_sEdge.m_slrPointA);
						LogRec::LogoGetTtriAngles(ltr1);
						LogRec::LogoGetTtiReOrientation(ltr1);
						LogRec::LogoPackTriInfoToSignature(ltr1,m_gParams.m_binSizeOfTriDir,m_gParams.m_binSizeOfOriDir);
						ltriB.push_back(ltr1);

						LogRec::LogoSLREdge eCA(lsltri.m_sPoint,lsltri.m_sEdge.m_slrPointA);
						LogRec::LogoTri ltr2(eCA,lsltri.m_sEdge.m_slrPointB);
						LogRec::LogoGetTtriAngles(ltr2);
						LogRec::LogoGetTtiReOrientation(ltr2);
						LogRec::LogoPackTriInfoToSignature(ltr2,m_gParams.m_binSizeOfTriDir,m_gParams.m_binSizeOfOriDir);
						ltriB.push_back(ltr2);

						LogRec::LogoSLREdge eBA(lsltri.m_sEdge.m_slrPointB,lsltri.m_sEdge.m_slrPointA);
						LogRec::LogoTri ltr3(eBA,lsltri.m_sPoint);
						LogRec::LogoGetTtriAngles(ltr3);
						LogRec::LogoGetTtiReOrientation(ltr3);
						LogRec::LogoPackTriInfoToSignature(ltr3,m_gParams.m_binSizeOfTriDir,m_gParams.m_binSizeOfOriDir);
						ltriB.push_back(ltr3);

						LogRec::LogoSLREdge eAC(lsltri.m_sEdge.m_slrPointA,lsltri.m_sPoint);
						LogRec::LogoTri ltr4(eAC,lsltri.m_sEdge.m_slrPointB);
						LogRec::LogoGetTtriAngles(ltr4);
						LogRec::LogoGetTtiReOrientation(ltr4);
						LogRec::LogoPackTriInfoToSignature(ltr4,m_gParams.m_binSizeOfTriDir,m_gParams.m_binSizeOfOriDir);
						ltriB.push_back(ltr4);

						LogRec::LogoSLREdge eCB(lsltri.m_sPoint,lsltri.m_sEdge.m_slrPointB);
						LogRec::LogoTri ltr5(eCB,lsltri.m_sEdge.m_slrPointA);
						LogRec::LogoGetTtriAngles(ltr5);
						LogRec::LogoGetTtiReOrientation(ltr5);
						LogRec::LogoPackTriInfoToSignature(ltr5,m_gParams.m_binSizeOfTriDir,m_gParams.m_binSizeOfOriDir);
						ltriB.push_back(ltr5);	
					}
				}
			}
		}
	}

	void CTriTrain::GetCandidateSignatureByIndex( std::list<LogRec::LogoSLREdge> &AEdgeSet,std::list<LogRec::LogoSLREdge> &BEdgeSet,std::list<LogRec::LogoSLRPoint> &VSetA,std::list<LogRec::LogoSLRPoint> &VSetB, 
		std::set<long> &m_edgeIndex,std::set<long long> &m_triIndex,std::string fileA,std::string fileB)
	{
		//找在图像A特征点上所有符合约束条件的三角形，并生成签名
	
		std::map<long long, long long> llmap;
		std::map<long long, LogRec::LogoTri> llTTmap;
			//处理图像B
		for (std::list<LogRec::LogoSLRPoint>::iterator vitrb = VSetB.begin();
			vitrb != VSetB.end(); vitrb++)
		{
			for	(std::list<LogRec::LogoSLREdge>::iterator eitrb = BEdgeSet.begin();
				eitrb != BEdgeSet.end(); eitrb++)
			{

				LogRec::LogoTri lsltri(*eitrb,*vitrb);
				if (!LogRec::IsTriHasSameLabel(lsltri) && LogRec::IsInProportion(lsltri)
					&& LogRec::IsLargerThanShortestLen(lsltri,m_gParams.m_eMin)) 	
				{
					LogRec::LogoGetTtriAngles(lsltri);
					if (IsLargeThanSmallestAngle(lsltri,m_gParams.m_triMinAngle) && (LogRec::LogoGetTriangleArea(lsltri) < m_gParams.m_triMaxArea))
					{
						LogRec::LogoGetTtiReOrientation(lsltri);
						LogRec::LogoPackTriInfoToSignature(lsltri,m_gParams.m_binSizeOfTriDir,m_gParams.m_binSizeOfOriDir);
						//ltriB.push_back(lsltri);
						/*if (llmap.find(lsltri.m_signature) == llmap.end())
						{}*/
						llmap[lsltri.m_signature] = lsltri.m_signature;
						llTTmap[lsltri.m_signature] = lsltri;
						//llTTmap[ppltra->m_signature] = *ppltra;
					
						LogRec::LogoSLREdge eBC(lsltri.m_sEdge.m_slrPointB,lsltri.m_sPoint);
						LogRec::LogoTri ltr1(eBC,lsltri.m_sEdge.m_slrPointA);
						LogRec::LogoGetTtriAngles(ltr1);
						LogRec::LogoGetTtiReOrientation(ltr1);
						LogRec::LogoPackTriInfoToSignature(ltr1,m_gParams.m_binSizeOfTriDir,m_gParams.m_binSizeOfOriDir);
						//ltriB.push_back(ltr1);
						/*if (llmap.find(ltr1.m_signature) == llmap.end())
						{}*/
						llmap[ltr1.m_signature] = ltr1.m_signature;
						llTTmap[ltr1.m_signature] = ltr1;
							//llTTmap[ppltra->m_signature] = *ppltra;
					
						LogRec::LogoSLREdge eCA(lsltri.m_sPoint,lsltri.m_sEdge.m_slrPointA);
						LogRec::LogoTri ltr2(eCA,lsltri.m_sEdge.m_slrPointB);
						LogRec::LogoGetTtriAngles(ltr2);
						LogRec::LogoGetTtiReOrientation(ltr2);
						LogRec::LogoPackTriInfoToSignature(ltr2,m_gParams.m_binSizeOfTriDir,m_gParams.m_binSizeOfOriDir);
						/*if (llmap.find(ltr2.m_signature) == llmap.end())
						{}*/
						llmap[ltr2.m_signature] = ltr2.m_signature;
						llTTmap[ltr2.m_signature] = ltr2;
							//llTTmap[ppltra->m_signature] = *ppltra;
					

						LogRec::LogoSLREdge eBA(lsltri.m_sEdge.m_slrPointB,lsltri.m_sEdge.m_slrPointA);
						LogRec::LogoTri ltr3(eBA,lsltri.m_sPoint);
						LogRec::LogoGetTtriAngles(ltr3);
						LogRec::LogoGetTtiReOrientation(ltr3);
						LogRec::LogoPackTriInfoToSignature(ltr3,m_gParams.m_binSizeOfTriDir,m_gParams.m_binSizeOfOriDir);
						/*if (llmap.find(ltr3.m_signature) == llmap.end())
						{}*/
						llmap[ltr3.m_signature] = ltr3.m_signature;
						llTTmap[ltr3.m_signature] = ltr3;
						//llTTmap[ppltra->m_signature] = *ppltra;
					
						LogRec::LogoSLREdge eAC(lsltri.m_sEdge.m_slrPointA,lsltri.m_sPoint);
						LogRec::LogoTri ltr4(eAC,lsltri.m_sEdge.m_slrPointB);
						LogRec::LogoGetTtriAngles(ltr4);
						LogRec::LogoGetTtiReOrientation(ltr4);
						LogRec::LogoPackTriInfoToSignature(ltr4,m_gParams.m_binSizeOfTriDir,m_gParams.m_binSizeOfOriDir);
						/*if (llmap.find(ltr4.m_signature) == llmap.end())
						{}*/
						llmap[ltr4.m_signature] = ltr4.m_signature;
						llTTmap[ltr4.m_signature] = ltr4;
						//llTTmap[ppltra->m_signature] = *ppltra;

						LogRec::LogoSLREdge eCB(lsltri.m_sPoint,lsltri.m_sEdge.m_slrPointB);
						LogRec::LogoTri ltr5(eCB,lsltri.m_sEdge.m_slrPointA);
						LogRec::LogoGetTtriAngles(ltr5);
						LogRec::LogoGetTtiReOrientation(ltr5);
						LogRec::LogoPackTriInfoToSignature(ltr5,m_gParams.m_binSizeOfTriDir,m_gParams.m_binSizeOfOriDir);
						/*if (llmap.find(ltr5.m_signature) == llmap.end())
						{}*/
						llmap[ltr5.m_signature] = ltr5.m_signature;
						llTTmap[ltr5.m_signature] = ltr5;
							//llTTmap[ppltra->m_signature] = *ppltra;
						
					}
				}
			}
		}

		std::vector<LogRec::LogoTri> lista;
		std::vector<LogRec::LogoTri> listb;
		int hit = 0;
		for (std::list<LogRec::LogoSLRPoint>::iterator vitr = VSetA.begin();
				vitr != VSetA.end(); vitr++)
		{
			for	(std::list<LogRec::LogoSLREdge>::iterator eitr = AEdgeSet.begin();
				eitr != AEdgeSet.end(); eitr++)
			{			
				LogRec::LogoTri lsltri(*eitr,*vitr);
				//****约束条件1：组成三角形的三个特征点不能有相同的label;
				//****约束条件2：三条边的比例范围在[1/3,3]	
				//****约束条件3: 最短边长大于5 pixel
				//****约束条件4: 最小角度大于20°
				//****约束条件5：最大面积不超过20000 pixel²
				if (!LogRec::IsTriHasSameLabel(lsltri) && LogRec::IsInProportion(lsltri)
					&& LogRec::IsLargerThanShortestLen(lsltri,m_gParams.m_eMin))					
				{
					LogRec::LogoGetTtriAngles(lsltri);
					if (IsLargeThanSmallestAngle(lsltri,m_gParams.m_triMinAngle) && (LogRec::LogoGetTriangleArea(lsltri) < m_gParams.m_triMaxArea))
					{
						LogRec::LogoGetTtiReOrientation(lsltri);
						//三角形三个内角和三个特征点相对角度的矢量化的bin size分别为5和10
						//每个三角形的六个签名
						//ABC

						LogRec::LogoPackTriInfoToSignature(lsltri,m_gParams.m_binSizeOfTriDir,m_gParams.m_binSizeOfOriDir);
						if (llmap.find(lsltri.m_signature) != llmap.end())
						{
							lista.push_back(lsltri);
							listb.push_back(llTTmap[lsltri.m_signature]);
							m_triIndex.insert(lsltri.m_signature);
							//分别插入三条边的索引Index
							LogRec::LogoSLREdge eAB(lsltri.m_sEdge);
							LogRec::LogoGetEdgeSig(eAB,m_gParams.m_binSizeOfOriDir);
							m_edgeIndex.insert(eAB.m_edgeSig);
								//llTTmap[ppltra->m_signature] = *ppltra;
						
							LogRec::LogoSLREdge eBC(lsltri.m_sEdge.m_slrPointB,lsltri.m_sPoint);
							LogRec::LogoGetEdgeRelativeAngles(eBC);
							LogRec::LogoGetEdgeSig(eBC,m_gParams.m_binSizeOfOriDir);
							m_edgeIndex.insert(eBC.m_edgeSig);

							LogRec::LogoSLREdge eCA(lsltri.m_sPoint,lsltri.m_sEdge.m_slrPointA);
							LogRec::LogoGetEdgeRelativeAngles(eCA);
							LogRec::LogoGetEdgeSig(eCA,m_gParams.m_binSizeOfOriDir);
							m_edgeIndex.insert(eCA.m_edgeSig);

							LogRec::LogoTri ltr1(eBC,lsltri.m_sEdge.m_slrPointA);
							LogRec::LogoGetTtriAngles(ltr1);
							LogRec::LogoGetTtiReOrientation(ltr1);
							LogRec::LogoPackTriInfoToSignature(ltr1,m_gParams.m_binSizeOfTriDir,m_gParams.m_binSizeOfOriDir);
							//ltriA.push_back(ltr1);
							//if (llmap.find(ltr1.m_signature) != llmap.end())
							//{}
							m_triIndex.insert(ltr1.m_signature);
						
					
							LogRec::LogoTri ltr2(eCA,lsltri.m_sEdge.m_slrPointB);
							LogRec::LogoGetTtriAngles(ltr2);
							LogRec::LogoGetTtiReOrientation(ltr2);
							LogRec::LogoPackTriInfoToSignature(ltr2,m_gParams.m_binSizeOfTriDir,m_gParams.m_binSizeOfOriDir);
							/*if (llmap.find(ltr2.m_signature) != llmap.end())
							{}*/
							m_triIndex.insert(ltr2.m_signature);
						

							LogRec::LogoSLREdge eBA(lsltri.m_sEdge.m_slrPointB,lsltri.m_sEdge.m_slrPointA);
							LogRec::LogoTri ltr3(eBA,lsltri.m_sPoint);
							LogRec::LogoGetTtriAngles(ltr3);
							LogRec::LogoGetTtiReOrientation(ltr3);
							LogRec::LogoPackTriInfoToSignature(ltr3,m_gParams.m_binSizeOfTriDir,m_gParams.m_binSizeOfOriDir);
							/*if (llmap.find(ltr3.m_signature) != llmap.end())
							{
							
							}*/
							m_triIndex.insert(ltr3.m_signature);
							LogRec::LogoSLREdge eAC(lsltri.m_sEdge.m_slrPointA,lsltri.m_sPoint);
							LogRec::LogoTri ltr4(eAC,lsltri.m_sEdge.m_slrPointB);
							LogRec::LogoGetTtriAngles(ltr4);
							LogRec::LogoGetTtiReOrientation(ltr4);
							LogRec::LogoPackTriInfoToSignature(ltr4,m_gParams.m_binSizeOfTriDir,m_gParams.m_binSizeOfOriDir);
							/*if (llmap.find(ltr4.m_signature) != llmap.end())
							{}*/
							m_triIndex.insert(ltr4.m_signature);
						

							LogRec::LogoSLREdge eCB(lsltri.m_sPoint,lsltri.m_sEdge.m_slrPointB);
							LogRec::LogoTri ltr5(eCB,lsltri.m_sEdge.m_slrPointA);
							LogRec::LogoGetTtriAngles(ltr5);
							LogRec::LogoGetTtiReOrientation(ltr5);
							LogRec::LogoPackTriInfoToSignature(ltr5,m_gParams.m_binSizeOfTriDir,m_gParams.m_binSizeOfOriDir);
							/*if (llmap.find(ltr5.m_signature) != llmap.end())
							{}*/
							m_triIndex.insert(ltr5.m_signature);
						

							hit++;
						}
					}
				}
			}
		}
		cout << "hit:" << hit << endl;
		ShowMatchResultOfSig(fileA,fileB,lista,listb);
	}


	CTriBase::CTriBase()
	{
	}

	CTriBase::~CTriBase()
	{
	}

	CTriTrain::CTriTrain()
	{
	}

	CTriTrain::~CTriTrain()
	{
	}

	CTriAngleQuery::CTriAngleQuery()
	{
	}

	CTriAngleQuery::~CTriAngleQuery()
	{
	}

	bool CTriBase::LoadCodeBook()
	{//读取visual words vocabulary
		ifstream fin((m_sFilePath+m_valDicFile).c_str());//("flickr_logos_32_dataset_training_set_annotation.txt");

		if (!fin)
		{
			//cout << "file open error!" << __FILE__ << __LINE__ << endl;//use log in the future!
			LogRec::LOG_LEVEL_ERROR
			LogRec::PRINT_LOG_MESSAGE("failed to load codebook! ");
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
			m_visualDict.push_back(lcb);
		}
		return true;
	}

	bool CTriAngleQuery::LoadModel()
	{
		std::ifstream ifcin;
		ifcin.open((m_sFilePath+m_sModelFileList).c_str(),std::ios_base::in);

		if (!ifcin)
		{
			fprintf(stderr,"open modelfile failed!\n");
			return false;
		}

	


		std::string sClassName;
		//读入模型
		while (ifcin >> sClassName)
		{
			std::ifstream ifEdge((m_sFilePath+sClassName+"_edge.txt").c_str());
			//ifEdge.open((sClassName+"_edge.txt").c_str());
			std::ifstream ifTri((m_sFilePath+sClassName+"_tri.txt").c_str());
			//ifTri.open((sClassName+"_tri.txt").c_str());

			if (!ifEdge || !ifTri)
			{
				fprintf(stderr,"open %s failed!\n",sClassName.c_str());
				ifEdge.close();
				ifTri.close();
				continue;
			}

			std::string line;
			while (std::getline(ifEdge,line))
			{
				std::string sLogo;
				std::istringstream iStrGetter;
				iStrGetter.str(line.c_str());
				iStrGetter >> sLogo;
				long lEdgeSig;
				while (iStrGetter >> lEdgeSig)
				{
					m_mm_edgeIndex[sLogo].insert(lEdgeSig);
				}

			}
			while (std::getline(ifTri,line))
			{
				std::string sLogo;
				std::istringstream iStrGetter;
				iStrGetter.str(line.c_str());
				iStrGetter >> sLogo;
				long long llTriSig;
				while (iStrGetter >> llTriSig)
				{
					m_mm_triIndex[sLogo].insert(llTriSig);
				}
			}
			ifEdge.close();
			ifTri.close();

		}

		ifcin.close();
		//提取图片特征并同时进行矢量量化

		return true;
	}

	int CTriBase::getVisualWordCode(float fdes[])
	{
		float fmin = 0.0;
		int sig = 0;
		for (std::vector<LogRec::LogoCodeBookFormat>::iterator itr = m_visualDict.begin();
			itr != m_visualDict.end(); itr++)
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

	bool CTriAngleQuery::GetFeatureOfSinglePic()
	{
		//对单个图片的特征进行抽取，进行矢量量化，把descriptor转化为visual word
		std::string sfile = m_sFilePath+m_sQuery;
		const char* imgfile = (const char*)sfile.c_str();
		CvMemStorage * storage = cvCreateMemStorage(0);
		IplImage* image = cvLoadImage( imgfile, CV_LOAD_IMAGE_GRAYSCALE );

		if ( !image )
		{
			fprintf( stderr, "Can not laod %s \n",imgfile );
			return false;
		}

		CvSeq* imgKeypoints   = 0;
		CvSeq* imgDescriptors = 0;

		CvSURFParams params = cvSURFParams( 500,1 );

		cvExtractSURF( image, 0, &imgKeypoints, &imgDescriptors, storage, params );
	
		CvSeqReader imgReader;
		int len = ( int )( imgDescriptors->elem_size/( sizeof(float) ) );
		float* img_ptr = (float*)malloc( sizeof(float)*len );
		cvStartReadSeq( imgDescriptors, &imgReader );
		for ( int i = 0; i < imgDescriptors->total; i++ )
		{
			const float* descriptor = (const float*)imgReader.ptr;
			CV_NEXT_SEQ_ELEM( imgReader.seq->elem_size, imgReader );
			memcpy( img_ptr, descriptor, len*sizeof( float ) );
			::CvSURFPoint surfp = *(CvSURFPoint*) cvGetSeqElem ( imgKeypoints,i );
			int iVisualId = getVisualWordCode(img_ptr );
			LogRec::LogoSLRPoint lP = LogRec::logoSLRPoint( surfp, iVisualId );
			m_vPicFeature.push_back(lP);
		}
		free(img_ptr);
		return true;
	}

	bool CTriAngleQuery::GetRandomEdgesByMontCarlo()
	{//
		//FP_OFF();
		int samplingNum = 0;
		LogRec::RandomNumber randum(0);

		for (int i = 0; i < m_vPicFeature.size(); )
		{
			unsigned int x = randum.Random(m_vPicFeature.size());
			unsigned int y = randum.Random(m_vPicFeature.size());
			if (x != y)
			{
				i++;
				m_vRandEdge.push_back(LogRec::LogoSLREdge(m_vPicFeature[x],m_vPicFeature[y]));
			}
		}

		return true;
	}

	bool CTriAngleQuery::GetRandomEdgeByReserviorSample()
	{
		int totalSize = m_vPicFeature.size()*m_vPicFeature.size();

		if ( totalSize < m_gParams.m_iMontCarloDensity )
		{
			for ( int i = 0; i < m_vPicFeature.size(); i++ )
			{
				for ( int j= 0; j < m_vPicFeature.size(); j++ )
				{
					if ( i != j )
					{
						m_vRandEdge.push_back(LogRec::LogoSLREdge(m_vPicFeature[i],m_vPicFeature[j]));
					}
				}
			}
		}
		else
		{
			LogRec::RandomNumber rdom(0);
			int* sample = new int[m_gParams.m_iMontCarloDensity];
			for ( int i = 0; i < m_gParams.m_iMontCarloDensity ; i++ )
			{
				sample[i] = i+1;
			}
			for (int j = m_gParams.m_iMontCarloDensity; j < totalSize; j++)
			{
				unsigned int rd = rdom.Random(j);
				if ( rd < m_gParams.m_iMontCarloDensity )
				{
					sample[rd] = j;
				}
			}

			for ( int i = 0; i < m_gParams.m_iMontCarloDensity; i++ )
			{
				int x = (sample[i])%(m_vPicFeature.size());
				int y = (sample[i])/(m_vPicFeature.size());
				if (x == y)
				{
					m_vRandEdge.push_back( LogRec::LogoSLREdge(m_vPicFeature[x], m_vPicFeature[y+1]));
				}
				else
				{
					m_vRandEdge.push_back( LogRec::LogoSLREdge(m_vPicFeature[x], m_vPicFeature[y]) );
				}
			}
			delete []sample;
		}
		return true;
	}

	bool CTriAngleQuery::GetCandidateQueryEdge()
	{
		for (std::vector<LogRec::LogoSLREdge>::iterator itr = m_vRandEdge.begin(); itr != m_vRandEdge.end(); itr++)
		{
			LogRec::LogoGetEdgeRelativeAngles(*itr);
			LogRec::LogoGetEdgeSig(*itr,LogRec::fToi(m_gParams.m_binSizeOfOriDir));
			for (std::map<std::string, std::set<long> >::iterator mitr = m_mm_edgeIndex.begin(); mitr != m_mm_edgeIndex.end(); mitr++)
			{
				if (mitr->second.find(itr->m_edgeSig) != mitr->second.end())
				{
					m_vCandidateEdge.push_back(*itr);
					if (!IsPointAlreadyIn(m_vCandidatePoint,itr->m_slrPointA))
					{
						m_vCandidatePoint.push_back(itr->m_slrPointA);
					}
					if (!IsPointAlreadyIn(m_vCandidatePoint,itr->m_slrPointB))
					{
						m_vCandidatePoint.push_back(itr->m_slrPointB);
					}
				}
			}
		}

		return true;
	}

	bool CTriBase::IsPointAlreadyIn(std::vector<LogRec::LogoSLRPoint> &VSet,LogRec::LogoSLRPoint &Pt)
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

	bool CTriAngleQuery::GetQueryResult()
	{
		for ( std::vector<LogRec::LogoSLRPoint>::iterator itrP = m_vCandidatePoint.begin(); itrP != m_vCandidatePoint.end(); itrP++ )
		{
			for ( std::vector<LogRec::LogoSLREdge>::iterator itrE = m_vCandidateEdge.begin(); itrE != m_vCandidateEdge.end(); itrE++ )
			{
				LogRec::LogoTri logoTri( *itrE,*itrP );
				if ( !LogRec::IsTriHasSameLabel( logoTri ) )
				{
					LogRec::LogoGetTtriAngles( logoTri );
					LogRec::LogoGetTtiReOrientation( logoTri );
					LogRec::LogoPackTriInfoToSignature( logoTri,m_gParams.m_binSizeOfTriDir,m_gParams.m_binSizeOfOriDir );
					for ( std::map<std::string, std::set<long long> >::iterator itrTri = m_mm_triIndex.begin(); 
						itrTri != m_mm_triIndex.end(); itrTri++ )
					{
						if ( itrTri->second.find(logoTri.m_signature) != itrTri->second.end())
						{
							if ( m_resultCal.find(itrTri->first) == m_resultCal.end() )
							{
								m_resultCal[itrTri->first] = 1;
							}
							else
							{
								m_resultCal[itrTri->first] ++;
							}
						}
					}
				}
			}
		}
		return true;
	}

	bool CTriAngleQuery::Query(std::string sQuery)
	{
		m_sQuery = sQuery;
		LoadCodeBook();
		LoadModel();
		GetFeatureOfSinglePic();
		//LogoGetRandomEdgesByMontCarlo(vPicFeature,vRandEdge);
		GetRandomEdgeByReserviorSample();
		GetCandidateQueryEdge();
		GetQueryResult();

		std::cout << "The final static result is :" << std::endl;
		for (std::map<std::string,int>::iterator itrR = m_resultCal.begin(); itrR != m_resultCal.end(); itrR++)
		{
			std::cout << itrR->first << " : " << itrR->second << std::endl;
		}

		return true;
	}

	bool CTriTrain::ReadTrainSRLFileList()
	{
		ifstream fin((m_sFilePath+m_sTrainList).c_str());

		if (!fin)
		{
			//std::cerr << "open trainListfile failed in " << __FILE__ << __LINE__ << std::endl;
			LogRec::LOG_LEVEL_ERROR
			LogRec::PRINT_LOG_MESSAGE("failed to open trainListfile! ");
			return false;
		}

		//std::vector<LogRec::LogoCodeBookFormat> vVocabul;
		

		std::string line;

		while(std::getline(fin,line))
		{
			std::istringstream is;
			is.str(line.c_str());
			std::string sClass;
			std::string sName;
			is >> sClass;
			is >> sName;
			m_mLogoList[sClass].push_back(sName);
		}

		return true;
	}

	//去掉初始候选边中的重复边，获取用来生成三角形签名的候选边以及后续点

	void CTriTrain::GetCandidatePointAndEdge(	std::list<LogRec::LogoSLREdge> &EdgeSetA, std::list<LogRec::LogoSLREdge> &EdgeSetB,std::list<LogRec::LogoSLREdge> &AEdgeSet, 
		std::list<LogRec::LogoSLREdge> &BEdgeSet,std::list<LogRec::LogoSLRPoint> &VSetA, std::list<LogRec::LogoSLRPoint> &VSetB )
	{
		//存放顶点的hash_map
		map<int,int> AVHmap;
		map<int,int> BVHmap;
		//存放边索引的hash,用来进行加速
		map<long,long> AEdgeHmap;
		map<long,long> BEdgeHmap;
		//利用hash_map来加速，以少量的空间代价换取时间
		for	(std::list<LogRec::LogoSLREdge>::iterator eitr = EdgeSetA.begin();
			eitr != EdgeSetA.end(); eitr++)
		{
			LogRec::LogoSLREdge EA = *eitr;

			if ((LogRec::LogoGetTwoPointsDistance(EA.m_slrPointA,EA.m_slrPointB) > m_gParams.m_eMin))
				//&& (AEdgeHmap.find(EA.m_edgeSig) == AEdgeHmap.end()))
			{
				AEdgeSet.push_back(EA);
				//AEdgeHmap.insert(std::make_pair(EA.m_edgeSig,EA.m_edgeSig));

				if (AVHmap.find(EA.m_slrPointA.m_ix) == AVHmap.end())
				{
					AVHmap[EA.m_slrPointA.m_ix] = EA.m_slrPointA.m_iy;
					VSetA.push_back(EA.m_slrPointA);
				}
				if (AVHmap.find(EA.m_slrPointB.m_ix) == AVHmap.end())
				{
					AVHmap[EA.m_slrPointB.m_ix] = EA.m_slrPointB.m_iy;
					VSetA.push_back(EA.m_slrPointB);
				}
			}
		}

		for	(std::list<LogRec::LogoSLREdge>::iterator eitrb = EdgeSetB.begin();
			eitrb != EdgeSetB.end(); eitrb++)
		{
			LogRec::LogoSLREdge EB = *eitrb;
			//LogRec::LogoGetEdgeSig(EB,10);

			if ((LogRec::LogoGetTwoPointsDistance(EB.m_slrPointA,EB.m_slrPointB) > m_gParams.m_eMin))
				//&& (BEdgeHmap.find(EB.m_edgeSig) == BEdgeHmap.end()))
			{
				BEdgeSet.push_back(EB);
				//BEdgeHmap.insert(std::make_pair(EB.m_edgeSig,EB.m_edgeSig));

				if (BVHmap.find(EB.m_slrPointA.m_ix) == BVHmap.end())
				{
					BVHmap[EB.m_slrPointA.m_ix] = EB.m_slrPointA.m_iy;
					VSetB.push_back(EB.m_slrPointA);
				}
				if (BVHmap.find(EB.m_slrPointB.m_ix) == BVHmap.end())
				{
					BVHmap[EB.m_slrPointB.m_ix] = EB.m_slrPointB.m_iy;
					VSetB.push_back(EB.m_slrPointB);
				}
			}
		}
	}


	void CTriTrain::GetCandidateSignatureRelease( std::list<LogRec::LogoSLREdge> &AEdgeSet,std::list<LogRec::LogoSLREdge> &BEdgeSet,std::list<LogRec::LogoSLRPoint> &VSetA,std::list<LogRec::LogoSLRPoint> &VSetB)
	{
		//找在图像A特征点上所有符合约束条件的三角形，并生成签名
		const int MAX_TRI_NUM = 10000000;
		const int MAX_HIT_NUM = 3000;
		std::map<long long, long long> llmap;
			//处理图像B
		for (std::list<LogRec::LogoSLRPoint>::iterator vitrb = VSetB.begin();
			vitrb != VSetB.end(); vitrb++)
		{
			for	(std::list<LogRec::LogoSLREdge>::iterator eitrb = BEdgeSet.begin();
				eitrb != BEdgeSet.end(); eitrb++)
			{
			
				LogRec::LogoTri lsltri(*eitrb,*vitrb);
				if (!LogRec::IsTriHasSameLabel(lsltri) && LogRec::IsInProportion(lsltri)
					&& LogRec::IsLargerThanShortestLen(lsltri,m_gParams.m_eMin)) 	
				{
					LogRec::LogoGetTtriAngles(lsltri);
					if (IsLargeThanSmallestAngle(lsltri,m_gParams.m_triMinAngle) && (LogRec::LogoGetTriangleArea(lsltri) < m_gParams.m_triMaxArea))
					{
						LogRec::LogoGetTtiReOrientation(lsltri);
						LogRec::LogoPackTriInfoToSignature(lsltri,m_gParams.m_binSizeOfTriDir,m_gParams.m_binSizeOfOriDir);
						//ltriB.push_back(lsltri);
						/*if (llmap.find(lsltri.m_signature) == llmap.end())
						{}*/
						llmap[lsltri.m_signature] = lsltri.m_signature;
						//llTTmap[ppltra->m_signature] = *ppltra;
					
						LogRec::LogoSLREdge eBC(lsltri.m_sEdge.m_slrPointB,lsltri.m_sPoint);
						LogRec::LogoTri ltr1(eBC,lsltri.m_sEdge.m_slrPointA);
						LogRec::LogoGetTtriAngles(ltr1);
						LogRec::LogoGetTtiReOrientation(ltr1);
						LogRec::LogoPackTriInfoToSignature(ltr1,m_gParams.m_binSizeOfTriDir,m_gParams.m_binSizeOfOriDir);
						//ltriB.push_back(ltr1);
						/*if (llmap.find(ltr1.m_signature) == llmap.end())
						{}*/
						llmap[ltr1.m_signature] = ltr1.m_signature;
							//llTTmap[ppltra->m_signature] = *ppltra;
					
						LogRec::LogoSLREdge eCA(lsltri.m_sPoint,lsltri.m_sEdge.m_slrPointA);
						LogRec::LogoTri ltr2(eCA,lsltri.m_sEdge.m_slrPointB);
						LogRec::LogoGetTtriAngles(ltr2);
						LogRec::LogoGetTtiReOrientation(ltr2);
						LogRec::LogoPackTriInfoToSignature(ltr2,m_gParams.m_binSizeOfTriDir,m_gParams.m_binSizeOfOriDir);
						/*if (llmap.find(ltr2.m_signature) == llmap.end())
						{}*/
						llmap[ltr2.m_signature] = ltr2.m_signature;
							//llTTmap[ppltra->m_signature] = *ppltra;
					

						LogRec::LogoSLREdge eBA(lsltri.m_sEdge.m_slrPointB,lsltri.m_sEdge.m_slrPointA);
						LogRec::LogoTri ltr3(eBA,lsltri.m_sPoint);
						LogRec::LogoGetTtriAngles(ltr3);
						LogRec::LogoGetTtiReOrientation(ltr3);
						LogRec::LogoPackTriInfoToSignature(ltr3,m_gParams.m_binSizeOfTriDir,m_gParams.m_binSizeOfOriDir);
						/*if (llmap.find(ltr3.m_signature) == llmap.end())
						{}*/
						llmap[ltr3.m_signature] = ltr3.m_signature;
						//llTTmap[ppltra->m_signature] = *ppltra;
					
						LogRec::LogoSLREdge eAC(lsltri.m_sEdge.m_slrPointA,lsltri.m_sPoint);
						LogRec::LogoTri ltr4(eAC,lsltri.m_sEdge.m_slrPointB);
						LogRec::LogoGetTtriAngles(ltr4);
						LogRec::LogoGetTtiReOrientation(ltr4);
						LogRec::LogoPackTriInfoToSignature(ltr4,m_gParams.m_binSizeOfTriDir,m_gParams.m_binSizeOfOriDir);
						/*if (llmap.find(ltr4.m_signature) == llmap.end())
						{}*/
						llmap[ltr4.m_signature] = ltr4.m_signature;
						//llTTmap[ppltra->m_signature] = *ppltra;

						LogRec::LogoSLREdge eCB(lsltri.m_sPoint,lsltri.m_sEdge.m_slrPointB);
						LogRec::LogoTri ltr5(eCB,lsltri.m_sEdge.m_slrPointA);
						LogRec::LogoGetTtriAngles(ltr5);
						LogRec::LogoGetTtiReOrientation(ltr5);
						LogRec::LogoPackTriInfoToSignature(ltr5,m_gParams.m_binSizeOfTriDir,m_gParams.m_binSizeOfOriDir);
						/*if (llmap.find(ltr5.m_signature) == llmap.end())
						{}*/
						llmap[ltr5.m_signature] = ltr5.m_signature;
							//llTTmap[ppltra->m_signature] = *ppltra;
						
					}
					if (llmap.size() > MAX_TRI_NUM)
					{
						break;
					}
				}
			}
			if (llmap.size() > MAX_TRI_NUM)
			{
				break;
			}
		}

		int hit = 0;
		for (std::list<LogRec::LogoSLRPoint>::iterator vitr = VSetA.begin();
				vitr != VSetA.end(); vitr++)
		{
			for	(std::list<LogRec::LogoSLREdge>::iterator eitr = AEdgeSet.begin();
				eitr != AEdgeSet.end(); eitr++)
			{			
				LogRec::LogoTri lsltri(*eitr,*vitr);
				//****约束条件1：组成三角形的三个特征点不能有相同的label;
				//****约束条件2：三条边的比例范围在[1/3,3]	
				//****约束条件3: 最短边长大于5 pixel
				//****约束条件4: 最小角度大于20°
				//****约束条件5：最大面积不超过20000 pixel²
				if (!LogRec::IsTriHasSameLabel(lsltri) && LogRec::IsInProportion(lsltri)
					&& LogRec::IsLargerThanShortestLen(lsltri,m_gParams.m_eMin))					
				{
					LogRec::LogoGetTtriAngles(lsltri);
					if (IsLargeThanSmallestAngle(lsltri,m_gParams.m_triMinAngle) && (LogRec::LogoGetTriangleArea(lsltri) < m_gParams.m_triMaxArea))
					{
						LogRec::LogoGetTtiReOrientation(lsltri);
						//三角形三个内角和三个特征点相对角度的矢量化的bin size分别为5和10
						//每个三角形的六个签名
						//ABC

						if (hit > MAX_HIT_NUM)
						{
							break;
						}
						LogRec::LogoPackTriInfoToSignature(lsltri,m_gParams.m_binSizeOfTriDir,m_gParams.m_binSizeOfOriDir);
						if (llmap.find(lsltri.m_signature) != llmap.end())
						{
							m_triIndex.insert(lsltri.m_signature);
							//分别插入三条边的索引Index
							LogRec::LogoSLREdge eAB(lsltri.m_sEdge);
							LogRec::LogoGetEdgeSig(eAB,m_gParams.m_binSizeOfOriDir);
							m_edgeIndex.insert(eAB.m_edgeSig);
								//llTTmap[ppltra->m_signature] = *ppltra;
						
							LogRec::LogoSLREdge eBC(lsltri.m_sEdge.m_slrPointB,lsltri.m_sPoint);
							LogRec::LogoGetEdgeRelativeAngles(eBC);
							LogRec::LogoGetEdgeSig(eBC,m_gParams.m_binSizeOfOriDir);
							m_edgeIndex.insert(eBC.m_edgeSig);

							LogRec::LogoSLREdge eCA(lsltri.m_sPoint,lsltri.m_sEdge.m_slrPointA);
							LogRec::LogoGetEdgeRelativeAngles(eCA);
							LogRec::LogoGetEdgeSig(eCA,m_gParams.m_binSizeOfOriDir);
							m_edgeIndex.insert(eCA.m_edgeSig);

							LogRec::LogoTri ltr1(eBC,lsltri.m_sEdge.m_slrPointA);
							LogRec::LogoGetTtriAngles(ltr1);
							LogRec::LogoGetTtiReOrientation(ltr1);
							LogRec::LogoPackTriInfoToSignature(ltr1,m_gParams.m_binSizeOfTriDir,m_gParams.m_binSizeOfOriDir);
							//ltriA.push_back(ltr1);
							//if (llmap.find(ltr1.m_signature) != llmap.end())
							//{}
							m_triIndex.insert(ltr1.m_signature);
						
					
							LogRec::LogoTri ltr2(eCA,lsltri.m_sEdge.m_slrPointB);
							LogRec::LogoGetTtriAngles(ltr2);
							LogRec::LogoGetTtiReOrientation(ltr2);
							LogRec::LogoPackTriInfoToSignature(ltr2,m_gParams.m_binSizeOfTriDir,m_gParams.m_binSizeOfOriDir);
							/*if (llmap.find(ltr2.m_signature) != llmap.end())
							{}*/
							m_triIndex.insert(ltr2.m_signature);
						

							LogRec::LogoSLREdge eBA(lsltri.m_sEdge.m_slrPointB,lsltri.m_sEdge.m_slrPointA);
							LogRec::LogoTri ltr3(eBA,lsltri.m_sPoint);
							LogRec::LogoGetTtriAngles(ltr3);
							LogRec::LogoGetTtiReOrientation(ltr3);
							LogRec::LogoPackTriInfoToSignature(ltr3,m_gParams.m_binSizeOfTriDir,m_gParams.m_binSizeOfOriDir);
							/*if (llmap.find(ltr3.m_signature) != llmap.end())
							{
							
							}*/
							m_triIndex.insert(ltr3.m_signature);
							LogRec::LogoSLREdge eAC(lsltri.m_sEdge.m_slrPointA,lsltri.m_sPoint);
							LogRec::LogoTri ltr4(eAC,lsltri.m_sEdge.m_slrPointB);
							LogRec::LogoGetTtriAngles(ltr4);
							LogRec::LogoGetTtiReOrientation(ltr4);
							LogRec::LogoPackTriInfoToSignature(ltr4,m_gParams.m_binSizeOfTriDir,m_gParams.m_binSizeOfOriDir);
							/*if (llmap.find(ltr4.m_signature) != llmap.end())
							{}*/
							m_triIndex.insert(ltr4.m_signature);
						

							LogRec::LogoSLREdge eCB(lsltri.m_sPoint,lsltri.m_sEdge.m_slrPointB);
							LogRec::LogoTri ltr5(eCB,lsltri.m_sEdge.m_slrPointA);
							LogRec::LogoGetTtriAngles(ltr5);
							LogRec::LogoGetTtiReOrientation(ltr5);
							LogRec::LogoPackTriInfoToSignature(ltr5,m_gParams.m_binSizeOfTriDir,m_gParams.m_binSizeOfOriDir);
							/*if (llmap.find(ltr5.m_signature) != llmap.end())
							{}*/
							m_triIndex.insert(ltr5.m_signature);
						

							hit++;
						}
					}
				}
			}
			if (hit > MAX_HIT_NUM)
			{
				std::cout << "match number biger than 3000!" << __LINE__ << std::endl;
				break;
			}
		}
		cout << "hit:" << hit << endl;
	}

	//提高比较效率的函数对象
	struct CLogCom
	{
	public:
		CLogCom()
		{
		}
		CLogCom(LogRec::LogoSLRPoint lpt,float dis)
		: m_lpt(lpt),m_dis(dis)
		{
		}
		bool operator () (const CLogCom &C1, const CLogCom &C2)
		{
			return C1.m_dis < C2.m_dis;
		}
	public:
		LogRec::LogoSLRPoint m_lpt;
		float m_dis;
	};
	bool operator < (const CLogCom &C1, const CLogCom &C2)
	{
		return C1.m_dis < C2.m_dis;
	}

	bool FNNPoints(LogRec::LogFPtF &ptA, list<LogRec::LogoSLRPoint> &PSetB,list<LogRec::LogFPtF> &VSetB, std::list<LogRec::LogoSLRPoint> &vlist, int n)
	{
		if (n > PSetB.size())
		{
			return false;
		}
		LogRec::LOG_LEVEL_INFO;
		LogRec::PRINT_LOG_MESSAGE("NOW BEGIN FIND %d NEAREST POINTS FROM %d POINTS",n,PSetB.size());
		std::vector<CLogCom> vcomp;
		vcomp.reserve(PSetB.size()+1);
		//std::list<LogRec::LogoSLRPoint> vlist;
		std::vector<float> fv;
		fv.reserve(n);
		std::vector<LogRec::LogoSLRPoint> pv;
		pv.reserve(n);

		std::list<LogRec::LogoSLRPoint>::iterator pbitr = PSetB.begin();
		for (std::list<LogRec::LogFPtF>::iterator vbitr = VSetB.begin(); pbitr != PSetB.end() && vbitr != VSetB.end(); vbitr++,pbitr++)
		{
			vcomp.push_back(CLogCom(*pbitr,LogRec::LogoEuclidean(ptA.m_fDescriptor,vbitr->m_fDescriptor,128)));
		}
		//把前n小的放到第n个位置前
		std::nth_element(vcomp.begin(),vcomp.begin()+n-1,vcomp.end());
		//std::nth_element(vcomp.begin(),vcomp.begin()+n-1,vcomp.end(),CLogCom());
		//vlist.insert(vlist.begin(),pv.begin(),pv.end());
		for (int i = 0; i < n; i++)
		{
			vlist.push_back(vcomp[i].m_lpt);
		}
		return false;
	}

	bool CTriTrain::FindkNearstPoint(LogRec::LogFPtF &ptA, list<LogRec::LogoSLRPoint> &PSetB,list<LogRec::LogFPtF> &VSetB, std::list<LogRec::LogoSLRPoint> &vlist, int n)
	{
		if (n > PSetB.size())
		{
			return false;
		}

		//std::list<LogRec::LogoSLRPoint> vlist;
		std::vector<float> fv;
		fv.reserve(n);
		std::vector<LogRec::LogoSLRPoint> pv;
		pv.reserve(n);

		std::list<LogRec::LogoSLRPoint>::iterator pbitr = PSetB.begin();
		for (std::list<LogRec::LogFPtF>::iterator vbitr = VSetB.begin(); pbitr != PSetB.end() && vbitr != VSetB.end(); vbitr++,pbitr++)
		{
			if (pv.size() < n)
			{
				pv.push_back(*pbitr);
				fv.push_back(LogRec::LogoEuclidean(ptA.m_fDescriptor,vbitr->m_fDescriptor,128));
			}
			else
			{
				float fd = LogRec::LogoEuclidean(ptA.m_fDescriptor,vbitr->m_fDescriptor,128);
				for (int i = 0; i < n; i++)
				{
					if (fd < fv[i])
					{
						pv[i] = *pbitr;
						fv[i] = fd;
						break;
					}
				}
			}
		}

		//vlist.insert(vlist.begin(),pv.begin(),pv.end());
		for (int i = 0; i < n; i++)
		{
			vlist.push_back(pv[i]);
		}
		return false;
	}


	//根据相同的label以及边的相似度量函数得到初始的候选边
	void CTriTrain::GetInitCandidateEdge( LogPicVW &PSetA, LogPicVW &PSetB, LogPicVWFP &PVWSetA, LogPicVWFP &PVWSetB,
		  std::list<LogRec::LogoSLREdge> &EdgeSetA, std::list<LogRec::LogoSLREdge> &EdgeSetB )
	{
		//根据特征数来自适应条件每个特征点最多匹配的点数
		#define	HCOARSE	0
		int FNumOfImgA = 0;
		int FNumOfImgB = 0;
		for (LogPicVW::iterator pitr = PSetA.begin();
			pitr != PSetA.end(); pitr++)
		{
			FNumOfImgA += pitr->second.size();
			FNumOfImgB += PSetB[pitr->first].size();
		}

	#if HCOARSE

		if (FNumOfImgB < 1100)
		{
			m_gParams.m_iMaxComNum = 200;
		}
		else if (FNumOfImgB < 1600)
		{
			m_gParams.m_iMaxComNum = 150;
		}
		else if (FNumOfImgB < 2100)
		{
			m_gParams.m_iMaxComNum = 100;
		}
		else
		{
			m_gParams.m_iMaxComNum = 40;
		}
	
	#else

		if (FNumOfImgB < 50)
		{
			m_gParams.m_iMaxComNum = 50;
		}
		else if (FNumOfImgB < 100)
		{
			m_gParams.m_iMaxComNum = 40;
		}
		else if (FNumOfImgB < 200)
		{
			m_gParams.m_iMaxComNum = 30;
		}
		else if (FNumOfImgB < 400)
		{
			m_gParams.m_iMaxComNum = 20;
		}
		else if (FNumOfImgB < 600)
		{
			m_gParams.m_iMaxComNum = 30;
		}
		else if (FNumOfImgB < 800)
		{
			m_gParams.m_iMaxComNum = 20;
		}
		else if (FNumOfImgB < 1100)
		{
			m_gParams.m_iMaxComNum = 15;
		}
		else if (FNumOfImgB < 1600)
		{
			m_gParams.m_iMaxComNum = 10;
		}
		else if (FNumOfImgB < 2100)
		{
			m_gParams.m_iMaxComNum = 5;
		}
		else
		{
			m_gParams.m_iMaxComNum = 4;
		}
	#endif
		//判断是否需要优化m_gParams

		//找出B中和A中某特征点最接近的n个特征点
		//该结构存放和A中每一个点距离最近的B中n(300)个点
		//map的第一维表示A中特征点的label(visual word),第二维的链表表示A中所有具有该label值的特征点
		//链表中的链表表示与该特征点(第一维链表中的某个元素)具有相同label值且距离最近的B中的n个点
		std::map<int, std::list<std::list<LogRec::LogoSLRPoint> > > mmlist;

		std::map<int, std::list<LogRec::LogFPtF> >::iterator pvitr = PVWSetA.begin();
		
		for (LogPicVW::iterator pitr = PSetA.begin(); pitr != PSetA.end(); pitr++,pvitr++)
		{
			list<LogRec::LogFPtF>::iterator pvvitr = pvitr->second.begin();
			for(list<LogRec::LogoSLRPoint>::iterator ppitr = pitr->second.begin();
				ppitr != pitr->second.end(); ppitr++,pvvitr++)
			{
				//对A中的每一个特征点，只保留B中和其lable相同的距离最近的前m_iMaxComNum(300)个
				if (PSetB[pitr->first].size() < m_gParams.m_iMaxComNum)
				{
					//和另外一个结构同步即可
					mmlist[pitr->first].push_back(PSetB[pitr->first]);
				}
				else
				{
					std::list<LogRec::LogoSLRPoint> loglstpt; 
					//FindkNearstPoint(*pvvitr,PSetB[pitr->first],PVWSetB[pitr->first],loglstpt,m_gParams.m_iMaxComNum);
					FNNPoints(*pvvitr,PSetB[pitr->first],PVWSetB[pitr->first],loglstpt,m_gParams.m_iMaxComNum);				
					mmlist[pitr->first].push_back(loglstpt);
				}
			}
		}

		int times = 0;
		int i = 0;

		//
		std::map<int, std::list<std::list<LogRec::LogoSLRPoint> > >::iterator first_itr_mmlist = mmlist.begin();
		//PSetA存放了图片A中特征点的所有信息，第一维为特征点的label,第二维为label值相同的所有点，放在一个List中		
		//注：该list与mmlist中的list同步
		
		//第一重循环，按不同label值个数来进行遍历
		int rc = 0;
		for (LogPicVW::iterator first_itr_psetA = PSetA.begin(); (first_itr_psetA != PSetA.end()) && i < PSetA.size(); 
			first_itr_psetA++,first_itr_mmlist++)
		{
			rc++;
			//变量具有共同label的多个点集
			int j = 0;
			//第二重循环，遍历A中具有相同的某个label值的的所有点
			for(list<LogRec::LogoSLRPoint>::iterator sitr_psetA = first_itr_psetA->second.begin(); 
				sitr_psetA != first_itr_psetA->second.end() && j < first_itr_psetA->second.size(); sitr_psetA++,j++)
			{
				int k = 0;
				std::map<int, std::list<std::list<LogRec::LogoSLRPoint> > >::iterator mtt		= first_itr_mmlist;
				std::map<int, std::list<std::list<LogRec::LogoSLRPoint> > >::iterator second_itr_mmlist	= ++mtt;
				
				//第三重循环，依次遍历除了第一重循环外的其他label
				for (LogPicVW::iterator t = first_itr_psetA, spitr_psetA = ++t; spitr_psetA != PSetA.end(); spitr_psetA++,second_itr_mmlist++)
				{
					int m = 0;
					//第四重循环，遍历下一个具有共同label的的所有特征点
					for(list<LogRec::LogoSLRPoint>::iterator cspitr_psetA = spitr_psetA->second.begin();
						cspitr_psetA != spitr_psetA->second.end() && m < spitr_psetA->second.size(); cspitr_psetA++,m++)
					{
						//提前优化，两个点构成的边如果长度小于某个阀值(5pixel),则去掉
						if(LogRec::LogoGetTwoPointsDistance(*sitr_psetA,*cspitr_psetA) <= m_gParams.m_eMin)
						{
							continue;
						}

						if (m > 200)
						{
							break;
						}
						int l = 0;
						//下五、六重循环主要处理的情况：
						/**
						 *
						 *对于图片A中某两个具有不同label的特征点构成的边，找出B中和其对应的所有边
						 *
						 *方法：根据A中的某个特征点
						 *
						 **/
						for(list<LogRec::LogoSLRPoint>::iterator fist_points_itr = (first_itr_mmlist->second.begin())->begin();
							fist_points_itr != (first_itr_mmlist->second.begin())->end(); fist_points_itr++,l++)
						{
							if (l > 200)
							{
								break;
							}

							int q = 0;
							for (list<LogRec::LogoSLRPoint>::iterator second_points_itr = (second_itr_mmlist->second.begin())->begin();
							second_points_itr != (second_itr_mmlist->second.begin())->end(); second_points_itr++,q++)
							{
								if(LogRec::LogoGetTwoPointsDistance(*fist_points_itr,*second_points_itr) <= m_gParams.m_eMin)
								{
									continue;
								}

								if (q > 200)
								{
									break;
								}

								if (times%3000000 == 0)
								{
									LogRec::LOG_LEVEL_INFO;
									LogRec::PRINT_LOG_MESSAGE("CURRENT ROUND: %d",times);
								}
								times++;

								/*if ((sitr_psetA->m_uVisualWord != cspitr_psetA->m_uVisualWord) && (fist_points_itr->m_uVisualWord != second_points_itr->m_uVisualWord)
									&& (sitr_psetA->m_laplacian == fist_points_itr->m_laplacian) && (cspitr_psetA->m_laplacian == second_points_itr->m_laplacian))*/
								{
									LogRec::LogoSLREdge edgeA(*sitr_psetA,*cspitr_psetA);
									LogRec::LogoSLREdge edgeB(*fist_points_itr,*second_points_itr);
									float simEdge = LogRec::LogoGetSimEdgeScore(edgeA,edgeB,m_gParams.m_alta,m_gParams.m_sigma);
									if (simEdge > m_gParams.m_eSimThreshold)// && (!IsEdgeAlreadyIn(EdgeSet,edgeA)))
									{									
										LogRec::LogoGetEdgeSig(edgeA,m_gParams.m_binSizeOfOriDir);
										LogRec::LogoGetEdgeSig(edgeB,m_gParams.m_binSizeOfOriDir);
										/*LogRec::LogoSLREdge edgeAA(edgeA.m_slrPointB,edgeA.m_slrPointA);
										LogRec::LogoSLREdge edgeBB(edgeB.m_slrPointB,edgeB.m_slrPointA);
										float asimEdge = LogRec::LogoGetSimEdgeScore(edgeAA,edgeBB,m_gParams.m_alta,m_gParams.m_sigma);*/

										if (edgeA.m_edgeSig == edgeB.m_edgeSig)
										{
											if (simEdge > 0.9999)
											{
												std::cout << simEdge << std::endl;
											}
											EdgeSetA.push_back(edgeA);
											EdgeSetB.push_back(edgeB);	
										}
									}
								}
							}
						}
					}
				}
			}
		}//end of sixth for
		//cout << times << " round" << endl;
	}


	//得到结对的两张图片的训练结果，即边和三角形的签名索引
	void CTriTrain::ImgsPairTraining(LogoSTClass::iterator &itr, LogoSTClass::iterator &sitr, LogoFPSTClass::iterator &ditr, LogoFPSTClass::iterator &sditr, std::string className)
	{

		std::list<LogRec::LogoTri> ltriA;
		std::list<LogRec::LogoTri> ltriB;
		//用hash表来存放签名值
		//std::hash_set<long long> llset;
		std::map<long long, long long> llmap;

		//llmap的同签名值映射
		std::map<long long, LogRec::LogoTri> llTTmap;
		//std::set<long long> ltrset;

		//有相同label的点存放在一起，map的key作为特征点的label值，value为所有这样点的集合，用list来表示
		LogPicVW PSetA;
		LogPicVW PSetB;

		//用来提高精度和加速的结构
		LogPicVWFP PVWSetA;
		LogPicVWFP PVWSetB;
		
		//初始的候选边集合
		std::list<LogRec::LogoSLREdge> EdgeSetA;
		std::list<LogRec::LogoSLREdge> EdgeSetB;
		/***
		 *  找出两个特征点集合中有相同label的子集合
		**/
		//GetSubSetOfSameLabelOfPairImgs(PSetA,PSetB,fitr,sitr,PVWSetA,PVWSetB,ffitr,fsitr);

		LogPicVWFP::iterator vfmitr = ditr->second.begin();
		for (LogPicVW::iterator ffitr = itr->second.begin(); ffitr != itr->second.end(); ffitr++,vfmitr++)
		{
			if (sitr->second.find(ffitr->first) != (sitr->second.end()))
			{
				PSetA[ffitr->first] = ffitr->second;
				//PSetA;
				PVWSetA[vfmitr->first] = vfmitr->second;

				PSetB[ffitr->first] = sitr->second[ffitr->first];

				PVWSetB[vfmitr->first] = sditr->second[vfmitr->first];

				if (ffitr->second.size() > 300 || sitr->second[ffitr->first].size() > 300) 
				{
					LogRec::LOG_LEVEL_WARN;
					LogRec::PRINT_LOG_MESSAGE("SIZE OF VISUAL %d IN IMGA: %d IN IMGB:%d", ffitr->first,
						                       ffitr->second.size(),sitr->second[ffitr->first].size());
				}
			}
		}


		//找出满足条件的候选边集合,即边上对应点label相同，相对角度也相同
		GetInitCandidateEdge(PSetA,PSetB,PVWSetA,PVWSetB,EdgeSetA,EdgeSetB);
		
		LogRec::LOG_LEVEL_INFO;
		LogRec::PRINT_LOG_MESSAGE( "SIZE OF INIT CANDIDATE EDGE A:%d  B:%d",EdgeSetA.size(),EdgeSetB.size() );
		//std::cout << "initial NUM edges A: " << EdgeSetA.size() << " B:" << EdgeSetB.size() << std::endl;

		std::list<LogRec::LogoSLREdge> AEdgeSet;
		std::list<LogRec::LogoSLREdge> BEdgeSet;

		//存放对应的EdgeSet中点
		std::list<LogRec::LogoSLRPoint> VSetA;
		std::list<LogRec::LogoSLRPoint> VSetB;
		//获取候选边和边中的点
		GetCandidatePointAndEdge( EdgeSetA, EdgeSetB, AEdgeSet, BEdgeSet, VSetA, VSetB );

		LogRec::LOG_LEVEL_INFO;
		LogRec::PRINT_LOG_MESSAGE( "SIZE OF CANDIDATE EDGE A:%d  B:%d",AEdgeSet.size(),BEdgeSet.size() );
		//std::cout << "candidate NUM edges A: " << AEdgeSet.size() << " B:" << BEdgeSet.size() << std::endl;

		std::map<long long,long long> llvsig;
		//获取候选签名

	#define	TEST	1

	#if	TEST
		//当数量太大时，ltriA和ltriB很占内存,因此仅在需要进行实时图片对比是才用
		GetCandidateSignature( AEdgeSet,BEdgeSet, VSetA, VSetB,llmap,llTTmap,ltriA,ltriB);

		std::cout << " candidate signature A: " << ltriA.size() << " B:" << ltriB.size() << std::endl;
	
	
	
		std::vector<LogRec::LogoTri> lhitA;
		std::vector<LogRec::LogoTri> lhitB;

		//最后查找在图片A、B中同时存在的三角形签名，即为签名索引
		for (std::list<LogRec::LogoTri>::iterator itrB = ltriB.begin();
			itrB != ltriB.end(); itrB++)
		{
			if (llmap.find(itrB->m_signature) != llmap.end())
			{
				LogRec::LogoTri lltri = llTTmap[itrB->m_signature];
			
				LogRec::LogoGetEdgeSig(lltri.m_sEdge,m_gParams.m_binSizeOfOriDir);
				m_edgeIndex.insert(lltri.m_sEdge.m_edgeSig);
				m_triIndex.insert(lltri.m_signature);

				//
				float f1 = LogRec::LogoGetSimEdgeScore(itrB->m_sEdge,lltri.m_sEdge,m_gParams.m_alta,m_gParams.m_sigma);
				LogRec::LogoSLREdge aeBA(itrB->m_sEdge.m_slrPointB,itrB->m_sEdge.m_slrPointA);
				LogRec::LogoSLREdge beBA(lltri.m_sEdge.m_slrPointB,lltri.m_sEdge.m_slrPointA);
				float f11 = LogRec::LogoGetSimEdgeScore(aeBA,beBA,m_gParams.m_alta,m_gParams.m_sigma);

				//LogRec::LogoSLREdge aeBA();
				LogRec::LogoSLREdge aeBC(lltri.m_sEdge.m_slrPointB,lltri.m_sPoint);
				LogRec::LogoSLREdge beBC(itrB->m_sEdge.m_slrPointB,itrB->m_sPoint);
				//LogRec::LogoGetEdgeRelativeAngles(aeBC);
				//LogRec::LogoGetEdgeRelativeAngles(beBC);
				float f2 = LogRec::LogoGetSimEdgeScore(aeBC,beBC,m_gParams.m_alta,m_gParams.m_sigma);
				LogRec::LogoTri ltria2(aeBC,lltri.m_sEdge.m_slrPointA);
				LogRec::LogoTri ltrib2(beBC,itrB->m_sEdge.m_slrPointA);
		
			
				LogRec::LogoGetEdgeSig(aeBC,m_gParams.m_binSizeOfOriDir);
				LogRec::LogoGetEdgeSig(beBC,m_gParams.m_binSizeOfOriDir);
				m_edgeIndex.insert(aeBC.m_edgeSig);

				LogRec::LogoGetTtriAngles(ltria2);
				LogRec::LogoGetTtiReOrientation(ltria2);
				LogRec::LogoPackTriInfoToSignature(ltria2,m_gParams.m_binSizeOfTriDir,m_gParams.m_binSizeOfOriDir);	
				m_triIndex.insert(ltria2.m_signature);

				LogRec::LogoSLREdge aeCA(lltri.m_sEdge.m_slrPointA,lltri.m_sPoint);
				LogRec::LogoSLREdge beCA(itrB->m_sEdge.m_slrPointA,itrB->m_sPoint);
				//LogRec::LogoGetEdgeRelativeAngles(aeCA);

				//LogRec::LogoGetEdgeRelativeAngles(beCA);
				float f3 = LogRec::LogoGetSimEdgeScore(aeCA,beCA,m_gParams.m_alta,m_gParams.m_sigma);
				LogRec::LogoGetEdgeSig(aeCA,m_gParams.m_binSizeOfOriDir);
				LogRec::LogoGetEdgeSig(beCA,m_gParams.m_binSizeOfOriDir);

				LogRec::LogoTri ltria3(aeCA,lltri.m_sEdge.m_slrPointB);
				LogRec::LogoTri ltrib3(beCA,itrB->m_sEdge.m_slrPointB);
				LogRec::LogoGetEdgeSig(aeCA,m_gParams.m_binSizeOfOriDir);
				LogRec::LogoGetEdgeSig(beCA,m_gParams.m_binSizeOfOriDir);
				m_edgeIndex.insert(aeCA.m_edgeSig);

				LogRec::LogoGetTtriAngles(ltria3);
				LogRec::LogoGetTtiReOrientation(ltria3);
				LogRec::LogoPackTriInfoToSignature(ltria3,m_gParams.m_binSizeOfTriDir,m_gParams.m_binSizeOfOriDir);	
				m_triIndex.insert(ltria3.m_signature);

				if (llvsig.find(itrB->m_signature) == llvsig.end())//(f3 > 0.5 && f2 > 0.5)
				{
					llvsig[itrB->m_signature] = itrB->m_signature;
					lhitA.push_back(lltri);
					lhitB.push_back(*itrB);
				}
				//LogRec::LogoSLRPoint A(),
				//ltrset.insert(itrB->m_signature);
				//cout << itrB->m_signature << " ";
				//ofin << LogRec::LogoToString(itrB->m_signature) << " ";
			}
		}

		//图片显示匹配效果
		std::string sfileA = m_sDatasetPath+className+"\\"+itr->first;
		std::string sfileB = m_sDatasetPath+className+"\\"+sitr->first;

		ShowMatchResultOfSig(sfileA,sfileB,lhitA,lhitB);


		ofstream ofin( itr->first.substr(0, itr->first.find_first_of('.'))+"_"+sitr->first.substr(0,sitr->first.find_first_of('.'))+"_hit.txt");
		ofin << className << " ";
	
		//把匹配结果写到以图片名字命名的文本文档中
		int hit = 0;
		for (std::map<long long,long long>::iterator lseitr = llvsig.begin(); lseitr != llvsig.end(); lseitr++)
		{
			ofin << lseitr->first << " ";
			hit++;
		}
		cout << "hits:" << hit << endl; 
			
		ofin << endl << "totalhit:" << hit << endl; /**/
		ofin.close();
	#else 
		std::string sfileA = m_sFilePath+className+"\\"+itr->first;
		std::string sfileB = m_sFilePath+className+"\\"+sitr->first;
		//GetCandidateSignatureByIndex(AEdgeSet,BEdgeSet, VSetA, VSetB,m_edgeIndex,m_triIndex,sfileA,sfileB);
		GetCandidateSignatureRelease(AEdgeSet,BEdgeSet, VSetA, VSetB);
	#endif
	}

	struct CorrespondCandidatePts
	{
		
	};
	//单个数据训练完并写入文件之后进行清空
	bool CTriTrain::DataClearn()
	{
		m_edgeIndex.clear();
		m_triIndex.clear();
		return true;
	}
	//训练某一类logo
	bool CTriTrain::SingleClassTraining( std::string className,LogoSTClass &sFeature, LogoFPSTClass &sDFeature )
	{
		//结对 n(n-1)/2 times
		LogoFPSTClass::iterator ditr = sDFeature.begin();
		for (LogoSTClass::iterator itr = sFeature.begin(); itr != sFeature.end(); itr++,ditr++)
		{
			LogoFPSTClass::iterator ft = ditr, fditr = ++ft; 
			for (LogoSTClass::iterator tt = itr,sitr = ++tt; sitr != sFeature.end() ;sitr++, fditr++)
			{
				//存放符合要求的三角形
				LogRec::LOG_LEVEL_INFO;
				LogRec::PRINT_LOG_MESSAGE("NOW TRAINING PIC: %s AND PIC: %s ",itr->first.c_str(),sitr->first.c_str());
				ImgsPairTraining(itr,sitr,ditr,fditr,className);
			}
		}

		//	边索引和三角形索引
		std::string st1 = className+"_edge.txt";
		ofstream edgeof(st1.c_str());
		edgeof << className << " ";

		for (std::set<long>::iterator eitr = m_edgeIndex.begin(); eitr != m_edgeIndex.end(); eitr++)
		{
			edgeof << *eitr << " ";
		}

		edgeof << endl;
		std::string str = className+"_tri.txt";
		ofstream triof(str.c_str());
		triof << className << " ";
		for (std::set<long long>::iterator eitr = m_triIndex.begin(); eitr != m_triIndex.end(); eitr++)
		{
			triof << *eitr << " ";
		}
		triof << endl;
		triof.close();

		return true;
	}

	bool CTriTrain::ExtractFeatures(std::string className, std::vector<std::string> &sLogoList)
	{		
		/**
		*
		*存放某一类logo的所有训练数的据特征提取信息，除了描述点以外
		*包括该类每一张图片所提取出来的除了descriptor以外的其他附加信息
		*
		**/
		LogoSTClass sFeature;
		
		//结构同上，但只有描述点、签名(visual word)、和特征点坐标(descriptor)
		//作用：图片A中的每一个特征点，找出B中与之lable相同的其欧式距离最近的n个点
		LogoFPSTClass sDFeature;

		std::vector<LogRec::LogoTri> ltri;
		//存放所有logo及其相应的label的特征点
		
		for (std::vector<std::string>::iterator itr = sLogoList.begin(); itr != sLogoList.end(); itr++)
		{
			string fullNamePath = m_sDatasetPath+className+"/"+(*itr);
			//根据路径抽取图片特征点
			const char* image_filename = fullNamePath.c_str();
			Mat image = imread(fullNamePath, CV_LOAD_IMAGE_GRAYSCALE);//CV_LOAD_IMAGE_COLOR

			///######################
			if( !image.data )
			{
				LogRec::LOG_LEVEL_ERROR;
				LogRec::PRINT_LOG_MESSAGE("--(!) Error read image %s \n",itr->c_str());
				return false;
			}	
			///######################
#define _SURF_

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
			std::vector<KeyPoint> keypoints;
			detector.detect( image,keypoints );

			extractor.compute( image, keypoints, descriptors );

			int length = 128;//keypoints.size();

			//std::vector<LogRec::LogoPoint> vlp;
			//std::vector<LogRec::LogoCodeBookFormat> vDecriptors;
				//提取到矩阵里
			//关闭日志的其他信息(文件、行和时间)
			LogRec::LOG_SET_OPTION(LogRec::LOG_OPTION_INFO);
			for (int i = 0; i < descriptors.rows; i++)
			{
				LogRec::LogoSLRPoint slrp;
				LogRec::LogFPtF lfpf;
				//const float* descriptor = (const float*)img_reader.ptr;
				float* p_imgPtr = (float*)descriptors.ptr<float>(i);
				//method one
				/*for (int j = 0; j < descriptors.cols; j++)
				{
					lfpf.m_fDescriptor[j] = p_imgPtr[j];
				}*/
				memcpy(lfpf.m_fDescriptor, p_imgPtr, length*sizeof(float));

				//把特征点方向区间由(-180,180)转化到(0,360)
				if (keypoints[i].angle < 0.0)
				{
					keypoints[i].angle += 360;
				}
				CvSURFPoint cvsurf = KeyPointToSurfPoint( keypoints[i] );
				
				/*if (keypoints[i].angle > 180 || keypoints[i].angle < -180)
				{
					LogRec::PRINT_LOG_MESSAGE("CURRENT POINT ANGLE %f", keypoints[i].angle);
				}*/

				int visualWord = getVisualWordCode(p_imgPtr);

				slrp = LogRec::logoSLRPoint(cvsurf,visualWord);

				lfpf.m_ix = slrp.m_ix;
				lfpf.m_iy = slrp.m_iy;

				if (i%50 == 0)
				{
					LogRec::PRINT_LOG_MESSAGE("--THE---%d ROUND [POINTS(%f,%f) SIZE(%d) ORI(%f)]--",
						i,cvsurf.pt.x,cvsurf.pt.y,cvsurf.size,cvsurf.dir);
					/*std::cout << "---THE--- " << i << " ROUND" 
						      << "POINTS: (" <<  lfpf.m_ix << "," 
							  << lfpf.m_iy << ") " << "size:" << cvsurf.size 
							  << " ori: " << cvsurf.dir << std::endl;*/
				}	
				sFeature[*itr][visualWord].push_back(slrp);
				sDFeature[*itr][visualWord].push_back(lfpf);
			}
			LogRec::LOG_SET_OPTION(LogRec::LOG_OPTION_ALL);
		}
		SingleClassTraining( className, sFeature,  sDFeature );
		return true;
	}


	//训练每一类logo
	bool CTriTrain::TrainSRLSingleLogoClass(std::string className,
		std::vector<std::string> &sLogoList)
	{
	
		//用来加速的辅助数据结构，部分空间换取时间

		//图片A中的每一个特征点，找出B中与之lable相同的其欧式距离最近的n个点
		LogoFPSTClass fmTri;

		std::vector<LogRec::LogoTri> ltri;
		//存放所有logo及其相应的label的特征点
		std::map<std::string,std::map<int,std::list<LogRec::LogoSLRPoint> > > mmtri;

		for (std::vector<std::string>::iterator itr = sLogoList.begin(); itr != sLogoList.end(); itr++)
		{
			string fullNamePath = m_sDatasetPath+className+"/"+(*itr);
			std::vector<LogRec::LogoSLRPoint> vPts;
			//根据路径抽取图片特征点
			const char* image_filename = fullNamePath.c_str();
			CvMemStorage* storage = cvCreateMemStorage(0);

		///######################
			IplImage* image = cvLoadImage( image_filename, CV_LOAD_IMAGE_GRAYSCALE );//CV_LOAD_IMAGE_GRAYSCALE
 
			if(!image)
			{
				fprintf( stderr, "Can not load %s \n",image_filename );
				return false;
			}	
		///######################

			CvSeq *imageKeypoints = 0, *imageDescriptors = 0;

			CvSURFParams params = cvSURFParams( 500 ,1 );
			//CvSURFParams params = cvSURFParams(500, 1);
			cvExtractSURF( image, 0, &imageKeypoints, &imageDescriptors, storage, params );

			int length = (int)(imageDescriptors->elem_size/sizeof(float));
	
			CvRect rect = {0,0,image->width,image->height};

			cvReleaseImage(&image);

			std::vector<LogRec::LogoPoint> vlp;
			std::vector<LogRec::LogoCodeBookFormat> vDecriptors;
				//提取到矩阵里
			CvSeqReader img_reader;
			cvStartReadSeq( imageDescriptors, &img_reader );

			float* p_imgPtr = (float*)malloc(length*sizeof(float));

			LogRec::LOG_SET_OPTION(LogRec::LOG_OPTION_INFO);
			for (int i = 0; i < imageKeypoints->total; i++)
			{
				LogRec::LogoSLRPoint slrp;
				LogRec::LogFPtF lfpf;
				const float* descriptor = (const float*)img_reader.ptr;
				CV_NEXT_SEQ_ELEM( img_reader.seq->elem_size, img_reader );

				memcpy(p_imgPtr, descriptor, length*sizeof(float));
				memcpy(lfpf.m_fDescriptor, descriptor, length*sizeof(float));

				CvSURFPoint cvsurf = *(CvSURFPoint*)cvGetSeqElem(imageKeypoints,i);
			
				int visualWord = getVisualWordCode(p_imgPtr);

				slrp = LogRec::logoSLRPoint(cvsurf,visualWord);

				lfpf.m_ix = slrp.m_ix;
				lfpf.m_iy = slrp.m_iy;

				if (i%50 == 0)
				{
					LogRec::PRINT_LOG_MESSAGE("---THE---%d ROUND [POINTS(%f,%f) SIZE(%d) ORI(%f)] \n",i,cvsurf.pt.x,cvsurf.pt.y,cvsurf.size,cvsurf.dir);
					std::cout << "---THE--- " << i << " ROUND" 
						      << "POINTS: (" <<  lfpf.m_ix << "," 
							  << lfpf.m_iy << ") " << "size:" << cvsurf.size 
							  << " ori: " << cvsurf.dir << std::endl;/**/
				}	

				mmtri[*itr][visualWord].push_back(slrp);
				fmTri[*itr][visualWord].push_back(lfpf);
				vPts.push_back(slrp);
			}
			LogRec::LOG_SET_OPTION(LogRec::LOG_OPTION_ALL);
			cvReleaseMemStorage(&storage);
			free(p_imgPtr);

		}
		//training n(n-1)/2 times
		LogoFPSTClass::iterator fmitr = fmTri.begin();
		for (LogoSTClass::iterator fitr = mmtri.begin();
			fitr != mmtri.end(); fitr++,fmitr++)
		{
			LogoFPSTClass::iterator ft = fmitr, ffmitr = ++ft; 
			for (LogoSTClass::iterator tt = fitr,sitr = ++tt;
				sitr != mmtri.end() ;sitr++,ffmitr++)
			{
				//存放符合要求的三角形
				//
				ImgsPairTraining(fitr,sitr,fmitr,ffmitr, className);
			}
		}
		//	边索引和三角形索引
		std::string st1 = className+"_edge.txt";
		ofstream edgeof(st1.c_str());
		edgeof << className << " ";
		for (std::set<long>::iterator eitr = m_edgeIndex.begin(); eitr != m_edgeIndex.end(); eitr++)
		{
			edgeof << *eitr << " ";
		}
		edgeof << endl;
		std::string str = className+"_tri.txt";
		ofstream triof(str.c_str());
		triof << className << " ";
		for (std::set<long long>::iterator eitr = m_triIndex.begin(); eitr != m_triIndex.end(); eitr++)
		{
			triof << *eitr << " ";
		}
		triof << endl;
		triof.close();
	}

	bool CTriTrain::StartTraining()
	{
		for (std::map<std::string,std::vector<std::string> >::iterator itr = m_mLogoList.begin(); 
			itr != m_mLogoList.end(); itr++)
		{
			LogRec::LOG_LEVEL_INFO;
			LogRec::PRINT_LOG_MESSAGE("class: %s ",itr->first.c_str());
			//std::cout << "class:" << itr->first;//<< std::endl;
			for (std::vector<string>::iterator vtr = itr->second.begin(); vtr != itr->second.end(); vtr++)
			{
				LogRec::LOG_LEVEL_INFO;
				LogRec::PRINT_LOG_MESSAGE("%s ",(*vtr).c_str());
				//std::cout << *vtr << "  ";
			}
			//std::cout << endl;
			//TrainSRLSingleLogoClass( itr->first, itr->second );
			ExtractFeatures( itr->first, itr->second );
		}
		return true;
	}

	bool CTriTrain::Training()
	{
		if ( !ReadTrainSRLFileList() )
		{
			LogRec::LOG_LEVEL_ERROR
			LogRec::PRINT_LOG_MESSAGE("Read train file list failed!");
			return false;
		}
		if ( !LoadCodeBook() )
		{
			LogRec::LOG_LEVEL_ERROR
			LogRec::PRINT_LOG_MESSAGE("LoadCodeBook failed!");
			return false;
		}
		if ( !StartTraining() )
		{
			LogRec::LOG_LEVEL_ERROR
			LogRec::PRINT_LOG_MESSAGE("PairTraining failed!");
			return false;
		}
		return true;
	}

	bool CTriBase::LoadConf(std::string confFile)
	{
		LogRec::LOG_SET_MIN_LEVEL(LogRec::LEVEL_OFF);//LEVEL_ALL
	
		std::ifstream ifconf(confFile.c_str());

		if ( !ifconf )
		{
			//std::cerr << "open conf File failed! " << __FILE__ << __LINE__ << std::endl;
			LogRec::LOG_LEVEL_ERROR
			LogRec::PRINT_LOG_MESSAGE("Load Conf file error!\n");
			return false;
		}
		std::map<std::string, std::string> m_confList;	
		std::string key;
		std::string value;
		std::string sline;

		while(std::getline(ifconf,sline))
		{
			LogRec::LogoCutBlankBE(sline);
			if ( sline.find('#') == 0 )
			{
				m_confList["comments"] += sline;
			}
			else if ( ( sline.find('#') != std::string::npos ) )
			{
				std::string line = sline.substr( 0,sline.find_first_of('#') );
				key = line.substr(0,line.find_first_of('='));
				value = line.substr(line.find_first_of('=')+1);

				LogRec::LOG_LEVEL_INFO
				LogRec::PRINT_LOG_MESSAGE("KEY: %s VALUE: %s \n",key.c_str(),value.c_str());
				//std::cout << "key:" << key << " value:" << value << std::endl;
				LogRec::LogoCutBlankBE(key);
				LogRec::LogoCutBlankBE(value);
				m_confList[key] = value;
				m_confList["comments"] += sline.substr(sline.find_first_of('#'));
			}
			else
			{
				std::string line = sline;
				key = line.substr(0,line.find_first_of('='));
				value = line.substr(line.find_first_of('=')+1);
				LogRec::LogoCutBlankBE(key);
				LogRec::LogoCutBlankBE(value);
				LogRec::LOG_LEVEL_INFO
				LogRec::PRINT_LOG_MESSAGE("KEY: %s VALUE: %s \n",key.c_str(),value.c_str());				
				//std::cout << "key:" << key << " value:" << value << std::endl;
				m_confList[key] = value;
			}
			m_sFilePath		 = m_confList["conf_file_path"];
			m_valDicFile	 = m_confList["val_dict_file"];
			m_sModelFileList = m_confList["model_list"];
			m_sTrainList	 = m_confList["train_list"];
			m_sDatasetPath	 = m_confList["dataset_path"];
			//...
		}

		LogRec::LOG_SET_MIN_LEVEL(LogRec::LEVEL_ALL);//LEVEL_OFF
		return true;
	}

	CDelaunayBase::CDelaunayBase()
	{
	}
	CDelaunayBase::~CDelaunayBase()
	{
	}

	CDelaunayTrain::CDelaunayTrain()
	{
	}
	CDelaunayTrain::~CDelaunayTrain()
	{
	}
	CDelaunayQuery::CDelaunayQuery()
	{
	}
	CDelaunayQuery::~CDelaunayQuery()
	{
	}

	bool CDelaunayQuery::LoadModel()
	{
		ifstream fin((m_sFilePath+m_sModelFile).c_str());//("flickr_logos_27_dataset_training_set_annotation.txt");

		if (!fin)
		{
			//cout << "file open error!" << endl;//use log in the future!
			LogRec::LOG_LEVEL_ERROR
			LogRec::PRINT_LOG_MESSAGE("File open error!");
			return false;
		}

		string line;
		while (getline(fin,line))
		{
			//LogRec::LogoCodeBookFormat lcb;
			istringstream eleExtractor;
			eleExtractor.str(line.c_str());
	
			std::string className;
			eleExtractor >> className;

			long long sigNature;
			while(eleExtractor >> sigNature)
			{
				m_mModel[className].insert(sigNature);
			}
		}

		return true;
	}

	bool CDelaunayBase::LoadCodeBook()
	{//读取visual words vocabulary
		ifstream fin((m_sFilePath+m_valDicFile).c_str());//("flickr_logos_32_dataset_training_set_annotation.txt");

		if (!fin)
		{
			//cout << "file open error!" << __FILE__ << __LINE__ << endl;//use log in the future!
			LogRec::LOG_LEVEL_ERROR
			LogRec::PRINT_LOG_MESSAGE("File open error!");
			return false;
		}

		string line;
		int iSig = 1000;
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
			m_visualDict.push_back(lcb);
		}
		return true;
	}

	bool CDelaunayBase::getTotalVisualWordCode(std::vector<LogRec::LogoCodeBookFormat> &vDecriptors)
	{
		int sig = 0;
		std::vector<float> vFm(vDecriptors.size(),0.0);
		for (std::vector<LogRec::LogoCodeBookFormat>::iterator itr = m_visualDict.begin();
			itr != m_visualDict.end(); itr++)
		{
			//for (std::vector<LogRec::LogoCodeBookFormat>::iterator itr1 = vDecriptors.begin();itr1 != vDecriptors.end(); itr1++)
			for (int i = 0; i < vDecriptors.size(); i++)
			{
				float ft = LogRec::LogoEuclidean(vDecriptors[i].m_fDescriptor,itr->m_fDescriptor,128);
				//float ft = LogRec::LogoManhaton(vDecriptors[i].m_fDescriptor,itr->m_fDescriptor,128);
				if (vFm[i] <= ft)
				{
					vFm[i] = ft;
					vDecriptors[i].m_iSig = itr->m_iSig;
				}
			}
		}
		return true;
	}
	void CDelaunayBase::logo_single_denaulay(CvRect &rect, CvSubdiv2D* subdiv, std::vector<LogRec::LogoTriangle> &vLtri,
		std::vector<LogRec::LogoPoint> &vlp, float triArea)
	{
		//#####################

		CvSeqReader reader;
		cvStartReadSeq( (CvSeq*)(subdiv->edges), &reader, 0 );

		int elem_size = subdiv->edges->elem_size;

		std::vector<CvSubdiv2DEdge> edge_org;

		for (int j = 0; j < subdiv->edges->total; j++)
		{
			CvQuadEdge2D* edge = (CvQuadEdge2D*)(reader.ptr);
	//
	#if 0
			if (j%50 == 0)
			{
				std::cout << "the " << j << " round" << std::endl;
			}
	#endif
			if (CV_IS_SET_ELEM( edge ))
			{
				CvSubdiv2DEdge e = (CvSubdiv2DEdge)edge;
				int t_round = 0;
				while (t_round <= 1)
				{
					CvSubdiv2DEdge t;
					if (t_round == 0)
					{
						t = e;
					}
					else
					{
						t = cvSubdiv2DRotateEdge( e, 3 );
					}

					t_round++;
					CvPoint buf[3];
					int iPointNum = 3;
					//cout << j << " edges :" << t << endl; 
					int k = 0;
					for (; k < iPointNum; k++)
					{
						CvSubdiv2DPoint* pt = cvSubdiv2DEdgeOrg( t );
						if (!pt)
						{
							break;
						}
						buf[k] = cvPoint( cvRound(pt->pt.x), cvRound(pt->pt.y) );
						t = cvSubdiv2DGetEdge( t, CV_NEXT_AROUND_LEFT );
					}

					if (k == iPointNum)
					{
						LogRec::LogoPoint AP[3];
						AP[0].m_ix = buf[0].x;
						AP[0].m_iy = buf[0].y;
						AP[1].m_ix = buf[1].x;
						AP[1].m_iy = buf[1].y;
						AP[2].m_ix = buf[2].x;
						AP[2].m_iy = buf[2].y;
						LogRec::LogoTriangle ltr(AP[0],AP[1],AP[2]);
						if (!TriangleIsInRect(rect,buf))
						{
							continue;
						}
						if(!IsAlreadyInTriangles(vLtri,ltr))
						{
							CvPoint2D32f A[3];
							A[0].x = buf[0].x;
							A[0].y = buf[0].y;
							A[1].x = buf[1].x;
							A[1].y = buf[1].y;
							A[2].x = buf[2].x;
							A[2].y = buf[2].y;
							if (cvTriangleArea(A[0],A[1],A[2]) > triArea)
							{//默认10
								if (GetTriCode(vlp,ltr))
								{							
									vLtri.push_back(ltr);
								}
							}
						}				
					}
					CV_NEXT_SEQ_ELEM( elem_size, reader );
				}
			}
		}
	}
	void CDelaunayBase::multi_scale_denaulay(std::vector<LogRec::LogoTriangle> &vLtri, LogRec::LogoRect &lrect)
	{
		string fullNamePath = m_sDatasetPath+m_sQuery;

		//根据路径抽取图片特征点
		const char* image_filename = fullNamePath.c_str();
		CvMemStorage* storage_k = cvCreateMemStorage(0);

	///####################
	///使用bounding box 来限定训练区域，即在图片的某个指定矩形区域

		IplImage* src = cvLoadImage( image_filename, CV_LOAD_IMAGE_GRAYSCALE );//CV_LOAD_IMAGE_GRAYSCALE
 
		if(!src)
		{
			//fprintf( stderr, "Can not load %s \n",image_filename );
			LogRec::LOG_LEVEL_ERROR
			LogRec::PRINT_LOG_MESSAGE("Can not load %s \n",image_filename);
			return;
		}	

		//src = cvLoadImage("C:\\Users\\beyondboy\\Desktop\\logo recognition\\flickr_logos_27_dataset.tar\\flickr_logos_27_dataset\\flickr_logos_27_dataset_images\\flickr_logos_27_dataset_images\\3294282629.jpg",1);
		CvMat *dst;
		dst = cvCreateMat(lrect.m_iRight-lrect.m_iLeft,lrect.m_iBottom-lrect.m_iTop,CV_8UC1);
		CvRect a = cvRect(lrect.m_iLeft ,lrect.m_iTop ,lrect.m_iRight-lrect.m_iLeft,lrect.m_iBottom-lrect.m_iTop);
		IplImage *image;

		cvGetSubRect(src,dst,a);

		image = cvCreateImage(cvSize(lrect.m_iRight-lrect.m_iLeft,lrect.m_iBottom-lrect.m_iTop),IPL_DEPTH_8U,1);
	
		cvGetImage(dst,image);

	//######################

		CvSeq *imageKeypoints = 0, *imageDescriptors = 0;

		CvSURFParams params = cvSURFParams(500, 1);
		cvExtractSURF( image, 0, &imageKeypoints, &imageDescriptors, storage_k, params );

		int length = (int)(imageDescriptors->elem_size/sizeof(float));
	
		CvRect rect = {0,0,image->width,image->height};

		cvReleaseMat(&dst);
		cvReleaseImage(&src);
		src = NULL;

	#if 0
		//
	#endif
		LogRec::LOG_LEVEL_INFO
		LogRec::PRINT_LOG_MESSAGE("Denaulay triangulation process will begin!!!\n");
		//printf("Denaulay triangulation process will begin!!!\n");
		float minF = 0;
		float maxF = 0;
		if (imageKeypoints->total > 0)
		{
			minF = (float)((CvSURFPoint*)cvGetSeqElem(imageKeypoints,0))->size;
			maxF = (float)((CvSURFPoint*)cvGetSeqElem(imageKeypoints,0))->size;
		}

		std::vector<LogRec::LogoPoint> vlp;
		std::vector<LogRec::LogoCodeBookFormat> vDecriptors;
			//提取到矩阵里
		CvSeqReader img_reader;
		cvStartReadSeq( imageDescriptors, &img_reader );
		float* p_imgPtr = (float*)malloc(length*sizeof(float));
		for (int i = 0; i < imageKeypoints->total; i++)
		{
			const float* descriptor = (const float*)img_reader.ptr;
			CV_NEXT_SEQ_ELEM( img_reader.seq->elem_size, img_reader );

			//memcpy(p_imgPtr, descriptor, length*sizeof(float));
			LogRec::LogoCodeBookFormat lbf;
			memcpy(lbf.m_fDescriptor, descriptor, length*sizeof(float));
			vDecriptors.push_back(lbf);
			if (minF > ((CvSURFPoint*)cvGetSeqElem(imageKeypoints,i))->size)
			{
				minF = ((CvSURFPoint*)cvGetSeqElem(imageKeypoints,i))->size;
			}
			if (maxF < ((CvSURFPoint*)cvGetSeqElem(imageKeypoints,i))->size)
			{
				maxF = ((CvSURFPoint*)cvGetSeqElem(imageKeypoints,i))->size;
			}
			LogRec::LogoPoint lpp(cvRound(((CvSURFPoint*)cvGetSeqElem(imageKeypoints,i))->pt.x),cvRound(((CvSURFPoint*)cvGetSeqElem(imageKeypoints,i))->pt.y));
			//lpp.m_uVisualWord = getVisualWordCode(vCodeBook, p_imgPtr);
			if (i%50 == 0)
			{
				//std::cout << "the " << i << " round" << std::endl;
				LogRec::LOG_LEVEL_INFO
				LogRec::PRINT_LOG_MESSAGE("The %d round!\n",i);
			}	

			vlp.push_back(lpp);
			//cvSubdivDelaunay2DInsert(subdiv,((CvSURFPoint*)cvGetSeqElem(imageKeypoints,i))->pt);
		}
		free(p_imgPtr);

		getTotalVisualWordCode(vDecriptors);
		if (vlp.size() != vDecriptors.size())
		{
			//cout << "TTTTTTTTTTTTT getcode error!" << endl;
			LogRec::LOG_LEVEL_WARN
			LogRec::PRINT_LOG_MESSAGE("Getcode error!\n");
		}
		else
		{
			for (int i = 0; i < vlp.size(); i++)
			{
				vlp[i].m_uVisualWord = vDecriptors[i].m_iSig;
			}
		}

		float sigma = minF;
		float w = 2.5;
		float t = 2;

		//#######################
		while (sigma < maxF)
		{
			CvMemStorage* storage;
			CvSubdiv2D* subdiv;

			storage = cvCreateMemStorage(0);

			subdiv = cvCreateSubdiv2D( CV_SEQ_KIND_SUBDIV2D, sizeof(*subdiv),
									   sizeof(CvSubdiv2DPoint),
									   sizeof(CvQuadEdge2D),
									   storage );
			//cvReleaseSubdiv2D();
			cvInitSubdivDelaunay2D( subdiv, rect );
			for (int i = 0; i < imageKeypoints->total; i++)
			{
				if ((float)((CvSURFPoint*)cvGetSeqElem(imageKeypoints,i))->size >= sigma 
					 && (float)((CvSURFPoint*)cvGetSeqElem(imageKeypoints,i))->size < (sigma+w))
				{
					cvSubdivDelaunay2DInsert(subdiv,((CvSURFPoint*)cvGetSeqElem(imageKeypoints,i))->pt);
				}
			}
		
			logo_single_denaulay(rect,subdiv,vLtri,vlp, 10.0);
			//######################

			sigma += t;
			cvReleaseMemStorage(&storage);
		}
		//cvReleaseImage(&image);
		cvReleaseImageHeader(&image);
		cvReleaseMemStorage(&storage_k);
	}

	void CDelaunayQuery::LogoQuery(std::string sQuery)
	{

		//统计排序的结构
		//LoadConf();
		LoadCodeBook();
	
		//读取模型
		LoadModel();		

		int para = 0;
		m_sQuery = sQuery;
		//求Logo Query的三角剖分签名
		while (m_sQuery != "00")
		{	
			std::vector<LogRec::LogoRanker> lrank;
			std::vector<LogRec::LogoTriangle> vLtri;
			IplImage* src = cvLoadImage( (m_sDatasetPath+m_sQuery).c_str(), CV_LOAD_IMAGE_GRAYSCALE );//CV_LOAD_IMAGE_GRAYSCALE
			
			if ( !src )
			{
				//std::cerr << "fail to open file!" << std::endl;
				LogRec::LOG_LEVEL_ERROR
				LogRec::PRINT_LOG_MESSAGE("File open error!");
				return ;
			}

			multi_scale_denaulay(vLtri,LogRec::LogoRect(0,0,src->height,src->width));
	
			ofstream ofin("Qres.txt");

			for (std::map<std::string,std::set<long long> >::iterator mitr = m_mModel.begin(); 
				mitr != m_mModel.end(); mitr++)
			{

				LogRec::LogoRanker lr;
				lr.m_sClassName = mitr->first;
				lr.m_iCount = 0;
				//int ic1 = 0;	
				for (std::vector<LogRec::LogoTriangle>::iterator itr1 = vLtri.begin(); 
					itr1 != vLtri.end(); itr1++)
				{
					itr1->Logo_ToSignature();
					if ((mitr->second.find(itr1->m_llSignature)) != mitr->second.end())
					{
						lr.m_iCount++;
					}
				}
				ofin  << "clasName: " << lr.m_sClassName << "  count:" << lr.m_iCount << std::endl;
				lrank.push_back(lr);
			}
			if (lrank.size() > 0)
			{
				std::vector<LogRec::LogoRanker>::iterator lt = std::max_element(lrank.begin(),lrank.end(),LogRec::LogoRanker());  
				ofin  << "logo result: " << lt->m_sClassName << "  count:" << lt->m_iCount << std::endl;
				std::cout  << "logo result: " << lt->m_sClassName << "  count:" << lt->m_iCount << std::endl;
				std::cout << "\nPlease input the query logo image, end with 00" << endl;
				
			}
			std::cin >> sQuery;
		}
	}
	bool CDelaunayBase::LoadConf(std::string confFile)
	{
		
		std::ifstream ifconf(confFile.c_str());

		if ( !ifconf )
		{
			LOG_LEVEL_ERROR
			PRINT_LOG_MESSAGE("open conf File failed!\n");
			return false;
		}

		std::map<std::string, std::string> m_confList;	
		std::string key;
		std::string value;
		std::string sline;

		while(std::getline(ifconf,sline))
		{
			LogRec::LogoCutBlankBE(sline);
			if ( sline.find('#') == 0 )
			{
				m_confList["comments"] += sline;
			}
			else if ( ( sline.find('#') != std::string::npos ) )
			{
				std::string line = sline.substr( 0,sline.find_first_of('#') );
				key = line.substr(0,line.find_first_of('='));
				value = line.substr(line.find_first_of('=')+1);
				
				LOG_LEVEL_INFO
				PRINT_LOG_MESSAGE("KEY: %s  VALUE: %s \n",key.c_str(),value.c_str());//
				//std::cout << "key:" << key << " value:" << value << std::endl;
				LogRec::LogoCutBlankBE(key);
				LogRec::LogoCutBlankBE(value);
				m_confList[key] = value;
				m_confList["comments"] += sline.substr(sline.find_first_of('#'));
			}
			else
			{
				std::string line = sline;
				key = line.substr(0,line.find_first_of('='));
				value = line.substr(line.find_first_of('=')+1);
				LogRec::LogoCutBlankBE(key);
				LogRec::LogoCutBlankBE(value);

				LOG_LEVEL_INFO
				PRINT_LOG_MESSAGE("KEY: %s  VALUE: %s \n",key.c_str(),value.c_str());//

				m_confList[key] = value;
			}

			m_sFilePath		 = m_confList["delaunay_path"];
			m_valDicFile	 = m_confList["delau_dict_file"];
			m_sSrcModelFile	 = m_confList["delau_src_model"];
			m_sModelFile	 = m_confList["delau_src_model"];
			m_sDstModelFile	 = m_confList["delau_dst_model"];
			m_sTrainList	 = m_confList["delau_train_list"];
			m_sDatasetPath	 = m_confList["delau_dset_path"];
			m_sTestList		 = m_confList["delau_test_list"];
			//...
		}
		return true;
	}

	void CDelaunayTrain::logoRecProcess()
	{
	
		//总的索引和模型
		std::map<std::string,std::set<long long> > mSig;
		//统计排序的结构
		std::vector<LogRec::LogoRanker> lrank;
	
		LoadCodeBook();

		//std::string sPath = "C:\\Users\\beyondboy\\Desktop\\logo recognition\\flickr_logos_27_dataset.tar\\flickr_logos_27_dataset\\flickr_logos_27_dataset_images\\flickr_logos_27_dataset_images\\";
		//读训练集
		std::vector<LogRec::LogoTrainFileListFormat> vTrainList;

		LogRec::LogoReadTrainFileList(m_sFilePath,m_sTrainList,vTrainList);
		//开始训练
		int tround = 0;
		bool bFirst = true;
		std::string lastName;
		ofstream ofin(m_sDstModelFile.c_str());
		for (std::vector<LogRec::LogoTrainFileListFormat>::iterator itr = vTrainList.begin();
			itr != vTrainList.end(); itr++)
		{
			if (bFirst)
			{
				lastName = itr->m_sClasName;
				bFirst = false;
			}
		
			if (lastName != itr->m_sClasName)
			{
				string sline = lastName + " ";
				if (mSig.size() > 0)
				{
					for (std::set<long long>::iterator itt = (mSig.begin())->second.begin(); 
						itt != (mSig.begin())->second.end(); itt++)
					{
						string tstr;
						ostringstream ostr;
						ostr << *itt << " ";
						sline += ostr.str();
						//istringstream istr(ostr.str());
						//istr >> tstr;
					}
					lastName = itr->m_sClasName;
					ofin << sline << endl;
					mSig.clear();
				}
			}
			std::vector<LogRec::LogoTriangle> vLtri;


	#if 0
			param_multi_scale_denaulay(sPath,itr->m_sFileName,vCodeBook,vLtri,itr->m_rBoundBox,winSize,ste,triArea);
	#else
			m_sQuery = itr->m_sFileName;
			multi_scale_denaulay(vLtri,itr->m_rBoundBox);
	#endif

			for (std::vector<LogRec::LogoTriangle>::iterator sitr = vLtri.begin(); 
				sitr != vLtri.end(); sitr++)
			{
				sitr->Logo_ToSignature();
				mSig[itr->m_sClasName].insert(sitr->m_llSignature);
			
			}
			std::cout << tround << " round" << std::endl;
			tround++;
		}

	#if 0
		std::string sQ1 = "4605630935.jpg";
		std::string sQ2 = "2802471529.jpg";

		std::vector<LogRec::LogoTriangle> vLtri1;
		std::vector<LogRec::LogoTriangle> vLtri2;

		std::vector<LogRec::LogoRanker> lrank1;
		std::vector<LogRec::LogoRanker> lrank2;


		IplImage* src = cvLoadImage( (sPath+sQ1).c_str(), CV_LOAD_IMAGE_GRAYSCALE );//CV_LOAD_IMAGE_GRAYSCALE
		multi_scale_denaulay(sPath,sQ1,vCodeBook,vLtri1,LogRec::LogoRect(0,0,src->height,src->width));

		src = cvLoadImage( (sPath+sQ2).c_str(), CV_LOAD_IMAGE_GRAYSCALE );//CV_LOAD_IMAGE_GRAYSCALE
		multi_scale_denaulay(sPath,sQ2,vCodeBook,vLtri2,LogRec::LogoRect(0,0,src->height,src->width));

		for (std::vector<LogRec::LogoTriangle>::iterator itr1 = vLtri1.begin(); 
			itr1 != vLtri1.end(); itr1++)
		{
			itr1->Logo_ToSignature();
		}
		for (std::vector<LogRec::LogoTriangle>::iterator itr2 = vLtri2.begin(); 
			itr2 != vLtri2.end(); itr2++)
		{
			itr2->Logo_ToSignature();	
		}


		ofstream ofres1("res1.txt");
		ofstream ofres2("res2.txt");

		for (std::map<std::string,std::set<long long> >::iterator mitr = mSig.begin(); 
			mitr != mSig.end(); mitr++)
		{

			LogRec::LogoRanker lr1;
			LogRec::LogoRanker lr2;
			lr1.m_sClassName = mitr->first;
			lr2.m_sClassName = mitr->first;

			lr1.m_iCount = 0;
			lr2.m_iCount = 0;
			//int ic1 = 0;	
			for (std::vector<LogRec::LogoTriangle>::iterator itr1 = vLtri1.begin(); 
				itr1 != vLtri1.end(); itr1++)
			{
				if ((mitr->second.find(itr1->m_llSignature)) != mitr->second.end())
				{
					lr1.m_iCount++;
				}
			}
			for (std::vector<LogRec::LogoTriangle>::iterator itr2 = vLtri2.begin(); 
				itr2 != vLtri2.end(); itr2++)
			{
				if ((mitr->second.find(itr2->m_llSignature)) != mitr->second.end())
				{
					lr2.m_iCount++;
				}			
			}


			ofres1  << "clasName: " << lr1.m_sClassName << "  count:" << lr1.m_iCount << std::endl;
			ofres2  << "clasName: " << lr2.m_sClassName << "  count:" << lr2.m_iCount << std::endl;

			lrank1.push_back(lr1);
			lrank2.push_back(lr2);
		}





		std::vector<LogRec::LogoRanker>::iterator lt1 = std::max_element(lrank1.begin(),lrank1.end(),LogRec::LogoRanker());  
		std::vector<LogRec::LogoRanker>::iterator lt2 = std::max_element(lrank2.begin(),lrank2.end(),LogRec::LogoRanker());  	
		//LogRec::LogoRanker *r1 = 
		ofin  << "q1:" << lt1->m_sClassName << "  count:" << lt1->m_iCount << std::endl;
		ofin  << "q2:" << lt2->m_sClassName << "  count:" << lt2->m_iCount << std::endl;

		ofstream ofin1("qr1.txt");
		LogRec::LogoRanker qr1;
		string tstr = lt1->m_sClassName;
		for (std::vector<LogRec::LogoTriangle>::iterator itr1 = vLtri1.begin(); 
		itr1 != vLtri1.end(); itr1++)
		{
		
			ostringstream ostr;
			ostr << itr1->m_llSignature << " ";
			tstr += ostr.str();
			qr1.m_iCount++;

		}
		ofin1 << tstr << endl;

		std::cout << "q1:" << lt1->m_sClassName << "  count:" << lt1->m_iCount << std::endl;  
		std::cout << "q2:" << lt2->m_sClassName << "  count:" << lt2->m_iCount << std::endl;  
	#endif
	}

}
