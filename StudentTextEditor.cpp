#include "StudentTextEditor.h"
#include "Undo.h"
#include <string>
#include <vector>
#include<fstream>
#include<iostream>
using namespace std;
TextEditor* createTextEditor(Undo* un)
{
    return new StudentTextEditor(un);
}

StudentTextEditor::StudentTextEditor(Undo* undo)
 : TextEditor(undo) {
     row=0;
     col=0;
     undoObj=undo;
     buffer=new list<string*>();
    // buffer.resize(1024);
     filename="";
}

StudentTextEditor::~StudentTextEditor()
{
    for(auto it=buffer->begin();it!=buffer->end();++it)
    delete *it;
    delete buffer;
}

 bool StudentTextEditor::load(std::string file) {
     int count=0;
    if(file=="")
    {
        for(int i=1;i<=8000;i++){
        cout<<" ";
        if(i%80==0)
            cout<<endl;
       
        }
    }
    else{
        
    
     std::ifstream fin(file);
     
     if(fin.fail()){
            
         return false;
     }
     
     
    filename=file;
     if(buffer->size()>0)
     {
         reset();
     }
     
        
            
            
            string *line=new string();
            char ch;
            while(fin)
            {
                fin.get(ch);
                
                if(ch!='\r'){
                    if(ch=='"')
                        ch='\"';
                    
                    
                    line->push_back(ch);
                
                    std::cout<<ch;
                
                }
                if(ch=='\n')
                {
                    buffer->push_back(line);
                    line=new string();
                    count++;
                    if(count>=buffer->max_size())
                    buffer->resize(count*2);
                }
                
            }
            fin.close();
        
            
    }
    
    return true;  // TODO
}

bool StudentTextEditor::save(std::string file) {
    
    ofstream fout(file);
    if(!fout.is_open())
        return false;
    
    for (string*x : *buffer) {
        fout << *x;
        }
    
    fout.close();
    return true;  // TODO
}

void StudentTextEditor::reset() {

   
    while(buffer->size()>0)
    {
        buffer->pop_front();
    }
    row=0;
    col=0;
    
    //AFTER the UNDO obk=ject pop all the elements of undo stack
    undoObj->clear();
}

void StudentTextEditor::move(Dir dir) {
switch(dir)
{
    case (TextEditor::Dir::UP):
        if(row>0)
        row--;
    break;
    case (TextEditor::Dir::DOWN):
        if(buffer->size()>row)
        row++;
    break;
    case (TextEditor::Dir::LEFT):
        if(col>0)
        col--;
        
        else if(col==0){
            row--;
            std::list<string*>::iterator it = buffer->begin();
            std::advance(it, row);
            string *s=*it;
            col= (int)s->size();}
    break;
    case (TextEditor::Dir::RIGHT):{
        std::list<string*>::iterator it = buffer->begin();
        std::advance(it, row);
        string *s=*it;
        
        if(col<s->size())
            col++;
        else
        {   col=0;
            row++;
        }
    }
    break;
    case (TextEditor::Dir::HOME):
       // row=0;
        
        col=0;
    break;
    case TextEditor::Dir::END:
        // Need to solve
       
        std::list<string*>::iterator it = buffer->begin();
        std::advance(it, row);
        string *s=*it;
        col=(int)s->size()-1;
    break;
}
    
}

void StudentTextEditor::del() {
    std::list<string*>::iterator it = buffer->begin();
    std::advance(it, row);
    string *s=*it;
    string *t=s;
    if(col>s->size())
        return;
    if(col!=s->size()-1){
    s->erase(col, 1);
    //std::replace (buffer->begin(), buffer->end(), t, s);
        undoObj->submit(Undo::Action::DELETE, row, col,t->at(col));
    }
    else{
        ++it;
        string *line2=*it;
        s->erase(s->size()-1,1);
        s->append(*line2);
        buffer->remove(line2);
       // std::replace (buffer->begin(), buffer->end(), t, s);
        undoObj->submit(Undo::Action::JOIN, row, col,'\n');
    }
    
}

void StudentTextEditor::backspace() {
    std::list<string*>::iterator it = buffer->begin();
    std::advance(it, row);
    string *s=*it;
    string *t=s;
    
    
    if(col>0){
        char ch=s->at(col-1);
    s->erase(col-1, 1);
    
    undoObj->submit(Undo::Action::DELETE, row, col-1,ch);
        col--;
    }
    else{
        --it;
        string *line1=*it;
        string temp=*line1;
        line1->erase(line1->size()-1,1);
        line1->append(*s);
     //   std::replace (buffer->begin(), buffer->end(), temp, line1);
        row--;
        col=(int)temp.size()-1;
       undoObj->submit(Undo::Action::JOIN, row, col,'\n');
       // ++it;
        
        buffer->remove(t);
        
    }
}

void StudentTextEditor::insert(char ch) {
    
  std::list<string*>::iterator it = buffer->begin();
    std::advance(it, row);
    string *s=*it;
    
    if(ch!='\t'){
    s->insert(col,1,ch);
        col++;
    undoObj->submit(Undo::Action::INSERT, row, col,ch);
    }
        
    else{
        ch=' ';
        for(int i=1;i<=4;i++){
        s->insert(col,1,ch);
            col++;
       undoObj->submit(Undo::Action::INSERT, row, col,ch);
            
            
        }
        
       
    }
    
    
}

void StudentTextEditor::enter() {
    std::list<string*>::iterator it = buffer->begin();
    std::advance(it, row);
    string *s=*it;
    string l1=s->substr(0,col+1)+"\n";
    string l2=s->substr(col+1, s->size());
    string *line1=new string();
    line1->append(l1);
    string *line2=new string();
    line2->append(l2);
    std::replace (buffer->begin(), buffer->end(), s, line1);
    ++it;
    buffer->insert(it, line2);
    undoObj->submit(Undo::Action::SPLIT, row, col,10);  //test
    row++;
    col=0;
    
    
    
    
}

void StudentTextEditor::getPos(int& row, int& col) const {
    row=this->row;
    col=this->col;
}

int StudentTextEditor::getLines(int startRow, int numRows, std::vector<std::string>& lines) const {
    int count=0;
    if(startRow<0 || numRows<0 ||startRow>=buffer->size())
    return -1; // TODO
    lines.clear();
    auto l_front = buffer->begin();

    std::advance(l_front, startRow);

    for(int i=1;i<=numRows;i++)
    {
        
        if(l_front!=buffer->end()){
            string *s=*l_front;
        lines.push_back(*s);
            count++;
        l_front++;
        }
        else
            break;
        
    }

    
    
    return count;
}

void StudentTextEditor::undo() {
    int rowmodified, colmodified,count;
    string text;
    Undo::Action action=undoObj->get(rowmodified, colmodified,count, text);
    switch (action) {
        case Undo::Action::INSERT:{
            row=rowmodified;
            col=colmodified;
            std::list<string*>::iterator it = buffer->begin();
            std::advance(it, row);
            string *s=*it;
            
            s->insert(col,text);
           // std::replace (buffer->begin(), buffer->end(), t, s);
        }
            break;
        case Undo::Action::DELETE:{
            row=rowmodified;
            col=colmodified;
            std::list<string*>::iterator it = buffer->begin();
            std::advance(it, row);
            string *s=*it;
            //string *t=s;
            s->erase(col,count);
            //std::replace (buffer->begin(), buffer->end(), t, s);
        }break;
        case Undo::Action::SPLIT:{
            row=rowmodified;
            col=colmodified;
            std::list<string*>::iterator it = buffer->begin();
            std::advance(it, row);
            string* s=*it;
            string l1=s->substr(0,col)+"\n";
            string l2=s->substr(col, s->size());
            string *line1=new string();
            line1->append(l1);
            string *line2=new string();
            line2->append(l2);
            std::replace (buffer->begin(), buffer->end(), s, line1);
            ++it;
            buffer->insert(it, line2);
            
            row++;
            col=0;
            
        }break;
        case Undo::Action::JOIN:{
            
                row=rowmodified;
                col=colmodified;
                std::list<string*>::iterator it = buffer->begin();
                std::advance(it, row);
                string *s=*it;
                string *t=s;
                ++it;
                string *line1=*it;
                string *temp=line1;
                s->erase(s->size()-1,1);
            s->append(*line1);
                std::replace (buffer->begin(), buffer->end(), t, s);
            
        
                buffer->remove(temp);
            
        }
        
            break;
        case Undo::Action::ERROR:
            break;
            
        default:
            break;
    }
    // TODO
}
