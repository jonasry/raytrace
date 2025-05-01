//	List class for class T
//	used to group lights and objects

#ifndef __LIST2_H__
#define __LIST2_H__

#ifndef _WINDOWS

class Item;

class CSceneObject;

typedef Item* POSITION;               
//#define NULL 0


//	----------------------------
//	----------------------------
//	class CList
//	----------------------------
//	----------------------------

class CList {

friend class Item;

private:


public:
	CList();
	~CList();

	void Add(CSceneObject* Object);	// Add a new object to this list
	void AddTail(CSceneObject* Object) {Add(Object);}
	CSceneObject* RemoveTail();		// Remove current object from list
	void RemoveAt(POSITION) {};
	void Front() const;				// Put current object first in list
	void Back() const;				// Put current object last in list
	POSITION  		Reset() const;
	POSITION  		GetHeadPosition() const { return m_Top; }
	CSceneObject*	GetNext(POSITION&) const;
	CSceneObject* 	Get(POSITION) const;		// Get a reference to an item

private:
	Item*	m_Top;
	Item*	m_Last;

};                  

typedef class CList CObList;



//	----------------------------
//	----------------------------
//	class Item
//	----------------------------
//	----------------------------

class Item {

friend class CList;

public:

	Item(CSceneObject* O):Object(O),Next(0),Prev(0) {}

	~Item() {

		if (Next!=0) delete Next;
//		delete Object;

	}

private:

	CSceneObject*	Object;
	Item*	Next;
	Item*   Prev;


};



//	----------------------------
//	----------------------------
//	Member function definitions
//	----------------------------
//	----------------------------

inline
CList::CList() {

	m_Top=0;
	m_Last=m_Top;
}



inline
CList::~CList() {

	if (m_Top!=0) delete m_Top;

}



inline
void CList::Add(CSceneObject* Object) {
                  
	//	Add the object to bottom of the list

	if (m_Last==0)
		m_Top = m_Last = new Item(Object);
	else {
		m_Last->Next = new Item(Object);
		m_Last->Next->Prev=m_Last;
		m_Last = m_Last->Next;
	}

}                                      



inline
CSceneObject* CList::RemoveTail() {
                  
	//	Remove the object at the bottom of the list

	if (m_Last==0)
		return 0;
	else {
		Item* tmpItem=m_Last;
		CSceneObject* theObject;
		theObject = tmpItem->Object;
		m_Last = m_Last->Prev;
		if (m_Last==0) 
			m_Top=0;
		delete tmpItem;
		return theObject;

	}
}                                      



inline
CSceneObject* CList::Get(POSITION I) const {

	return I->Object;

}



inline
CSceneObject* CList::GetNext(POSITION& I) const {

	CSceneObject* tmpObj = 0;
	if (I!=0) {
		tmpObj=Get(I);
		I = I->Next;
	}
	return tmpObj;

}



inline
POSITION CList::Reset() const {

	return POSITION(m_Top);

}
#endif
#endif
