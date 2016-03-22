// network.cpp
// Noah Betzen & Lonny Strunk
// CS 405

// Compile and run with:
// clang++ -Ofast --std=c++1y network.cpp -lcurl -o network.o && time ./network.o

// network protocol: http://skynet.cs.uaf.edu/doc/network_protocol/

#include <curl/curl.h> // requires libcurl4-gnutls-dev
#include <iostream>
using std::cout;
using std::endl;
#include <string>
using std::string;
#include <sstream>
using std::ostringstream;
#include "json.hpp" // https://github.com/nlohmann/json
using json = nlohmann::json;

int main()
{
    const string theURL = "http://skynet.cs.uaf.edu/?list_games=true";
    
    CURL *curl = curl_easy_init();
    
    if(curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, theURL.c_str());
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1);
    
        CURLcode res;
        res = curl_easy_perform(curl);
        
        if(res != CURLE_OK)
        {
            cout << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
        }
        
        ostringstream stream;
        stream << res;
        string output = stream.str();
        output.pop_back();
        
        json theJSON = output;
        
        cout << "The JSON:\n" << theJSON << endl;
        
        //curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "name=daniel&project=curl");
        
        curl_easy_cleanup(curl);
    }
    
    curl_global_cleanup();
    
    return 0;
}