#include <iostream>
#include <fstream>
#include <string>
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
    int nonBlank = 0; 
    int numW = 0;
    int integ = 0;
    int names = 0; 
    int wordless = 0;
    while(getline (File,text)){
        line += 1; 
            if(text.length() > 0){
            nonBlank += 1;
            int count = 0;
            string word;
            for(int i = 0; i < text.length(); i++){
                if(!isspace(text[i])){
                    word += text[i];
                    count += 1;
                    if(i == text.length()-1){
                        numW += 1;
                        bool num = true;
                        bool nme = false;
                        if(isalpha(word[0])){
                            nme = true; 
                        }
                        for(char c : word){
                            if(!isdigit(c)){
                                num = false;
                            }
                            if((nme)){
                                if(!((isdigit(c))||(isalpha(c))||(c == '_')||(c == '@'))){
                                    nme = false;
                                }
                            }
                        }
                        if(num){
                            integ += 1; 
                        }
                        else if(nme){
                            names += 1;
                        }
                        if(word.length()<=5){
                            wordless += 1;
                        }
                        word = ""; 
                    }
                }
                else{
                    if(word.length()>0){
                        numW += 1;
                        bool num1 = true;
                        bool nme1 = false;
                        if(isalpha(word[0])){
                            nme1 = true; 
                        }
                        for(char c : word){
                            if(!isdigit(c)){
                                num1 = false;
                            }
                            if((nme1)){
                                if(!((isdigit(c))||(isalpha(c))||(c == '_')||(c == '@'))){
                                    nme1 = false;
                                }
                            }
                        }
                        if(num1){
                            integ += 1; 
                        }
                        else if(nme1){
                            names += 1;
                        }
                        if(word.length()<=5){
                            wordless += 1;
                        }
                        word = ""; 
                    }
                }
            }
    }
    }
    if(line == 0){
        cout << "File is empty.\n";
    }
    else{
    cout << "Total Number of Lines: " << line << "\n";
    cout << "Number of non-blank lines: " << nonBlank << "\n";
    cout << "Number of Words: " << numW << "\n";
    cout << "Number of Integers: " << integ << "\n";
    cout << "Number of Names: " << names << "\n";
    cout << "Number of Words Less Than or Equal to 5 Characters: " << wordless << "\n";
    cout << "Number of Words Greater Than 5 Characters: " << (numW - wordless) << "\n";
    }
    }
    else{
        cout << "CANNOT OPEN THE FILE: " << argv[1] << "\n";
    }
    File.close();
    return 0;
}