#ifndef BST_ST_H
#define BST_ST_H

#include <iostream>
#include "symbol_table.h"
#include <stack>  //for iterative root insert

template <class Item, class Key>
class BstSt : public ST<Item, Key> {
  struct Node {
    Item item;
    Node* left, *right;
    int size;
    Node (Item x):item(x), left(0), right(0), size(1){}
  };
  typedef Node* Link;
  Link head;
  bool recursive;
  Item null_item;
  int n;
  
  void insertI (Item item);
  void insertR (Link& v, Item item);
  void insertRootR (Link& v, Item item);
  void insertRootI (Item item);
  Item searchI (Key k) ;
  Item searchR (Link v, Key k);
  void visitR (Link v);
  void rotLeft (Link& v);
  void rotRight (Link& v);
  void swapSize (Link a, Link b);
  int  size (Link v);
  Item selectI (int k);
  Item selectR (Link v, int k);
  void partitionR (Link& v, int k);

  //for debug
  void dumpIntNode(std::ostream& os, Item v, int h) const;
  void dumpExtNode(std::ostream& os, int h) const; 
  void dump(std::ostream& os, Link t, int h) const;
public:
  BstSt (bool rec = true):head(0), recursive(rec){}
  int count (void) {return n;}
  void insert (Item item);
  void insertRoot (Item item);
  Item search (Key k);
  void remove (Item item);
  Item select (int k);
  void visit (void);
  void operator+=(BstSt<Item, Key>& rhs);

  friend std::ostream& operator<< (std::ostream& os, const BstSt& rhs) {
    rhs.dump(os, rhs.head, 0);
    return os;
  }
};

void BstSt<Item, Key>::operator+=(BstSt<Item, Key>& rhs) {

}

//print internal node
template <class Item, class Key>
void BstSt<Item, Key>::dumpIntNode(std::ostream& os, Item v, int h) const {
  for (int i = 0; i < h; ++i) os << "  ";
  os << v.key() << std::endl;
}

//print external node
template <class Item, class Key>
void BstSt<Item, Key>::dumpExtNode(std::ostream& os, int h) const {
  for (int i = 0; i < h; ++i) os << "  ";
  os << '*' << std::endl;
}

//print (sub)tree
template <class Item, class Key>
void BstSt<Item, Key>::dump(std::ostream& os, Link t, int h) const {
  if (t == 0) { dumpExtNode(os, h); return; }

  dump(os, t->right, h + 3);
  dumpIntNode(os, t->item, h);
  dump(os, t->left, h + 3);
}

template <class Item, class Key>
Item BstSt<Item, Key>::search (Key k) {
  if (recursive) return searchR(head, k);
  else  return searchI(k);
}

template <class Item, class Key>
Item BstSt<Item, Key>::searchR (Link v, Key k) {
  if (v == 0) return null_item;
  Key v_key = v->item.key();
  if (v_key == k) return v->item;
  if (v_key >  k) return searchR(v->left, k);
  else return searchR(v->right, k); 
}

template <class Item, class Key>
Item BstSt<Item, Key>::searchI (Key k) {
  Link tmp = head;
  while (tmp) {
     Key v_key = tmp->item.key();
     if (v_key == k) return tmp->item;
     if (v_key >  k) tmp = tmp->left;
     else tmp = tmp->right; 
  }
  return null_item;
}

template <class Item, class Key>
void BstSt<Item, Key>::insert (Item item) {
  if (recursive) return insertR(head, item);
  else return insertI(item);
}

template <class Item, class Key>
void BstSt<Item, Key>::insertR (Link& v, Item item) {
  if (v == 0) {
    v = new Node(item);
    return;
  }
  if (v->item.key() >  item.key()) insertR(v->left, item);
  else insertR(v->right, item);
  v->size++;
}

template <class Item, class Key>
void BstSt<Item, Key>::insertI (Item item) {

  if (!head) head =  new Node(item);
  
  Link tmp = head;
  Key k = item.key();
  
  while (tmp) {
     if (tmp->item.key() >  k) {
       if (!tmp->left) {
	 tmp->left = new Node(item);
	 break;
       }
       tmp = tmp->left;
     }
     else {
        if (!tmp->right) {
	 tmp->right = new Node(item);
	 break;
       }
       tmp = tmp->right;
     }
     tmp->size++;
  }
}

template <class Item, class Key>
void BstSt<Item, Key>::remove (Item item) {
  removeR(Link& v, Item, item);
}

template <class Item, class Key>
void BstSt<Item, Key>::removeR (Link& v, Item item) {
  if (v == 0) return;//item is not in the ST
  if (v->item.key() == item.key()) {
    Link tmp = v;
    joinLRChild(v);
    delete tmp;
    return;//no need of this return putting the if at the end
  }
  if (v->item.key() < item.key()) removeR(v->left, item);
  else removeR(v->right, item); 
}

template <class Item, class Key>
void BstSt<Item, Key>::joinLRChild (Link& v) {
  if (!v->right) {
    v = v->left;
    return;
  }
  partitionR(v->right, 1);
  v->right->left = v->left;
  v = v->right;
}

template <class Item, class Key>
void BstSt<Item, Key>::partitionR (Link& v, int k) {
  if (!v || k == 0) return;
  if (v->size == k) return;//no need of this return putting the if at the end
  int sz = size(v->left);
  if (sz > k) {
    partitionR(v->left, k);
    rotRight(v);
  }
  else {
    partitionR(v->right, k-sz-1);
    rotLeft(v);
  }
}

template <class Item, class Key>
Item BstSt<Item, Key>::select (int k) {
  if (recursive) Return selectR(head, k);
  else return selectI(k);
}

template <class Item, class Key>
Item BstSt<Item, Key>::selectR (Link v, int k) {
  if (v->size == k) return v->item;
  int sz = size(v->left);
  if (sz > k) return selectR(v->left, k);
  else return selectR(v->right, k-sz-1);
}

template <class Item, class Key>
Item BstSt<Item, Key>::selectI (int k) {
  Link tmp = head;
  while (size(tmp) != k) {
    int sz = size(tmp->left);
    if (sz > k) {
      tmp = tmp->left;
    }
    else {
      tmp = tmp->right;
      k = k - sz -1;
    }
  }
  return tmp->item;
}

template <class Item, class Key>
void BstSt<Item, Key>::visit (void) {
  //first sort the list and then print nodes
  //std::cout << *this;
  visitR(head);
}

template <class Item, class Key>
void BstSt<Item, Key>::visitR (Link v) {
  if(!v) return;
  visitR(v->left);
  std::cout << v->item;
  visitR(v->right);
}

//left rotation: swap v with its right child r
//v become the left child of r
//the left child of r become the right child of v
template <class Item, class Key>
void BstSt<Item, Key>::rotLeft  (Link& v) {
  Link tmp = v->right;//save r
  tmp->left = v;//v become left child of r
  v->right = tmp->left;//left child of r become the right child of v
  v = tmp;//swap
  swapSize(v, tmp);
}

//right rotation: swap v with its left child l
//v become the right child of l
//the right child of l become the left child of v
template <class Item, class Key>
void BstSt<Item, Key>::rotRight  (Link& v) {
  Link tmp = v->left;
  tmp->right = v;
  v->left = tmp->right;
  v = tmp;
  swapSize(v, tmp);
}

template <class Item, class Key>
void BstSt<Item, Key>::swapSize (Link a, Link b) {
  int size = a->item.size;
  a->item.size = b->item.size;
  b->item.size = size;
}

template <class Item, class Key>
int BstSt<Item, Key>::size (Link v) {
  if (!v) return 0;
  else return v->size;
}

template <class Item, class Key>
void BstSt<Item, Key>::insertRoot (Item item) {
  if (recursive) insertRootR(head, item);
  else insertRootI(item);
}

template <class Item, class Key>
void BstSt<Item, Key>::insertRootR (Link& v, Item item) {
  if (v == 0) {
    v = new Node(item);
    return;
  }
  if (v->item.key() >  item.key()) {
    insertRoot(v->left, item);
    rotRight(v);
  }
  else {
    insertR(v->right, item);
    rotLeft(v);
  }
}

template <class Item, class Key>
void BstSt<Item, Key>::insertRootI (Item item) {
  //first node
  if (!head) head =  new Node(item);
  
  std::stack<Link> s;
  Link tmp = head;
  Key k = item.key();
  
  while (tmp) {
     if (tmp->item.key() >  k) {
       if (!tmp->left) {
	 tmp->left = new Node(item);
	 break;
       }
       s.push(tmp);
       tmp = tmp->left;
     }
     else {
        if (!tmp->right) {
	 tmp->right = new Node(item);
	 break;
       }
       s.push(tmp);
       tmp = tmp->right;
     } 
  }

  while (s.empty()) {
    tmp = s.top();
    s.pop();
    if (tmp->item.key() < k) rotateLeft(tmp);
    else rotateRight(tmp);
  }
}

#endif
