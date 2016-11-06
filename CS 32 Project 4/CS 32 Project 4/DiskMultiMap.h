#pragma once

#ifndef DISKMULTIMAP_H_
#define DISKMULTIMAP_H_

#include <string>
#include "MultiMapTuple.h"
#include "BinaryFile.h"
#include <stdlib.h>
#include <cstring>
#include <map>

using namespace std; 

class DiskMultiMap
{
public:

	class Iterator
	{
	public:
		Iterator();
		// You may add additional constructors
		Iterator(const std::string& key, const std::string& context, const std::string& value, BinaryFile::Offset loc, BinaryFile* bf);
		bool isValid() const;
		Iterator& operator++();
		MultiMapTuple operator*();

	private:
		// Your private member declarations will go here
		
		BinaryFile::Offset curPos;
		string key; 
		string value; 
		string context; 

		bool m_valid; 
		BinaryFile* m_bfIterator; 
	};

	DiskMultiMap();
	~DiskMultiMap();
	bool createNew(const std::string& filename, unsigned int numBuckets);
	bool openExisting(const std::string& filename);
	void close();
	bool insert(const std::string& key, const std::string& value, const std::string& context);
	Iterator search(const std::string& key);
	int erase(const std::string& key, const std::string& value, const std::string& context);

private:
	struct Header
	{
		BinaryFile::Offset firstBucket; 
		BinaryFile::Offset firstEmpty; 
	};
	struct DiskNode
	{
		DiskNode(const string& value, const string& context, BinaryFile::Offset next)
		{

			//m_value[0] = value[0]; 
			//m_context[0] = context[0]; 
			strcpy_s(m_value,121,value.c_str()); 
			strcpy_s(m_context,121, context.c_str()); 
			nextNode = next; 
		}
		
		//member variables
		char m_value[121]; 
		char m_context[121]; 
		BinaryFile::Offset nextNode; 

	};

	//member functions
	int hashFunc(const string& s);
	BinaryFile::Offset BucketOffset(int bucketNum); 
	BinaryFile::Offset acquireNode(); 
	BinaryFile::Offset offsetOfFirstNode(const string& key);
	BinaryFile m_bf; 
	int n_Buckets; 
	string m_filename; 
	Header head; 
};
#endif // DISKMULTIMAP_H_