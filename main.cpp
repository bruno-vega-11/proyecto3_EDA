#include <iostream>

using namespace std;

struct Node {
    long long int value;
    int size = 1; // nodos en el subarbol contado el mismo nodo
    Node* right = nullptr;
    Node* left = nullptr;
    Node* parent = nullptr;
    Node(long long int _v):value(_v){}
    Node(long long int _v,Node* _right,Node* _left,Node* _parent):value(_v),right(_right),left(_left),parent(_parent){}
};

class SplayTree {
    Node* root = nullptr;

    // Mantener size;
    void update_size(Node* x) {
        int left = x->left? x->left->size:0;
        int right = x->right? x->right->size:0;
        x->size = 1 + left + right;
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
        update_size(p);
        update_size(x);
    }

    void Splay(Node* x) {
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

        inorderTraversal(node->left);
        cout << node->value << " ";
        inorderTraversal(node->right);
    }

public:
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
            update_size(temp);
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
        update_size(node);
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

        update_size(tree1_modified);
        return tree1_modified;
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
            cout << "Consulta tipo A" << endl;
        } else if (wa == 'R') {
            cout << "Consulta tipo R" << endl;
        }
    }

    t.print();
    Node* wa = t.kth(3);
    cout << wa->value << endl;
}