/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ztest.h>
#include "strange_encode.h"

void test_numbers(void)
{
	const uint8_t exp_payload_numbers1[] = {
		0x8A, // List start
			0x01, // 1
			0x21, // -2
			0x05, // 5
			0x19, 0x01, 0x00, // 256
			0x1A, 0x01, 0x02, 0x03, 0x04, // 0x01020304
			0x39, 0x13, 0x87, // -5000
			0x1A, 0xEE, 0x6B, 0x28, 0x00, // 4000000000
			0x3A, 0x7F, 0xFF, 0xFF, 0xFF, // -2^31
			0x00, // 0
			0x01 // 1
	};

	Numbers_t numbers = {0};
	uint8_t output[100];
	size_t out_len;

	numbers._Numbers_fourtoten = 3; // Invalid
	numbers._Numbers_twobytes = 256;
	numbers._Numbers_onetofourbytes = 0x01020304;
	numbers._Numbers_minusfivektoplustwohunred = -5000;
	numbers._Numbers_negint = -2147483648;
	numbers._Numbers_posint = 0;
	numbers._Numbers_integer = 1;

	zassert_false(cbor_encode_Numbers(output,
		sizeof(output), &numbers, &out_len), NULL);

	numbers._Numbers_fourtoten = 11; // Invalid
	zassert_false(cbor_encode_Numbers(output,
		sizeof(output), &numbers, &out_len), NULL);

	numbers._Numbers_fourtoten = 5; // Valid
	zassert_true(cbor_encode_Numbers(output,
		sizeof(output), &numbers, &out_len), NULL);
	zassert_equal(sizeof(exp_payload_numbers1), out_len, NULL);
	zassert_mem_equal(exp_payload_numbers1, output, sizeof(exp_payload_numbers1), NULL);

	numbers._Numbers_negint = 1; // Invalid
	zassert_false(cbor_encode_Numbers(output,
		sizeof(output), &numbers, &out_len), NULL);

	numbers._Numbers_negint = -1; // Valid
	zassert_true(cbor_encode_Numbers(output,
		sizeof(output), &numbers, &out_len), NULL);

	numbers._Numbers_minusfivektoplustwohunred = -5001; // Invalid
	zassert_false(cbor_encode_Numbers(output,
		sizeof(output), &numbers, &out_len), NULL);

	numbers._Numbers_minusfivektoplustwohunred = 201; // Invalid
	zassert_false(cbor_encode_Numbers(output,
		sizeof(output), &numbers, &out_len), NULL);

	numbers._Numbers_minusfivektoplustwohunred = 200; // Valid
	zassert_true(cbor_encode_Numbers(output,
		sizeof(output), &numbers, &out_len), NULL);
}


void test_strings(void)
{
	const uint8_t exp_payload_strings1[] = {
		0x86,
		0x65, 0x68, 0x65, 0x6c, 0x6c, 0x6f, // "hello"
		0x59, 0x01, 0x2c, // 300 bytes
		0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,
		0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,
		0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,
		0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,
		0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,
		0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,
		0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,
		0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,
		0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,
		0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,
		0x78, 0x1E, // 30 bytes
		0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,
		0x58, 26, // Numbers (len: 26)
			0x8A, // List start
				0x01, // 1
				0x21, // -2
				0x05, // 5
				0x19, 0xFF, 0xFF, // 0xFFFF
				0x18, 0x18, // 24
				0x00, // 0
				0x1A, 0xEE, 0x6B, 0x28, 0x00, // 4000000000
				0x3A, 0x7F, 0xFF, 0xFF, 0xFF, // -2^31
				0x1A, 0xFF, 0xFF, 0xFF, 0xFF, // 0xFFFFFFFF
				0x09, // 9
		0x4f, // Primitives (len: 15)
			0x84, // List start
				0xF5, // True
				0xF4, // False
				0xF4, // False
				0xF6, // Nil
			0x84, // List start
				0xF5, // True
				0xF4, // False
				0xF5, // True
				0xF6, // Nil
			0x84, // List start
				0xF5, // True
				0xF4, // False
				0xF4, // False
				0xF6, // Nil
		0x59, 0x01, 0x63, // Strings (len: 355)
			0x85,
			0x65, 0x68, 0x65, 0x6c, 0x6c, 0x6f, // "hello"
			0x59, 0x01, 0x2c, // 300 bytes
			0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,
			0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,
			0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,
			0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,
			0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,
			0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,
			0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,
			0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,
			0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,
			0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,
			0x6A, // 10 bytes
			0,1,2,3,4,5,6,7,8,9,
			0x58, 26, // Numbers (len: 26)
				0x8A, // List start
					0x01, // 1
					0x21, // -2
					0x05, // 5
					0x19, 0xFF, 0xFF, // 0xFFFF
					0x18, 0x18, // 24
					0x00, // 0
					0x1A, 0xEE, 0x6B, 0x28, 0x00, // 4000000000
					0x3A, 0x7F, 0xFF, 0xFF, 0xFF, // -2^31
					0x1A, 0xFF, 0xFF, 0xFF, 0xFF, // 0xFFFFFFFF
					0x29, // -10
			0x45, // Primitives (len: 5)
				0x84, // List start
					0xF5, // True
					0xF4, // False
					0xF4, // False
					0xF6, // Nil
	};

	const uint8_t bytes300[] = {
		0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,
		0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,
		0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,
		0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,
		0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,
		0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,
		0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,
		0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,
		0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,
		0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,
	};

	Strings_t strings1 = {0};
	Strings_t strings2 = {0};
	Numbers_t numbers1 = {0};
	uint8_t output1[100];
	uint8_t output2[100];
	uint8_t output3[400];
	uint8_t output4[800];
	size_t out_len;

	numbers1._Numbers_fourtoten = 5;
	numbers1._Numbers_twobytes = 0xFFFF;
	numbers1._Numbers_onetofourbytes = 24;
	numbers1._Numbers_minusfivektoplustwohunred = 0;
	numbers1._Numbers_negint = -2147483648;
	numbers1._Numbers_posint = 0xFFFFFFFF;
	numbers1._Numbers_integer = 9;

	strings1._Strings_optCborStrings_present = true;
	strings1._Strings_threehundrebytebstr.len = 300;
	strings1._Strings_threehundrebytebstr.value = bytes300;
	strings1._Strings_tentothirtybytetstr.len = 30;
	strings1._Strings_tentothirtybytetstr.value = bytes300;
	strings1._Strings_cborseqPrimitives_cbor_count = 3;
	strings1._Strings_cborseqPrimitives_cbor[0]._Primitives_boolval = false;
	strings1._Strings_cborseqPrimitives_cbor[1]._Primitives_boolval = true;
	strings1._Strings_cborseqPrimitives_cbor[2]._Primitives_boolval = false;

	strings2._Strings_threehundrebytebstr.len = 300;
	strings2._Strings_threehundrebytebstr.value = bytes300;
	strings2._Strings_tentothirtybytetstr.len = 9; // Invalid
	strings2._Strings_tentothirtybytetstr.value = bytes300;
	strings2._Strings_cborseqPrimitives_cbor_count = 1;
	strings2._Strings_cborseqPrimitives_cbor[0]._Primitives_boolval = false;

	zassert_true(cbor_encode_Numbers(output2, sizeof(output2), &numbers1, &out_len), NULL);
	strings1._Strings_cborNumbers.value = output2;
	strings1._Strings_cborNumbers.len = out_len;
	numbers1._Numbers_integer = -10;
	zassert_true(cbor_encode_Numbers(output1, sizeof(output1), &numbers1, &out_len), NULL);
	strings2._Strings_cborNumbers.value = output1;
	strings2._Strings_cborNumbers.len = out_len;
	zassert_false(cbor_encode_Strings(output3, sizeof(output3), &strings2, &out_len), NULL);
	strings2._Strings_tentothirtybytetstr.len = 31; // Invalid
	zassert_false(cbor_encode_Strings(output3, sizeof(output3), &strings2, &out_len), NULL);
	strings2._Strings_tentothirtybytetstr.len = 10; // Valid
	zassert_true(cbor_encode_Strings(output3, sizeof(output3), &strings2, &out_len), NULL);
	strings1._Strings_optCborStrings.value = output3;
	strings1._Strings_optCborStrings.len = out_len;
	zassert_true(cbor_encode_Strings(output4, sizeof(output4), &strings1, &out_len), NULL);
	zassert_equal(sizeof(exp_payload_strings1), out_len, "expected: %d, actual: %d\r\n", sizeof(exp_payload_strings1), out_len);

	zassert_mem_equal(exp_payload_strings1, output4, sizeof(exp_payload_strings1), NULL);
}

void test_optional(void)
{
	const uint8_t exp_payload_optional1[] = {
		0x83 /* List start */, 0xF4 /* False */, 0x02, 0x03,
	};
	const uint8_t exp_payload_optional2[] = {
		0x82 /* List start */, 0xF4 /* False */, 0x03,
	};
	const uint8_t exp_payload_optional3[] = {
		0x83 /* List start */, 0xF4 /* False */, 0x02, 0x01,
	};
	const uint8_t exp_payload_optional4[] = {
		0x83 /* List start */, 0xF5 /* True */, 0x02, 0x02,
	};
	const uint8_t exp_payload_optional5[] = {
		0x84 /* List start */, 0xF5 /* True */, 0xF4 /* False */, 0x02, 0x02,
	};

	Optional_t optional1 = {._Optional_opttwo_present = true, ._Optional_manduint = 3};
	Optional_t optional2 = {._Optional_manduint = 3};
	Optional_t optional3 = {._Optional_opttwo_present = true, ._Optional_manduint = 1};
	Optional_t optional4 = {._Optional_boolval = true, ._Optional_opttwo_present = true,
				._Optional_manduint = 2};
	Optional_t optional5 = {._Optional_boolval = true, ._Optional_optbool_present = true,
				._Optional_opttwo_present = true, ._Optional_manduint = 2};
	uint8_t output[10];
	size_t out_len;

	zassert_true(cbor_encode_Optional(output,
			sizeof(output), &optional1, &out_len), NULL);
	zassert_equal(sizeof(exp_payload_optional1), out_len, NULL);
	zassert_mem_equal(exp_payload_optional1, output, sizeof(exp_payload_optional1), NULL);

	zassert_true(cbor_encode_Optional(output,
			sizeof(output), &optional2, &out_len), NULL);
	zassert_equal(sizeof(exp_payload_optional2), out_len, NULL);
	zassert_mem_equal(exp_payload_optional2, output, sizeof(exp_payload_optional2), NULL);

	zassert_true(cbor_encode_Optional(output,
			sizeof(output), &optional3, &out_len), NULL);
	zassert_equal(sizeof(exp_payload_optional3), out_len, NULL);
	zassert_mem_equal(exp_payload_optional3, output, sizeof(exp_payload_optional3), NULL);

	zassert_true(cbor_encode_Optional(output,
			sizeof(output), &optional4, &out_len), NULL);
	zassert_equal(sizeof(exp_payload_optional4), out_len, NULL);
	zassert_mem_equal(exp_payload_optional4, output, sizeof(exp_payload_optional4), NULL);

	zassert_true(cbor_encode_Optional(output,
			sizeof(output), &optional5, &out_len), NULL);
	zassert_equal(sizeof(exp_payload_optional5), out_len, NULL);
	zassert_mem_equal(exp_payload_optional5, output, sizeof(exp_payload_optional5), NULL);
}

void test_union(void)
{
	const uint8_t exp_payload_union1[] = {0x01, 0x21};
	const uint8_t exp_payload_union2[] = {0x03, 0x23};
	const uint8_t exp_payload_union3[] = {0x03, 0x04};
	const uint8_t exp_payload_union4[] = {
		0x65, 0x68, 0x65, 0x6c, 0x6c, 0x6f // "hello"
	};
	const uint8_t exp_payload_union5[] = {0x03, 0x23, 0x03, 0x23, 0x03, 0x23};

	_Union_t _union1 = {._Union_choice = _Union__Group};
	_Union_t _union2 = {._Union_choice = _Union__MultiGroup, ._Union__MultiGroup._MultiGroup_count = 1};
	_Union_t _union3 = {._Union_choice = _Union__uint3};
	_Union_t _union4 = {._Union_choice = _Union_hello_tstr};
	_Union_t _union5 = {._Union_choice = _Union__MultiGroup, ._Union__MultiGroup._MultiGroup_count = 3};

	uint8_t output[10];
	size_t out_len;

	zassert_true(cbor_encode_Union(output, sizeof(output),
				&_union1, &out_len), NULL);
	zassert_equal(sizeof(exp_payload_union1), out_len, NULL);
	zassert_mem_equal(exp_payload_union1, output, sizeof(exp_payload_union1), NULL);

	zassert_true(cbor_encode_Union(output, sizeof(output),
				&_union2, &out_len), NULL);
	zassert_equal(sizeof(exp_payload_union2), out_len, NULL);
	zassert_mem_equal(exp_payload_union2, output, sizeof(exp_payload_union2), NULL);

	zassert_true(cbor_encode_Union(output, sizeof(output),
				&_union3, &out_len), NULL);
	zassert_equal(sizeof(exp_payload_union3), out_len, NULL);
	zassert_mem_equal(exp_payload_union3, output, sizeof(exp_payload_union3), NULL);

	zassert_true(cbor_encode_Union(output, sizeof(output),
				&_union4, &out_len), NULL);
	zassert_equal(sizeof(exp_payload_union4), out_len, NULL);
	zassert_mem_equal(exp_payload_union4, output, sizeof(exp_payload_union4), NULL);

	zassert_true(cbor_encode_Union(output, sizeof(output),
				&_union5, &out_len), NULL);
	zassert_equal(sizeof(exp_payload_union5), out_len, NULL);
	zassert_mem_equal(exp_payload_union5, output, sizeof(exp_payload_union5), NULL);
}

void test_levels(void)
{
	const uint8_t exp_payload_levels1[] = {
		0x81, // Level1
		0x82, // Level2
		0x84, // Level3 no 1
		0x81, 0x00, // Level4 no 1
		0x81, 0x00, // Level4 no 2
		0x81, 0x00, // Level4 no 3
		0x81, 0x00, // Level4 no 4
		0x84, // Level3 no 2
		0x81, 0x00, // Level4 no 1
		0x81, 0x00, // Level4 no 2
		0x81, 0x00, // Level4 no 3
		0x81, 0x00, // Level4 no 4
	};
	uint8_t output[20];
	size_t out_len;

	Level2_t level1 = {._Level2__Level3_count = 2, ._Level2__Level3 = {
		{._Level3__Level4_count = 4}, {._Level3__Level4_count = 4}
	}};
	zassert_true(cbor_encode_Level1(output,
		sizeof(output), &level1, &out_len), NULL);

	zassert_equal(sizeof(exp_payload_levels1), out_len, NULL);
	zassert_mem_equal(exp_payload_levels1, output, sizeof(exp_payload_levels1), NULL);
}


void test_map(void)
{
	const uint8_t exp_payload_map1[] = {
		0xa4, 0x82, 0x05, 0x06, 0xF4, // [5,6] => false
		0x07, 0x01, // 7 => 1
		0xf6, 0x45, 0x68, 0x65, 0x6c, 0x6c, 0x6f, // nil => "hello"
		0xf6, 0x40, // nil => ""
	};
	const uint8_t exp_payload_map2[] = {
		0xa5, 0x82, 0x05, 0x06, 0xF5, // [5,6] => true
		0x07, 0x01, // 7 => 1
		0xf6, 0x45, 0x68, 0x65, 0x6c, 0x6c, 0x6f, // nil => "hello"
		0xf6, 0x40, // nil => ""
		0xf6, 0x40, // nil => ""
	};
	const uint8_t exp_payload_map3[] = {
		0xa4, 0x82, 0x05, 0x06, 0xF4, // [5,6] => false
		0x27, 0x01, // -8 => 1
		0xf6, 0x45, 0x68, 0x65, 0x6c, 0x6c, 0x6f, // nil => "hello"
		0xf6, 0x40 // nil => ""
	};

	Map_t map1 = {
		._Map_union_choice = _Map_union_uint7uint,
		._Map_union_uint7uint = 1,
		._Map_twotothree_count = 2,
		._Map_twotothree = {
			{._Map_twotothree = {.value = "hello", .len = 5}},
			{._Map_twotothree = {.len = 0}},
		}
	};
	Map_t map2 = {
		._Map_key = true,
		._Map_union_choice = _Map_union_uint7uint,
		._Map_union_uint7uint = 1,
		._Map_twotothree_count = 3,
		._Map_twotothree = {
			{._Map_twotothree = {.value = "hello", .len = 5}},
			{._Map_twotothree = {.len = 0}},
			{._Map_twotothree = {.len = 0}},
		}
	};
	Map_t map3 = {
		._Map_union_choice = _Map_union_nintuint,
		._Map_union_nintuint = 1,
		._Map_twotothree_count = 2,
		._Map_twotothree = {
			{._Map_twotothree = {.value = "hello", .len = 5}},
			{._Map_twotothree = {.len = 0}},
		}
	};

	uint8_t output[25];
	size_t out_len;

	zassert_true(cbor_encode_Map(output, sizeof(output),
			&map1, &out_len), NULL);
	zassert_equal(sizeof(exp_payload_map1), out_len, NULL);
	zassert_mem_equal(exp_payload_map1, output, sizeof(exp_payload_map1), NULL);

	zassert_true(cbor_encode_Map(output, sizeof(output),
			&map2, &out_len), NULL);
	zassert_equal(sizeof(exp_payload_map2), out_len, NULL);
	zassert_mem_equal(exp_payload_map2, output, sizeof(exp_payload_map2), NULL);

	zassert_true(cbor_encode_Map(output, sizeof(output),
			&map3, &out_len), NULL);
	zassert_equal(sizeof(exp_payload_map3), out_len, NULL);
	zassert_mem_equal(exp_payload_map3, output, sizeof(exp_payload_map3), NULL);
}

void test_nested_list_map(void)
{
	const uint8_t exp_payload_nested_lm1[] = {0x80};
	const uint8_t exp_payload_nested_lm2[] = {0x81, 0xa0};
	const uint8_t exp_payload_nested_lm3[] = {0x81, 0xa1, 0x01, 0x04};
	const uint8_t exp_payload_nested_lm4[] = {0x82, 0xa0, 0xa1, 0x01, 0x04};
	const uint8_t exp_payload_nested_lm5[] = {0x83, 0xa0, 0xa0, 0xa0};
	NestedListMap_t listmap1 = {
		._NestedListMap_map_count = 0,
	};
	NestedListMap_t listmap2 = {
		._NestedListMap_map_count = 1,
		._NestedListMap_map = {
			{._NestedListMap_map_uint4_present = false},
		}
	};
	NestedListMap_t listmap3 = {
		._NestedListMap_map_count = 1,
		._NestedListMap_map = {
			{._NestedListMap_map_uint4_present = true},
		}
	};
	NestedListMap_t listmap4 = {
		._NestedListMap_map_count = 2,
		._NestedListMap_map = {
			{._NestedListMap_map_uint4_present = false},
			{._NestedListMap_map_uint4_present = true},
		}
	};
	NestedListMap_t listmap5 = {
		._NestedListMap_map_count = 3,
		._NestedListMap_map = {
			{._NestedListMap_map_uint4_present = false},
			{._NestedListMap_map_uint4_present = false},
			{._NestedListMap_map_uint4_present = false},
		}
	};
	uint8_t output[25];
	size_t out_len;

	zassert_true(cbor_encode_NestedListMap(output,
			sizeof(output), &listmap1, &out_len), NULL);

	zassert_equal(sizeof(exp_payload_nested_lm1), out_len, NULL);
	zassert_mem_equal(exp_payload_nested_lm1, output, sizeof(exp_payload_nested_lm1), NULL);

	zassert_true(cbor_encode_NestedListMap(output,
			sizeof(output), &listmap2, &out_len), NULL);

	zassert_equal(sizeof(exp_payload_nested_lm2), out_len, NULL);
	zassert_mem_equal(exp_payload_nested_lm2, output, sizeof(exp_payload_nested_lm2), NULL);

	zassert_true(cbor_encode_NestedListMap(output,
			sizeof(output), &listmap3, &out_len), NULL);

	zassert_equal(sizeof(exp_payload_nested_lm3), out_len, NULL);
	zassert_mem_equal(exp_payload_nested_lm3, output, sizeof(exp_payload_nested_lm3), NULL);

	zassert_true(cbor_encode_NestedListMap(output,
			sizeof(output), &listmap4, &out_len), NULL);

	zassert_equal(sizeof(exp_payload_nested_lm4), out_len, NULL);
	zassert_mem_equal(exp_payload_nested_lm4, output, sizeof(exp_payload_nested_lm4), NULL);

	zassert_true(cbor_encode_NestedListMap(output,
			sizeof(output), &listmap5, &out_len), NULL);

	zassert_equal(sizeof(exp_payload_nested_lm5), out_len, NULL);
	zassert_mem_equal(exp_payload_nested_lm5, output, sizeof(exp_payload_nested_lm5), NULL);
}

void test_nested_map_list_map(void)
{
	const uint8_t exp_payload_nested_mlm1[] = {0xa1, 0x80, 0x80};
	const uint8_t exp_payload_nested_mlm2[] = {0xa1, 0x80, 0x81, 0xa0};
	const uint8_t exp_payload_nested_mlm3[] = {0xa1, 0x80, 0x82, 0xa0, 0xa0};
	const uint8_t exp_payload_nested_mlm4[] = {0xa2, 0x80, 0x80, 0x80, 0x80};
	const uint8_t exp_payload_nested_mlm5[] = {0xa3, 0x80, 0x80, 0x80, 0x80, 0x80, 0x82, 0xa0, 0xa0};
	NestedMapListMap_t maplistmap1 = {
		._NestedMapListMap_key_count = 1,
		._NestedMapListMap_key = {{0}}
	};
	NestedMapListMap_t maplistmap2 = {
		._NestedMapListMap_key_count = 1,
		._NestedMapListMap_key = {
			{._NestedMapListMap_key_map_count = 1}
		}
	};
	NestedMapListMap_t maplistmap3 = {
		._NestedMapListMap_key_count = 1,
		._NestedMapListMap_key = {
			{._NestedMapListMap_key_map_count = 2}
		}
	};
	NestedMapListMap_t maplistmap4 = {
		._NestedMapListMap_key_count = 2,
		._NestedMapListMap_key = {
			{._NestedMapListMap_key_map_count = 0},
			{._NestedMapListMap_key_map_count = 0},
		}
	};
	NestedMapListMap_t maplistmap5 = {
		._NestedMapListMap_key_count = 3,
		._NestedMapListMap_key = {
			{._NestedMapListMap_key_map_count = 0},
			{._NestedMapListMap_key_map_count = 0},
			{._NestedMapListMap_key_map_count = 2},
		}
	};
	uint8_t output[15];
	size_t out_len;

	zassert_true(cbor_encode_NestedMapListMap(output,
			sizeof(output), &maplistmap1, &out_len), NULL);

	zassert_equal(sizeof(exp_payload_nested_mlm1), out_len, NULL);
	zassert_mem_equal(exp_payload_nested_mlm1, output, sizeof(exp_payload_nested_mlm1), NULL);

	zassert_true(cbor_encode_NestedMapListMap(output,
			sizeof(output), &maplistmap2, &out_len), NULL);

	zassert_equal(sizeof(exp_payload_nested_mlm2), out_len, NULL);
	zassert_mem_equal(exp_payload_nested_mlm2, output, sizeof(exp_payload_nested_mlm2), NULL);

	zassert_true(cbor_encode_NestedMapListMap(output,
			sizeof(output), &maplistmap3, &out_len), NULL);

	zassert_equal(sizeof(exp_payload_nested_mlm3), out_len, NULL);
	zassert_mem_equal(exp_payload_nested_mlm3, output, sizeof(exp_payload_nested_mlm3), NULL);

	zassert_true(cbor_encode_NestedMapListMap(output,
			sizeof(output), &maplistmap4, &out_len), NULL);

	zassert_equal(sizeof(exp_payload_nested_mlm4), out_len, NULL);
	zassert_mem_equal(exp_payload_nested_mlm4, output, sizeof(exp_payload_nested_mlm4), NULL);

	zassert_true(cbor_encode_NestedMapListMap(output,
			sizeof(output), &maplistmap5, &out_len), NULL);

	zassert_equal(sizeof(exp_payload_nested_mlm5), out_len, NULL);
	zassert_mem_equal(exp_payload_nested_mlm5, output, sizeof(exp_payload_nested_mlm5), NULL);
}

void test_main(void)
{
	ztest_test_suite(cbor_encode_test5,
			 ztest_unit_test(test_numbers),
			 ztest_unit_test(test_strings),
			 ztest_unit_test(test_optional),
			 ztest_unit_test(test_union),
			 ztest_unit_test(test_levels),
			 ztest_unit_test(test_map),
			 ztest_unit_test(test_nested_list_map),
			 ztest_unit_test(test_nested_map_list_map)
	);
	ztest_run_test_suite(cbor_encode_test5);
}