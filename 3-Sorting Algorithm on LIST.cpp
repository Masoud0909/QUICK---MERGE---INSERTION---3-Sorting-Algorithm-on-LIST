#include <iostream>
#include <list>
#include <iterator>
#include <iterator>
#include <chrono>
#include <cstdlib>	
#include<algorithm>
#include<vector>	
/*Our sorting algorithms can be applied to any type since I used template typenames and did not specified type, but for testing,only for the purpose of testing I used int s*/
/*describing the overall results of my timing experiments according to the result of the test:
Merge Sort performed: 13 milisecond for Randoms to sort
					  8 milisec for Duplicates
					  7 milisecs for Reverse Sorted
					  7 milisec for sorted list
					  so it performs worst when data is Random and		Best when it's sorted (reverse or normal) and		average when we have duplicates


Quick sort performed: 4 milisecond for Randoms to sort
					  2 milisec for Duplicates
					  4 milisecs for Reverse Sorted
					  3 milisec for sorted list
					  so it performs worst when data is Random or		Reverse and Best when it's Duplicates	
					 average when we have sorted list

					 
					 
Insertion Sort Performed:2 milisecond for Randoms to sort
						 1 milisec for Duplicates
						 2 milisecs for Reverse Sorted
						 580 nanosec for sorted list
						 so it performs worst when data is Reversed or	   Random and Best when it's sorted (
						 average when we have duplicates */

/*I used nanoseconds for better illustration as output but in my summary i used milisec for the summarization.*/


/***********************INSERTION SORT******************************************/
template<typename T>									//use template so our function can be applicable for any data
void InsertionSort(std::list<T>& A) {					//we use '&' to make sure we change the list itslef not a copy 	
	for (auto it = A.begin(); it != A.end(); ++it) {	//loop traverses through the list by it iterator	
		T key = std::move(*it);							//using move to transfer the ownership of current element to key(not copying)
		auto j = it;									// initializing iterator j to the current element
		while (j != A.begin() && *(std::prev(j)) > key) {// moving j backwards and shifting elements to the right until current position for insertion is found
			*j = std::move(*(std::prev(j)));			// moving the j element to the right
			--j;										//in order to keepm iterating , moving j backwards
		}
		*j = std::move(key);							// using move to move the key into its correct position
	}
}
/**************************MERGE & MERGESORT***************************************/
template<typename Iter>
void merge(Iter beg, Iter mid, Iter end)
{
	std::vector<typename Iter::value_type> temp;//we need a temp vectr to hold the merged elems
	temp.reserve(std::distance(beg, end));//this function is used to reserve memor to avoid frquesnt reallocation
	Iter left = beg;		//we make two (iterators) pointers for the two halves 
	Iter right = mid;
	while (left != mid and right != end) {//make the two halves in to temp vect.
		if (*right < *left) {//to compare and insert the smaller one into temp
			temp.emplace_back(*right++);
		}
		else {
			temp.emplace_back(*left++);
		}
	}
	temp.insert(temp.end(), left, mid);//insert remaining elems from the left half (if any )
	temp.insert(temp.end(), right, end); //same but for right half

	std::move(temp.begin(), temp.end(), beg);//move merged elems back to original range 
}
template<typename Iter>
void merge_sort(Iter beg, Iter end)
{
	int size = std::distance(beg, end);//getting the size of list
	if (size <= 1) {					//if it s empty return
		return;
	}
	auto mid = std::next(beg, size / 2);
	merge_sort(beg, mid);				// sort left half
	merge_sort(mid, end);				// sort right half
	merge(beg, mid, end);				// merge halves
}

/*************************QUICK SORT****************************************/
template<typename T>
typename std::list<T>::iterator partition(typename std::list<T>::iterator start, typename std::list<T>::iterator end) {
	auto pivot = std::prev(end);		//define the pivot as the last element
	auto i = start;
	for (auto j = start; j != std::prev(end); ++j) {//iterting thro the list
		if (*j < *pivot) {				//current should be smaller than pivot
			std::swap(*i, *j);			//swaping elems at pos i and j
			++i;						//moving i to the next pos
		}
	}
	std::swap(*i, *pivot);				//swapping pivot and its correct position
	return i;							//return the position of the pivot
}
template<typename T>
void quicksort(typename std::list<T>::iterator start, typename std::list<T>::iterator end) {
	if (std::distance(start, end) > 1) {	//to check if the last elem has more than one value
		auto pivot = partition<T>(start, end);//applying the partition function on the list to get the pivot position
		quicksort<T>(start, pivot);			//once perform quicksort on the list before the pivot
		quicksort<T>(std::next(pivot), end);//secondly perform quicksort on the list after the pivot
	}
}
template<typename T>						//finaly we make a function to perform quicksort on the entire list
void quicksort(std::list<T>& lst) {			//and then we call this function not the above ones
	quicksort<T>(lst.begin(), lst.end());
}
/*****************************************************************************************/
int main()
{
	std::cout << "all functions are performing Randoms, Duplicates, Reversed and Sorted lists, RESPECTIVELY" << std::endl << std::endl << std::endl << std::endl;

	std::list <int> Randoms;
	srand(time(NULL));
	for (int i = 0; i < 10; i++)
		Randoms.push_back(rand() % 100);
	std::list<int>duplicate_elems = { 55,33,2,44,2,55,33,44,7,7 };
	std::list<int>reverese_sorted = { 9,8,7,6,5,4,3,2,1,0 };
	std::list<int>sorted = { 1,2,3,4,5,6,7,8,9,10 };

	/*insertion on randoms*/
	std::cout << "INSERTION SORT" << std::endl;
	std::cout << "Before Sort: ";
	for (int elems : Randoms) {
		std::cout << elems << " ";
	}	std::cout << std::endl;

	auto start = std::chrono::high_resolution_clock::now();
	InsertionSort(Randoms);
	auto end = std::chrono::high_resolution_clock::now();
	auto delta = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
	std::cout << "Sorted List: " << std::endl;
	for (int elems : Randoms) {
		std::cout << elems << " ";
	}
	std::cout << std::endl;
	std::cout << "Total time for Insertion Sort on Randoms: " << delta << " nanoseconds\n";

	/*insertion on duplicates*/
	std::cout << "Before Sort: ";
	for (int elems : duplicate_elems) {
		std::cout << elems << " ";
	}	std::cout << std::endl;

	auto start1 = std::chrono::high_resolution_clock::now();
	InsertionSort(duplicate_elems);
	auto end1 = std::chrono::high_resolution_clock::now();
	auto delta1 = std::chrono::duration_cast<std::chrono::nanoseconds>(end1 - start1).count();
	std::cout << "Sorted List: " << std::endl;
	for (int elems : duplicate_elems) {
		std::cout << elems << " ";
	}
	std::cout << std::endl;
	std::cout << "Total time for Insertion Sort on Duplicates: " << delta1 << " nanoseconds\n";

	/*insertion on Reveresedlist*/
	std::cout << "Before Sort: ";
	for (int elems : reverese_sorted) {
		std::cout << elems << " ";
	}	std::cout << std::endl;

	auto start2 = std::chrono::high_resolution_clock::now();
	InsertionSort(reverese_sorted);
	auto end2 = std::chrono::high_resolution_clock::now();
	auto delta2 = std::chrono::duration_cast<std::chrono::nanoseconds>(end2 - start2).count();
	std::cout << "Sorted List: " << std::endl;
	for (int elems : reverese_sorted) {
		std::cout << elems << " ";
	}
	std::cout << std::endl;
	std::cout << "Total time for Insertion Sort on reverese_sorted list: " << delta2 << " nanoseconds\n";

	/*insertion on sorted list*/
	std::cout << "Before Sort: ";
	for (int elems : sorted) {
		std::cout << elems << " ";
	}	std::cout << std::endl;

	auto start0 = std::chrono::high_resolution_clock::now();
	InsertionSort(sorted);
	auto end0 = std::chrono::high_resolution_clock::now();
	auto delta0 = std::chrono::duration_cast<std::chrono::nanoseconds>(end0 - start0).count();
	std::cout << "Sorted List: " << std::endl;
	for (int elems : sorted) {
		std::cout << elems << " ";
	}
	std::cout << std::endl;
	std::cout << "Total time for Insertion Sort on sorted list: " << delta0 << " nanoseconds\n";
	std::cout << std::endl << std::endl;
	/*******************************************************************************************/

	//merge_sort on randoms
	std::cout << "MERGE SORT" << std::endl;
	std::cout << "Before Sort: ";
	for (int elems : Randoms) {
		std::cout << elems << " ";
	}	std::cout << std::endl;

	auto start3 = std::chrono::high_resolution_clock::now();
	merge_sort(Randoms.begin(), Randoms.end());
	auto end3 = std::chrono::high_resolution_clock::now();
	auto delta3 = std::chrono::duration_cast<std::chrono::nanoseconds>(end3 - start3).count();
	std::cout << "Sorted List: " << std::endl;
	for (int elems : Randoms) {
		std::cout << elems << " ";
	}
	std::cout << std::endl;
	std::cout << "Total time for Merge Sort on Randoms: " << delta3 << " nanoseconds\n";

	/*Merge Sort on duplicates*/
	std::cout << "Before Sort: ";
	for (int elems : duplicate_elems) {
		std::cout << elems << " ";
	}	std::cout << std::endl;

	auto start4 = std::chrono::high_resolution_clock::now();
	merge_sort(duplicate_elems.begin(), duplicate_elems.end());
	auto end4 = std::chrono::high_resolution_clock::now();
	auto delta4 = std::chrono::duration_cast<std::chrono::nanoseconds>(end4 - start4).count();
	std::cout << "Sorted List: " << std::endl;
	for (int elems : duplicate_elems) {
		std::cout << elems << " ";
	}
	std::cout << std::endl;
	std::cout << "Total time for Merge Sort on Duplicates: " << delta4 << " nanoseconds\n";

	/*MergeSort on Reveresedlist*/
	std::cout << "Before Sort: ";
	for (int elems : reverese_sorted) {
		std::cout << elems << " ";
	}	std::cout << std::endl;

	auto start21 = std::chrono::high_resolution_clock::now();
	merge_sort(reverese_sorted.begin(), reverese_sorted.end());
	auto end21 = std::chrono::high_resolution_clock::now();
	auto delta21 = std::chrono::duration_cast<std::chrono::nanoseconds>(end21 - start21).count();
	std::cout << "Sorted List: " << std::endl;
	for (int elems : reverese_sorted) {
		std::cout << elems << " ";
	}
	std::cout << std::endl;
	std::cout << "Total time for Merge Sort on reverese_sorted list: " << delta21 << " nanoseconds\n";

	/*Mergesort on sorted list*/
	std::cout << "Before Sort: ";
	for (int elems : sorted) {
		std::cout << elems << " ";
	}	std::cout << std::endl;

	auto start10 = std::chrono::high_resolution_clock::now();
	merge_sort(sorted.begin(), sorted.end());
	auto end10 = std::chrono::high_resolution_clock::now();
	auto delta10 = std::chrono::duration_cast<std::chrono::nanoseconds>(end10 - start10).count();
	std::cout << "Sorted List: " << std::endl;
	for (int elems : sorted) {
		std::cout << elems << " ";
	}
	std::cout << std::endl;
	std::cout << "Total time for Merge Sort on sorted list: " << delta10 << " nanoseconds\n";
	std::cout << std::endl << std::endl;
	/*******************************************************************************************/

	//quicksort(Randoms);
	std::cout << "QUICK SORT" << std::endl;
	std::cout << "Before Sort: ";
	for (int elems : Randoms) {
		std::cout << elems << " ";
	}	std::cout << std::endl;

	auto start31 = std::chrono::high_resolution_clock::now();
	quicksort(Randoms);
	auto end31 = std::chrono::high_resolution_clock::now();
	auto delta31 = std::chrono::duration_cast<std::chrono::nanoseconds>(end31 - start31).count();
	std::cout << "Sorted List: " << std::endl;
	for (int elems : Randoms) {
		std::cout << elems << " ";
	}
	std::cout << std::endl;
	std::cout << "Total time for Quick Sort on Randoms: " << delta31 << " nanoseconds\n";

	/*Quick Sort on duplicates*/
	std::cout << "Before Sort: ";
	for (int elems : duplicate_elems) {
		std::cout << elems << " ";
	}	std::cout << std::endl;

	auto start41 = std::chrono::high_resolution_clock::now();
	quicksort(duplicate_elems);
	auto end41 = std::chrono::high_resolution_clock::now();
	auto delta41 = std::chrono::duration_cast<std::chrono::nanoseconds>(end41 - start41).count();
	std::cout << "Sorted List: " << std::endl;
	for (int elems : duplicate_elems) {
		std::cout << elems << " ";
	}
	std::cout << std::endl;
	std::cout << "Total time for Quick Sort on Duplicates: " << delta41 << " nanoseconds\n";

	/*Quick sort on Reveresedlist*/
	std::cout << "Before Sort: ";
	for (int elems : reverese_sorted) {
		std::cout << elems << " ";
	}	std::cout << std::endl;

	auto start211 = std::chrono::high_resolution_clock::now();
	quicksort(reverese_sorted);
	auto end211 = std::chrono::high_resolution_clock::now();
	auto delta211 = std::chrono::duration_cast<std::chrono::nanoseconds>(end211 - start211).count();
	std::cout << "Sorted List: " << std::endl;
	for (int elems : reverese_sorted) {
		std::cout << elems << " ";
	}
	std::cout << std::endl;
	std::cout << "Total time for Quick Sort on reverese_sorted list: " << delta211 << " nanoseconds\n";

	/*Quick sort on sorted list*/
	std::cout << "Before Sort: ";
	for (int elems : sorted) {
		std::cout << elems << " ";
	}	std::cout << std::endl;

	auto start2111 = std::chrono::high_resolution_clock::now();
	quicksort(sorted);
	auto end2111 = std::chrono::high_resolution_clock::now();
	auto delta2111 = std::chrono::duration_cast<std::chrono::nanoseconds>(end2111 - start2111).count();
	std::cout << "Sorted List: " << std::endl;
	for (int elems : sorted) {
		std::cout << elems << " ";
	}
	std::cout << std::endl;
	std::cout << "Total time for Quick Sort on sorted list: " << delta2111 << " nanoseconds\n";
	std::cout << std::endl << std::endl;
	/*************************************************************************************************/
	return 0;
}
