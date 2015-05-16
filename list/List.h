/*
 * List
 *
 *  Created on: Jan 14, 2015
 *      Author: Philip
 */

#ifndef LIST_
#define LIST_

#include <iostream>

using std::cin;
using std::cout;
using std::endl;

//min size before a bottem feeder summons a left and right skeleton
#define MinSize 2
// specify different levels to get different levels of ease of use.
// Increased ease of use decreases program speed
//x < 0 for no ease of use. x > 40 for infinite ease of use
// x > 0 for negative memp
#define UserLevel 33
// must be 1 in order for debug to work. has several cout<< statements that may allude to an error
#define debug 1
// if quickfind is 1, the list sees if the element was recently found
#define quickfind 0
//distance in both directions useed in navigate 2 for quickly finding previous values
#define quickfindThreshold 10
//approaches a node from this->right instead of this->left in order to find it faster. 1 for true
#define ReverseFind 0
//when decides whether values to be added to the list are pass by value or pass by reference
#define PassByReference 1

typedef double datatype;

//operations that act on a node
//returns 1 if a > b
bool isBigger(const datatype &a, const datatype& b) {
	return a > b;
}
//returns 1 if a < b
bool isSmaller(const datatype &a, const datatype& b) {
	return b > a;
}
//returns 1 if a==b
bool isEqual(const datatype &a, const datatype& b) {
	return a == b;
}

bool BiggerEqual(const datatype &a, const datatype& b) {
	return !isSmaller(a, b);
}

bool SmallerEqual(const datatype &a, const datatype& b) {
	return !isBigger(a, b);
}

class node {
public:
	datatype TheStuff;
	node* next;
	node* prev;

	bool operator >(const node& cat) {
		return (isBigger(this->TheStuff, cat.TheStuff));
	}
	bool operator <(const node& cat) {
		return (isSmaller(this->TheStuff, cat.TheStuff));
	}
	bool operator ==(const node& cat) {
		return (isEqual(this->TheStuff, cat.TheStuff));
	}
	bool operator >=(const node& cat) {
		return BiggerEqual(this->TheStuff, cat.TheStuff);
	}
	bool operator <=(const node& cat) {
		return SmallerEqual(this->TheStuff, cat.TheStuff);
	}
};

void NodalPrint(node* someting) {
	cout << someting->TheStuff;
}

class skeleton {
	friend class overloard;
public:
	//LeftLim,RightLim points to a pointer that contains the location of a center loc. They are set by boss when summoned
	//LeftLim and RightLim point to where another pointer (such as bos->center , (first, last)(controlled by overload/whatever uses it))
	node** LeftLim;
	node** RightLim;
	node* center;
	//The left and right skeleton that might be summoned. They are initially uninitialized
	skeleton* left;
	skeleton* right;
	//Size the number of nodes starting at LeftLim and working up to RightLim-1
	//There is a right for every left at a given level
	long size;
	skeleton* boss;
	bool bottemfeeder;
	bool IsOverloard;
	//the increment for when the skeleton is still a bottom feeder
	//is only called when it indeed is a bottom feeder as steps taken to ensure that it will not be called accidently.

	skeleton() {
#if debug==1
		left = 0; //Unnecessary
		right = 0; //Unnecessary
		LeftLim = 0; //Unnecessary
		RightLim = 0; //Unnecessary
		center = 0; //Unnecessary as balance later
		boss = 0; //Unnecessary
#endif
		size = 0;
		bottemfeeder = 1;
		IsOverloard = 0;
	}
	~skeleton() {
		//doesn't have a left and right if it is a bottemfeeder
		if (!bottemfeeder) {
			delete left;
			delete right;
			left = 0;
			right = 0;
#if debug==1
			left = 0;
			right = 0;
#endif
		}
//deletes its own memory
	}
	void bottemfeeder_incSize();
	void bottemfeeder_decSize();
	void incSize();
	void decSize();
	void balance();
	node* memp(int num);
	node* RelativeNavigate1(int displacement);
	node* RelativeNavigate2(int displacement);
	node* mempN(int num);
	void summon();
	void organize();
	void SubLevelPrint();
	void BottemFeederPrint();
};
void skeleton::incSize() {
	size++;
	if (!IsOverloard)
		boss->incSize();
}
void skeleton::decSize() {
	size--;
	if (!IsOverloard)
		boss->decSize();
}

//memp(memory +) navigates num nodes starting at last
node* skeleton::memp(int num) {
	//Finds where the first node is located in the skeleton sub-level structure
	node* curp = (node*) *LeftLim;
	//for debug
#if debug==1
	if (num < 0)
		cout << "memp error!" << endl;
#endif
	for (int i = 0; i < num; i++) {
		curp = curp->next;
	}
	return curp;
}

//finds the node that is displacement away from this->(node*)LeftLim
//simmilar to memp except designed to skip through skeleton to find the node by navigating the boss and it's own structure
//does not do negative displacement except for when quickfind
//can always replace memp
// doesn't go below the level it is at to aviod problems when skelleton* organize() is called
node* skeleton::RelativeNavigate1(int displacement) {
#if quickfind == 1
	if (displacement > -quickfindThreshold && displacement < quickfindThreshold) {
		if (displacement >= 0)
		return memp(displacement);
		else
		return mempN(displacement);
	}
#endif
#if debug==1
	if (displacement < 0)
		cout << "RelativeNavigate2 error!" << endl;
#endif

	int level = 0;
//first goes up
	skeleton* curskel = this;
	while (1) {
		if (curskel->size >= displacement)
			break;
		else {
			curskel = curskel->boss;
			level++;
		}
	}
	//now comes down to an appropriate bottem feeder
	while (level != 0) {
		if (curskel->bottemfeeder)
			break;
		if (curskel->left->size > displacement) {
			curskel = curskel->left;
		}
		else {
			displacement += curskel->left->size;
			curskel = curskel->right;
		}
		level--;
	}
	//will always be less than half when used with balance()

	//now memps to the correct location
#if ReverseFind==1
	if (displacement > curskel->size / 2) {
		cout << "true" << endl;
		return curskel->mempN(curskel->size - displacement - 1);
	}
	else {
		return curskel->memp(displacement);
	}
#else
	return curskel->memp(displacement);
#endif
}

//only used for balance as balance never does mempN
node* skeleton::RelativeNavigate2(int displacement) {
#if quickfind == 1
	if (displacement > -quickfindThreshold && displacement < quickfindThreshold) {
		if (displacement >= 0)
		return memp(displacement);
		else
		return mempN(displacement);
	}
#endif
#if debug==1
	if (displacement < 0)
		cout << "RelativeNavigate2 error!" << endl;
#endif

	int level = 0;
//first goes up
	skeleton* curskel = this;
	while (1) {
		if (curskel->size >= displacement)
			break;
		else {
			curskel = curskel->boss;
			level++;
		}
	}
	//now comes down to an appropriate bottem feeder
	while (level != 0) {
		if (curskel->bottemfeeder)
			break;
		if (curskel->left->size > displacement) {
			curskel = curskel->left;
		}
		else {
			displacement += curskel->left->size;
			curskel = curskel->right;
		}
		level--;
	}
	//will always be less than half when used with balance()
	return curskel->memp((int) displacement);

	//now memps to the correct location
#if ReverseFind==1
	if (displacement > curskel->size / 2) {
		cout << "true" << endl;
		return curskel->mempN(curskel->size - displacement - 1);
		//	return curskel->mempN(curskel->size);

	}
	else {
//		cout << "false" << endl;
		return curskel->memp(displacement);
	}
#endif
}

//just like memp, but in the negative direction
// note rightlim instead of leftlim
node* skeleton::mempN(int num) {
	//Finds where the first node is located in the skeleton sub-level structure
	node* curp = (node*) *RightLim;
	//for debug
#if debug==1
	if (num < 0)
		cout << "mempN error!" << endl;
#endif
	for (int i = 0; i < num; i++) {
		curp = curp->prev;
	}
	return curp;
}

//Different from skeleton::incSize in order to be more efficient
void skeleton::bottemfeeder_incSize() {
	this->incSize();
	//If it isn't a bottemfeeder, a mistake is made; this function shouldn't have been called!
	if (bottemfeeder && size > MinSize)
		summon();
}
void skeleton::bottemfeeder_decSize() {
	this->decSize();
	//If it isn't a bottemfeeder, a mistake is made; this function shouldn't have been called!
}

//balances the center pointer. causes problems if it is not the bottom feeder as sizes would need to be changed
void skeleton::balance() {
//	center = memp(size / 2); //was memp(size/2)
	center = RelativeNavigate2(size / 2); //was memp(size/2)
}

void skeleton::summon() {
//	cout << "summoning!" << endl;
	this->balance();
	right = new skeleton;
	left = new skeleton;
	right->boss = this;
	left->boss = this;

	left->LeftLim = this->LeftLim;
	//points to the address of center so that it changes automatically if boss->center (currently this) changes
	left->RightLim = (node**) &center; //void** unnecessary as center is void*

	right->RightLim = this->RightLim;
	right->LeftLim = (node**) &center; //same thing. unnecessary

	left->size = size / 2;
	right->size = size - size / 2;
	bottemfeeder = 0;
}
//a recursive re-organization of center values
//adds left and right if it is a bit big
//only the overloard know it's size in the beginning
void skeleton::organize() {
	this->balance();
	//summons left and right if it (a bottem feeder) is larger than the min size required
//cout << "organizing" << endl;
//this->SubLevelPrint();
	if (bottemfeeder && size > MinSize) {
		summon();
		left->organize();
		right->organize();
		return;
	}
	if (!bottemfeeder) {
		//triggers the recursive delete of left and right if they arn't big enough
		if (size < MinSize) {
			//Checks to see if it's boss is less than the MinSize. If this is the case, the boss needs to get organized
			if (boss->size < MinSize) {
				//the overloard has no boss. organizing itself would create an infinite overflow (aka stack overflow!)
				if (!IsOverloard)
					boss->organize();
				else
					this->bottemfeeder = 1;
			}
			else {
				delete left;
				delete right;
				bottemfeeder = 1;
			}
		}
		else {
//#ifdef debug
//			cout << "condition met" << endl;
//#endif
//before left and right are organized, they need to know their size
			left->size = this->size / 2;
			right->size = size - left->size;
			//triggers the recursive organizaton of its left and right minions
			left->organize();
			right->organize();
		}
	}
}
void skeleton::SubLevelPrint() {
	node* curent;
	curent = (node*) *LeftLim;
//	cout << "current: " << curent << endl;
//	cout << "Last: " << *LeftLim << endl;
//	cout << "size " << size << endl;
	for (int i = 0; i < this->size; i++) {
		NodalPrint(curent);
		cout << "  ";
		curent = curent->next;
	}
#if debug==1
	cout << " Size: " << size;
#endif
	cout << endl;
}

void skeleton::BottemFeederPrint() {
	if (this->bottemfeeder) {
		SubLevelPrint();
		return;
	}
	else {
		left->BottemFeederPrint();
		right->BottemFeederPrint();
		return;
	}
}

//There is only 1 overloard. It has no boss
class overloard: public skeleton {
public:
//boss is unused, but does nothing.
// LeftLim = last*
// RightLim = first*
	//size is the number of nodes
	overloard(void** last, void** first) {
		this->LeftLim = (node**) last;
		this->RightLim = (node**) first;
		this->organize();
		size = 1;
		//many times stuff asks to see the boss
		boss = this;
		//to prevent errors when calling dec and inc
		//	boss = new skeleton;
		IsOverloard = 1;
	}
	overloard() {
	}
	//overloard doesn't have a boss
	void incSize() {
		size += 1;
	}
};

class list {
public:
	long* size;
	node* last; //smallest, element 0
	node* first; //biggest, element size-1
	node* allocator;
	overloard* servant;
	long LastElement;
	// used for quick find
	node* LastElementNode;
	node* theNULL;
	list(datatype& FirstValue) {
		theNULL = new node;
#if debug==1
		theNULL->TheStuff = 999;
#endif
		// size = 1; is set by the overloard
		allocator = new node[1];
		allocator->next = theNULL;
		allocator->prev = theNULL;
		first = allocator;
		last = allocator;
		allocator->TheStuff = FirstValue;
		servant = new overloard((void**) &last, (void**) &first);
		size = &servant->size;
		LastElement = 0;
		LastElementNode = allocator;

	}

	void print();

#if (PassByReference)
  	void append(datatype& 	cat);
#else
    void append(datatype cat);
#endif

	void rmNode(long element);
	//Adds the Node to the left of the specified element
	void mkNodeLeft(int element, datatype
#if (PassByReference)
					&
#endif
					cat);
	void mkNodeRight(int element, datatype
#if (PassByReference)
						&
#endif
						cat);
	void navigate(long element, node*& curNode, skeleton*& LastSkel);
	node* navigate2(long element);
	node* QuickPosNav(int num, node*& LastNode);
	node* QuickNegNav(int num, node*& LastNode);
	void ProperSortAdd1(node* bro, overloard* &SomeOverloard);
	void MergeForSort(node* lonely, skeleton* &curSkel /*points to address of curSkel*/);
	void FullSort1();
	void setElement(int element, datatype
#if (PassByReference)
						&
#endif
	                value);
	void properAdd(datatype
#if (PassByReference)
						&
#endif
						bob);

};
//for simply finding the node that something belongs to. basically has 1 argument
node* list::navigate2(long element) {
	node* curNode;
#if quickfind==1

	if ((quickfindThreshold + LastElement > element) && (LastElement - quickfindThreshold < element)) {
		if (element - LastElement >= 0) {
			LastElementNode = QuickPosNav(element - LastElement, LastElementNode);
		}
		else {
			LastElementNode = QuickNegNav(LastElement - element, LastElementNode);
		}
		LastElement = element;
		return LastElementNode;
	}
#endif
//else...
	//makes static skel to make navigate happy
	static skeleton* DontCare;
	navigate(element, curNode, DontCare);
	return curNode;
}

//finds the node address and the skeleton bottom feeder that contains the node
void list::navigate(long element, node* &curNode, skeleton* &curSkel) {

	//	skeleton* curSkel = servant;
	curSkel = servant;
	//lastElement is usually going to be a center distance from LeftLim
	long lastElement = 0;
	for (int sent = 0; sent < 99; sent++) {
		if (curSkel->bottemfeeder) {
//the strait forward approach
//(lastElement <= element) always evaluates to be true

#if ReverseFind==1
#define displacement element - lastElement
			if (element - lastElement > curSkel->size / 2) {

				curNode = curSkel->mempN(curSkel->size - (element - lastElement) - 1);
			}
			else {

				curNode = (node*) curSkel->memp(element - lastElement);
			}
#else
			curNode = (node*) curSkel->memp(element - lastElement);
#endif

//		curNode = (node*) curSkel->memp(element - lastElement);

			/*
			 #if quickfind==1
			 LastElement = element;
			 LastElementNode = curNode;
			 #endif
			 */
			return;
		}
		else {
			//the center piece belongs to right
			if (lastElement + curSkel->left->size <= element) {
				lastElement += curSkel->left->size;
				curSkel = curSkel->right;
			}
			else {
//don't change lastElement 'because boss->LeftLim == left->LeftLim
				curSkel = curSkel->left;
			}
		}
	}

}

node* list::QuickPosNav(int num, node*& LastNode) {
	node* returnNode = LastNode;
	for (int i = 0; i < num; i++)
		returnNode = returnNode->next;
	return returnNode;
}
//takes absolute value of num. goes in negative direction
node* list::QuickNegNav(int num, node*& LastNode) {
	node* returnNode = LastNode;
	for (int i = 0; i < num; i++)
		returnNode = returnNode->prev;
	return returnNode;
}

//just adds a value to the list
void list::append(datatype
#if (PassByReference)
						&
#endif
                  cat) {
	allocator = new node;
//the former first node is previous
	allocator->prev = first;
//makes it point to theNULL
	allocator->next = theNULL;
//allocator's node is the new first
	first->next = allocator;
//it's now official
	first = allocator;
//get the value it's told to get
	allocator->TheStuff = cat;
	*size += 1;
}

//prints starting at last and ending at first
void list::print() {
	node* curent;
	curent = last;
	for (int i = 0; i < *size; i++) {
		NodalPrint(curent);
		cout << ", ";
		curent = curent->next;
		if (i % 20 == 0 && i != 0)
			cout << endl;
	}
	cout << endl;
}

//see list::navigate; most comes from navigate
void list::rmNode(long element) {
	node* nodeLoc;
	skeleton* curSkel;
	navigate(element, nodeLoc, curSkel);
	//gotta decriment the size before the skeleton is modified
//	curSkel->size -= 1;
	//If LeftLim is pointing at the node
	curSkel->decSize();

	if ((void*) nodeLoc == *curSkel->LeftLim) {

		/*		if (nodeLoc == this->last) {
		 *curSkel->RightLim = nodeLoc->prev;
		 }*/
//error if no next
		nodeLoc->prev->next = nodeLoc->next;
		nodeLoc->next->prev = nodeLoc->prev;
		//	cout << "moving" << endl;
		*curSkel->LeftLim = nodeLoc->next;
		if (curSkel->size < MinSize) {
			curSkel->boss->organize();
//			cout << "boss reorg" << endl;
//			cout << "New boss:" << endl;
//			curSkel->boss->SubLevelPrint();
//
//			(curSkel->boss)->organize();
//			cout << "new left:" << endl;
//			curSkel->boss->left->BottemFeederPrint();
//			cout << "new right:" << endl;
//			curSkel->boss->right->BottemFeederPrint();
		}
	}
	else {
		nodeLoc->prev->next = nodeLoc->next;
		nodeLoc->next->prev = nodeLoc->prev;
	}
//curSkel -> size -= 1;
//cout << nodeLoc ->TheStuff << endl;
//cout << nodeLoc ->prev ->TheStuff << endl;
//cout << nodeLoc -> next ->TheStuff << endl;

	delete nodeLoc;
}
//Creates a node with value cat to the left of the specified element
void list::mkNodeLeft(int element, datatype
#if (PassByReference)
						&
#endif
						cat) {
	node* RightNode;
	skeleton* CurSkel;
	navigate(element, RightNode, CurSkel);
	//CurSkel remains the skeleton that contains the point
	allocator = new node;
	node* &InsertedNode = allocator;
	InsertedNode->TheStuff = cat;

	InsertedNode->next = RightNode;
	InsertedNode->prev = RightNode->prev;
	RightNode->prev = InsertedNode;
	InsertedNode->prev->next = InsertedNode;

	//CurSkel needs to point to the inserted node if it was pointing to the right node
	//This is also where element zero checking is done.
	//Will never be a last element as there will always be something to it's right
//Keep in mind that node* theNull exits to prevent errors!
	if ((void*) RightNode == *CurSkel->LeftLim) {
		*CurSkel->LeftLim = (node*) InsertedNode;
	}
	CurSkel->incSize();
}
//slightly different from mkNodeRight
void list::mkNodeRight(int element, datatype
#if (PassByReference)
						&
#endif
						cat) {
	node* leftNode;
	skeleton* curSkel;
	navigate(element, leftNode, curSkel);
	allocator = new node[1];
	node* &insertedNode = allocator;
	if ((void*) leftNode == *curSkel->RightLim) {
		*curSkel->RightLim = (node*) insertedNode;
	}
	insertedNode->TheStuff = cat;
	insertedNode->next = leftNode->next;
	insertedNode->prev = leftNode;
	leftNode->next = insertedNode;
	insertedNode->next->prev = insertedNode;
	curSkel->incSize();
}

//largest values to to first, smallest values to last
//changes wherever node* bro is, so don't expect it not to WARNING!
// adds bro to the list... or at least some skeleton
/*bro wants to know where he fits in*/
void list::ProperSortAdd1(node* bro, overloard* &SomeOverloard) {
//bear in mind that both skeleton.left and skeleton.right contain center
	skeleton* curSkel = SomeOverloard;
//	curSkel = this->servant;

#if debug==1
//	if (curSkel != servant)
//		cout << "other pointer error!" << endl;
	if (curSkel == 0)
		cout << "null pointer!" << endl;
#endif
	//the thing needs to be expanded if the node is outside the bounds. as a result, this is by itself
	//On bottomfeeder level: if >= , it defaults to the right
	if (*bro >= *curSkel->center) {
		// needs to be >= to avoid rightlim not changing even though it's added
		if (*bro >= *(node*) *curSkel->RightLim) {
			node &firstNode = *(node*) *curSkel->RightLim;
			firstNode.next = bro;
			*curSkel->RightLim = /*(void*)*/bro;
			bro->prev = &firstNode;
			bro->next = this->theNULL;
			//change this later for more speed?
			curSkel->incSize();
			return;	//Unnecessary
		}
		//if it's bigger or equal to the center, but less than rightlim
		//right is == defaulted
		else {
			MergeForSort(bro, curSkel);
			//		curSkel->incSize();
		}
	}
	//its less or equal to the center
	else {
		//if it's a new extreme
		if (*bro < *(node*) *curSkel->LeftLim) {
			node& lastNode = *(node*) *curSkel->LeftLim;
			bro->next = &lastNode;
			bro->prev = theNULL;
			lastNode.prev = bro;
			*curSkel->LeftLim = bro;
			curSkel->incSize();
			return;
		}
		//if it's not an extreme, but is in the lower section
		else {
			MergeForSort(bro, curSkel);
//			curSkel->incSize();
		}
	}
}

void list::MergeForSort(node* lonely, skeleton* &curSkel) {
	//earlier stuff garantees that the lonely node will be able to fit in
	node* leftOfLonely;
	if (curSkel->bottemfeeder) {
		leftOfLonely = (node*) *curSkel->RightLim;
		do {
			if (*lonely < *leftOfLonely) {
				leftOfLonely = leftOfLonely->prev;
				//stops if lonely >= leftOfLonely
				continue;
			}
			// if leftOfLonely <= lonely, insert the lonely node
			else {

				lonely->prev = leftOfLonely;
				lonely->next = leftOfLonely->next;
				leftOfLonely->next->prev = lonely;
				leftOfLonely->next = lonely;
				//curSkel already set
				// Size incrementation is not handled by this function
				curSkel->incSize();
				return;
			}
		} while (1);
	}
	else {
		//goes right if ==
		if (*lonely < *curSkel->center)
			curSkel = curSkel->left;
		else
			curSkel = curSkel->right;
		//recursively calles itself again. gotta get that bottom feeder!
		MergeForSort(lonely, curSkel);
	}
}

void list::FullSort1() {
	node* curNode = this->last;
#if debug ==1
	if (size <= 0)
		cout << "size error!" << endl;
#endif
	overloard* newOverloard = new overloard((void**) &last, (void**) &first);
	newOverloard->center = curNode;
//effectively compleyly overwirtes the servant and list first and last
	this->first = curNode;
	this->last = curNode;
	curNode = curNode->next;
	node* next;
	for (long i = 1; i < *this->size; i++) {
		next = curNode->next;
		ProperSortAdd1(curNode, newOverloard);
		curNode = next;
	}
	delete servant;
	servant = newOverloard;
}

//adds it by finding where it fits in
void list::properAdd(datatype
#if (PassByReference)
						&
#endif
						bob) {
	allocator = new node;
	allocator->TheStuff = bob;
	ProperSortAdd1(allocator, servant);
}
void list::setElement(int element, datatype
#if (PassByReference)
						&
#endif
						value) {
	allocator = navigate2(element);
	allocator->TheStuff = value;
}
#endif
