#include "IntelWeb.h"
#include "BinaryFile.h"
#include "DiskMultiMap.h"
#include "InteractionTuple.h"
#include "MultiMapTuple.h"
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <set>
#include <queue>

IntelWeb::IntelWeb()
{}
IntelWeb:: ~IntelWeb()
{
	this->close(); 
}

bool IntelWeb::createNew(const std::string& filePrefix, unsigned int maxDataItems)
{
	this-> close(); 
	bool isSuccessful = m_mapKeyToValue.createNew(filePrefix + "Creation.dat", maxDataItems)
	&& m_mapValueToKey.createNew(filePrefix + "Download.dat", maxDataItems)	;
	return isSuccessful; 
}

bool IntelWeb::openExisting(const std::string& filePrefix)
{
	bool openSuccessful = m_mapKeyToValue.openExisting(filePrefix + "Creation.dat") && m_mapValueToKey.openExisting(filePrefix + "Download.dat");
	if (!openSuccessful)
		close(); 
	return openSuccessful; 
}

void IntelWeb::close()
{
	 m_mapKeyToValue.close(); 
	 m_mapValueToKey.close();
}

bool IntelWeb::ingest(const std::string& telemetryFile)
{
	ifstream inf(telemetryFile); 
	if (!inf)
	{
		cout << "Cannot open telemetryFile." << endl; 
		return false;
	}

	for (int i = 0; i < maxItems; i++)
	{
		prevalance[i] = 0; 
	}

	string line;
	while (getline(inf, line))
	{
		istringstream iss(line);
		string key;
		string value;
		string context;
		if (!(iss >> context >> key >> value))
		{
			cout << "Ignoring badly-formatted input line: " << line << endl;
			continue;
		}
		char dummy;
		if (iss >> dummy) // succeeds if there a non-whitespace char
			cout << "Ignoring extra data in line: " << line << endl;

		int keyBucket = hashFunc(key); 
		int valueBucket = hashFunc(value); 

		prevalance.at(keyBucket)++;
		prevalance.at(valueBucket)++; 

		m_mapKeyToValue.insert(key, value, context);
		m_mapValueToKey.insert(value, key, context);
	}
	return true; 
}

unsigned int IntelWeb:: crawl(const std::vector<std::string>& indicators, unsigned int minPrevalenceToBeGood, std::vector<std::string>& badEntitiesFound,
	std::vector<InteractionTuple>& badInteractions )
{
	badEntitiesFound.clear();
	badInteractions.clear(); 
	DiskMultiMap::Iterator it, itVal;
	set<string> badEntities;
	set<InteractionTuple> badInter; 
	queue<string> badStuff; 

	for (int i = 0; i < indicators.size(); i++)
	{
		badStuff.push(indicators[i]); 
	}
	
	while(!badStuff.empty())
	{
		//check whether the key is in the q. 
		string s = badStuff.front();
		badStuff.pop(); 
		it = m_mapKeyToValue.search(s);
		while (it.isValid() ) 
		{
			if(!isPrevalanceGood((*it).key, minPrevalenceToBeGood))
			{
				badEntities.insert((*it).value);
				InteractionTuple tuple;
				tuple.from = (*it).key;
				tuple.to = (*it).value;
				tuple.context = (*it).context;
				badInter.insert(tuple);
				if (badEntities.find((*it).value) != badEntities.end())
					badStuff.push((*it).value);
			}
			++it;
		}
		itVal = m_mapValueToKey.search(s);
		while (itVal.isValid() )
		{
			if (!isPrevalanceGood((*itVal).key, minPrevalenceToBeGood))
			{
				badEntities.insert((*itVal).value);
				InteractionTuple tuple;
				tuple.from = (*itVal).key;
				tuple.to = (*itVal).value;
				tuple.context = (*itVal).context;
				badInter.insert(tuple);
				if (badEntities.find((*itVal).value) != badEntities.end())
					badStuff.push((*itVal).value);
			}
			++itVal;
		}
		set<string> ::iterator itEntity; 
		itEntity = badEntities.begin(); 
		while (!badEntities.empty())
		{
			badEntitiesFound.push_back(*itEntity); 
			itEntity++; 
		}

		set<InteractionTuple>::iterator itInteraction;
		itInteraction = badInter.begin();
		while (!badEntities.empty())
		{
			badInteractions.push_back(*itInteraction);
			itInteraction++;
		}
		
	}
	return false; 
}


bool IntelWeb:: purge(const std::string& entity)
{
	//Case 1 : remove if entity is a key. 
	DiskMultiMap::Iterator it; 
	it = m_mapKeyToValue.search(entity); 

	if (!it.isValid())
		return false; 
	while (it.isValid())
	{
		m_mapKeyToValue.erase(entity, (*it).value, (*it).context);
		++it; 
	}

	DiskMultiMap::Iterator itValue;
	itValue = m_mapValueToKey.search(entity);
	
	if (!itValue.isValid())
		return false; 

	while (itValue.isValid())
	{
		m_mapValueToKey.erase(entity, (*it).value, (*it).context);
		++itValue; 
	}
	return true;
}

//========================================================================
// member functions
//========================================================================

int IntelWeb::hashFunc(const string& key)
{
	int total = 0;	for (int i = 0; i<key.length(); i++)		total = total + int(key[i]);	total = total % maxItems;	return(total);
}

bool IntelWeb::isPrevalanceGood(const string& key, int minPrevalanceToBeGood)
{
	int target = hashFunc(key);
	int prevalanceOfKey = prevalance[target]; 
	if (prevalanceOfKey >= minPrevalanceToBeGood)
		return true;
	else
		return false;
}
