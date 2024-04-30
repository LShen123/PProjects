//Implementation of Recursive-Descent Parser
//for the SFort95 Language
#include <stack>

#include "parserInterp.h"

map<string, bool> defVar;
map<string, Token> SymTable;

map<string, Value> TempsResults; //Container of temporary locations of Value objects for results of expressions, variables values and constants 
queue <Value> * ValQue; //declare a pointer variable to a queue of Value objects

LexItem tempType;
bool work = true;
int CurrentLine;
namespace Parser {
	bool pushed_back = false;
	LexItem	pushed_token;

	static LexItem GetNextToken(istream& in, int& line) {
		if( pushed_back ) {
			pushed_back = false;
			return pushed_token;
		}
		return getNextToken(in, line);
	}

	static void PushBackToken(LexItem & t) {
		if( pushed_back ) {
			abort();
		}
		pushed_back = true;
		pushed_token = t;	
	}

}

static int error_count = 0;

int ErrCount()
{
    return error_count;
}

void ParseError(int line, string msg)
{
	++error_count;
	cout << line << ": " << msg << endl;
}

bool IdentList(istream& in, int& line);


//Program is: Prog = PROGRAM IDENT {Decl} {Stmt} END PROGRAM IDENT
bool Prog(istream& in, int& line)
{
	CurrentLine = line;
	bool dl = false, sl = false;
	LexItem tok = Parser::GetNextToken(in, line);
		
	if (tok.GetToken() == PROGRAM) {
		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == IDENT) {
			dl = Decl(in, line);
			if( !dl  )
			{
				ParseError(CurrentLine, "Incorrect Declaration in Program");
				return false;
			}
			sl = Stmt(in, line);
			if( !sl  )
			{
				ParseError(CurrentLine, "Incorrect Statement in program");
				return false;
			}	
			tok = Parser::GetNextToken(in, line);
			
			if (tok.GetToken() == END) {
				tok = Parser::GetNextToken(in, line);
				
				if (tok.GetToken() == PROGRAM) {
					tok = Parser::GetNextToken(in, line);
					
					if (tok.GetToken() == IDENT) {
						cout << "(DONE)" << endl;
						return true;
					}
					else
					{
						ParseError(CurrentLine, "Missing Program Name");
						return false;
					}	
				}
				else
				{
					ParseError(CurrentLine, "Missing PROGRAM at the End");
					return false;
				}	
			}
			else
			{
				ParseError(CurrentLine, "Missing END of Program");
				return false;
			}	
		}
		else
		{
			ParseError(CurrentLine, "Missing Program name");
			return false;
		}
	}
	else if(tok.GetToken() == ERR){
		ParseError(CurrentLine, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	
	else
	{
		ParseError(CurrentLine, "Missing Program keyword");
		return false;
	}
}

//Decl ::= Type :: VarList 
//Type ::= INTEGER | REAL | CHARARACTER [(LEN = ICONST)] 
bool Decl(istream& in, int& line) {
	
	bool status = false;
	LexItem tok;
	string strLen;
	
	LexItem t = Parser::GetNextToken(in, line);
	CurrentLine = line;
	if(t == INTEGER || t == REAL || t == CHARACTER ) {
		tok = t;
		tempType = tok;
		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == DCOLON) {
			status = VarList(in, line,t);
			
			if (status)
			{
				status = Decl(in, line);
				if(!status)
				{
					ParseError(CurrentLine, "Declaration Syntactic Error.");
					return false;
				}
				return status;
			}
			else
			{
				ParseError(CurrentLine, "Missing Variables List.");
				return false;
			}
		}
		else if(t == CHARACTER && tok.GetToken() == LPAREN)
		{
			tok = Parser::GetNextToken(in, line);
			
			if(tok.GetToken() == LEN)
			{
				tok = Parser::GetNextToken(in, line);
				
				if(tok.GetToken() == ASSOP)
				{
					tok = Parser::GetNextToken(in, line);
					
					if(tok.GetToken() == ICONST)
					{ 
						strLen = tok.GetLexeme();
						int num = stoi(strLen);
						tok = Parser::GetNextToken(in, line);
						if(tok.GetToken() == RPAREN)
						{
							tok = Parser::GetNextToken(in, line);
							if(tok.GetToken() == DCOLON)
							{
								status = VarList(in, line, t, num);
								if (status)
								{
									//cout << "Definition of Strings with length of " << strLen << " in declaration statement." << endl;
									status = Decl(in, line);
									if(!status)
									{
										ParseError(CurrentLine, "Declaration Syntactic Error.");
										return false;
									}
									return status;
								}
								else
								{
									ParseError(CurrentLine, "Missing Variables List.");
									return false;
								}
							}
						}
						else
						{
							ParseError(CurrentLine, "Missing Right Parenthesis for String Length definition.");
							return false;
						}
					
					}
					else
					{
						ParseError(CurrentLine, "Incorrect Initialization of a String Length");
						return false;
					}
				}
			}
		}
		else
		{
			ParseError(CurrentLine, "Missing Double Colons");
			return false;
		}
			
	}
		
	Parser::PushBackToken(t);
	return true;
}//End of Decl

//Stmt ::= AssigStmt | BlockIfStmt | PrintStmt | SimpleIfStmt
bool Stmt(istream& in, int& line) {
	CurrentLine = line;
	bool status;
	
	LexItem t = Parser::GetNextToken(in, line);
	
	switch( t.GetToken() ) {

	case PRINT:
		status = PrintStmt(in, line);
		
		if(status)
			status = Stmt(in, line);
		break;

	case IF:
		status = BlockIfStmt(in, line);
		if(status)
			status = Stmt(in, line);
		break;

	case IDENT:
		Parser::PushBackToken(t);
        status = AssignStmt(in, line);
		if(status)
			status = Stmt(in, line);
		break;
		
	
	default:
		Parser::PushBackToken(t);
		return true;
	}

	return status;
}//End of Stmt

bool SimpleStmt(istream& in, int& line) {
	CurrentLine = line;
	bool status;
	
	LexItem t = Parser::GetNextToken(in, line);
	
	switch( t.GetToken() ) {

	case PRINT:
		status = PrintStmt(in, line);
		
		if(!status)
		{
			ParseError(CurrentLine, "Incorrect Print Statement");
			return false;
		}	
		//cout << "Print statement in a Simple If statement." << endl;
		break;

	case IDENT:
		Parser::PushBackToken(t);
        status = AssignStmt(in, line);
		if(!status)
		{
			ParseError(CurrentLine, "Incorrect Assignent Statement");
			return false;
		}
		//cout << "Assignment statement in a Simple If statement." << endl;
			
		break;
		
	
	default:
		Parser::PushBackToken(t);
		return true;
	}

	return status;
}//End of SimpleStmt

//VarList ::= Var [= Expr] {, Var [= Expr]} CHANGED
bool VarList(istream& in, int& line, LexItem & idtok, int strlen) {
	bool status = false, exprstatus = false;
	string identstr;
	
	
	LexItem tok = Parser::GetNextToken(in, line);
	CurrentLine = line;
	if(tok == IDENT)
	{
		
		identstr = tok.GetLexeme();
		if (!(defVar.find(identstr)->second))
		{
			defVar[identstr] = true;
			SymTable[identstr] = idtok.GetToken();
			if(SymTable[identstr] == CHARACTER){
				string s;
				for(int i = s.length(); i < strlen; i ++){
					s += " ";
				}
				TempsResults[identstr] = Value(s);
			}
			defVar[identstr] = true;
			SymTable[identstr] = idtok.GetToken();
		}	
		else
		{
			ParseError(CurrentLine, "Variable Redefinition");
			return false;
		}
		
	}
	else
	{
		ParseError(CurrentLine, "Missing Variable Name");
		return false;
	}
		
	tok = Parser::GetNextToken(in, line);
	if(tok == ASSOP)
	{
		Value retVal;
		exprstatus = Expr(in, line, retVal);
		if(!exprstatus)
		{
			ParseError(CurrentLine, "Incorrect initialization for a variable.");
			return false;
		}
		if(SymTable[identstr] == CHARACTER){
			string s = retVal.GetString();
			if(s.length()>=strlen){
				retVal.SetString(s.substr(0,strlen));
			}
			else{
				for(int i = s.length(); i < strlen; i ++){
					s += " ";
				}
				retVal.SetString(s);
			}
		}
		if(idtok.GetToken() == REAL){
			retVal = Value(0.0) + retVal;
		}
		TempsResults[identstr] = retVal;
		//cout<< "Initialization of the variable " << identstr <<" in the declaration statement." << endl;
		tok = Parser::GetNextToken(in, line);
		
		if (tok == COMMA) {
			
			status = VarList(in, line, idtok, strlen);
			
		}
		else
		{
			Parser::PushBackToken(tok);
			return true;
		}
	}
	else if (tok == COMMA) {
		
		status = VarList(in, line, idtok, strlen);
	}
	else if(tok == ERR)
	{
		ParseError(CurrentLine, "Unrecognized Input Pattern");
		
		return false;
	}
	else {
		
		Parser::PushBackToken(tok);
		return true;
	}
	
	return status;
	
}//End of VarList
	


//PrintStmt:= PRINT *, ExpreList  CHANGED
bool PrintStmt(istream& in, int& line) {
	CurrentLine = line;
	LexItem t;
	ValQue = new queue<Value>;
	
	
	t = Parser::GetNextToken(in, line);
	
 	if( t != DEF ) {
		
		ParseError(CurrentLine, "Print statement syntax error.");
		return false;
	}
	t = Parser::GetNextToken(in, line);
	
	if( t != COMMA ) {
		
		ParseError(CurrentLine, "Missing Comma.");
		return false;
	}
	bool ex = ExprList(in, line);
	
	if( !ex ) {
		ParseError(CurrentLine, "Missing expression after Print Statement");
		return false;
	}
	
	while (!(*ValQue).empty())
	{
		Value nextVal = (*ValQue).front();
		if(work){
			cout << nextVal;
		}
		ValQue->pop();
	}
	if(work){
		cout << endl;
	}
	return ex;
}//End of PrintStmt

//BlockIfStmt:= if (RelExpr) then {Stmt} [Else Stmt]
//SimpleIfStatement := if (RelExpr) SimpleStmt
bool BlockIfStmt(istream& in, int& line) {
	CurrentLine = line;
	bool ex=false, status ; 
	static int nestlevel = 0;
	//int level;
	LexItem t;
	
	t = Parser::GetNextToken(in, line);
	if( t != LPAREN ) {
		
		ParseError(CurrentLine, "Missing Left Parenthesis");
		return false;
	}
	Value retValB; 
	ex = RelExpr(in, line, retValB);
	work = retValB.GetBool();
	if( !ex ) {
		ParseError(CurrentLine, "Missing if statement condition");
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t != RPAREN ) {
		
		ParseError(CurrentLine, "Missing Right Parenthesis");
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t != THEN)
	{
		Parser::PushBackToken(t);
		
		status = SimpleStmt(in, line);
		if(status)
		{
			return true;
		}
		else
		{
			ParseError(CurrentLine, "If-Stmt Syntax Error");
			return false;
		}
		
	}
	nestlevel++;
	//level = nestlevel;
	status = Stmt(in, line);
	if(!status)
	{
		ParseError(CurrentLine, "Missing Statement for If-Stmt Then-Part");
		return false;
	}
	t = Parser::GetNextToken(in, line);
	if( t == ELSE ) {
		work = !work;
		status = Stmt(in, line);
		work = true;
		if(!status)
		{
			ParseError(CurrentLine, "Missing Statement for If-Stmt Else-Part");
			return false;
		}
		else
		  t = Parser::GetNextToken(in, line);
		
	}
	
	
	if(t != END ) {
		
		ParseError(CurrentLine, "Missing END of IF");
		return false;
	}
	t = Parser::GetNextToken(in, line);
	if(t == IF ) {
		//cout << "End of Block If statement at nesting level " << level << endl;
		return true;
	}	
	Parser::PushBackToken(t);
	ParseError(CurrentLine, "Missing IF at End of IF statement");
	return false;
}//End of IfStmt function

//Var:= ident // CHANGED
bool Var(istream& in, int& line, LexItem & idtok)
{
	CurrentLine = line;
	string identstr;
	
	LexItem tok = Parser::GetNextToken(in, line);
	if (tok == IDENT){
		identstr = tok.GetLexeme();
		idtok = tok;
		if (!(defVar.find(identstr)->second))
		{
			ParseError(CurrentLine, "Undeclared Variable");
			return false;
		}	
		return true;
	}
	else if(tok.GetToken() == ERR){
		ParseError(CurrentLine, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	return false;
}//End of Var

//AssignStmt:= Var = Expr // CHANGED
bool AssignStmt(istream& in, int& line) {
	CurrentLine = line;
	bool varstatus = false, status = false;
	LexItem t;
	LexItem CurrentVar;
	varstatus = Var(in, line, CurrentVar);
	if (varstatus){
		t = Parser::GetNextToken(in, line);
		
		if (t == ASSOP){
			Value retValA;
			status = Expr(in, line, retValA);
			if(!status) {
				ParseError(CurrentLine, "Missing Expression in Assignment Statment");
				return status;
			}
			if(work){
				if(SymTable[CurrentVar.GetLexeme()] == CHARACTER){
					if(!retValA.IsString()){
						ParseError(CurrentLine, "Illegal mixed-mode assignment operation");
						return false; 
					}
					string s = retValA.GetString();
					int strlens = TempsResults[CurrentVar.GetLexeme()].GetString().length();
					if(s.length()>=strlens){
						retValA.SetString(s.substr(0,strlens));
					}
					else{
						for(int i = s.length(); i < strlens; i ++){
							s += " ";
						}
						retValA.SetString(s);
					}
				}
				TempsResults[CurrentVar.GetLexeme()] = retValA;
			}
			
		}
		else if(t.GetToken() == ERR){
			ParseError(CurrentLine, "Unrecognized Input Pattern");
			cout << "(" << t.GetLexeme() << ")" << endl;
			return false;
		}
		else {
			ParseError(CurrentLine, "Missing Assignment Operator");
			return false;
		}
	}
	else {
		ParseError(CurrentLine, "Missing Left-Hand Side Variable in Assignment statement");
		return false;
	}
	return status;	
}//End of AssignStmt

//ExprList:= Expr {,Expr} CHANGED
bool ExprList(istream& in, int& line) {
	CurrentLine = line;
	bool status = false;
	Value retVal;
	
	status = Expr(in, line, retVal);
	if(!status){
		ParseError(CurrentLine, "Missing Expression");
		return false;
	}
	ValQue->push(retVal);
	LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok == COMMA) {
		
		status = ExprList(in, line);
		
	}
	else if(tok.GetToken() == ERR){
		ParseError(CurrentLine, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else{
		Parser::PushBackToken(tok);
		return true;
	}
	return status;
}//End of ExprList

//RelExpr ::= Expr  [ ( == | < | > ) Expr ]
bool RelExpr(istream& in, int& line, Value & retVal) {
	CurrentLine = line;
	retVal = Value(true);
	Value retValE;
	bool t1 = Expr(in, line,retValE);
	LexItem tok;
	if( !t1 ) {
		return false;
	}
	
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(CurrentLine, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	if ( tok == EQ || tok == LTHAN || tok == GTHAN) 
	{
		Value retValR;
		t1 = Expr(in, line,retValR);
		if(tok == EQ){
			if((retValE.IsString() && retValR.IsString()) || (!retValE.IsString() && !retValR.IsString()))
				retVal = retValE == retValR;
			else{
				ParseError(CurrentLine, "Illegal operand types for a Relational operation.");
				return false;
			}
		}
		else if(tok == LTHAN){
			if(!retValE.IsString() && !retValR.IsString())
				retVal = retValE < retValR;
			else{
				ParseError(CurrentLine, "Illegal operand types for a Relational operation.");
				return false;
			}
		}
		else if(tok == GTHAN){
			if(!retValE.IsString() && !retValR.IsString())
				retVal = retValE > retValR;
			else{
				ParseError(CurrentLine, "Illegal operand types for a Relational operation.");
				return false;
			}
		}
		if( !t1 ) 
		{
			ParseError(CurrentLine, "Missing operand after operator");
			return false;
		}
		
	}
	return true;
}//End of RelExpr

//Expr ::= MultExpr { ( + | - | // ) MultExpr } // CHANGED
bool Expr(istream& in, int& line, Value & retVal) {
	CurrentLine = line;
	bool t1 = MultExpr(in, line, retVal);
	LexItem tok;
	if( !t1 ) {
		return false;
	}
	
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(CurrentLine, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while ( tok == PLUS || tok == MINUS || tok == CAT) 
	{
		if(retVal.IsString() && (tok == PLUS || tok == MINUS)){
			ParseError(CurrentLine, "Illegal operand type for the operation.");
			return false;
		}
		Value retValM;
		t1 = MultExpr(in, line, retValM);
		if(retValM.IsString() && (tok == PLUS || tok == MINUS)){
			ParseError(CurrentLine, "Illegal operand type for the operation.");
			return false;
		}
		if( !t1 ) 
		{
			ParseError(CurrentLine, "Missing operand after operator");
			return false;
		}
		if(tok == PLUS){
			retVal = retVal + retValM;
		}
		else if(tok == MINUS){
			retVal = retVal - retValM;
		}
		else if(tok == CAT){
			retVal = retVal.Catenate(retValM);
		}
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(CurrentLine, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
		
	}
	Parser::PushBackToken(tok);
	return true;
}//End of Expr

//MultExpr ::= TermExpr { ( * | / ) TermExpr } // CHANGED
bool MultExpr(istream& in, int& line, Value & retVal) {
	CurrentLine = line;
	bool t1 = TermExpr(in, line,retVal);
	LexItem tok;
	
	if( !t1 ) {
		return false;
	}
	
	tok	= Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
			ParseError(CurrentLine, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
	}
	while ( tok == MULT || tok == DIV  )
	{
		Value retValT;
		t1 = TermExpr(in, line, retValT);
		
		if( !t1 ) {
			ParseError(CurrentLine, "Missing operand after operator");
			return false;
		}
		if(tok == MULT){
			retVal = retVal * retValT;
		}
		else if(tok == DIV){
			if((retValT == Value(0)).GetBool() || (retValT == Value(0.0)).GetBool()){
				ParseError(CurrentLine, "Run-Time Error-Illegal division by Zero");
				return false;
			}
			retVal = retVal/retValT;
		}
		tok	= Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(CurrentLine, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
		
	}
	Parser::PushBackToken(tok);
	return true;
}//End of MultExpr

//TermExpr ::= SFactor { ** SFactor } // CHANGED
bool TermExpr(istream& in, int& line, Value & retVal) {
	CurrentLine = line;
	bool t1 = SFactor(in, line, retVal);
	LexItem tok;

	if( !t1 ) {
		return false;
	}
	
	tok	= Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
			ParseError(CurrentLine, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
	}
	stack<Value> ValS;
	bool POWS = false;
	while ( tok == POW  )
	{	
		POWS = true;
		Value retValT;
		t1 = SFactor(in, line, retValT);
		
		if( !t1 ) {
			ParseError(CurrentLine, "Missing exponent operand");
			return false;
		}
		
		tok	= Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(CurrentLine, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
			ValS.push(retValT);
	}
	Value tempVal(1.00);
	if(!ValS.empty()){
		tempVal = ValS.top();
		ValS.pop();
	}
	while(!ValS.empty()){
		tempVal = ValS.top().Power(tempVal);
		ValS.pop();
	}
	if(POWS){
		retVal = retVal.Power(tempVal);
	}
	Parser::PushBackToken(tok);
	return true;
}//End of TermExpr

//SFactor = Sign Factor | Factor //CHANGED
bool SFactor(istream& in, int& line, Value & retVal)
{
	CurrentLine = line;
	LexItem t = Parser::GetNextToken(in, line);
	
	bool status;
	int sign = 0;
	if(t == MINUS )
	{
		sign = -1;
	}
	else if(t == PLUS)
	{
		sign = 1;
	}
	else
		Parser::PushBackToken(t);
		
	status = Factor(in, line, sign, retVal);
	return status;
}//End of SFactor

//Factor := ident | iconst | rconst | sconst | (Expr)
bool Factor(istream& in, int& line, int sign, Value & retVal) { //CHANGED
	LexItem tok = Parser::GetNextToken(in, line);
	CurrentLine = line;
	//cout << tok.GetLexeme() << endl;
	if( tok == IDENT ) {
		
		string lexeme = tok.GetLexeme();
		if (!(defVar.find(lexeme)->second))
		{
			ParseError(CurrentLine, "Using Undefined Variable");
			return false;	
		}
		if(work && TempsResults[lexeme].IsErr()){
			ParseError(CurrentLine, "Using Undefined Variable");
			return false;
		}
		if(sign == -1){
			if(SymTable[lexeme] == CHARACTER){
				ParseError(CurrentLine, "Illegal Operand Type for Sign Operator");
				return false;
			}
			retVal = Value(Value(-1)*TempsResults[lexeme]);
		}
		else{
			retVal = TempsResults[lexeme];
		}
		return true;
	}
	else if( tok == ICONST ) {
		retVal = Value(stoi(tok.GetLexeme()));
		return true;
	}
	else if( tok == SCONST ) {
		retVal = Value(tok.GetLexeme());
		return true;
	}
	else if( tok == RCONST ) {
		retVal = Value(stod(tok.GetLexeme()));
		return true;
	}
	else if( tok == LPAREN ) {
		Value retValF;
		bool ex = Expr(in, line, retValF);
		if( !ex ) {
			ParseError(CurrentLine, "Missing expression after (");
			return false;
		}
		if( Parser::GetNextToken(in, line) == RPAREN ){
			retVal = retValF;
			return ex;
		}
		else 
		{
			Parser::PushBackToken(tok);
			ParseError(CurrentLine, "Missing ) after expression");
			return false;
		}
	}
	else if(tok.GetToken() == ERR){
		ParseError(CurrentLine, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}

	
	return false;
}



