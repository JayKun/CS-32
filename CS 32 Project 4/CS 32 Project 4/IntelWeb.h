#pragma once
#ifndef INTELWEB_H_
#define INTELWEB_H_

#include "InteractionTuple.h"
#include "DiskMultiMap.h"
#include "MultiMapTuple.h"
#include <string>
#include <vector>

class IntelWeb
{
public:
	IntelWeb();
	~IntelWeb();
	bool createNew(const std::string& filePrefix, unsigned int maxDataItems);
	bool openExisting(const std::string& filePrefix);
	void close();
	bool ingest(const std::string& telemetryFile);
	unsigned int crawl(const std::vector<std::string>& indicators,
		unsigned int minPrevalenceToBeGood,
		std::vector<std::string>& badEntitiesFound,
		std::vector<InteractionTuple>& badInteractions
		);
	bool purge(const std::string& entity);

private:
	// Your private member declarations will go here
	DiskMultiMap m_mapKeyToValue; 
	DiskMultiMap m_mapValueToKey;
	int maxItems; 
	int hashFunc(const string& key); 
	vector<int> prevalance; 
	bool isPrevalanceGood(const string& key, int minPrevelanceToBeGood);
};

inline
bool operator<(const InteractionTuple& a, const InteractionTuple &b)
{
	if (a.context != b.context)
		return a.context < b.context;
	else if (a.from != b.from)
		return a.from < b.from;
	else if (a.to != b.to)
		return a.to < b.to;
	return false; 


}
#endif // INTELWEB_H_