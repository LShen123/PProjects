#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <cctype>
#include <ctype.h>
using namespace std;
int main(int argc, char* argv[]){
    if(argc <= 1){
        cout << "NO SPECIFIED INPUT FILE NAME.\n";
        return 0;
    }
    ifstream File;
    File.open(argv[1]);
    if(File.is_open()){
        string text;
        int line = 0;
        map<char, int> MapN;
        map<char, int> MapL;
        map<char, int> MapP;  
        bool num = false;
        bool letter = false;
        bool punct = false; 
        while(getline (File,text)){
            line += 1;
            for(int i = 0; i < text.length(); i++){
                if(!isspace(text[i])){
                    char ch = text[i];
                    if(isalpha(ch)){
                        if(MapL.find(toupper(ch)) == MapL.end()){
                            MapL[toupper(ch)] = 1;
                        }
                        else{
                            MapL[toupper(ch)] += 1;
                        }
                    }
                    else if(isdigit(ch)){
                        if(MapN.find(ch) == MapN.end()){
                            MapN[ch] = 1;
                        }
                        else{
                            MapN[ch] += 1; 
                        }
                    }
                    else if(ispunct(ch)){
                        if(MapP.find(ch) == MapP.end()){
                            MapP[ch] = 1;
                        }
                        else{
                            MapP[ch] += 1; 
                        }
                    }
                }
            }
        }
        if(line == 0){
            cout << "File is empty.\n";
        }
        else{
            bool L = false;
            bool D = false;
            bool P = false; 
            if(argc > 2){
                for(int i = 3; i <= argc; i ++){
                    if(argv[i-1] == "-all"sv){
                        L = true;
                        D = true;
                        P = true;
                    }
                    else if(argv[i-1] == "-L"sv){
                        L = true;
                    }
                    else if(argv[i-1] == "-D"sv){
                        D = true;
                    }
                    else if(argv[i-1] == "-P"sv){
                        P = true; 
                    }
                }
            }
            if(!MapL.empty()){
                char max;
                int maxNum = 0; 
                map<char,int>::iterator it;
                for(it = MapL.begin(); it != MapL.end(); it++){
                    if(it->second >= maxNum){
                        maxNum = it->second;
                        max = it->first; 
                    }
                }
                cout << "Letter of Largest number of occurrences: " << max << " occurred " << maxNum << " times\n";
            }
            if(!MapN.empty()){
                char max;
                int maxNum = 0; 
                map<char,int>::iterator it;
                for(it = MapN.begin(); it != MapN.end(); it++){
                    if(it->second >= maxNum){
                        maxNum = it->second;
                        max = it->first; 
                    }
                }
                cout << "Digits of Largest number of occurrences: " << max << " occurred " << maxNum << " times\n";
            }
            if(!MapP.empty()){
                char max;
                int maxNum = 0; 
                map<char,int>::iterator it;
                for(it = MapP.begin(); it != MapP.end(); it++){
                    if(it->second >= maxNum){
                        maxNum = it->second;
                        max = it->first; 
                    }
                }
                cout << "Punctuations of Largest number of occurrences: " << max << " occurred " << maxNum << " times\n";
            }
            if(L && !MapL.empty()){
                cout << "List of Letters seen in the file and their number of occurrences:\n";
                for(int i = 65; i < 91; i ++){
                    if(MapL.count(char(i))!= 0){
                        cout << char(i) << ": " << MapL[char(i)]<<"\n\n";
                    }
                }
            }
            if(D && !MapN.empty()){
                cout << "List of Digits seen in the file and their number of occurrences:\n";
                for(int i = 48; i < 58; i ++){
                    if(MapN.count(char(i)) != 0){
                        cout << char(i) << ": " << MapN[char(i)]<<"\n\n";
                    }
                }
            }
            if(P && !MapP.empty()){
                cout << "List of Punctuations seen in the file and their number of occurrences:\n";
                for(int i = 33; i < 127; i ++){
                    if(MapP.count(char(i)) != 0){
                        cout << char(i) << ": " << MapP[char(i)] << "\n\n";
                    }
                }
            }
        }
    }
    else{
        cout << "CANNOT OPEN THE FILE: " << argv[1] << "\n";
    }

}