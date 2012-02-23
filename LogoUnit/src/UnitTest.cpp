#include "CppUnit\TestCase.h"
#include "CppUnit\TestCaller.h"
#include <CppUnit\TestResult.h>
#include <CppUnit\TestSuite.h>
#include <CppUnit\TestRunner.h>

#include <Windows.h>
#include <string>
#include <iostream>

using namespace CppUnit;

class Complex {
private:
	friend bool operator == (const Complex& a, const Complex& b);
public:
	Complex()
	: m_real(0), m_imaginary(0)
	{

	}

	Complex(double real, double imaginary)
	: m_real(real), m_imaginary(imaginary)
	{
	
	}
	Complex operator+(const Complex& other)
	{
		return Complex(this->m_real+other.m_real,this->m_imaginary+other.m_imaginary);
	}

private:
	double m_real;
	double m_imaginary;
};

bool operator== (const Complex& a, const Complex& b)
{
	return ((a.m_real == b.m_real) && (a.m_imaginary == b.m_imaginary));
}

class CompleNumberTest : public CppUnit::TestCase
{
private:
	Complex *m_pC1;
	Complex *m_pC2;
	Complex *m_pC3;
public:
	void setUp()
	{
		m_pC1 = new Complex(1,1);
		m_pC2 = new Complex(1,2);
		m_pC3 = new Complex(2,3);
	}

	void tearDown()
	{
		delete m_pC1;
		delete m_pC2;
		delete m_pC3;
	}

	void testEquality()
	{
		assert(*m_pC1 == *m_pC2);
		assert(*m_pC1 == *m_pC3);
	}

	void testAddition()
	{
		assert((*m_pC1 + *m_pC2) == *m_pC3);
	}

public:
	CompleNumberTest (std::string name) : CppUnit::TestCase (name) {}
	
	static Test *suite()
	{
		TestSuite *suiteOfTest = new TestSuite;
		suiteOfTest->addTest(new TestCaller<CompleNumberTest> ("testEquality", &CompleNumberTest::testEquality));
		suiteOfTest->addTest(new TestCaller<CompleNumberTest> ("testAddition", &CompleNumberTest::testAddition));
		return suiteOfTest;
	}
	
	/*void testSuite()
	{
		TestCaller<CompleNumberTest> test("testEquality", testEquality);
		test.run();
	}
	
	void runTest()
	{

		assert( Complex(10,1) == Complex(10,1) );
		assert( Complex(10,10) == Complex(11,11) );
		try
		{
			assert( Complex(10,1) == Complex(10,1) );
			assert( Complex(10,10) == Complex(11,11) );
		}
		catch (CppUnitException &cue)
		{
			std::cout << "exception: " << cue.CPPUNIT_UNKNOWNFILENAME 
				<< " in line :" << cue.lineNumber() << " in file: " 
				<< cue.fileName() << std::endl;
		}
	}*/	
};

class ComplexNumberTest : public CppUnit::TestCase
{
private:
	Complex *m_pC1;
	Complex *m_pC2;
	Complex *m_pC3;
public:
	void setUp()
	{
		m_pC1 = new Complex(1,1);
		m_pC2 = new Complex(1,1);
		m_pC3 = new Complex(2,3);
	}

	void tearDown()
	{
		delete m_pC1;
		delete m_pC2;
		delete m_pC3;
	}

	void testEquality()
	{
		assert(*m_pC1 == *m_pC2);
		assert(*m_pC1 == *m_pC3);
	}

	void testAddition()
	{
		assert((*m_pC1 + *m_pC2) == *m_pC3);
	}

public:
	ComplexNumberTest (std::string name) : CppUnit::TestCase (name) {}
	
	static Test *suite()
	{
		TestSuite *suiteOfTest = new TestSuite;
		suiteOfTest->addTest(new TestCaller<ComplexNumberTest> ("testEquality", &ComplexNumberTest::testEquality));
		suiteOfTest->addTest(new TestCaller<ComplexNumberTest> ("testEquality", &ComplexNumberTest::testAddition));
		return suiteOfTest;
	}
	
	/*void testSuite()
	{
		TestCaller<CompleNumberTest> test("testEquality", testEquality);
		test.run();
	}
	
	void runTest()
	{

		assert( Complex(10,1) == Complex(10,1) );
		assert( Complex(10,10) == Complex(11,11) );
		try
		{
			assert( Complex(10,1) == Complex(10,1) );
			assert( Complex(10,10) == Complex(11,11) );
		}
		catch (CppUnitException &cue)
		{
			std::cout << "exception: " << cue.CPPUNIT_UNKNOWNFILENAME 
				<< " in line :" << cue.lineNumber() << " in file: " 
				<< cue.fileName() << std::endl;
		}
	}*/	
};

int main(int ac, char **av)
{
	/*CompleNumberTest cnt("complete!");
	cnt.run();
	cnt.testSuite();*/
	//CompleNumberTest
	/*TestCaller<CompleNumberTest> test("testEquality", &CompleNumberTest::testEquality);
	test.run();

	TestResult result;
	TestCaller<CompleNumberTest> test("testAddition", &CompleNumberTest::testEquality);
	test.run(&result);

	TestSuite suite;
	TestResult result;
	suite.addTest(new TestCaller<CompleNumberTest> ("testEquality", &CompleNumberTest::testEquality));
	suite.addTest(new TestCaller<CompleNumberTest> ("testEquality", &CompleNumberTest::testAddition));
	suite.run(&result);*/

	TestRunner runner;
	TestResult result;
	runner.addTest("test1",CompleNumberTest::suite());
	runner.addTest("test2",ComplexNumberTest::suite());
	//runner.run(&result);
												
	std::vector<std::string> args;					
	/*for (int i = 0; i < ac; ++i)					
		args.push_back(std::string(av[i]));			
	CppUnit::TestRunner runner;	*/					
	//runner.addTest(#testCase, testCase::suite());	
	args.push_back(std::string(av[0]));
	args.push_back("-all");
	args.push_back("-wait");
	args.push_back("-print");
	//runner.run(args);
	return runner.run(args) ? 0 : 1;				
	
	//system("pause");
	//return 0;
}
//CppUnitMain(CompleNumberTest)