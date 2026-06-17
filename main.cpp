#include <iostream>
#include <curl/curl.h>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

size_t WriteCallback(void* contents,size_t size,size_t nmemb,string* output){
    output->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main(){
    string username;
    cout << "Enter Username: ";
    getline(cin, username);

    string url =
    "https://api.github.com/users/" + username;

    cout << url << endl;

    CURL* curl = curl_easy_init();

    if(!curl){
        cout << "Failed to initialize CURL\n";
        return 1;
    }

    string response;

    curl_easy_setopt(curl,CURLOPT_URL,url.c_str());

    curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,WriteCallback);

    curl_easy_setopt(curl,CURLOPT_WRITEDATA,&response);

    curl_easy_setopt(curl,CURLOPT_USERAGENT,"GithubExplorer");

    CURLcode result = curl_easy_perform(curl);

    if(result != CURLE_OK){
    cout << curl_easy_strerror(result) << endl;
    }else{
        json data = json::parse(response);

        cout<<"Name: "<<data["name"]<<endl;
    }

    curl_easy_cleanup(curl);

    return 0;
}