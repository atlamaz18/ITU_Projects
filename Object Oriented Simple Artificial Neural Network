/* @Author
Student Name: Burak Atlamaz
Student ID: 150180067 */

#include <iostream>
#include <fstream> // for file-access
#include <cmath>   //for pow and max calculations
using namespace std;

class Neuron {      //Abstract base class
protected:
    double value, activated_value;   //protected variables
public:
    void setValue(const double &number) {
        value = number;
    }
    double getValue() {
        return value;
    }
    void setActivatedValue(const double &number) {
        activated_value = number;
    }
    double getActivatedValue() {
        return activated_value;     //getter and setter functions 
    }
    virtual void activate() = 0;    //pure virtual function
};  //Do not have destructor due to do not have any pointer

class SigmoidNeuron :public Neuron { //subclass of Neuron
public:
    void activate();    //Calculation function from z to to a
    SigmoidNeuron();    //Constructor
};
SigmoidNeuron::SigmoidNeuron() {
    setValue(1.0);
    setActivatedValue(1.0);
}
void SigmoidNeuron::activate() {
    double  x = getValue();
    const double EulerConstant = std::exp(1.0);
    x *= -1.0;
    const double result = 1.0 / (1 + pow(EulerConstant, x)); //a = f(z)
    setActivatedValue(result);
}   //Do not have destructor due to do not have any pointer

class ReluNeuron :public Neuron {    //subclass of Neuron
public:
    void activate();    //Calculation function from z to to a
    ReluNeuron();       //Constructor
};
ReluNeuron::ReluNeuron() {
    setValue(1.0);
    setActivatedValue(1.0);
}
void ReluNeuron::activate() {
    double x = getValue();
    const double result = std::max(0.0, x);  //a = f(z)
    setActivatedValue(result);
}   //Do not have destructor due to do not have any pointer

class LReluNeuron :public Neuron {   //subclass of Neuron
public:
    void activate();    //Calculation function from z to to a
    LReluNeuron();      //Constructor
};
LReluNeuron::LReluNeuron() {
    setValue(1.0);
    setActivatedValue(1.0);
}
void LReluNeuron::activate() {
    double x = getValue();
    const double result = max((0.1 * x), x); //a = f(z)
    setActivatedValue(result);
}   //Do not have destructor due to do not have any pointer

class Layer {
protected:
    Neuron** neuron_array;     //Holds neuron pointers which have subclasses of abstract base class
    int dest_number;    //To find how many object neuron_array has
public:
    Layer() {   //No variable constructor
        neuron_array = NULL;   //Avoid seg fault
    }
    Layer(const int &neuron_number, const int &type) {    //Two variable constructor
        neuron_array = new Neuron * [neuron_number];    //neuron_array for each layer_array
        dest_number = neuron_number;
        for (int i = 0; i < neuron_number; i++) {
            if (type == 0) {    //SigmoidNeuron number in input
                neuron_array[i] = new SigmoidNeuron();
            }
            else if (type == 1) {   //LReluNeuron number in input
                neuron_array[i] = new LReluNeuron();
            }
            else if (type == 2) {   //ReluNeuron number in input
                neuron_array[i] = new ReluNeuron();
            }
        }
    }
    Neuron* getNeuron(const int &index) {  //getter to reach value and activated_value
        return neuron_array[index];
    }
    ~Layer() {  //Destructor
        for (int i = 0; i < dest_number; i++) {
            delete[] neuron_array[i];
        }
        delete[] neuron_array;
    }   //To avoid memory leak
};

class Network {
protected:
    Layer** layer_array;    //Holds layer pointers which have neuron_arrays
    int dest_number;    //To find how array object layer_array has
public:
    Network(int** array, const int &layers) {  //Constructor
        layer_array = new Layer * [layers];   ////layer_array for each place
        dest_number = layers;
        for (int i = 0; i < layers; i++) {
            layer_array[i] = new Layer(array[0][i], array[1][i]);   //Reserve memory for each neuron_array
        }
    }
    ~Network() {    //Destructor
        for (int i = 0; i < dest_number; i++) {
            delete layer_array[i];
        }
        delete[] layer_array;

    }   //To avoid memory leak
    Layer* getLayer(const int &index) {    //getter to reach neuron_array that Layer class hold
        return layer_array[index];
    }
};

void calculate(Network* layer_array, int** array, const int &layer) { //Calculates activated_value and prints them
    const int old_neuron_size = array[0][layer - 1];
    double* x_vector = new double[old_neuron_size];          //One dimensional dynamic array 
    for (int i = 0; i < old_neuron_size; i++) {
        x_vector[i] = layer_array->getLayer(layer - 1)->getNeuron(i)->getActivatedValue();
    }  //previous layer's activated values
    //X vector replaced
    const int new_neuron_size = array[0][layer];
    double** w_matrice = new double* [new_neuron_size];      //Two dimensional dynamic array
    for (int i = 0; i < new_neuron_size; i++) {
        w_matrice[i] = new double[old_neuron_size];          //Every one dimension of that array
    }
    for (int i = 0; i < new_neuron_size; i++) {
        for (int j = 0; j < old_neuron_size; j++) {
            w_matrice[i][j] = 0.1;
        }
    }  //0.1 for matrice multiplication
    //w_matrice replaced
    double* z_vector = new double[new_neuron_size];     //One dimensional dynamic array
    for (int i = 0; i < new_neuron_size; i++) {
        z_vector[i] = 0;
    }
    for (int i = 0; i < new_neuron_size; i++) {
        for (int j = 0; j < old_neuron_size; j++) {
            z_vector[i] += w_matrice[i][j] * x_vector[j];
        }
    }  //activated_value vector before sum with b_vector and activate()
    //z_vector replaced with calculated variables
    double* b_vector = new double[new_neuron_size];          //One dimensional dynamic array
    //print part of function
    cout << "Layer " << layer << ":" << endl;
    for (int i = 0; i < new_neuron_size; i++) {
        b_vector[i] = 0.1;     //b_vector's every block is placed one by one
        z_vector[i] += b_vector[i];    //after that process it added to z_vector for activate()
        layer_array->getLayer(layer)->getNeuron(i)->setValue(z_vector[i]);
        layer_array->getLayer(layer)->getNeuron(i)->activate();
        cout << layer_array->getLayer(layer)->getNeuron(i)->getActivatedValue() << endl;
    }
    delete[] z_vector;     //delete one dimensional array
    delete[] x_vector;
    delete[] b_vector;

    for (int i = 0; i < new_neuron_size; i++) {
        delete[] w_matrice[i];
    }
    delete[] w_matrice;    //delete two dimensional array
}

int main(int argc, char* argv[]) {
    ifstream input(argv[1]);    //open the file
    if (input.is_open() && input.good()) {
        int layers;
        input >> layers;
        int** array = new int* [layers];    //2d array to hold all inputs
        for (int i = 0; i < layers; i++) {
            array[i] = new int[3];  //Column is always three, no need to define number
        }
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < layers; j++) {
                input >> array[i][j];
            }
        }   //File reading is done, all variables are in dynamic 2d array
        try {
            for (int i = 0; i < layers; i++) {
                if ((array[1][i] < 0) || (array[1][i] > 2)) {
                    throw 1;
                }
            }
            if (!input.eof()) {
                throw 2;
            }
        }
        catch (const int &i) {
            if (i == 1) {
                cout << "Unidentified activation function!" << endl;
            }
            else if (i == 2) {
                cout << "Input shape does not match!" << endl;
            }
            return 1;
        }
        //No exceptions, layers and neurons can be created
        Network* layer_array = new Network(array, layers);
        for (int i = 0; i < layers; i++) {
            layer_array->getLayer(0)->getNeuron(i)->setValue(array[2][i]);          //Replace layer 0
            layer_array->getLayer(0)->getNeuron(i)->setActivatedValue(array[2][i]); //Without any calculation
        }
        cout << "Layer 0:" << endl;
        for (int i = 0; i < array[0][0]; i++) {
            cout << layer_array->getLayer(0)->getNeuron(i)->getActivatedValue() << endl;
        }
        //Calculation will start from that line, function do not used due to it is not efficient in my program
        for (int i = 1; i < layers; i++) {
            calculate(layer_array, array, i);
        }
        for (int i = 0; i < layers; ++i) {
            delete[] array[i];
        }
        delete[] array;
        delete layer_array;
    }
    else {
        cout << "The Network.txt file cannot be opened." << endl;
        return 1;
    }
    input.close();
    return 0;
}
