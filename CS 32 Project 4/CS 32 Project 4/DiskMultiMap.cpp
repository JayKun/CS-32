#include "DiskMultiMap.h"
#include <cstring>
using namespace std; 

//=======================================================================
// Iterator Class 
//=======================================================================

DiskMultiMap::Iterator:: Iterator()
{
	m_valid = false; 
	context = "";
	key = "";
	value = ""; 
	curPos = 0; 
	m_bfIterator = nullptr; 
}

DiskMultiMap::Iterator:: Iterator(const std::string& key, const std::string& context, const std::string& value, BinaryFile::Offset loc, BinaryFile* bf)
{
	m_valid = true; 
	this-> key = key; 
	this-> context = context; 
	this-> value = value;
	curPos = loc; 
	m_bfIterator = bf; 

}

bool DiskMultiMap::Iterator:: isValid() const
{
	return m_valid; 
}


DiskMultiMap::Iterator& DiskMultiMap::Iterator:: operator++()
{
	DiskNode tempNode("", "", 0);
	m_bfIterator->read(tempNode, curPos);
	if (tempNode.nextNode == 0)
	{
		return *(new Iterator()); 
	}
	DiskNode nextNode("","",0); 
	curPos = tempNode.nextNode;
	m_bfIterator->read(nextNode, curPos);
	value = nextNode.m_value;
	return *this; 
}

MultiMapTuple DiskMultiMap::Iterator:: operator*()
{
	MultiMapTuple tuple; 
	tuple.context = this->context; 
	tuple.value = this->value;
	tuple.key = this->key; 
	return tuple; 
}

DiskMultiMap::DiskMultiMap()
{
	head.firstBucket = 0; 
	head.firstEmpty = 0; 
}

DiskMultiMap::~DiskMultiMap()
{

		close(); 
}

bool DiskMultiMap::createNew(const std::string& filename, unsigned int numBuckets)
{
	acquireNode();
	head.firstBucket = 0;
	head.firstEmpty = 0;
	m_filename = filename; 
	n_Buckets = numBuckets; 
	m_bf.createNew(filename); 
	openExisting(filename); 
	m_bf.write(head, 0); 
	for (int i = 0; i < numBuckets; i++)
	{
		BinaryFile::Offset newBucket = 0; 
		m_bf.write(newBucket, m_bf.fileLength());  // write 
		
	}

	// TEST CASE :: 
	for (int i = 0; i < numBuckets; i++)
	{
		BinaryFile::Offset testBucket; 
		m_bf.read(testBucket, head.firstBucket + i * 4); 
	}
	head.firstBucket = 8;
	close(); 
	return true; 
}
bool DiskMultiMap::openExisting(const string& filename)
{
	close(); 
	return m_bf.openExisting(filename); 
}

void DiskMultiMap::close()
{
	m_bf.close(); 
}

bool DiskMultiMap::insert(const std::string& key, const std::string& value, const std::string& context)
{
	if (key.size() >= 120 ||value.size() >= 120 || context.size() >= 120)
		return false;
	
	if (!openExisting(m_filename))			// if the file does not exist then return false
	{
		return false; 
	}
	

	int bucketNum = hashFunc(key); // Choose the "key or Bucket" that the data is supposed to go. 
	BinaryFile::Offset corresBucketOffset = BucketOffset(bucketNum); // The offset of the key in the BinaryFile. 
	BinaryFile::Offset OffsetOfFirstNode; 
	m_bf.read(OffsetOfFirstNode, corresBucketOffset); // assign offsetOfFirstNode to the Corresponding bucket offset. 

	if (OffsetOfFirstNode == 0) // Case 1: the bucket is empty
	{
		DiskNode newNode(value, context, 0);
		BinaryFile::Offset placeToInsert = BinaryFile::Offset(acquireNode()); 
		m_bf.write(BinaryFile::Offset(placeToInsert), corresBucketOffset );  // ASIGN THE OFFSET OF FIRST NODE TO THE CORRESPONDING BUCKET.
		m_bf.write(newNode, placeToInsert);  // ADD NEW NODE TO THE END OF BINARY FILE

	}

	else // Case 2 : the bucket has at least one node.
	{
		DiskNode newNode(value, context, OffsetOfFirstNode);
		BinaryFile::Offset placeToInsert = BinaryFile::Offset(acquireNode());
		m_bf.write(placeToInsert,corresBucketOffset);  // WRITE THE OFFSET OF THE NEW NODE TO THE BUCKET.
		m_bf.write(newNode, placeToInsert); // ADD NEW NODE TO THE END OF BINARY FILE





		//newNode.nextNode = OffsetOfFirstNode; // assign the new node's nextNode to the previous Node. 
		DiskNode testNode("", "", 0);
		m_bf.read(testNode, offsetOfFirstNode(key)); 
	}
	return true; 
	
}
DiskMultiMap::Iterator DiskMultiMap::search(const std::string& key)
{
	int numBucket = hashFunc(key); //find the bucket 
	BinaryFile::Offset corresBucketOffset = BucketOffset(numBucket); // The offset of the key in the BinaryFile. 
	BinaryFile::Offset OffsetOfFirstNode; //find the offset
	
	if (corresBucketOffset == 0)  // KEY IS NOT FOUND
		return Iterator(); 

	m_bf.read(OffsetOfFirstNode, corresBucketOffset); //find the first node from the bucket

	DiskNode tempNode("","",0); // Create Temporary Node

	m_bf.read(tempNode, OffsetOfFirstNode);  // RETRIEVE FIRST NODE
	Iterator it(key, tempNode.m_value, tempNode.m_context, OffsetOfFirstNode, &m_bf);
	return it;  // RETURN THE FIRST NODE OF THE BUCKET.
	// iterate through the list/ 

}


int DiskMultiMap::erase(const std::string& key, const std::string& value, const std::string& context)
{

	int counter = 0;
	if (offsetOfFirstNode(key) == 0)
		return false;
	DiskNode traverseNode("", "", 0);
	DiskNode previousNode("", "", 0);
	DiskNode testFirstNode("", "", 0);
	BinaryFile::Offset previousNodeOffset;
	m_bf.read(testFirstNode, offsetOfFirstNode(key));
	bool firstNodeisErased = false;

	do {
		if (testFirstNode.m_context == context && testFirstNode.m_value == value)
		{
			counter++;
			int bucketNum = hashFunc(key); // Choose the "key or Bucket" that the data is supposed to go. 
			BinaryFile::Offset corresBucketOffset = BucketOffset(bucketNum); // The offset of the key in the BinaryFile. 
			BinaryFile::Offset OffsetOfFirstNode;
			m_bf.read(OffsetOfFirstNode, corresBucketOffset);

			DiskNode NoteToBeDeleted("", "", 0);    // RETRIEVE THE NOTE TO BE DELETED
			m_bf.read(NoteToBeDeleted, OffsetOfFirstNode);
			m_bf.write(NoteToBeDeleted.nextNode, corresBucketOffset); // WRITE THE NEXT NODE OF THE NODE TO BE DELETED TO CORRESBUCKETOFFSET 

			if (head.firstEmpty == 0)
			{
				head.firstEmpty = OffsetOfFirstNode; // Assign the first empty offset to the newly deleted note. 
				DiskNode EmptyNode("", "", 0);
				m_bf.write(EmptyNode, OffsetOfFirstNode); // OVERWRIRE THE DELETED NODE WITH AN EMPTY NODE	
			}
			else // IF the head's first empty is empty 
			{
				DiskNode EmptyNode("", "", head.firstEmpty);
				m_bf.write(EmptyNode, OffsetOfFirstNode); // OVERWRITE THE DELETED NODE WITH AN EMPTY NODE
				head.firstEmpty = OffsetOfFirstNode;
			}
			if (offsetOfFirstNode(key) == 0)
				return counter;
			firstNodeisErased = true;
			m_bf.read(testFirstNode, testFirstNode.nextNode);
			continue;
		}
		firstNodeisErased = false;
	} while (firstNodeisErased);

	m_bf.read(testFirstNode, offsetOfFirstNode(key));
	// Beyond this point we can be sure that the first Node is no longer the target Node 
	if (traverseNode.nextNode != 0)
	{
		// Case 2 : Second Item is removed
		m_bf.read(traverseNode, testFirstNode.nextNode);
		m_bf.read(previousNode, offsetOfFirstNode(key));

		bool isSecondNodeRemobed = false; 
		do {
			if (traverseNode.m_value == value && traverseNode.m_context == context)
			{
				DiskNode newPrevNode(previousNode.m_value, previousNode.m_context, traverseNode.nextNode);
				counter++;
				if (head.firstEmpty == 0)
				{
					DiskNode EmptyNode("", "", 0);
					m_bf.write(EmptyNode, previousNode.nextNode);
					head.firstEmpty = previousNode.nextNode;
				}

				else
				{
					DiskNode EmptyNode("", "", head.firstEmpty);
					m_bf.write(EmptyNode, previousNode.nextNode);
					head.firstEmpty = previousNode.nextNode;
				}
				m_bf.write(newPrevNode, offsetOfFirstNode(key));
				m_bf.read(traverseNode, previousNode.nextNode);
				isSecondNodeRemobed = true;
				if (traverseNode.nextNode == 0)
					break;
				continue;
			}

			isSecondNodeRemobed = false;

		} while (isSecondNodeRemobed);
	}

		if (traverseNode.nextNode != 0)
		{
			previousNodeOffset = testFirstNode.nextNode;
			m_bf.read(traverseNode, traverseNode.nextNode);
			m_bf.read(previousNode, previousNode.nextNode);
			bool isThirdNodeRemoved = false;
			// AT THIS POINT THERE IS TWO NODES IN FRONT OF THE TARGET NODE. 
			// CASE 3 : AT LEAST TWO NODES IN FRONT OF TARGET NODE
			do
			{
				if (traverseNode.m_value == value && traverseNode.m_context == context)
				{
					DiskNode newPrevNode("", "", traverseNode.nextNode);
					counter++;
					if (head.firstEmpty == 0)
					{
						DiskNode EmptyNode("", "", 0);
						m_bf.write(EmptyNode, previousNode.nextNode);
						head.firstEmpty = previousNode.nextNode;
					}

					else
					{
						DiskNode EmptyNode("", "", head.firstEmpty);
						m_bf.write(EmptyNode, previousNode.nextNode);
						head.firstEmpty = previousNode.nextNode;
					}
					m_bf.write(newPrevNode, previousNodeOffset);
					m_bf.read(traverseNode, previousNode.nextNode);
					isThirdNodeRemoved = true;
					if (traverseNode.nextNode == 0)
						break;
					continue;

				}
				isThirdNodeRemoved = false;
			} while (isThirdNodeRemoved);
		}

		if (traverseNode.nextNode!= 0)
		{
			// CASE 4 : AT THIS POINT THERE ARE THREE CORRECT NODES IN FRONT OF TARGET NODE 
			previousNodeOffset = previousNode.nextNode;
			m_bf.write(previousNode, previousNode.nextNode);
			m_bf.read(traverseNode, traverseNode.nextNode);
			while (traverseNode.nextNode != 0)
			{
				if (traverseNode.m_value == value && traverseNode.m_context == context)
				{
					DiskNode newPrevNode("", "", traverseNode.nextNode);
					counter++;
					if (head.firstEmpty == 0)
					{
						DiskNode EmptyNode("", "", 0);
						m_bf.write(EmptyNode, previousNode.nextNode);
						head.firstEmpty = previousNode.nextNode;
					}

					else
					{
						DiskNode EmptyNode("", "", head.firstEmpty);
						m_bf.write(EmptyNode, previousNode.nextNode);
						head.firstEmpty = previousNode.nextNode;
					}
					m_bf.write(newPrevNode, previousNodeOffset);
					m_bf.read(traverseNode, previousNode.nextNode);
				}
				previousNodeOffset = previousNode.nextNode;
				m_bf.write(previousNode, previousNode.nextNode);
				m_bf.write(traverseNode, traverseNode.nextNode);
			}
		}
		// AT THIS POINT THE TRAVERSE NODE IS ALREADY AT THE FINAL NODE
		//CASE 5: DELETING THE LAST NODE
		if (traverseNode.m_context == context && traverseNode.m_value == value)
		{
			DiskNode newPrevNode("", "", 0);
			counter++;
			if (head.firstEmpty == 0)
			{
				DiskNode EmptyNode("", "", 0);
				m_bf.write(EmptyNode, previousNode.nextNode);
				head.firstEmpty = previousNode.nextNode;
			}

			else
			{
				DiskNode EmptyNode("", "", head.firstEmpty);
				m_bf.write(EmptyNode, previousNode.nextNode);
				head.firstEmpty = previousNode.nextNode;
			}
			m_bf.write(newPrevNode, previousNodeOffset);

		}
		return counter;
}



// ==============================================================
// PRIVATE MEMBER FUNCTIONS 
//===============================================================

int DiskMultiMap::hashFunc(const string &name){	int total = 0;	for (int i = 0; i<name.length(); i++)		total = total + int(name[i]);	total = total % n_Buckets;	return(total);}BinaryFile::Offset DiskMultiMap::BucketOffset(int bucketNum){	BinaryFile::Offset offset; 	offset =  4* (bucketNum) + 8; 	return offset; }BinaryFile::Offset DiskMultiMap::acquireNode(){	if (head.firstEmpty == 0)
	{

		return m_bf.fileLength();
	}

	BinaryFile::Offset OffsetOfEmptyNode; 
	OffsetOfEmptyNode = head.firstEmpty;
	DiskNode theEmptyNode("", "", 0);
	m_bf.read(theEmptyNode, head.firstEmpty); 
	head.firstEmpty = theEmptyNode.nextNode; 
	
	return OffsetOfEmptyNode;}BinaryFile::Offset DiskMultiMap::offsetOfFirstNode(const string&key){	int bucketNum = hashFunc(key); // Choose the "key or Bucket" that the data is supposed to go. 
	BinaryFile::Offset corresBucketOffset = BucketOffset(bucketNum); // The offset of the key in the BinaryFile. 
	BinaryFile::Offset OffsetOfFirstNode;
	m_bf.read(OffsetOfFirstNode, corresBucketOffset);	return OffsetOfFirstNode; }