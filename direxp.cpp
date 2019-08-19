#include <exception>
#include "direxp.h"
#include "format_string.h"
#include <string.h>
#include <vector>

#ifdef DEBUG
#include <iostream>
#endif

using namespace std;

string direxp::get_pwd()
{
    char *ret;
    char buffer[512];
    string s = "";
    
    ret =  getcwd(buffer,512);
    
    if (ret != NULL)
        s = string(buffer);
    
    return s;
}

int direxp::file_select(const struct dirent *entry)
{    
    if ((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0))
        return 0;
    else
        return 1;
}

int direxp::dir_explorer(const string &pathname,callback_t todo_files,
            callback_t todo_dir) const
{
    struct dirent **entries;
    int count;
    vector<string> files_entries;
    string file_name;
    string original_path = get_pwd();
        
    if (chdir(pathname) != 0) {
#ifdef DEBUG
        cerr << "Could not change dir" << endl;
#endif
        return -1;
    }
    
    if (todo_dir != NULL)
        todo_dir(pathname.c_str());
    
    count = scandir(pathname.c_str(), &entries, file_select, alphasort);
    
    for (int i = 0; i < count; ++i) {
        if (entries[i]->d_type == TYPE_FILE){
            file_name = pathname;
            file_name = put_slash(file_name) + entries[i]->d_name;
            files_entries.push_back(file_name);
        }
        free(entries[i]);
    }
    free(entries);
    
    if (todo_files != NULL)
        for (unsigned i=0; i<files_entries.size(); ++i)
            todo_files(files_entries[i].c_str());
    
    if (chdir(original_path) != 0) {
#ifdef DEBUG
        cerr << "Could not change dir" << endl;
#endif
        return -1;
    }
    
    return count >= 0 ? 0 : -1;
}


int direxp::dir_rec(const std::string &pathname, callback_t todo_files,
            callback_t todo_dir,callback_t todo_parentdir) const
{
    struct dirent **entries;
    int count;
    string new_path;
    vector<string> files_entries;
    string file_name;
    
    if (todo_dir != NULL)
        todo_dir(pathname.c_str());
    
    count = scandir(pathname.c_str(), &entries, file_select, alphasort);
    
    for (int i = 0; i < count; ++i) {
        if (entries[i]->d_type == TYPE_DIR) {
            if (chdir(entries[i]->d_name) != 0) {
#ifdef DEBUG
                cerr << "Could not change dir" << endl;
#endif
                return -1;
            }            
            
            new_path = pathname;
            new_path = put_slash(new_path) + entries[i]->d_name;            
            dir_rec(new_path,todo_files,todo_dir,todo_parentdir);
            
            if (chdir("..") != 0) {
#ifdef DEBUG
                cerr << "Could not change dir" << endl;
#endif
                return -1;
            }
            if (todo_parentdir != NULL) {
                todo_parentdir(get_pwd().c_str());
            }
        }
        else {
            file_name = pathname;
            file_name = put_slash(file_name) + entries[i]->d_name;
            files_entries.push_back(file_name);
        }
        
        free(entries[i]);
    }
        
    free(entries);
    
    if (todo_files != NULL)
        for (unsigned i=0; i<files_entries.size(); ++i)
            todo_files(files_entries[i].c_str());
    
    return 0;
}

int direxp::dir_explorer_rec(const string &pathname,callback_t todo_files,
            callback_t todo_dir,callback_t todo_parentdir) const
{
    int i;
    string original_path = get_pwd();
        
    if (chdir(pathname) != 0) {
#ifdef DEBUG
        cerr << "Could not change dir" << endl;
#endif
        return -1;
    }
    
    i = dir_rec(pathname,todo_files,todo_dir,todo_parentdir);
    
    if (chdir(original_path) != 0) {
#ifdef DEBUG
        cerr << "Could not change dir" << endl;
#endif
        return -1;
    }
    
    return i;
}

bool direxp::remove_dir(const string &dir)
{
    if (rmdir(dir.c_str()) == 0)
        return true;
    
    return false;
}

bool direxp::create_dir(const string &dir)
{
    if (mkdir(dir.c_str(),S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) == 0)
        return true;
    
    return false;
}

bool direxp::remove_file(const string &file)
{
    if (remove(file.c_str()) != -1)
        return true;
    
    return false;
}

bool direxp::is_empty(const string &dir)
{
    struct dirent **entries;
    int count;
            
    count = scandir(dir.c_str(), &entries, file_select, alphasort);
    
    for (int i = 0; i < count; ++i) {
        free(entries[i]);
    }
    free(entries);
    
    return count == 0 ? true : false;
}

string &direxp::put_slash(string &pathname)
{
    if (!format_string::ends_with(pathname,"/"))
        pathname += "/";
    
    return pathname;
        
}

string direxp::get_file_name(const string &pathname)
{
    string s = pathname;
    string::size_type pos;
    
    if (format_string::ends_with(s,"/"))
        s.erase(s.size()-1,1);
    
    pos = s.rfind("/");
    if (pos == string::npos) return s;
        
    ++pos;
    return s.substr(pos,s.size()-pos);
}

string direxp::get_absolute_dir(const string &relative_dir)
{
    string path = relative_dir;
    
    if (!format_string::starts_with(path,"/")) {
        path = get_pwd() + "/" + path;
    }
    
    return path;
}

bool direxp::is_dir(const string &path)
{
    struct stat buf = {0};
    
    stat(path.c_str(),&buf);
    
    return S_ISDIR(buf.st_mode);
}
