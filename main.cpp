#include <iostream>
#include <vector>

using namespace std;

struct Node {
    long long int value;
    long long int add = 0; // value que falta sumar hijos y descendientes (nunca a si mismo)
    long long int min_subtree = -1; // minimo value en el subrabol contando el nodo mismo
    int size = 1; // nodos en el subarbol contado el mismo nodo
    bool rev = false; // reversa pendiente para hijos y descendientes (el nodo ya tiene sus hijos intercambiados)
    Node* right = nullptr;
    Node* left = nullptr;
    Node* parent = nullptr;
    Node(long long int _v):value(_v),min_subtree(_v){}
    Node(long long int _v,Node* _right,Node* _left,Node* _parent):value(_v),right(_right),left(_left),parent(_parent){}
};

class SplayTree {
public:
    Node* root = nullptr;

    // Mantener size y minsubtree;
    void update(Node* x) {
        int left_size,right_size;
        x->min_subtree = x->value;
        if (x->left) {
            left_size = x->left->size;
            x->min_subtree = min(x->left->min_subtree,x->min_subtree);
        } else {
            left_size = 0;
        }
        if (x->right) {
            right_size = x->right->size;
            x->min_subtree = min(x->right->min_subtree,x->min_subtree);
        } else {
            right_size = 0;
        }
        x->size = 1 + left_size + right_size;
    }

    // Rotacion
    void rotate(Node* x) {
        Node* p = x->parent;
        Node* g = p->parent;
        if (p->left == x) { // right
            p->left = x->right;
            if (x->right) {
                x->right->parent = p;
            }
            x->right = p;
        } else { // left
            p->right = x->left;
            if (x->left) {
                x->left->parent = p;
            }
            x->left = p;
        }
        p->parent = x;
        x->parent = g;
        if (g) {
            if (g->left == p) {
                g->left = x;
            } else {
                g->right = x;
            }
        } else {
            root = x;
        }
        update(p);
        update(x);
    }

    void Splay(Node* x) {
        vector<Node*> camino_pendiente;
        Node* temp = x;
        while (temp) {
            camino_pendiente.push_back(temp);
            temp = temp->parent;
        }
        int size_camino_pendiente = static_cast<int>(camino_pendiente.size());
        for (int i = size_camino_pendiente - 1; i >= 0;i--) {
            cascade_sum_k(camino_pendiente[i]);
        }
        while (x->parent) {
            Node* p = x->parent;
            Node* g = p->parent;
            if (!g) {
                rotate(x);
            } else {
                if ((g->left == p) == (p->left == x)) {
                    rotate(p);
                    rotate(x);
                } else {
                    rotate(x);
                    rotate(x);
                }
            }
        }
    }

    void inorderTraversal(Node* node) {
        if (node == nullptr)return;

        cascade_sum_k(node);
        inorderTraversal(node->left);
        cout << node->value << " ";
        inorderTraversal(node->right);
    }

    SplayTree() = default;

    // Insert (por valor) adaptado mantener size (pruebas)
    void insert(long long int value) {
        Node* new_node = new Node(value);

        if (!root) {
            root = new_node;
            return;
        }

        Node* curr = root;
        Node* anterior = nullptr;
        while (curr) {
            anterior = curr;
            curr = (value < curr->value)? curr->left:curr->right;
        }
        new_node->parent = anterior;
        if (value < anterior->value) {
            anterior->left = new_node;
        } else {
            anterior->right = new_node;
        }
        Node* temp = anterior;
        while (temp != nullptr) {
            update(temp);
            temp = temp->parent;
        }
        Splay(new_node);
    }

    // Insert (por posicion)
    void insertkth(int k,long long value) {
        Node* a = Split(k); // en root < k , a contiene >= k
        Merge(root, new Node(value)); //
        Merge(root,a);
    }

    // Busqueda nodo posición k
    Node* kth(int k) {
        Node* curr = root;
        while (curr != nullptr) {
            cascade_sum_k(curr); // propagar antes de bajar (necesario por la reversa)
            int izq = (curr->left)?curr->left->size:0;
            if (k < izq) {
                curr = curr->left;
            } else if (k == izq) {
                Splay(curr);
                return curr;
            } else {
                k -= izq + 1;
                curr = curr->right;
            }
        }
        return nullptr;
    }

    void print() {
        inorderTraversal(root);
        cout << "\n";
    }

    // dejar en root posiciones < k y devolver >= k
    Node* Split(int k) {
        if (!root) return nullptr;
        if (k <= 0) { // todo a la derecha
            Node* r = root;
            root = nullptr;
            return r;
        }
        if (k >= root->size) return nullptr; // todo a la izquierda
        Node* node = kth(k);
        root = node->left;
        root->parent = nullptr;
        node->left = nullptr;
        update(node);
        return node;
    }

    // une 2 arboles donde inorder es todo tree1 seguido de tree2
    // tree 1 y tree 2 son arboles sueltos
    Node* Merge(Node* tree1,Node* tree2) {
        if (tree1 == nullptr) {
            root = tree2;
            return tree2;
        }
        if (tree2 == nullptr) {
            root = tree1;
            return tree1;
        }
        root = tree1;
        Node* tree1_modified = kth(root->size - 1);
        tree1_modified->right = tree2;
        tree2->parent = tree1_modified;

        update(tree1_modified);
        return tree1_modified;
    }

    void cascade_sum_k(Node* node) {
        if (node->add != 0) {
            if (node->left) {
                sum_k(node->left,node->add);
            }
            if (node->right) {
                sum_k(node->right,node->add);
            }
            node->add = 0;
        }
        if (node->rev) {
            if (node->left) {
                reverse_k(node->left);
            }
            if (node->right) {
                reverse_k(node->right);
            }
            node->rev = false;
        }
    }

    // helper queryR
    void reverse_k(Node* node) {
        swap(node->left,node->right);
        node->rev = !node->rev;
    }

    // helper queryA
    void sum_k(Node* node,long long int k) {
        node->value += k;
        node->min_subtree+= k;
        node->add += k;
    }

    // se asume que l <= r
    long long int queryM(int l, int r) {
        Node* A = Split(l-1); //root = [0...l), A = [l...n]
        Node* izq = root;
        root = A;
        Node* B = Split(r-l+1); //root = [l...r], B = (r...n]
        long long rpta = root->min_subtree;
        Node* rango = root;
        Merge(izq,rango);
        Merge(root,B);
        return rpta;
    }

    // se asume que l <= r;
    void queryA(int l,int r,long long int x) {
        Node* A = Split(l-1); //root = [0...l), A = [l...n]
        Node* izq = root;
        root = A;
        Node* B = Split(r-l+1);//root = [l...r], B = (r...n]
        sum_k(root,x);
        // volver a unirlo
        Node* temp = root;
        Merge(izq,temp); // root = [0...l...r]
        Merge(root,B);   // root = [0...l...r...n]
    }

    //se asume que 1<=k<=n
    void queryE(int k) {
        Node* A = Split(k-1); //root = [0...k), A = [k...n]
        Node* izq = root;
        root = A;
        Node* B = Split(1); //root = [k], B = (k...n]
        delete root;
        Merge(izq,B);
    }

    // se asume que l <= r
    void queryR(int l,int r) {
        Node* A = Split(l-1); 
        Node* izq = root;
        root = A;
        Node* B = Split(r-l+1);
        reverse_k(root);
        // volver a unirlo
        Node* temp = root;
        Merge(izq,temp); 
        Merge(root,B);   
    }

    //se asume que l <= r
    void queryO(int l,int r,int k) {
        Node* A = Split(l-1); //root =[0...l), A =[l...n]
        Node* izq = root;
        root = A;
        Node* B = Split(r-l+1); //root = [l...r], B = (r...m]
        k %= r-l+1;
        if (k>0) {
            Node* X = Split(r-l+1 -k);
            Node* Y = root;
            Merge(X,Y);
        }
        // la rotacion deja igual
        // falta unir
        Node* temp = root;
        Merge(izq,temp);
        Merge(root,B);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    SplayTree t;

    int n,q;
    cin>>n>>q;

    for (int i = 0; i < n; i ++) {
        long long int a;
        cin >> a;
        t.insertkth(i,a);
    }

    for (int i = 0;i< q; i++) {
        char wa;
        cin >> wa;
        if (wa == 'A') {
            int l,r;
            long long int x;
            cin >> l >> r >> x;
            t.queryA(l,r,x);
        } else if (wa == 'R') {
            int l,r;
            cin >> l >> r;
            t.queryR(l,r);
        } else if (wa == 'E') {
            int k;
            cin >> k;
            t.queryE(k);
        } else if (wa == 'I'){
            int k;
            long long int x;
            cin >> k >> x;
            t.insertkth(k,x);
        } else if (wa == 'O') {
            int l,r,k;
            cin >> l >> r >> k;
            t.queryO(l,r,k);
        }else if (wa == 'M') {
            int l,r;
            cin >> l >> r;
            cout << t.queryM(l,r) << "\n";
        }
    }
}