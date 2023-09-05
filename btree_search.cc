#include "btree_search.h"

static void * add(const void * target,uintptr_t const& offset){ return (void*) (uintptr_t(target) + (offset)); }
typedef int (*compare_t)( const void * key,const void * hash_and_rowid);


// todo : bad return value.  the adress of res should be relative.
int kautil_bt_search(void ** res,const void * key,const void * target,uint64_t const& size,uint64_t const& block_size,compare_t f){
     if(size < block_size){   // not redundant
//    if(size == block_size){
        *res = const_cast<void*>(target);
        return f(key,target);  /* arr[0] */
    }else{
        auto const& middle = (size/2) - ((size/2)%block_size);  /*  blcck_size+n => blcck_size */
        auto const& cur = add(target, middle);
        if(f(key,cur) == -1){ /* case " key < " */
            return kautil_bt_search(res, key, target, middle, block_size, f); /* arr[0] => middle */
        }else if(f(key,cur) == 1){  /* case " key > "  */
            if(!(size - middle - block_size)){ *res = cur;return 1; }  /* returning immediately not need to enter kautil_bt_search  */
            return kautil_bt_search(res, key
                              , add(cur,block_size) /* from {arr[at+1]} to {max - arr-size - 1}  */
                              , size - middle - block_size
                              , block_size, f);
        }else{
            *res = add(target, middle);
//            *res = add(target, middle);
            return  f(key,add(target, middle));
        }
    }
}


#define ip(o) reinterpret_cast<intptr_t>(o)
int kautil_bt_search_v2(void ** res,const void * key,const void * data,uint64_t const& data_bytes,uint64_t const& block_size,void * arg,compare_v2_t f){
     if(data_bytes < block_size){   // not redundant
        *res = const_cast<void*>(data);
        return f(ip(key),ip(data),arg);  /* arr[0] */
    }else{
        auto const& middle = (data_bytes/2) - ((data_bytes/2)%block_size);  /*  blcck_size+n => blcck_size */
        auto const& cur = add(data, middle);
        if(f(ip(key),ip(cur),arg) == -1){ /* case " key < " */
            return kautil_bt_search_v2(res, key, data, middle, block_size,arg, f); /* arr[0] => middle */
        }else if(f(ip(key),ip(cur),arg) == 1){  /* case " key > "  */
            if(!(data_bytes - middle - block_size)){ *res = cur;return 1; }  /* returning immediately not need to enter kautil_bt_search  */
            return kautil_bt_search_v2(res, key
                              , add(cur,block_size) /* from {arr[at+1]} to {max - arr-size - 1}  */
                              , data_bytes - middle - block_size
                              , block_size, arg,f);
        }else{
            *res = add(data, middle);
            return  f(ip(key),ip(add(data, middle)),arg);
        }
    }
}

int kautil_bt_search_v3(void ** res,const void * key,const void * begin,const void * end,uint64_t const& block_size,void * arg,compare_v2_t f){ return kautil_bt_search_v2(res,key,begin,ip(end) - ip(begin),block_size,arg,f); }

