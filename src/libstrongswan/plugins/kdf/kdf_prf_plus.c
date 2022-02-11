/*
 * Copyright (C) 2022 Tobias Brunner, codelabs GmbH
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "kdf_prf_plus.h"

#include <crypto/prf_plus.h>

typedef struct private_kdf_t private_kdf_t;

/**
 * Private data.
 */
struct private_kdf_t {

	/**
	 * Public interface.
	 */
	kdf_t public;

	/**
	 * Underlying PRF.
	 */
	prf_t *prf;

	/**
	 * Salt value.
	 */
	chunk_t salt;
};

METHOD(kdf_t, get_type, key_derivation_function_t,
	private_kdf_t *this)
{
	return KDF_PRF_PLUS;
}

METHOD(kdf_t, get_bytes, bool,
	private_kdf_t *this, size_t out_len, uint8_t *buffer)
{
	prf_plus_t *prf_plus;
	bool success;

	prf_plus = prf_plus_create(this->prf, TRUE, this->salt);
	if (!prf_plus)
	{
		return FALSE;
	}
	success = prf_plus->get_bytes(prf_plus, out_len, buffer);
	prf_plus->destroy(prf_plus);
	return success;
}

METHOD(kdf_t, allocate_bytes, bool,
	private_kdf_t *this, size_t out_len, chunk_t *chunk)
{
	*chunk = chunk_alloc(out_len);

	if (!get_bytes(this, out_len, chunk->ptr))
	{
		chunk_free(chunk);
		return FALSE;
	}
	return TRUE;
}

METHOD(kdf_t, set_param, bool,
	private_kdf_t *this, kdf_param_t param, ...)
{
	chunk_t chunk;
	bool success = FALSE;

	switch (param)
	{
		case KDF_PARAM_KEY:
			VA_ARGS_GET(param, chunk);
			success = this->prf->set_key(this->prf, chunk);
			break;
		case KDF_PARAM_SALT:
			VA_ARGS_GET(param, chunk);
			chunk_clear(&this->salt);
			this->salt = chunk_clone(chunk);
			success = TRUE;
			break;
	}
	return success;
}

METHOD(kdf_t, destroy, void,
	private_kdf_t *this)
{
	this->prf->destroy(this->prf);
	chunk_clear(&this->salt);
	free(this);
}

/*
 * Described in header
 */
kdf_t *kdf_prf_plus_create(key_derivation_function_t algo, va_list args)
{
	private_kdf_t *this;
	pseudo_random_function_t prf_alg;
	prf_t *prf;

	if (algo != KDF_PRF_PLUS)
	{
		return NULL;
	}

	VA_ARGS_VGET(args, prf_alg);
	prf = lib->crypto->create_prf(lib->crypto, prf_alg);
	if (!prf)
	{
		DBG1(DBG_LIB, "failed to create %N for prf+",
			 pseudo_random_function_names, prf_alg);
		return NULL;
	}

	INIT(this,
		.public = {
			.get_type = _get_type,
			.get_bytes = _get_bytes,
			.allocate_bytes = _allocate_bytes,
			.set_param = _set_param,
			.destroy = _destroy,
		},
		.prf = prf,
	);

	return &this->public;
}
