#pragma once
#include "MemoryAllocator.h"

template <typename T> class DynamicPointerArray {
private:
    union element
    {
      T* singleElement;
      T** multipleElements;
    } u;
    uint16_t arraySize = 0;
public:
    ~DynamicPointerArray()
    {
      if (arraySize > 2)
        free(u.multipleElements);
    }

    void clear()
    {
      if (arraySize > 2)
        free(u.multipleElements);
      arraySize = 0;
      u.singleElement = nullptr;
    }
  
    uint16_t size() const { return arraySize; }

    void push_back(T* element){
      uint16_t currentSize = arraySize;
      arraySize++;
      if (currentSize == 0)
      {
        u.singleElement = element; 
      }
      else if (currentSize == 1)
      {
        T** newBuffer = (T**) HS_MALLOC(arraySize * sizeof(T*));
        if (newBuffer == NULL)
          throw "Fatal error, no more memory";
        newBuffer[0] = u.singleElement;
        newBuffer[1] = element;
        u.multipleElements = newBuffer;
      }
      else
      {
        T** newBuffer = (T**) HS_REALLOC(u.multipleElements, arraySize * sizeof(T*));
        if (newBuffer == NULL)
          throw "Fatal error, no more memory";
        newBuffer[arraySize - 1] = element;
        u.multipleElements = newBuffer;
      }
    }
    T* front() const
    {
        return get(0);
    }
    T* back() const
    {
      return get(arraySize - 1);
    }
    bool empty() const
    {
      return arraySize == 0;
    }
    T* operator[](uint16_t index) const
    {
      return get(index);
    }

    T* get(uint16_t index) const
    {
      if (index >= arraySize)
      {
        throw "Fatal error. Index does not exist.";
      }
      if (arraySize == 1)
        return u.singleElement;
      else
        return u.multipleElements[index];
    }


    void remove(uint16_t index)
    {
      if (index >= arraySize)
      {
        throw "Fatal error. Index does not exist.";
      }
      uint16_t currentSize = arraySize;
      if (currentSize == 1)
      {
        u.singleElement = nullptr;
      }
      else if (currentSize == 2)
      {

        if (index == 1)
        {
          T* firstElement = u.multipleElements[0];
          free(u.multipleElements);
          u.singleElement = firstElement;
        }
        else
        {
          T* secondElement = u.multipleElements[1];
          free(u.multipleElements);
          u.singleElement = secondElement;
        }
      }
      else
      {
        for (int i = index; i < currentSize - 1; i++)
        {
          u.multipleElements[i] = u.multipleElements[i + 1];
        }
        T** newBuffer = (T**) HS_REALLOC(u.multipleElements, arraySize * sizeof(T*));
        if (newBuffer == NULL)
          throw "Fatal error, no more memory";
        u.multipleElements = newBuffer;
      }
      arraySize--;
    
    }

    class DynamicPointerArrayIterator
    {
        const DynamicPointerArray* data;
        uint32_t currentIndex;
        friend class DynamicPointerArray;
    public:
        DynamicPointerArrayIterator(const DynamicPointerArrayIterator& r) :
        data(r.data),currentIndex(r.currentIndex) { }
       
        DynamicPointerArrayIterator(const DynamicPointerArray* arr, uint32_t startIndex): data(arr),currentIndex(startIndex) { }
        T* operator*() const
        {
            return data->get(currentIndex);
        }
        const DynamicPointerArrayIterator operator++(int)
        {
            DynamicPointerArrayIterator temp = *this;
            ++*this;
            return temp;
        }
        DynamicPointerArrayIterator& operator++()
        {
            ++currentIndex;
            return *this;
        }
        bool operator==(const DynamicPointerArrayIterator& lhs)
        {
            return currentIndex==lhs.currentIndex;
        }
        bool operator!=(const DynamicPointerArrayIterator& lhs)
        {
            return currentIndex!=lhs.currentIndex;
        }
    };
    DynamicPointerArrayIterator begin() const{ return DynamicPointerArrayIterator(this,0); }
    DynamicPointerArrayIterator end() const{ return DynamicPointerArrayIterator(this, arraySize); }

    class DynamicPointerArrayReverseIterator
    {
        const DynamicPointerArray* data;
        uint32_t currentIndex;
        friend class DynamicPointerArray;
    public:
        DynamicPointerArrayReverseIterator(const DynamicPointerArrayReverseIterator& r) :
        data(r.data),currentIndex(r.currentIndex) { }
        
        DynamicPointerArrayReverseIterator(const DynamicPointerArray* arr, uint32_t startIndex): data(arr),currentIndex(startIndex) { }
        T* operator*() const
        {
            return data->get(currentIndex);
        }
        const DynamicPointerArrayReverseIterator operator++(int)
        {
            DynamicPointerArrayReverseIterator temp = *this;
            ++*this;
            return temp;
        }
        DynamicPointerArrayReverseIterator& operator++()
        {
            currentIndex--;
            return *this;
        }
        bool operator==(const DynamicPointerArrayReverseIterator& lhs)
        {
            return currentIndex==lhs.currentIndex;
        }
        bool operator!=(const DynamicPointerArrayReverseIterator& lhs)
        {
            return currentIndex!=lhs.currentIndex;
        }
    };
    DynamicPointerArrayReverseIterator rbegin() const{ return DynamicPointerArrayReverseIterator(this,arraySize - 1); }
    DynamicPointerArrayReverseIterator rend() const{ return DynamicPointerArrayReverseIterator(this,  0 - 1); }

    void erase(DynamicPointerArrayIterator iterator)
    {
      remove(iterator.currentIndex);
    }
};