#include "stdafx.h"
#include <CppUnit\TestCase.h>
#include <CppUnit\TestCaller.h>
#include <CppUnit\TestResult.h>
#include <CppUnit\TestSuite.h>
#include <CppUnit\TestRunner.h>
#include "Logo_unitTest.h"
#include "Logo_common.h"

using namespace CppUnit;

namespace LogRec
{
	void CLogoTestFrame::setUp()
	{//init operation for resource

	}

	void CLogoTestFrame::tearDown()
	{//release operation of resource

	}
	//
	void CLogoTestFrame::testCutBlank()
	{
		std::string str = "  libiao ";
		LogRec::LogoCutBlankBE(str);
		assert(str == "libiao");
	}
	void CLogoTestFrame::testIsDigit()
	{
		assert(LogRec::LogoIsDigit("34223"));
	}
	void CLogoTestFrame::testIsAllAlpha()
	{
		assert(LogRec::LogoIsAllAlpha("libiao"));
	}
	//...
	CLogoTestFrame::CLogoTestFrame(std::string name) : CppUnit::TestCase(name)
	{

	}

	Test* CLogoTestFrame::suite()
	{// add test task here
		TestSuite *suiteOfTest = new TestSuite;
		suiteOfTest->addTest(new TestCaller<CLogoTestFrame> ("testCutBlank ",&CLogoTestFrame::testCutBlank));
		suiteOfTest->addTest(new TestCaller<CLogoTestFrame> ("testIsDigit ",&CLogoTestFrame::testIsDigit));
		suiteOfTest->addTest(new TestCaller<CLogoTestFrame> ("testIsAllAlpha ",&CLogoTestFrame::testIsAllAlpha));
		return suiteOfTest;
	}

	Test* CLogoTestFrame::suite1()
	{// add test task here
		TestSuite *suiteOfTest = new TestSuite;
		suiteOfTest->addTest(new TestCaller<CLogoTestFrame> ("testCutBlank ",&CLogoTestFrame::testCutBlank));
		suiteOfTest->addTest(new TestCaller<CLogoTestFrame> ("testIsDigit ",&CLogoTestFrame::testIsDigit));
		suiteOfTest->addTest(new TestCaller<CLogoTestFrame> ("testIsAllAlpha ",&CLogoTestFrame::testIsAllAlpha));
		return suiteOfTest;
	}

	bool startUnitTest()
	{
		TestRunner runner;
		runner.addTest("test calss 1",CLogoTestFrame::suite());
		runner.addTest("test calss 2",CLogoTestFrame::suite());
		//...
		std::vector<std::string> args;

		args.push_back("UnitTest");
		args.push_back("-all");
		args.push_back("-wait");
		args.push_back("-print");

		return runner.run(args);// ? 0 : 1;
	}
}