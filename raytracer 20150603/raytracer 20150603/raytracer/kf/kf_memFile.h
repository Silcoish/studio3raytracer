#ifndef KF_MEMORY_FILE_HEADER
#define KF_MEMORY_FILE_HEADER

#include "kf/kf_memBlock.h"
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include <ostream>
#include <string>

// to do: add search to memBlock

namespace kf
{
   class KF_API MemFile
   {
   public:
      MemFile();
      MemFile(const std::string &name, bool read_only = true, bool create_new = false, u32 size = 0);
      ~MemFile();
      bool open(const std::string &name, bool read_only = true);
      bool create(const std::string &name, u32 size);
      void close();
      bool is_good() const;
      MemBlock get_block();
   protected:
      bool map();
      HANDLE m_handle_file;
      HANDLE m_handle_map;
      u32 m_size;
      bool m_read_only;
      MemBlock m_mem_block;
      std::string m_name;
   };
}

#endif
