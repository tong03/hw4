#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void insertFix(AVLNode<Key,Value>* parent, AVLNode<Key,Value>* node);
    void removeFix(AVLNode<Key,Value>* n, int diff);
    void rotateRight(AVLNode<Key,Value>* node);
    void rotateLeft(AVLNode<Key,Value>* node);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
  // 1. walking the tree

  AVLNode<Key, Value> *curr = static_cast<AVLNode<Key, Value>*>(this->root_);
  AVLNode<Key, Value> *parent = NULL;
  while(curr != NULL){
    // if new Key is less than current Key, go left
    if(new_item.first < curr->getKey()){
      parent = curr;
      curr = curr->getLeft();
    }
    else if(new_item.first > curr->getKey()){
      parent = curr;
      curr = curr->getRight();
    }
    // if key already in tree
    else {
      curr->setValue(new_item.second);
      parent = curr;
      return;
    }
  }
  // 2. create new node
  AVLNode<Key, Value> *newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);

  // 3. insert new node

  // if parent still empty, then empty tree
  if(parent == NULL){
    this->root_ = newNode;
  }
  else{
    // insert node right if bigger than parent
    if(newNode->getKey() > parent->getKey()){
      newNode->setParent(parent);
      parent->setRight(newNode);
      // update parent balance if it was -1 or 1 before
      if(parent->getBalance() == 1 || parent->getBalance() == -1){
        parent->setBalance(0);
      }
      else{
        // otherwise change bf to 1 because inserted right
        parent->setBalance(1);
        // then call insertFix b/c old bf = 0
        insertFix(parent, newNode);
      }
    }
    // insert node left if smaller than parent
    else {
      newNode->setParent(parent);
      parent->setLeft(newNode);
      // update parent balance if itwas -1 or 1 prior
      if(parent->getBalance() == 1 || parent->getBalance() == -1){
        parent->setBalance(0);
      }
      // change bf to -1 because inserted left
      else{
        parent->setBalance(-1);
        // call insertFix b/c old bf = 0
        insertFix(parent, newNode);
      }
    }
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix (AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node)
{
  AVLNode<Key, Value>* grandp = parent->getParent();
  if(parent == NULL || grandp == NULL){
    return;
  }

  // case for p is left child
  if(grandp->getLeft() == parent){
    // update grand bf to accurate value
    grandp->updateBalance(-1);

    // Case 1: bf = 0
    if(grandp->getBalance() == 0) {return;}

    // Case 2: bf = -1, then recurse
    else if (grandp->getBalance() == -1){
      // keep going up to check for potential imbalance
      insertFix(grandp, parent);
    }
    // Case 3: bf = -2, then this subtree imbalanced
    else {
      // zig-zig case, heavy left so rotate right
      if(parent->getLeft() == node){
        rotateRight(grandp);
        // bf(p) = bf(g) after rotate
        parent->setBalance(0);
        grandp->setBalance(0);
      }
      // zig-zag case. do rotateLeft(p) then rotatteRight(g)
      else if (parent->getRight() == node){
        rotateLeft(parent);
        rotateRight(grandp);

        // Update balances
        // Case 3a:
        // b(n) = -1; then b(p) = 0, b(g) = 1, b(n) = 0
        if(node->getBalance() == -1){
          parent->setBalance(0);
          grandp->setBalance(1);
          node->setBalance(0);
        }
        // Case 3b:
        // b(n) = 0; then b(p)=0, b(g)=0, b(n)=0
        else if(node->getBalance() == 0){
          parent->setBalance(0);
          grandp->setBalance(0);
          node->setBalance(0);
        }
        // Case 3c:
        //b(n) = 1; then b(p)=-1, b(g)=0, b(n)=0
        else {
          parent->setBalance(-1);
          grandp->setBalance(0);
          node->setBalance(0);
        }
      }
    }
  }

  // case for p is right child
  // grandp->getRight() == parent;
  else{
    grandp->updateBalance(1);

    // Case 1:
    if(grandp->getBalance() == 0){
      return;
    }
    // Case 2:
    else if(grandp->getBalance() == 1){
      insertFix(grandp, parent);
    }
    // Case 3: grandp->getBalance() == 2
    else{
      // zig-zig case: heavy right so rotate left
      if(parent->getRight() == node){
        rotateLeft(grandp);
        parent->setBalance(0);
        grandp->setBalance(0);
      }
      // zig-zag: rotateRight(p), then rotateLeft(g)
      else if (parent->getLeft() == node){
        rotateRight(parent);
        rotateLeft(grandp);

        // Update balance
        // Case 3a: b(n)=1, then b(p)=0, b(g)=-1, b(n)=0
        if(node->getBalance() == 1){
          parent->setBalance(0);
          grandp->setBalance(-1);
          node->setBalance(0);
        }
        // Case 3b: b(n)=0, then b(p)=0, b(g)=0, b(n)=0
        else if(node->getBalance() == 0){
          parent->setBalance(0);
          grandp->setBalance(0);
          node->setBalance(0);
        }
        // Case 3c: b(n)=-1, then b(p)=1, b(g)=0, b(n)=0
        else {
          parent->setBalance(1);
          grandp->setBalance(0);
          node->setBalance(0);
        }
      }
    }
  }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
  AVLNode<Key,Value> *target = static_cast<AVLNode<Key,Value>*>(BinarySearchTree<Key, Value>::internalFind(key));
  if(target == NULL){
    return;
  }

  // if target is root_, remove and set root to NULL
  if(target == this->root_ && target->getLeft() == NULL && target->getRight() == NULL){
    delete target;
    this->root_ = NULL;
    return;
  }

  // check for 2 children first to reduce to 1 or 0 children case
  if(target->getLeft() != NULL && target->getRight() != NULL){
    AVLNode<Key, Value> *pred = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key,Value>::predecessor(target));
    nodeSwap(pred, target);
  }

  // find the child
  AVLNode<Key, Value>* child = NULL;
  if(target->getLeft() != NULL || target->getRight() != NULL){
    if(target->getLeft() != NULL){
      child = target->getLeft();
    }
    else {
      child = target->getRight();
    }
  }
  // find parent
  int diff = 0;
  AVLNode<Key, Value>* parent = target->getParent();
  

  // set child's parent to target's parent since it'll be deleted
  if(child != NULL){
    child->setParent(parent);
  }

  // if parent is null, child will be root after deletion
  if(parent == NULL){
    this->root_ = child;
  }

  else if(parent != NULL){
    // check if target is left child
    if(parent->getLeft() == target){
      diff = 1;
      parent->setLeft(child);
    }
    else{
      // check if target is right child
      diff = -1;
      parent->setRight(child);
    }
  }
  delete target;
  removeFix(parent, diff);
}

template<class Key, class Value>
void AVLTree<Key, Value>:: removeFix(AVLNode<Key, Value>* n, int diff)
{
  if(n == NULL){
    return;
  }
  // initialize nextdiff
  int ndiff = 0;
  // find parent
  AVLNode<Key, Value>* p = n->getParent();
  if(p != NULL){
    if(p->getLeft() == n){
      ndiff = 1;
    }
    else {
      ndiff = -1;
    }
  }

  // assume diff = -1;
  if(diff == -1){
    // Case 1: diff + b(n) = -2
    if(diff + n->getBalance() == -2){
      // get taller children
      AVLNode<Key, Value>* c = n->getLeft();
      // Case 1a: b(c)=-1 -> zig-zig
      if(c->getBalance() == -1){
        rotateRight(n);
        n->setBalance(0);
        c->setBalance(0);
        removeFix(p, ndiff);
      }
      // Case 1b: b(c)=0 -> zig-zig
      else if(c->getBalance() == 0){
        rotateRight(n);
        n->setBalance(-1);
        c->setBalance(1);
      }
      // Case 1c: b(c)=1 -> zig-zag
      else {
        AVLNode<Key, Value>* g = c->getRight();
        rotateLeft(c);
        rotateRight(n);
        if(g->getBalance() == 1){
          n->setBalance(0);
          c->setBalance(-1);
          g->setBalance(0);
        }
        else if(g->getBalance() == 0){
          n->setBalance(0);
          c->setBalance(0);
          g->setBalance(0);
        }
        // g->getBalance() == -1
        else {
          n->setBalance(1);
          c->setBalance(0);
          g->setBalance(0);         
        }
        removeFix(p, ndiff);
      }
    }
    // Case 2: diff + b(n) = -1
    else if(diff + n->getBalance() == -1){
      n->setBalance(-1);
      return;
    }
    // Case 3: diff + b(n) = 0
    else {
      n->setBalance(0);
      removeFix(p, ndiff);
    }
  }

  // diff = 1
  else if (diff == 1){
    // Case 1: diff + b(n) == 2
    if(diff + n->getBalance() == 2){
      AVLNode<Key, Value>* c = n->getRight();
      // Case 1a: zig-zig
      if(c->getBalance() == 1){
        rotateLeft(n);
        n->setBalance(0);
        c->setBalance(0);
        removeFix(p, ndiff);
      }
      // Case 1b: zig-zig
      else if (c->getBalance() == 0){
        rotateLeft(n);
        n->setBalance(1);
        c->setBalance(-1);
      }
      // Case 1c: zig-zag
      else if (c->getBalance() == -1){
        AVLNode<Key, Value>* g = c->getLeft();
        rotateRight(c);
        rotateLeft(n);
        if(g->getBalance() == -1){
          n->setBalance(0);
          c->setBalance(1);
          g->setBalance(0);
        }
        else if (g->getBalance() == 0){
          n->setBalance(0);
          c->setBalance(0);
          g->setBalance(0);
        }
        else if(g->getBalance() == 1){
          n->setBalance(-1);
          c->setBalance(0);
          g->setBalance(0);
        }
        removeFix(p, ndiff);
      }
    }

    // Case 2: diff + b(n) == 1
    else if (diff + n->getBalance() == 1){
      n->setBalance(1);
    }
    // Case 3: diff + b(n) == 0
    else{
      n->setBalance(0);
      removeFix(p,ndiff);
    }
  }
}


template<class Key, class Value>
void AVLTree<Key, Value>:: rotateRight(AVLNode<Key, Value>* node)
{
  // done if nothing on left side to rotate
  if(node->getLeft() == NULL){
    return;
  }

  // node's parent
  AVLNode<Key, Value> *parent = node->getParent();
  // left = y, while z = node
  AVLNode<Key, Value> *y = node->getLeft();

  // asign c to z
  node->setLeft(y->getRight());
  if(node->getLeft() != NULL){
    node->getLeft()->setParent(node);
  }
  // if z was root, change to y
  if(parent == NULL){
    this->root_ = y;
  }
  // if z had parents, update pointers
  // if right side
  else if(parent->getRight() == node){
    parent->setRight(y);
  }
  // if left side
  else if (parent->getLeft() == node){
    parent->setLeft(y);
  }

  y->setParent(parent);
  y->setRight(node);
  node->setParent(y);
}

template<class Key, class Value>
void AVLTree<Key, Value>:: rotateLeft(AVLNode<Key, Value>* node)
{
  // done if no node to rotate
  if(node->getRight() == NULL){
    return;
  }
  // node's parent
  AVLNode<Key, Value> *parent = node->getParent();
  // left = y, while x = node
  AVLNode<Key, Value> *y = node->getRight();

  // assign b to x
  node->setRight(y->getLeft());
  if(node->getRight() != NULL){
    node->getRight()->setParent(node);
  }

  // if x was root, set to y instead
  if(parent == NULL){
    this->root_ = y;
  }

  // if x had parents, update pointers
  else if(parent->getLeft() == node){
    parent->setLeft(y);
  }
  else if(parent->getRight() == node){
    parent->setRight(y);
  }
  
  y->setParent(parent);
  y->setLeft(node);
  node->setParent(y);
}



template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
