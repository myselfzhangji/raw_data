// This project is designed to generate a dst width x height raw data
// according to a src raw data.
// Note:
//   (1)Both src raw and dst raw are 16 bit bytes, if raw resoultion is 1920x1080
//      then data size is 1920x1080x2
//   (2)Width is in pixel unit, pitch is in byte unit. So if raw is 1920x1080
//      then width = 1920, pitch = 1920 x 2
// History:
// 2020/11/12 - [Ryan Zhang] created file
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNES FOR A PARTIICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, see <http://www.gnu.org/license>
//

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <windows.h>
using namespace std;

typedef unsigned int u32;

#define FILE_PATH ".\\"
#define INPUT_NAME "cap_raw_raw_1920x1080_3840.raw"
#define INPUT_FILE_LENGTH 120
#define OUTPUT_FILE_LENGTH 120
#define OUTPUT_FILE_NAME_LENGTH 40

#define SRC_RAW_WIDTH (1920)
#define SRC_RAW_HEIGHT (1080)
#define SRC_RAW_SIZE (SRC_RAW_WIDTH * SRC_RAW_HEIGHT * 2)
#define DST_RAW_WIDTH (4784)
#define DST_RAW_HEIGHT (3190)
#define DST_RAW_SIZE (DST_RAW_WIDTH * DST_RAW_HEIGHT * 2)

static int read_src_raw_data(char* src_raw_data)
{
	FILE* fp = NULL;
	char* input_file = NULL;
	int ret = 0;

	input_file = (char*)malloc(INPUT_FILE_LENGTH);
	if (input_file == NULL) {
		printf("malloc input_file error!\n");
		ret = -1;
		goto READ_SRC_RAW_DATA_EXIT;
	}
	memset(input_file, 0, INPUT_FILE_LENGTH);

	strcpy_s(input_file, strlen(FILE_PATH) + 1, FILE_PATH);
	strcat_s(input_file, strlen(FILE_PATH) + strlen(INPUT_NAME) + 1, INPUT_NAME);
	printf("input file name : %s\n", input_file);

	fopen_s(&fp, input_file, "rb");
	if (fp == NULL) {
		printf("File is not exist!\n");
		ret = -1;
		goto READ_SRC_RAW_DATA_EXIT;
	}

	ret = fread(src_raw_data, SRC_RAW_SIZE, 1, fp);
	if (ret < 0) {
		printf("Fail to read file!\n");
		ret = -1;
		goto READ_SRC_RAW_DATA_EXIT;
	}

READ_SRC_RAW_DATA_EXIT:
	if (fp != NULL) {
		fclose(fp);
	}
	if (input_file != NULL) {
		free(input_file);
		input_file = NULL;
	}

	return ret;
}

static int generate_dst_raw_data(char* src_raw_data, char* dst_raw_data)
{
	int ret = 0;
	u32 i, j, dst_index, src_index;
	u32 src_pitch, dst_pitch;

	src_pitch = SRC_RAW_WIDTH * 2;
	dst_pitch = DST_RAW_WIDTH * 2;

	memset(dst_raw_data, 0, DST_RAW_SIZE);

	for (i = 0; i < DST_RAW_HEIGHT; i++) {
		for (j = 0; j < dst_pitch; j++) {
			if (i < SRC_RAW_HEIGHT && j < src_pitch) {
				src_index = i * src_pitch + j;
				dst_index = i * dst_pitch + j;
				dst_raw_data[dst_index] = src_raw_data[src_index];
			}
		}
	}

	return ret;
}

static int write_dst_raw_data(char* dst_raw_data)
{
	FILE* fp = NULL;
	char* output_file = NULL;
	char* output_file_name = NULL;
	int ret = 0;

	output_file = (char*)malloc(OUTPUT_FILE_LENGTH);
	if (output_file == NULL) {
		printf("malloc output_file error!\n");
		ret = -1;
		goto WRITE_DST_RAW_DATA_EXIT;
	}
	memset(output_file, 0, OUTPUT_FILE_LENGTH);

	output_file_name = (char*)malloc(OUTPUT_FILE_NAME_LENGTH);
	if (output_file_name == NULL) {
		printf("malloc output_file_name error!\n");
		ret = -1;
		goto WRITE_DST_RAW_DATA_EXIT;
	}
	snprintf(output_file_name, OUTPUT_FILE_NAME_LENGTH, "out_raw_%dx%d_%d.raw",
		DST_RAW_WIDTH, DST_RAW_HEIGHT, DST_RAW_WIDTH * 2);

	strcpy_s(output_file, strlen(FILE_PATH) + 1, FILE_PATH);
	strcat_s(output_file, strlen(FILE_PATH) + strlen(output_file_name) + 1,
		output_file_name);

	printf("output file name : %s\n", output_file);

	fopen_s(&fp, output_file, "wb");
	if (fp == NULL) {
		printf("File is not exist!\n");
		ret = -1;
		goto WRITE_DST_RAW_DATA_EXIT;
	}

	ret = fwrite(dst_raw_data, DST_RAW_SIZE, 1, fp);
	if (ret < 0) {
		printf("Fail to read file!\n");
		ret = -1;
		goto WRITE_DST_RAW_DATA_EXIT;
	}

WRITE_DST_RAW_DATA_EXIT:
	if (fp != NULL) {
		fclose(fp);
	}
	if (output_file != NULL) {
		free(output_file);
		output_file = NULL;
	}
	if (output_file_name != NULL) {
		free(output_file_name);
		output_file_name = NULL;
	}
	return ret;
}

int main()
{
	char* src_raw_data = NULL;
	char* dst_raw_data = NULL;
	int ret = 0;

	src_raw_data = (char*)malloc(SRC_RAW_SIZE);
	if (src_raw_data == NULL) {
		printf("malloc src_raw_data error!\n");
		return -1;
	}

	ret = read_src_raw_data(src_raw_data);
	if (ret < 0) {
		printf("read_src_raw_data error!\n");
		return -1;
	}

	dst_raw_data = (char*)malloc(DST_RAW_SIZE);
	if (dst_raw_data == NULL) {
		printf("malloc dst_raw_data error!\n");
		return -1;
	}

	ret = generate_dst_raw_data(src_raw_data, dst_raw_data);
	if (ret < 0) {
		printf("read_src_raw_data error!\n");
		return -1;
	}

	ret = write_dst_raw_data(dst_raw_data);
	if (ret < 0) {
		printf("write_dst_raw_data error!\n");
		return -1;
	}

	if (src_raw_data != NULL) {
		free(src_raw_data);
		src_raw_data = NULL;
	}
	if (dst_raw_data != NULL) {
		free(dst_raw_data);
		dst_raw_data = NULL;
	}

	return 0;
}
