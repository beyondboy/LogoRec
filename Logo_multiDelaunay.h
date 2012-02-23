#ifndef	LOGO_MULTI_SCALE__H
#define LOGO_MULTI_SCALE_H

#include "Logo_base.h"

namespace LogRec
{
	class LogoMultiScaleDelaunay
	{
	public:
		LogoMultiScaleDelaunay();
		~LogoMultiScaleDelaunay();
		bool Logo_IsCodeAlreadyIn(LogRec::LogoPoint &lp);
		bool Logo_InsertCode(LogRec::LogoPoint &lp);
		unsigned Logo_GetCode(LogRec::LogoPoint &lp);

		bool Logo_IsTriAlreadyIn(LogRec::LogoTriangle &ltr);
		bool Logo_InsertTri(LogRec::LogoTriangle &ltr);
		long long Logo_GetSignature(LogRec::LogoTriangle &ltr);

	public:
		std::vector<LogRec::LogoPoint> m_vCode;
		std::vector<LogRec::LogoTriangle> m_vSig;
		std::string m_sClassName;
	};
}

#endif