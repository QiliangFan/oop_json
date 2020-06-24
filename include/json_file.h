#ifndef JSON_FILE_H
#define JSON_FILE_H
#include<iostream>
#include<string>
#include<fstream>
#include<stack>
#include <nlohmann/json.hpp>

// for internet
#include<winsock2.h>
#include<windows.h>
#pragma comment(lib,"WS2_32.LIB")

using json = nlohmann::json;
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::ofstream;

class json_file
{
    public:
        json_file();
        virtual ~json_file();

        // read from local file object
        json_file(ifstream & file);

        // read from local file name
        json_file(string& filename);

        // read from url
        json_file(const string& url, int& status_code);

        // read from json object
        json_file(json& jo);

        void print();

        void write(string filename);

    protected:

    private:
        json *json_object = nullptr;
        string *json_string = nullptr;
};

#endif // JSON_FILE_H
