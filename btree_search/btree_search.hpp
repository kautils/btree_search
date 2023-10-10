#ifndef KAUTIL_ALGORITHM_BTREE_SEARCH_BTREE_SEARCH_BTREE_SEARCH_H
#define KAUTIL_ALGORITHM_BTREE_SEARCH_BTREE_SEARCH_BTREE_SEARCH_H

namespace kautil{
namespace algorithm{


template <typename pref>
struct btree_search{
    
    using  value_type= typename pref::value_type;
    using  offset_type= typename pref::offset_type;
    
    btree_search(pref * prf) : prf(prf){}
    
    /// @return true if find [want]
    struct btree_search_result{
        //offset_type * nearest_pos;
        value_type nearest_value;
        offset_type nearest_pos; 
        
        value_type neighbor_value;
        offset_type neighbor_pos; 
        
        int direction = 0;
        bool overflow = false;
        pref * prf=0;
        
    };


    /** @note side-effect  
     *  [nearest_pos]
     *  [nearest_direction] : (-1,0,1) (left,exact,right) 
     **/
    btree_search_result search(value_type const& want,int neighbor_value=true /* true/false */){
        
        struct value_info{
            value_type ent;
            value_type *v=&ent; // in case of wanting to avoid copying value. for future, especially bigint. 
            offset_type b = offset_type(0);
            bool done=false;
            bool break_bit = false;
        };
        
        auto l = value_info{};
        auto r = value_info{};
        
        bool exact = false;
        auto res = short(-2);
        auto pos = offset_type(0);
        auto lower_limit_size = offset_type(0);
        auto upper_limit_size = prf->size();
        auto max_size = upper_limit_size; 
        auto min_size = offset_type(0);
        auto entire_direction = 1;
        auto entire_direction_init = false;
        auto block_size = prf->block_size();
        auto max_size__ = offset_type (0); 
        auto min_size__ = offset_type (0); 
        auto is_overflow_max=false;
        auto is_overflow_min=false;
        
        for(;res;){
            auto mid = ((max_size-min_size) /  2 + min_size);
            auto adj = mid % (block_size);
            
            l.b =     (mid > adj + (block_size))*static_cast<offset_type>(mid - adj - (block_size))
                    +!(mid > adj + (block_size))*lower_limit_size;
            r.b = (mid > adj)*static_cast<offset_type>(mid - adj ) + !(mid > adj)*lower_limit_size;
            
            is_overflow_max=(l.b >=upper_limit_size)+(r.b>=upper_limit_size);
            is_overflow_min=(l.b<lower_limit_size) +(r.b<lower_limit_size);
            // confirm to check both pole(lb,rb) because i ignore calclation of rb if possible.
            if((2==(max_size==max_size__)+ (min_size==min_size__)) 
                +r.break_bit
                +is_overflow_max
                +is_overflow_min){
                break;
            } 
            
            min_size__=min_size; 
            max_size__=max_size;
            
            prf->read_value(l.b,&l.v);
            l.done=true;
            res = /*((want == lv) * 0) +*/ ((want < *l.v)*-1) + (!(want < *l.v)/* *1 */);  
            pos=static_cast<offset_type>(l.b-(block_size)*bool(res));

            if(res==1){
                prf->read_value(r.b,&r.v);
                r.done=true;
                res = /*((want == rv) * 0) +*/ ((want > *r.v)/* *1 */);  
                r.break_bit = !(want > *r.v);
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
        // printf("---- %d %ld [%lld %lld] (%lld %lld) |%ld %ld|\n",res,pos,l.b,r.b,*l.v,*r.v,min_size,max_size); fflush(stdout);
        
        {
            auto result = btree_search_result{};
            auto a = (want>*r.v)*(want - *r.v)+!(want>*r.v)*(*r.v-want); 
            auto b = (want>*l.v)*(want - *l.v)+!(want>*l.v)*(*l.v-want); 
            result.nearest_pos = static_cast<offset_type>(!(a<b))*l.b + static_cast<offset_type>(a<b)*r.b; // if abs is equal, l is prior 
            result.nearest_pos = (result.nearest_pos>=upper_limit_size)*(upper_limit_size-block_size) + !(result.nearest_pos>=upper_limit_size)*result.nearest_pos; 
            result.nearest_value = !(a<b)* *l.v + (a<b)* *r.v; // if abs is equal, l is prior 
            result.direction = !exact*((want > result.nearest_value)*1+(want < result.nearest_value)*-1);
            //result.overflow = is_overflow_max+is_overflow_min;
            result.overflow=
                 (2==(upper_limit_size<=pos)+(res>0))
                +(2==(lower_limit_size>=pos)+(res<0) );
            
            if(neighbor_value){
                if(!exact*!result.overflow){
                    result.neighbor_pos = static_cast<offset_type>(!(a<b))*(result.nearest_pos+block_size) + static_cast<offset_type>(a<b)*(result.nearest_pos-block_size); 
                    if(r.b==result.neighbor_pos){
                        result.neighbor_value = *r.v;
                    }else{
                        auto neighbor_value_ptr=&result.neighbor_value;
                        prf->read_value(result.neighbor_pos,&neighbor_value_ptr);
                        if(neighbor_value_ptr)result.neighbor_value=*neighbor_value_ptr;
                    }
                }
            }
            return result;
        }
    }
    
    
    
private:
    pref * prf=0;
};



}// namespace algorithm
}// namespace kautil


#endif