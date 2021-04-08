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

    BTreeNode *search(int k);
    void remove(BTreeData k);
    void removeFromNonLeaf(int index);
    void fill(int index);
    void borrowFromPrev(int index);
    void borrowFromNext(int index);
    void merge(int index);
}; 
  
struct BTree { 
    BTreeNode *root; 
    int tree_degree;  
    
    void create(int);
    void insert(BTreeData data);
    void remove(BTreeData k); 
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
}//Temiz galiba add yaparken boku yedik bulamadim
void BTreeNode::remove(BTreeData k){
    //Key bul
    int index = 0;
    switch (k.key) {
        case 'x':
            while(k.x > keys[index].x && index < count)  ++index;
            break; 
        case 'y':
            while(k.y > keys[index].y && index < count)  ++index;
            break;
        case 'z':
            while(k.z > keys[index].z && index < count)  ++index;
            break;
    }
    switch (k.key) {
        case 'x':
            if(index < count && keys[index].x == k.x){
                if(leaf){  
                    for(int i = index + 1; i < count; ++i){
                        keys[i-1] = keys[i];
                    }
                    count--;    
                }
                else{
                    removeFromNonLeaf(index);
                }
            return; 
            }
        case 'y':
            if(index < count && keys[index].y == k.y){
                if(leaf){  
                    for(int i = index + 1; i < count; ++i){
                        keys[i-1] = keys[i];
                    }
                    count--;
                }
                else{   
                    removeFromNonLeaf(index);
                }
            return;
            }
        case 'z':
            if(index < count && keys[index].z == k.z){
                if(leaf){  
                    for(int i = index + 1; i < count; ++i){
                        keys[i-1] = keys[i];
                    }
                    count--;
                }
                else{
                    removeFromNonLeaf(index);
                }
                return;
            }
    }
    if(leaf){
        cout << "Key does not exist" << endl;
        return;
    }
    bool checker;
        if(index == count){
            checker = true;
        }
        else{
            checker = false;
        }
        if(child[index]->count < tree_degree){
            fill(index);
        }
        if(index > count && checker){
            child[index - 1]->remove(k);
        }
        else{
            child[index]->remove(k);    
        }
}
//Nonleaf Temiz
void BTreeNode::removeFromNonLeaf(int index){  
    BTreeData data = keys[index];

    if(child[index]->count >= tree_degree){
        BTreeNode* current = child[index];
        while(!current->leaf){
            current = current->child[current->count];
        }     
        BTreeData temp = current->keys[current->count - 1];
        keys[index] = temp;
        child[index]->remove(temp);
    }
    else if(child[index+1]->count >= tree_degree){
        BTreeNode* current = child[index + 1];
        while(!current->leaf){
            current = current->child[0];
        }
        BTreeData success =  current->keys[0]; 
        keys[index] = success;
        child[index+1]->remove(success);
    }
    else{
        merge(index);
        child[index]->remove(data);
    }
}
//Fill temiz
void BTreeNode::fill(int index){
    if(index != 0 && child[index - 1]->count >= tree_degree){
        borrowFromPrev(index);
    }
    else if(index != count && child[index + 1]->count >= tree_degree){
        borrowFromNext(index);
    }
    else{
        if(index != count){
            merge(index);
        }
        else{
            merge(index - 1);
        }
    }
}
//Prev Temiz
void BTreeNode::borrowFromPrev(int index){
    BTreeNode *c = child[index]; 
    BTreeNode *s = child[index - 1];

    for(int i = c->count - 1; i >= 0; --i){
        c->keys[i+1] = c->keys[i];
    }
    if(!c->leaf){
        for(int i = c->count; i >= 0; --i){
            c->child[i + 1] = c->child[i];
        }
    }

    c->keys[0] = keys[index - 1];
    if(!c->leaf){
        c->child[0] = s->child[s->count];
    }

    keys[index - 1] = s->keys[s->count - 1];
    c->count++;
    s->count--;    
}
//Next temiz
void BTreeNode::borrowFromNext(int index){
    BTreeNode *c = child[index]; 
    BTreeNode *s = child[index + 1];

    c->keys[(c->count)] = keys[index];
    if(!c->leaf){
        c->child[(c->count) + 1] = s->child[0];
    }

    keys[index] = s->keys[0];

    for(int i = 1; i < s->count; ++i){
        s->keys[i - 1] = s->keys[i];
    }

    if(!s->leaf){
        for(int i = 1; i <= s->count; ++i){
            s->child[i - 1] = s->child[i];
        }
    }
    c->count++;
    s->count--;    
}

void BTree::remove(BTreeData k){
    if(root){
        root->remove(k);

        if(root->count == 0){
            BTreeNode* temp = root;
            if(!root->leaf){
                root = root->child[0];
            }
            else{
                root = NULL;
            }
            delete temp;
        }
    }
    else{
        cout << "Tree is empty" << endl;
    }
}
//Merge temiz
void BTreeNode::merge(int index){
    BTreeNode* c = child[index];
    BTreeNode* s = child[index + 1];
    c->keys[tree_degree - 1] = keys[index];

    for(int i = 0; i < s->count; ++i){
        c->keys[i + tree_degree] = s->keys[i];
    }
    if(!c->leaf){
        for(int i = 0; i <= s->count; ++i){
            c->child[i + tree_degree] = s->child[i];
        }
    }
    for(int i = index + 1; i < count; ++i){
        keys[i - 1] = keys[i];
    }
    for(int i = index + 2; i <= count; ++i){
        child[i - 1] = child[i];
    }

    c->count += s->count + 1;
    count--;
    delete(s);
}
int main(){
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
    BTreeData delete_data;
    delete_data.key = key;
    switch(key){
        case 'x':
            cin >> delete_data.x;
        case 'y':
            cin >> delete_data.y;
        case 'z':
            cin >> delete_data.z;
    }
    b.remove(delete_data);
    b.root->traverse(); 
  
    return 0;
} 
