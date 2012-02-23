
#ifndef LOGO_BASE_H
#define	LOGO_BASE_H

#include "stdafx.h"

namespace LogRec
{
	//used for random chose edges
	const unsigned long maxshort = 65535L;
	const unsigned long multiplier = 1194211693L;
	const unsigned long adder = 12345L;

	class RandomNumber
	{
	private:
		unsigned long randSeed;

	public:
		RandomNumber(unsigned long s = 0)
		{
			if (s == 0)
			{
				randSeed = time(0);
			}
			else
			{
				randSeed = s;
			}
		}

		unsigned short Random(unsigned long n)
		{
			randSeed = multiplier * randSeed + adder;
			return (unsigned short)((randSeed >> 16) % n);
		}
		double fRandom(void)
		{
			return Random(maxshort)/double(maxshort);
		}
	};

	struct LogFPtF
	{
		int m_ix;
		int m_iy;
		float m_fDescriptor[128];//128 dimension descriptor of a single feature
		int m_iSig;//4 digit (1000,...,9999)
	};

	struct LotoDelauParaSettings
	{

	};
	struct LogParams
	{//实验参数设置
		LogParams()
		: m_alta(1.0), m_sigma(8.0), m_eSimThreshold(0.9), m_binSizeOfTriDir(5.0),
		  m_binSizeOfOriDir(10.0),m_triMinAngle(15.0), m_eMin(5.0), m_triMaxArea(30000.0),
		  m_minProportion(1.0/3.0), m_maxProportion(3.0), m_iMaxComNum(8), m_iMontCarloDensity(20000)
		{

		}
		LogParams( float alta, float sigma,float eSimThreshold,float binSizeOfTriDir,
			float binSizeOfOriDir,float triMinAngle, float eMin, float triMaxArea,
			float minProportion, float maxProportion ,int iMaxComNum, int iMontCarloDensity)
		: m_alta(alta), m_sigma(sigma), m_eSimThreshold(eSimThreshold), m_binSizeOfTriDir(binSizeOfTriDir),
		  m_binSizeOfOriDir(binSizeOfOriDir),m_triMinAngle(triMinAngle), m_eMin(eMin), m_triMaxArea(triMaxArea),
		  m_minProportion(minProportion), m_maxProportion(maxProportion), m_iMaxComNum(iMaxComNum),m_iMontCarloDensity(iMontCarloDensity)
		{

		}

		float m_alta;
		float m_sigma;
		float m_eSimThreshold;
		float m_binSizeOfTriDir;
		float m_binSizeOfOriDir;
		float m_triMinAngle;
		float m_eMin;
		float m_triMaxArea;
		float m_minProportion;
		float m_maxProportion;
		int m_iMaxComNum;
		int m_iMontCarloDensity;
		//int m_iDimOfDescriptor;
	};

	struct LogoSLRPoint
	{
		LogoSLRPoint()
		{
		}
		LogoSLRPoint(int x, int y)
		: m_ix(x), m_iy(y)
		{
		}

		bool operator == (LogoSLRPoint &olp)
		{
			if (olp.m_ix == m_ix && olp.m_iy == m_iy)
			{
				return true;
			}
			return false;
		}

		void operator = (const LogoSLRPoint &lp)
		{
			m_ix = lp.m_ix;
			m_iy = lp.m_iy;
			m_laplacian = lp.m_laplacian;
			m_size = lp.m_size;
			m_dir = lp.m_dir;
			m_hessian = lp.m_hessian;
			m_uVisualWord = lp.m_uVisualWord;
		}

		void operator = (CvSURFPoint &surfp)
		{
			//最接近的转换
			CvPoint cp = cvPointFrom32f(surfp.pt);
			m_ix = cp.x;
			m_iy = cp.y;
			m_laplacian = surfp.laplacian;
			m_size = surfp.size;
			m_dir = surfp.dir;
			m_hessian = surfp.hessian;
		}
		//add 2.18
		void operator = (KeyPoint &keypt)
		{
			CvPoint cp = cvPointFrom32f(keypt.pt);
			m_ix = cp.x;
			m_iy = cp.y;
			//m_ix = keypt.pt.x;
			//m_iy = keypt.pt.y;
			m_laplacian = 0;//keypt.octave;//?? need to be revised!
			m_size = keypt.size;
			m_dir = keypt.angle;
			m_hessian = 0;//keypt.class_id;
		}

	public:
		int m_ix;
		int m_iy;
	    int m_laplacian;
	    int m_size;
	    float m_dir;
	    float m_hessian;
		unsigned short m_uVisualWord;
	};
	struct LogoSLREdge
	{
		LogoSLREdge()
		{
		}

		LogoSLREdge(LogoSLRPoint PA, LogoSLRPoint PB)
		: m_slrPointA(PA), m_slrPointB(PB)
		{
		}

		bool operator < (LogoSLREdge &oslre)
		{
			return oslre.m_slrPointA.m_size < m_slrPointA.m_size;
		}

		bool operator == (LogoSLREdge &oslre)
		{
			return ((oslre.m_slrPointA == m_slrPointA && oslre.m_slrPointB == m_slrPointB)
				|| (oslre.m_slrPointA == m_slrPointB && oslre.m_slrPointB == m_slrPointA));
		}

		void operator = (const LogoSLREdge &oslre)
		{
			m_slrPointA = oslre.m_slrPointA;
			m_slrPointB = oslre.m_slrPointB;
			m_rAngleA = oslre.m_rAngleA;
			m_rAngleB = oslre.m_rAngleB;
			m_edgeSig = oslre.m_edgeSig;
		}

	public:
		LogoSLRPoint m_slrPointA;
		LogoSLRPoint m_slrPointB;
		float m_rAngleA;
		float m_rAngleB;
		long m_edgeSig;
	};

	struct LogoTri
	{
		LogoTri()
		{
		}
		LogoTri(LogoSLREdge &oEdge,LogoSLRPoint &oPoint)
		: m_sEdge(oEdge), m_sPoint(oPoint)
		{
		}
		bool operator = (const LogoTri &lsr)
		{
			m_sEdge = lsr.m_sEdge;
			m_sPoint = lsr.m_sPoint;
			m_thetaA = lsr.m_thetaA;
			m_thetaB = lsr.m_thetaB;
			m_thetaC = lsr.m_thetaC;
			m_rapha = lsr.m_rapha;
			m_rbeta = lsr.m_rbeta;
			m_rgama = lsr.m_rgama;
			m_signature = lsr.m_signature;	
			return true;	
		}
	public:
		LogoSLREdge  m_sEdge;
		LogoSLRPoint m_sPoint;
		float m_thetaA;
		float m_thetaB;
		float m_thetaC;
		float m_rapha;
		float m_rbeta;
		float m_rgama;
		long long m_signature;
	};

	/*struct LogoTriangle
	{
	public:
		LogoTriangle();
		LogoTriangle(LogoPoint A,LogoPoint B,LogoPoint C);
		bool Logo_ToSignature();

		bool Logo_IsPointInTriangle(LogoTriangle &ltr, LogoPoint &lpt)
		{
			if (ltr.m_A == lpt || ltr.m_B == lpt || ltr.m_C == lpt)
			{
				return true;
			}
			return false;
		}

		bool operator == (LogoTriangle &otri)
		{
			if (Logo_IsPointInTriangle(otri,m_A) && Logo_IsPointInTriangle(otri,m_B)
				&& Logo_IsPointInTriangle(otri,m_C))
			{
				return true;
			}
			return false;
		}

		bool operator = (LogoTriangle &lptr)
		{
			m_A = lptr.m_A;
			m_B = lptr.m_B;
			m_C = lptr.m_C;
			m_llSignature = lptr.m_llSignature;
		}

	public:
		LogoPoint m_A;
		LogoPoint m_B;
		LogoPoint m_C;
		long long m_llSignature;
	};*/
	
	struct LogoRect
	{
	public:
		LogoRect()
		{
		}
		LogoRect(int top, int left, int bottom, int right)
		: m_iTop(top), m_iLeft(left), 
		  m_iBottom(bottom), m_iRight(right)
		{

		}
	public:
		int m_iLeft;
		int m_iTop;
		int m_iRight;
		int m_iBottom;
	};

	struct LogoTrainFileListFormat
	{
		std::string m_sFileName;
		std::string m_sClasName;
		int	m_iSubClassType;//(1,...,6)
		LogoRect m_rBoundBox;
	};
	
	struct LogoTestFileListFormat
	{
		std::string m_sFileName;
		std::string m_sClassName;
	};

	struct LogoCodeBookFormat
	{
		float m_fDescriptor[128];//128 dimension descriptor of a single feature
		int m_iSig;//4 digit (1000,...,9999)
	};

	struct LogoRanker
	{
	public:
		bool operator () (LogoRanker &lr1, LogoRanker &lr2)
		{
			return lr1.m_iCount < lr2.m_iCount;
		}
		std::string m_sClassName;
		int m_iCount;
	};

	//hash comparor for functional objector
	struct eqll
	{
		bool operator () (const long long l1, const long long l2)
		{
			return l1 == l2;
		}
	};
	
	//******************************
	struct LogoPoint
	{
	public:
		LogoPoint();
		LogoPoint(int x, int y);

		bool operator == (LogoPoint &olp)
		{
			if (olp.m_ix == m_ix && olp.m_iy == m_iy)
			{
				return true;
			}
			return false;
		}
		void operator = (const LogoPoint &lp)
		{
			m_ix = lp.m_ix;
			m_iy = lp.m_iy;
			m_uVisualWord = lp.m_uVisualWord;
		}
	public:
		int m_ix;
		int m_iy;
		unsigned short m_uVisualWord;
	};

	struct LogoTriangle
	{
	public:
		LogoTriangle();
		LogoTriangle(LogoPoint A,LogoPoint B,LogoPoint C);
		bool Logo_ToSignature();

		bool Logo_IsPointInTriangle(LogoTriangle &ltr, LogoPoint &lpt)
		{
			if (ltr.m_A == lpt || ltr.m_B == lpt || ltr.m_C == lpt)
			{
				return true;
			}
			return false;
		}

		bool operator == (LogoTriangle &otri)
		{
			if (Logo_IsPointInTriangle(otri,m_A) && Logo_IsPointInTriangle(otri,m_B)
				&& Logo_IsPointInTriangle(otri,m_C))
			{
				return true;
			}
			return false;
		}

		bool operator = (const LogoTriangle &lptr)
		{
			m_A = lptr.m_A;
			m_B = lptr.m_B;
			m_C = lptr.m_C;
			m_llSignature = lptr.m_llSignature;
			return true;
		}

	public:
		LogoPoint m_A;
		LogoPoint m_B;
		LogoPoint m_C;
		long long m_llSignature;
	};



	//*******************************
	struct eqstr
	{
		bool operator () (const std::string s1, const std::string s2)
		{
			return s1 == s2;
		}
	};

	struct eqcstr
	{
		bool operator () (const char* cs1, const char* cs2)
		{
			return strcmp(cs1,cs2) == 0;
		}
	};

	struct eqpoint
	{
		bool operator () (const LogoSLREdge oslre, const LogoSLREdge oslre1) const
		{
			return oslre.m_slrPointA.m_size < oslre1.m_slrPointA.m_size;
		}
	};
}

#endif