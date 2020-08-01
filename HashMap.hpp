//
// Created by gueta on 13/09/2019.
//

#ifndef EX3_HASHMAP_HPP
#define EX3_HASHMAP_HPP

#include <iostream>
#include <vector>
#include <cassert>
#include <memory>
#include <algorithm>


#define CAPACITY 16
#define SIZE 0
#define LOWER_BOUND 0.25
#define UPPER_BOUND 0.75
#define HASHMAP_CTOR_INVALID_MSG "HashMap Constructor Invalid Input"
#define HASHMAP_KEY_NOT_FOUND_MSG "HashMap at Invalid Input: key not found"



/// ###### exceptions #######
/**
 * the InvalidInputException is an abstract class represents an invalid
 */
class InvalidInputException: public std::exception
{
public:
    /**
    * @return the error msg to std cerr in case of this error
    */
    virtual const char* what() const noexcept override = 0;
};

/**
 * the HashMapInvalidInputException is an abstract class represents an invalid input to the HashMap
 * class
 */
class HashMapInvalidInputException : public InvalidInputException
{
    /**
    * @return the error msg to std cerr in case of this error
    */
    virtual const char* what() const noexcept override = 0;

};

/**
 * the HashMapInvalidInputConstructorException class represents an invalid input to the HashMap
 * Constructor
 */
class HashMapInvalidInputConstructorException : public HashMapInvalidInputException
{
    /**
    * @return the error msg to std cerr in case of this error
    */
    inline const char* what() const noexcept override { return HASHMAP_CTOR_INVALID_MSG; }
};

/**
 * the HashMapInvalidInputKeyException class represents an invalid input to the HashMap
 * functions that require to find a key and it does not exist
 */
class HashMapInvalidKeyException : public HashMapInvalidInputException
{
    /**
    * @return the error msg to std cerr in case of this error
    */
    inline const char* what() const noexcept override { return HASHMAP_KEY_NOT_FOUND_MSG; }
};



/// ### end of exceptions ###///





/**
 * this class reprasents hash map
 * @tparam KeyT the type key of the hash map
 * @tparam ValueT the type of the value in the hash map
 */
template <class KeyT, class ValueT>
class HashMap
{
    // it was said its ok to use typename to decribe a bucket
    using Bucket  = typename std::vector<std::pair<KeyT, ValueT>>;

private:

    /** the hash map lower load factor*/
    double _lowerLoadFactor;

     /** the hash map upper load factor*/
     double _upperLoadFactor;

     /** number of buckets in the vecOfVectors*/
     double _capacity;


    /** numbet of pairs in the hash map*/
    int _size;


    /** the load factor of the hash map */
    double _loadFactor;

    /** the hash function we use to map the elemant*/
    std :: hash<KeyT> _hash;

    /** vetor of buckets , represents the hash table*/
    std::vector<Bucket> _bucketsVec;

    /**
     *
     * @param key the key that we check if in the bucket
     * @param bucket the bucket that we check if the key is there
     * @return true if so false otherwise
     */
    bool _bucketContainKey(const KeyT &key, Bucket bucket) const;

    /**
     * insert a pait in the bucket
     * @param key the key that we add to the pair bucket
     * @param value the value the pair in the bucket
     */
    void _addToBucketVec(const KeyT &key, const ValueT& value);

    /**
     * erase a pair from the bucet by the key
     * @param key the key that we erase
     */
    void _eraseFromBucketVec(const KeyT &key);

    /**
     * find the index of the key in the bucket vec
     * @param key the key that we look for
     * @return  index of the key in the bucket vec
     */
    int _findIndex(const KeyT key) const;

    /**
     * re size the capacity of the hash map acoording to the bool given
     * @param inLarge if true we inlarge the table , if false we shrink.
     */
    void _reSize(const bool inLarge);

    /**
     * this is an help funck to the vector const, its insertingg the pairs from the vectors to the hash map
     * @param key the key that we insert
     * @param value the value we insert
     */
    void _vectorInsert(const KeyT& key, const ValueT& value);





public:

    /**
     *
     * @param lowerLoadFactor lowerLoadFactor of the table
     * @param upperLoadFactor upperLoadFactor of the table
     */
    HashMap(double lowerLoadFactor, double upperLoadFactor): _lowerLoadFactor(lowerLoadFactor),
                                                              _upperLoadFactor(upperLoadFactor),
                                                              _capacity(CAPACITY),
                                                              _size(SIZE), _loadFactor(0.0)

    {
        if (lowerLoadFactor >= upperLoadFactor)
        {
            throw HashMapInvalidInputConstructorException();
        }
        if (lowerLoadFactor > 1 || upperLoadFactor > 1 ||
            lowerLoadFactor < 0  || upperLoadFactor < 0)
        {
            throw HashMapInvalidInputConstructorException();
        }
        _bucketsVec.resize(CAPACITY);
    };

    /**
     * efault constructor sets the lower factor to 0.25 and upper factor to 0.75
     */
    HashMap(): HashMap(LOWER_BOUND, UPPER_BOUND){};

    /**
     * Receiving two vectors of keys and values, this constructor sets the map through that
     * @param keyVector vector of keys.
     * @param valuesVector vector of values.
     */
    HashMap(std::vector<KeyT> keyVector, std::vector<ValueT> valuesVector):
            HashMap(LOWER_BOUND, UPPER_BOUND)
    {
        if (keyVector.size() != valuesVector.size())
        {
            throw HashMapInvalidInputConstructorException();
        }
        for (int i = 0; i < keyVector.size(); i++)
        {
            _vectorInsert(keyVector.at(i), valuesVector.at(i));
        }
    }

    /**
     * copy constructor
     * @param other the other HashMap that been copied
     */
    HashMap(HashMap &  other) = default;

    /**
     * copy move constructor
     * @param other other the other HashMap that been copied
     */
    HashMap(HashMap &&  other) = default;

    /**
     * distructor
     */
    ~HashMap() = default;

    /**
     *
     * @return  _capacity
     */
    int capacity() const{ return _capacity; }

    /**
     *
     * @return _size/
     */
    int size() const { return _size; }

    /**
     *
     * @return _loadFactor
     */
    double getLoadFactor() const {return _loadFactor; }

    /**
     *
     * @return true if the hash Map is empty false otherwise.
     */
    bool empty() const { return  (_size == 0); }

    /**
     * insert a pair the to hash map
     * @param key the key that we insert
     * @param value the value that we insert
     * @return true if the insertion was sueccsid false oherwise
     */
    bool insert(const KeyT& key, const ValueT& value);

    /**
     * checks if hash map contains a certion key
     * @param key the key that we check if is containing
     * @return true if so false otherwise
     */
    bool containsKey(const KeyT& key) const ;

    /**
     *
     * @param key the key that we look for is value
     * @return the value of the key in the hash map
     */
    const ValueT& at(const KeyT& key) const;

    /**
    *
    * @param key the key that we look for is value
    * @return the value of the key in the hash map
    */
    ValueT& at(const KeyT& key);


    /**
     *
     * @param keyToFindBucket the key that we want his bucket
     * @return the size of the bucket fo the key we want
     */
    int bucketSize (const KeyT& keyToFindBucket) const;

    /**
     * overloading the operator !=
     * @param other the other we doing the != with
     * @return true if this != other , false otherwise
     */
    inline bool operator!=(const HashMap<KeyT, ValueT>& other) const { return !(*this == other); }

    /**
     * overloading the operator =
     * @param other the other that we asiigen this to
     * @return this
     */
    HashMap<KeyT, ValueT>& operator=(const HashMap<KeyT, ValueT>& other) = default;

    /**
     * we erase a key from the hash map
     * @param key the key that we want to erase
     * @return true if we erase, false otherwise.
     */
    bool erase(const KeyT& key);

    /**
     * clear all the hash map/
     */
    void clear();

    /**
     * default move assignment
     * @param other other to assign to
     * @return the assigned member
     */
    HashMap<KeyT, ValueT>& operator=(HashMap<KeyT, ValueT> && other) noexcept = default;
    /**
     * the function return the begin of the hash map iterator
     * @return iterator object, points to the begin
     */

    /**
     * overloading the operator []
     * @param key the key that we want is value
     * @return the value of the key in the hash map
     */
    ValueT& operator[](const KeyT& key);

    /**
     * overloading the operator []
     * @param key the key that we want is value
     * @return the value of the key in the hash map
     */
    const ValueT& operator[](const KeyT& key) const;

    /**
     * overloading the operator []
     * @param other the other hash map that we compering to
     * @return true if this == other , false other wise.
     */
    bool operator==(const HashMap<KeyT, ValueT>& other) const;

    /**
     * Class that enables iterating over the map, where it stays constant
     */
    class const_iterator
    {

    private:

        /** pointer to the hash msp*/
        const HashMap *_map;

        /** 2 indexes that indecates on the loocation in the hash map*/
        int _bucketIndex, _vectorIndex;

    public:

        /**
         * Constructor of the iterator, sets the iteration to the first one
         * @param map pointer of hashmap
         * @param bucketIndex has default value of 0
         * @param vectorIndex has default value of 0
         */
        explicit const_iterator(const HashMap * map, int bucketIndex = 0, int vectorIndex = 0)
                : _map(map), _bucketIndex(bucketIndex), _vectorIndex(vectorIndex)
        {
            if(!_map->empty())
            {

                while ( _bucketIndex != _map->_capacity && _map->_bucketsVec.at(_bucketIndex).empty() )
                {
                    _bucketIndex++;
                }
            }
        }

        /**
         * overloading the operator ++this
         * @return this
         */
        const const_iterator& operator++()
        {
            if(_bucketIndex == _map->_capacity)
            {
                return *this;
            }
            if((int)_map->_bucketsVec.at(_bucketIndex).size() - 1 == _vectorIndex)
            {
                _bucketIndex ++ ;
                _vectorIndex = 0;
                while ( _map->_capacity != _bucketIndex && _map->_bucketsVec.at(_bucketIndex).empty())
                {
                    _bucketIndex ++;
                }
                return *this;
            }
            else
            {
                _vectorIndex ++;
            }
            return *this;
        }

        /**
         * overloading the operator this++
         * @return this
         */
        const HashMap::const_iterator operator++(int)
        {
            const_iterator temp = *this;
            ++(*this);
            return temp;
        }

        /**
         * overloading the operator *
         * @return the pair that in that index
         */
        const std :: pair<KeyT, ValueT> &operator*()const;

        /**
         * overloading the operator ->
         * @return the pointer to the pair that in that index
         */
        const std :: pair<KeyT, ValueT> *operator->()const;

        /**
         * overloading the operator ==
         * @param other the other hash map we == with
         * @return true if this == other ' false otherwise
         */
        bool operator==(const_iterator const &other) const;

        /**
         * overloading the operator !=
         * @param other the other hash map we == with
         * @return true if this != other ' false otherwise
         */
        inline bool operator!=(const const_iterator other) const{ return !(*this == other); }
    };

    /**
     * First iterator of the hash map
     * @return the iterator of the beginning of the map
     */
    inline const_iterator begin() const{ return const_iterator(this); }

    /**
     * last iterator of the hash map
     * @return he iterator of the end of the map
     */
    inline const_iterator end() const{ return const_iterator(this, _capacity, 0); }

    /**
    * First iterator of the hash map
    * @return the iterator of the beginning of the map
    */
    inline const_iterator cbegin() const{ return const_iterator(this); }

    /**
    * First iterator of the hash map
    * @return the iterator of the beginning of the map
    */
    inline const_iterator cend() const{ return const_iterator(this, _capacity, 0); }



};



/**
* overloading the operator *
* @return the pair that in that index
*/
template<class KeyT, class ValueT>
const std::pair<KeyT, ValueT> &HashMap<KeyT, ValueT>::const_iterator::operator*() const
{
    return _map->_bucketsVec.at(_bucketIndex).at(_vectorIndex);
}

/**
* overloading the operator ->
* @return the pointer to the pair that in that index
*/
template<class KeyT, class ValueT>
const std::pair<KeyT, ValueT> *HashMap<KeyT, ValueT>::const_iterator::operator->() const
{
    return &(_map->_bucketsVec.at(_bucketIndex).at(_vectorIndex));
}

/**
* overloading the operator ==
@param other the other hash map we == with
* @return true if this != other ' false otherwise
*/
template<class KeyT, class ValueT>
bool HashMap<KeyT, ValueT>::const_iterator::operator==(const const_iterator &other) const
{
    return (_map == other._map && _vectorIndex == other._vectorIndex &&
            _bucketIndex == other._bucketIndex );
}

/**
* insert a pair the to hash map
* @param key the key that we insert
* @param value the value that we insert
* @return true if the insertion was sueccsid false oherwise
*/
template<class KeyT, class ValueT>
bool HashMap<KeyT, ValueT>::insert(const KeyT &key, const ValueT &value)
{
    if(containsKey(key))
    {
        return false;
    }
    _addToBucketVec(key, value);
    _size ++;
    _loadFactor = (double) _size / _capacity;
    if (_loadFactor > _upperLoadFactor)
    {
        this->_reSize(true);
    }
    return true;
}

/**
* find the index of the key in the bucket vec
* @param key the key that we look for
* @return  index of the key in the bucket vec
*/
template<class KeyT, class ValueT>
int HashMap<KeyT, ValueT>::_findIndex(const KeyT key) const
{
    return (int)_hash(key) & (int)(_capacity - 1);
}


/**
 * insert a pait in the bucket
 * @param key the key that we add to the pair bucket
 * @param value the value the pair in the bucket
 */
template<class KeyT, class ValueT>
bool HashMap<KeyT, ValueT>::containsKey(const KeyT& key) const
{
   int index = _findIndex(key);
   Bucket bucketToCheck = _bucketsVec.at(index);
   return _bucketContainKey(key, bucketToCheck);
}

/**
* checks if hash map contains a certion key
* @param key the key that we check if is containing
* @return true if so false otherwise
*/
template<class KeyT, class ValueT>
bool HashMap<KeyT, ValueT>::_bucketContainKey(const KeyT &key, Bucket bucket) const
{
    for(int i = 0 ; i < (int)bucket.size(); i ++)
    {
        if(bucket.at(i).first == key)
        {
            return true;
        }
    }
    return false;
}



/**
 * insert a pait in the bucket
 * @param key the key that we add to the pair bucket
 * @param value the value the pair in the bucket
 */
template<class Key, class ValueT>
void HashMap<Key, ValueT>::_addToBucketVec(const Key &key, const ValueT& value)
{
    int index = _findIndex(key);
    Bucket& bucketToAdd = _bucketsVec.at(index);
    std :: pair<Key, ValueT> pairToAdd(key, value);
    bucketToAdd.push_back(pairToAdd);
}


/**
 * re size the capacity of the hash map acoording to the bool given
 * @param inLarge if true we inlarge the table , if false we shrink.
 */
template<class KeyT, class ValueT>
void HashMap<KeyT, ValueT>::_reSize(const bool inLarge)
{
    int newCap = inLarge ? _capacity * 2 : _capacity / 2;
    HashMap<KeyT, ValueT> newHashTable;
    newHashTable._upperLoadFactor = _upperLoadFactor;
    newHashTable._lowerLoadFactor = _lowerLoadFactor;
    newHashTable._capacity = newCap;
    newHashTable._bucketsVec.resize((unsigned long)newCap);
    for (auto& vec: _bucketsVec)
    {
        for (auto& pair : vec)
        {
            newHashTable.insert(pair.first, pair.second);
        }
    }
    *this = newHashTable;
}

/**
*
* @param key the key that we look for is value
* @return the value of the key in the hash map
*/
template<class KeyT, class ValueT>
const ValueT &HashMap<KeyT, ValueT>::at(const KeyT &key) const
{
    if(!containsKey(key))
    {
        throw HashMapInvalidKeyException();
    }
    int index = _findIndex(key);
    Bucket bucketToCheck = _bucketsVec.at(index);
    for(int i = 0 ; i < bucketToCheck.size(); i ++)
    {
        if(bucketToCheck.at(i).first == key)
        {
            ValueT& second = bucketToCheck.at(i).second;
            return second;
        }
    }

}



/**
*
* @param key the key that we look for is value
* @return the value of the key in the hash map
*/
template<class KeyT, class ValueT>
ValueT &HashMap<KeyT, ValueT>::at(const KeyT &key)
{
    if(!containsKey(key))
    {
        throw HashMapInvalidKeyException();
    }
    int index = _findIndex(key);
    Bucket bucketToCheck = _bucketsVec.at(index);
    for(int i = 0 ; i < bucketToCheck.size(); i ++)
    {
        if(bucketToCheck.at(i).first == key)
        {
            return _bucketsVec.at(index).at(i).second;
        }
    }
}


/**
*
* @param keyToFindBucket the key that we want his bucket
* @return the size of the bucket fo the key we want
 */
template<class KeyT, class ValueT>
int HashMap<KeyT, ValueT>::bucketSize(const KeyT &keyToFindBucket) const
{
    if(!containsKey(keyToFindBucket))
    {
        throw HashMapInvalidKeyException();
    }
    int index = _findIndex(keyToFindBucket);
    return _bucketsVec.at(index).size();
}


/**
* we erase a key from the hash map
* @param key the key that we want to erase
* @return true if we erase, false otherwise.
*/
template<class KeyT, class ValueT>
bool HashMap<KeyT, ValueT>::erase(const KeyT &key)
{
    if(!containsKey(key))
    {
        return false;
    }
    _eraseFromBucketVec(key);
    _size--;
    _loadFactor = (double) _size / _capacity;
    if (_loadFactor < _lowerLoadFactor)
    {
        this->_reSize(false);
    }
    return true;
}

/**
* erase a pair from the bucet by the key
* @param key the key that we erase
*/
template<class KeyT, class ValueT>
void HashMap<KeyT, ValueT>::_eraseFromBucketVec(const KeyT &key)
{
    int index = _findIndex(key);
    Bucket& bucket = _bucketsVec.at(index);
    ValueT val = at(key);
    std :: pair<KeyT, ValueT> pairToRemove(key, val);
    bucket.erase(std::remove(bucket.begin(), bucket.end(), pairToRemove), bucket.end());
}

/**
* default move assignment
* @param other other to assign to
* @return the assigned member
*/
template<class KeyT, class ValueT>
void HashMap<KeyT, ValueT>::clear()
{
    for(auto& bucket : _bucketsVec)
    {
        bucket.clear();
    }
    _size = 0;
}


/**
* overloading the operator []
* @param key the key that we want is value
* @return the value of the key in the hash map
*/
template<class KeyT, class ValueT>
ValueT &HashMap<KeyT, ValueT>::operator[](const KeyT &key)
{
    if(containsKey(key))
    {
        return at(key);
    }
    ValueT toAdd;
    if (insert(key, toAdd))
    {
        return at(key);
    }
}

/**
* overloading the operator []
* @param key the key that we want is value
* @return the value of the key in the hash map
 */
template<class KeyT, class ValueT>
const ValueT &HashMap<KeyT, ValueT>::operator[](const KeyT &key) const
{
    if(containsKey(key))
    {
        return at(key);
    }
    else
    {
        throw(HashMapInvalidKeyException());
    }
}

/**
* overloading the operator []
* @param other the other hash map that we compering to
* @return true if this == other , false other wise.
*/
template<class KeyT, class ValueT>
bool HashMap<KeyT, ValueT>::operator==(const HashMap<KeyT, ValueT> &other) const
{

    if (_lowerLoadFactor != other._lowerLoadFactor || _size != other._size ||
        _capacity != other._capacity || _upperLoadFactor != other._upperLoadFactor)
    {
        return false;
    }
    for (int i = 0; i < _capacity  ; i++)
    {
        for(auto& pair : _bucketsVec.at(i))
        {
            if (std::find(other._bucketsVec[i].begin(), other._bucketsVec[i].end(), pair)
                == other._bucketsVec[i].end())
            {
                return false;
            }
        }
    }
    return true;


}

/**
* this is an help funck to the vector const, its insertingg the pairs from the vectors to the hash map
* @param key the key that we insert
* @param value the value we insert
*/
template<class KeyT, class ValueT>
void HashMap<KeyT, ValueT>::_vectorInsert(const KeyT &key, const ValueT &value)
{
    if(containsKey(key))
    {
       this->at(key) = value;

    }
    else
    {
        insert(key, value);
    }

}


#endif //EX3_HASHMAP_HPP
