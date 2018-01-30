#pragma once
#include <vector>

struct hit
{
	unsigned int fec;
	unsigned int chip_id;
	unsigned int framecounter;
	unsigned int srs_timestamp;
	unsigned int channel;
	unsigned int bcid;
	unsigned int tdc;
	unsigned int adc;
	unsigned int overthreshold;
};



std::vector<hit> Run16_1_to_16
{
{ 1, 1, 255776505, 181469184, 58, 3543, 183, 170, 1 },
{ 1, 1, 255776505, 181469184, 59, 3543, 137, 498, 1 },
{ 1, 1, 255776505, 181469184, 60, 3543, 155, 864, 1 },
{ 1, 1, 255776505, 181469184, 61, 3543, 137, 560, 1 },
{ 1, 1, 255776505, 181469184, 62, 3543, 193, 176, 1 },
{ 1, 1, 255776505, 181469184, 63, 3543, 227, 0, 1 },
{ 1, 1, 255776505, 181469184, 33, 3940, 231, 104, 1 },
{ 1, 1, 255776505, 181469184, 30, 3943, 168, 104, 1 },
{ 1, 1, 255776505, 181469184, 31, 3942, 100, 164, 1 },
{ 1, 1, 255776505, 181469184, 32, 3942, 152, 169, 1 },
{ 1, 1, 255776505, 181469184, 35, 606, 144, 104, 1 },
{ 1, 1, 255776505, 181469184, 38, 606, 208, 154, 1 },
{ 1, 1, 255776505, 181469184, 39, 607, 167, 327, 1 },
{ 1, 1, 255776505, 181469184, 40, 607, 168, 505, 1 },
{ 1, 1, 255776505, 181469184, 41, 607, 161, 410, 1 },
{ 1, 1, 255776505, 181469184, 42, 607, 136, 320, 1 },
{ 1, 1, 255776505, 181469184, 43, 607, 96, 256, 1 },
{ 1, 1, 255776505, 181469184, 29, 607, 161, 232, 1 },
{ 1, 1, 255776505, 181469184, 30, 607, 155, 336, 1 },
{ 1, 1, 255776505, 181469184, 31, 607, 152, 178, 1 },
{ 1, 1, 255776505, 181469184, 28, 921, 185, 95, 1 },
{ 1, 1, 255776505, 181469184, 29, 921, 180, 112, 1 },
{ 1, 6, 255776506, 181469184, 13, 843, 123, 96, 1 },
{ 1, 6, 255776506, 181469184, 15, 846, 184, 208, 1 },
{ 1, 6, 255776506, 181469184, 16, 846, 137, 182, 1 },
{ 1, 6, 255776506, 181469184, 17, 849, 189, 338, 1 },
{ 1, 6, 255776506, 181469184, 18, 849, 217, 190, 1 },
{ 1, 6, 255776506, 181469184, 10, 847, 177, 138, 1 },
{ 1, 7, 255776507, 181469184, 32, 3651, 209, 132, 1 },
{ 1, 7, 255776507, 181469184, 33, 3651, 200, 368, 1 },
{ 1, 7, 255776507, 181469184, 34, 3651, 176, 256, 1 },
{ 1, 11, 255776509, 181469184, 63, 607, 103, 119, 1 },
{ 1, 14, 255776510, 181469184, 21, 2116, 102, 143, 1 },
{ 1, 14, 255776510, 181469184, 35, 3234, 96, 89, 1 },
{ 1, 14, 255776510, 181469184, 36, 3235, 207, 74, 1 },
{ 1, 14, 255776510, 181469184, 31, 3236, 96, 79, 1 },
{ 1, 14, 255776510, 181469184, 18, 3542, 80, 96, 1 },
{ 1, 14, 255776510, 181469184, 19, 3543, 136, 249, 1 },
{ 1, 14, 255776510, 181469184, 20, 3543, 128, 880, 1 },
{ 1, 14, 255776510, 181469184, 21, 3543, 120, 1023, 1 },
{ 1, 14, 255776510, 181469184, 22, 3543, 120, 520, 1 },
{ 1, 14, 255776510, 181469184, 23, 3543, 182, 132, 1 },
{ 1, 14, 255776510, 181469184, 24, 3543, 181, 73, 1 },
{ 1, 14, 255776510, 181469184, 34, 3650, 96, 93, 1 },
{ 1, 14, 255776510, 181469184, 35, 3651, 185, 388, 1 },
{ 1, 14, 255776510, 181469184, 36, 3651, 193, 347, 1 },
{ 1, 14, 255776510, 181469184, 37, 3650, 135, 96, 1 },
{ 1, 14, 255776510, 181469184, 8, 3940, 120, 142, 1 },
{ 1, 14, 255776510, 181469184, 9, 3941, 58, 114, 1 },
{ 1, 14, 255776510, 181469184, 10, 3942, 83, 129, 1 },
{ 1, 14, 255776510, 181469184, 11, 3942, 56, 166, 1 },
{ 1, 14, 255776510, 181469184, 12, 3943, 100, 194, 1 },
{ 1, 14, 255776510, 181469184, 13, 3943, 75, 80, 1 },
{ 1, 14, 255776510, 181469184, 0, 607, 80, 192, 1 },
{ 1, 14, 255776510, 181469184, 1, 607, 122, 241, 1 },
{ 1, 14, 255776510, 181469184, 2, 606, 58, 192, 1 },
{ 1, 14, 255776510, 181469184, 3, 606, 50, 191, 1 },
{ 1, 14, 255776510, 181469184, 4, 606, 60, 272, 1 },
{ 1, 14, 255776510, 181469184, 5, 607, 96, 379, 1 },
{ 1, 14, 255776510, 181469184, 6, 605, 72, 324, 1 },
{ 1, 14, 255776510, 181469184, 7, 606, 90, 352, 1 },
{ 1, 14, 255776510, 181469184, 8, 606, 122, 171, 1 },
{ 1, 14, 255776510, 181469184, 9, 607, 80, 118, 1 },
{ 1, 14, 255776510, 181469184, 30, 843, 128, 123, 1 },
{ 1, 14, 255776510, 181469184, 38, 847, 192, 80, 1 },
{ 1, 14, 255776510, 181469184, 57, 843, 144, 111, 1 },
{ 1, 14, 255776510, 181469184, 21, 848, 124, 130, 1 },
{ 1, 14, 255776510, 181469184, 22, 848, 114, 128, 1 },
{ 1, 14, 255776510, 181469184, 23, 849, 176, 384, 1 },
{ 1, 14, 255776510, 181469184, 24, 846, 115, 127, 1 },
{ 1, 14, 255776510, 181469184, 25, 846, 160, 278, 1 },
{ 1, 14, 255776510, 181469184, 26, 846, 208, 152, 1 },
{ 1, 14, 255776510, 181469184, 27, 921, 123, 107, 1 },
{ 1, 14, 255776510, 181469184, 28, 921, 196, 125, 1 },
{ 1, 14, 255776510, 181469184, 29, 919, 136, 112, 1 },
{ 1, 14, 255776510, 181469184, 30, 918, 92, 69, 1 },
{ 1, 14, 255776510, 181469184, 23, 924, 103, 64, 1 },
{ 1, 14, 255776510, 181469184, 24, 924, 128, 103, 1 },
{ 1, 15, 255776511, 181469184, 11, 2161, 210, 102, 1 },
{ 1, 15, 255776511, 181469184, 30, 844, 145, 125, 1 },
{ 1, 15, 255776511, 181469184, 28, 847, 111, 64, 1 },
{ 1, 15, 255776511, 181469184, 29, 847, 179, 87, 1 },
{ 1, 0, 255776512, 181534720, 46, 2350, 130, 135, 1 },
{ 1, 0, 255776512, 181534720, 47, 2351, 214, 550, 1 },
{ 1, 0, 255776512, 181534720, 48, 2351, 216, 881, 1 },
{ 1, 0, 255776512, 181534720, 49, 2351, 140, 573, 1 },
{ 1, 0, 255776512, 181534720, 50, 2352, 212, 224, 1 },
{ 1, 0, 255776512, 181534720, 51, 2353, 202, 90, 1 },
{ 1, 0, 255776512, 181534720, 52, 2353, 144, 215, 1 },
{ 1, 0, 255776512, 181534720, 53, 2354, 192, 352, 1 },
{ 1, 0, 255776512, 181534720, 54, 2354, 184, 189, 1 },
{ 1, 0, 255776512, 181534720, 59, 1513, 92, 160, 1 },
{ 1, 0, 255776512, 181534720, 60, 1513, 91, 438, 1 },
{ 1, 0, 255776512, 181534720, 61, 1513, 103, 340, 1 },
{ 1, 0, 255776512, 181534720, 62, 1513, 138, 166, 1 },
{ 1, 0, 255776512, 181534720, 63, 1512, 65, 86, 1 },
{ 1, 0, 255776512, 181534720, 52, 1988, 194, 113, 1 },
{ 1, 0, 255776512, 181534720, 42, 1990, 217, 74, 1 },
{ 1, 1, 255776513, 181534720, 34, 294, 138, 128, 1 },
{ 1, 1, 255776513, 181534720, 35, 294, 105, 352, 1 },
{ 1, 1, 255776513, 181534720, 36, 294, 119, 304, 1 },
{ 1, 1, 255776513, 181534720, 37, 294, 112, 100, 1 },
{ 1, 1, 255776513, 181534720, 42, 294, 110, 112, 1 },
{ 1, 1, 255776513, 181534720, 21, 1224, 113, 221, 1 },
{ 1, 1, 255776513, 181534720, 22, 1223, 85, 302, 1 },
{ 1, 1, 255776513, 181534720, 23, 1222, 80, 432, 1 },
{ 1, 1, 255776513, 181534720, 24, 1222, 105, 368, 1 },
{ 1, 1, 255776513, 181534720, 25, 1222, 91, 628, 1 },
{ 1, 1, 255776513, 181534720, 26, 1222, 118, 376, 1 },
{ 1, 1, 255776513, 181534720, 27, 1223, 210, 321, 1 },
{ 1, 1, 255776513, 181534720, 28, 1223, 201, 816, 1 },
{ 1, 1, 255776513, 181534720, 29, 1223, 228, 755, 1 },
{ 1, 1, 255776513, 181534720, 30, 1222, 135, 208, 1 },
{ 1, 1, 255776513, 181534720, 62, 1301, 160, 96, 1 },
{ 1, 1, 255776513, 181534720, 2, 1510, 105, 274, 1 },
{ 1, 1, 255776513, 181534720, 1, 1510, 104, 116, 1 },
{ 1, 1, 255776513, 181534720, 17, 1985, 65, 170, 1 },
{ 1, 1, 255776513, 181534720, 18, 1985, 96, 135, 1 },
{ 1, 1, 255776513, 181534720, 39, 1988, 144, 96, 1 },
{ 1, 6, 255776514, 181534720, 19, 148, 155, 143, 1 },
{ 1, 6, 255776514, 181534720, 20, 148, 169, 168, 1 },
{ 1, 6, 255776514, 181534720, 21, 148, 176, 160, 1 },
{ 1, 6, 255776514, 181534720, 8, 151, 99, 104, 1 },
{ 1, 6, 255776514, 181534720, 9, 151, 116, 111, 1 },
{ 1, 6, 255776514, 181534720, 10, 151, 114, 139, 1 },
{ 1, 6, 255776514, 181534720, 13, 150, 91, 128, 1 },
{ 1, 6, 255776514, 181534720, 14, 150, 112, 144, 1 },
{ 1, 7, 255776515, 181534720, 31, 699, 88, 121, 1 },
{ 1, 7, 255776515, 181534720, 32, 700, 202, 214, 1 },
{ 1, 7, 255776515, 181534720, 33, 700, 192, 186, 1 },
{ 1, 7, 255776515, 181534720, 47, 705, 136, 137, 1 },
{ 1, 7, 255776515, 181534720, 48, 705, 111, 209, 1 },
{ 1, 10, 255776516, 181534720, 32, 705, 99, 244, 1 },
{ 1, 10, 255776516, 181534720, 26, 1509, 110, 205, 1 },
{ 1, 10, 255776516, 181534720, 19, 1513, 160, 92, 1 },
{ 1, 10, 255776516, 181534720, 20, 1513, 118, 459, 1 },
{ 1, 10, 255776516, 181534720, 21, 1513, 128, 611, 1 },
{ 1, 10, 255776516, 181534720, 22, 1513, 161, 217, 1 },
{ 1, 10, 255776516, 181534720, 23, 1512, 76, 113, 1 },
{ 1, 10, 255776516, 181534720, 24, 1510, 116, 144, 1 },
{ 1, 10, 255776516, 181534720, 25, 1510, 145, 374, 1 },
{ 1, 11, 255776517, 181534720, 63, 151, 139, 83, 1 },
{ 1, 11, 255776517, 181534720, 5, 699, 20, 102, 1 },
{ 1, 11, 255776517, 181534720, 6, 699, 22, 154, 1 },
{ 1, 11, 255776517, 181534720, 7, 699, 27, 153, 1 },
{ 1, 11, 255776517, 181534720, 8, 699, 30, 118, 1 },
{ 1, 11, 255776517, 181534720, 17, 700, 53, 112, 1 },
{ 1, 11, 255776517, 181534720, 54, 703, 200, 94, 1 },
{ 1, 11, 255776517, 181534720, 3, 700, 51, 129, 1 },
{ 1, 14, 255776518, 181534720, 22, 2351, 129, 138, 1 },
{ 1, 14, 255776518, 181534720, 23, 2351, 153, 143, 1 },
{ 1, 14, 255776518, 181534720, 24, 2351, 183, 129, 1 },
{ 1, 14, 255776518, 181534720, 25, 2351, 176, 512, 1 },
{ 1, 14, 255776518, 181534720, 26, 2351, 193, 1023, 1 },
{ 1, 14, 255776518, 181534720, 27, 2351, 186, 852, 1 },
{ 1, 14, 255776518, 181534720, 28, 2350, 141, 183, 1 },
{ 1, 14, 255776518, 181534720, 29, 2350, 132, 80, 1 },
{ 1, 14, 255776518, 181534720, 11, 2357, 56, 88, 1 },
{ 1, 14, 255776518, 181534720, 12, 2355, 81, 107, 1 },
{ 1, 14, 255776518, 181534720, 13, 2356, 52, 317, 1 },
{ 1, 14, 255776518, 181534720, 15, 2354, 128, 117, 1 },
{ 1, 14, 255776518, 181534720, 16, 2354, 198, 198, 1 },
{ 1, 14, 255776518, 181534720, 17, 2353, 104, 338, 1 },
{ 1, 14, 255776518, 181534720, 18, 2353, 128, 328, 1 },
{ 1, 14, 255776518, 181534720, 19, 2352, 80, 185, 1 },
{ 1, 14, 255776518, 181534720, 20, 2352, 154, 134, 1 },
{ 1, 14, 255776518, 181534720, 21, 2351, 88, 189, 1 },
{ 1, 14, 255776518, 181534720, 2, 149, 127, 173, 1 },
{ 1, 14, 255776518, 181534720, 3, 148, 96, 263, 1 },
{ 1, 14, 255776518, 181534720, 4, 147, 67, 205, 1 },
{ 1, 14, 255776518, 181534720, 13, 154, 89, 128, 1 },
{ 1, 14, 255776518, 181534720, 15, 154, 136, 80, 1 },
{ 1, 14, 255776518, 181534720, 0, 150, 62, 210, 1 },
{ 1, 14, 255776518, 181534720, 1, 150, 71, 266, 1 },
{ 1, 14, 255776518, 181534720, 32, 294, 113, 96, 1 },
{ 1, 14, 255776518, 181534720, 33, 294, 131, 144, 1 },
{ 1, 14, 255776518, 181534720, 34, 294, 160, 92, 1 },
{ 1, 14, 255776518, 181534720, 35, 294, 131, 155, 1 },
{ 1, 14, 255776518, 181534720, 36, 294, 120, 400, 1 },
{ 1, 14, 255776518, 181534720, 37, 294, 128, 354, 1 },
{ 1, 14, 255776518, 181534720, 38, 294, 163, 103, 1 },
{ 1, 14, 255776518, 181534720, 21, 1142, 114, 117, 1 },
{ 1, 14, 255776518, 181534720, 23, 1152, 184, 66, 1 },
{ 1, 14, 255776518, 181534720, 24, 1151, 83, 64, 1 },
{ 1, 14, 255776518, 181534720, 17, 1223, 147, 145, 1 },
{ 1, 14, 255776518, 181534720, 18, 1222, 88, 143, 1 },
{ 1, 14, 255776518, 181534720, 19, 1222, 121, 414, 1 },
{ 1, 14, 255776518, 181534720, 20, 1222, 153, 544, 1 },
{ 1, 14, 255776518, 181534720, 21, 1222, 144, 491, 1 },
{ 1, 14, 255776518, 181534720, 22, 1222, 100, 944, 1 },
{ 1, 14, 255776518, 181534720, 23, 1223, 147, 1023, 1 },
{ 1, 14, 255776518, 181534720, 24, 1222, 86, 494, 1 },
{ 1, 14, 255776518, 181534720, 25, 1223, 168, 268, 1 },
{ 1, 14, 255776518, 181534720, 26, 1223, 176, 208, 1 },
{ 1, 14, 255776518, 181534720, 27, 1301, 176, 101, 1 },
{ 1, 14, 255776518, 181534720, 28, 1301, 185, 82, 1 },
{ 1, 14, 255776518, 181534720, 10, 1225, 67, 153, 1 },
{ 1, 14, 255776518, 181534720, 11, 1225, 104, 103, 1 },
{ 1, 14, 255776518, 181534720, 13, 1223, 48, 122, 1 },
{ 1, 14, 255776518, 181534720, 15, 1223, 98, 80, 1 },
{ 1, 14, 255776518, 181534720, 16, 1223, 123, 201, 1 },
{ 1, 14, 255776518, 181534720, 44, 1985, 128, 218, 1 },
{ 1, 14, 255776518, 181534720, 45, 1985, 144, 127, 1 },
{ 1, 14, 255776518, 181534720, 37, 1992, 185, 131, 1 } };
