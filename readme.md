### kautil_btree_search
* template class for btree search. 

### note
* int8,int16,int32,uint8,uint16,uint32,float,double and array of them.
* both filesystem and memory.
* both systemcall and non-systemcall.
* haeder only

### example
```c++
#ifdef TMAIN_KAUTIL_ALGORITHM_BTREE_SEARCH_INTERFACE

#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include "stdint.h"
#include "btree_search.hpp"

int file_syscall_16b(uint64_t const & search_v){
    
    struct file_syscall_16b_pref{
        using value_type = uint64_t;
        using offset_type = long;

        int fd=-1;
        
        offset_type block_size(){ return sizeof(value_type)*2; }
        offset_type max_size(){
            struct stat st;
            fstat(fd,&st);
            return static_cast<uint64_t>(st.st_size);
        }

        void read_value(offset_type const& offset, value_type ** value){
            lseek(fd,offset,SEEK_SET);
            read(fd,*value,sizeof(value_type));
        }
    };
    
    
    auto f = "./tmain_kautil_cache_file_cache_static";
    remove(f);

    auto fd = int(-1);
    {
        struct stat st;
        if(stat(f,&st)){
            fd = open(f,O_CREAT|O_EXCL|O_RDWR,0755);
        }else{
            fd = open(f,O_RDWR);
        }
    }
    for(auto i = 0; i < 100 ; ++i){
        auto beg = file_syscall_16b_pref::value_type(i*10);
        auto end = beg+10;
        auto cur = tell(fd);
        auto block_size = sizeof(file_syscall_16b_pref::value_type);
        write(fd,&beg,block_size);
        lseek(fd,cur+block_size,SEEK_SET);
        write(fd,&end,block_size);
        lseek(fd,cur+block_size*2,SEEK_SET);
    }
    lseek(fd,0,SEEK_SET);
    
    

    { // main 
        auto pref = file_syscall_16b_pref{.fd=fd};
        auto bt = kautil::algorithm::btree_search{&pref};
        auto pos = file_syscall_16b_pref::offset_type(0);
        auto dir = int(0);
        auto found = bt.search(static_cast<file_syscall_16b_pref::value_type>(search_v),&pos,&dir);
        printf("%s. pos is %ld. direction is %d\n",found?"found": "not found",pos,dir);

        return 0;
    }
}

int memory_16b(uint64_t const& search_v){
        
    struct memory_16b_arr_pref{
        using value_type = uint64_t;
        using offset_type = uint64_t;

        value_type * data =0;
        offset_type size = 0;
        
        offset_type block_size(){ return sizeof(value_type)*2; }
        offset_type max_size(){ return size; }
        void read_value(offset_type const& offset, value_type ** value){ *value=&data[offset/sizeof(value_type)]; }
        
    };

    constexpr auto len = 100; 
    memory_16b_arr_pref::value_type data[len*2];
    auto cnt = 0;
    for(auto i = 0; i <len*2; i+=2){
        data[i] = cnt*10;
        data[i+1] = data[i]+10;
        ++cnt;
    }
    
    { // main 
        auto pref = memory_16b_arr_pref{.data=data,.size=sizeof(data)};
        auto pos = memory_16b_arr_pref::offset_type(0);
        auto dir = int(0);
        auto bt = kautil::algorithm::btree_search{&pref};
        auto found = bt.search(static_cast<memory_16b_arr_pref::value_type>(search_v),&pos,&dir);
        printf("%s. pos is %lld. direction is %d\n",found?"found": "not found",pos,dir);
        return bt.search(static_cast<memory_16b_arr_pref::value_type>(search_v));
        
    }
}


int memory_8b(uint64_t const& search_v ){
    struct memory_8b_arr_pref{
        using value_type = uint64_t;
        using offset_type = uint64_t;
    
        value_type * data =0;
        offset_type size = 0;
        
        offset_type block_size(){ return sizeof(value_type); }
        offset_type max_size(){ return size; }
        void read_value(offset_type const& offset, value_type ** value){ *value=&data[offset/sizeof(value_type)]; }
    };

    constexpr auto len = 100; 
    memory_8b_arr_pref::value_type data[len];
    //auto search_v=memory_8b_arr_pref::value_type(550);
    auto cnt = 0;
    for(auto i = 0; i <len; ++i) data[i] = i * 10;
    auto pref = memory_8b_arr_pref{.data=data,.size=sizeof(data)};
    {// main
        auto pos = memory_8b_arr_pref::offset_type(0);
        auto dir = int(0);
        auto bt = kautil::algorithm::btree_search{&pref};
        auto found = bt.search(static_cast<memory_8b_arr_pref::value_type>(search_v),&pos,&dir);
        printf("%s. pos is %lld. direction is %d\n",found?"found": "not found",pos,dir);
        return bt.search(static_cast<memory_8b_arr_pref::value_type>(search_v));
    }
}

int main(){
        
    auto search_v = uint64_t(90);
    
    //  16 bytes array
    file_syscall_16b(search_v);
    memory_16b(search_v);
    
    // 8 bytes array
    memory_8b(search_v);
    
    
    return 0;
}

#endif


```