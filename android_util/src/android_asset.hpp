

#ifndef _ANDROID_ASSET_H_
#define _ANDROID_ASSET_H_


#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>





extern "C"
{
	int android_asset_init(void *mgr);

	void * android_asset_open(const char *fileName, int mode);

	int android_asset_read(void *asset, char *buffer, int bufsize);

	int android_asset_length(void *asset);

	void android_asset_close(void *asset);

	void android_asset_seek(void *asset, int offset, int where);
}


#endif


 
