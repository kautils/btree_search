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
    
    /** @note side-effect  
     *  [nearest_pos]
     *  [nearest_direction] : (-1,0,1) (left,exact,right) 
     **/
    bool search(value_type const& want,offset_type * nearest_pos = nullptr, int * nearest_direction = nullptr){
        
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
        auto upper_limit_size = prf->max_size();
        auto max_size = upper_limit_size; 
        auto min_size = offset_type(0);
        auto entire_direction = 1;
        auto entire_direction_init = false;
        auto block_size = prf->block_size();
        //auto is_continue = true;
        auto max_size__ = offset_type (0); 
        auto min_size__ = offset_type (0); 
        
        
        for(;res;){
            auto mid = ((max_size-min_size) /  2 + min_size);
            auto adj = mid % (block_size);
            
            l.b =     (mid > adj + (block_size))*static_cast<offset_type>(mid - adj - (block_size))
                    +!(mid > adj + (block_size))*lower_limit_size;
            r.b = (mid > adj)*static_cast<offset_type>(mid - adj ) + !(mid > adj)*lower_limit_size;
            
            // confirm to check both pole(lb,rb) because i ignore calclation of rb if possible.
            if( (2==(max_size==max_size__)+ (min_size==min_size__)) + r.break_bit +  (l.b >=upper_limit_size) + (r.b>=upper_limit_size) + (l.b<lower_limit_size) + (r.b<lower_limit_size))break;
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
        res = r.break_bit + !r.break_bit*res ;
        
        //printf("---- %d %ld [%lld %lld] (%lld %lld) |%ld %ld|\n",res,pos,l.b,r.b,*l.v,*r.v,min_size,max_size); fflush(stdout);
        
        if(nearest_pos){
            auto a = (want>*r.v)*(want - *r.v)+!(want>*r.v)*(*r.v-want); 
            auto b = (want>*l.v)*(want - *l.v)+!(want>*l.v)*(*l.v-want); 
            *nearest_pos = static_cast<offset_type>(!(a<b))*l.b + static_cast<offset_type>(a<b)*r.b; // if abs is equal, l is prior 
            if(nearest_direction)*nearest_direction= !exact*res;
        }
        
        return exact;
    }
    
    
    
    struct btree_search_result{
        //offset_type * nearest_pos;
        
        value_type lv;
        offset_type lpos; 
        
        value_type rv;
        offset_type rpos; 
        
        bool found = false;
        
        
    };


    
    /** @note side-effect  
     *  [nearest_pos]
     *  [nearest_direction] : (-1,0,1) (left,exact,right) 
     **/
    btree_search_result __search(value_type const& want){
        
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
        auto upper_limit_size = prf->max_size();
        auto max_size = upper_limit_size; 
        auto min_size = offset_type(0);
        auto entire_direction = 1;
        auto entire_direction_init = false;
        auto block_size = prf->block_size();
        //auto is_continue = true;
        auto max_size__ = offset_type (0); 
        auto min_size__ = offset_type (0); 
        
        
        for(;res;){
            auto mid = ((max_size-min_size) /  2 + min_size);
            auto adj = mid % (block_size);
            
            l.b =     (mid > adj + (block_size))*static_cast<offset_type>(mid - adj - (block_size))
                    +!(mid > adj + (block_size))*lower_limit_size;
            r.b = (mid > adj)*static_cast<offset_type>(mid - adj ) + !(mid > adj)*lower_limit_size;
            
            // confirm to check both pole(lb,rb) because i ignore calclation of rb if possible.
            if( (2==(max_size==max_size__)+ (min_size==min_size__)) + r.break_bit +  (l.b >=upper_limit_size) + (r.b>=upper_limit_size) + (l.b<lower_limit_size) + (r.b<lower_limit_size))break;
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
        res = r.break_bit + !r.break_bit*res ;
        
        //printf("---- %d %ld [%lld %lld] (%lld %lld) |%ld %ld|\n",res,pos,l.b,r.b,*l.v,*r.v,min_size,max_size); fflush(stdout);
//        if(nearest_pos){
//        }
        offset_type nearest_pos=0;value_type nearest_value=0;       
        offset_type next_pos=0;value_type next_value=0;       
        int direction=0;{
            auto a = (want>*r.v)*(want - *r.v)+!(want>*r.v)*(*r.v-want); 
            auto b = (want>*l.v)*(want - *l.v)+!(want>*l.v)*(*l.v-want); 
            nearest_pos = static_cast<offset_type>(!(a<b))*l.b + static_cast<offset_type>(a<b)*r.b; // if abs is equal, l is prior 
            nearest_value = !(a<b)* *l.v + (a<b)* *r.v; // if abs is equal, l is prior 
            direction = !exact*res;
            
            next_pos = static_cast<offset_type>(!(a<b))*(nearest_pos+block_size) + static_cast<offset_type>(a<b)*(nearest_pos-block_size); 
            
            value_type * next_value_ptr=0;
            prf->read_value(next_pos,&next_value_ptr);
            next_value=*next_value_ptr;
            
        }
        
        
        //auto next_pos = nearest_pos+(direction*block_size);
        //auto next_value = value_type{};
        

        auto result = btree_search_result{};
        {
            {
                auto ptr = (offset_type *) (uintptr_t(nearest_value >= want)*uintptr_t(&result.lpos) + !uintptr_t(nearest_value >= want)*uintptr_t(&result.rpos));
                *ptr = nearest_pos;
            }
            
            {
                auto ptr = (offset_type *) (uintptr_t(nearest_value >= want)*uintptr_t(&result.lv) + !uintptr_t(nearest_value >= want)*uintptr_t(&result.rv));
                *ptr = nearest_value;
            }
        }

        {
            {
                auto ptr = (offset_type *) (uintptr_t(next_value >= want)*uintptr_t(&result.lpos) + !uintptr_t(next_value >= want)*uintptr_t(&result.rpos));
                *ptr = next_pos;
            }
            
            
            {
                auto ptr = (offset_type *) (uintptr_t(next_value >= want)*uintptr_t(&result.lv) + !uintptr_t(next_value >= want)*uintptr_t(&result.rv));
                *ptr = next_value;
            }
        }
        
//        result.lv = *l.v;
//        result.lpos = l.b;
//        result.rv = *r.v;
//        result.lpos = r.b;
        result.found = exact;
        return result;
        
        
        
        
        //return exact;
    }
    
private:
    pref * prf=0;
};



}// namespace algorithm
}// namespace kautil


#endif