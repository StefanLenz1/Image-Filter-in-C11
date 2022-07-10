#include <stdint.h>

/* Von https://docs.microsoft.com/en-us/previous-versions/aa922590(v=msdn.10)?redirectedfrom=MSDN */
// Farbenkomposition:
typedef struct { 
  uint8_t rgbtBlue; 
  uint8_t rgbtGreen; 
  uint8_t rgbtRed; 
} __attribute__((__packed__))
RGBTRIPLE;

/* Von https://docs.microsoft.com/en-us/previous-versions//dd183376(v=vs.85)?redirectedfrom=MSDN */
// Info Header:
typedef struct tagBITMAPINFOHEADER {
  uint32_t biSize;
  int32_t biWidth;
  int32_t biHeight;
  uint16_t biPlanes;
  uint16_t biBitCount;
  uint32_t biCompression;
  uint32_t biSizeImage;
  int32_t biXPelsPerMeter;
  int32_t biYPelsPerMeter;
  uint32_t biClrUsed;
  uint32_t biClrImportant;
} __attribute__((__packed__))
BITMAPINFOHEADER;


/* Von https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapfileheader?redirectedfrom=MSDN */
// File Header:
typedef struct tagBITMAPFILEHEADER {
  uint16_t  bfType;
  uint32_t bfSize;
  uint16_t  bfReserved1;
  uint16_t  bfReserved2;
  uint32_t bfOffBits;
} __attribute__((__packed__))
BITMAPFILEHEADER;