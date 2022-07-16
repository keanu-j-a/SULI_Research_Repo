// Keanu J. Ammons
// 6/10/2022

/*

This code will be a cpp equal to the python code written on jupyter notebooks.
The purpose of this code will be to analyze how parallel computing effects computation
time and cpu % usage. Data collected on the Python solution is provided on Slack, JupyterNotebooks,
and the INL work computer. The cpp CPU usage data will be gathered directly from the VS debugging tools
as opposed to the in-code analysis used in the python solution.

Note that this code may also include short scripts that show the cpu % usage and the time to perform
each sucessive computation.

Once again, the solution to the fibb sequence problem is:

4613732

*/

// all the libraries necessary to compile the c++ equal are shown below
#include <iostream>
#include <list>
#include <functional>   
#include <numeric> 
#include <chrono>
#include <iterator>
#include <stdlib.h>

using namespace std;
using namespace std::chrono;

high_resolution_clock::time_point get_time() {
	return high_resolution_clock::now();
}

// for list printing ( other methodds are possible, but I liked this one =D )
float print(list<float> const& list)
{
	for (auto const& i : list) {
		cout << i << endl;
	}
	return 0;
}

// initalize the list of values 
list<int> fibb_even;
int i = 0;
// list<double> computation_times;

int main() {

	// call the developed function and perform a basic computation

	int l = 0;
	// this extra while loop performs the same operation as
	// shown in the Python code and compute the solution a designated
	// number of times.
	list<double> comp_times;

	while (l < 100) {

		auto t_start = get_time();
		int TestCount = 1;
		double sum;

		// reset the list to 0 to prevent values from accumulating upon one another.
		fibb_even = { 0 };

		// document inital values
		int num1 = 1;
		int num2 = 2;
		int SumNum = num1 + num2;

		// This library had to be researched, since I was unfamiliar with it.
		// refer to the stackoverflow discussion
		// or the documentation for <chrono>


		// solve the fibb sequence problem (and obtain the correct answer)
		// using similar methodology to the Python code develeoped previously.
		while (SumNum < 4000001)
		{
			SumNum = num1 + num2;
			num1 = num2;
			num2 = SumNum;
			if (SumNum % 2 == 0) {
				// append items to a list
				fibb_even.push_back(SumNum);
			}
		}
		// compile the solution and print the result (if necessary)
		sum = accumulate(begin(fibb_even), end(fibb_even), 0.0);
		i++;

		// print the final result
		cout << "This is the final solution: " << sum << " number: " << i << endl;

		// append the comp time to a list for future indexing
		// computation_times.push_back(elapsed_time_s);
		auto t_end = get_time();

		// compute the execution time
		auto elapsed_time_s = duration_cast<duration<double>>(t_end - t_start);
		comp_times.push_back(elapsed_time_s.count());
		cout << elapsed_time_s.count() * 1000 << endl;
		l = l + 1;
	}
	double final_time = accumulate(begin(comp_times), end(comp_times), 0.0);

	return 0;
}
