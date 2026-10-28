#include <iostream>

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
                if ((g->left == p)&&(p->left == x)) {
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

    Node* Search(int v) {
        Node* curr = root;

    }


};