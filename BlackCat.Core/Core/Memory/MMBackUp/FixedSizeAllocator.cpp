//  [8/23/2013 MRB]

#include "CorePCH.h"
#include "Allocators.h"
#include "CoreExceptions.h"

namespace BlackCat
{
	namespace Core
	{
#ifdef BC_MEMORY_ENABLE

		bcFixedSizeAllocator::bcFixedSizeAllocator() 
		{
		}
		bcFixedSizeAllocator::~bcFixedSizeAllocator()
		{
			SAFE_ALIGNED_FREE( mBlocks );
			SAFE_ALIGNED_FREE( mHeap );
		}
		void bcFixedSizeAllocator::initialize(	bcUINT32 pNumBlock, 
												bcUINT32 pBlockSize, 
												const bcCHAR* pTag)
		{
			mNumBlock = pNumBlock;
			mBlockSize = pBlockSize;

			if(mNumBlock % FIXEDSIZE_INDEX_SIZE == 0)
			{
				mNumBitBlocks = mNumBlock / FIXEDSIZE_INDEX_SIZE;
			}
			else
			{
				mNumBlock = ((mNumBlock / FIXEDSIZE_INDEX_SIZE) + 1) * FIXEDSIZE_INDEX_SIZE;
				mNumBitBlocks = mNumBlock / FIXEDSIZE_INDEX_SIZE;
			}
			mAllocatedBlock = 0;

			mBlocks = (FIXEDSIZE_INDEX_TYPE*)_aligned_malloc( mNumBitBlocks * sizeof(FIXEDSIZE_INDEX_TYPE), BC_MEMORY_MIN_ALIGN );
			if(!mBlocks)
				throw Exception(L"Memory Allocation Error");
			std::memset(mBlocks, 0, mNumBitBlocks * sizeof(FIXEDSIZE_INDEX_TYPE));
			
			// == We alloc mHeap by min align defined in CorePCH because we
			// want all of our allocations have MIN_Align ========== MRB ==
			mHeap = (bcUBYTE*)_aligned_malloc((mBlockSize * mNumBlock) * sizeof(bcUBYTE), BC_MEMORY_MIN_ALIGN);
			if(!mHeap)
			{
				SAFE_ALIGNED_FREE(mBlocks);
				throw Exception(L"Memory Allocation Error");
			}

			bcAllocator::mTracer.initialize((mBlockSize * mNumBlock) * sizeof(bcUBYTE));
			bcAllocator::tag(pTag);
		}

		void* bcFixedSizeAllocator::alloc(MemBlock* pMemBlock)
		{
			bcUINT32 lSize = pMemBlock->size();

			// == we can't handle allocations larger than our block size == MRB ==
			bcAssert( lSize <= mBlockSize );
			bcAssert( lSize > 0 );

			// == Search for a free block == MRB ==
			int lBlock = -1;
			void* lResult = bcNULL;

			bcBOOL lReachEnd = bcFALSE;
			for( register bcINT32 i = mAllocatedBlock; (i != mAllocatedBlock || !lReachEnd); ++i)
			{
				if(i >= mNumBitBlocks)
				{
					i = 0;
					lReachEnd = bcTRUE;
				}
				FIXEDSIZE_INDEX_TYPE& lCurrentBlock = mBlocks[i];

				// any free blocks in this chunk?
				if ( FIXEDSIZE_INDEX_MASK != lCurrentBlock )
				{
					// find a free entry in this chunk
					// then allocate it and set the proper block index
					for ( int j = 0; j < FIXEDSIZE_INDEX_SIZE; ++j )
					{
						if ( FIXEDSIZE_INDEX_TYPE(0) == (lCurrentBlock & (FIXEDSIZE_INDEX_TYPE(1) << j)) )
						{
							lCurrentBlock |= (FIXEDSIZE_INDEX_TYPE(1) << j);
							lBlock = i * FIXEDSIZE_INDEX_SIZE + j;
							// Store current block. For next allocation we will start
							// searching from this block
							mAllocatedBlock = i;
							break;
						}
					}
				 
					break;
				}
			}

			// == A free block found == MRB ==
			if ( -1 != lBlock || lBlock <= mNumBlock )
			{
				lResult = reinterpret_cast<void*>((bcUINT32)mHeap + (lBlock * mBlockSize));
				mTracer.acceptAlloc(lSize);
			}
			else
			{
				lResult = bcNULL;
				mTracer.rejectAlloc(lSize);
			}
			
			return lResult;
		}
		void bcFixedSizeAllocator::free(const void* pPointer, MemBlock* pMemBlock)
		{
			bcUBYTE* lPointer = reinterpret_cast<bcUBYTE*>(const_cast<void*>(pPointer));
			
			// is this pointer in our heap?
			bcAssert( (lPointer >= mHeap) && (lPointer < mHeap + mBlockSize * mNumBlock) );

			// convert the pointer into a block index
			bcINT32 block = static_cast<bcINT32>(lPointer - mHeap) / mBlockSize;

			// reset the bit in our block management array
			bcINT32 chunkIndex = block / FIXEDSIZE_INDEX_SIZE;
			bcINT32 bitIndex = block % FIXEDSIZE_INDEX_SIZE;

			mBlocks[chunkIndex] &= ~(FIXEDSIZE_INDEX_TYPE(1) << bitIndex);

			pMemBlock->free(bcTRUE);

			mTracer.acceptFree(pMemBlock->size());
		}

		bcBOOL bcFixedSizeAllocator::containPointer(const void* pPointer)
		{
			return (pPointer >= mHeap && pPointer < mHeap + (mBlockSize * mNumBlock))? bcTRUE: bcFALSE;
		}
		bcBOOL bcFixedSizeAllocator::isEmpty()
		{
			for( bcINT32 i = 0; i < mNumBitBlocks; ++i)
			{
				FIXEDSIZE_INDEX_TYPE& lCurrentBlock = mBlocks[i];

				if ( !FIXEDSIZE_INDEX_MASK != lCurrentBlock )
				{
					return false;
				}
			}

			return true;
		}
		void bcFixedSizeAllocator::clear()
		{
			std::memset(mBlocks, 0, mNumBitBlocks * sizeof(FIXEDSIZE_INDEX_TYPE));
			
			mTracer.acceptClear();
		}
#endif
	}
}