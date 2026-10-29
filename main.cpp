#include <iostream>

using namespace std;

struct Node {
    int value;
    Node* right = nullptr;
    Node* left = nullptr;
    Node* parent = nullptr;
    Node(int _v):value(_v){}
    Node(int _v,Node* _right,Node* _left,Node* _parent):value(_v),right(_right),left(_left),parent(_parent){}
};

class SplayTree {
    Node* root = nullptr;

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
public:
    SplayTree() = default;

    void insert(int value) {
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
        Splay(new_node);
    }

    void inorderTraversal(Node* node) {
        if (node == nullptr)return;

        inorderTraversal(node->left);
        cout << node->value << " ";
        inorderTraversal(node->right);
    }

    void print() {
        inorderTraversal(root);
        cout << "\n";
    }
};

int main() {
    SplayTree t;
    int arr[] = {5,3,8,1,4,7,9};
    for (int i=0;i<7;i++) {
        t.insert(arr[i]);
    }
    t.print();
}