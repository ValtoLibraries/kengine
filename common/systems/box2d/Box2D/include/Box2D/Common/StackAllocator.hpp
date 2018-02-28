/*
* Copyright (c) 2006-2009 Erin Catto http://www.box2d.org
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#ifndef B2_STACK_ALLOCATOR_HPP
#define B2_STACK_ALLOCATOR_HPP

#include <Box2D/Common/Settings.hpp>

namespace b2
{

const int32 stackSize = 100 * 1024;	// 100k
const int32 maxStackEntries = 32;

struct StackEntry
{
	char* data;
	int32 size;
	bool usedMalloc;
};

// This is a stack allocator used for fast per step allocations.
// You must nest allocate/free pairs. The code will assert
// if you try to interleave multiple allocate/free pairs.
class StackAllocator
{
public:
	StackAllocator();
	~StackAllocator();

	void* Allocate(int32 size);
	void Free(void* p);

	int32 GetMaxAllocation() const;

private:

	char m_data[stackSize];
	int32 m_index;

	int32 m_allocation;
	int32 m_maxAllocation;

	StackEntry m_entries[maxStackEntries];
	int32 m_entryCount;
};

} // namespace b2

#endif // B2_STACK_ALLOCATOR_HPP
