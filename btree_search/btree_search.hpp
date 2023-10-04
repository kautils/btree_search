#ifndef KAUTIL_ALGORITHM_BTREE_SEARCH_BTREE_SEARCH_BTREE_SEARCH_H
#define KAUTIL_ALGORITHM_BTREE_SEARCH_BTREE_SEARCH_BTREE_SEARCH_H

namespace kautil{
namespace algorithm{

template <typename pref>
struct btree_search{
    
    using  value_type= typename pref::value_type;
    using  offset_type= typename pref::offset_type;
    btree_search(pref * prf) : prf(prf){}
    bool search(value_type const& want,offset_type * nearest_pos = nullptr, int * nearest_direction = nullptr){
        
        value_type lv_ent;
        auto *lv=&lv_ent; // in case of wanting to avoid copying value. for future, especially bigint. 
        auto lb = offset_type(0);
        
        value_type rv_ent;
        auto *rv=&rv_ent;
        auto rb = offset_type(0);
        
        bool exact = false;
        auto res = short(0);
        auto pos = offset_type(0);
        auto lower_limit_size = offset_type(0);
        auto upper_limit_size = prf->max_size();
        auto max_size = upper_limit_size; 
        auto min_size = offset_type(0);
        auto entire_direction = 1;
        auto entire_direction_init = false;
        auto block_size = prf->block_size();
        auto is_continue = true;
        
        
        
        while(is_continue){
            auto mid = ((max_size-min_size) /  2 + min_size);
            auto adj = mid % (block_size);
            lb =   (mid > adj + (block_size))*static_cast<offset_type>(mid - adj - (block_size))
                    +!(mid > adj + (block_size))*lower_limit_size;
            rb = (mid > adj)*static_cast<offset_type>(mid - adj ) + !(mid > adj)*lower_limit_size;

            prf->read_value(lb,&lv);

            res = /*((want == lv) * 0) +*/ ((want < *lv)*-1) + (!(want < *lv)/* *1 */);  
            pos=static_cast<offset_type>(lb-(block_size)*bool(res));

            if(res==1){
                prf->read_value(rb,&rv);
                res = /*((want == rv) * 0) +*/ ((want > *rv)/* *1 */);  
                pos=static_cast<offset_type>(rb+(block_size*bool(res)));
            }

            entire_direction = (entire_direction_init)*entire_direction + !(entire_direction_init)*res; 
            entire_direction_init=true;

            pos= (res>0)*pos;
            max_size=
                  (res>0)*((entire_direction==-1)*pos +!(entire_direction==-1)*max_size)
                +!(res>0)*offset_type(lb); 

            min_size=static_cast<offset_type>(
                  (res>0)*((entire_direction==-1)*(rb+(block_size)) +!(entire_direction==-1)*pos)
                +!(res>0)*min_size );

            //printf("%d %ld [%ld %ld] (%ld %ld) |%ld %ld|\n",res,pos,lb,rb,lv,rv,min_size,max_size); fflush(stdout);
            // confirm to check both pole(lb,rb) because i ignore calclation of rb if possible. 
            is_continue= !( (res==0) + (lb>=upper_limit_size) + (rb>=upper_limit_size) + (lb<=lower_limit_size) + (rb<=lower_limit_size)); 
        }

        exact = (want == *rv) + (want == *lv);
        if(nearest_pos){
            auto a = (want>*rv)*(want - *rv)+!(want>*rv)*(*rv-want); 
            auto b = (want>*lv)*(want - *lv)+!(want>*lv)*(*lv-want); 
            *nearest_pos = (a>b)*lb + (a<b)*rb;
            if(nearest_direction)
                *nearest_direction= !exact*(
                          (a>b)*((want<*lv)*-1+!(want<*lv))
                        +!(a>b)*((want<*rv)*-1+!(want<*rv))
                    );
             //printf("%s. pos is %ld. direction is %d\n",exact?"found": "not found",*nearest_pos,*nearest_direction);
        }
        return exact;
    }
    
private:
    pref * prf=0;
};



}// namespace algorithm
}// namespace kautil


#endif