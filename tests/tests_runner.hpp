/*
 *	Copyright (C) 2008 - 2013, XiaoJSoft and/or its affiliates. All right reserved.
 *	XIAOJSOFT PROPRIETARY/CONFIDENTAL. Use is subject to license terms.
*/

#ifndef __TESTS_RUNNER_HPP__
	#define __TESTS_RUNNER_HPP__
	struct t_unit_test_table {
		string name;
		string path;
		int lower, higher;
		void (*test)(string inpath, string outpath);
	};

	/*  Declares  */
	bool tester_check_result(const string &path, const string &sample);
	int main(int argc, char **argv);
#endif

