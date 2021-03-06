//
// Copyright (c) Microsoft Corporation.  All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//
#pragma once

namespace rh { namespace util
{
    //---------------------------------------------------------------------------------------------
    // Represents value range [a,b), and provides various convenience methods.

    template <typename VALUE_TYPE, typename LENGTH_TYPE = VALUE_TYPE>
    class Range
    {
        typedef Range<VALUE_TYPE, LENGTH_TYPE> THIS_T;

    public:
        //-----------------------------------------------------------------------------------------
        // Construction

        Range()
            : m_start(0),
              m_end(0)
            {}

        Range(Range const & range)
            : m_start(range.m_start),
              m_end(range.m_end)
            {}

        template <typename VALUE_TYPE>
        static Range<VALUE_TYPE> CreateWithEndpoint(VALUE_TYPE start,
                                                    VALUE_TYPE end)
            { return Range<VALUE_TYPE>(start, end); }

        template <typename VALUE_TYPE, typename LENGTH_TYPE>
        static Range<VALUE_TYPE> CreateWithLength(VALUE_TYPE start, LENGTH_TYPE len)
            { return Range<VALUE_TYPE>(start, start + len); }

        //-----------------------------------------------------------------------------------------
        // Operations

        THIS_T& operator=(THIS_T const & range)
            { m_start = range.m_start; m_end = range.m_end; return *this; }

        bool Equals(THIS_T const & range) const
            { return GetStart() == range.GetStart() && GetEnd() == range.GetEnd(); }

        bool operator==(THIS_T const & range) const
            { return Equals(range); }

        bool operator!=(THIS_T const & range) const
            { return !Equals(range); }

        VALUE_TYPE GetStart() const
            { return m_start; }

        VALUE_TYPE GetEnd() const
            { return m_end; }

        LENGTH_TYPE GetLength() const
            { return m_end - m_start; }

        bool IntersectsWith(THIS_T const &range) const
            { return range.GetStart() < GetEnd() && range.GetEnd() > GetStart(); }

        bool IntersectsWith(VALUE_TYPE start,
                            VALUE_TYPE end) const
            { return IntersectsWith(THIS_T(start, end)); }

        bool Contains(THIS_T const &range) const
            { return GetStart() <= range.GetStart() && range.GetEnd() <= GetEnd(); }

        bool IsAdjacentTo(THIS_T const &range) const
            { return GetEnd() == range.GetStart() || range.GetEnd() == GetStart(); }

    protected:
        Range(VALUE_TYPE start, VALUE_TYPE end)
            : m_start(start),
              m_end(end)
            { ASSERT(start <= end); }

        VALUE_TYPE m_start;
        VALUE_TYPE m_end;
    };
    
    //---------------------------------------------------------------------------------------------
    // Represents address range [a,b), and provides various convenience methods.

    class MemRange : public Range<UInt8*, UIntNative>
    {
        typedef Range<UInt8*, UIntNative> BASE_T;

    public:
        //-----------------------------------------------------------------------------------------
        // Construction

        MemRange()
            : BASE_T()
            {}

        MemRange(void* pvMemStart,
                 UIntNative cbMemLen)
            : BASE_T(reinterpret_cast<UInt8*>(pvMemStart), reinterpret_cast<UInt8*>(pvMemStart) + cbMemLen)
            {}

        MemRange(void* pvMemStart,
                 void* pvMemEnd)
            : BASE_T(reinterpret_cast<UInt8*>(pvMemStart), reinterpret_cast<UInt8*>(pvMemEnd))
            {}

        MemRange(MemRange const & range)
            : BASE_T(range)
            { }

        //-----------------------------------------------------------------------------------------
        // Operations

        MemRange& operator=(MemRange const & range)
            { BASE_T::operator=(range); return *this; }

        UIntNative GetPageCount() const
        {
            UInt8 *pCurPage = ALIGN_DOWN(GetStart(), OS_PAGE_SIZE);
            UInt8 *pEndPage = ALIGN_UP(GetEnd(), OS_PAGE_SIZE);
            return (pEndPage - pCurPage) / OS_PAGE_SIZE;
        }

        UInt8* GetStartPage() const
            { return ALIGN_DOWN(GetStart(), OS_PAGE_SIZE); }

        // The page immediately following the last page contained by this range.
        UInt8* GetEndPage() const
            { return ALIGN_UP(GetEnd(), OS_PAGE_SIZE); }

        MemRange GetPageRange() const
            { return MemRange(GetStartPage(), GetEndPage()); }
    };
}// namespace util
}// namespace rh

