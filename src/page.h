#ifndef PAGE_H
#define PAGE_H

#define NUM_PAGE 15
#define PAGE_LEN 64 /**< 64 caractères sans le marqueur de fin*/
#define NAI 255 /**< Not An Index*/

typedef struct page {
    const char *str;
    int positions[16];
    int num_param;
} Page;

int change_page(void);
byte get_page_index(void);
void set_page_index(byte index); 
void set_cursor_index(byte index); 
byte get_par_index(void);
page get_page(void);
int get_cur_pos(void);
int get_next_cur_pos(void);
byte cc2index(byte cc); 
void move_cursor(int8_t rotation);
void get_string(char str[8]); 

#endif
