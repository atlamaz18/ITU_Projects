/* @Author
Student Name: <Burak Atlamaz>
Student ID : <150180067>
Date: <10.12.2020> */

#include<iostream>
#include<fstream>
#include<string>
#include<chrono>	//for calculate time
#include<iomanip>	//setprecision

using namespace std;

struct carry {		//Struct that keeps data, used for swap
	string country;
	string item_type;
	string order_id;
	int units_sold;
	float total_profit;
};

struct product {	//Struct that keeps pointers, node of linked list
	product* next;
	product* prev;
	carry* data;
};

struct sales {		//List struct; keeps func, head and tail
	product* head;
	product* tail;
	void create();
	void add_product(carry*);
	void Quicksort(product*, product*);
	product* partition(product*, product*);
	void clear();
	void print(string);
	void swap(product*, product*);
};

void sales::create() {	//Head and tail initialized
	head = NULL;
	tail = NULL;
}

void sales::add_product(carry* x) {	//carry holds all the variables, all needed is add those to the last
	product* newnode = new product;
	newnode->data = x;
	newnode->next = NULL;
	if (!head) {	//If head is NULL, had must be last 
		head = newnode;
		head->prev = NULL;
		tail = head;
		return;
	}
	tail->next = newnode;	//If I do everyhting with head instead of tail
	newnode->prev = tail;	//The program works 100 times slower to find the last element all the time
	tail = newnode;			//It visits all the elements and that is such a waste of time, tail is much more better
}

void sales::Quicksort(product* left, product* right) {	//Begin and end of the divided list, at first nondivided obviously
	if (!right || left == right || left == right->next) {
		return;	//Quicksort cannot be applied, like there is only one element
	}
	product* divided = partition(left, right);	//Just like the slide, divide two parts
	Quicksort(left, divided->prev);				//At first left part sends
	Quicksort(divided->next, right);			//Than right part
}

product* sales::partition(product* left, product* right) {	//Begin and end of the divided list
	product* rtrn = left->prev;
	product* traverse = left;	//Visits every node in divided list
	string s = right->data->country;
	float f = right->data->total_profit;	//String and float is for not searc same value more than one
	while (traverse != right) {				//Searech all the divided list
		if ((traverse->data->country < s) || ((traverse->data->country == s) && (traverse->data->total_profit > f)))	//Simple compare
		{
			if (!rtrn) {
				rtrn = left;	//We might have take NULL, if left is the first element it automaticly left
			}					//Else after first step it will be left, in first loop I simply check to guarentee
			else {
				rtrn = rtrn->next;
			}
			swap(traverse, rtrn);//Swap datas
		}
		traverse = traverse->next;	//Every time search different nodes
	}
	if (!rtrn) {	//Just like above
		rtrn = left;
	}
	else {
		rtrn = rtrn->next;
	}
	swap(rtrn, right);
	return rtrn;
}

void sales::swap(product* a, product* b) {	//To swap easily, I add extra data struct
	carry* temp = a->data;					//With that, I simply change data pointers using temp
	a->data = b->data;
	b->data = temp;
}

void sales::print(string first_line) {
	ofstream write_file("sorted.txt");	//New file opened
	product* traverse = head;
	carry* d;
	write_file << first_line << endl;
	while (traverse) {
		d = traverse->data;	//This step is for make faster, searching traverse->data->x each time costs more 
		write_file << d->country << "	" << d->item_type << "	" << d->order_id << "	" <<
			d->units_sold << "	" << d->total_profit << endl;
		traverse = traverse->next;
	}
	write_file.close();	//Close file
}

void sales::clear() {	//Avoiding memory leak
	product* traverse;
	while (head) {
		traverse = head;
		head = head->next;
		delete traverse->data;
		delete traverse;
	}
}

int main(int argc, char* argv[]) {

	ifstream file;
	file.open("sales.txt");	//open file

	if (!file) {
		cerr << "File cannot be opened!";
		return 1;
	}

	int get_i, N = std::stoi(argv[1]); //you should read value of N from command line
	float get_f;			//I use float and int due to while we read sorted.txt instead of sales.txt we cannot use stoi, it will give error
	string first_line, line;//If we compare strings, 123 will be smaller than 23, so float is fine

	getline(file, first_line); //this is the header line
	struct sales new_sale;
	new_sale.create();	//head == NULL, tail == NULL

	for (int i = 0; i < N; i++) {
		carry* newnode = new carry;
		getline(file, line, '\t'); //country (string)
		newnode->country = line;
		getline(file, line, '\t'); //item type (string)
		newnode->item_type = line;
		getline(file, line, '\t'); //order id (string)
		newnode->order_id = line;
		file >> get_i; //units sold (integer)
		newnode->units_sold = get_i;
		file >> get_f; //total profit (float)
		newnode->total_profit = get_f;
		getline(file, line, '\n'); //this is for reading the \n character into dummy variable.
		new_sale.add_product(newnode);
	}
	file.close();

	// The time function in the slides gives 0 sec for N = 10/100 so I used chrono easily
	auto start = chrono::high_resolution_clock::now();
	ios_base::sync_with_stdio(false);

	new_sale.Quicksort(new_sale.head, new_sale.tail);

	auto end = chrono::high_resolution_clock::now();
	double time_taken = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
	time_taken *= 1e-9;
	cout << fixed << time_taken << setprecision(9) << " sec" << endl;	//Only Quicksort time calculated, not all the program

	new_sale.print(first_line);	//Read to the sorted.txt file
	new_sale.clear();			//Avoid memory leak

	return 0;
}
