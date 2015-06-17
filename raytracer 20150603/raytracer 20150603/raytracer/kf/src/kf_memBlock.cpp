#include "kf/kf_memBlock.h"

namespace kf
{
	namespace
	{
		u8 bytetohex(u8 b)
		{
			if((b & 0x0f) < 10)
			{
				return '0' + (b & 0x0f);
			}
			else
			{
				return 'A' + ((b & 0x0f) - 10);
			}
		}
	}

	MemBlock::MemBlock()
	{
		m_begin = NULL;
		m_end = NULL;
		m_current = m_begin;
	}

	MemBlock::MemBlock(u8 *start, u64 length)
	{
		m_begin = start;
		m_end = m_begin + length;
		m_current = m_begin;
	}

	MemBlock::~MemBlock()
	{
	}

	void MemBlock::setRange(u8 *start, u64 length)
	{
		m_begin = start;
		m_end = m_begin + length;
		m_current = m_begin;
	}

	bool MemBlock::seek(s64 offset, bool relative)
	{
		if(relative)
		{
			if(offset>=0) 
			{
				u64 distance = m_end - m_current;
				if((u64)offset <= distance)
				{
					m_current += offset;
					return true;
				}
				else
				{
					m_current += distance;
					return false;
				}
			}
			else
			{
				offset = -offset;
				u64 distance = m_current - m_begin;
				if((u64)offset <= distance)
				{
					m_current -= offset;
					return true;
				}
				else
				{
					m_current -= distance;
					return false;
				}
			}
		}
		else
		{
			if(m_end - m_begin > (u64)offset)
			{
				m_current = m_begin + (u64)offset;
				return true;
			}
			else
			{
				m_current = m_end;
				return false;
			}
		}
	}

	MemBlock &MemBlock::operator+=(s64 offset)
	{
		seek(offset, true);
		return *this;
	}

	MemBlock &MemBlock::operator-=(s64 offset)
	{
		seek(offset, true);
		return *this;
	}

	MemBlock &MemBlock::operator++()
	{
		if(m_current != m_end)
		{
			m_current++;
		}
		return *this;
	}

	MemBlock &MemBlock::operator--()
	{
		if(m_current != m_begin)
		{
			m_current--;
		}
		return *this;
	}

	bool MemBlock::operator==(const MemBlock &rhs)
	{
		return m_current == rhs.m_current;
	}

	MemBlock &MemBlock::operator=(const MemBlock &rhs)
	{
		m_current = rhs.m_current;
		m_begin = rhs.m_begin;
		m_end = rhs.m_end;
		return *this;
	}

	MemBlock MemBlock::operator()(s64 offset, bool relative)
	{
		MemBlock m = *this;
		m.seek(offset, relative);
		return m;
	}

	u8 *MemBlock::begin() const
	{
		return m_begin;
	}

	u8 *MemBlock::end() const
	{
		return m_end;
	}

	u8 *MemBlock::current() const
	{
		return m_current;
	}

	u64 MemBlock::size() const
	{
		return (m_end - m_begin);
	}

	u64  MemBlock::getU64(bool move)
	{
		u64 nearEnd = m_end - m_current;
		u8 *temp = m_current;
		if(nearEnd >= sizeof(u64))
		{
			if(move)
				operator+=(sizeof(u64));
			return *((u64 *)temp);
		}
		else
		{
			//Todo:
			u64 q;
			q = getU32(true);
			q |= ((u64)getU32(true)) << 32;
			if(!move)
			{
				m_current = temp;
			}
			return q;
		}
	}

	s64  MemBlock::getS64(bool move)
	{
		u64 nearEnd = m_end - m_current;
		u8 *temp = m_current;
		if(nearEnd >= sizeof(s64))
		{
			if(move)
				operator+=(sizeof(s64));
			return *((s64 *)temp);
		}
		else
		{
			//Todo:
			s64 q;
			q = getU32(true);
			q |= ((u64)getU32(true)) << 32;
			if(!move)
			{
				m_current = temp;
			}
			return q;
		}
	}


	u32  MemBlock::getU32(bool move)
	{
		u64 nearEnd = m_end - m_current;
		if(nearEnd >= sizeof(u32))
		{
			u8 *temp = m_current;
			if(move)
				operator+=(sizeof(u32));
			return *((u32 *)temp);
		}
		else if(nearEnd >= 3)
		{
			return getU24(move);
		}
		else if(nearEnd >= sizeof(u16))
		{
			return getU16(move);
		}
		else if(nearEnd >= sizeof(u8))
		{
			return getU8(move);
		}
		return 0;
	}

	s32 MemBlock::getS32(bool move)
	{
		u64 nearEnd = m_end - m_current;
		if(nearEnd >= sizeof(s32))
		{
			u8 *temp = m_current;
			if(move)
				operator+=(sizeof(s32));
			return *((s32 *)temp);
		}
		else if(nearEnd >= 3)
		{
			return getS24(move);
		}
		else if(nearEnd >= sizeof(s16))
		{
			return getS16(move);
		}
		else if(nearEnd >= sizeof(s8))
		{
			return getS8(move);
		}
		return 0;
	}

	u32 MemBlock::getU24(bool move)
	{
		u64 nearEnd = m_end - m_current;
		if(nearEnd >= 3)
		{
			u8 *temp = m_current;
			if(move)
				operator+=(3);
			return temp[0] | (temp[1] << 8) | (temp[2] << 16);
		}
		else if(nearEnd >= sizeof(u16))
		{
			return getU16(move);
		}
		else if(nearEnd >= sizeof(u8))
		{
			return getU8(move);
		}
		return 0;
	}

	s32 MemBlock::getS24(bool move)
	{
		u64 nearEnd = m_end - m_current;
		if(nearEnd >= 3)
		{
			u8 *temp = m_current;
			if(move)
				operator+=(3);
			s32 val = temp[0] | (temp[1] << 8) | (temp[2] << 16);
			if(val & 0x800000) val |= 0xff000000;
			return val;
		}
		else if(nearEnd >= sizeof(u16))
		{
			return getU16(move);
		}
		else if(nearEnd >= sizeof(u8))
		{
			return getU8(move);
		}
		return 0;
	}

	u16 MemBlock::getU16(bool move)
	{
		u64 nearEnd = m_end - m_current;
		if(nearEnd >= sizeof(u16))
		{
			u8 *temp = m_current;
			if(move)
				operator+=(sizeof(u16));
			return *((u16 *)temp);
		}
		else if(nearEnd >= sizeof(u8))
		{
			return getU8(move);
		}
		return 0;
	}

	s16  MemBlock::getS16(bool move)
	{
		u64 nearEnd = m_end - m_current;
		if(nearEnd >= sizeof(s16))
		{
			u8 *temp = m_current;
			if(move)
				operator+=(sizeof(s16));
			return *((s16 *)temp);
		}
		else if(nearEnd >= sizeof(s8))
		{
			return getS8(move);
		}
		return 0;
	}

	u8 MemBlock::getU8(bool move)
	{
		u64 nearEnd = m_end - m_current;
		if(nearEnd >= sizeof(u8))
		{
			u8 *temp = m_current;
			if(move)
				operator+=(sizeof(u8));
			return *((u8 *)temp);
		}
		return 0;
	}

	s8 MemBlock::getS8(bool move)
	{
		u64 nearEnd = m_end - m_current;
		if(nearEnd >= sizeof(s8))
		{
			u8 *temp = m_current;
			if(move)
				operator+=(sizeof(s8));
			return *((s8 *)temp);
		}
		return 0;
	}

	float MemBlock::getFloat(bool move)
	{
		u64 nearEnd = m_end - m_current;
		if(nearEnd >= sizeof(float))
		{
			u8 *temp = m_current;
			if(move)
				operator+=(sizeof(float));
			return *((float *)temp);
		}
		return 0.0f;
	}

	double MemBlock::getDouble(bool move)
	{
		u64 nearEnd = m_end - m_current;
		if(nearEnd >= sizeof(double))
		{
			u8 *temp = m_current;
			if(move)
				operator+=(sizeof(double));
			return *((double *)temp);
		}
		return 0.0;
	}

	void MemBlock::getBlock(u8 *destination, u64 length, bool move)
	{
		u8 *temp = m_current;
		operator+=(length);
		memcpy(destination, temp, (u32)(m_current-temp));
	}

	std::string MemBlock::getString(u64 length, bool move)
	{
		std::string s;
		u8 *temp = m_current;
		if(length > m_end - m_begin)
		{
			length = m_end - m_begin;
		}
		while(m_current < m_end)
		{
			if(*m_current == 0 || (length > 0 && (u32)(m_current - temp) >= length))
			{
				s = std::string(temp, m_current);
				if(length > 0)
				{
					m_current = temp + length;
				}
				else
				{
					m_current++;
				}
				return s;
			}
			m_current++;
		}
		return std::string(temp, m_end);
	}
	
	std::string MemBlock::getHex(u64 length, u64 division, bool move)
	{
		std::string s;
		//s.resize(length * 2 + (division>0)? length / division : 0);
		for(unsigned int i = 0; i < length; i++)
		{
			s += bytetohex(m_current[i] >> 4);
			s += bytetohex(m_current[i] & 0x0f);
			if(division && i%division == division - 1)
			{
				s += ' ';
			}
		}
		if(move)
			operator+=(length);
		return s;
	}

	void MemBlock::setU32(u32 value, bool move)
	{
		*((u32 *)m_current) = value;
		operator+=(sizeof(u32));
	}

	void MemBlock::setS32(s32 value, bool move)
	{
		*((s32 *)m_current) = value;
		operator+=(sizeof(s32));
	}

	void MemBlock::setU16(u16 value, bool move)
	{
		*((u16 *)m_current) = value;
		operator+=(sizeof(u16));
	}

	void MemBlock::setS16(s16 value, bool move)
	{
		*((s16 *)m_current) = value;
		operator+=(sizeof(s16));
	}

	void MemBlock::setU8(u8 value, bool move)
	{
		*((u8 *)m_current) = value;
		operator+=(sizeof(u8));
	}

	void MemBlock::setS8(s8 value, bool move)
	{
		*((s8 *)m_current) = value;
		operator+=(sizeof(s8));
	}

	void MemBlock::setFloat(float value, bool move)
	{
		*((float *)m_current) = value;
		operator+=(sizeof(float));
	}

	void MemBlock::setDouble(double value, bool move)
	{
		*((double *)m_current) = value;
		operator+=(sizeof(double));
	}

	void MemBlock::setBlock(u8 *src, u64 length, bool move)
	{
		u8 *temp = m_current;
		operator+=(length);
		memcpy(temp,src,m_current-temp);
	}

	void MemBlock::setString(const std::string &value, bool include_terminator, u64 length, bool pad, bool move)
	{
		u8 *temp = m_current;
		u64 stringlength = u64(value.size()) + (include_terminator ? 1 : 0);


		if(length > 0)
		{
			if(length < stringlength)
			{
				stringlength = length - (include_terminator ? 1 : 0);
			}
			operator+=(length);
		}
		else
		{
			operator+=(stringlength);
		}
		memcpy(temp, value.c_str(), stringlength);
		if(length > 0 && stringlength < length && pad)
		{
			temp += stringlength;
			for(u64 i = 0; i < length - stringlength; i++)
			{
				if(temp >= m_end)
				{
					return;
				}
				*(temp++) = 0;
			}
		}
	}


	std::ostream &operator<<(std::ostream &os, const MemBlock &src)
	{
		u8 c;
		u8 buf[81]={0};
		u64 i,j;
		u64 address;
		MemBlock mi(src);
		mi.seek(0);

		for(j = 0; j < (mi.size() + 15) / 16; j++)
		{
			for(i = 0; i < 79; i++)
			{
				buf[i]=' ';
			}
			address = ((u64)mi.begin()) + j * 16;
			for(i = 0; i < 8; i++)
			{
				buf[i] = bytetohex((u8)((address >> ((7 - i) * 4)) & 0xff));
			}

			for(i = 0; i < 16; i++)
			{
				if(i + j * 16 >= mi.size())
				{
					break;
				}
				c = mi.getU8();
				buf[i * 3 + 9 + (i/4)] = bytetohex(c >> 4);
				buf[i * 3 + 10 + (i/4)] = bytetohex(c & 0x0f);

				if(c >= 32 && c <= 127)
				{
					buf[63 + i] = c;
				}
				else
				{
					buf[63 + i] = '.';
				}
			}
			os << buf << "\n";
		}
		return os;
	}
}
