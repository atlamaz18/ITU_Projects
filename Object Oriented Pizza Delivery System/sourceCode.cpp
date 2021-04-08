/* @Author
Student Name: Burak Atlamaz		
Student ID: 150180067
Date: 06.04.2020 */

#include <iostream>
#include <stdio.h>
#include <cstdio>
#include <stdbool.h>
#include <fstream>
#include <string.h>
#include <string> 
#include <stdlib.h>
#include <limits>
using namespace std;		//Using namespace to access te functions

class Pizza{
	friend class Order;			//To use variables in other classes
	friend class OrderList;
	private: 
		string pizza_type;
		string size;
		string crust_type;
		string* ingredients;		//Dynamic array or linked list wanted from us
	public:
		Pizza* counterpart;			//Traverse in pizza list we  will create
		Pizza();
		Pizza(const Pizza &);		
		Pizza(int, string, string );
		~Pizza();
};

Pizza::Pizza(){						//The standart maded pizza with basic ingredients
	size = "medium";
	crust_type = "normal";
	ingredients = new string[6];
	ingredients[0] = "mozarella";
	counterpart = NULL;				//For not taking any segmentation fault
}

Pizza::Pizza(const Pizza &object_in){		//Copy Constructor + const variable, it cannot be changed
	cout << "Please enter the number of the ingredient you want to remove from your pizza"<< endl; 	//Asking due to delete ingredients
	pizza_type = object_in.pizza_type;			//object_in is not pointer
	size = object_in.size;						//it called with call by reference
	crust_type = object_in.crust_type;
	ingredients = new string[6];				
	int i;
	for(i=0;i<6;i++){
		ingredients[i] = object_in.ingredients[i];		//Ingredients printed to choose for delete
		cout << i+1 << ". " << ingredients[i] << endl;
	}
	cout << "Press 0 to save it." << endl;
	cin >> i;
	while(cin.fail()) { 				//If user entered non integer variable it will be checked
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(),'\n');
			cout << "Please enter an integer: " << endl;
			cin >> i;
	}
	
	while(i != 0){
		if(i < 7 && i > 0){
			ingredients[i - 1] = "-";						//The ingredients that will be delete changed one by one	
		}
		else{
			cout << "Wrong number of ingredient, please try again: ";
		}
		cin >> i;
		while(cin.fail()) { 				//If user entered non integer variable it will be checked
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(),'\n');
			cout << "Please enter an integer: " << endl;
			cin >> i;
	}
	}	

}

Pizza::Pizza(int pizza_type, string size, string crust_type ){
	this->size = size;
	this->crust_type = crust_type;
	counterpart = NULL;							//For not taking any segmentation fault
	ingredients = new string[6];				//Dynamic allocated memory
	switch(pizza_type){							//pizza_type integer to understand which strings will be printed and saved
		 case 1:
		 	this->pizza_type = "Chicken Pizza ";
			ingredients[0] = "mozarella";
			ingredients[1] = "chicken";
			ingredients[2] = "mushroom";
			ingredients[3] = "corn";
			ingredients[4] = "onion";
			ingredients[5] = "tomato";
            break;

        case 2:
        	this->pizza_type = "Broccoli Pizza ";
			ingredients[0] = "mozarella";
			ingredients[1] = "broccoli";
			ingredients[2] = "pepperoni";
			ingredients[3] = "olive";
			ingredients[4] = "corn";
			ingredients[5] = "onion";
            break;
            
        case 3:
        	this->pizza_type = "Sausage Pizza ";
			ingredients[0] = "mozarella";
			ingredients[1] = "sausage";
			ingredients[2] = "tomato";
			ingredients[3] = "olive";
			ingredients[4] = "mushroom";
			ingredients[5] = "corn";
            break;
			    
		default:								//There is not any other step in creating pizza object with given variables
			break;
	}
}
	
Pizza::~Pizza(){								//when delete called deconstructors also called
	delete[] ingredients;
	delete counterpart;
	//If we have pointer or allocated memory we have to free its memory in destructor
	//With that case we do avoid have any unused or unnecessary space
}

class Order{
	friend class Pizza;
	friend class OrderList;
	private:
		string customer;
		int *drink_order; 					//order[0] for check, the other elements for looking how many drink that customer wants
		Pizza* element;						//Dynamic array or linked list wanted from us
	public:												
		Order* next;					
		Order(string, Pizza*, int*);		//Order HAS pizza
		Order(string, Pizza*);				//So we can pass from order to order and from order to pizza
		~Order();							//IN this case we can travel in both pizza list ande order list
		float getPrice();
		void printOrder();
		
};

Order::Order(string customer, Pizza* newpizza){
	this->element = newpizza;				//Making order object that HAS pizza
	this->customer = customer;				//Drink orders willl be added when it asked
	drink_order = new int[5]; 
	int i;
	for(i = 0; i< 5;i++){
		drink_order[i] = 0;
	}
	next = NULL;							//For not taking any segmentation fault
}

Order::Order(string customer, Pizza* newpizza, int* drink ){
	this->element = newpizza;				//Making order object that HAS pizza
	this->customer = customer;				//Drink orders willl be added when it asked
	drink_order = new int[5]; 
	int i;
	for(i = 0; i< 5;i++){
		drink_order[i] = drink[i];
	}
	next = NULL;							//For not taking any segmentation fault
}

Order::~Order(){					//when delete called deconstructors also called
	delete element;
	delete[] drink_order;
	//If we have pointer or allocated memory we have to free its memory in destructor
	//With that case we do avoid have any unused or unnecessary space	
}

void Order::printOrder() {

	Pizza* travel;
	travel = element;
		
	cout << "------------" << endl;					//12 - as the homework want
	cout << "Name: " << customer << endl << endl;
	int i;
	while(travel){
		cout << travel->pizza_type << "(";
		for(i = 0 ; i < 6 ; i++){
			if(travel->ingredients[i] != "-"){
				cout << travel->ingredients[i] << ", ";
			}
		}
		cout << ")" << endl;
		cout << "size: " << travel->size << ", crust: " << travel->crust_type << endl << endl;
		travel = travel->counterpart;  
	}
	if(drink_order[0] != 0){
		if(drink_order[1] != 0)	cout << drink_order[1] << " cola, ";
		if(drink_order[2] != 0)	cout << drink_order[2] << " soda, ";
		if(drink_order[3] != 0)	cout << drink_order[3] << " ice tea, ";
		if(drink_order[4] != 0)	cout << drink_order[4] << " fruit juice, ";
	}
	cout << endl << "-------------" << endl ;						//13 - as the homework want
}

float Order::getPrice(){
	float pizza_number = 0, total_price = 0;
	if(drink_order[0] != 0){
		total_price += 4.0 * drink_order[1];			//All of the numbers in that func are float
		total_price += 2.0 * drink_order[2];			//Do not ewant to have any misunderstanding when the code readed by other people
		total_price += 3.0 * drink_order[3];
		total_price += 3.5 * drink_order[4];
	}
	Pizza* traverse = element;
	while(traverse){
		pizza_number += 1.0;							//All of the pizzas have same size
		traverse = traverse->counterpart;				//So the price is number*size_price
	}
	if(element->size == "small")	total_price += pizza_number * 15;
	else if(element->size == "medium")	total_price += pizza_number * 20;
	else if(element->size == "big")	total_price += pizza_number * 25;
	
	return total_price;									//Float return due to cents(Fruit juice is 3.5 TL )
}

class OrderList {
//	friend class Pizza;					//To use all of the variables when they are needed
//	friend class Order;
	private:
		int n;
		Order* head;					//When orderlist object created also order list will be created
	public:
		OrderList();
		void takeOrder();
		void listOrders();
		void deliverOrders();
		void login_screen();
		void pizza_menu();
		void drink_menu();
		bool perform_operation(int );
};

OrderList::OrderList() {
//	n = 0;
	head = NULL;				//Order list created
}

void OrderList::login_screen() {
    cout << "Welcome to Unicorn Pizza!" << endl;
    cout << "1. Add an order" << endl;
	cout << "2. List orders" << endl;
	cout << "3. Deliver order" << endl;
	cout << "4. Exit" << endl;
    cout <<"Choose what to do: ";			//Just like the assignment
}

void OrderList::pizza_menu() {
    cout << "Pizza Menu" << endl;
    cout << "1. Chicken Pizza (mozarella, chicken, mushroom, corn, olive, onion, tomato)" << endl;
	cout << "2. Broccoli Pizza (mozarella, broccoli, pepperoni, olive, corn, onion)" << endl;
	cout << "3. Sausage Pizza (mozarella, sausage, tomato, olive, mushroom, corn)" << endl;
    cout << "0. Back to main menu" << endl;			//Just like the assignment
}

void OrderList::drink_menu() {
	cout << "Please choose a drink:" << endl;
	cout << "0. no drink" << endl;
	cout << "1. cola 4 TL" << endl;
	cout << "2. soda 2 TL" << endl;
	cout << "3. ice tea 3 TL" << endl;
	cout << "4. fruit juice 3.5 TL" << endl;
	cout << "Press -1 to save your order" << endl;		//Just like the assignment
}

void OrderList::takeOrder(){					
	pizza_menu();
	int pizza_type;			
	cin >> pizza_type;									//The wanted pizza number is taken from the customer   
	while(cin.fail()) { 				//If user entered non integer variable it will be checked
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(),'\n');
			cout << "Please enter an integer: " << endl;
			cin >> pizza_type;
	} 
	while(pizza_type > 3 || pizza_type < 1){
		cout << "You have entered an invalid choice" << endl;		//For wrong number enterence
		cout << "Please try again {1, 2, 3}: ";
		cin >> pizza_type;
	}
    if (pizza_type == 0) {
    	return;				//Select order pizza and turn back
    }
   	else{
   		cout<< "Select size: small (15 TL), medium (20 TL), big (25 TL)" << endl;
   		string size;
   		cin >> size;
   		while((size != "small" ) && (size != "medium" ) && (size != "big")){
   			cout << "Wrong size, please try again" << endl;
   			cout<< "Select size: small (15 TL), medium (20 TL), big (25 TL)" << endl;
   			cin >> size;
		   }
		
		cout<< "Select crust type: thin, normal, thick" << endl;
		string crust_type;
   		cin >> crust_type;
   		while((crust_type != "thin" ) && (crust_type != "normal" ) && (crust_type != "thick")){
   			cout << "Wrong crust type, please try again" << endl;
   			cout<< "Select crust type: thin, normal, thick" << endl;
   			cin >> crust_type;
		   }
   		
   		cout<< "Enter the amount:";
   		int amount;
   		cin >> amount;
   		while(cin.fail()) { 				//If user entered non integer variable it will be checked
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(),'\n');
			cout << "Please enter an integer" << endl;
			cin >> amount;
		}
   		while(amount < 1){
   			cout << "Wrong number of amount, please try again" << endl;
   			cout << "Enter the amount:";
   			cin >> amount;
		   }
   		
		cout << endl;								//The variables which are needed for pizza are taken to make pizza and order class
		
		   		
   		Pizza *newpizza = new Pizza(pizza_type, size, crust_type);	//Pizza constructor called
		Order *neworder = new Order("Customer", newpizza);			//Order constructor called

		if(head == NULL){
			head = neworder;
			head->next = NULL;
		}	
		else{											
			Order* traverse;
			Order* holder;
			traverse = head;
			while(traverse){
				holder = traverse;
				traverse = traverse->next;
			}
			holder->next = neworder;
			neworder->next = NULL;
		}												//New orderobject is created and added into the last of order list	
	
		if(amount >1){	
			int i = amount;
			Pizza* tail;
			tail = newpizza;							//It holds head in the first time	
			for(; i >1 ; i--){																	
				Pizza* addpizza = new Pizza(*newpizza);
				tail->counterpart = addpizza;
				tail = addpizza;
				tail->counterpart = NULL;
			}
		}   											//Pizza is finished, turn for drink and customer          
   		
   		int drink_order;
		drink_menu();
		cin >> drink_order;
		while(cin.fail()){ 				//If user entered non integer variable it will be checked
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(),'\n');
			cout << "Please enter an integer" << endl;
			cin >> drink_order;
		}																	
		if(drink_order != 0){
			neworder->drink_order[0] = 1;
			while(drink_order != -1 && drink_order != 0){
				if(drink_order < 5 && drink_order > 0){
					neworder->drink_order[drink_order] += 1;
				}
				else{
					cout << "Wrong number of drink, please try again: ";
				} 
				cin >> drink_order;
				while(cin.fail()){ 				//If user entered non integer variable it will be checked
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(),'\n');
					cout << "Please enter an integer" << endl;
					cin >> drink_order;
				}
			}
		}										
		if(drink_order == 0){
			int xd;
			for(xd = 0 ; xd < 5 ; xd++){
				neworder->drink_order[xd] = 0;
			}	
		}															//Drink is finished, turn for customer		
		
		cout << "Please enter your name" << endl;
		cin >> neworder->customer;
		cout << endl;
		cout << "Your order is saved, it will be delivered when it is ready..." << endl;	 
		neworder->printOrder();  						//The order goes to the last, parameteers are not needed
	}
}

void OrderList::listOrders(){
	Order* traverse;									//To visit in the order list
	traverse = head;
	
	if(head == NULL){
		cout << "empty" << endl;						//Error message basically
	}
	else{
		int counter = 1;
		while(traverse){								//with while loop
			cout << counter << endl;					//All of the orders printed from head to the last	
			traverse->printOrder();						//and all of them are printed one by one with pizzas and drinks
			traverse = traverse->next;
			counter++;									//The number of order are counted and printed
		}							
	}
}

void OrderList::deliverOrders(){
	listOrders();										//The basic steps in the top of that func
	Order* traverse;									//Traverse in the order list
	Order* tail;										//Hold the last object we are into
	traverse = head;
	tail = traverse;
	
	if(traverse == NULL){
		return;											//Only one empty message, which in listOrders()
	}
	
	cout << "Please write the customer name in order to deliver his/her order: ";
	string find_name;
	cin >> find_name;	

	while( traverse && traverse->customer != find_name ){
		tail = traverse;
		traverse = traverse->next;
	}
	if(!traverse && tail->customer != find_name){
		cout << "Wrong name" << endl;
		return;												//Mistake message 														
	}														//The name is not one of the customers

	cout << "Following order is delivering..." << endl;
	tail->printOrder();										//Printing this customers order again
	float total_price = tail->getPrice();			
	cout << "Total price: " << total_price << endl;
	
	string promotion_ask;
	cout << "Do you have promotion code? (y/n)" << endl;
	cin >> promotion_ask;
	while (promotion_ask != "y" && promotion_ask != "Y" && promotion_ask != "N" && promotion_ask != "n" ){
		cout << "Wrong character, please try again: ";
		cin >> promotion_ask;
	}
	if(promotion_ask == "y" || promotion_ask == "Y"){		//Both two symbol of yes
		cout << "Please enter your code: ";					//If customer does not have any code directly skips this step
		string code;
		getline(cin,code);
		getline(cin,code);	
		if(code == "I am a student")	cout << "Discounted price: " << ( total_price * 9) / 10 << endl;	//Float aritmetics
		else 							cout << "Wrong number, there is not any discount" << endl;
	}
	cout << "The order is delivered successfully!" << endl << endl;
	// Only delivering the object and deconstractors are unaccomplished tasks.
	
	if(tail == head){
		head = head->next;
		delete tail;
		return;
	}
	traverse = head;
	while(traverse->next != tail && traverse){
		traverse = traverse->next;
	}
	if(traverse->next == tail){
		traverse->next = tail->next;
		delete tail;
	}
}												//With those steps the allocated memories are free


bool OrderList::perform_operation(int loginmenu_choice){
	bool terminate = false;
	 switch (loginmenu_choice) {
        case 1:										//Opens pizza menu
        	takeOrder();
            break;

        case 2:
			listOrders();							//Shows all the orders
            break;									//If there is not any, prints empty

        case 3:
			deliverOrders();						//deliver the order and delete that from orderlist
            break;
		
		case 4:
			cout << "Goodbye...";
			terminate = true;						//While loop is ended, EOF after scanning 4 number
			break;
			
       	default:
			cout << "You have entered an invalid choice" << endl;		//For wrong number enterence
			cout << "Please try again {1, 2, 3, 4}: ";
		
			
			cin >> loginmenu_choice;
		//	cout << endl;
			terminate = perform_operation(loginmenu_choice); 
            break;
        }
    
	return terminate;    
}

int main(){	
	bool end = false;					//For while loop for perform operation
	int loginmenu_choice;
	OrderList functionCall;				//Orderlist created, we have head and a list now
	while(!end){;
		functionCall.login_screen();
		cin >> loginmenu_choice;
		while(cin.fail()){ 				//If user entered non integer variable it will be checked
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(),'\n');
			cout << "Please enter an integer: " << endl;
			cin >> loginmenu_choice;
		}
		end = functionCall.perform_operation(loginmenu_choice);		//For holding while loop
	}
	return EXIT_SUCCESS;											//The end...
}
