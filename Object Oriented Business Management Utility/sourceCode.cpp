/* @Author
Student Name: Burak Atlamaz		
Student ID: 150180067
Date: 04.05.2020 */

#include<iostream>
#include<string>
using namespace std;		//Using namespace to access te functions

class Person{								//Person class
	string name;							//Both Courier and Owner class has it'S attributes					
	string surname;							//Name and surname are private
	public:
		void setName(string name){
			this->name = name;				//Set name
		}									//We cannot change name without call that func
		void setSurname(string surname){
			this->surname = surname;		//Set surname 
		}									//We cannot change surname without call that func
		string getName(){
			return name;					//Only way to get name due to it is private
		}
		string getSurname(){
			return surname;					//Only way to get surname due to it is private
		}
};

class Owner:public Person{					//Subclass of Person
	float ownership;						//Holds how many owner does this business have
	public:
		void setOwnership(float number){
			ownership = number;
		}
		float getOwnership(){
			return ownership;
		}
		Owner(string, string);				//Does not have destructor owing to there is not any pointer
		Owner();
};

Owner::Owner(){								//To make an owner object without any parameters	
	setName("-1");							//After create it will be changed, this is like a draft
	setSurname("-1");
	setOwnership(1);
}

Owner::Owner(const string name, const string surname){	//To make an owner object with given parameters
	setName(name);
	setSurname(surname);
}

class Courier:public Person{				//Subclass of Person 
	string vehicle;
	public:
		Courier* next_courier;				//Connects courier objects
		void setVehicle(string vehicle){
			while( (vehicle != "car") && (vehicle != "motorcycle") && (vehicle != "bicycle") ){
				cout << "Error! Invalid vehicle name for " << getName() << " " << getSurname() << ". Please write it again: ";
				cin >> vehicle;				//If vehicle is wrong setter will ask again to make it correct
			}
			this->vehicle = vehicle;
		}
		string getVehicle(){				//Only way to get vehicle because of it is private
			return this->vehicle;
		}
		Courier(const string, const string, const string);
		~Courier();
		bool operator==(Courier&) ;			//Looks that the couriers equal or not
};
											//Compare two couirer in terms of properties
bool Courier::operator==(Courier& x) {		//Does not const, it gives an error for getters
	const bool terminate = false;			//We does not learn getters in class so I cannot fix it
	if(x.getName() != getName())				return terminate;
	else if(x.getSurname() != getSurname())		return terminate;
	else if(x.getVehicle() != getVehicle())		return terminate;
	return !terminate; 
}

Courier::Courier(const string name, const string surname, const string vehicle){
	next_courier = NULL;					//Prevent segmentation fault
	setName(name);							//Replace given attributes
	setSurname(surname);
	setVehicle(vehicle);	
}

Courier::~Courier(){
	delete next_courier;					//Prevent memory loss
}	

class Business{						
	string business_name;
	string business_address;
	Owner* owner_array;
	Courier* head_courier;
	public:
		void setBusinessName(const string name){
			business_name = name;		//Set business name
		}								//We cannot change name without call that func
		string getBusinessName(){
			return business_name;
		}
		void setBusinessAddress(const string address){
			business_address = address;	//Set business name
		}								//We cannot change name without call that func
		string getBusinessAddress(){
			return business_address;
		}
		void setOwnerArray(Owner* array, const int number_of_arrays){
			owner_array = new Owner[number_of_arrays];	//Memort allocation with dynamic array
			int i = 0;
			for(; i < number_of_arrays; i++){
				owner_array[i] = array[i];				//Replace owners to array
				owner_array[i].setOwnership(number_of_arrays);
			}
		}
		Owner getOwnerArray_Index(const int number){
			return owner_array[number];
		}
		void hire_courier(Courier& );
		void fire_courier(Courier& );
		int calculate_shipment_capacity();
		void list_couriers();
		void list_owners();
		void operator()();
		Courier operator[](int);
		Business(string, string, Owner*, const float);
		~Business(){
			Courier* p;
			p = head_courier;
			while(p){
				head_courier = head_courier->next_courier;
				delete p;
				p = head_courier;
			}
			delete[] owner_array;
		}	
};

Business::Business(const string name, const string address, Owner* owner_array, const float number_of_arrays){	//FINISHED
	head_courier = NULL;							//Add given variables to the business
	this->owner_array = NULL;
	setBusinessName(name);
	setBusinessAddress(address);
	setOwnerArray(owner_array, number_of_arrays);	
}

void Business::list_owners(){									
	int i = 0;
	Owner* node = new Owner;
	*node = getOwnerArray_Index(i);
	const int owner = node->getOwnership();
	for(; i < owner; i++){
		*node = getOwnerArray_Index(i);
		cout << node->getName() << " " << node->getSurname() << " " << 100 / node->getOwnership() << endl;;
	}
	delete node;
}

void Business::hire_courier(Courier& old_courier){		
	Courier* new_courier = new Courier(old_courier);	//New memory allocation
	if(head_courier == NULL){				//Adding first courier
		head_courier = new_courier;
		head_courier->next_courier = NULL; 
	}
	else if(head_courier->next_courier == NULL){
		head_courier->next_courier = new_courier;
	}
	else{									//Adding to the last place
		Courier *tail, *traverse;
		traverse = head_courier;			//Start travel from head
		while(traverse){
			tail = traverse;
			traverse = traverse->next_courier;	//Finding the last place
		}
		tail->next_courier = new_courier;		//Last place is new coureir
		new_courier->next_courier = NULL;
	}
}

void Business::list_couriers(){					//List from first to the last
	if(head_courier == NULL){
		cout << "There is not any courier. " << endl;	//Error message
	}
	else{
		Courier *traverse;
		traverse = head_courier;				//Start travel from head
		while(traverse){
			cout << traverse->getName() << " ";
			cout << traverse->getSurname() << " ";
			cout << traverse->getVehicle() << endl;	//Print all attributes
			traverse = traverse->next_courier;		//And pass to the next
		}
	}
}

void Business::fire_courier(Courier& search){
	search.next_courier = NULL;			//Prevent segmentation fault
	Courier *tail, *traverse;
	if(head_courier == NULL){			//There is not any courier to fire
		return;
	}
	else if((search.getName() == "-1") && (search.getSurname() == "-1") ){
		return;							//[] operator find anything and send error courier
	}
	else if( (search == *head_courier ) ){	//Fire first courier
		traverse = head_courier;
		head_courier = head_courier->next_courier;	//Head is second courier
		traverse->next_courier = NULL;
		delete traverse;							//First one deleted, head is first again
		return;
	}
	else{
		traverse = head_courier;
		while(traverse && !( *traverse == search) ){	//Searching the courier
			tail = traverse;
			traverse = traverse->next_courier;
		}
		if(traverse == NULL){
			return;
		}
		else if( *traverse == search ){						//Courier founded		
			tail->next_courier = traverse->next_courier;
			traverse->next_courier = NULL;
			delete traverse;
			return;
		}
	}	
}

void Business::operator()() {		//Overload ()
	cout << getBusinessName() << " " << getBusinessAddress() << endl;
	list_owners();
	list_couriers();
}

Courier Business::operator[](int index){	//Overload []
	while(true){							//Function ends when courier with given index founded
		index++;
		if(head_courier == NULL){
			cout << "There is not any courier. " << endl;;
			Courier unnecessary("-1","-1","car");
			unnecessary.next_courier = NULL;
			return unnecessary;
		}
		else if(index < 1){					//Wrong input
			cout << "Index must be bigger than zero, ";
		}
		else if(index == 1){				//Head founded
			return *head_courier;
		}
		else{
			Courier *tail, *traverse;
			traverse = head_courier;
			int counter = 0;
			while( (counter < index) && traverse ){		//Search courier
				tail = traverse;
				traverse = traverse->next_courier;
				counter++;
			}
			if( (counter == index) ){		//Courier founded
				return *tail;
			}	
			cout << "Invalid index, ";		//Courier cannot founded
		}
			cout << "Please write the number of courier again: ";
			cin >> index;
	}
}

int Business::calculate_shipment_capacity(){				//FINISHED
	Courier *traverse;
	traverse = head_courier;	//Start from first courier
	int sum = 0;
	while(traverse){		//Exit when there is not any courier
		if( traverse->getVehicle() == "bicycle")	sum += 10;
		else if( traverse->getVehicle() == "motorcycle")	sum += 35;
		else	sum += 200;					//Add it to total sum
		traverse = traverse->next_courier;	//Visit every courier
	}	
	return sum;
}

int main(){
    // Create owners of the business
    //Constructor parameters: name, surname
    Owner o1 = Owner("Owner_name1", "surname1");
    Owner o2 = Owner("Owner_name2", "surname2");
    Owner* owner_arr = new Owner[2];
    owner_arr[0] = o1;
    owner_arr[1] = o1; 
    //Crete the business itself
    //Constructor parameters: name, address, owner_array, number_of_owners
    Business atlas = Business("Atlas", "Maslak Istanbul/Turkey", owner_arr, 2);
    
    // Print owner info to screen: name, surname, ownership
    atlas.list_owners();

    // Add new employees
    // Constructor parameters: name, surname, vehicle_type
    Courier c1 = Courier("Courier", "surname1", "car");
    Courier c2 = Courier("Courier", "surname2", "motorcycle");
    Courier c3 = Courier("Courier", "surname3", "motorcycle");  

    atlas.hire_courier(c1);
    atlas.hire_courier(c2);
    atlas.hire_courier(c3);


    // Print business info to screen
    // Name, address, owners, couriers
  	atlas();

    // Get courier with index 1 and remove it from the list/array
    Courier cour = atlas[1];
    atlas.fire_courier(cour);
    
    
    // Print remaining couriers
    atlas.list_couriers();
    
    // Print current maximum shipment capacity
    std::cout << atlas.calculate_shipment_capacity() << std::endl;
    return 0;
}
