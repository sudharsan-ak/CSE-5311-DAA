/*
The following code is run on online GCC compiler (IDE)
IDE Link: https://ide.geeksforgeeks.org/

Input is fed into the input space provided.

Locally, the program can be run in Visual Studio Code latest version or any Software that has GCC Compiler installed (Ex: CodeBlocks, Dev CPP version 5.11  etc)
*/
#include <iostream>
#include <map>
#include <vector>
#include <ctime>
using namespace std;
#define MAX  (25000)

/*
The following function creates a multi-map of all unique values of 2nd sequence and replaces each value of the 1st sequence 
with their Index positions provided in the multi-map.

Reference: 
http://www.cplusplus.com/reference/map/multimap/
http://www.programming-algorithms.net/article/40549/Counting-sort
https://gist.github.com/saimageshvar/6bf97980b63d837679423900bd0a705f
*/
int repSize = 0;
int LSISLength;

int* ReplaceFunction(int n1, int a1[], int a2[], int n2) 
{
	int i, j, k;
	int idx = 0;
	int min, max;
	int n = n2;
	int *arr = new int[n2];
	multimap<int, int> multi_map;
	multimap<int, int>::iterator it;

	//Sorting the array
	for (i = 0; i < n2; i++) 
	{
		arr[i] = a2[i];
	}

	//Get the range of the values in the sequence
	min = max = arr[0];
	for (i = 1; i < n2; i++) 
	{
		min = (arr[i] < min) ? arr[i] : min;
		max = (arr[i] > max) ? arr[i] : max;
	}

	k = max - min + 1;

	//Create a count of all the values in the sequence
	int *B = new int[k];
	for (i = 0; i < k; i++) B[i] = 0;

	for (i = 0; i < n2; i++) B[arr[i] - min]++;
	for (i = min; i <= max; i++)
		for (j = 0; j < B[i - min]; j++) arr[idx++] = i;

	//Eliminate Duplicate Elements
	for (i = 0; i<n; ++i)
		for (j = i + 1; j<n;) 
		{
			if (arr[i] == arr[j]) 
			{
				for (k = j; k<n - 1; ++k)
					arr[k] = arr[k + 1];
				--n;
			}
			else
				++j;
		}

	//Map containing Sequence values and their Index positions
	for (i = 0; i < n; i++) {
		for (j = n2 - 1; j >= 0; j--) 
		{
			if (arr[i] == a2[j]) 
			{
				multi_map.insert(std::pair<int, int>(arr[i], j));
			}
		}
	}
	delete[] arr;

	//Replace values in the 1st sequence
	std::vector<int> rv;

	for (i = 0; i < n1; i++) 
	{
		std::pair <std::multimap<int, int>::iterator, std::multimap<int, int>::iterator> ret;
		ret = multi_map.equal_range(a1[i]);
		for (std::multimap<int, int>::iterator it = ret.first; it != ret.second; ++it) 
		{
			rv.push_back(it->second);
		}
	}

	//Store the values of the vector into an array
	int *repArr = new int[rv.size()];

	for (int i = 0; i < rv.size(); i++) 
	{
		repArr[i] = rv[i];
	}

	rv.clear();

	repSize = sizeof(repArr);

	return repArr;
}

/*
The following function takes both the array sequences to compute the common sequence.
Reference: http://ranger.uta.edu/~weems/NOTES5311/LISgriesStrict.c
*/

int* LSISfunction(int n1, int n2, int a1[], int a2[]) 
{

	int *repArr = ReplaceFunction(n1, a1, a2, n2);

	int m[25000];      
	int seq[25000];    
	int sub[25000];    
	int mLink[25000];  
	int i, j, k, mid, low, high;

	k = 1;
	m[1] = repArr[0];
	sub[0] = (-1);
	mLink[1] = 0;
	for (i = 1; i < repSize; i++)
		if (repArr[i] == m[k]) 
		{
			continue;
		}
		else if (repArr[i]>m[k]) 
		{
			k++;
			m[k] = repArr[i];
			sub[i] = mLink[k - 1];
			mLink[k] = i;
		}
		else if (repArr[i]<m[1]) 
		{
			m[1] = repArr[i];
			sub[i] = (-1);
			mLink[1] = i;
		}
		else 
		{
			low = 1;
			high = k;
			while (low != high - 1) 
			{
				mid = (low + high) / 2;
				if (m[mid] <= repArr[i])
					low = mid;
				else
					high = mid;
			}
			// Check search result
			if (m[low]>repArr[i] || m[high] <= repArr[i])
				cout << "error\n";
			if (m[high - 1] == repArr[i]) 
			{
				continue;
			}
			m[high] = repArr[i];
			sub[i] = mLink[high - 1];
			mLink[high] = i;
		}
		
		
		// Get result sequence
		i = mLink[k];
		
		int size = 0;
		for (j = k - 1; j >= 0; j--) 
		{
			seq[j] = repArr[i];
			i = sub[i];
		}

		LSISLength = k;
		int *LSIS = new int[k];
		for (i = 0; i < k; i++) 
		{
			LSIS[i] = a2[seq[i]];
		}

		return LSIS;
}

/*
The following function takes both the array sequences and their LCS length 
to compute the LCS using Matrix Method.
Reference: https://gist.github.com/saimageshvar/65d0c1e0b302886d7502c7f2b6e83ce7
*/
int* LCSfunction(int *a1, int *a2, int n1, int n2, int *LCSlength) 
{

	vector<vector<short> > v;

	//Build a matrix of both the sequences
	for (int i = 0; i <= n1; i++)
	{
		vector<short> temp;
		v.push_back(temp);
		for (int j = 0; j <= n2; j++)
		{
			if (i == 0 || j == 0) 
			{
				v[i].push_back(0);
			}
			else if (a1[i - 1] == a2[j - 1]) 
			{
				v[i].push_back(v[i - 1][j - 1] + 1);
			}
			else 
			{
				if (v[i - 1][j] < v[i][j - 1]) 
				{
					v[i].push_back(v[i][j - 1]);
				}
				else 
				{
					v[i].push_back(v[i-1][j]);
				}
			}		
		}
	}

	int index = v[n1][n2];
	*LCSlength = v[n1][n2];

	//An array to store the LCS sequence
	int *LCS = new int[index + 1];
	
	int i = n1, j = n2;
	while (i > 0 && j > 0) 
	{
		
		//Check if the current value is part of both the sequences or not.
		if (a1[i - 1] == a2[j - 1]) 
		{
			LCS[index - 1] = a1[i - 1]; // Store the result in the result sequence
			i--; j--; index--;
		}

		//If not find the larger of the two neighboring values and go in that direction
		else if (v[i - 1][j] > v[i][j - 1])
			i--;
		else
			j--;
	}

	return LCS;
}

/*Driver Function*/

int main()
{
	int n1, n2, LCSLength;
	
	//Accepting input size of both the sequences
	cin >> n1;
	cin >> n2;

	int *a1 = new int[n1];
	int *a2 = new int[n2];

	//Accepting the inputs of the 1st sequence	
	for (int i = 0; i < n1; i++) 
	{
		cin >> a1[i];
	}
	
	int temp1;
	cin>>temp1;
	
	//Accepting the inputs of the 2nd sequence
	for (int i = 0; i < n2; i++) 
	{
		cin >> a2[i];
	}
	
	int temp2;
	cin>>temp2;

	//Clock function to compute the CPU time taken for LSISfunction
    
	std::clock_t LSIS_Start_Time= std::clock();
	int *LSIS = LSISfunction(n1, n2, a1, a2);
    std::clock_t LSIS_End_Time = std::clock();
    double LSIS_CPU_Time = 1000.0 * (LSIS_End_Time - LSIS_Start_Time) / CLOCKS_PER_SEC;
    LSIS_CPU_Time= LSIS_CPU_Time*100000.0;

    //Clock function to compute the CPU time taken for LCSfunction
	
	std::clock_t LCS_Start_Time= std::clock();
	int *LCS = LCSfunction(a1, a2, n1, n2, &LCSLength);
    std::clock_t LCS_End_Time = std::clock(); 
    double LCS_CPU_Time = 1000.0 * (LCS_End_Time-LCS_Start_Time) / CLOCKS_PER_SEC;
    LCS_CPU_Time= LCS_CPU_Time*100000.0;

	//Check if both sequences are equal
	
	int flag = 0;
	if (LCSLength == LSISLength) 
	{
		for (int i = 0; i < LCSLength; i++) 
		{
			if (LCS[i] != LSIS[i]) 
			{
				flag = 1;
				break;
			}
		}
	}

	//Outputs the length and the digits of the sequence only if both the sequences are equal
	
	if (flag != 1) 
	{
		cout << "Length of the Common Sequence: "<<LCSLength;
		cout<<"\nSequence:";
		for (int i = 0; i < LCSLength; i++) 
		{
			cout << "\n" << LCS[i];
		}
		cout << "\n-1";
	}

	//Displaying the CPU time taken for both the methods
	cout << "\nLSIS CPU Time: " <<LSIS_CPU_Time<<"ms";
	cout << "\nLCS CPU Time: "<<LCS_CPU_Time<<"ms";

	cin.clear();
	cin.ignore();
	cin.get();
}
