
#include "android_asset.hpp"

#include <string.h>


static AAssetManager *assetManager = NULL;

static JavaVM *javaVM = NULL;


JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *arg)
{
	javaVM = vm;
	return 0;
}

extern "C"
{
	int android_asset_init(void *mgr)
	{
		if(mgr == NULL)
			return -1;
		if(javaVM == NULL)
			return -2;

		jobject jmgr = static_cast<jobject>(mgr);
		if(jmgr == NULL)
			return -3;

		JNIEnv *env = NULL;
		int status = javaVM->GetEnv((void **)&env, JNI_VERSION_1_6);
		if(status < 0)
		{
			status = javaVM->AttachCurrentThread(&env, NULL);
			if(status < 0)
			{
				env = NULL;
			}
		}

		if(env == NULL)
			return -4;

		assetManager = AAssetManager_fromJava(env, jmgr);
		if(assetManager == NULL)
			return -5;

		return 0;
	}

	void * android_asset_open(const char *fileName, int mode)
	{
		if(fileName == NULL)
			return NULL;
		int len = strlen(fileName);
		char c = '?';
		int index = 0;
		for(int i = 0; i < len; i++)
		{
			c = fileName[i];
			if(c == '!')
			{
				if(i + 7 < len)
				{
					if(fileName[i + 1] == '/' && fileName[i + 2] == 'a' && fileName[i + 3] == 's' && fileName[i + 4] == 's' && fileName[i + 5] == 'e' && fileName[i + 6] == 't')
					{
						index = i + 9;
					}
				}
			}
		}

		if(index > 0 && index < len)
			return AAssetManager_open(assetManager, &(fileName[index]), mode);
		else
			return AAssetManager_open(assetManager, fileName, mode);
	}

	int android_asset_read(void *asset, char *buffer, int bufsize)
	{
		AAsset *jasset = static_cast<AAsset *>(asset);
		if(jasset == NULL)
			return -100;
		return AAsset_read(jasset, (void *)buffer, bufsize);
	}

	int android_asset_length(void *asset)
	{
		AAsset *jasset = static_cast<AAsset *>(asset);
		if(jasset == NULL)
			return 0;

		return AAsset_getLength(jasset);
	}

	void android_asset_close(void *asset)
	{
		AAsset *jasset = static_cast<AAsset *>(asset);
		if(jasset == NULL)
			return;

		return AAsset_close(jasset);
	}

	void android_asset_seek(void *asset, int offset, int where)
	{
		AAsset *jasset = static_cast<AAsset *>(asset);
		if(jasset == NULL)
			return;

		AAsset_seek(jasset, offset, where);
	}
}


