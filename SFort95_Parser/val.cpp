#include <string>
#include <iostream>
#include <map>
#include <string>
#include <cctype>
using namespace std;
#include "val.h"

Value Value::operator+(const Value& op) const{ // + 
    if(T == VERR || op.T == VERR){
        return Value();
    }
    else if(T == VINT && op.T == VINT ){
        return Value(Itemp + op.Itemp);
    }
    else if(T == VINT && op.T == VREAL ){
        return Value((double)Itemp + op.Rtemp);
    }
    else if(T == VREAL && op.T == VINT ){
        return Value((double)Rtemp + op.Itemp);
    }
    else if(T == VREAL && op.T == VREAL ){
        return Value((double)Rtemp + op.Rtemp);
    }
    return Value();
}

Value Value::operator-(const Value& op) const{ // -
    if(T == VERR || op.T == VERR){
        return Value();
    }
    else if(T == VINT && op.T == VINT ){
        return Value(Itemp - op.Itemp);
    }
    else if(T == VINT && op.T == VREAL ){
        return Value((double)Itemp - op.Rtemp);
    }
    else if(T == VREAL && op.T == VINT ){
        return Value((double)Rtemp - op.Itemp);
    }
    else if(T == VREAL && op.T == VREAL ){
        return Value((double)Rtemp - op.Rtemp);
    }
    return Value();
}

Value Value::operator/(const Value& op) const{ // /
    if(T == VERR || op.T == VERR){
        return Value();
    }
    else if(T == VINT && op.T == VINT ){
        if(op.Itemp == 0){
            return Value();
        }
        return Value(Itemp/op.Itemp);
    }
    else if(T == VINT && op.T == VREAL ){
        if(op.Rtemp == 0){
            return Value();
        }
        return Value(Itemp/op.Rtemp);
    }
    else if(T == VREAL && op.T == VINT ){
        if(op.Itemp == 0){
            return Value();
        }
        return Value(Rtemp/op.Itemp);
    }
    else if(T == VREAL && op.T == VREAL ){
        if(op.Rtemp == 0){
            return Value();
        }
        return Value(Rtemp/op.Rtemp);
    }
    return Value();
}

Value Value::operator*(const Value& op) const{ // *
    if(T == VERR || op.T == VERR){
        return Value();
    }
    else if(T == VINT && op.T == VINT ){
        return Value(Itemp * op.Itemp);
    }
    else if(T == VINT && op.T == VREAL ){
        return Value((double)Itemp * op.Rtemp);
    }
    else if(T == VREAL && op.T == VINT ){
        return Value((double)Rtemp * op.Itemp);
    }
    else if(T == VREAL && op.T == VREAL ){
        return Value((double)Rtemp * op.Rtemp);
    }
    return Value();
}
Value Value::operator==(const Value& op) const{
    if(T == VERR || op.T == VERR){
        return Value();
    }
    else if(T == VINT && op.T == VINT ){
        return Value(Itemp == op.Itemp);
    }
    else if(T == VINT && op.T == VREAL ){
        return Value(Itemp == op.Rtemp);
    }
    else if(T == VREAL && op.T == VINT ){
        return Value(Rtemp == op.Itemp);
    }
    else if(T == VREAL && op.T == VREAL ){
        return Value(Rtemp == op.Rtemp);
    }
    return Value();
}
Value Value::operator>(const Value& op) const{
    if(T == VERR || op.T == VERR){
        return Value();
    }
    else if(T == VINT && op.T == VINT ){
        return Value(Itemp > op.Itemp);
    }
    else if(T == VINT && op.T == VREAL ){
        return Value(Itemp > op.Rtemp);
    }
    else if(T == VREAL && op.T == VINT ){
        return Value(Rtemp > op.Itemp);
    }
    else if(T == VREAL && op.T == VREAL ){
        return Value(Rtemp > op.Rtemp);
    }
    return Value();
}
Value Value::operator<(const Value& op) const{
    if(T == VERR || op.T == VERR){
        return Value();
    }
    else if(T == VINT && op.T == VINT ){
        return Value(Itemp < op.Itemp);
    }
    else if(T == VINT && op.T == VREAL ){
        return Value(Itemp < op.Rtemp);
    }
    else if(T == VREAL && op.T == VINT ){
        return Value(Rtemp < op.Itemp);
    }
    else if(T == VREAL && op.T == VREAL ){
        return Value(Rtemp < op.Rtemp);
    }
    return Value();
}
Value Value::Catenate(const Value & op) const{
    if(T == VERR || op.T == VERR){
        return Value();
    }
    else if(T == VSTRING && op.T == VSTRING){
        string t = "";
        t = Stemp + op.Stemp;
        return Value(t);
    }
    return Value();
}
Value Value::Power(const Value & op) const{
    if(T == VERR || op.T == VERR){
        return Value();
    }
    else if(T == VINT && op.T == VINT ){
        return Value(pow(Itemp,op.Itemp));
    }
    else if(T == VINT && op.T == VREAL ){
        return Value(pow(Itemp,op.Rtemp));
    }
    else if(T == VREAL && op.T == VINT ){
        return Value(pow(Rtemp,op.Itemp));
    }
    else if(T == VREAL && op.T == VREAL ){
        return Value(pow(Rtemp,op.Rtemp));
    }
    return Value();
}
