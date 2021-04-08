/* @Author
Student Name: <Burak Atlamaz>
Student ID : <150180067>
Date: <11.01.2021> */
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

struct rb_max {
	int rebound;
	int assist;
	int point;
	string r_name;
	string a_name;
	string p_name;
};

struct rb_data {
	string name;	//Key Point
	string team;
	int rebounds;
	int assists;
	int points;
};

struct rb_node {
	rb_node* parent;
	rb_node* left;
	rb_node* right;
	rb_data* data;
	char color;	//RED-BLACK
	int depth;
};

struct rb_list {
	rb_node* root;
	void traverse(rb_node*);
	void depth(rb_node*, int);
	void rb_insert(rb_node*);
	rb_max tree_insert(rb_data*, rb_max);
	void right_rotate(rb_node*);
	void left_rotate(rb_node*);
	void print_max(rb_max, int);
	void create() {
		root = NULL;
	}
	void clear(rb_node*);
};

void rb_list::depth(rb_node* x_node, int depth_num) {
	if (x_node) {
		x_node->depth = depth_num;
		depth(x_node->left, ++depth_num);
		depth(x_node->right, ++depth_num);
	}
}

void rb_list::traverse(rb_node* x_node) {
	if (x_node) {
		for (int i = 0; i < x_node->depth; i++) {
			cout << "-";
		}
		if (x_node->color == 'b') {
			cout << "(BLACK) ";
		}
		else {
			cout << "(RED) ";
		}
		cout << x_node->data->name << endl;

		traverse(x_node->left);
		traverse(x_node->right);
	}
}

void rb_list::rb_insert(rb_node* x_node) {
																	//x_node == root does not send to this func, do not need to check
	while (x_node->color != 'b' && x_node->parent->color == 'r') {	//x_node changes in rotate functions, so I check it too

		//p[x] = left[p[p[x]] condition
		if (x_node->parent == x_node->parent->parent->left) {
			rb_node* aunt = x_node->parent->parent->right;

			//Case 1 -> Recolor, Page 54 
			if (aunt && aunt->color == 'r') {
				aunt->color = 'b';
				x_node->parent->color = 'b';
				x_node->parent->parent->color = 'r';
				x_node = x_node->parent->parent;
			}

			//Case 2 + Case 3 -> Case 2 falls into Case 3	
			else if (x_node == x_node->parent->right) {
				//Case 2 -> Left Rotate 
				x_node = x_node->parent;
				left_rotate(x_node);	//Rotate basis is B in page 54 -> B is parent

				//Case 3 -> Right Rotate
				x_node->parent->color = 'b';	//x_node changed in left_rotate -> do not use parent
				x_node->parent->parent->color = 'r';
				right_rotate(x_node->parent->parent);	//Rotate basis is C in page 54 -> C is grand_parent
			}

			//Case 3 -> Right Rotate
			else {
				x_node->parent->color = 'b';
				x_node->parent->parent->color = 'r';
				right_rotate(x_node->parent->parent);	//Rotate basis is C in page 54 -> C is grand_parent
			}
		}

		//p[x] != left[p[p[x]] condition -> Right and left swapped including rotates
		else {
			rb_node* aunt = x_node->parent->parent->left;

			//Case 1 -> Recolor, Page 54 
			if (aunt && aunt->color == 'r') {
				aunt->color = 'b';
				x_node->parent->color = 'b';
				x_node->parent->parent->color = 'r';
				x_node = x_node->parent->parent;
			}

			//Case 2 + Case 3 -> Case 2 falls into Case 3
			else if (x_node == x_node->parent->left) {
				//Case 2 -> Left Rotate
				x_node =x_node->parent; 
				right_rotate(x_node);	//Rotate basis is B in page 54 -> B is parent

				//Case 3 -> Right Rotate
				x_node->parent->color = 'b'; //x_node changed in left_rotate -> do not use parent
				x_node->parent->parent->color = 'r';
				left_rotate(x_node->parent->parent); // Rotate basis is C in page 54->C is grand_parent

			}

			//Case 3 -> Right Rotate
			else {
				x_node->parent->color = 'b';
				x_node->parent->parent->color = 'r';
				left_rotate(x_node->parent->parent);
			}
		}
		root->color = 'b';	//Root is always black, so after every while set root black to not get red->parent == red in top
	}
}

rb_max rb_list::tree_insert(rb_data* x_data, rb_max max_value) {

	//Add first rb_node aka head is empty
	if (!root) {
		//Insert values and NULL
		rb_node* x_node = new rb_node;
		x_node->data = x_data;
		x_node->depth = 0;
		x_node->color = 'b';	//Root is always black
		x_node->left = NULL;
		x_node->right = NULL;
		x_node->parent = NULL;

		//Replace
		root = x_node;
		return max_value;
	}

	//Add the other nodes
	rb_node	*tail, *traverse = root;
	//Find the last node aka parent
	while (traverse && traverse->data->name != x_data->name) {
		tail = traverse;
		if (traverse->data->name > x_data->name) {
			traverse = traverse->left;
		}
		else if(traverse->data->name < x_data->name){
			traverse = traverse->right;
		}
	}

		//Check the player is in rb_tree or not
	if (traverse && traverse->data->name == x_data->name) {
		//Increasing
		traverse->data->rebounds += x_data->rebounds;
		traverse->data->assists += x_data->assists;
		traverse->data->points += x_data->points;

		//Check for max values
		if(traverse->data->rebounds > max_value.rebound) {
			max_value.r_name= traverse->data->name;
			max_value.rebound = traverse->data->rebounds;
		}
		if (traverse->data->assists > max_value.assist) {
			max_value.a_name = traverse->data->name;
			max_value.assist = traverse->data->assists;
		}
		if (traverse->data->points > max_value.point) {
			max_value.p_name = traverse->data->name;
			max_value.point = traverse->data->points;
		}
	}
	//Add new player due to not in the rb_tree
	else {
		rb_node* x_node = new rb_node;
		x_node->data = x_data;
		x_node->color = 'r';
		x_node->left = NULL;
		x_node->right = NULL;
		x_node->parent = tail;	///Assign parent

		//Add new node to left-right leaf
		if (tail->data->name > x_node->data->name) {
			tail->left = x_node;
		}
		else {
			tail->right = x_node;
		}
		//Page 54 in slide
		rb_insert(x_node);
	}
	return max_value;
}

void rb_list::right_rotate(rb_node* x_node) {
	rb_node* rotate_node = x_node->left;
	x_node->left = rotate_node->right;

	if (rotate_node->right) {
		rotate_node->right->parent = x_node;
	}
	rotate_node->parent = x_node->parent;
	//x_node is root then rotate becomes new root
	if (!x_node->parent) {
		root = rotate_node;
	}
	//if x_node is a left child then adjust x_node's parent's left child or ...
	else if (x_node == x_node->parent->right) {
		x_node->parent->right = rotate_node;
	}
	// adjust x_node's parent's right child
	else {
		x_node->parent->left = rotate_node;
	}

	rotate_node->right = x_node; //the right child of rotate is now x_node
	x_node->parent = rotate_node; // the parent of x_node is now rotate
}

void rb_list::left_rotate(rb_node* x_node) {	//Swap some left and right only
	rb_node* rotate_node = x_node->right;
	x_node->right = rotate_node->left;

	if (rotate_node->left) {
		rotate_node->left->parent = x_node;
	}
	rotate_node->parent = x_node->parent;
	//x_node is root then rotate becomes new root
	if (!x_node->parent) {
		root = rotate_node;
	}
	//if x_node is a left child then adjust x_node's parent's left child or ...
	else if (x_node == x_node->parent->left) {
		x_node->parent->left = rotate_node;
	}
	// adjust x_node's parent's right child
	else {
		x_node->parent->right = rotate_node;
	}

	rotate_node->left = x_node;	//the left child of rotate is now x_node
	x_node->parent = rotate_node; // the parent of x_node is now rotate
}

void rb_list::print_max(rb_max max_value, int year) {
	cout << "End of the " << year<< " - " << year + 1<< " Season" << endl;
	cout << "Max Points: " << max_value.point << " - Player Name: " << max_value.p_name << endl;
	cout << "Max Assists: " << max_value.assist << " - Player Name: " << max_value.a_name << endl;
	cout << "Max Rebs: " << max_value.rebound << " - Player Name:  " << max_value.r_name << endl;
}

void rb_list::clear(rb_node* x_node) {
	if (!x_node) {
		return;
	}
	clear(x_node->left);
	clear(x_node->right);

	delete x_node->data;
	delete x_node;
}

int main(int argc, char* argv[]) {

	//Open the file
	ifstream input(argv[1]);
	if (!input.is_open()) {
		cout << "File cannot be opened" << endl;
		return 1;
	}

	//Start rb_list
	rb_list list_pointer;
	list_pointer.create();	//root = NULL
		
	//Set variables
	string line, name, team, cache;
	int year, rebound, assist, point, year_cache;
	bool year_bool = false, firstyear_bool = true;

	//Hold max player variables in there
	rb_max max_value;

	getline(input, line);//Header line captured
	//Read 
	while (getline(input, line)) {
		rb_data* new_data = new rb_data;

		stringstream ss(line);
		getline(ss, cache, '-');
		istringstream(cache) >> year;

		if (year_cache == year) {
			year_bool = true;
		}

		getline(ss, cache, ',');	//Year+1, not necessarily
		istringstream(cache) >> year_cache;

		//name
		getline(ss, name, ',');
		new_data->name = name;

		//team
		getline(ss, team, ',');
		new_data->team = team;

		//rebounds->convert to int
		getline(ss, cache, ',');
		istringstream(cache) >> rebound;
		new_data->rebounds = rebound;

		//assist->convert to int
		getline(ss, cache, ',');
		istringstream(cache) >> assist;
		new_data->assists = assist;

		//point->convert to int
		getline(ss, cache, ',');
		istringstream(cache) >> point;
		new_data->points = point;

		//Before new year adding, print best of last year
		if (year_bool) {
			year_bool = false;
			list_pointer.print_max(max_value, year - 1);
			if (firstyear_bool) {
				list_pointer.depth(list_pointer.root, 0);
				list_pointer.traverse(list_pointer.root);
				firstyear_bool = false;
			}
		}

		//Add new node to black_red_tree, update best of year
		max_value = list_pointer.tree_insert(new_data, max_value);

		//Check for max -> Works for 1st season
		if (new_data->rebounds > max_value.rebound) {
			max_value.r_name = new_data->name;
			max_value.rebound = new_data->rebounds;
		}
		if (new_data->assists > max_value.assist) {
			max_value.a_name = new_data->name;
			max_value.assist = new_data->assists;
		}
		if (new_data->points > max_value.point) {
			max_value.p_name = new_data->name;
			max_value.point = new_data->points;
		}
		
	}

	//Print one more time due to year skip does not happen
	list_pointer.print_max(max_value, year);
	list_pointer.clear(list_pointer.root);
	return 0;
}
