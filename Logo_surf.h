
#ifndef	LOGO_SURF_H
#define LOGO_SURF_H

namespace LogRec
{
	class CLogoSURF_Wrapper
	{
	public:
		bool findKeyPoints(const char* imgFile);
		bool matchOfTwoImgs(const char* firstImg, const char* secondImg);
	private:

	};
}

#endif