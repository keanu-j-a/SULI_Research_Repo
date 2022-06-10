// Keanu J. Ammons
// 6/10/2022

/*

This code will be a cpp equal to the python code written on jupyter notebooks.
The purpose of this code will be to analyze how parallel computing effects computation
time and cpu % usage. Data collected on the Python solution is provided on Slack, JupyterNotebooks,
and the INL work computer.

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
#include <Windows.h>

using namespace std;

/////////////////////////////////////////////////////NOT MY CODE/////////////////////////////////////////////////////////NOT MY CODE/////////////////////////////////////////////////////////////
/*
static float CalculateCPULoad(unsigned long long idleTicks, unsigned long long totalTicks)
{
	static unsigned long long _previousTotalTicks = 0;
	static unsigned long long _previousIdleTicks = 0;

	unsigned long long totalTicksSinceLastTime = totalTicks - _previousTotalTicks;
	unsigned long long idleTicksSinceLastTime = idleTicks - _previousIdleTicks;

	float ret = 1.0f - ((totalTicksSinceLastTime > 0) ? ((float)idleTicksSinceLastTime) / totalTicksSinceLastTime : 0);

	_previousTotalTicks = totalTicks;
	_previousIdleTicks = idleTicks;
	return ret;
}

static unsigned long long FileTimeToInt64(const FILETIME& ft) { return (((unsigned long long)(ft.dwHighDateTime)) << 32) | ((unsigned long long)ft.dwLowDateTime); }

// Returns 1.0f for "CPU fully pinned", 0.0f for "CPU idle", or somewhere in between
// You'll need to call this at regular intervals, since it measures the load between
// the previous call and the current one.  Returns -1.0 on error.
float GetCPULoad()
{
	FILETIME idleTime, kernelTime, userTime;
	return GetSystemTimes(&idleTime, &kernelTime, &userTime) ? CalculateCPULoad(FileTimeToInt64(idleTime), FileTimeToInt64(kernelTime) + FileTimeToInt64(userTime)) : -1.0f;
}
*/
/////////////////////////////////////////////////////NOT MY CODE/////////////////////////////////////////////////////////NOT MY CODE/////////////////////////////////////////////////////////////

void FibbSolver(int TestCount)
{
	int i = 0;
	double sum;

	// This library had to be researched, since I was unfamiliar with it.
	// refer to the stackoverflow discussion
	// or the documentation for <chrono>
	auto t_start = std::chrono::high_resolution_clock::now();

	// document inital values
	int num1 = 1;
	int num2 = 2;
	int SumNum = num1 + num2;
	list<int> fibb_even;
	list<float> computation_times;

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

	// compute processing time:
	auto t_end = std::chrono::high_resolution_clock::now();
	double elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();


	// append the comp time to a list for future indexing
	computation_times.push_back(elapsed_time_ms);

}

int main() {

	// call the developed function and perform a basic computation

	int l = 0;
	// this extra while loop performs the same operation as
	// shown in the Python code and compute the solution a designated
	// number of times.
	while (l < 100) {
		int TestCount = 1;

		FibbSolver(TestCount);

		l = l + 1;

	}
	return 0;
}