#ifndef BMP_H_INCLUDED
#define BMP_H_INCLUDED

void open_image(const char *);          
void open_hidden(const char *);                     
void open_output(const char *);                     
void close_files();                     
int giveMeSize(const char *);
bool check_if_right_path(const char *);
void get_header(FILE* fp);              
void print_header(FILE* fp);            
void print_last_modified(const char *);
void hide_message(const char *);                    
void unhide_message(const char *, int);                                        
int countCharacters(const char *);
void goForward();
#endif