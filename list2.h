// list2.h: stubbed CObList using std::list for portability
#ifndef __LIST2_H__
#define __LIST2_H__

#include <list>

class CSceneObject;

// CObList: simple wrapper around std::list for CSceneObject* pointers
class CObList {
public:
    // Iterator type for list traversal (immutable)
    using POSITION = std::list<CSceneObject*>::const_iterator;

    CObList() = default;
    ~CObList() = default;

    // Add object to end of list
    void AddTail(CSceneObject* obj) {
        list_.push_back(obj);
    }

    // Get iterator to first element
    POSITION GetHeadPosition() const {
        return list_.cbegin();
    }
    // Get iterator to end (one past last element)
    POSITION GetEndPosition() const {
        return list_.cend();
    }

    // Get next object and advance position
    CSceneObject* GetNext(POSITION& pos) const {
        if (pos == list_.end()) return nullptr;
        CSceneObject* tmp = *pos;
        ++pos;
        return tmp;
    }

    // Remove element at position
    void RemoveAt(POSITION pos) {
        list_.erase(pos);
    }

private:
    std::list<CSceneObject*> list_;
};

// Global alias for list iterator position (legacy POSITION)
using POSITION = CObList::POSITION;

#endif // __LIST2_H__