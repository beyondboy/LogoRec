#include "stdafx.h"
#include "Logo_common.h"
#include "Logo_multiDelaunay.h"

namespace LogRec
{
	LogoPoint::LogoPoint()
	{
		LogoPoint::m_uVisualWord = 0;
	}
	LogoPoint::LogoPoint(int x, int y)
	{
		LogoPoint::m_uVisualWord = 0;
		LogoPoint::m_ix = x;
		LogoPoint::m_iy = y;
	}
	LogoTriangle::LogoTriangle()
	{
		
	}
	LogoTriangle::LogoTriangle(LogoPoint A,LogoPoint B,LogoPoint C)
		: m_A(A), m_B(B), m_C(C)
	{

	}
	bool LogoTriangle::Logo_ToSignature()
	{
		long long yum = 0;
		unsigned short uA = LogoTriangle::m_A.m_uVisualWord;
		unsigned short uB = LogoTriangle::m_B.m_uVisualWord;
		unsigned short uC = LogoTriangle::m_C.m_uVisualWord;
		//保证签名按升序组合
		if (LogoTriangle::m_A.m_uVisualWord > LogoTriangle::m_B.m_uVisualWord)
		{
			if (LogoTriangle::m_A.m_uVisualWord > LogoTriangle::m_C.m_uVisualWord)
			{
				if (LogoTriangle::m_B.m_uVisualWord > LogoTriangle::m_C.m_uVisualWord)
				{
					uA = LogoTriangle::m_C.m_uVisualWord;
					uB = LogoTriangle::m_B.m_uVisualWord;
					uC = LogoTriangle::m_A.m_uVisualWord;
				}
				else
				{
					uA = LogoTriangle::m_B.m_uVisualWord;
					uB = LogoTriangle::m_C.m_uVisualWord;
					uC = LogoTriangle::m_A.m_uVisualWord;
				}
			}
			else
			{
				uA = LogoTriangle::m_B.m_uVisualWord;
				uB = LogoTriangle::m_A.m_uVisualWord;
				uC = LogoTriangle::m_C.m_uVisualWord;
			}
		}
		else
		{
			if (LogoTriangle::m_A.m_uVisualWord < LogoTriangle::m_C.m_uVisualWord)
			{
				if (LogoTriangle::m_B.m_uVisualWord < LogoTriangle::m_C.m_uVisualWord)
				{
					uA = LogoTriangle::m_A.m_uVisualWord;
					uB = LogoTriangle::m_B.m_uVisualWord;
					uC = LogoTriangle::m_C.m_uVisualWord;
				}
				else
				{
					uA = LogoTriangle::m_A.m_uVisualWord;
					uB = LogoTriangle::m_C.m_uVisualWord;
					uC = LogoTriangle::m_B.m_uVisualWord;
				}
			}
			else
			{
				uA = LogoTriangle::m_C.m_uVisualWord;
				uB = LogoTriangle::m_A.m_uVisualWord;
				uC = LogoTriangle::m_B.m_uVisualWord;
			}
		}
		std::ostringstream os;
		os << uA << uB << uC;
		std::istringstream is(os.str());
		is >> m_llSignature;
		//string sSig = LogoToString(uA)+LogRec::LogoToString(uB)+LogRec::LogoToString(uC);
		//LogoTiangle::m_llSignature = LogRec::LogoFromString(LogoToString(uA)+LogRec::LogoToString(uB)+LogRec::LogoToString(uC),yum);
		return true;
	}

	LogoMultiScaleDelaunay::LogoMultiScaleDelaunay()
	{
	}
	LogoMultiScaleDelaunay::~LogoMultiScaleDelaunay()
	{
	}

	bool LogoMultiScaleDelaunay::Logo_IsCodeAlreadyIn(LogRec::LogoPoint &lp)
	{
		for (std::vector<LogRec::LogoPoint>::iterator itr = m_vCode.begin(); itr != m_vCode.end(); itr++)
		{
			if ((lp.m_ix = itr->m_ix) && (lp.m_iy == itr->m_iy))
			{
				return true;
			}
		}
		return false;
	}

	bool LogoMultiScaleDelaunay::Logo_InsertCode(LogRec::LogoPoint &lp)
	{
		if (Logo_IsCodeAlreadyIn(lp))
		{
			return false;
		}

		m_vCode.push_back(lp);
		return true;
	}

	unsigned LogoMultiScaleDelaunay::Logo_GetCode(LogRec::LogoPoint &lp)
	{
		for (std::vector<LogRec::LogoPoint>::iterator itr = m_vCode.begin(); itr != m_vCode.end(); itr++)
		{
			if ((lp.m_ix = itr->m_ix) && (lp.m_iy == itr->m_iy))
			{
				return itr->m_uVisualWord;
			}
		}

		return 0;
	}

	bool LogoMultiScaleDelaunay::Logo_IsTriAlreadyIn(LogRec::LogoTriangle &ltr)
	{
		for (std::vector<LogRec::LogoTriangle>::iterator itr = m_vSig.begin(); itr != m_vSig.end(); itr++)
		{
			if (*itr == ltr)
			{
				return true;
			}
		}
		return false;
	}
	bool LogoMultiScaleDelaunay::Logo_InsertTri(LogRec::LogoTriangle &ltr)
	{
		if (Logo_IsTriAlreadyIn(ltr))
		{
			return false;
		}

		m_vSig.push_back(ltr);

		return true;
	}
	long long LogoMultiScaleDelaunay::Logo_GetSignature(LogRec::LogoTriangle &ltr)
	{
		for (std::vector<LogRec::LogoTriangle>::iterator itr = m_vSig.begin(); itr != m_vSig.end(); itr++)
		{
			if (*itr == ltr)
			{
				return itr->m_llSignature;
			}
		}
		return 0;
	}
}