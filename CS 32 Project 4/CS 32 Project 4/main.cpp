#include "BinaryFile.h"
#include "DiskMultiMap.h"
#include "IntelWeb.h"
#include "MultiMapTuple.h"
#include "InteractionTuple.h"

using namespace std;

int main()
{
	DiskMultiMap x; 
	x.createNew("mylist.dat", 5); 
	x.openExisting("mylist.dat"); 
	x.insert("lol", "save", "haha");
	x.insert("b", "asdasdas", "z");
	x.insert("lol", "suck", "lasa");
	x.insert("lol", "suck", "lasa"); 
	x.insert("lol", "suck", "lasa");
	x.insert("lol", "suck", "lasa");
	cout << "erased : " << x.erase("lol", "suck", "lasa") << endl;
	cout << "erased : " << x.erase("b", "asdasdas", "z") << endl; 
	x.insert("lol", "suck", "lasa");
	x.insert("lol", "suck", "lasa");
	x.search("lol");
	x.close(); 
	system("pause"); 
}