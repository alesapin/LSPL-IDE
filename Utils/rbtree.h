#ifndef RBTREE_H
#define RBTREE_H
#include <QDebug>
enum Color{
    RED,BLACK
};

template <typename K, typename V>
class RBTree
{
    struct Node{
        K key;
        V value;
        Node* parent;
        Node* left;
        Node* right;
        Color color;
        Node():parent(0),left(0),right(0),color(BLACK){}
        Node(K k, V v):key(k),value(v),parent(0),left(0),right(0),color(BLACK){}
    };
    typedef Node* Pointer;
    static Pointer NIL_NODE;
    Pointer root;
    std::size_t _size;

    void leftRotate(Pointer x);
    void rightRotate(Pointer x);
    void fixInsert(Pointer z);
    void fixRemove(Pointer z);
    void insert(Pointer z);
    Pointer remove(Pointer z);
    Pointer find(K key, Pointer r) const;
    Pointer treeSuccessor(Pointer x) const;
    void clearTree(Pointer r);
    Pointer initTree(Pointer myParent,Pointer myRoot,Pointer otherRoot);
public:
    RBTree();
    RBTree(const RBTree& other);
    RBTree &operator=(const RBTree& other);
    V& operator[](const K& key);
    void erase(const K& key);
    bool contains(const K& key) const;
    std::size_t size() const;
    virtual ~RBTree();


};
template <typename K, typename V>
typename RBTree<K,V>::Pointer RBTree<K,V>::NIL_NODE = new RBTree<K,V>::Node();
template <typename K, typename V>
RBTree<K,V>::RBTree():root(NIL_NODE),_size(0)
{
}
template <typename K, typename V>
RBTree<K,V>::RBTree(const RBTree<K,V> &other)
{
    clearTree(root);
    initTree(NIL_NODE,root,other.root);
    _size = other._size;
}
template <typename K, typename V>
RBTree<K,V> &RBTree<K,V>::operator=(const RBTree<K,V> &other)
{
    clearTree(root);
    root = initTree(NIL_NODE,root,other.root);
    _size = other._size;
    return *this;
}
template <typename K, typename V>
V& RBTree<K,V>::operator[](const K& key)
{
    Pointer current;
    if( (current = find(key,root)) != NIL_NODE){
        return current->value;
    } else{
        current = new Node();
        current->key = key;
        current->parent = NIL_NODE;
        current->left = NIL_NODE;
        current->right = NIL_NODE;
        insert(current);
        _size++;
        return current->value;
    }
}
template <typename K, typename V>
void RBTree<K,V>::erase(const K &key)
{
    Pointer current;
   if (root == NIL_NODE) return;
    if( (current = find(key,root)) != NIL_NODE){
        Pointer result = remove(current);
        _size--;
        delete result;
    }
}
template <typename K, typename V>
bool RBTree<K,V>::contains(const K &key) const
{
    return (find(key,root) != NIL_NODE);
}
template <typename K, typename V>
std::size_t RBTree<K,V>::size() const
{
    return _size;
}
template <typename K, typename V>
RBTree<K,V>::~RBTree()
{
    clearTree(root);
}
template <typename K, typename V>
void RBTree<K,V>::leftRotate(Pointer x)
{
    Pointer y = x->right;
    x->right = y->left;
    if(y->left != NIL_NODE){
        y->left->parent = x;
    }
    y->parent = x->parent;
    if(x->parent == NIL_NODE){
        root = y;
    }else if (x == x->parent->left){
        x->parent->left = y;
    }else{
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}
template <typename K, typename V>
void RBTree<K,V>::rightRotate(Pointer x)
{
    Pointer y = x->left;
    x->left = y->right;
    if(y->right != NIL_NODE){
        y->right->parent = x;
    }
    y->parent = x->parent;
    if(x->parent == NIL_NODE){
        root = y;
    } else if (x==x->parent->right){
        x->parent->right = y;
    }else{
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
}
template <typename K, typename V>
void RBTree<K,V>::fixInsert(Pointer z)
{
    while( z->parent->color  == RED){
        if(z->parent == z->parent->parent->left){
            Pointer y = z->parent->parent->right;
            if( y->color == RED){
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right){
                    z = z->parent;
                    leftRotate(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(z->parent->parent);
            }
        }else{
            Pointer y = z->parent->parent->left;
            if(y->color == RED){
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left){
                    z = z->parent;
                    rightRotate(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(z->parent->parent);
            }
        }
    }
    root->color = BLACK;
}
template <typename K, typename V>
void RBTree<K,V>::insert(Pointer z)
{
    Pointer y = NIL_NODE;
    Pointer x = root;
    while (x != NIL_NODE){
        y = x;
        if(z->key < x->key){
            x = x->left;
        }else{
            x= x->right;
        }
    }
    z->parent = y;
    if ( y == NIL_NODE) {
        root = z;
    } else if ( z->key < y->key){
        y->left = z;
    } else {
        y->right = z;
    }
    z->left = NIL_NODE;
    z->right = NIL_NODE;
    z->color =RED;
    fixInsert(z);
}
template <typename K, typename V>
typename RBTree<K,V>::Pointer RBTree<K,V>::remove(RBTree<K,V>::Pointer z)
{
    Pointer y , x;
    if(z->left == NIL_NODE || z->right == NIL_NODE){
        y = z;
    }else{
        y = treeSuccessor(z);
    }
    if(y->left != NIL_NODE){
        x = y->left;
    }else{
        x = y->right;
    }
    x->parent = y->parent;
    if(y->parent == NIL_NODE){
        root = x;
    }else{
        if(y == y->parent->left){
            y->parent->left = x;
        }else{
            y->parent->right = x;
        }
    }
    if(y!=z){
        z->key = y->key;
        z->value = y->value;
    }
    if(y->color == BLACK){
        fixRemove(x);
    }
    return y;
}
template <typename K, typename V>
typename RBTree<K,V>::Pointer RBTree<K,V>::find(K key, Pointer r) const
{
    //qDebug() << "LOOKING FOR:" <<key;

    while(r != NIL_NODE){
        if(key < r->key){
            r = r->left;
        }else if (key > r->key){
            r = r->right;
        } else {
            return r;
        }
    }
    return NIL_NODE;
}
template <typename K, typename V>
void RBTree<K,V>::clearTree(RBTree<K,V>::Pointer r)
{
    if(r->left != NIL_NODE){
        clearTree(r->left);
    }
    if(r->right != NIL_NODE){
        clearTree(r->right);
    }
    if(r==root){
        delete r;
        root = NIL_NODE;
        _size = 0;
    }else{
        delete r;
    }

}
template <typename K, typename V>
typename RBTree<K,V>::Pointer RBTree<K,V>::initTree(RBTree<K,V>::Pointer myParent, RBTree<K,V>::Pointer myRoot, RBTree<K,V>::Pointer otherRoot)
{
    myRoot = new Node(otherRoot->key,otherRoot->value);
    myRoot->color = otherRoot->color;
    myRoot->left = NIL_NODE;
    myRoot->right = NIL_NODE;
    myRoot->parent = myParent;
    if(otherRoot->left != NIL_NODE){
        myRoot->left = initTree(myRoot,myRoot->left,otherRoot->left);
    }
    if(otherRoot->right != NIL_NODE){
        myRoot->right = initTree(myRoot,myRoot->right,otherRoot->right);
    }
    return myRoot;
}

template <typename K, typename V>
typename RBTree<K,V>::Pointer RBTree<K,V>::treeSuccessor(RBTree<K,V>::Pointer x) const
{
    if(x->right != NIL_NODE){
        Pointer result = x->right;
        while(result->left != NIL_NODE){
            result = result->left;
        }
        return result;
    }
    Pointer y = x->parent;
    while( y != NIL_NODE && x == y->right){
        x = y;
        y= y->parent;
    }
    return y;
}
template <typename K, typename V>
void RBTree<K,V>::fixRemove(Pointer x){
    while(x != root && x->color == BLACK){
        if(x == x->parent->left){
            Pointer w = x->parent->right;
            if(w->color == RED){
                w->color = BLACK;
                x->parent->color = RED;
                leftRotate(x->parent);
                w = x->parent->right;
            }
            if(w->right->color == BLACK && w->left->color == BLACK ){
                w->color = RED;
                x = x->parent;
            } else{
                if (w->right->color == BLACK){
                    w->left->color = BLACK;
                    w->color = RED;
                    rightRotate(w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                leftRotate(x->parent);
                x = root;
            }
        }else{
            Pointer w = x->parent->left;
            if(w->color == RED){
                w->color = BLACK;
                x->parent->color = RED;
                rightRotate(x->parent);
                w = x->parent->left;
            }
            if(w->right->color == BLACK && w->left->color == BLACK){
                w->color = RED;
                x = x->parent;
            } else{
                if (w->left->color == BLACK){
                    w->right->color = BLACK;
                    w->color = RED;
                    leftRotate(w);
                    w=x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rightRotate(x->parent);
                x = root;
            }
        }
    }
    x->color = BLACK;
}

#endif // RBTREE_H
