#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>
#include <future>

using namespace std;
using namespace std::chrono;

typedef long int ull;
typedef struct sum
{
	ull oddSum = 0;
	ull evenSum = 0;
}Sum;

Sum findOddEvenSum(ull start, ull end)
{
	Sum res;
	for(ull i = 0; i <= end; i++)
	{
		if( i & 1)
		{
			res.oddSum += i;
		}
		else
		{
			res.evenSum += i;
		}
	}

	return res;
}

int main()
{
	ull start = 0, end = 1900000000;

	std::future<Sum> oddEvenSum = std::async(std::launch::async, findOddEvenSum, start, end);

	Sum res = std::move(oddEvenSum.get());

	std::cout << "Odd Sum : " << res.oddSum << " :: Even Sum : " << res.evenSum << std::endl;

	return 0;
}
