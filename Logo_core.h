#ifndef	LOGO_CORE_H
#define	LOGO_CORE_H

#include "Logo_base.h"

namespace LogRec
{
	struct CSVset
	{
		int m_vWord;
		std::list<LogRec::LogFPtF> m_lPoints;
	};

	struct CPicFeature
	{
		std::string m_picName;		
		std::map<int, std::list<LogRec::LogFPtF>> m_picFeature;
	};

	typedef std::map<std::string, std::map<int, std::list<LogRec::LogoSLRPoint> > > LogoSTClass;
	typedef std::map<std::string, std::map<int, std::list<LogRec::LogFPtF> > > LogoFPSTClass;

	typedef std::map<int, std::list<LogRec::LogFPtF> > LogPicVWFP; 
	typedef std::map<int, std::list<LogRec::LogoSLRPoint> > LogPicVW;

	class CTriBase
	{
	public:
		CTriBase();
		~CTriBase();
		bool IsPointAlreadyIn(std::vector<LogRec::LogoSLRPoint> &VSet,LogRec::LogoSLRPoint &Pt);
		bool LoadCodeBook();
		int getVisualWordCode(float fdes[]);
		bool LoadConf(std::string confFile);
	public:
		std::string m_sFilePath;
		std::string m_sDatasetPath;
		std::string m_valDicFile;
		std::string m_sModelFileList;
		std::string m_sTrainList;
		std::map<std::string, std::string> m_confList;
		std::vector<LogRec::LogoCodeBookFormat> m_visualDict;
		LogRec::LogParams m_gParams; 
	};

	class CTriTrain : public CTriBase
	{
	public:
		CTriTrain();
		~CTriTrain();
		bool Training();
		bool ReadTrainSRLFileList();
		bool StartTraining();
		bool TrainSRLSingleLogoClass(std::string className, std::vector<std::string> &sLogoList );

		bool ExtractFeatures( std::string className, std::vector<std::string> &sLogoList );

		bool SingleClassTraining( std::string className, LogoSTClass &sFeature, LogoFPSTClass &sDFeature );
		void GetCandidatePointAndEdge(	std::list<LogRec::LogoSLREdge> &EdgeSetA, std::list<LogRec::LogoSLREdge> &EdgeSetB,std::list<LogRec::LogoSLREdge> &AEdgeSet, 
			std::list<LogRec::LogoSLREdge> &BEdgeSet,std::list<LogRec::LogoSLRPoint> &VSetA, std::list<LogRec::LogoSLRPoint> &VSetB );

		void GetCandidateSignatureRelease( std::list<LogRec::LogoSLREdge> &AEdgeSet,std::list<LogRec::LogoSLREdge> &BEdgeSet,std::list<LogRec::LogoSLRPoint> &VSetA,
			std::list<LogRec::LogoSLRPoint> &VSetB );

		bool FindkNearstPoint( LogRec::LogFPtF &ptA, list<LogRec::LogoSLRPoint> &PSetB, list<LogRec::LogFPtF> &VSetB, std::list<LogRec::LogoSLRPoint> &vlist, int n );

		void GetInitCandidateEdge( LogPicVW &PSetA, LogPicVW &PSetB, LogPicVWFP &PVWSetA, LogPicVWFP &PVWSetB, std::list<LogRec::LogoSLREdge> &EdgeSetA, std::list<LogRec::LogoSLREdge> &EdgeSetB );
		
		void ImgsPairTraining( LogoSTClass::iterator &fitr, LogoSTClass::iterator &sitr, LogoFPSTClass::iterator &ffitr, LogoFPSTClass::iterator &fsitr, std::string className );


		//test function
		void GetCandidateSignatureByIndex( std::list<LogRec::LogoSLREdge> &AEdgeSet,std::list<LogRec::LogoSLREdge> &BEdgeSet,std::list<LogRec::LogoSLRPoint> &VSetA,std::list<LogRec::LogoSLRPoint> &VSetB, 
			std::set<long> &m_edgeIndex,std::set<long long> &m_triIndex,std::string fileA,std::string fileB);

		void GetCandidateSignature( std::list<LogRec::LogoSLREdge> &AEdgeSet,std::list<LogRec::LogoSLREdge> &BEdgeSet,std::list<LogRec::LogoSLRPoint> &VSetA,std::list<LogRec::LogoSLRPoint> &VSetB, 
			std::map<long long, long long> &llmap, std::map<long long, LogRec::LogoTri> &llTTmap,std::list<LogRec::LogoTri> &ltriA,std::list<LogRec::LogoTri> &ltriB);
		
		bool DataClearn();
	public:
		std::map<std::string,std::vector<std::string> > m_mLogoList;
		std::set<long> m_edgeIndex;
		std::set<long long> m_triIndex;
	};


	class CTriAngleQuery : public CTriBase
	{//Test and Validatation
	public:
		CTriAngleQuery();
		~CTriAngleQuery();
		bool Query(std::string sQuery);
		bool GetQueryResult();

		bool GetCandidateQueryEdge();

		bool LoadModel();
		bool GetFeatureOfSinglePic();

		bool GetRandomEdgesByMontCarlo();
		bool GetRandomEdgeByReserviorSample();

	public:

		//边索引
		std::map<std::string, std::set<long> > m_mm_edgeIndex;
		//三角形索引
		std::map<std::string, std::set<long long> > m_mm_triIndex;
		//图片特征点
		std::vector<LogRec::LogoSLRPoint> m_vPicFeature;
		//随机采样的边
		std::vector<LogRec::LogoSLREdge> m_vRandEdge;
		//候选边
		std::vector<LogRec::LogoSLREdge> m_vCandidateEdge;
		//候选点
		std::vector<LogRec::LogoSLRPoint> m_vCandidatePoint;
		//统计结果
		std::map<std::string,int> m_resultCal;
		//
		std::string m_sQuery;
	};


	class CDelaunayBase
	{
	public:
		CDelaunayBase();
		~CDelaunayBase();

		bool IsPointAlreadyIn(std::vector<LogRec::LogoSLRPoint> &VSet,LogRec::LogoSLRPoint &Pt);
		bool LoadCodeBook();
		int getVisualWordCode(float fdes[]);
		bool LoadConf(std::string confFile);
		bool getTotalVisualWordCode(std::vector<LogRec::LogoCodeBookFormat> &vDecriptors);
		void multi_scale_denaulay(std::vector<LogRec::LogoTriangle> &vLtri, LogRec::LogoRect &lrect);
		void logo_single_denaulay(CvRect &rect, CvSubdiv2D* subdiv, std::vector<LogRec::LogoTriangle> &vLtri,
			std::vector<LogRec::LogoPoint> &vlp, float triArea);
	public:
		std::string m_sFilePath;
		std::string m_valDicFile;
		std::string m_sTrainList;
		std::string m_sDatasetPath;
		std::string m_sModelFile;
		std::string m_sDstModelFile;
		std::string m_sSrcModelFile;
		std::string m_sTestList;
		std::map<std::string, std::string> m_confList;
		std::vector<LogRec::LogoCodeBookFormat> m_visualDict;
		LotoDelauParaSettings m_delauParas;
		std::string m_sQuery;
	};

	class CDelaunayTrain : public CDelaunayBase
	{
	public:
		CDelaunayTrain();
		~CDelaunayTrain();
		void logoRecProcess();
	};

	class CDelaunayQuery : public CDelaunayBase
	{
	public:
		CDelaunayQuery();
		~CDelaunayQuery();
		bool LoadModel();
		void LogoQuery(std::string sQuery);
	public:
	//总的索引和模型
	//std::map<std::string,std::set<long long> > mSig;
		std::map<std::string,std::set<long long> > m_mModel;

	};
}

#endif