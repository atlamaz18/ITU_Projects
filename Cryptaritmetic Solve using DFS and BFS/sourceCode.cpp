/*
Burak Atlamaz
150180067
Compile with g++ -std=c++11 due to file reading
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <sys/time.h>
using namespace std;

class Node{
public:
   char letter;         //Node is for a letter so it holds value of letter
   vector<int> values;  //Every values is a row, just like matrix but only have 1 not .
   Node** child;        //Every node has childs so I hold them with pointer of pointer
   Node* next;          //Next pointer is for queue
   Node(char, int);     //Constructor
   ~Node();             //Destructor
};

Node::Node(char letter, int size){   //Matrix olmadan deniyorum vira Bismillah
   this->letter = letter;
   child = new Node*[size];
   for(int i = 0; i < size; i++){
      child[i] = NULL;
   }
}

Node::~Node(){ //delete_tree is the main destructor
   delete[] child;
}

class Tree{
   public:
   Node *root;
   int queue_size = 0, visited_nodes = 0, kept_in_memory = 0;;
   string names[4];  //Three variable and outputFileName
   vector <char> letters;
   vector <int> final_values;
   void create_tree(Node*, int);
   void delete_tree(Node*, int);
   bool is_valid(vector <int>);
   bool summation_rules(vector <int>);
   void DFS(Node*, vector <int>, bool);
   void BFS();
   void write_matrix(vector <int>);
   void print_all();
};

class Queue{
   public:
   int size;
   Node* head;
   Node* tail;
   Queue();
   void enqueue(Node*);
   Node* dequeue();
   bool is_empty();
};

Queue::Queue(){
   size = 0;
   head = NULL;
   tail = NULL;
}

void Queue::enqueue(Node* newnode){
   size++;

   if(tail){
      tail->next = newnode;
      tail = tail->next;
      return;
   }

   head = newnode;
   tail = newnode;
}

Node* Queue::dequeue(){
   if(!head){
      return NULL;
   }

   size--;

   if(head == tail){
      Node* n = head;
      head = NULL;
      tail = NULL;
      return n;
   }

   Node* n = head;
   head = head->next;
   return n;
}

bool Queue::is_empty(){
   return size == 0;
}

void Tree::create_tree(Node* traverse, int layer_num){
   if(traverse == root){
      for(int i = 0; i < 10; i++){
         traverse->child[i] = new Node(letters[layer_num], 10);  //Root has 10 child sue to any value is duplicated
         traverse->child[i]->values.push_back(i);   //Assign first letter for each value 0 to 9
         create_tree(traverse->child[i], layer_num + 1);   
      }   
   }
   else if(layer_num != letters.size()){
      int decrease_number = 0;
      for(int i = 0; i < 10; i++){     //For all times check for 10, which is total number of values
         bool is_used = false;
         for(int x = 0; x < traverse->values.size(); x++){
            if(traverse->values[x] == i){
               decrease_number++; //Hold value to find which values are used
               is_used = true;    //If this value is used before do not make new child for that value
            }
         }
         //This value is not used before, so we create a child for it
         if(!is_used){
            //For each layer we have decreasing number of child, to check this decreasing number ve used layer_num
            traverse->child[i - decrease_number] = new Node(letters[layer_num], 10 - layer_num);
            //We have to add values of parent to the child so it can hold all of the values
            for(int j = 0; j < traverse->values.size(); j++){
               traverse->child[i - decrease_number]->values.push_back(traverse->values[j]);
            }
            //After old values we add the last value which differs us from parent
            traverse->child[i - decrease_number]->values.push_back(i);
            //Now we have to do this steps until we have all of the childs
            create_tree(traverse->child[i - decrease_number], layer_num + 1);
         }
      }   
   }
}

void Tree::delete_tree(Node* node, int row){
   if(node->child[0] != NULL)
    {
        for(int i = 0; i < row; i++)
        {
            delete_tree(node->child[i], row - 1);
        }
    }
    delete node;
}

bool Tree::is_valid(vector <int> values){
   for(int i = 0; i < values.size(); i++){   // n(n+1)/2 combination
      //At first check any of the last digits are zero
      if(values[i] == 0 && (letters[i] == names[0][0]|| letters[i] == names[1][0]|| letters[i] == names[2][0])){
         return false;
      }
   }

   int v1 = 0, v2 = 0, v3 = 0, size = 0, counter = 0, m = 1;
   for(int i = names[0].size() - 1; i >= 0; i--){  //Search all the letters in s1
      for(int j = letters.size() - 1; j >= 0; j--){//Search all the letters in letter vector
         if(letters[j] == names[0][i]){   //If they are the same
            counter = j;
            v1 += m * values[j];
            m*= 10;     //Get the number, each time increase with 10^x
         }
      }
   }
   m = 1;
   for(int i = names[1].size() - 1; i >= 0; i--){
      for(int j = letters.size() - 1; j >= 0; j--){
         if(letters[j] == names[1][i]){
            counter = j;
            v2 += m * values[j];
            m*= 10;
         }
      }
   }
   m = 1;
   for(int i = names[2].size() - 1; i >= 0; i--){ 
      for(int j = letters.size() - 1; j >= 0; j--){
         if(letters[j] == names[2][i]){
            counter = j;
            v3 += m * values[j];
            m*= 10;
         }
      }
   }
   if(v3 == (v1 + v2)){
      return true;
   }
   return false;
}

bool Tree::summation_rules(vector <int> values){
   for(int i = 0; i < values.size(); i++){   // n(n+1)/2 combination
      //At first check any of the last digits are zero
      if(values[i] == 0 && (letters[i] == names[0][0]|| letters[i] == names[1][0]|| letters[i] == names[2][0])){
         return false;
      }
   }
   //En başta 0 var mı kontrol ettik
   //Zaten aynı rakamı 2 defa yazmadık create_tree içinde ona da dikkat ettik
   //Geriye sadece çarpımlar aynı mı o kaldı hadi aslanım benim yazarsın sen onu
   int i = 0;
   //At first we have to find 
   if(names[0].size() < names[1].size())  i = names[0].size() - 1;
   else  i = names[1].size() - 1;
   //Now we are searching in descending order, just like the sum
   while(i != -1){      //We pass the leftmost side
      int counter = 0, constraint;
      while(letters[counter] != names[0][i]){
         counter++;
      }
      if(values.size() < counter + 1)   return true;  //We cannot find
      constraint = values[counter];
      counter = 0;
      while(letters[counter] != names[1][i]){
         counter++;
      }
      if(values.size() < counter + 1)   return true;  //We cannot find
      constraint += values[counter];
      counter = 0;
      while(letters[counter] != names[2][i]){
         counter++;
      }
      if(values.size() < counter + 1)   return true;  //We cannot find
      if((values[counter] == constraint) || (values[counter] == (constraint + 10))){   //Check for carry, if total
         return false;                                                                 //sum equal or +1 return false
      }
      i--;
   }
   return true;
}

void Tree::DFS(Node* traverse, vector <int> values, bool finish){
   //Now we are in the parent of last node, so we can do operations in last node
   visited_nodes++; //Enter for values.size() time
   kept_in_memory--;
   if(traverse->values.size() == letters.size()){ //We are in the last layer right now
      if(is_valid(traverse->values)){
         write_matrix(traverse->values);
         finish = true; //Main plan in this, taking reference and it will be the same for all recursives
      }
   }
   else if(!finish){
      kept_in_memory += 10;
      for(int i = 0; i < 10 - traverse->values.size(); i++){  //Every node has "10 - layer" child
         if(summation_rules(values)){
            DFS(traverse->child[i], traverse->child[i]->values, finish);
            //kept_in_memory-= 1;

         }
         //Mantık şu, en derinde değilsen sırasıyla onun child'larına git ve leaf'e ulaş
         //for içinde olduğundan child'lar preorder çağırılacak, yani ilk başta en soldaki leaf
         //daha sonra ise onun sağı ... diye gideceksin, bitince geri dönüp bir sağdaki node üzerinden devam edecek      
      }
      visited_nodes--;
      kept_in_memory-= values.size();  //Exit for one time
   }
}

void Tree::BFS(){

   if(!root){  //Root is NULL so just return
      return;
   }

   Queue q;
   bool finish = false; //To get only one value

   q.enqueue(root);
   while(!q.is_empty()){
      Node* reference = q.head;//queue.front();
      if(reference->child[0] == NULL && !finish){       
         if(is_valid(reference->values)){
            write_matrix(reference->values);
         }
      }
      else if(!finish){ //If we are not in the last layer
         for(int i = 0; i < 10 - reference->values.size(); i++){  //We will add all of the childs to the queue
            if(summation_rules(reference->values)){
               q.enqueue(reference->child[i]);
               visited_nodes++;
            }   
         }   
      }
      if(q.size > kept_in_memory){
         kept_in_memory = q.size;
      }
      q.dequeue();
      //Continue looping until all the nodes visited
   }

}

void Tree::write_matrix(vector <int> values){

   ofstream output_file;
   string file_name = names[3];
   output_file.open(file_name);
   output_file << "\t0\t1\t2\t3\t4\t5\t6\t7\t8\t9\n";
   for(int i = 0; i < letters.size(); i++){
      char c = tolower(letters[i]);
      output_file << c;
      for(int j = 0; j < 10; j++){
         if(j == values[i]){
            final_values.push_back(values[i]);  //To print at the end
            output_file << "\t1";
         }
         else{
            output_file << "\t.";
         }
      }
      output_file << "\n";
   }
   output_file.close();

}

void Tree::print_all(){
   cout << "Number of the visited nodes: " << visited_nodes << endl;
   cout << "Maximum number of nodes kept in the memory: " << kept_in_memory << endl;  //En sonda bir fazla dequeue

   int i;
   for(i = 0; i < letters.size() - 1; i++){ //Last letter does not have ','
      cout << letters[i] << ": " << final_values[i] << ", ";
   }
   cout << letters[i] << ": " << final_values[i] << endl;
}

int main(int argc, char* argv[]){
   //Get the name of variables
   string file_name = argv[2];   file_name += " ";
   file_name += argv[3];      file_name += " ";
   file_name += argv[4];      file_name += ".txt";
       
   //Open the file and check is it opened
	ifstream input(file_name);
	if (!input.is_open()) {
		cout << "File cannot be opened" << endl;
		return 1;
	}
    
   //File opened, rest of it will be started
   vector <int> dummy;
   string dummy_line;
   char temp;
   getline(input, dummy_line);//Header line captured

   //Add names to the tree class
   Tree t;
   for(int i = 0; i < 4; i++){
      t.names[i] = argv[i + 2];
   }
   t.root = new Node('x', 10);  //Random letter which is not capital

   //Read letters from input fike
   while(true) {				
		input >> temp;	
      t.letters.push_back(temp - 32);
      getline(input, dummy_line);//Header line captured
		if (input.eof()) break;			//Last line does not readed twice
	}
   //Create tree
   t.create_tree(t.root, 0);

   //Start time
	const clock_t begin_time = clock();
   
   //Do DFS or BFS
   if(argv[1][0] == 'D'){
      cout << "Algorithm: DFS" << endl;
      bool finish = false;
      t.DFS(t.root, dummy, finish);
   }
   if(argv[1][0] == 'B'){
      cout << "Algorithm: BFS" << endl;
      t.BFS();
   }

   //End time, print time and values
	cout << "Running time: " << float( clock () - begin_time ) /  CLOCKS_PER_SEC << endl << "Solution: ";
   t.print_all();

   //delete tree andl close
   t.delete_tree(t.root, 10);
   return 0;
}
