#ifndef KF_MEMORY_POOL_HEADER
#define KF_MEMORY_POOL_HEADER

#include "kf/kf_types.h"
#include "kf/kf_math.h"
#include "vector"

namespace kf
{
   template<typename T>
   class MemoryPool
   {
   public:
      MemoryPool(u32 element_count)
      {
         m_element_count_total = 0;
         m_element_count_per_block = element_count;
         m_element_size = sizeof(T);
      }
      ~MemoryPool()
      {
         for(unsigned int i=0;i<m_blocks.size();i++)
         {
            delete [] m_blocks[i];
         }
      }
      T &operator[](unsigned int index)
      {
         if(index >= m_element_count_total)
         {
            index = m_element_count_total-1;
         }
         return m_blocks[index/m_element_count_per_block][index%m_element_count_per_block];
      }
      u32 append(const T &elem)
      {
         if(m_element_count_total % m_element_count_per_block == 0)
         {
            m_blocks.push_back(new T[m_element_count_per_block]);
         }
         m_blocks[m_element_count_total / m_element_count_per_block][m_element_count_total % m_element_count_per_block] = elem;
         return m_element_count_total++;
      }
      u32 size()
      {
         return m_element_count_total;
      }
      

   protected:
      u32 m_element_size;
      u32 m_element_count_per_block;
      std::vector<T *> m_blocks;
      u32 m_element_count_total;
   };
}
#endif
