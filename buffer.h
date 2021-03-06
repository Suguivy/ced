#ifndef BUFFER_H
#define BUFFER_H

#include <wchar.h>

/* The char_node and line_node structs represent respectively a character of a sigular line
 * and a line of a singular buffer, in the form of a double linked list.
 */
struct char_node {
  wchar_t elem[2];
  struct char_node *prev_char;
  struct char_node *next_char;
};

struct line_node {
  struct char_node *first_char;
  struct char_node *last_char;
  struct line_node *prev_line;
  struct line_node *next_line;
};

/* The buffer represents a structure of a double linked list containing the lines,
 * and references to the current line and the current char pointed by the cursor.
 * It also contains the position of the cursor in the x-y axis. The variable
 * cursor_real_x is used when we are on a line and we move to a shorter one
 * and the cursor is forced to move to the left. The value of the last position
 * is stored in that variable and remains unchanged unless we move horizontally.
 */
struct buffer {
  struct line_node beg_sentinel; // TODO: make const if possible
  struct line_node end_sentinel;
  struct line_node *current_line;
  struct char_node *current_char;
  int displacement_y;
  int displacement_x;
  int cursor_x;
  int cursor_real_x;
  int cursor_y;
  int memory_buffer;
  char *name;
};

#define buffer_first_line(b) (b->beg_sentinel.next_line)
#define buffer_last_line(b) (b->end_sentinel.prev_line)

#define buffer_bol(b) (b->current_char == b->current_line->first_char) // Beggining of line
#define buffer_eol(b) (b->current_char == b->current_line->last_char)  // End of line
#define buffer_bob(b) (b->current_line->prev_line == &b->beg_sentinel) // First line of buffer
#define buffer_eob(b) (b->current_line->next_line == &b->end_sentinel) // Last line of buffer

#define buffer_move_x_end(b) while(buffer_move_x(b, 1));
#define buffer_move_x_home(b) while(buffer_move_x(b, -1));
#define buffer_move_y_bottom(b) while(buffer_move_x(b, 1));
#define buffer_move_y_top(b) while(buffer_move_y(b, -1));
#define buffer_update_real_x(b) (b->cursor_real_x = b->cursor_x)

#define line_node_empty(l) (l->first_char == l->last_char)

// Creates a new char node
struct char_node *char_node_new(unsigned int ch);

// Creates a new empty line
struct line_node *line_node_new();

// Frees the memory of a line_node
void line_node_free(struct line_node *line);

// Creates a new buffer with an empty line
struct buffer *buffer_new(char *filename);

// Returns the name of the buffer
char *buffer_get_name(struct buffer *buff);

// Frees the memory of a buffer
void buffer_free(struct buffer *buff);

// Inserts a character into the buffer
void buffer_insert_char(struct buffer *buff, unsigned int ch);

// Deletes the character that the cursor is pointing to
void buffer_delete_char(struct buffer *buff);
 
// Deletes the line that the cursor is pointing to.
// If the line is the last line in the buffer, the cursor is moved to the line
// above before the deletion. If there is only one line, the line is erased.
void buffer_delete_line(struct buffer *buff);

// Moves the line contents from the cursor to the end into a new line below
void buffer_split_line(struct buffer *buff);

// Joins the current line and the line below into one.
void buffer_join_line(struct buffer *buff);

// Moves the x coordinate of the buffer cursor.
// Returns the numbers of steps that the cursor has moved
int buffer_move_x(struct buffer *buff, int dx);

// Moves the y coordinate of the buffer cursor
// Returns the numbers of steps that the cursor has moved
int buffer_move_y(struct buffer *buff, int dy);

// Reads a file and writes it into the buffer
void buffer_read_file(struct buffer *buff, char *filename);

// Saves the current opened file of the buffer
void buffer_save_file(struct buffer *buff);

#endif // BUFFER_H
