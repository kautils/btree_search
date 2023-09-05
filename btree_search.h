

#ifndef KAUTIL_ALGORITHM_SEARCH_BTREE_H
#define KAUTIL_ALGORITHM_SEARCH_BTREE_H

#include <stdint.h>

/// @return callback
///   -1 : l < r
///   1  : l > r
///   0  : l == r
//int compare_function(const key_t * l,const key_t * r);
typedef int (*compare_t)( void const* l,const void * r);
typedef int (*compare_v2_t)( intptr_t l,intptr_t r,void * arg);
extern "C" int kautil_bt_search(void ** res,const void * key,const void * target,uint64_t const& size,uint64_t const& block_size,compare_t f);
extern "C" int kautil_bt_search_v2(void ** res,const void * key,const void * data,uint64_t const& data_bytes,uint64_t const& block_size,void * arg,compare_v2_t f);
extern "C" int kautil_bt_search_v3(void ** res,const void * key,const void * begin,const void * end,uint64_t const& block_size,void * arg,compare_v2_t f);

// need not to be unique but to be sorted

#endif


