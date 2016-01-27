#ifndef INTERVALRBTREE_H
#define INTERVALRBTREE_H
#include <QVector>
#include <QString>
#include <exception>
#include <stdexcept>
#include <QDebug>
enum Color{
    RED,BLACK
};
class IntervalNotFoundException : public std::runtime_error{
private:
    int start;
    int end;
public:
    IntervalNotFoundException(int start,int end):runtime_error("Interval not found"),start(start),end(end){}
    virtual const char* what() const throw(){
        return QString("Interval ["+QString::number(start)+","+QString::number(end)+"] was not found").toStdString().c_str();
    }
};

template<typename T>
class IntervalRBTree
{

public:
    struct Interval{
        friend class IntervalRBTree;
        int low;
        int high;
        T value;
    private:
        int max;
        Interval* parent;
        Interval *left;
        Interval* right;
        Color color;
        Interval():low(0),high(0),max(0),parent(0),left(0),right(0),color(BLACK){}
        Interval(int l,int h,const T& v):low(l),high(h),max(h),value(v),parent(0),left(0),right(0),color(BLACK){}
    };
    typedef Interval* Pointer;
    class iterator{
        friend class IntervalRBTree;
    public:
        iterator():current(NIL_NODE){}
        iterator(const iterator&o){current = o.current;}
        ~iterator(){}
        iterator& operator=(const iterator&o){current = o.current;return *this;}
        iterator& operator++(){current = treeSuccessor(current); return *this;}
        bool operator==(const iterator& o){return current == o.current;}
        bool operator!=(const iterator& o){return current != o.current;}
        Interval operator*() const {return *current;}
        Interval* operator->() const {return current;}
    private:
        Pointer current;
    };
friend class iterator;
private:
    static Pointer NIL_NODE;
    Pointer root;
    unsigned _size;
    void leftRotate(Pointer x);
    void rightRotate(Pointer x);
    void fixInsert(Pointer z);
    void fixRemove(Pointer z);
    void insert(Pointer z);
    Pointer remove(Pointer z);
    Pointer intervalSearch(int i,int j, Pointer r) const;
    static Pointer treeSuccessor(Pointer x);
    Pointer findEqual(int low,int high) const;
    void clearTree(Pointer r);
    Pointer initTree(Pointer myParent,Pointer myRoot,Pointer otherRoot);
    int maximum( int a, int b ) { return a>b ? a : b ; }
    int maximum( int a, int b, int c ) { return maximum( maximum(a,b), c ) ; }
    bool intersects(int l1,int h1,int l2,int h2) const{
        return l1<=h2 && h1>=l2;
    }
    void recalcMax(Pointer p);
    void findAllIntersection(Pointer root,int low,int high,QVector<Pointer>* result) const;
public:
    IntervalRBTree();
    IntervalRBTree(const IntervalRBTree& other);
    IntervalRBTree &operator=(const IntervalRBTree& other);
    //Любой пересекающийся
    T& getFirstIntersection(int low,int high) const throw(IntervalNotFoundException);
    T& getFirstIntersection(int point) const throw(IntervalNotFoundException);
    T& getEqualInterval(int low,int high) const throw(IntervalNotFoundException);
    QVector<T> getAllIntersections(int low,int high) const;
    QVector<Interval> getAllIntervals() const;
    bool addInterval(int low,int high, const T& value);
    bool erase(int low,int high);
    void clear();
    bool containsIntersection(int low,int high) const;
    bool containsEqualInterval(int low,int high) const;
    unsigned size() const;
    bool empty() const {return size() == 0;}
    virtual ~IntervalRBTree();
    iterator begin() const{
        Pointer tmp = root;
        while(tmp->left != NIL_NODE){
            tmp = tmp->left;
        }
        iterator it;
        it.current = tmp;
        return it;
    }
    iterator end() const{
        iterator it;
        it.current = NIL_NODE;
        return it;
    }
};

template <typename T>
typename IntervalRBTree<T>::Pointer IntervalRBTree<T>::NIL_NODE = new IntervalRBTree<T>::Interval();
template <typename T>
IntervalRBTree<T>::IntervalRBTree():root(NIL_NODE),_size(0)
{
    root->left = NIL_NODE;
    root->right = NIL_NODE;
}
template <typename T>
IntervalRBTree<T>::IntervalRBTree(const IntervalRBTree<T> &other)
{
    clearTree(root);
    initTree(NIL_NODE,root,other.root);
    _size = other._size;
}
template <typename T>
IntervalRBTree<T> &IntervalRBTree<T>::operator=(const IntervalRBTree<T> &other)
{
    if(other.root != this->root){
        clearTree(root);
        root = initTree(NIL_NODE,root,other.root);
        _size = other._size;
    }
    return *this;
}

template <typename T>
T& IntervalRBTree<T>::getFirstIntersection(int low, int high) const throw(IntervalNotFoundException)
{
    Pointer x = root;
    while(x != NIL_NODE && !intersects(x->low,x->high,low,high)){
        if(x->left != NIL_NODE && x->left->max >= low){
            x = x->left;
        }else{
            x= x->right;
        }
    }
    if(x == NIL_NODE) throw IntervalNotFoundException(low,high);
    return x->value;
}
template <typename T>
T& IntervalRBTree<T>::getFirstIntersection(int point) const  throw(IntervalNotFoundException)
{
    return getFirstIntersection(point,point);
}
template <typename T>
T& IntervalRBTree<T>::getEqualInterval(int low, int high) const throw(IntervalNotFoundException)
{
    Pointer x = findEqual(low,high);
    if(x == NIL_NODE) throw IntervalNotFoundException(low,high);
    return x->value;
}
template <typename T>
QVector<T> IntervalRBTree<T>::getAllIntersections(int low, int high) const
{
   QVector<Pointer> tmp;
    findAllIntersection(root,low,high,&tmp);
    QVector<T> result(tmp.size());

    for(int i =0;i<tmp.size();++i){
        result[i] = tmp[i]->value;
    }
    return result;
}
template <typename T>
bool IntervalRBTree<T>::addInterval(int low, int high, const T &value)
{
    Pointer current = findEqual(low,high);
    if(current != NIL_NODE){
        current->value = value;
        return true;
    }else{
        Pointer current = new Interval(low,high,value);
        current->left = NIL_NODE;
        current->right = NIL_NODE;
        current->parent = NIL_NODE;
        insert(current);
        _size++;
        return false;
    }
}
template <typename T>
bool IntervalRBTree<T>::erase(int low, int high)
{
    Pointer current;
    if((current = findEqual(low,high))!= NIL_NODE){
        remove(current);
        --_size;
        return true;
    }
    return false;
}
template <typename T>
void IntervalRBTree<T>::clear()
{
    clearTree(root);
}
template <typename T>
bool IntervalRBTree<T>::containsIntersection(int low, int high) const
{
    Pointer x = root;
    while(x != NIL_NODE &&  !intersects(x->low,x->high,low,high)){
        if(x->left != NIL_NODE && x->left->max >= low){
            x = x->left;
        }else{
            x= x->right;
        }
    }
    return x!=NIL_NODE;
}
template <typename T>
bool IntervalRBTree<T>::containsEqualInterval(int low, int high) const
{
    return findEqual(low,high) != NIL_NODE;
}
template <typename T>
unsigned IntervalRBTree<T>::size() const
{
    return _size;
}
template <typename T>
IntervalRBTree<T>::~IntervalRBTree()
{
    clearTree(root);
}

template<typename T>
void IntervalRBTree<T>::leftRotate(IntervalRBTree<T>::Pointer x)
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
    recalcMax(x);
}
template<typename T>
void IntervalRBTree<T>::rightRotate(IntervalRBTree<T>::Pointer x)
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
    recalcMax(x);
}
template<typename T>
void IntervalRBTree<T>::fixInsert(IntervalRBTree<T>::Pointer z)
{
    while( z->parent->color  == RED){
        if(z->parent == z->parent->parent->left){
            Pointer y = z->parent->parent->right;
            if( y->color == RED){
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                recalcMax(z);
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
                recalcMax(z);
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
template<typename T>
void IntervalRBTree<T>::fixRemove(IntervalRBTree<T>::Pointer x)
{
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
template<typename T>
void IntervalRBTree<T>::insert(IntervalRBTree<T>::Pointer z)
{
    //qDebug() << z->value;
    Pointer y = NIL_NODE;
    Pointer x = root;
    while (x != NIL_NODE){
        y = x;
        if(z->low < x->low){
            x = x->left;
        }else{
            x= x->right;
        }
    }
    z->parent = y;
    if ( y == NIL_NODE) {
        root = z;
    } else if ( z->low < y->low){
        y->left = z;
    } else {
        y->right = z;
    }
    z->left = NIL_NODE;
    z->right = NIL_NODE;
    z->color =RED;
    fixInsert(z);
}
template<typename T>
typename IntervalRBTree<T>::Pointer IntervalRBTree<T>::remove(IntervalRBTree<T>::Pointer z)
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
template<typename T>
typename IntervalRBTree<T>::Pointer IntervalRBTree<T>::treeSuccessor(IntervalRBTree<T>::Pointer x)
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
        y = y->parent;
    }
    return y;
}
template<typename T>
typename IntervalRBTree<T>::Pointer IntervalRBTree<T>::findEqual(int low, int high) const
{
    Pointer x = root;
    while(x != NIL_NODE && (x->low != low || x->high != high)){
        if(x->left != NIL_NODE && x->low > low){
            x = x->left;
        }else{
            x= x->right;
        }
    }
    return x;
}
template<typename T>
void IntervalRBTree<T>::clearTree(IntervalRBTree<T>::Pointer r)
{
    if( r->left != NIL_NODE){
        clearTree(r->left);
    }
    if(r->right != NIL_NODE){
        clearTree(r->right);
    }
    if(r==root && root != NIL_NODE){
        delete r;
        root = NIL_NODE;
        _size = 0;
    }else if (root != NIL_NODE){
        delete r;
    }
}
template<typename T>
typename IntervalRBTree<T>::Pointer IntervalRBTree<T>::initTree(IntervalRBTree<T>::Pointer myParent, IntervalRBTree<T>::Pointer myRoot, IntervalRBTree<T>::Pointer otherRoot)
{
    myRoot = new Interval(otherRoot->low,otherRoot->high,otherRoot->value);
    myRoot->max = otherRoot->max;
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
template<typename T>
void IntervalRBTree<T>::recalcMax(IntervalRBTree<T>::Pointer p)
{
    if(p != NIL_NODE){
        p->max = maximum(p->high,p->left->max,p->right->max);
        recalcMax(p->parent);
    }
}
template<typename T>
void IntervalRBTree<T>::findAllIntersection(Pointer rt, int low, int high, QVector<Pointer> *result) const
{
//    qDebug() << "Searchin for: [" << low<<"," << high<<"]";
//    qDebug() << "Current Node:["<<rt->low<<","<<rt->high<<"]";
    if(rt == NIL_NODE) return;
//    if(low > rt->max){
//        return;
//    }
    if(rt->left != NIL_NODE){
        findAllIntersection(rt->left,low,high,result);
    }
    if(intersects(rt->low,rt->high,low,high)){
        result->append(rt);
    }
    if(high < rt->low){
        return;
    }
    if(rt->right != NIL_NODE){
        findAllIntersection(rt->right,low,high,result);
    }
}
#endif // INTERVALRBTREE_H


