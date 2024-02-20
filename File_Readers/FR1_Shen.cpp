#include <iostream>
#include <fstream>
#include <string>
using namespace std;
int main(){
    string name;
    cout << "Enter the name of a file to read from: ";
    cin >> name;
    ifstream File;
    File.open(name);
    if(File.is_open()){
    string text;
    string Maxword = "";
    int maxW = 0;
    int line = 0; 
    int nonComment = 0; 
    string Maxline = "";
    int maxL = 0; 
    int numW = 0;
    while(getline (File,text)){
        line += 1; 
        if((text.substr(0,1).compare("#") != 0) && (text.substr(0,2).compare("//") != 0)){
            nonComment += 1;
            if(text.length() > maxL){
                maxL = text.length();
                Maxline = text;  
            }
            string word;
            for(int i = 0; i < text.length(); i++){
                if(!isspace(text[i])){
                    word += text[i];
                    if(i == text.length()-1){
                        numW += 1;
                        if(word.length() > maxW){
                            Maxword = word;
                            maxW = word.length();
                        }
                        word == "";
                    }
                }
                else{
                    if(word.length()>0){
                        numW += 1;
                        if(word.length() > maxW){
                            Maxword = word;
                            maxW = word.length();
                        }
                        word = ""; 
                    }
                }
            }
        }
    }
    cout << "Total Number of Lines: " << line << "\n";
    cout << "Number of non-commented lines: " << nonComment << "\n";
    cout << "Line of Maximum Length: \"" << Maxline << "\"\n";
    cout << "Number of Words: " << numW << "\n";
    cout << "Word of Maximum Length: \"" << Maxword << "\"\n";
    }
    else{
        cout << "File cannot be opened " << name << "\n";
    }
    File.close();
    return 0;
}