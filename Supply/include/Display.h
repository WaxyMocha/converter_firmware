#ifndef _Display_H_
#define _Display_H_

const uint8_t SSD1306_I2C_ADDRESS = 0x78;

const uint8_t SSD1306_LCDWIDTH = 128;
const uint8_t SSD1306_LCDHEIGHT = 32;

const uint8_t SSD1306_SETCONTRAST = 0x81;
const uint8_t SSD1306_DISPLAYALLON_RESUME = 0xA4;
const uint8_t SSD1306_DISPLAYALLON = 0xA5;
const uint8_t SSD1306_NORMALDISPLAY = 0xA6;
const uint8_t SSD1306_INVERTDISPLAY = 0xA7;
const uint8_t SSD1306_DISPLAYOFF = 0xAE;
const uint8_t SSD1306_DISPLAYON = 0xAF;

const uint8_t SSD1306_SETDISPLAYOFFSET = 0xD3;
const uint8_t SSD1306_SETCOMPINS = 0xDA;

const uint8_t SSD1306_SETVCOMDETECT = 0xDB;

const uint8_t SSD1306_SETDISPLAYCLOCKDIV = 0xD5;
const uint8_t SSD1306_SETPRECHARGE = 0xD9;

const uint8_t SSD1306_SETMULTIPLEX = 0xA8;

const uint8_t SSD1306_SETLOWCOLUMN = 0x00;
const uint8_t SSD1306_SETHIGHCOLUMN = 0x10;

const uint8_t SSD1306_SETSTARTLINE = 0x40;

const uint8_t SSD1306_MEMORYMODE = 0x20;
const uint8_t SSD1306_COLUMNADDR = 0x21;
const uint8_t SSD1306_PAGEADDR =   0x22;

const uint8_t SSD1306_COMSCANINC = 0xC0;
const uint8_t SSD1306_COMSCANDEC = 0xC8;

const uint8_t SSD1306_SEGREMAP = 0xA0;

const uint8_t SSD1306_CHARGEPUMP = 0x8D;

const uint8_t SSD1306_EXTERNALVCC = 0x1;
const uint8_t  SSD1306_SWITCHCAPVCC = 0x2;

// Scrolling #defines
const uint8_t SSD1306_ACTIVATE_SCROLL = 0x2F;
const uint8_t SSD1306_DEACTIVATE_SCROLL = 0x2E;
const uint8_t SSD1306_SET_VERTICAL_SCROLL_AREA = 0xA3;
const uint8_t SSD1306_RIGHT_HORIZONTAL_SCROLL = 0x26;
const uint8_t SSD1306_LEFT_HORIZONTAL_SCROLL = 0x27;
const uint8_t SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL = 0x29;
const uint8_t SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL = 0x2A;


//functions

void command(uint8_t c);
void Display_init ();
void Display_invert(bool i);
void Display_scroll (uint8_t start, uint8_t stop, bool right);
void Display_diagonal_scroll (uint8_t start, uint8_t stop, bool right);
void Display_scroll_stop ();
void Display_dim (bool dim);
void Display_pixel(int x, int y, bool enable);
void Display_send ();
void Display_bitmap_P (int x, int y, const uint8_t *bitmap, uint8_t w, uint8_t h, bool enable);
void Display_line(int x0, int y0, int x1, int y1, bool enable);
void Display_veritcal_line(int x, int y, int h, bool enable);
void Display_horizontal_line(int x, int y, int w, bool enable);
void Display_fill_screen (bool enable);
void Display_char(int x, int y, char c, bool enable, bool background, uint8_t size);
void Display_text (int x, int y, char * str, uint8_t txt_size, bool eanable, bool background);
void Display_int (int x, int y, int data, uint8_t txt_size, bool enable, bool background);

void Display_rect (int x, int y, int w, int h, bool enable);
void Display_fill_rect (int x, int y, int w, int h, bool enable);

#endif /* _Display_H_ */