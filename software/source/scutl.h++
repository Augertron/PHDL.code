// Scutl -- Simple C++ Unit Testing Library
// by Wesley J. Landaker <wjl@icecavern.net>
// =========================================
// Version 1.1.0
//
// Scutl is a unit testing library that makes it painless to add unit tests to
// your software with a minimum of hassle. The entire library implementation
// and all of its documentation is contained in this single header file. The
// only dependency is a standard C++ compiler.
//
// Quick Start Example
// -------------------
//
// This is all you really need to know to get started. Come back and read the
// rest of the documentation whenever you'd like. You may not need to!
//
// Drop this file (scutl.h++) into your current directory.
//
// Put this in test.c++:
//
//     #define SCUTL_MAIN
//     #include "scutl.h++"
//
//     TEST(one_equals_one) {
//         int x = 1;
//         EXPECT(x == 1);
//     }
//
//     TEST(one_plus_one) {
//         int x = 1;
//         EXPECT((x+x) == 3);
//     }
//
//     TEST(two_times_two) {
//         int x = 1;
//         REQUIRE(x == 2);
//         EXPECT((x*x) == 4);
//     }
//
// Build & run with GCC:
//
//     g++ test.c++ -o test && ./test > test_log
//
// The result is:
//
//     scutl: test.c++:11:one_plus_one: ERROR: EXPECT((x+x) == 3)
//     scutl: test.c++:16:two_times_two: ERROR: REQUIRE(x == 2)
//     scutl: SUMMARY: |count   |started |complete|passed  |failed  |aborted |
//     scutl: SUMMARY: |       3|       3|       3|       1|       2|       1|
//
// Whoops! Looks like something wasn't quite right with our code. Fortunately,
// it should now be easy to go fix those bugs with confidence!
//
// Writing Tests
// -------------
//
// As seen in the quick start example above, using scutl is as easy as
// including this header and creating tests! Let's look at the basics:
//
// 1. Define any desired scutl options, such as SCUTL_MAIN or SCUTL_REPORTER.
// 2. Include this header, for example with #include <scutl.h++>.
// 3. Create tests using scutl features such as TEST(name) { /* body */ }
// 4. Inside tests, use scutl checks such as EXPECT(expr) and REQUIRE(expr).
// 5. Build and run your tests using several possible strategies.
//
// Scutl Options
// -------------
//
// Several options can be set before including this header to get specific
// behavior. They are listed here with examples, but they can of course be
// defined via compiler options (such as -D for gcc) instead if so desired.
//
// SCUTL_LIBRARY:
//
//     #define SCUTL_LIBRARY
//     #include <scutl.h++>
//
// When SCUTL_LIBRARY is defined before including the scutl header, scutl will
// emit the definition of its library internals. To avoid linker errors (due to
// multiple definitions), this can only be done in one file per executable.
//
// SCUTL_MAIN:
//
//     #define SCUTL_MAIN
//     #include <scutl.h++>
//
// When SCUTL_MAIN is defined before including the scutl header, scutl will
// provide an implementation of the main function. When SCUTL_MAIN is defined,
// SCUTL_LIBRARY is implicitly active an need not be specified separately.
//
// SCUTL_REPORTER:
//
//     #define SCUTL_REPORTER TAP_Reporter
//     #include <scutl.h++>
//
// When SCUTL_REPORTER is set to a predefined scutl reporter it will be used
// instead of the default reporter. Specifying a value for SCUTL_REPORTER
// implies SCUTL_MAIN, so that need not be defined additionally. (Use a
// non-built-in reporter is quite easy, and is explained in a later section,
// but doesn't use this option at all.)
//
// The currently available predefined reporters are:
//
// Default_Reporter -- writes test log to stdout and errors/summary to stderr
// TAP_Reporter     -- writes Test Anything Protocol format to stdout
//
// Including The Library
// ---------------------
//
// Except for the quick start example, which specifically showed using
// scutl.h++ dropped into the current directory for a demonstration, most
// examples show the scutl header (this file) being included with the
// system-header search syntax:
//
//     #include <scutl.h++>
//
// Even though scutl is entirely self-contained in a single file, and may be
// embedded in a project rather than being installed system-wide, it's highly
// to use the appropriate compiler flags (such as -I for gcc) to make this
// syntax work either way.
//
// Creating Tests & Using Fixtures
// -------------------------------
//
// Tests can be very easily created as either simple functions or as methods of
// a user-provided test fixture class. Either way, tests need to use the macros
// scutl provides to yield tests that work automatically. How to use these is
// shown here with some short examples:
//
// TEST:
//
//     TEST(one_equals_one) {
//         int x = 1;
//         EXPECT(x == 1);
//     }
//
// The TEST macro is used to define a plain test function. The test name is
// used in reports along with the file name and corresponding line number, so
// should be suitably descriptive.
//
// TEST_METHOD:
//
//     struct Test_Fixture {
//         Test_Fixture() : x(1) {}
//         ~Test_Fixture() {
//             EXPECT(x == 2);
//         }
//     };
//
//     TEST_METHOD(Test_Fixture, x_can_increment) {
//         REQUIRE(x == 1);
//         x++;
//     }
//
//     TEST_METHOD(Test_Fixture, x_can_be_assigned) {
//         REQUIRE(x == 1);
//         x = 2;
//     }
//
// The TEST_METHOD macro is used to run a test as a method on the given test
// fixture class. This makes it easy to use a common setup or common checks
// between mulitple tests. Because each test runs with a new copy of the
// fixture, tests do not interfere with each other.
//
// Using Assertions in Tests
// -------------------------
//
// Inside of tests is where the real goal of testing, which is actually
// validating assertions, happens. Scutl provides a few different types of
// assertion mechanisms. They are described here:
//
// EXPECT:
//
//     EXPECT(x == 2);
//     EXPECT(a < b);
//     EXPECT(list.size() == limit);
//     EXPECT(graph->get_node(id).has_property("blue"));
//
// The EXPECT assertion simply takes any expression and evaluates it in a
// boolean context. If it is false, then an error is reported. Either way, the
// current test continues executing. This is the most basic type of assertion.
//
// REQUIRE:
//
//     REQUIRE(library->has_feature_X());
//     REQUIRE(!list.empty());
//     for (auto item : list) {
//         auto result = library->use_feature_X(item);
//         EXPECT(result.has_property_X());
//     }
//
// The REQUIRE assertion works just like EXPECT, but when there is a failure,
// the current test is aborted. This assertion is used when an assertion
// failure implies that the current test should not even attempt to continue.
//
// Building Test Executables
// -------------------------
//
// Since you'd usually rather write tests than mess with boilerplate, you often
// want the library to implement the main function for you. Other times, you
// want to do something a little different. The following sections describe a
// few common schemes:
//
// One Source File Per Exectuable
// ------------------------------
//
// If you prefer to build an executable for each test source file, simply
// define SCUTL_MAIN before including this header, which will emit a main
// function implementation directly.
//
//     #define SCUTL_MAIN
//     #include <scutl.h++>
//     // Tests go here!
//
// Multiple Files Per Exectuable
// -----------------------------
//
// The suitation where you'd like to link multiple test files together, but
// still want scutl to provide the main implementation is similarly easy. Just
// choose one file to contain the main implementation, and define SCUTL_MAIN
// there. In all other files, don't define anything. Problem solved!
//
//     // In main.c++
//     #define SCUTL_MAIN
//     #include <scutl.h++>
//
//     // In baseline.c++, features.c++, process.c++, etc:
//     #include <scutl.h++>
//     // tests go here in each file
//
// Do It Yourself
// --------------
//
// If you don't want scutl to provide a main implementation and run itself
// automatically, nothing prevents you from doing this yourself! You will,
// however, still need scutl to emit its library implementation, and then
// you'll need to activate scutl by calling its run function. Fortunately, this
// is very easy to do:
//
//     // In main.c++
//     #define SCUTL_LIBRARY
//     #include <scutl.h++>
//     int main(int argc, char **argv) {
//         // your implementation here
//         scutl::Default_Reporter reporter;
//         bool passed = scutl::run(reporter);
//         // your implementation here
//     }
//
//     // In baseline.c++, features.c++, process.c++, etc:
//     #include <scutl.h++>
//     // tests go here in each file
//
// Custom Reporters
// ----------------
//
// If you'd like to use your own custom reporter, simplfy follow the
// instructions under the do-it-yourself section above, but instead of passing
// a predefined scutl reporter instance to scutl::run, hand it a reporter of
// your own. The interface for reporters is scutl::Reporter, and is documented
// fully where it's defined in the header down below.
//
// Further API & Implementation Details
// ------------------------------------
//
// By now, if you've read the above documentation, you should know everything
// you need to to use this library in all conventional ways. API details we've
// missed going into here, as well as complete, fully documented implementation
// details await you down below, inline with the code itself. Enjoy!
//
// Copyright & License Information
// -------------------------------
//
// Copyright © 2013 Wesley J. Landaker
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU General Public License along with
// this program. If not, see <http://www.gnu.org/licenses/>.

// Header guard
#ifndef SCUTL__header__
#define SCUTL__header__

// Required standard library headers
#include <iomanip>
#include <iostream>
#include <list>
#include <sstream>
#include <string>

// Helper macros for concatenation. Two levels of indirection are required in
// order to expand macros when concatenating. (For instance, this is necessary
// to concatenate __LINE__).
#define SCUTL_DETAIL_CONCAT_RAW(a, b) a ## b
#define SCUTL_DETAIL_CONCAT(a, b) SCUTL_DETAIL_CONCAT_RAW(a, b)

// Define a test with the given name and provided body.
//
//     TEST(name) { body }
//
#ifdef TEST
#error "Scutl wants to provide the TEST macro, but it's already defined!"
#endif
#define TEST(name) TEST_METHOD(::scutl::detail::Empty_Fixture, name)

// Define a test method on an instance of the given fixture class with the
// given name and provided body.
//
//     TEST_METHOD(fixture, name) { body }
//
#ifdef TEST_METHOD
#error "Scutl wants to provide the TEST_METHOD macro, but it's already defined!"
#endif
#define TEST_METHOD(fixture, name)\
\
	/* Enter a uniquified anonymous namespace to avoid conflicts both between\
	 * tests in a single file, and tests in other files. */\
	namespace {\
	namespace SCUTL_DETAIL_CONCAT(scutl_detail_line_,__LINE__) {\
\
	/* The function is implemented in a class inheriting from the provided\
	 * fixture class. This is done separately from the Test object to ensure\
	 * that the user's test function does not have a polluted namespace. */\
	struct Function : fixture {\
		void operator()();\
	};\
\
	/* Define our Test class. This calls the user-defined test function when\
	 * activtated. */\
	struct Test : ::scutl::detail::Test {\
		Test() {\
			/* Set up our Test_Info structure */\
			info.na##me  = #name;\
			info.file    = __FILE__;\
			info.line    = __LINE__;\
			/* We assume that we have passed until we see a failure */\
			info.passed  = true;\
			/* We assume that we have been aborted until we complete */\
			info.aborted = true;\
		}\
		virtual void operator()() {\
			function();\
		};\
		Function function;\
	};\
\
	/* Create an instance of the test, which will cause auto-registration */\
	Test test;\
\
	/* Close the namespaces we opened */\
	}}\
\
	/* Define our test function's body */\
	void SCUTL_DETAIL_CONCAT(scutl_detail_line_,__LINE__)\
		::Function::operator()()


// Helper macro for impementing assertions.
#define SCUTL_DETAIL_ASSERT(expression, assertion, required)\
\
	/* Wrap in do/while to give a statement-like interface. */\
	do {\
\
		/* On a failure, report the results and abort if necessary. */\
		if (!(expression)) {\
\
			/* Collect information about the assertion. */\
			::scutl::Error_Info error_info;\
			error_info.requi##red = required;\
			error_info.expre##ssion = assertion "(" #expression ")";\
			error_info.file = __FILE__;\
			error_info.line = __LINE__;\
\
			/* Mark current test as failed */\
			::scutl::detail::global.test->info.passed = false;\
\
			/* Report results */\
			::scutl::detail::global.reporter->report_test_error(\
				::scutl::detail::global.test->info,\
				error_info\
			);\
			/* Throw the error information as an exception to abort the test\
			 * if this was a required assertion. */\
			if (required) {\
				/* Mark current test as aborted */\
				::scutl::detail::global.test->info.aborted = true;\
				throw error_info;\
			}\
		}\
	} while(0)

// Expect that the given expression is true.
//
//     TEST(name) {
//         EXPECT(expression);
//     }
//
#ifdef EXPECT
#error "Scutl wants to provide the EXPECT macro, but it's already defined!"
#endif
#define EXPECT(expression) SCUTL_DETAIL_ASSERT(expression, "EXPECT", false)

// Require that the given expression is true; otherwise abort the test.
//
//     TEST(name) {
//         REQUIRE(expression);
//     }
//
#ifdef REQUIRE
#error "Scutl wants to provide the REQUIRE macro, but it's already defined!"
#endif
#define REQUIRE(expression) SCUTL_DETAIL_ASSERT(expression, "REQUIRE", true)

// Functions & classes in the public API
namespace scutl {

	// Information about a test. This is used in the implementation, but is
	// part of the public API because it used by the reporter interface.
	struct Test_Info {
		std::string name;    // From TEST(name)
		std::string file;    // Filename from __FILE__
		size_t      line;    // Line from __LINE__
		bool        passed;  // Test passed; only valid after completion
		bool        aborted; // Test aborted; only valid after completion
	};

	// Information about a failing assertion. This is used by the
	// implementation as an exception when aborting tests, but is also used by
	// the reporter interface.
	struct Error_Info {
		bool required;          // True if a failing REQUIRE
		std::string expression; // From EXPECT/REQUIRE(expression)
		std::string file;       // From __FILE__
		size_t      line;       // From __LINE__
	};

	// Statistics collected during test runnning. There is some duplicated
	// information, so it's possible that some of these numbers won't add up
	// if something severely wrong while running tests. But this exists
	// entirely for the benefit of reporters, which should be easy to create.
	struct Test_Statistics {
		size_t count;    // Count of tests that were registered to be run
		size_t started;  // Tests that were started
		size_t complete; // Tests that completed -- should matched started!
		size_t passed;   // Tests that passed
		size_t failed;   // Tested that failed. (passed + failed == complete)
		size_t aborted;  // Tests that were aborted; a subset of failed
	};

	// Abstract interface for reporters of test results. Both built-in and
	// custom reporters must implement all of these virtual functions.
	struct Reporter {
		Reporter() {}
		virtual ~Reporter() {}

		// Called at the beginning of testing to indicate how many tests are
		// planned to be run. This same data will be in the statistics object
		// later, but many reporters can benefit from knowing this early on.
		virtual void report_test_count   (size_t) = 0;

		// Called when a test has been started. The passed and aborted fields
		// of the Test_Info object of course are not valid at this point, but
		// the other fields are.
		virtual void report_test_started (const Test_Info &) = 0;

		// Called when a test has been completed. At this point, the reporter
		// can determine whether the test passed, failed, and/or was aborted.
		virtual void report_test_complete(const Test_Info &) = 0;

		// Called when there is an assertion error in one of the tests,
		// either from an EXPECT or a REQUIRE call. Which one can be
		// determined by looking at the Error_Info.
		//
		// This function is only called between report_test_started and
		// report_test_complete, but the current Test_Info is provided for
		// convenience. (Remember that its passed and abort members are still
		// not valid at this point!)
		virtual void report_test_error   (const Test_Info &, const Error_Info &) = 0;

		// Called after all tests have been run, in order to summarize all of
		// the results. The reporter could have collected this information,
		// but by providing this functionality, reporters are simpler and
		// easier to create.
		virtual void report_test_summary (const Test_Statistics &) = 0;
	};

	// Default reporter writes complete test log to stdout and errors/summary to stderr
	struct Default_Reporter : Reporter {
		Default_Reporter();
		virtual ~Default_Reporter();
		virtual void report_test_count   (size_t);
		virtual void report_test_started (const Test_Info &);
		virtual void report_test_complete(const Test_Info &);
		virtual void report_test_error   (const Test_Info &, const Error_Info &);
		virtual void report_test_summary (const Test_Statistics &);
	};

	// TAP reporter writes Test Anything Protocol format to stdout
	struct TAP_Reporter : Reporter {
		TAP_Reporter();
		virtual ~TAP_Reporter();
		virtual void report_test_count   (size_t);
		virtual void report_test_started (const Test_Info &);
		virtual void report_test_complete(const Test_Info &);
		virtual void report_test_error   (const Test_Info &, const Error_Info &);
		virtual void report_test_summary (const Test_Statistics &);
		private:
		size_t number;
	};

};

// This namespace is for implementation details only.
namespace scutl { namespace detail {

	// Base class to encapsulate tests. They are auto-registered in the global
	// list upon construction.
	struct Test {
		Test() { list().push_back(this); }
		virtual ~Test() {}
		virtual void operator()() = 0;

		// Test information
		Test_Info info;

		// Global test list
		typedef std::list<Test *> List;
		static List &list();
	};

	// Define an empty class used to implement TEST in terms of TEST_METHOD
	struct Empty_Fixture {};

	// Global pointers set by the test runner and use by assertions
	struct Global {
		Test     *test;
		Reporter *reporter;
	};
	extern Global global;

}}

// If a specific reporter is requested, this implies that the main
// implementation is desired as well.
#ifdef SCUTL_REPORTER
#ifndef SCUTL_MAIN
#define SCUTL_MAIN
#endif
#endif

// If the main implementation is requested, we need to add the library
// implementation as well.
#ifdef SCUTL_MAIN
#ifndef SCUTL_LIBRARY
#define SCUTL_LIBRARY
#endif
#endif

// Implement the scutl library when requested
#ifdef SCUTL_LIBRARY
namespace scutl { namespace detail {

	// Implement our list of tests. We use a function to return a single
	// instance instead of a straight global variable to avoid static-
	// initialization ordering issues.
	Test::List &Test::list() {
		static Test::List list;
		return list;
	}

	// Implement our global structure used to by the runner and assertions.
	Global global;

	// Helper function to run a single test with a given reporter.
	void run_test(Test &test, Reporter &reporter, Test_Statistics &statistics) {

		// Set the global test and reporter pointers for use by assertions.
		global.test     = &test;
		global.reporter = &reporter;

		// Report the test to the reporter.
		reporter.report_test_started(test.info);
		++statistics.started;

		// Run the test inside a try block. If we catch any exceptions,
		// report them as implicit failed assertions.
		try {
			test();
			// By default test assume they will be aborted. If we don't get
			// an exception, then we can be assured that didn't happen.
			test.info.aborted = false;
		} catch (const Error_Info &) {
			// If an Error_Info is received as an exception, we know it
			// has already been reported before being thrown, so additional
			// handling is unnecessary here.
		} catch (const std::exception &e) {
			Error_Info error_info;
			error_info.expression = std::string("unexpected exception: ") + e.what();
			error_info.file = test.info.file;
			error_info.line = test.info.line;
			test.info.passed  = false;
			test.info.aborted = true;
			reporter.report_test_error(test.info, error_info);
		} catch (...) {
			Error_Info error_info;
			error_info.expression = "unknown exception";
			error_info.file = test.info.file;
			error_info.line = test.info.line;
			test.info.passed  = false;
			test.info.aborted = true;
			reporter.report_test_error(test.info, error_info);
		}

		// Report the test completion
		reporter.report_test_complete(test.info);
		++statistics.complete;
		if (test.info.passed)  ++statistics.passed; else ++statistics.failed;
		if (test.info.aborted) ++statistics.aborted;
	}
}}

namespace scutl {

	// The scutl entry point, which can be called by the user when ready to
	// run all the registered tests against the given reporter. Returns true
	// if everything passed, otherwise false.
	bool run(Reporter &reporter) {

		// Zero out statistics we will be collecting while running
		Test_Statistics statistics;
		statistics.count    = 0;
		statistics.started  = 0;
		statistics.complete = 0;
		statistics.passed   = 0;
		statistics.failed   = 0;
		statistics.aborted  = 0;

		// Grab a reference to our test list
		scutl::detail::Test::List &test_list = scutl::detail::Test::list();

		// Inform the reporter of how many tests we are going to run.
		statistics.count = test_list.size();
		reporter.report_test_count(statistics.count);

		// Run each test with the given reporter.
		for (
			scutl::detail::Test::List::iterator test_iterator = test_list.begin();
			test_iterator != test_list.end();
			++test_iterator
		) {
			scutl::detail::Test &test = **test_iterator;
			scutl::detail::run_test(test, reporter, statistics);
		}

		// Report the final summary
		reporter.report_test_summary(statistics);

		return
			(statistics.count == statistics.started) &&
			(statistics.count == statistics.complete) &&
			(statistics.count == statistics.passed) &&
			(statistics.failed == 0) &&
			(statistics.aborted == 0);
	}

	// Implementation of the default reporter.

	Default_Reporter::Default_Reporter() {}

	Default_Reporter::~Default_Reporter() {}

	void Default_Reporter::report_test_count(size_t count) {
		std::cout << "scutl: Running " << count << " tests ...\n";
	}

	void Default_Reporter::report_test_started(const Test_Info &test_info) {
		std::cout
			<< "scutl: "
			<< test_info.file << ":"
			<< test_info.line << ":"
			<< test_info.name << ": starting\n";
		;
	}

	void Default_Reporter::report_test_complete(const Test_Info &test_info) {
		std::cout
			<< "scutl: "
			<< test_info.file << ":"
			<< test_info.line << ":"
			<< test_info.name << ":"
			<< (test_info.passed ? " passed" : " failed")
			<< (test_info.aborted ? " (aborted)\n" : "\n")
		;
	}

	void Default_Reporter::report_test_error(const Test_Info &test_info, const Error_Info &error_info) {
		std::ostringstream ss;
		ss
			<< "scutl: "
			<< error_info.file << ":"
			<< error_info.line << ":"
			<< test_info.name << ": ERROR: "
			<< error_info.expression << "\n"
		;
		std::cout << ss.str();
		std::cerr << ss.str();
	}

	void Default_Reporter::report_test_summary(const Test_Statistics &statistics) {
		std::ostringstream ss;
		ss
			<< "scutl: SUMMARY: |"
			<< "count   " << "|"
			<< "started " << "|"
			<< "complete" << "|"
			<< "passed  " << "|"
			<< "failed  " << "|"
			<< "aborted " << "|"
			<< "\n"
			<< "scutl: SUMMARY: |"
			<< std::setw(8) << statistics.count    << "|"
			<< std::setw(8) << statistics.started  << "|"
			<< std::setw(8) << statistics.complete << "|"
			<< std::setw(8) << statistics.passed   << "|"
			<< std::setw(8) << statistics.failed   << "|"
			<< std::setw(8) << statistics.aborted  << "|"
			<< "\n"
		;
		std::cout << ss.str();
		std::cerr << ss.str();
	}

	// Implementation of the Test Anything Protocol (TAP) reporter.

	TAP_Reporter::TAP_Reporter() {
		std::cout << "TAP version 13\n";
		number = 0;
	}

	TAP_Reporter::~TAP_Reporter() {}

	void TAP_Reporter::report_test_count(size_t count) {
		std::cout << "1.." << count << "\n";
	}

	void TAP_Reporter::report_test_started(const Test_Info &) {
		++number;
	}

	void TAP_Reporter::report_test_complete(const Test_Info &test_info) {
		std::cout
			<< (test_info.passed ? "ok " : "not ok ")
			<< number << " - "
			<< test_info.name << ":"
			<< test_info.file << ":"
			<< test_info.line
			<< (test_info.aborted ? ": aborted\n" : "\n")
		;
	}

	void TAP_Reporter::report_test_error(const Test_Info &test_info, const Error_Info &error_info) {
		std::cout
			<< error_info.file << ":"
			<< error_info.line << ":"
			<< test_info.name << ": ERROR: "
			<< error_info.expression << "\n"
		;
	}

	void TAP_Reporter::report_test_summary(const Test_Statistics &) {}

}
#endif

// Implement the main function when requested.
#ifdef SCUTL_MAIN

// Define SCUTL_REPORTER if the user has not done so.
#ifndef SCUTL_REPORTER
#define SCUTL_REPORTER Default_Reporter
#endif

int main() {
	scutl::SCUTL_REPORTER reporter;
	bool passed = scutl::run(reporter);
	return !passed;
}
#endif

#endif // Header guard
