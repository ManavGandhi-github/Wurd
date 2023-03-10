#ifndef STUDENTSPELLCHECK_H_
#define STUDENTSPELLCHECK_H_

#include "SpellCheck.h"

#include <string>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;


class Trie
{
    
private:
struct Node
{
    int prefix_count;
    bool isEnd;
    Node *child[27];
}*headptr;
    vector<Node*> nodeList;
public:
Trie()
{
    headptr = new Node();
    nodeList.push_back(headptr);
    headptr->isEnd = false;
    headptr->prefix_count = 0;
}
~Trie()
    {
        for (auto it = nodeList.begin(); it != nodeList.end(); ++it)
        delete *it;
    }
void insert(string word)
{
    toLower(word);
    Node *current = headptr;
    current->prefix_count++;
    
    for(int i = 0 ; i < word.length(); ++i)
    {
        int letter;
        if(word[i]!='\'')
        letter= (int)word[i] - (int)'a';
        else
        letter=26;
        //extrct first character of word
        if(current->child[letter] == NULL){
            current->child[letter] = new Node();
            nodeList.push_back(current->child[letter]);
        }
        current->child[letter]->prefix_count++;
        current = current->child[letter];
    }
    current->isEnd = true;
}

bool search(string word)
    {  toLower(word);
    Node *current = headptr;
    for(int i = 0 ; i < word.length(); ++i)
    {
        int letter;
        if(word[i]!='\'')
        
        letter= (int)word[i] - (int)'a';
        else
        letter=26;
        if(current->child[letter] == NULL)
            return false;        //not found
        current = current->child[letter];
    }
    return current->isEnd;
}
Node* getHeader()
{
    return headptr;
}
bool compare(string s1, string s2)
{   int count=0;
    for(int i=0;i<s1.size();i++)
    {
        if(s1[i]==s2[i])
        continue;
        count++;
        if(count==2)
        return false;
    }
    return true;
}
void searchList(Node *p, int curLength, int maxLength,string search,string msg,vector<string> &v,int max_no_suggestions)
{
   if(p==nullptr||max_no_suggestions==v.size())
    return ;
    if(curLength==maxLength){
        
        if(p->isEnd)
        {   if(compare(search,msg)){
            v.push_back(search);
            
        }
        }
         else
         return;
    }else
    {
        for(int i=0;i<27;i++)
        {
            if(p->child[i]!=nullptr)
            {  char ch;
                if(i>=0&&i<=25)
                ch=(char)(i+97);
                else
                ch='\'';
                
                searchList(p->child[i],curLength+1, maxLength,search+ch,msg,v,max_no_suggestions);
            }
        }
    }
    
}
void toLower(string &word)
    {
        for(int i=0;i<word.size();i++)
        {
            if(word[i]>='A'&&word[i]<='Z')
                word[i]=word[i]+32;
        }
    }
int words_with_prefix(string prefix)
{
    toLower(prefix);
    Node *current = headptr;
    for(int i = 0; i < prefix.length() ; ++i)
    {int letter;
            if(prefix[i]!='\'')
        
            letter= (int)prefix[i] - (int)'a';
            else
            letter=26;
            
        if(current->child[letter] == NULL)
            return 0;
        else
            current = current->child[letter];
    }
    return current->prefix_count;
}
};


class StudentSpellCheck : public SpellCheck {
public:
    StudentSpellCheck() {
        
    }
    virtual ~StudentSpellCheck();
    bool load(std::string dict_file);
    bool spellCheck(std::string word, int maxSuggestions, std::vector<std::string>& suggestions);
    void spellCheckLine(const std::string& line, std::vector<Position>& problems);
    struct Position
    {
        int start;
        int end;
        
    };
private:
    Trie dict;
    
};

#endif  // STUDENTSPELLCHECK_H_
