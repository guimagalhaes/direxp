#ifndef DIREXPH_
#define DIREXPH_

#include <unistd.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <cstdio>

#include <string>

//order entries returned on each directory
//used on scandir
extern int alphasort();

typedef void (*callback_t)(const char*);

class direxp
{
    private:
        //used on scandir
        //select entries to be readed
        static int file_select(const struct dirent *entry);
            
        //search dir and subdirs, initialize at pathname
        //execute todo function on each entry of TYPE_FILE type
        int dir_rec(const std::string &pathname, callback_t todo_files,
             callback_t todo_dir,callback_t todo_parentdir) const;
        
    public:
        static const unsigned TYPE_DIR    = 4;
        static const unsigned TYPE_FILE    = 8;
        
        //save original path, call dir_rec and go back to original path
        //recursive
        //returns zero
        int dir_explorer_rec(const std::string &pathname, callback_t todo_files,
            callback_t todo_dir=NULL,callback_t todo_parentdir=NULL) const;
        
        //save original path, read dir entries and go back to original path
        //not recursive
        //returns -1 if a error occurs, returns zero otherwise
        int dir_explorer(const std::string &pathname, callback_t todo_files,
            callback_t todo_dir=NULL) const;
        
        //return true if a file could be removed
        static bool remove_dir(const std::string &dir);
        //return true if a directory could be removed
        static bool remove_file(const std::string &file);
        
        //create directory
        static bool create_dir(const std::string &dir);
        
        //return true if directory is empty
        static bool is_empty(const std::string &dir);
        
        //return current dir
        //after returned value be used, its buffer must be released with delete[]
        static std::string get_pwd();
        
        //call chdir
        static int chdir(const std::string &path) {return ::chdir(path.c_str());}
        
        //return true if pathname ends with "/"
        //static bool endsWithSlash(const std::string &path);
        //put "/" on pathname if it not ends with "/"
        static std::string &put_slash(std::string &pathname);
        
        //return file name of a file path or last folder if a is just a path
        static std::string get_file_name(const std::string &pathname);
        
        //return true if source string starts with prefix string
        //static bool startsWith(const std::string &source, const std::string &prefix);
        
        //return a string containing the absolute directory of relative_dir string 
        static std::string get_absolute_dir(const std::string &relative_dir);
        
        //return true if the path corresponds to a directory
        static bool is_dir(const std::string &path);
};

#endif
