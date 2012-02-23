#ifndef	LOGO_UTIL_H
#define LOGO_UTIL_H

namespace LogRec
{

	bool TriangleIsInRect(CvRect &rect,CvPoint *tri);
	bool IsAlreadyInTriangles(std::vector<LogRec::LogoTriangle> &vLtri, LogRec::LogoTriangle &tris);
	bool GetTriCode(std::vector<LogRec::LogoPoint> &vlp, LogRec::LogoTriangle &tris);
	CvSURFPoint  KeyPointToSurfPoint(KeyPoint &keypt);
	LogoSLRPoint logoSLRPoint(KeyPoint &keypt,unsigned short visualWord);
	LogoSLRPoint logoSLRPoint(CvSURFPoint &surfp,unsigned short visualWord);
	//LogoSLRPoint 
	void LogoReadTrainFileList(std::string filePath, std::string fileName, std::vector<LogRec::LogoTrainFileListFormat> &vTrainList);
	bool LogoLoadCodeBook(std::string filePath, std::string fileName, std::vector<LogRec::LogoCodeBookFormat> &vCodeBook);
	//for draw
	void Logo_draw_subdiv_point( IplImage* img, CvPoint2D32f fp, CvScalar color );
	void Logo_paint_voronoi(CvSubdiv2D* subdiv, IplImage* img);
	void Logo_draw_subdiv_facet(IplImage* img,CvSubdiv2DEdge edge);

	bool LogoVectorIsNotInFormerTriangles(const vector<vector<int> >& one_tri, const vector<vector<vector<int> > > &tris);
	bool LogoEdgeIsRepeated(std::vector<unsigned int> &edges, unsigned int e);
	int Logo_get_visual_vocabulary(std::string dstVocabFile, int thresh_size, int k, std::string filePath, std::string fileList);
	int Logo_get_visual_vocabularyRWI(std::string dstVocabFile, int thresh_size, int k, std::string filePath, std::string fileList);
	
	int Logo_get_visual_vocabularySIFT(std::string dstVocabFile, int thresh_size, int k, std::string filePath, std::string fileList);
	int Logo_get_visual_vocabularySURF();

}

#endif