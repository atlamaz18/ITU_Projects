#include<iostream> 
using namespace std; 
  
struct BTreeData{
    int x;
    int y;
    char z;
    char key;
};

struct BTreeNode {   
    BTreeData *keys;  
    int tree_degree;      
    BTreeNode **child; 
    int count;   
    bool leaf; 
  
    void insertNonFull(BTreeData k); 
    void splitChild(int i, BTreeNode *y); 
    void traverse(); 
}; 
  
struct BTree { 
    BTreeNode *root; 
    int tree_degree;  
    
    void create(int);
      
    void insert(BTreeData data); 
}; 

void BTree::create(int tree_degree) {
    root = NULL;
    this->tree_degree = tree_degree; 
} 
  
void BTreeNode::traverse() {
    for (int i = 0; i < count; i++) 
    { 
        cout << "(" << keys[i].x << "," <<keys[i].y << "," << keys[i].z << ")"; 
    } 
    cout << endl;
    int j;
    for(j = 0; j < count; j++){
        if(!leaf){
            child[j]->traverse();
        }
    }

    if (!leaf) 
        child[j]->traverse(); 
}

void BTree::insert(BTreeData b_data) { 
    if (root == NULL) { 
        BTreeNode* newnode = new BTreeNode();
        newnode->tree_degree = tree_degree;
        newnode->leaf = true;
        newnode->keys = new BTreeData[2*tree_degree-1];
        newnode->keys[0] = b_data;
        newnode->child = new BTreeNode* [2*tree_degree];
        newnode->count = 1;
        
        root = newnode; 
    } 
    else { 
        if (root->count == 2*tree_degree-1) { 
            BTreeNode* s = new BTreeNode;
            s->tree_degree = tree_degree;
            s->keys = new BTreeData[2*tree_degree-1];
            s->child = new BTreeNode* [2*tree_degree];
            s->child[0] = root;
            s->leaf = false;
            s->count = 0;
    
            s->splitChild(0, root); 
  
            int i = 0;
            switch (b_data.key) {
                case 'x':
                    if (s->keys[0].x < b_data.x){
                        i++; 
                    }
                    break; 
                case 'y':
                    if (s->keys[0].y < b_data.y){
                        i++; 
                    }
                    break;
                case 'z':
                    if (s->keys[0].z < b_data.z){
                        i++; 
                    }
                    break;
            } 

            s->child[i]->insertNonFull(b_data);
   
            root = s; 
        } 
        else {
            root->insertNonFull(b_data); 
        } 
    } 
} 
  
void BTreeNode::insertNonFull(BTreeData b_data) {  
    int i = count-1; 
  
    
    if (leaf) { 

        switch (b_data.key) {
            case 'x':
                while (i >= 0 && keys[i].x > b_data.x) 
                { 
                    keys[i+1] = keys[i]; 
                    i--; 
                }
                break; 
            case 'y':
                while (i >= 0 && keys[i].y > b_data.y) 
                { 
                    keys[i+1] = keys[i]; 
                    i--; 
                }
                break;
            case 'z':
                while (i >= 0 && keys[i].z > b_data.z) 
                { 
                    keys[i+1] = keys[i]; 
                    i--; 
                }
                break;
            } 
  
        keys[i+1] = b_data; 
        count++; 
    } 
    else { 
         switch (b_data.key) {
            case 'x':
                while (i >= 0 && keys[i].x > b_data.x) 
                { 
                    i--; 
                }
                break; 
            case 'y':
                while (i >= 0 && keys[i].y > b_data.y) 
                { 
                    i--; 
                }
                break;
            case 'z':
                while (i >= 0 && keys[i].z > b_data.z) 
                { 
                    i--; 
                }
                break;
            } 
  
        if (child[i+1]->count == 2*tree_degree-1) { 
            splitChild(i+1, child[i+1]); 

            switch (b_data.key) {
            case 'x':
                if (keys[i+1].x < b_data.x) 
                { 
                    i++; 
                }
                break; 
            case 'y':
                if (keys[i+1].y < b_data.y) 
                { 
                    i++; 
                }
                break;
            case 'z':
                if (keys[i+1].z < b_data.z) 
                { 
                    i++; 
                }
                break;
            }  
        } 
        child[i+1]->insertNonFull(b_data); 
    } 
} 

void BTreeNode::splitChild(int i, BTreeNode *y) { 
    BTreeNode* newnode = new BTreeNode;
    newnode->tree_degree = y->tree_degree;
    newnode->leaf = y->leaf;
    newnode->keys = new BTreeData[2*tree_degree-1];
    newnode->child = new BTreeNode* [2*tree_degree];
    newnode->count = tree_degree - 1;
    BTreeNode *z;
    z = newnode;
  
    for (int j = 0; j < tree_degree-1; j++) 
        z->keys[j] = y->keys[j+tree_degree]; 
  
    if (y->leaf == false) 
    { 
        for (int j = 0; j < tree_degree; j++) 
            z->child[j] = y->child[j+tree_degree]; 
    } 
  
    y->count = tree_degree - 1; 

    for (int j = count; j >= i+1; j--) 
        child[j+1] = child[j]; 
  
    child[i+1] = z; 
  
    for (int j = count-1; j >= i; j--) 
        keys[j+1] = keys[j]; 
  
    keys[i] = y->keys[tree_degree-1]; 
    count++; 
} 
  
int main() {   
    int node_count, tree_degree;
    char key;
    cin >> node_count >> tree_degree >> key;

    BTree b;
    b.create(tree_degree); 
    for(int i = 0; i < node_count; i++){
        BTreeData data;
        cin >> data.x >> data.y >> data.z;
        data.key = key;
        b.insert(data);
    } 
    b.root->traverse(); 
  
    return 0; 
}
