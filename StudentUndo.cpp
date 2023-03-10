#include "StudentUndo.h"

Undo* createUndo()
{
    return new StudentUndo();
}
/*ERROR = 0,
 INSERT = 1,
 SPLIT = 2,
 DELETE = 3,
 JOIN = 4*/


void StudentUndo::submit(const Action action, int row, int col, char ch) {
    // TODO
    Info *info=new Info;
    info->action=action;
    info->Row=row;
    info->Col=col;
    
    if(s->size()==0)
    {
        string s1(1,ch);
        info->ch=s1;
        s->push(info);
    }
    else{
    switch (action) {
        case StudentUndo::Action::DELETE:
        {
            Info *top=s->top();
            
            if(!(top->Row==row || top->Col==col))
            {
                string s(1,ch);
                info->ch=s;
                
            }
            else{
                string temp="";
                
                if(!s->empty() && s->top()->Row==row && s->top()->Col-1==col)
                {
                    temp=ch+s->top()->ch;
                    s->pop();
                }
                else if(!s->empty() && s->top()->Row==row && s->top()->Col==col)
                {
                    temp=s->top()->ch+ch;
                }
                info->ch=temp;
            }
            s->push(info);
        }
            break;
            
        case StudentUndo::Action::INSERT:
        {
            
          Info *top=s->top();
            
            if(!(top->Row==row || top->Col==col))
            {
                string s(1,ch);
                info->ch=s;
                
            }
            else{
                string temp="";
                
                if(!s->empty() && s->top()->Row==row && s->top()->Col+1==col)
                {
                    temp=s->top()->ch+ch;
                    s->pop();
                }
                info->ch=temp;
            }
            s->push(info);
        }
            break;
        case  StudentUndo::Action::SPLIT:
        case StudentUndo::Action::JOIN:
        {
            string s1(1,ch);
            info->ch=s1;
            s->push(info);
        }
        
            break;
        default:
            break;
    }
    }
    
}

StudentUndo::Action StudentUndo::get(int& row, int& col, int& count, std::string& text) {
    if(s->empty())
        return Action::ERROR;
    
    Info *info=s->top();
    s->pop();
    row=info->Row;
    col=info->Col;
    text=info->ch;
    if(info->action==Action::INSERT)
    {
         size_t n=text.length();
            col=(int)(col-n);
           count=(int)n;
        return Action::DELETE;
        
    }
    else if(info->action==Action::DELETE)
    {
         
        return Action::INSERT;
        
    }
    
    else if(info->action==Action::SPLIT)
    {
        return Action::JOIN;
    }
    else if(info->action==Action::JOIN)
    {
        return Action::SPLIT;
    }
   // return 0;
   return Action::ERROR;  // TODO
}

void StudentUndo::clear() {
    // TODO
    while (!s->empty()) {
        s->pop();
    }
}
