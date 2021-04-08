/* @Author
Student Name: <Burak Atlamaz>
Student ID : <150180067>
Date: <25.12.2020>

The code is compiled as g++ -Wall -Werror main.cpp*/

#include<iostream>
#include<fstream>		//File reading
#include<vector>		//Heap
#include<math.h>		//Euclidian
#include<sys/time.h>	//Time
#include<cstdlib>		//Rand

using namespace std;

struct taxi {
	int size;
	vector<double> heap;
	int left_child(int x) {
		return 2 * x + 1;
	};
	int right_child(int x) {
		return 2 * x + 2;
	};
	int parent(int x) {
		return (x - 1) / 2;
	};
	void create() {
		size = 0;
	};
	void min_heapify(int);
	void insert(double);
	void decrease_key(int, double);
	double extract_min();
	double euclidian(double, double);
};

void taxi::min_heapify(int x) {	//In extract_min func, deletes smallest element
	int left = left_child(x);									//O(1)
	int right = right_child(x);									//O(1)
	int smallest = x;											//O(1)
	if (left < size && heap[left] < heap[x]) {					//O(2)
		smallest = left;										//O(1)
	}
	else {														//O(1)
		smallest = x;											//O(1)
	}
	if (right <= size && heap[right] < heap[smallest]) {		//O(2)
		smallest = right;										//O(1)
	}
	if (smallest != x) {											//O(1)
		//Exchange
		double a = heap[x];										//O(1)	
		heap[x] = heap[smallest];								//O(1)
		heap[smallest] = a;										//O(1) -> Exchange : O(3)

		//Recursive
		min_heapify(smallest);									//O(lg n)
	}
}//Cost is O(lg n)

void taxi::insert(double key) {	//Insert in slide does not worked, so I write different version
	int x = size++;																	// O(2)
	heap.push_back(key);	//Add to the last, then swap between parent-child		// O(1)

	/*If I call decrease_key, the func will give "larger" error each time
	due to heap[size] = +infinite line, so I write similar func again*/

	while (x > 0 && heap[parent(x)] > heap[x]) {									// O(lg n)
		//Exchange 
		double a = heap[x];															// O(1)
		heap[x] = heap[parent(x)];													// O(1)
		heap[parent(x)] = a;														// O(1) -> Exchange : O(3)

		//From child to parent until parent is smaller
		x = parent(x);																// O(1)
	}
}//Cost is O(lg n)

void taxi::decrease_key(int x, double key) {
	if (key > heap[x]) {										// O(1)
		cout << "new_key_is_larger_than_current_key" << endl;	// O(1)
		return;													// O(1)
	}
	heap[x] = key;												// O(1)
	while (x > 0 && heap[parent(x)] > heap[x]) {				// O(lgn)
		//Exchange 
		double a = heap[x];										// O(1)
		heap[x] = heap[parent(x)];								// O(1)
		heap[parent(x)] = a;									// O(1) -> Exchange : O(3)

		//From child to parent until parent is smaller
		x = parent(x);											// O(1)
	}
}//Cost is O(lg n)

double taxi::extract_min() {
	//Error Message
	if (size < 1) {							// O(1)
		cout << "heap_underflow" << endl;	// O(1)
		return -1;							// O(1), not in the slide but have to return something
	}

	double max = heap[0];					// O(1)
	heap[0] = heap[--size];					// O(2)
	min_heapify(0);							// O(lg n)
	return max;								// O(1)
}//Cost is O(lg n)

double taxi::euclidian(double lon, double lat) {
	lon -= 33.40819;
	lat -= 39.19001;

	//calculating Euclidean distance
	double dist = pow(lon, 2) + pow(lat, 2);
	return sqrt(dist);
}

int main(int argc, char* argv[]) {

	//Open file
	ifstream file;
	file.open("locations.txt");

	//Check file is opened or not
	if (!file) {
		cerr << "File cannot be opened!";
		return 1;
	}

	//Initialize values 
	int  m = atoi(argv[1]); //you should read value of m from command line;
	float p = atof(argv[2]); //you should read value of p from command line;
	cout << m << " " << p << endl;
	srand(time(NULL));

	//Header line longitude-latitude readed
	string line;
	getline(file, line); //this is the header line

	//Now Priority Queue with Binary Heap is starting(min-heap)

	taxi t;	//struct initialized
	t.create();

	//Counters initialized
	int counter = 1, additions = 0, updates = 0;
	bool taxi_counter = true;

	//Start time
	struct timeval start_time, end_time;
	double milli_time, seconds, useconds;
	gettimeofday(&start_time, NULL);

	for (int i = 0; i < m; i++) {
		bool TrueFalse = (rand() % 100) < (p * 100);

		//Update
		if (TrueFalse && t.size > 0) {	//t.size > 0 -> Block floating point exception
			//Finding random taxi to update
			int random_taxi_index = rand() % t.size;
			t.decrease_key(random_taxi_index, t.heap[random_taxi_index] - 0.01);
			updates++;
		}

		//Addition
		else if (!TrueFalse) {
			//File reading
			double longitude, latitude, distance;
			file >> longitude;
			file >> latitude;
			getline(file, line, '\n'); //this is for reading the \n character into dummy variable.

			//Add distance to the vector which is founded by using euclidian func
			distance = t.euclidian(longitude, latitude);
			t.insert(distance);
			additions++;
		}

		//In every 100 process, taxi with least distance called 
		counter++;
		if (counter == 100) {
			counter = 1;
			//At first time, express that printing the distance of called taxis 
			if (taxi_counter) {
				cout << "Distance of the called taxis:" << endl;
				taxi_counter = false;
			}
			cout << "Called taxi distance: " << t.extract_min() << endl;
		}
	}
	file.close();

	//End time
	gettimeofday(&end_time, NULL);
	seconds = end_time.tv_sec - start_time.tv_sec; //seconds
	useconds = end_time.tv_usec - start_time.tv_usec; //milliseconds
	milli_time = ((seconds) * 1000 + useconds / 1000.0);

	//Output
	cout << "The number of taxi additions: " << additions << endl;
	cout << "The number of distance updates: " << updates << endl;
	cout << "Running time: " << milli_time << " milliseconds\n";

	return 0;
}
