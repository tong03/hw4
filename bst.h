#ifndef BST_H
#define BST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <utility>
#include <algorithm>

/**
 * A templated class for a Node in a search tree.
 * The getters for parent/left/right are virtual so
 * that they can be overridden for future kinds of
 * search trees, such as Red Black trees, Splay trees,
 * and AVL trees.
 */
template <typename Key, typename Value>
class Node
{
public:
    Node(const Key& key, const Value& value, Node<Key, Value>* parent);
    virtual ~Node();

    const std::pair<const Key, Value>& getItem() const;
    std::pair<const Key, Value>& getItem();
    const Key& getKey() const;
    const Value& getValue() const;
    Value& getValue();

    virtual Node<Key, Value>* getParent() const;
    virtual Node<Key, Value>* getLeft() const;
    virtual Node<Key, Value>* getRight() const;

    void setParent(Node<Key, Value>* parent);
    void setLeft(Node<Key, Value>* left);
    void setRight(Node<Key, Value>* right);
    void setValue(const Value &value);

protected:
    std::pair<const Key, Value> item_;
    Node<Key, Value>* parent_;
    Node<Key, Value>* left_;
    Node<Key, Value>* right_;
};

/*
  -----------------------------------------
  Begin implementations for the Node class.
  -----------------------------------------
*/

/**
* Explicit constructor for a node.
*/
template<typename Key, typename Value>
Node<Key, Value>::Node(const Key& key, const Value& value, Node<Key, Value>* parent) :
    item_(key, value),
    parent_(parent),
    left_(NULL),
    right_(NULL)
{

}

/**
* Destructor, which does not need to do anything since the pointers inside of a node
* are only used as references to existing nodes. The nodes pointed to by parent/left/right
* are freed by the BinarySearchTree.
*/
template<typename Key, typename Value>
Node<Key, Value>::~Node()
{

}

/**
* A const getter for the item.
*/
template<typename Key, typename Value>
const std::pair<const Key, Value>& Node<Key, Value>::getItem() const
{
    return item_;
}

/**
* A non-const getter for the item.
*/
template<typename Key, typename Value>
std::pair<const Key, Value>& Node<Key, Value>::getItem()
{
    return item_;
}

/**
* A const getter for the key.
*/
template<typename Key, typename Value>
const Key& Node<Key, Value>::getKey() const
{
    return item_.first;
}

/**
* A const getter for the value.
*/
template<typename Key, typename Value>
const Value& Node<Key, Value>::getValue() const
{
    return item_.second;
}

/**
* A non-const getter for the value.
*/
template<typename Key, typename Value>
Value& Node<Key, Value>::getValue()
{
    return item_.second;
}

/**
* An implementation of the virtual function for retreiving the parent.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getParent() const
{
    return parent_;
}

/**
* An implementation of the virtual function for retreiving the left child.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getLeft() const
{
    return left_;
}

/**
* An implementation of the virtual function for retreiving the right child.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getRight() const
{
    return right_;
}

/**
* A setter for setting the parent of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setParent(Node<Key, Value>* parent)
{
    parent_ = parent;
}

/**
* A setter for setting the left child of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setLeft(Node<Key, Value>* left)
{
    left_ = left;
}

/**
* A setter for setting the right child of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setRight(Node<Key, Value>* right)
{
    right_ = right;
}

/**
* A setter for the value of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setValue(const Value& value)
{
    item_.second = value;
}

/*
  ---------------------------------------
  End implementations for the Node class.
  ---------------------------------------
*/

/**
* A templated unbalanced binary search tree.
*/
template <typename Key, typename Value>
class BinarySearchTree
{
public:
    BinarySearchTree(); //TODO
    virtual ~BinarySearchTree(); //TODO
    virtual void insert(const std::pair<const Key, Value>& keyValuePair); //TODO
    virtual void remove(const Key& key); //TODO
    void clear(); //TODO
    bool isBalanced() const; //TODO
    void print() const;
    bool empty() const;

    template<typename PPKey, typename PPValue>
    friend void prettyPrintBST(BinarySearchTree<PPKey, PPValue> & tree);
public:
    /**
    * An internal iterator class for traversing the contents of the BST.
    */
    class iterator  // TODO
    {
    public:
        iterator();

        std::pair<const Key,Value>& operator*() const;
        std::pair<const Key,Value>* operator->() const;

        bool operator==(const iterator& rhs) const;
        bool operator!=(const iterator& rhs) const;

        iterator& operator++();

    protected:
        friend class BinarySearchTree<Key, Value>;
        iterator(Node<Key,Value>* ptr);
        Node<Key, Value> *current_;
    };

public:
    iterator begin() const;
    iterator end() const;
    iterator find(const Key& key) const;
    Value& operator[](const Key& key);
    Value const & operator[](const Key& key) const;

protected:
    // Mandatory helper functions
    Node<Key, Value>* internalFind(const Key& k) const; // TODO
    Node<Key, Value> *getSmallestNode() const;  // TODO
    static Node<Key, Value>* predecessor(Node<Key, Value>* current); // TODO
    // Note:  static means these functions don't have a "this" pointer
    //        and instead just use the input argument.

    // Provided helper functions
    virtual void printRoot (Node<Key, Value> *r) const;
    virtual void nodeSwap( Node<Key,Value>* n1, Node<Key,Value>* n2) ;

    // Add helper functions here
    void clearTraversal(Node<Key, Value>* root);
    int getBalance(Node<Key, Value> *node) const;
    static Node<Key, Value>* successor(Node<Key, Value>* current);

protected:
    Node<Key, Value>* root_;
    // You should not need other data members
};

/*
--------------------------------------------------------------
Begin implementations for the BinarySearchTree::iterator class.
---------------------------------------------------------------
*/

/**
* Explicit constructor that initializes an iterator with a given node pointer.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator(Node<Key,Value> *ptr)
{
    this->current_ = ptr;
}

/**
* A default constructor that initializes the iterator to NULL.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator() 
{
    this->current_ = NULL;
}

/**
* Provides access to the item.
*/
template<class Key, class Value>
std::pair<const Key,Value> &
BinarySearchTree<Key, Value>::iterator::operator*() const
{
    return current_->getItem();
}

/**
* Provides access to the address of the item.
*/
template<class Key, class Value>
std::pair<const Key,Value> *
BinarySearchTree<Key, Value>::iterator::operator->() const
{
    return &(current_->getItem());
}

/**
* Checks if 'this' iterator's internals have the same value
* as 'rhs'
*/
template<class Key, class Value>
bool
BinarySearchTree<Key, Value>::iterator::operator==(
    const BinarySearchTree<Key, Value>::iterator& rhs) const
{
    return this->current_ == rhs.current_;
}

/**
* Checks if 'this' iterator's internals have a different value
* as 'rhs'
*/
template<class Key, class Value>
bool
BinarySearchTree<Key, Value>::iterator::operator!=(
    const BinarySearchTree<Key, Value>::iterator& rhs) const
{
    // TODO
    return this->current_ != rhs.current_;
}


/**
* Advances the iterator's location using an in-order sequencing
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator&
BinarySearchTree<Key, Value>::iterator::operator++()
{
    // check if current node is NULL first
    if(this->current_ == NULL){
        return *this;
    }

    // now check if curr has a right child
    if(this->current_->getRight() != NULL){
        // go right once and then keeps going left
        this->current_ = this->current_->getRight();
        while(this->current_->getLeft() != NULL){
            this->current_ = this->current_->getLeft();
        }
    }
    // otherwise must traverse back up to where curr
    // subtree is a left child to traverse right
    else {
        Node<Key, Value> *prev = this->current_;
        this->current_ = this->current_->getParent();
        // loop only work if prev still not a left child
        // of an ancestor
        while(this->current_ != NULL && this->current_->getRight() == prev){
            prev = this->current_;
            this->current_ = this->current_->getParent();
        }
    }

    return *this;
}


/*
-------------------------------------------------------------
End implementations for the BinarySearchTree::iterator class.
-------------------------------------------------------------
*/

/*
-----------------------------------------------------
Begin implementations for the BinarySearchTree class.
-----------------------------------------------------
*/

/**
* Default constructor for a BinarySearchTree, which sets the root to NULL.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::BinarySearchTree() :
    root_(NULL)
{

}

template<typename Key, typename Value>
BinarySearchTree<Key, Value>::~BinarySearchTree()
{
    // TODO
    clear();
}

/**
 * Returns true if tree is empty
*/
template<class Key, class Value>
bool BinarySearchTree<Key, Value>::empty() const
{
    return root_ == NULL;
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::print() const
{
    printRoot(root_);
    std::cout << "\n";
}

/**
* Returns an iterator to the "smallest" item in the tree
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::begin() const
{
    BinarySearchTree<Key, Value>::iterator begin(getSmallestNode());
    return begin;
}

/**
* Returns an iterator whose value means INVALID
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::end() const
{
    BinarySearchTree<Key, Value>::iterator end(NULL);
    return end;
}

/**
* Returns an iterator to the item with the given key, k
* or the end iterator if k does not exist in the tree
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::find(const Key & k) const
{
    Node<Key, Value> *curr = internalFind(k);
    BinarySearchTree<Key, Value>::iterator it(curr);
    return it;
}

/**
 * @precondition The key exists in the map
 * Returns the value associated with the key
 */
template<class Key, class Value>
Value& BinarySearchTree<Key, Value>::operator[](const Key& key)
{
    Node<Key, Value> *curr = internalFind(key);
    if(curr == NULL) throw std::out_of_range("Invalid key");
    return curr->getValue();
}
template<class Key, class Value>
Value const & BinarySearchTree<Key, Value>::operator[](const Key& key) const
{
    Node<Key, Value> *curr = internalFind(key);
    if(curr == NULL) throw std::out_of_range("Invalid key");
    return curr->getValue();
}

/**
* An insert method to insert into a Binary Search Tree.
* The tree will not remain balanced when inserting.
* Recall: If key is already in the tree, you should 
* overwrite the current value with the updated value.
*/
template<class Key, class Value>
void BinarySearchTree<Key, Value>::insert(const std::pair<const Key, Value> &keyValuePair)
{
    // define a curr to traverse and a parent tracker
    Node<Key, Value> *curr = root_;
    Node<Key, Value> *parent = NULL;

    // keeps traveling until curr reach null
    while(curr != NULL){
        if(keyValuePair.first < curr->getKey()){
            parent = curr;
            curr = curr->getLeft();
        }
        else if(keyValuePair.first > curr->getKey()){
            parent = curr;
            curr = curr->getRight();
        }
        // if key is already in tree
        else {
            curr->setValue(keyValuePair.second);
            parent = curr;
            return;
        }
    }

    // once traverse done, initialize new node
    Node<Key, Value> *nodeNew = new Node<Key, Value>(keyValuePair.first, keyValuePair.second, NULL);

    // check if parent still is null, then empty BST;
    if(parent == NULL){
      root_ = nodeNew;
    }
    else {
      // check if newNode is bigger than parent
      if(nodeNew->getKey() > parent->getKey()){
          nodeNew->setParent(parent);
          // add to the right of parent;
          parent->setRight(nodeNew);
      }
      // otherwise newNode is smaller than parent
      else{
          nodeNew->setParent(parent);
          parent->setLeft(nodeNew);
      }
    }
}


/**
* A remove method to remove a specific key from a Binary Search Tree.
* Recall: The writeup specifies that if a node has 2 children you
* should swap with the predecessor and then remove.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::remove(const Key& key)
{
  // find the key
  Node<Key, Value> *target = internalFind(key);
  if(target == NULL){
    // if key not found, do nothing
    return;
  }

  // if target is a single root_, remove and set root to NULL
  if(target == root_ && target->getLeft() == NULL && target->getRight() == NULL){
    delete target;
    root_ = NULL;
    return;
  }

  // check case for 2 children first because we reduces it 
  // to 1 or 0 children case
  if(target->getLeft() != NULL && target->getRight() != NULL){
    Node<Key, Value> *pre = predecessor(target);
    nodeSwap(target, pre);
  }

  Node<Key, Value> *child = NULL;
  // if 1 children
  // check right side child first
  if(target->getRight() != NULL){
    // set child to right side
    child = target->getRight();
    // change child's parent to target's parent
    child->setParent(target->getParent());
    // check if target parent isn't null
    if(target->getParent() != NULL){
      // change target's parent to point to child instead of target
      if(target->getParent()->getLeft() == target){
        // if target is left-child, set left to child
        target->getParent()->setLeft(child);
      }
      // otherwise target is right-child
      else{
        target->getParent()->setRight(child);
      }
    }
    // otherwise target was the root with 1 child
    else{
      // child now becomes the root
      root_ = child;
    }
    delete target;
    return;
  }
  // check left side next
  else if(target->getLeft() != NULL){
    child = target->getLeft();
    child->setParent(target->getParent());
    if(target->getParent() != NULL){
      if(target->getParent()->getLeft() == target){
        target->getParent()->setLeft(child);
      }
      else{
        target->getParent()->setRight(child);
      }
    }
    else{
      root_ = child;
    }
    delete target;
    return;
  }
  // if child is still null, target has 0 child
  if(child == NULL){
    // change who target's parent is pointing to
    // check if target was left child
    if(target->getParent()->getLeft() == target){
      target->getParent()->setLeft(NULL);
      delete target;
      return;
    }
    // otherwise target is right child
    else{
      target->getParent()->setRight(NULL);
      delete target;
      return;
    }
  }
}


template<class Key, class Value>
Node<Key, Value>*
BinarySearchTree<Key, Value>::successor(Node<Key, Value>* current)
{
  if(current == NULL){
    return NULL;
  }
  // initialize succ
  Node<Key, Value> *succ = current;
  // if 1 right child
  if(succ->getRight() != NULL){
    // go right once, before going left all the way
    succ = succ->getRight();
    while(succ->getLeft() != NULL){
      succ = succ->getLeft();
    }
  }
  else{
    // otherwise no right child
    Node<Key, Value> *parent = succ->getParent();
    // traverse up while parent isn't a left child of its parent
    while(parent != NULL && parent->getParent()->getLeft() != parent){
      parent = parent->getParent();
    }
    // otherwise next parent is successor
    parent = parent->getParent();
    succ = parent;
  }
  return succ;
}


template<class Key, class Value>
Node<Key, Value>*
BinarySearchTree<Key, Value>::predecessor(Node<Key, Value>* current)
{
    // if current's null, just return null
    if(current == NULL){
      return NULL;
    }
    // initialize pred
    Node<Key, Value> *pred = current;

    // if current has 1 left child
    if(pred->getLeft() != NULL){
      // go left once, then go all the way right
      pred = pred->getLeft();
      while(pred->getRight() != NULL){
        pred = pred->getRight();
      }
    }

    // other wise if there's no left child
    else{
      // find first parent of pred
      Node<Key, Value> *parent = pred->getParent();
      // traverse up while the pred parent isn't a right child of its parent
      while(parent != NULL && parent->getParent()->getRight() != parent){
        parent = parent->getParent();
      }
      // otherwise, the next parent is the predecessor
      parent = parent->getParent();
      // now assign current to the parent found
      pred = parent;
    }
    return pred; 
}


/**
* A method to remove all contents of the tree and
* reset the values in the tree for use again.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::clear()
{
  clearTraversal(root_);
  // resets root_ at the end;
  root_ = NULL;
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::clearTraversal(Node<Key, Value>* root)
{
  if(root != NULL){
    // assume delete left side
    clearTraversal(root->getLeft());
    // assume delete right side
    clearTraversal(root->getRight());
    delete root;
  }
}


/**
* A helper function to find the smallest node in the tree.
*/
template<typename Key, typename Value>
Node<Key, Value>*
BinarySearchTree<Key, Value>::getSmallestNode() const
{
  Node<Key, Value> *target = root_;

  // find the left most node as that will be the smallest
  while(target != NULL && target->getLeft() != NULL){
    target = target->getLeft();
  }

  return target;
}

/**
* Helper function to find a node with given key, k and
* return a pointer to it or NULL if no item with that key
* exists
*/
template<typename Key, typename Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::internalFind(const Key& key) const
{
  // initialize curr at root
  Node<Key, Value> *curr = root_;

  // traverse while curr isn't null
  while(curr != NULL){
    // if key is less than curr's key, go left
    if(key < curr->getKey()){
      curr = curr->getLeft();
    }
    // if key is greater than curr's key, go right
    else if(key > curr->getKey()){
      curr = curr->getRight();
    }
    // otherwise if they match, return this node
    else {
      return curr;
    }
  }

  return curr;
}

/**
 * Return true iff the BST is balanced.
 */
template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::isBalanced() const
{
  return getBalance(root_) != -1;
}

template<typename Key, typename Value>
int BinarySearchTree<Key, Value>::getBalance(Node<Key, Value> *node) const
{
  if(node == NULL){
    return 0;
  }
  // get height of left, if not balanced, will end with -1
  int leftHeight = getBalance(node->getLeft());
  if(leftHeight == -1) {return -1;}
  // get Height of right, if not balanced, will end with -1
  int rightHeight = getBalance(node->getRight());
  if(rightHeight == -1) {return -1;}

  // in the case both subtrees are balanced, find diff
  // of height for whole tree
  int diff = abs(leftHeight - rightHeight);
  if (diff > 1){
    return -1;
  }
  // otherwise just keeps returning the height of tree
  // for each recursion
  return std::max(leftHeight, rightHeight) + 1;
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::nodeSwap( Node<Key,Value>* n1, Node<Key,Value>* n2)
{
    if((n1 == n2) || (n1 == NULL) || (n2 == NULL) ) {
        return;
    }
    Node<Key, Value>* n1p = n1->getParent();
    Node<Key, Value>* n1r = n1->getRight();
    Node<Key, Value>* n1lt = n1->getLeft();
    bool n1isLeft = false;
    if(n1p != NULL && (n1 == n1p->getLeft())) n1isLeft = true;
    Node<Key, Value>* n2p = n2->getParent();
    Node<Key, Value>* n2r = n2->getRight();
    Node<Key, Value>* n2lt = n2->getLeft();
    bool n2isLeft = false;
    if(n2p != NULL && (n2 == n2p->getLeft())) n2isLeft = true;


    Node<Key, Value>* temp;
    temp = n1->getParent();
    n1->setParent(n2->getParent());
    n2->setParent(temp);

    temp = n1->getLeft();
    n1->setLeft(n2->getLeft());
    n2->setLeft(temp);

    temp = n1->getRight();
    n1->setRight(n2->getRight());
    n2->setRight(temp);

    if( (n1r != NULL && n1r == n2) ) {
        n2->setRight(n1);
        n1->setParent(n2);
    }
    else if( n2r != NULL && n2r == n1) {
        n1->setRight(n2);
        n2->setParent(n1);

    }
    else if( n1lt != NULL && n1lt == n2) {
        n2->setLeft(n1);
        n1->setParent(n2);

    }
    else if( n2lt != NULL && n2lt == n1) {
        n1->setLeft(n2);
        n2->setParent(n1);

    }


    if(n1p != NULL && n1p != n2) {
        if(n1isLeft) n1p->setLeft(n2);
        else n1p->setRight(n2);
    }
    if(n1r != NULL && n1r != n2) {
        n1r->setParent(n2);
    }
    if(n1lt != NULL && n1lt != n2) {
        n1lt->setParent(n2);
    }

    if(n2p != NULL && n2p != n1) {
        if(n2isLeft) n2p->setLeft(n1);
        else n2p->setRight(n1);
    }
    if(n2r != NULL && n2r != n1) {
        n2r->setParent(n1);
    }
    if(n2lt != NULL && n2lt != n1) {
        n2lt->setParent(n1);
    }


    if(this->root_ == n1) {
        this->root_ = n2;
    }
    else if(this->root_ == n2) {
        this->root_ = n1;
    }

}

/**
 * Lastly, we are providing you with a print function,
   BinarySearchTree::printRoot().
   Just call it with a node to start printing at, e.g:
   this->printRoot(this->root_) // or any other node pointer

   It will print up to 5 levels of the tree rooted at the passed node,
   in ASCII graphics format.
   We hope it will make debugging easier!
  */

// include print function (in its own file because it's fairly long)
#include "print_bst.h"

/*
---------------------------------------------------
End implementations for the BinarySearchTree class.
---------------------------------------------------
*/

#endif
