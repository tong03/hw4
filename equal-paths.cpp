#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <iostream>
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

bool checkDepth (Node* root, int currH, int& leafH){
  // if empty tree or no path, then true;
  if (root == nullptr){
    return true;
  }
  // if both left and right is null, it's a leaf node
  if(root->left == nullptr && root->right == nullptr){
    // set height if the first leaf node encountered;
    if (leafH == -1){
      leafH = currH;
    }
    // otherwise only true if currH same as first leafH
    return leafH == currH;
  }

  // check left and right side of tree
  return checkDepth(root->left, currH + 1, leafH) &&
    checkDepth(root->right, currH + 1, leafH);
}


bool equalPaths(Node * root)
{
    // Add your code below

    // intialize leafH
    int leafH = -1;
    return checkDepth(root, 0, leafH);
}

