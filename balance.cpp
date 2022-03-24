#include <iostream>
#include <fstream>
#include <istream>
#include <string>
#include <stack>
using namespace std;

void printFile(istream &dictfile) {
    int num = 1;
    dictfile.clear();
    dictfile.seekg(0, dictfile.beg);
    string str;
    while (getline(dictfile, str)){
        cout<<num<<" : "<<str<<endl;
        num++;
    }
}

bool isOpeningBrace(char c){
    if (c == '(' || c == '{' || c == '[')
        return true;
    return false;
}
bool isClosingBrace(char c){
    if (c == ')' || c == '}' || c == ']')
        return true;
    return false;
}

bool isBalancedBrace(char c, char d){
    if (c == '(' && d == ')')
        return true;
    else if (c == '{' && d == '}')
        return true;
    else if (c == '[' && d == ']')
        return true;
    return false;
}

bool isQuotation(char c){
    if (c == '\'' || c == '"')
        return true;
    return false;
}

bool openBlockComment(char line[]){
    if (*line == '/'){
        if (*(line-1) == '*'){
            return true;
        }else if (*(line-1) == '/'){
            return true;
        }
    }
    else if (*line == '*'){
        if (*(line-1) == '/'){
            return true;
        }
    }
    return false;
}

bool balanceFile(istream &dictfile) {
    char c,d;
    stack<char> mark;
    stack<int> line;
    bool match = true;
    int count = 1;
    bool status = false;
    dictfile.clear();
    dictfile.seekg(0, dictfile.beg);
    string li;
    while (getline(dictfile,li)){
        int num = 0;
        while (num < li.size()){ // num < li.size() / li[num] != '\0'
            if (openBlockComment(&li[num]) || isQuotation(li[num])){
                //Checking "
                if (li[num] == '"'){
                    int i;
                    mark.push(li[num]);
                    line.push(li[num]);
                    for (i = num+1; li[i]!='"'; i++){
                        if (i >= li.size()){
                            match = false;
                            break;
                        }
                    }
                    if (match == false){
                        break;
                    }
                    num = i;
                    status = false;
                    mark.pop();
                    line.pop();
                    cout<<"pair matching \" and \""<<endl;
                //Checking '
                }else if (li[num] == '\''){
                    int i;
                    mark.push(li[num]);
                    line.push(count);
                    for (i = num+1; li[i]!= '\''; i++){
                        if (i >= li.size()){
                            match = false;
                            break;
                        }
                    }
                    if (match == false){
                        break;
                    }
                    num = i;
                    status = false;
                    mark.pop();
                    line.pop();
                    cout<<"pair matching \' and \'"<<endl;
                //Checking */
                }else if (li[num] == '*' && li[num-1] == '/'){
                    if (mark.empty() || mark.top() != '*'){
                        mark.push(li[num]);
                        line.push(count);
                        status = true;
                    }
                }else if (li[num] == '/'){
                    //Checking /*
                    if (li[num-1] == '*'){
                        if (mark.top() == '*'){
                            mark.pop();
                            line.pop();
                            status = false;
                            cout<<"pair matching /* and */"<<endl;
                        }
                    //Checking //
                    }else if (li[num-1] == '/'){
                            status = false;
                        break;
                    }
                }
            }
            if (status == false && isOpeningBrace(li[num])){
                mark.push(li[num]);
                line.push(count);
            }else if (status == false && isClosingBrace(li[num])){
                c = li[num];
                if (mark.empty()){
                    match = false;
                    break;
                }
                d = mark.top();
                if (!isBalancedBrace(d, c)){
                    match = false;
                    break;
                }else{
                    cout<<"pair matching "<<d<< " and "<<c<<endl;
                }
                mark.pop();
                line.pop();
            }
            num++;
        }
        if (match == false){
            break;
        }
        count++;
    }
    if (!mark.empty()){
        match = false;
    }
    if (match == true)
        return true;
    else
        cout<<"unmatched "<<mark.top()<<" symbol on line "<<line.top()<<endl;
        return false;
}

int main()
{
    ifstream infile;
    string filename;


    cout << "Please enter filename for C++ code: ";
    cin >> filename;
    infile.open(filename.c_str());

    //for test
//    string dic = "/Users/junnymac/Desktop/Fall 2021/CSCI 02 0130/Assignment/Project4/bad_balance2.cpp";
//    infile.open(dic);

    if (!infile) {
        cout << "File not found!" << endl;
        return (1);
    }

    printFile(infile);

    if (balanceFile(infile))
        cout << "Balance is ok" << endl;
//    else
//        cout<<"Balance not ok"<<endl;
}