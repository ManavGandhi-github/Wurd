#ifndef STUDENTUNDO_H_
#define STUDENTUNDO_H_
#include <stack>
#include "Undo.h"
using namespace std;
struct Info
{
    Undo::Action action;
    int Row;
    int Col;
    string ch;
    
};
class StudentUndo : public Undo {
public:
    StudentUndo(){
        s=new stack<Info*>();
    }
    void submit(Action action, int row, int col, char ch = 0);
    Action get(int& row, int& col, int& count, std::string& text);
    void clear();

private:
    stack<Info*> *s;
    
};

#endif // STUDENTUNDO_H_
