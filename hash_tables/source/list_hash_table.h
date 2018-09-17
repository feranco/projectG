#include <algorithm>
#include <functional>
#include <list>
#include <utility>
#include <vector>
#include <iostream>
using std::find;
using std::list;

using std::pair;
using std::vector;

template <typename Key, typename T, typename Hash = std::hash<Key>>
class HashTable {
 
  using valueType =  pair<const Key, T>;
  using HashTableType = HashTable<Key, T, Hash>;
  vector<list<valueType>> hashTable;
  size_t bucketSize = 0;
  size_t size = 0;
  Hash hashFunct;

 public:
  //typedef typename std::list<valueType> array_type;
  //typedef typename x std::list<valueType>::iterator iterator;
  //using hashFunction = std::function<unsigned int(Key, unsigned int)>;
  //hashFunction hash;

  //Custom iterator for the HashTable
  class iterator {
  public:
    //Define iterator traits
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = size_t;
    using value_type        = valueType;
    using pointer           = value_type*;
    using reference         = value_type&;

    template <typename TT>
    using listIterator =  typename std::list<TT>::iterator;

    
    //Constructor
    iterator() { };
    iterator (HashTableType* ht, listIterator<valueType> it) : hashTableIt(ht), lIt(it) {};
    
    iterator& operator++()
    {
      this->goToNextElement();
      return *this;
    }
    
    iterator operator++(int)
    {
      this->goto_next_element();
      return iterator(hashTableIt, lIt);
    }
    

    reference operator*()
    {
      return *lIt;
    }

    pointer operator->()
    {
      return lIt;
    }

    bool operator==(const iterator& rhs) const
    {
      return lIt == rhs.lIt;
    }

    bool operator!=(const iterator& rhs) const
    {
      return lIt != rhs.lIt;
    }

    
  private:
    void goToNextElement()
    {
      Key k = lIt->first;
      size_t h = this->hashFunct(k);
      h = h % this->bucketSize();
      lIt++;
      if (lIt != this->hashTable[h].end()) {
	return lIt;
      }

      for (size_t i = h; i < this->bucketSize(); ++i) {
	auto it = this->hashTable[h].begin();
	if (it != this->hashTable[h].end()) {
	  return it;
	}	  
      }
      return iterator(hashTableIt, hashTableIt->hashTable[hashTableIt->bucketSize-1].end());
    }
    
  public:
    HashTableType* hashTableIt;
    listIterator<valueType> lIt;
    
  };

  iterator begin()
  {
    for (auto bucket : hashTable) {
      auto it = bucket.begin();
      if (it != bucket.end()) {
	return it;
      }
    }
    return iterator(this, hashTable[bucketSize-1].end());
  }

  iterator end()
  {
    return iterator(this, hashTable[bucketSize-1].end());
  }
  

/*  
  using iterator =  typename std::list<valueType>::iterator;
  iterator begin() noexcept { return hashTable[0].begin(); }
  inline iterator end() noexcept { return hashTable[0].end(); }
*/ 
  HashTable () {}
  //HashTable (hashFunction hf) : hash(hf){}
  
  HashTable (unsigned int n);
  iterator search (const Key& k);
  iterator insert (Key k, T value);
  void remove (iterator it);

};

template <typename Key, typename T, typename Hash>
  HashTable<Key,T, Hash>::HashTable(unsigned int size) {
  hashTable.resize(size);
  bucketSize = size;
}


template <typename Key, typename T, typename Hash>
  typename HashTable<Key, T, Hash>::iterator  HashTable<Key,T, Hash>::search(const Key& k)  {
  auto h =  hashFunct(k);//Hash<T>(k);
  h = h%bucketSize;
  auto lambda = [&](valueType x){return x.first == k;}; 
  auto it =  find_if(hashTable[h].begin(), hashTable[h].end(),lambda);
  std::cout << "search: " << it->first << std::endl;
  return iterator(this, hashTable[h].begin());
}

template <typename Key, typename T, typename Hash>
  typename HashTable<Key,T, Hash>::iterator HashTable<Key,T, Hash>::insert(Key k, T value) {
  auto h = hashFunct(k);
  h = h%bucketSize;
  std::cout << h << std::endl;
  hashTable[h].push_front(std::make_pair(k,value));
  size++;
  return iterator(this, hashTable[h].begin());
}

/// Erase an element using an iterator.
/// return false if element was not found.

template <typename Key, typename T, typename Hash>
void HashTable<Key,T, Hash>::remove(typename HashTable<Key,T, Hash>::iterator it) {
   auto h = hashFunct(it->first);
   h = h%bucketSize;
   hashTable[h].erase(it);
   size--;
}

//OVFQEI
