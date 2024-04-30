#include <string>
#include <iostream>
#include <map>
#include <string>
#include <cctype>
using std::map;
using namespace std;
#include "lex1.h"

LexItem id_or_kw(const string& lexeme, int linenum){
    static map<string,Token> kWs = {{"IF",IF}, {"ELSE",ELSE}, {"PRINT",PRINT}, {"INTEGER",INTEGER},{"REAL",REAL},
{"CHARACTER",CHARACTER},{"END",END},{"THEN",THEN},{"PROGRAM",PROGRAM},{"LEN",LEN} };
    Token tk = IDENT;
    string str = "";
    for(int i = 0; i < lexeme.length(); i ++){
        str += toupper(lexeme[i]);
    }
    if(kWs.find(str) != kWs.end()){
        tk = kWs.find(str)->second;
    }
    return LexItem(tk, lexeme, linenum);
    
}

LexItem getNextToken(istream& in, int& linenum){
char prev;
enum TokState {START, INID, ININT, INSTRING, INREAL,
                INCOMMENT,
                IDENT,} lexstate = START;
string lexeme;
char ch;
while(in.get(ch)){

    if( ch == '\n'){
        linenum += 1;
    }
    char next = in.peek();
    switch(lexstate){
        case START://Start of Call
            if(ch == '!'){
                lexstate = INCOMMENT;
            }
            else if((ch != ' ') && (ch != '\t') && (ch != '\n') && (ch != '\r') && (ch != '\f')){
                lexeme += ch;
                if(isalpha(ch)){
                    lexstate = INID;
                    if((next!='_') && (!isdigit(next)) && (!isalpha(next))){
                        return id_or_kw(lexeme,linenum);
                    }
                }
                else if(isdigit(ch)){
                    lexstate = ININT;
                    if((next != '.') && (!isdigit(next))){
                        return LexItem((Token)ICONST,lexeme,linenum);
                    }
                }
                else{
                    switch(ch){//SYMBOLS
                        case '!': //COMMENT
                            lexstate = INCOMMENT;
                        break;
                        case '+': //PLUS
                            return LexItem((Token)PLUS,lexeme,linenum);
                        break;
                        case '-': //MINUS
                            return LexItem((Token)MINUS,lexeme,linenum);
                        break;
                        case '*': //POW OR MULT OR DEF
                            if(next == '*'){
                                in.get(ch);
                                lexeme += ch;
                                return LexItem((Token)POW,lexeme,linenum);
                            } 
                            else if(next == ','){
                                return  LexItem((Token)DEF,lexeme,linenum);
                            }
                            else{
                                return LexItem((Token)MULT,lexeme,linenum);
                            }
                        break;
                        case '/': //CAT OR DIV
                            if(next == '/'){
                                in.get(ch);
                                lexeme += ch;
                                return LexItem((Token)CAT,lexeme,linenum);
                            }
                            else{
                                return LexItem((Token)DIV,lexeme,linenum);
                            }
                        break;
                        case '=': //EQ OR ASSOP
                            if(next == '='){
                                in.get(ch);
                                lexeme += ch;
                                return LexItem((Token)EQ,lexeme,linenum);
                            }
                            else{
                                return LexItem((Token)ASSOP,lexeme,linenum);
                            }
                        break;
                        case '<': //LTHAN
                            return LexItem((Token)LTHAN,lexeme,linenum);
                        break;
                        case '>': //GTHAN
                            return LexItem((Token)GTHAN, lexeme, linenum);
                        break;
                        case ','://COMMA
                            return LexItem((Token)COMMA,lexeme,linenum);
                        break;
                        case '(': //LPAREN
                            return LexItem((Token)LPAREN,lexeme,linenum);
                        break;
                        case ')': //RPAREN
                            return LexItem((Token)RPAREN,lexeme,linenum);
                        break;
                        case ':'://DCOLON
                            if(next == ':'){
                                in.get(ch);
                                lexeme += ch;
                                return LexItem((Token)DCOLON,lexeme,linenum); 
                            }
                            else{
                                return LexItem((Token)ERR,lexeme,linenum);
                            }
                        break;
                        case '.'://DOT and real num
                            if(isdigit(next)){
                                lexstate = INREAL;
                            }
                            else{
                                return LexItem((Token)DOT,lexeme,linenum);
                            }
                        break;
                        case '\'': //STRING?
                            lexstate = INSTRING;
                            prev = '\'';
                        break;
                        case '\"': //STRING?
                            lexstate = INSTRING;
                            prev = '\"';
                        break;
                        default://ERROR
                            return LexItem((Token)ERR,lexeme,linenum);
                        continue;
                    }   
                }
            }
        break;
        case INID://IDENT
            if((ch == '_')||(isdigit(ch)||(isalpha(ch)))){
                lexeme += ch;
            }
            if(next != '_' && !isdigit(next) && !(isalpha(next))){
                    return id_or_kw(lexeme,linenum);
            }

        break;
        case ININT://ICONST
            lexeme += ch;
            if(ch == '.'){
                lexstate = INREAL;
                if(!isdigit(next)){
                    return LexItem((Token)ERR,lexeme,linenum);//MIGHT NEED TO CHECK LATER
                }
            }
            else{
                if((next!='.')&&(!isdigit(next))){
                    return LexItem((Token)ICONST,lexeme,linenum);
                }
            }
        break;
        case INREAL://RCONST
            lexeme += ch;
            if(ch == '.'){
                return LexItem((Token)ERR,lexeme,linenum);
            }
            if(!isdigit(next)){
                if(next != '.'){
                    return LexItem((Token)RCONST,lexeme,linenum);
                }
            }
        break;
        case INSTRING://SCONST
            if(ch == '\n'){
                return LexItem((Token)ERR,lexeme,linenum);
            }
            lexeme += ch;
            if((ch == '\'') || (ch == '\"')){
                if(ch == prev){
                    return LexItem((Token)SCONST,lexeme,linenum);
                }
                else{
                    return LexItem((Token)ERR,lexeme,linenum);
                }
            }
        break;
        case INCOMMENT://INCOMMENT
            if(ch == '\n'){
                lexstate = START;
            }
        break;
        default:
            return LexItem((Token)ERR,lexeme,linenum);
        continue;
    }
}
if(in.eof()){
    return LexItem((Token)DONE,lexeme,linenum);
}
return LexItem((Token)ERR,"SOMETHINGUP",1);


}

ostream& operator<<(ostream& out, const LexItem& tok){
    map<Token,string> tks = {{IF,"IF"}, {ELSE,"ELSE"}, {PRINT,"PRINT"}, {INTEGER,"INTEGER"},{REAL,"REAL"},
{CHARACTER,"CHARACTER"},{END,"END"},{THEN,"THEN"},{PROGRAM,"PROGRAM"},{LEN,"LEN"}, {IDENT,"IDENT"}, {ICONST,"ICONST"}, {RCONST,"RCONST"}, 
{SCONST,"SCONST"}, {BCONST,"BCONST"}, {PLUS,"PLUS"}, {MINUS,"MINUS"}, {MULT,"MULT"}, {DIV,"DIV"}, {ASSOP,"ASSOP"}, {EQ,"EQ"}, {POW,"POW"}, {GTHAN,"GTHAN"}, {LTHAN,"LTHAN"}, {CAT,"CAT"}, 
{COMMA,"COMMA"}, {LPAREN,"LPAREN"}, {RPAREN,"RPAREN"}, {DOT,"DOT"}, {DCOLON,"DCOLON"}, {DEF,"DEF"}, {ERR,"ERR"}, {DONE,"DONE"}};
    Token tt = tok.GetToken();
	string s = tks.find(tt)->second;
	if(tt == ICONST || tt == RCONST || tt == BCONST) {
		out << s << ": (" << tok.GetLexeme() << ")" << endl;;
	}
    else if(tt == IDENT){
        out << s << ": \'" << tok.GetLexeme() << "\'" << endl; 
    }
    else if(tt == SCONST){
        out << s << ": \"" << tok.GetLexeme().substr(1,(tok.GetLexeme().length()-2)) << "\"" << endl;
    }
    else if(tt == ERR){
        out << "Error in line " << tok.GetLinenum() << ": Unrecognized Lexeme {" << tok.GetLexeme() << "}" << endl;
    }
    else if(tt == DONE){
        out << "";
    }
    else{
        out << s << endl;
    }
	return out;
}