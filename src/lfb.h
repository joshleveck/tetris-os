void lfb_init();
void lfb_print(int x, int y, char *s);
void lfb_proprint(int x, int y, char *s);
void draw_block(int x, int y, unsigned int colour);
void draw_x_line(int start_x, int start_y, int length, unsigned int colour);
void draw_y_line(int start_x, int start_y, int length, unsigned int colour);