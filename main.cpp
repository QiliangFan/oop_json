#include <iostream>
#include <iomanip>
#include<string>
#include <nlohmann/json.hpp>
#include"json_file.h"


using namespace std;

using json = nlohmann::json;

int main()
{

    json j = {1, 2};
    cout<<j.dump()<<endl;
    cout<<to_string(j)<<endl;
    int status_code;
    json_file jf("https://www.liangliang.fun/test.json", status_code);
    json_file jf2(j);
    jf2.write("liang.txt");

    json::parse()
    return 0;
}
