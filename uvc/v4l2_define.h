#pragma once

namespace uvc {

/*
 * Error Codes
 */
#define E_OK (0)
#define E_ALLOC_ERR (-1)
#define E_QUERYCAP_ERR (-2)
#define E_READ_ERR (-3)
#define E_MMAP_ERR (-4)
#define E_QUERYBUF_ERR (-5)
#define E_QBUF_ERR (-6)
#define E_DQBUF_ERR (-7)
#define E_STREAMON_ERR (-8)
#define E_STREAMOFF_ERR (-9)
#define E_FORMAT_ERR (-10)
#define E_REQBUFS_ERR (-11)
#define E_DEVICE_ERR (-12)
#define E_SELECT_ERR (-13)
#define E_SELECT_TIMEOUT_ERR (-14)
#define E_FBALLOC_ERR (-15)
#define E_NO_STREAM_ERR (-16)
#define E_NO_DATA (-17)
#define E_NO_CODEC (-18)
#define E_DECODE_ERR (-19)
#define E_BAD_TABLES_ERR (-20)
#define E_NO_SOI_ERR (-21)
#define E_NOT_8BIT_ERR (-22)
#define E_BAD_WIDTH_OR_HEIGHT_ERR (-23)
#define E_TOO_MANY_COMPPS_ERR (-24)
#define E_ILLEGAL_HV_ERR (-25)
#define E_QUANT_TBL_SEL_ERR (-26)
#define E_NOT_YCBCR_ERR (-27)
#define E_UNKNOWN_CID_ERR (-28)
#define E_WRONG_MARKER_ERR (-29)
#define E_NO_EOI_ERR (-30)
#define E_FILE_IO_ERR (-31)
#define E_UNKNOWN_ERR (-40)

#ifndef TRUE
#define TRUE (1)
#endif

#ifndef FALSE
#define FALSE (0)
#endif

/*  Four-character-code (FOURCC) */
#define v4l2_fourcc(a, b, c, d) \
    ((__u32)(a) | ((__u32)(b) << 8) | ((__u32)(c) << 16) | ((__u32)(d) << 24))
#define v4l2_fourcc_be(a, b, c, d) (v4l2_fourcc(a, b, c, d) | (1U << 31))

/*      Pixel format         FOURCC                          depth  Description  */

/* RGB formats (1 or 2 bytes per pixel) */
#define V4L2_PIX_FMT_RGB332 v4l2_fourcc('R', 'G', 'B', '1')      /*  8  RGB-3-3-2     */
#define V4L2_PIX_FMT_RGB444 v4l2_fourcc('R', '4', '4', '4')      /* 16  xxxxrrrr ggggbbbb */
#define V4L2_PIX_FMT_ARGB444 v4l2_fourcc('A', 'R', '1', '2')     /* 16  aaaarrrr ggggbbbb */
#define V4L2_PIX_FMT_XRGB444 v4l2_fourcc('X', 'R', '1', '2')     /* 16  xxxxrrrr ggggbbbb */
#define V4L2_PIX_FMT_RGBA444 v4l2_fourcc('R', 'A', '1', '2')     /* 16  rrrrgggg bbbbaaaa */
#define V4L2_PIX_FMT_RGBX444 v4l2_fourcc('R', 'X', '1', '2')     /* 16  rrrrgggg bbbbxxxx */
#define V4L2_PIX_FMT_ABGR444 v4l2_fourcc('A', 'B', '1', '2')     /* 16  aaaabbbb ggggrrrr */
#define V4L2_PIX_FMT_XBGR444 v4l2_fourcc('X', 'B', '1', '2')     /* 16  xxxxbbbb ggggrrrr */
#define V4L2_PIX_FMT_BGRA444 v4l2_fourcc('G', 'A', '1', '2')     /* 16  bbbbgggg rrrraaaa */
#define V4L2_PIX_FMT_BGRX444 v4l2_fourcc('B', 'X', '1', '2')     /* 16  bbbbgggg rrrrxxxx */
#define V4L2_PIX_FMT_RGB555 v4l2_fourcc('R', 'G', 'B', 'O')      /* 16  RGB-5-5-5     */
#define V4L2_PIX_FMT_ARGB555 v4l2_fourcc('A', 'R', '1', '5')     /* 16  ARGB-1-5-5-5  */
#define V4L2_PIX_FMT_XRGB555 v4l2_fourcc('X', 'R', '1', '5')     /* 16  XRGB-1-5-5-5  */
#define V4L2_PIX_FMT_RGBA555 v4l2_fourcc('R', 'A', '1', '5')     /* 16  RGBA-5-5-5-1  */
#define V4L2_PIX_FMT_RGBX555 v4l2_fourcc('R', 'X', '1', '5')     /* 16  RGBX-5-5-5-1  */
#define V4L2_PIX_FMT_ABGR555 v4l2_fourcc('A', 'B', '1', '5')     /* 16  ABGR-1-5-5-5  */
#define V4L2_PIX_FMT_XBGR555 v4l2_fourcc('X', 'B', '1', '5')     /* 16  XBGR-1-5-5-5  */
#define V4L2_PIX_FMT_BGRA555 v4l2_fourcc('B', 'A', '1', '5')     /* 16  BGRA-5-5-5-1  */
#define V4L2_PIX_FMT_BGRX555 v4l2_fourcc('B', 'X', '1', '5')     /* 16  BGRX-5-5-5-1  */
#define V4L2_PIX_FMT_RGB565 v4l2_fourcc('R', 'G', 'B', 'P')      /* 16  RGB-5-6-5     */
#define V4L2_PIX_FMT_RGB555X v4l2_fourcc('R', 'G', 'B', 'Q')     /* 16  RGB-5-5-5 BE  */
#define V4L2_PIX_FMT_ARGB555X v4l2_fourcc_be('A', 'R', '1', '5') /* 16  ARGB-5-5-5 BE */
#define V4L2_PIX_FMT_XRGB555X v4l2_fourcc_be('X', 'R', '1', '5') /* 16  XRGB-5-5-5 BE */
#define V4L2_PIX_FMT_RGB565X v4l2_fourcc('R', 'G', 'B', 'R')     /* 16  RGB-5-6-5 BE  */

/* RGB formats (3 or 4 bytes per pixel) */
#define V4L2_PIX_FMT_BGR666 v4l2_fourcc('B', 'G', 'R', 'H') /* 18  BGR-6-6-6	  */
#define V4L2_PIX_FMT_BGR24 v4l2_fourcc('B', 'G', 'R', '3')  /* 24  BGR-8-8-8     */
#define V4L2_PIX_FMT_RGB24 v4l2_fourcc('R', 'G', 'B', '3')  /* 24  RGB-8-8-8     */
#define V4L2_PIX_FMT_BGR32 v4l2_fourcc('B', 'G', 'R', '4')  /* 32  BGR-8-8-8-8   */
#define V4L2_PIX_FMT_ABGR32 v4l2_fourcc('A', 'R', '2', '4') /* 32  BGRA-8-8-8-8  */
#define V4L2_PIX_FMT_XBGR32 v4l2_fourcc('X', 'R', '2', '4') /* 32  BGRX-8-8-8-8  */
#define V4L2_PIX_FMT_BGRA32 v4l2_fourcc('R', 'A', '2', '4') /* 32  ABGR-8-8-8-8  */
#define V4L2_PIX_FMT_BGRX32 v4l2_fourcc('R', 'X', '2', '4') /* 32  XBGR-8-8-8-8  */
#define V4L2_PIX_FMT_RGB32 v4l2_fourcc('R', 'G', 'B', '4')  /* 32  RGB-8-8-8-8   */
#define V4L2_PIX_FMT_RGBA32 v4l2_fourcc('A', 'B', '2', '4') /* 32  RGBA-8-8-8-8  */
#define V4L2_PIX_FMT_RGBX32 v4l2_fourcc('X', 'B', '2', '4') /* 32  RGBX-8-8-8-8  */
#define V4L2_PIX_FMT_ARGB32 v4l2_fourcc('B', 'A', '2', '4') /* 32  ARGB-8-8-8-8  */
#define V4L2_PIX_FMT_XRGB32 v4l2_fourcc('B', 'X', '2', '4') /* 32  XRGB-8-8-8-8  */

/* Grey formats */
#define V4L2_PIX_FMT_GREY v4l2_fourcc('G', 'R', 'E', 'Y')      /*  8  Greyscale     */
#define V4L2_PIX_FMT_Y4 v4l2_fourcc('Y', '0', '4', ' ')        /*  4  Greyscale     */
#define V4L2_PIX_FMT_Y6 v4l2_fourcc('Y', '0', '6', ' ')        /*  6  Greyscale     */
#define V4L2_PIX_FMT_Y10 v4l2_fourcc('Y', '1', '0', ' ')       /* 10  Greyscale     */
#define V4L2_PIX_FMT_Y12 v4l2_fourcc('Y', '1', '2', ' ')       /* 12  Greyscale     */
#define V4L2_PIX_FMT_Y14 v4l2_fourcc('Y', '1', '4', ' ')       /* 14  Greyscale     */
#define V4L2_PIX_FMT_Y16 v4l2_fourcc('Y', '1', '6', ' ')       /* 16  Greyscale     */
#define V4L2_PIX_FMT_Y16_BE v4l2_fourcc_be('Y', '1', '6', ' ') /* 16  Greyscale BE  */

/* Grey bit-packed formats */
#define V4L2_PIX_FMT_Y10BPACK v4l2_fourcc('Y', '1', '0', 'B') /* 10  Greyscale bit-packed */
#define V4L2_PIX_FMT_Y10P v4l2_fourcc('Y', '1', '0', 'P')     /* 10  Greyscale, MIPI RAW10 packed */

/* Palette formats */
#define V4L2_PIX_FMT_PAL8 v4l2_fourcc('P', 'A', 'L', '8') /*  8  8-bit palette */

/* Chrominance formats */
#define V4L2_PIX_FMT_UV8 v4l2_fourcc('U', 'V', '8', ' ') /*  8  UV 4:4 */

/* Luminance+Chrominance formats */
#define V4L2_PIX_FMT_YUYV v4l2_fourcc('Y', 'U', 'Y', 'V')   /* 16  YUV 4:2:2     */
#define V4L2_PIX_FMT_YYUV v4l2_fourcc('Y', 'Y', 'U', 'V')   /* 16  YUV 4:2:2     */
#define V4L2_PIX_FMT_YVYU v4l2_fourcc('Y', 'V', 'Y', 'U')   /* 16 YVU 4:2:2 */
#define V4L2_PIX_FMT_UYVY v4l2_fourcc('U', 'Y', 'V', 'Y')   /* 16  YUV 4:2:2     */
#define V4L2_PIX_FMT_VYUY v4l2_fourcc('V', 'Y', 'U', 'Y')   /* 16  YUV 4:2:2     */
#define V4L2_PIX_FMT_Y41P v4l2_fourcc('Y', '4', '1', 'P')   /* 12  YUV 4:1:1     */
#define V4L2_PIX_FMT_YUV444 v4l2_fourcc('Y', '4', '4', '4') /* 16  xxxxyyyy uuuuvvvv */
#define V4L2_PIX_FMT_YUV555 v4l2_fourcc('Y', 'U', 'V', 'O') /* 16  YUV-5-5-5     */
#define V4L2_PIX_FMT_YUV565 v4l2_fourcc('Y', 'U', 'V', 'P') /* 16  YUV-5-6-5     */
#define V4L2_PIX_FMT_YUV24 v4l2_fourcc('Y', 'U', 'V', '3')  /* 24  YUV-8-8-8     */
#define V4L2_PIX_FMT_YUV32 v4l2_fourcc('Y', 'U', 'V', '4')  /* 32  YUV-8-8-8-8   */
#define V4L2_PIX_FMT_AYUV32 v4l2_fourcc('A', 'Y', 'U', 'V') /* 32  AYUV-8-8-8-8  */
#define V4L2_PIX_FMT_XYUV32 v4l2_fourcc('X', 'Y', 'U', 'V') /* 32  XYUV-8-8-8-8  */
#define V4L2_PIX_FMT_VUYA32 v4l2_fourcc('V', 'U', 'Y', 'A') /* 32  VUYA-8-8-8-8  */
#define V4L2_PIX_FMT_VUYX32 v4l2_fourcc('V', 'U', 'Y', 'X') /* 32  VUYX-8-8-8-8  */
#define V4L2_PIX_FMT_M420 \
    v4l2_fourcc('M', '4', '2', '0') /* 12  YUV 4:2:0 2 lines y, 1 line uv interleaved */

/* two planes -- one Y, one Cr + Cb interleaved  */
#define V4L2_PIX_FMT_NV12 v4l2_fourcc('N', 'V', '1', '2') /* 12  Y/CbCr 4:2:0  */
#define V4L2_PIX_FMT_NV21 v4l2_fourcc('N', 'V', '2', '1') /* 12  Y/CrCb 4:2:0  */
#define V4L2_PIX_FMT_NV16 v4l2_fourcc('N', 'V', '1', '6') /* 16  Y/CbCr 4:2:2  */
#define V4L2_PIX_FMT_NV61 v4l2_fourcc('N', 'V', '6', '1') /* 16  Y/CrCb 4:2:2  */
#define V4L2_PIX_FMT_NV24 v4l2_fourcc('N', 'V', '2', '4') /* 24  Y/CbCr 4:4:4  */
#define V4L2_PIX_FMT_NV42 v4l2_fourcc('N', 'V', '4', '2') /* 24  Y/CrCb 4:4:4  */
#define V4L2_PIX_FMT_HM12 v4l2_fourcc('H', 'M', '1', '2') /*  8  YUV 4:2:0 16x16 macroblocks */

/* two non contiguous planes - one Y, one Cr + Cb interleaved  */
#define V4L2_PIX_FMT_NV12M v4l2_fourcc('N', 'M', '1', '2')  /* 12  Y/CbCr 4:2:0  */
#define V4L2_PIX_FMT_NV21M v4l2_fourcc('N', 'M', '2', '1')  /* 21  Y/CrCb 4:2:0  */
#define V4L2_PIX_FMT_NV16M v4l2_fourcc('N', 'M', '1', '6')  /* 16  Y/CbCr 4:2:2  */
#define V4L2_PIX_FMT_NV61M v4l2_fourcc('N', 'M', '6', '1')  /* 16  Y/CrCb 4:2:2  */
#define V4L2_PIX_FMT_NV12MT v4l2_fourcc('T', 'M', '1', '2') /* 12  Y/CbCr 4:2:0 64x32 macroblocks */
#define V4L2_PIX_FMT_NV12MT_16X16 \
    v4l2_fourcc('V', 'M', '1', '2') /* 12  Y/CbCr 4:2:0 16x16 macroblocks */

/* three planes - Y Cb, Cr */
#define V4L2_PIX_FMT_YUV410 v4l2_fourcc('Y', 'U', 'V', '9')  /*  9  YUV 4:1:0     */
#define V4L2_PIX_FMT_YVU410 v4l2_fourcc('Y', 'V', 'U', '9')  /*  9  YVU 4:1:0     */
#define V4L2_PIX_FMT_YUV411P v4l2_fourcc('4', '1', '1', 'P') /* 12  YVU411 planar */
#define V4L2_PIX_FMT_YUV420 v4l2_fourcc('Y', 'U', '1', '2')  /* 12  YUV 4:2:0     */
#define V4L2_PIX_FMT_YVU420 v4l2_fourcc('Y', 'V', '1', '2')  /* 12  YVU 4:2:0     */
#define V4L2_PIX_FMT_YUV422P v4l2_fourcc('4', '2', '2', 'P') /* 16  YVU422 planar */

/* three non contiguous planes - Y, Cb, Cr */
#define V4L2_PIX_FMT_YUV420M v4l2_fourcc('Y', 'M', '1', '2') /* 12  YUV420 planar */
#define V4L2_PIX_FMT_YVU420M v4l2_fourcc('Y', 'M', '2', '1') /* 12  YVU420 planar */
#define V4L2_PIX_FMT_YUV422M v4l2_fourcc('Y', 'M', '1', '6') /* 16  YUV422 planar */
#define V4L2_PIX_FMT_YVU422M v4l2_fourcc('Y', 'M', '6', '1') /* 16  YVU422 planar */
#define V4L2_PIX_FMT_YUV444M v4l2_fourcc('Y', 'M', '2', '4') /* 24  YUV444 planar */
#define V4L2_PIX_FMT_YVU444M v4l2_fourcc('Y', 'M', '4', '2') /* 24  YVU444 planar */

/* Bayer formats - see http://www.siliconimaging.com/RGB%20Bayer.htm */
#define V4L2_PIX_FMT_SBGGR8 v4l2_fourcc('B', 'A', '8', '1')  /*  8  BGBG.. GRGR.. */
#define V4L2_PIX_FMT_SGBRG8 v4l2_fourcc('G', 'B', 'R', 'G')  /*  8  GBGB.. RGRG.. */
#define V4L2_PIX_FMT_SGRBG8 v4l2_fourcc('G', 'R', 'B', 'G')  /*  8  GRGR.. BGBG.. */
#define V4L2_PIX_FMT_SRGGB8 v4l2_fourcc('R', 'G', 'G', 'B')  /*  8  RGRG.. GBGB.. */
#define V4L2_PIX_FMT_SBGGR10 v4l2_fourcc('B', 'G', '1', '0') /* 10  BGBG.. GRGR.. */
#define V4L2_PIX_FMT_SGBRG10 v4l2_fourcc('G', 'B', '1', '0') /* 10  GBGB.. RGRG.. */
#define V4L2_PIX_FMT_SGRBG10 v4l2_fourcc('B', 'A', '1', '0') /* 10  GRGR.. BGBG.. */
#define V4L2_PIX_FMT_SRGGB10 v4l2_fourcc('R', 'G', '1', '0') /* 10  RGRG.. GBGB.. */
/* 10bit raw bayer packed, 5 bytes for every 4 pixels */
#define V4L2_PIX_FMT_SBGGR10P v4l2_fourcc('p', 'B', 'A', 'A')
#define V4L2_PIX_FMT_SGBRG10P v4l2_fourcc('p', 'G', 'A', 'A')
#define V4L2_PIX_FMT_SGRBG10P v4l2_fourcc('p', 'g', 'A', 'A')
#define V4L2_PIX_FMT_SRGGB10P v4l2_fourcc('p', 'R', 'A', 'A')
/* 10bit raw bayer a-law compressed to 8 bits */
#define V4L2_PIX_FMT_SBGGR10ALAW8 v4l2_fourcc('a', 'B', 'A', '8')
#define V4L2_PIX_FMT_SGBRG10ALAW8 v4l2_fourcc('a', 'G', 'A', '8')
#define V4L2_PIX_FMT_SGRBG10ALAW8 v4l2_fourcc('a', 'g', 'A', '8')
#define V4L2_PIX_FMT_SRGGB10ALAW8 v4l2_fourcc('a', 'R', 'A', '8')
/* 10bit raw bayer DPCM compressed to 8 bits */
#define V4L2_PIX_FMT_SBGGR10DPCM8 v4l2_fourcc('b', 'B', 'A', '8')
#define V4L2_PIX_FMT_SGBRG10DPCM8 v4l2_fourcc('b', 'G', 'A', '8')
#define V4L2_PIX_FMT_SGRBG10DPCM8 v4l2_fourcc('B', 'D', '1', '0')
#define V4L2_PIX_FMT_SRGGB10DPCM8 v4l2_fourcc('b', 'R', 'A', '8')
#define V4L2_PIX_FMT_SBGGR12 v4l2_fourcc('B', 'G', '1', '2') /* 12  BGBG.. GRGR.. */
#define V4L2_PIX_FMT_SGBRG12 v4l2_fourcc('G', 'B', '1', '2') /* 12  GBGB.. RGRG.. */
#define V4L2_PIX_FMT_SGRBG12 v4l2_fourcc('B', 'A', '1', '2') /* 12  GRGR.. BGBG.. */
#define V4L2_PIX_FMT_SRGGB12 v4l2_fourcc('R', 'G', '1', '2') /* 12  RGRG.. GBGB.. */
/* 12bit raw bayer packed, 6 bytes for every 4 pixels */
#define V4L2_PIX_FMT_SBGGR12P v4l2_fourcc('p', 'B', 'C', 'C')
#define V4L2_PIX_FMT_SGBRG12P v4l2_fourcc('p', 'G', 'C', 'C')
#define V4L2_PIX_FMT_SGRBG12P v4l2_fourcc('p', 'g', 'C', 'C')
#define V4L2_PIX_FMT_SRGGB12P v4l2_fourcc('p', 'R', 'C', 'C')
#define V4L2_PIX_FMT_SBGGR14 v4l2_fourcc('B', 'G', '1', '4') /* 14  BGBG.. GRGR.. */
#define V4L2_PIX_FMT_SGBRG14 v4l2_fourcc('G', 'B', '1', '4') /* 14  GBGB.. RGRG.. */
#define V4L2_PIX_FMT_SGRBG14 v4l2_fourcc('G', 'R', '1', '4') /* 14  GRGR.. BGBG.. */
#define V4L2_PIX_FMT_SRGGB14 v4l2_fourcc('R', 'G', '1', '4') /* 14  RGRG.. GBGB.. */
/* 14bit raw bayer packed, 7 bytes for every 4 pixels */
#define V4L2_PIX_FMT_SBGGR14P v4l2_fourcc('p', 'B', 'E', 'E')
#define V4L2_PIX_FMT_SGBRG14P v4l2_fourcc('p', 'G', 'E', 'E')
#define V4L2_PIX_FMT_SGRBG14P v4l2_fourcc('p', 'g', 'E', 'E')
#define V4L2_PIX_FMT_SRGGB14P v4l2_fourcc('p', 'R', 'E', 'E')
#define V4L2_PIX_FMT_SBGGR16 v4l2_fourcc('B', 'Y', 'R', '2') /* 16  BGBG.. GRGR.. */
#define V4L2_PIX_FMT_SGBRG16 v4l2_fourcc('G', 'B', '1', '6') /* 16  GBGB.. RGRG.. */
#define V4L2_PIX_FMT_SGRBG16 v4l2_fourcc('G', 'R', '1', '6') /* 16  GRGR.. BGBG.. */
#define V4L2_PIX_FMT_SRGGB16 v4l2_fourcc('R', 'G', '1', '6') /* 16  RGRG.. GBGB.. */

/* HSV formats */
#define V4L2_PIX_FMT_HSV24 v4l2_fourcc('H', 'S', 'V', '3')
#define V4L2_PIX_FMT_HSV32 v4l2_fourcc('H', 'S', 'V', '4')

/* compressed formats */
#define V4L2_PIX_FMT_MJPEG v4l2_fourcc('M', 'J', 'P', 'G')       /* Motion-JPEG   */
#define V4L2_PIX_FMT_JPEG v4l2_fourcc('J', 'P', 'E', 'G')        /* JFIF JPEG     */
#define V4L2_PIX_FMT_DV v4l2_fourcc('d', 'v', 's', 'd')          /* 1394          */
#define V4L2_PIX_FMT_MPEG v4l2_fourcc('M', 'P', 'E', 'G')        /* MPEG-1/2/4 Multiplexed */
#define V4L2_PIX_FMT_H264 v4l2_fourcc('H', '2', '6', '4')        /* H264 with start codes */
#define V4L2_PIX_FMT_H264_NO_SC v4l2_fourcc('A', 'V', 'C', '1')  /* H264 without start codes */
#define V4L2_PIX_FMT_H264_MVC v4l2_fourcc('M', '2', '6', '4')    /* H264 MVC */
#define V4L2_PIX_FMT_H263 v4l2_fourcc('H', '2', '6', '3')        /* H263          */
#define V4L2_PIX_FMT_MPEG1 v4l2_fourcc('M', 'P', 'G', '1')       /* MPEG-1 ES     */
#define V4L2_PIX_FMT_MPEG2 v4l2_fourcc('M', 'P', 'G', '2')       /* MPEG-2 ES     */
#define V4L2_PIX_FMT_MPEG2_SLICE v4l2_fourcc('M', 'G', '2', 'S') /* MPEG-2 parsed slice data */
#define V4L2_PIX_FMT_MPEG4 v4l2_fourcc('M', 'P', 'G', '4')       /* MPEG-4 part 2 ES */
#define V4L2_PIX_FMT_XVID v4l2_fourcc('X', 'V', 'I', 'D')        /* Xvid           */
#define V4L2_PIX_FMT_VC1_ANNEX_G \
    v4l2_fourcc('V', 'C', '1', 'G') /* SMPTE 421M Annex G compliant stream */
#define V4L2_PIX_FMT_VC1_ANNEX_L \
    v4l2_fourcc('V', 'C', '1', 'L')                      /* SMPTE 421M Annex L compliant stream */
#define V4L2_PIX_FMT_VP8 v4l2_fourcc('V', 'P', '8', '0') /* VP8 */
#define V4L2_PIX_FMT_VP8_FRAME v4l2_fourcc('V', 'P', '8', 'F') /* VP8 parsed frame */
#define V4L2_PIX_FMT_VP9 v4l2_fourcc('V', 'P', '9', '0')       /* VP9 */
#define V4L2_PIX_FMT_HEVC v4l2_fourcc('H', 'E', 'V', 'C')      /* HEVC aka H.265 */
#define V4L2_PIX_FMT_FWHT \
    v4l2_fourcc('F', 'W', 'H', 'T') /* Fast Walsh Hadamard Transform (vicodec) */
#define V4L2_PIX_FMT_FWHT_STATELESS v4l2_fourcc('S', 'F', 'W', 'H') /* Stateless FWHT (vicodec) */
#define V4L2_PIX_FMT_H264_SLICE v4l2_fourcc('S', '2', '6', '4')     /* H264 parsed slices */

/*  Vendor-specific formats   */
#define V4L2_PIX_FMT_CPIA1 v4l2_fourcc('C', 'P', 'I', 'A')        /* cpia1 YUV */
#define V4L2_PIX_FMT_WNVA v4l2_fourcc('W', 'N', 'V', 'A')         /* Winnov hw compress */
#define V4L2_PIX_FMT_SN9C10X v4l2_fourcc('S', '9', '1', '0')      /* SN9C10x compression */
#define V4L2_PIX_FMT_SN9C20X_I420 v4l2_fourcc('S', '9', '2', '0') /* SN9C20x YUV 4:2:0 */
#define V4L2_PIX_FMT_PWC1 v4l2_fourcc('P', 'W', 'C', '1')         /* pwc older webcam */
#define V4L2_PIX_FMT_PWC2 v4l2_fourcc('P', 'W', 'C', '2')         /* pwc newer webcam */
#define V4L2_PIX_FMT_ET61X251 v4l2_fourcc('E', '6', '2', '5')     /* ET61X251 compression */
#define V4L2_PIX_FMT_SPCA501 v4l2_fourcc('S', '5', '0', '1')      /* YUYV per line */
#define V4L2_PIX_FMT_SPCA505 v4l2_fourcc('S', '5', '0', '5')      /* YYUV per line */
#define V4L2_PIX_FMT_SPCA508 v4l2_fourcc('S', '5', '0', '8')      /* YUVY per line */
#define V4L2_PIX_FMT_SPCA561 v4l2_fourcc('S', '5', '6', '1')      /* compressed GBRG bayer */
#define V4L2_PIX_FMT_PAC207 v4l2_fourcc('P', '2', '0', '7')       /* compressed BGGR bayer */
#define V4L2_PIX_FMT_MR97310A v4l2_fourcc('M', '3', '1', '0')     /* compressed BGGR bayer */
#define V4L2_PIX_FMT_JL2005BCD v4l2_fourcc('J', 'L', '2', '0')    /* compressed RGGB bayer */
#define V4L2_PIX_FMT_SN9C2028 v4l2_fourcc('S', 'O', 'N', 'X')     /* compressed GBRG bayer */
#define V4L2_PIX_FMT_SQ905C v4l2_fourcc('9', '0', '5', 'C')       /* compressed RGGB bayer */
#define V4L2_PIX_FMT_PJPG v4l2_fourcc('P', 'J', 'P', 'G')         /* Pixart 73xx JPEG */
#define V4L2_PIX_FMT_OV511 v4l2_fourcc('O', '5', '1', '1')        /* ov511 JPEG */
#define V4L2_PIX_FMT_OV518 v4l2_fourcc('O', '5', '1', '8')        /* ov518 JPEG */
#define V4L2_PIX_FMT_STV0680 v4l2_fourcc('S', '6', '8', '0')      /* stv0680 bayer */
#define V4L2_PIX_FMT_TM6000 v4l2_fourcc('T', 'M', '6', '0')       /* tm5600/tm60x0 */
#define V4L2_PIX_FMT_CIT_YYVYUY \
    v4l2_fourcc('C', 'I', 'T', 'V') /* one line of Y then 1 line of VYUY */
#define V4L2_PIX_FMT_KONICA420 \
    v4l2_fourcc('K', 'O', 'N', 'I') /* YUV420 planar in blocks of 256 pixels */
#define V4L2_PIX_FMT_JPGL v4l2_fourcc('J', 'P', 'G', 'L')  /* JPEG-Lite */
#define V4L2_PIX_FMT_SE401 v4l2_fourcc('S', '4', '0', '1') /* se401 janggu compressed rgb */
#define V4L2_PIX_FMT_S5C_UYVY_JPG \
    v4l2_fourcc('S', '5', 'C', 'I')                        /* S5C73M3 interleaved UYVY/JPEG */
#define V4L2_PIX_FMT_Y8I v4l2_fourcc('Y', '8', 'I', ' ')   /* Greyscale 8-bit L/R interleaved */
#define V4L2_PIX_FMT_Y12I v4l2_fourcc('Y', '1', '2', 'I')  /* Greyscale 12-bit L/R interleaved */
#define V4L2_PIX_FMT_Z16 v4l2_fourcc('Z', '1', '6', ' ')   /* Depth data 16-bit */
#define V4L2_PIX_FMT_MT21C v4l2_fourcc('M', 'T', '2', '1') /* Mediatek compressed block mode  */
#define V4L2_PIX_FMT_INZI \
    v4l2_fourcc('I', 'N', 'Z', 'I') /* Intel Planar Greyscale 10-bit and Depth 16-bit */
#define V4L2_PIX_FMT_SUNXI_TILED_NV12 v4l2_fourcc('S', 'T', '1', '2') /* Sunxi Tiled NV12 Format */
#define V4L2_PIX_FMT_CNF4 \
    v4l2_fourcc('C', 'N', 'F', '4') /* Intel 4-bit packed depth confidence information */
#define V4L2_PIX_FMT_HI240 v4l2_fourcc('H', 'I', '2', '4') /* BTTV 8-bit dithered RGB */

/* 10bit raw bayer packed, 32 bytes for every 25 pixels, last LSB 6 bits unused */
#define V4L2_PIX_FMT_IPU3_SBGGR10 \
    v4l2_fourcc('i', 'p', '3', 'b') /* IPU3 packed 10-bit BGGR bayer */
#define V4L2_PIX_FMT_IPU3_SGBRG10 \
    v4l2_fourcc('i', 'p', '3', 'g') /* IPU3 packed 10-bit GBRG bayer */
#define V4L2_PIX_FMT_IPU3_SGRBG10 \
    v4l2_fourcc('i', 'p', '3', 'G') /* IPU3 packed 10-bit GRBG bayer */
#define V4L2_PIX_FMT_IPU3_SRGGB10 \
    v4l2_fourcc('i', 'p', '3', 'r') /* IPU3 packed 10-bit RGGB bayer */

/* SDR formats - used only for Software Defined Radio devices */
#define V4L2_SDR_FMT_CU8 v4l2_fourcc('C', 'U', '0', '8')     /* IQ u8 */
#define V4L2_SDR_FMT_CU16LE v4l2_fourcc('C', 'U', '1', '6')  /* IQ u16le */
#define V4L2_SDR_FMT_CS8 v4l2_fourcc('C', 'S', '0', '8')     /* complex s8 */
#define V4L2_SDR_FMT_CS14LE v4l2_fourcc('C', 'S', '1', '4')  /* complex s14le */
#define V4L2_SDR_FMT_RU12LE v4l2_fourcc('R', 'U', '1', '2')  /* real u12le */
#define V4L2_SDR_FMT_PCU16BE v4l2_fourcc('P', 'C', '1', '6') /* planar complex u16be */
#define V4L2_SDR_FMT_PCU18BE v4l2_fourcc('P', 'C', '1', '8') /* planar complex u18be */
#define V4L2_SDR_FMT_PCU20BE v4l2_fourcc('P', 'C', '2', '0') /* planar complex u20be */

/* Touch formats - used for Touch devices */
#define V4L2_TCH_FMT_DELTA_TD16 v4l2_fourcc('T', 'D', '1', '6') /* 16-bit signed deltas */
#define V4L2_TCH_FMT_DELTA_TD08 v4l2_fourcc('T', 'D', '0', '8') /* 8-bit signed deltas */
#define V4L2_TCH_FMT_TU16 v4l2_fourcc('T', 'U', '1', '6')       /* 16-bit unsigned touch data */
#define V4L2_TCH_FMT_TU08 v4l2_fourcc('T', 'U', '0', '8')       /* 8-bit unsigned touch data */

/* Meta-data formats */
#define V4L2_META_FMT_VSP1_HGO v4l2_fourcc('V', 'S', 'P', 'H') /* R-Car VSP1 1-D Histogram */
#define V4L2_META_FMT_VSP1_HGT v4l2_fourcc('V', 'S', 'P', 'T') /* R-Car VSP1 2-D Histogram */
#define V4L2_META_FMT_UVC v4l2_fourcc('U', 'V', 'C', 'H')      /* UVC Payload Header metadata */
#define V4L2_META_FMT_D4XX v4l2_fourcc('D', '4', 'X', 'X')     /* D4XX Payload Header metadata */
#define V4L2_META_FMT_VIVID v4l2_fourcc('V', 'I', 'V', 'D')    /* Vivid Metadata */

/* Vendor specific - used for RK_ISP1 camera sub-system */
#define V4L2_META_FMT_RK_ISP1_PARAMS \
    v4l2_fourcc('R', 'K', '1', 'P') /* Rockchip ISP1 3A Parameters */
#define V4L2_META_FMT_RK_ISP1_STAT_3A \
    v4l2_fourcc('R', 'K', '1', 'S') /* Rockchip ISP1 3A Statistics */

}  // namespace uvc