#ifndef KF_FLAG_HEADER
#define KF_FLAG_HEADER
#include "vector"
#include "iostream"
#include "string"
#include "kf/kf_types.h"

namespace kf
{
   class KF_API Flag
   {
   public:
      Flag():m_value(0),m_mask(0),m_text("") { }
      Flag(u32 flagmask, u32 flagvalue, const std::string &flagname):m_value(flagvalue),m_mask(flagmask),m_text(flagname) { }
      Flag(u32 flagmask, u32 flagvalue):m_value(flagvalue),m_mask(flagmask)
      {
         char buf[100];
         sprintf_s(buf,100,"%x",flagvalue);
         m_text = buf;
      }

      u32 m_value;
      u32 m_mask;
      std::string m_text;
   };

   class KF_API FlagGroup
   {
   public:
      std::vector<Flag> m_flags;
      void addFlag(u32 flagmask, u32 flagvalue, const std::string &flagname);
      std::string toString(u32 value);

   };

}
#endif
