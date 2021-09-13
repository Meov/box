#include <iostream> 
#include <map> 
using namespace std; 
  
int main() 
{ 
    // declaration of map container 
    map<char, int> mymap; 
    mymap['a'] = 1; 
    mymap['b'] = 2; 
    mymap['c'] = 3; 
  
    // using begin() to print map 
	std::map<char, int>::iterator it;

    for (it = mymap.begin(); it != mymap.end(); ++it) 
        cout << it->first << " = "
             << it->second << '\n'; 
    return 0; 
}
