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
    cout<<"\nEnter Username: ";
    getline(cin, username);

    string url="https://api.github.com/users/" + username;

    cout<<url<<endl;

    CURL* curl = curl_easy_init();

    if(!curl){
        cout<<"Failed to initialize CURL\n";
        return 1;
    }

    string response;

    curl_easy_setopt(curl,CURLOPT_URL,url.c_str());

    curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,WriteCallback);

    curl_easy_setopt(curl,CURLOPT_WRITEDATA,&response);

    curl_easy_setopt(curl,CURLOPT_USERAGENT,"GithubExplorer");

    CURLcode result = curl_easy_perform(curl);

    if(result != CURLE_OK){
    cout<<"\nNetwork Error: "<<curl_easy_strerror(result)<<endl;
    return 1;
    }else{
        json data = json::parse(response);

        if(data.contains("message") && data["message"] == "Not Found"){
            cout<<"\nUser not found!\n";
            return 1;
        }
        cout<<"\n===== USER INFO =====\n";

        cout<<"Name: "<< data["name"].get<string>()<<endl;

        cout<<"Username: "<<data["login"].get<string>()<<endl;

        cout<<"Followers: "<<data["followers"].get<int>()<<endl;

        cout<<"Following: "<<data["following"].get<int>()<<endl;

        cout<<"Public Repositories: "<<data["public_repos"].get<int>()<<endl;

        if(data["location"].is_null()){
            cout<<"Location: N/A\n";
        } else {
            cout<<"Location: "<<data["location"].get<string>()<<endl;
        }

        cout<<"Profile URL: "<<data["html_url"].get<string>()<<endl;
    }

    curl_easy_cleanup(curl);

    return 0;
}