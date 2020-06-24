#include "json_file.h"

json_file::json_file()
{
    //ctor
}

json_file::json_file(ifstream & fileObject)
{
    fileObject>>*json_string;
    json tmp_json = json::parse(*json_string);
    json_object = new json(tmp_json.begin(), tmp_json.end());
}

json_file::json_file(string& filename)
{
    ifstream inf(filename);
    inf>>*json_string;
    json tmp_json = json::parse(*json_string);
    json_object = new json(tmp_json.begin(), tmp_json.end());
}

json_file::json_file(const string& url, int& status_code)
{
    string host_name;
    string get_url;
    int tag = 0;
    if(url.find("http://") == 0) get_url = url.substr(7);

    int start = 0;
    for(auto ch  : url){
        start++;
        if(tag == 2 && ch == '/'){
            tag = -1;
            break;
        }
        if(tag == 2){
            host_name += ch;
        }
        if(tag < 2 && ch == '/'){
            tag ++;
        }

    }
    if(tag != -1 && tag < 2){
        for(auto ch : url)
        {
            start++;
            if(ch == '/') break;
            host_name += ch;
        }
    }

//    cout<<host_name<<endl;

    WSADATA wsaData;
    SOCKET Socket;
    SOCKADDR_IN SocketAddr;

    string website_HTML;

    int lineCount = 0;
    int rowCount = 0;

    struct hostent *host;
    std::string get_http;

    get_http = "GET /" + url.substr(start) + " HTTP/1.1\r\nHost: " + host_name + "\r\nConnection: close\r\n\r\n";

//    cout<<get_http<<endl;

    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
    {
        cout << "WSAStartup failed.\n";
        system("pause");
        //return 1;
    }


    Socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    host = gethostbyname(host_name.c_str());

    SocketAddr.sin_port = htons(80);
    SocketAddr.sin_family = AF_INET;
    SocketAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);

    cout << "Connecting to "<< url<<" ...\n";

    if(connect(Socket, (SOCKADDR*)(&SocketAddr), sizeof(SocketAddr)) != 0)
    {
        cout<<"Could not  connect"<<endl;
        system("pause");
        status_code = 400;
    }

    cout<<"Connected."<<endl;

    send(Socket, get_http.c_str(), strlen(get_http.c_str()), 0);

    char buffer[10000];

    int nDataLength;

    while((nDataLength = recv(Socket,buffer,10000,0)) > 0)
    {
        int i = 0;

        while (buffer[i] >= 32 || buffer[i] == '\n' || buffer[i] == '\r')
        {
            website_HTML+=buffer[i];
            i += 1;
        }
        status_code = 200;
    }

    std::stack<char> check;
    string json_str(website_HTML);
    string my_json;
    int write = 0;
    for(auto ch : json_str){
        if(ch==' '||ch=='\r'||ch=='\n') continue;
        if(ch == '{' && check.size() == 0){
            write = 1;
            check.push('{');
        }else if(ch == '{'){
            check.push('}');
        }else if(ch == '}'){
            my_json += ch;
            write = 0;
            check.pop();
        }
        if(write) my_json += ch;
    }
    json tmp_json = json::parse(my_json);
    json_object = new json(tmp_json.begin(), tmp_json.end());
    closesocket(Socket);
    WSACleanup();
}

json_file::json_file(json& jo){
    json_object = &jo;
}

json_file::~json_file()
{
    //dtor
}

void json_file::print(){
    cout<<*json_object<<endl;
}

void json_file::write(string filename){
    ofstream of(filename);
    if(json_object){
        string s = json_object->to_string();
        const char* c_array = s.c_str();
        of<<c_array<<endl;
    }else{
        of<<*json_string<<endl;
    }
}
