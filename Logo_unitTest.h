#ifndef LOGO_UNIT_H
#define	LOGO_UNIT_H
namespace LogRec
{
	class CLogoTestFrame : public CppUnit::TestCase
	{
	public:
		void setUp();

		void tearDown();
		//
		void testCutBlank();
		void testIsDigit();
		void testIsAllAlpha();
		//...
		CLogoTestFrame (std::string name);// : CppUnit::TestCase(name){}
		static CppUnit::Test* suite();
		static CppUnit::Test* suite1();
	};

	bool startUnitTest();
}
#endif