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
        
        struct value_info{
            value_type ent;
            value_type *v=&ent; // in case of wanting to avoid copying value. for future, especially bigint. 
            offset_type b = offset_type(0);
            bool done=false;
        };
        
        auto l = value_info{};
        auto r = value_info{};
        
        bool exact = false;
        auto res = short(-2);
        auto pos = offset_type(0);
        auto lower_limit_size = offset_type(0);
        auto upper_limit_size = prf->max_size();
        auto max_size = upper_limit_size; 
        auto min_size = offset_type(0);
        auto entire_direction = 1;
        auto entire_direction_init = false;
        auto block_size = prf->block_size();
        //auto is_continue = true;
        
        for(;res;){
            auto mid = ((max_size-min_size) /  2 + min_size);
            auto adj = mid % (block_size);
            l.b =   (mid > adj + (block_size))*static_cast<offset_type>(mid - adj - (block_size))
                    +!(mid > adj + (block_size))*lower_limit_size;
            r.b = (mid > adj)*static_cast<offset_type>(mid - adj ) + !(mid > adj)*lower_limit_size;
            // confirm to check both pole(lb,rb) because i ignore calclation of rb if possible.
            if((l.b>=upper_limit_size) + (r.b>=upper_limit_size) + (l.b<=lower_limit_size) + (r.b<=lower_limit_size))break; 
            
            prf->read_value(l.b,&l.v);
            l.done=true;
            res = /*((want == lv) * 0) +*/ ((want < *l.v)*-1) + (!(want < *l.v)/* *1 */);  
            pos=static_cast<offset_type>(l.b-(block_size)*bool(res));
            

            if(res==1){
                prf->read_value(r.b,&r.v);
                r.done=true;
                res = /*((want == rv) * 0) +*/ ((want > *r.v)/* *1 */);  
                pos=static_cast<offset_type>(r.b+(block_size*bool(res)));
            }
            

            entire_direction = (entire_direction_init)*entire_direction + !(entire_direction_init)*res; 
            entire_direction_init=true;

            pos= (res>0)*pos;
            max_size=
                  (res>0)*((entire_direction==-1)*pos +!(entire_direction==-1)*max_size)
                +!(res>0)*offset_type(l.b); 

            min_size=static_cast<offset_type>(
                  (res>0)*((entire_direction==-1)*(r.b+(block_size)) +!(entire_direction==-1)*pos)
                +!(res>0)*min_size );
            //printf("%d %ld [%lld %lld] (%lld %lld) |%ld %ld|\n",res,pos,l.b,r.b,*l.v,*r.v,min_size,max_size); fflush(stdout);
        }
        exact = r.done*(want == *r.v) + r.done*(want == *l.v);
        
        
        if(nearest_pos){
            auto a = (want>*r.v)*(want - *r.v)+!(want>*r.v)*(*r.v-want); 
            auto b = (want>*l.v)*(want - *l.v)+!(want>*l.v)*(*l.v-want); 
            *nearest_pos = static_cast<offset_type>(a>b)*l.b + static_cast<offset_type>(a<b)*r.b;
            if(nearest_direction)*nearest_direction= !exact*res;
        }
        
        return exact;
    }
    
private:
    pref * prf=0;
};



}// namespace algorithm
}// namespace kautil


#endif