/*
 * Copyright (C) 2019 Tobias Brunner
 * HSR Hochschule fuer Technik Rapperswil
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.  See <http://www.fsf.org/copyleft/gpl.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 */

#include "test_suite.h"

#include <library.h>

static struct {
	chunk_t key;
	chunk_t seed;
	chunk_t expected;
} test_data[] = {
	{ .key = chunk_from_chars(0x0b,0x0b,0x0b,0x0b,0x0b,0x0b,0x0b,0x0b,
							  0x0b,0x0b,0x0b,0x0b,0x0b,0x0b,0x0b,0x0b,
							  0x0b,0x0b,0x0b,0x0b),
	  .seed = chunk_from_chars(0x48,0x69,0x20,0x54,0x68,0x65,0x72,0x65),
	  .expected = chunk_from_chars(0xb9,0xbd,0xc0,0x89,0x88,0xb4,0xc2,0xb7,
								   0x5a,0xa9,0x3e,0x59,0x6a,0xc8,0x42,0x05,
								   0xfa,0x2d,0xdd,0xe1,0xbf,0x7a,0x25,0x72,
								   0x06,0x7b,0x00,0xe1,0x4b,0x23,0x77,0x32,
								   0x83,0x05,0x09,0x98,0x1a,0xd2,0xf9,0x4a,
								   0x8c,0x32,0xa4,0x7d,0xaa,0x22,0x55,0xb6,
								   0x60,0xc4,0x36,0x34,0x7a,0xe7,0x56,0xa6,
								   0xed,0xc0,0x23,0x47,0x7d,0x80,0x95,0x90,
								   0xe6,0x82,0xf6,0x1d,0x9c,0x04,0xb0,0x6b,
								   0x4a,0xd9,0x71,0xa3,0x4c,0x81,0x47,0xfa,
								   0x66,0x79,0x2f,0xf1,0x43,0x4b,0x93,0xc7,
								   0x22,0xb3,0x2e,0x12,0xf4,0x88,0x32,0xeb,
								   0xc1,0x5c,0xe2,0x36,0x9c,0xe7,0x1f,0xe9,
								   0xb7,0xb8,0x1e,0x57,0x04,0xc1,0x4d,0x0f,
								   0x52,0x80,0xa6,0xec,0x62,0x6e,0x99,0x2d,
								   0x7a,0x9f),
	},
	/* change the key, keep the seed */
	{ .key = chunk_from_chars(0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,
							  0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,
							  0x0a,0x0a,0x0a,0x0a),
	  .expected = chunk_from_chars(0x1a,0x2f,0xc7,0x4a,0x06,0x8c,0xae,0x76,
								   0xfa,0xb3,0xd6,0x34,0xae,0xe9,0x81,0x55,
								   0x11,0x6a,0x4b,0x21,0xe1,0x0d,0x1b,0x16,
								   0x45,0x7a,0x06,0xd9,0x42,0x27,0x93,0x98,
								   0xf7,0x4d,0xf1,0x59,0xc1,0x25,0x21,0xae,
								   0xe6,0xf4,0x80,0x01,0xe5,0x86,0x8e,0xa7,
								   0x4b,0x1e,0x13,0xd1,0xcf,0xdc,0xb7,0x7b,
								   0xf8,0xcf,0x75,0x2c,0x67,0x13,0x18,0x7a,
								   0x38,0x55,0xba,0x4b,0xf2,0x57,0x55,0xcd,
								   0x96,0x20,0xcb,0xe3,0xc4,0x8a,0x7f,0xa3,
								   0x86,0xa5,0xc6,0x26,0x8e,0x57,0xd8,0xe0,
								   0xb6,0xf9,0x8a,0x41,0x16,0x82,0x64,0x8a),
	},
	/* change the seed, keep the key */
	{ .seed = chunk_from_chars(0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,
							   0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10),
	  .expected = chunk_from_chars(0x8b,0x90,0x9c,0xbc,0xbb,0xf1,0x72,0x63,
								   0x6b,0x4e,0x63,0xcd,0x7e,0xc5,0xe3,0x45,
								   0x57,0x3e,0xbf,0x72,0x8f,0x62,0xa4,0x9b,
								   0x83,0x7b,0xd9,0x53,0xc0,0x0c,0xad,0x3d,
								   0x19,0x16,0x28,0x81,0x85,0xf9,0x27,0xb6,
								   0xc4,0x0c,0x48,0x31,0x45,0x12,0x3b,0x5a,
								   0xb8,0x47,0xd0,0x19,0x6e,0x6b,0x1c,0x5a,
								   0x2a,0xc1,0xe5,0x1e,0xc5,0x43,0xcc,0xd4,
								   0x28,0xba,0x30,0x4b,0x5e,0xad,0x97,0xa7,
								   0xc0,0x9d,0x13,0xdd,0xfb,0x4a,0x42,0x43,
								   0x87,0xd6,0x22,0xf6,0x03,0x19,0x21,0x31,
								   0x6a,0xa6,0x38,0x44,0xa5,0x61,0xf6,0x23,
								   0x0c,0x50,0x14,0xcc,0xce,0x09,0x5f,0xb3,
								   0xcc,0xe4,0xcb,0x8f,0x43,0xdc,0x2b,0x65,
								   0xfd,0x42,0xcc,0xeb,0x49,0x0e,0xcb,0xeb,
								   0x47,0x30,0xb5,0x18,0x6d,0x34,0x7a,0xea,
								   0xad,0xfd,0x66,0xa7,0x7e,0xd3,0x3f,0x42,
								   0xdf,0x75,0x54,0xef,0x5f,0x4f,0x7e,0x26,
								   0xf9,0x38,0x73,0x26,0x92,0x7a,0xc7,0x80),
	},
};

START_TEST(test_params)
{
	kdf_t *kdf;
	chunk_t out;
	int i;

	kdf = lib->crypto->create_kdf(lib->crypto, KDF_PRF_PLUS, PRF_HMAC_SHA2_256);
	if (!kdf)
	{
		warn("%N (%N) not supported", key_derivation_function_names,
			 KDF_PRF_PLUS, pseudo_random_function_names, PRF_HMAC_SHA2_256);
		return;
	}
	for (i = 0; i < countof(test_data); i++)
	{
		if (test_data[i].key.len)
		{
			ck_assert(kdf->set_param(kdf, KDF_PARAM_KEY, test_data[i].key));
		}
		if (test_data[i].seed.len)
		{
			ck_assert(kdf->set_param(kdf, KDF_PARAM_SALT, test_data[i].seed));
		}
		ck_assert(kdf->allocate_bytes(kdf, test_data[i].expected.len, &out));
		ck_assert_chunk_eq(test_data[i].expected, out);
		chunk_free(&out);
		/* same output the second time */
		ck_assert(kdf->allocate_bytes(kdf, test_data[i].expected.len, &out));
		ck_assert_chunk_eq(test_data[i].expected, out);
		chunk_free(&out);
	}
	kdf->destroy(kdf);
}
END_TEST

START_TEST(test_wrap)
{
	kdf_t *kdf;
	chunk_t out;

	kdf = lib->crypto->create_kdf(lib->crypto, KDF_PRF_PLUS, PRF_HMAC_SHA2_256);
	if (!kdf)
	{
		warn("%N (%N) not supported", key_derivation_function_names,
			 KDF_PRF_PLUS, pseudo_random_function_names, PRF_HMAC_SHA2_256);
		return;
	}
	ck_assert(kdf->set_param(kdf, KDF_PARAM_KEY, test_data[0].key));
	ck_assert(kdf->set_param(kdf, KDF_PARAM_SALT, test_data[0].seed));

	/* the 1-byte counter overflows after 255 blocks of the underlying PRF */
	out = chunk_alloc(32 * 255 + 1);
	ck_assert(kdf->get_bytes(kdf, out.len - 2, out.ptr));
	if (!kdf->get_bytes(kdf, out.len - 1, out.ptr))
	{	/* Botan has a check for (len/bs) >= 255 blocks, so we allow this */
		warn("unable to generate maximum-sized key for %N (%N) but maximum-1 "
			 "is fine", key_derivation_function_names, KDF_PRF_PLUS,
			 pseudo_random_function_names, PRF_HMAC_SHA2_256);
	}
	ck_assert(!kdf->get_bytes(kdf, out.len, out.ptr));
	chunk_free(&out);
	kdf->destroy(kdf);
}
END_TEST

Suite *prf_plus_suite_create()
{
	Suite *s;
	TCase *tc;

	s = suite_create("prf_plus");

	tc = tcase_create("params");
	tcase_add_test(tc, test_params);
	suite_add_tcase(s, tc);

	tc = tcase_create("wrap");
	tcase_add_test(tc, test_wrap);
	suite_add_tcase(s, tc);

	return s;
}
