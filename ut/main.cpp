#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/extensions/HelperMacros.h>

#include <cppunit/TestCase.h>
#include <cppunit/ui/text/TextTestRunner.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/XmlOutputter.h>
#include <netinet/in.h>
#include <list>
#include <iostream>

#include "ParserTest.h"
#include "ExtractorTest.h"
//using namespace CppUnit;

CPPUNIT_TEST_SUITE_REGISTRATION( ParserTest );
CPPUNIT_TEST_SUITE_REGISTRATION( ExtractorTest );

int main()
{
    //CppUnit::TestCaller<ParserTest> test( "testParse", &ParserTest::testParse );
    //CppUnit::TestResult result;
    //test.run( &result); 
   
    // informs test-listener about testresults
    CPPUNIT_NS::TestResult testresult;
    // register listener for collecting the test-results
    CPPUNIT_NS::TestResultCollector collectedresults;
    testresult.addListener (&collectedresults);

    //register listener for per-test progress output
    CPPUNIT_NS::BriefTestProgressListener progress;
    testresult.addListener (&progress);

    // insert test-suite at test-runner by registry
    CPPUNIT_NS::TestRunner testrunner;
    testrunner.addTest (CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest ());
    testrunner.run(testresult);

    // output results in compiler-format
    CPPUNIT_NS::CompilerOutputter compileroutputter(&collectedresults, std::cerr);
    compileroutputter.write (); 

    // return 0 if tests were successful
    return collectedresults.wasSuccessful() ? 0 : 1; 
}
