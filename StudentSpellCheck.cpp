#include "StudentSpellCheck.h"
#include <string>
#include <vector>
#include<fstream>
using namespace std;
SpellCheck* createSpellCheck()
{
    return new StudentSpellCheck();
}

StudentSpellCheck::~StudentSpellCheck() {
    // TODO
    
}

bool StudentSpellCheck::load(std::string dictionaryFile) {
    
   ifstream fin(dictionaryFile);
    if(fin.fail())
        return false;
    char ch;
    string s="";
    while(fin)
    {
        fin.get(ch);
        if(ch>='A'&&ch<='Z')
            ch=ch+32;
        if(ch>='a'&&ch<='z')
            s=s+ch;
        if(ch=='\'')
            s=s+ch;
        if(ch=='\n')
        {
            dict.insert(s);
            s="";
        }
        
        
    }
    
    return true; // TODO
}

bool StudentSpellCheck::spellCheck(std::string word, int max_suggestions, std::vector<std::string>& suggestions) {
    
    
  if(dict.search(word))
        return true;
    else
    {
        suggestions.clear();
        dict.searchList(dict.getHeader(), 0,(int)word.size(), "", word, suggestions,max_suggestions);
        
    }
    return false; // TODO
}

void StudentSpellCheck::spellCheckLine(const std::string& line, std::vector<SpellCheck::Position>& problems) {
   string line2=line;
    string s="";
    for(int i=0;i<line2.size();i++)
    {
        if(line2[i]>='A'&&line2[i]<='Z')
            line2[i]=(char)(line2[i]+32);
        if((line2[i]>='a'&&line2[i]<='z'))
            s=s+ line2[i];
       // if(line2[i]==)
        else
        {
            if(s.size()>0){
                if(!dict.search(s)){
                SpellCheck::Position p;
                p.start= (int)(i-s.size());
                p.end=i-1;
                problems.push_back(p);
                }
                s="";
            }
        }
        
    }
    
    // TODO
}
