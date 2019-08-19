#include "direxp.h"
#include <iostream>

using namespace std;

void print_filenames(const char *s)
{
    cout << s << endl;
}

int main()
{    
    direxp dir;
    string pathname = dir.get_pwd();
    
    cout << pathname << endl;    
    
    string s = "teste/teste2";
    cout << dir.put_slash(s) << endl;
    cout << dir.get_file_name(s) << endl;
    
    s = "teste/teste2/";
    cout << dir.put_slash(s) << endl;
    cout << dir.get_file_name(s) << endl;
    s = "teste/teste2/file.txt";
    cout << dir.get_file_name(s) << endl << endl;
    
    dir.create_dir("./new");
    if (!dir.create_dir("new dir"))
        cout << "create \"new dir\" failed" << endl;
        
    if (dir.is_dir("/home"))
        cout << "/home is dir" << endl;
        
    if (dir.is_dir("/home/guicampo/cd1"))
        cout << "/home/guicampo/cd1 is dir" << endl;

    if (dir.is_dir("/home/guicampo/cd1/"))
        cout << "/home/guicampo/cd1/ is dir" << endl;
    
    cout << endl;
    
    dir.dir_explorer(pathname,print_filenames);
    cout << "----" << endl;
    dir.dir_explorer_rec(pathname,print_filenames);
    
    return 0;
}
