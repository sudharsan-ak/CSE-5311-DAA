/*
Locally, the program can be run in Visual Studio Code latest version or any Software that has GCC Compiler installed (Ex: CodeBlocks, Dev CPP version 5.11  etc)

The instructions to run the code is given in Run Instructions.txt
*/
#include <iostream>
#include <algorithm>
#include <string>
#include <utility>
#include <climits>
#include <vector>
using namespace std;

/*
Reference: https://www.geeksforgeeks.org/shortest-superstring-problem/
*/ 
 
// Function to calculate maximum overlap in two given strings
int Overlap(string str1, string str2, string &str) 
{
	
	int high = INT_MIN;
	int length1 = str1.length();
	int length2 = str2.length();

	for (int i = 1; i <= min(length1, length2); i++) {
		if (str1.compare(length1-i, i, str2, 0, i) == 0) {
			if (high < i) {
				high = i;
				str = str1 + str2.substr(i);
			}
		}
	}

	for (int i = 1; i <= min(length1, length2); i++) {
		if (str1.compare(0, i, str2, length2-i, i) == 0) {
			if (high < i) {
				high = i;
				str = str2 + str1.substr(i);
			}
		}
	}

	return high;
}

// Function to calculate smallest string that contains each string in the given set as substring. 
string Scs(vector<string> arr, int length) {
	while(length != 1) {
		int high = INT_MIN;
		int l, r;
		string f_string;

		for (int i = 0; i < length; i++)
		{
			for (int j = i + 1; j < length; j++)
			{
				string str;
				int res = Overlap(arr[i], arr[j], str);
				if (high < res)
				{
					high = res;
					f_string.assign(str);
					l = i, r = j;
				}
			}
		}

		length--;

		if (high == INT_MIN)
			arr[0] =arr[0] + arr[length];
		else
		{
			arr[l] = f_string;
			arr[r] = arr[length];
		}
	}
	return arr[0];
}

//Main Function
int main()
{
	int n;
	cin>> n;
	vector<string> arr;
	string temp;
	for(int i = 0; i < n; i++){
		cin >> temp;
		arr.push_back(temp);
	}

	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			if(i != j && arr[i].find(arr[j]) != string::npos){
				cout << arr[j] << " is substring of " << arr[i] << endl;
				return 0;
			}
		}
	}

	string length = Scs(arr, arr.size());

	vector<pair<int, string> > string_index_found;

	for(int i = 0; i < arr.size(); i++)
		string_index_found.push_back(make_pair(length.find(arr[i]), arr[i]));

	sort(string_index_found.begin(), string_index_found.end());

	for(int i = 0; i < string_index_found.size(); i++){
		string temp(string_index_found[i].first, ' ');
		cout << temp + string_index_found[i].second << endl;
	}

	cout << "Length of SCS " << length.size() << endl;

	return 0;
}
