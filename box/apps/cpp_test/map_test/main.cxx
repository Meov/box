#include <iostream>
#include <map>
using namespace std;

int main()
{
    // declaration of map container
    map<char, int> mymap;
    mymap['a'] = 1;
    mymap['b'] = 2;
#if 0
    mymap['c'] = 3;
#endif
    std::map<char, int>::iterator it;

    std::cout << "map size: "<<mymap.size() << std::endl;
    for (it = mymap.begin(); it != mymap.end();it++){
        cout << it->first << " = "<< it->second << '\n';
        mymap.erase(it++);
//or use like:
//		it = mymap.erase(it);
		cout << "map size update: "<<mymap.size() << std::endl;
    }

    mymap.clear();
    return 0;
}
