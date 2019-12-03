#ifndef _voxelquest_cache_h_
#define _voxelquest_cache_h_

bool checkCacheEntry(int blockId, int chunkId, int holderId, bool doErase=false);

bool loadCacheEntry(int blockId, int chunkId, int holderId);

bool saveCacheEntry(int blockId, int chunkId, int holderId);

bool loadCacheMetaData();

bool saveCacheMetaData();

void clearCache();

bool updateCurCacheLoc();

#endif//_voxelquest_cache_h_
