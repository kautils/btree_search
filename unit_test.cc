

#ifdef TMAIN_KAUTIL_ARGORITHM_BTREE_SEARCH_STATIC

#include <vector>
#include <iostream>
#include <algorithm>
#include "btree_search.h"


void print(const void * digest,uint64_t const& bytes ){
    auto arr = reinterpret_cast<const uint32_t*>(digest);
    auto arr_len = bytes / sizeof(uint32_t);
    auto f = stdout;
    for(auto i = 0; i < arr_len; ++i) fprintf(f, "%08x", arr[i]);
    fprintf(f,"\n");
    fflush(stdout);
}


struct hash_and_rowid{ uint64_t abc[4]; };
#define __byte40(v) reinterpret_cast<uint64_t*>(reinterpret_cast<struct hash_and_rowid*>(const_cast<void*>(v))->abc)
static int __compare( const void * key,const void * hash_and_rowid){
    for(int8_t i = 3; i != -1; --i){
        if(__byte40(key)[i] == __byte40(hash_and_rowid)[i]) continue;
        if(__byte40(key)[i] < __byte40(hash_and_rowid)[i]) return -1;
        if(__byte40(key)[i] > __byte40(hash_and_rowid)[i]) return 1;
    }
    return 0;
}
#undef __byte40


hash_and_rowid set(hash_and_rowid & v,uint64_t _0,uint64_t _1,uint64_t _2,uint64_t _3){
    reinterpret_cast<uint64_t*>(v.abc)[0] = _0;
    reinterpret_cast<uint64_t*>(v.abc)[1] = _1;
    reinterpret_cast<uint64_t*>(v.abc)[2] = _2;
    reinterpret_cast<uint64_t*>(v.abc)[3] = _3;
    return v;
};



int tmain_kautil_argorithm_btree_search_static(){


    // assume sql
    std::vector<hash_and_rowid> assume_sql;

    // assume request want to fetch from sql
    std::vector<hash_and_rowid> request;
    std::vector<hash_and_rowid> request_ramdamized;
    auto ans_count = 0;
    for(auto i = uint64_t(0);i < 10; ++i){
        assume_sql.push_back({i ,i*i ,i*i*i ,i*i*i*i});
        if( uint64_t(3) <= i && i <= uint64_t(7) ){
            // should be hit
            request.push_back(assume_sql.back());
            printf("ans : ");
            print(&request.back(),32);
            ++ans_count;
        }
    }



    // should not be hit
    request.push_back({1596294928687370694ull ,1390175245591044143ull ,2426613603486987655ull ,3103576262232253609ull});
    std::swap(request[0],request[request.size()-1]);
    std::swap(request[2],request[3]);



    printf("before sort\n");
    for(auto & elem : request) print(&elem, 32);
    std::sort(request.begin(),request.end(),[](auto l, auto r){
        for(auto i = int(3); i != -1; --i){
            if(l.abc[i] < r.abc[i]) return true;
            else if(l.abc[i] > r.abc[i]) return false;
        }
        return &l.abc < &r.abc;
    });


    printf("after sort\n");
    for(auto & elem : request) print(&elem,32);

    // a and b are ordered by ascend
    auto limit = ans_count;
    auto offset=uintptr_t(0);
    for(auto & s : assume_sql){ //  for(;;) sqlite3_step(...)
        hash_and_rowid * res;
        printf("search for ");
        print(&s,32);
        printf(" starting from the offset position \"%d\" ... ",(int)offset);
        if(!kautil_bt_search((void**)&res,&s.abc,request.data()+offset,((request.size()-offset)*32),32, __compare)){
            printf("found");
            !--limit;
        }else{
            printf("not found");
        }
        printf("\n");
        fflush(stdout);
        if(!limit){
            printf("all the hash was found. break immediately.");
            break;
        }
        offset = res - request.data();
    }
    return 0;
}


int main(){
    return tmain_kautil_argorithm_btree_search_static();
}


#elif defined(TMAIN_KAUTIL_ARGORITHM_BTREE_SEARCH_STATIC_2)


#include <vector>
#include <stdint.h>
#include <iostream>
#include "btree_search.h"


// prepair to create dictionary of sqlite hash/rowid
// tsearch :
    // i couldn't understand why sample program didn't cause memory leak without destroy
    // == by runing printf memory leak disappeared.


/// @return callback
///   -1 : l < r
///   1  : l > r
///   0  : l == r
//int compare_function(const key_t * l,const key_t * r);

struct hash_and_rowid{ uint64_t abc[5]; };

#define __byte40(v) reinterpret_cast<uint64_t*>(reinterpret_cast<struct hash_and_rowid*>(const_cast<void*>(v))->abc)
static int __compare( const void * key,const void * hash_and_rowid){
    for(int8_t i = 3; i != -1; --i){
        auto a  = __byte40(key);
        auto b  = __byte40(hash_and_rowid);
//        std::cout <<  __byte40(key)[i] << " " << __byte40(hash_and_rowid)[i] << std::endl;
        if(__byte40(key)[i] == __byte40(hash_and_rowid)[i]) continue;
        if(__byte40(key)[i] < __byte40(hash_and_rowid)[i]) return -1;
        if(__byte40(key)[i] > __byte40(hash_and_rowid)[i]) return 1;
    }
    return 0;
}
#undef __byte40

#define __byte40(v) reinterpret_cast<uint64_t*>(reinterpret_cast<struct hash_and_rowid*>(v))
static int __compare_v3( intptr_t key,intptr_t  hash_and_rowid,void * arg){
    for(int8_t i = 3; i != -1; --i){
        auto a  = __byte40(key);
        auto b  = __byte40(hash_and_rowid);
//        std::cout <<  __byte40(key)[i] << " " << __byte40(hash_and_rowid)[i] << std::endl;
        if(__byte40(key)[i] == __byte40(hash_and_rowid)[i]) continue;
        if(__byte40(key)[i] < __byte40(hash_and_rowid)[i]) return -1;
        if(__byte40(key)[i] > __byte40(hash_and_rowid)[i]) return 1;
    }
    return 0;
}
#undef __byte40


hash_and_rowid set(hash_and_rowid & v,uint64_t _0,uint64_t _1,uint64_t _2,uint64_t _3){
    reinterpret_cast<uint64_t*>(v.abc)[0] = _0;
    reinterpret_cast<uint64_t*>(v.abc)[1] = _1;
    reinterpret_cast<uint64_t*>(v.abc)[2] = _2;
    reinterpret_cast<uint64_t*>(v.abc)[3] = _3;
    return v;
};



int main0(){


    {
        // char[40] == uint64_t[5]
        // assumed hash sha3-256
            // 0
            // 1
            // 2
            // 3
        // assumed rowid
            // 4


        constexpr auto len = 8;
        hash_and_rowid a[len];
        auto data_size = uint64_t (sizeof(a));
        auto step = uint64_t (data_size/len);
        auto cnt =0;
        set(a[cnt++],1,4,7,10);
        set(a[cnt++],1,4,7,10);
        set(a[cnt++],1,4,7,10);
        set(a[cnt++],1,4,7,10);
        set(a[cnt++],123,123,123,123);
        set(a[cnt++],123,123,123,123);
        set(a[cnt++],321,456,789,101112);
        set(a[cnt++],321,456,789,101112);
        auto b = reinterpret_cast<hash_and_rowid*>(a);
        void * res = nullptr;
        auto buf = hash_and_rowid{};
        for(auto & key : {
//            set(buf,123,123,123,123)
//            ,set(buf,320,456,789,101112)
//            set(buf,321,456,789,101112)
            set(buf,322,456,789,101112)
//            ,set(buf,123,123,123,0)
        }){

            if(!kautil_bt_search(&res,&key,b,data_size,step, __compare)){
                std::cout << "*" << "found" << std::endl;
                std::cout << *reinterpret_cast<uint64_t*>(res) << std::endl;
            }else{
                std::cout << "*" << "not found" << std::endl;
                std::cout << (uintptr_t(res) - uintptr_t(b)) / sizeof(hash_and_rowid) << std::endl;

            }
        }

    }




    return 0;
}

int main1(){
    {
        constexpr auto len = 4;
        hash_and_rowid a[len];
        auto data_size = uint64_t (sizeof(a));
        auto step = uint64_t (data_size/len);
        auto cnt =0;
        set(a[cnt++],1596294928687370694ull ,1390175245591044143ull ,2426613603486987655ull ,3103576262232253609ull);
        set(a[cnt++],5165459303042365843ull, 5744917992146087351ull ,1433916237725883015ull ,6793844683605979618ull);
        set(a[cnt++],4400573060620868831ull, 3886628451938246357ull, 15007925159579167585ull,11183394398599062877ull);
        set(a[cnt++],4516988236345250802ull ,14199077436836221109ull ,5925493545772223505ull,15662752953940500775ull);
        auto b = reinterpret_cast<hash_and_rowid*>(a);
        hash_and_rowid * res = nullptr;


        auto buf = hash_and_rowid{};
        for(auto & key : {
            set(buf,1596294928687370694ull ,1390175245591044143ull ,2426613603486987655ull ,3103576262232253609ull)
            ,set(buf,5165459303042365843ull, 5744917992146087351ull ,1433916237725883015ull ,6793844683605979618ull)
            ,set(buf,4516988236345250802ull ,14199077436836221109ull ,5925493545772223505ull ,15662752953940500775ull)
            ,set(buf,4400573060620868831ull, 3886628451938246357ull, 15007925159579167585ull, 11183394398599062877ull)
        }){

            if(!kautil_bt_search((void **)&res,&key,b,data_size,step, __compare)){
                std::cout << "*" << "found" << std::endl;
                std::cout << *reinterpret_cast<uint64_t*>(res) << std::endl;
            }else{
                std::cout << "*" << "not found" << std::endl;

                for(auto i = 0; i < 4; ++i) std::cout << reinterpret_cast<const uint64_t*>(key.abc)[i] << " ";
                std::cout << std::endl;
            }


            if(!kautil_bt_search_v3((void **)&res,&key,b,b+len,step,nullptr,__compare_v3)){
                std::cout << "+" << "found" << std::endl;
                std::cout << *reinterpret_cast<uint64_t*>(res) << std::endl;
            }else{
                std::cout << "+" << "not found" << std::endl;

                for(auto i = 0; i < 4; ++i) std::cout << reinterpret_cast<const uint64_t*>(key.abc)[i] << " ";
                std::cout << std::endl;
            }


        }

    }
    return 0;
}


int main(){ return main1(); }



#endif