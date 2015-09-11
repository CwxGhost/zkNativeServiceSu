#include <utils/Log.h>
#include <cutils/log.h>
#include <binder/IServiceManager.h>
#include <binder/IPCThreadState.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "ZkSuService.h"
#include "../md5/md5.h"

#include <time.h>

namespace android {

static struct sigaction oldact;
static pthread_key_t sigbuskey;

int ZkSuService::instantiate() {
	ALOGD("ZkSuService instantiate");
	// 调用ServiceManager的addService方法进行系统服务注册，这样客户端程序就可以通过ServiceManager获得此服务的代理对象，从而请求其提供的服务
	int r = defaultServiceManager()->addService(String16("com.zhaiker.su"),
			new ZkSuService());
	ALOGD("ZkSuService r = %d/n", r);
	return r;
}

bool ZkSuService::checkKey(const char* key) {
	char buf[1024];

	sprintf(buf, "testKey%ld", (long int) time(0));
	char* md5;
	MDString(buf, &md5);
	ALOGD("md5 -> %s", md5);
	if (strcmp(key, "testKey") == 0) {
		return true;
	}
	return false;
}

#define lenght 1024

void ZkSuService::executeCMD(const char *cmd, char** res) {
	char buf_ps[1024];
	char ps[100] = { 0 };
	int i = 1;
	char *result2 = NULL;
	char* result = NULL;
	FILE *ptr = NULL;
	strcpy(ps, cmd);
	if ((ptr = popen(ps, "r")) != NULL) {
		char ch = fgetc(ptr);
		if (ch == 255 || ch == EOF) {
			result = (char *) malloc(lenght * i * sizeof(char));
			strcpy(result, "shell is error!");
			*res = result;
			ALOGD("cmd %s error\n", cmd);
			return;
		}
		while (fgets(buf_ps, 1024, ptr) != NULL) {
			result = (char *) malloc(lenght * i * sizeof(char));
			memset(result, '\0', lenght * i);
			if (result2 != NULL)
				strcpy(result, result2);
			strcat(result, buf_ps);
			i++;
			result2 = (char *) malloc(lenght * (i - 1) * sizeof(char));
			memset(result2, '\0', lenght * (i - 1));
			strcpy(result2, result);
		}
		*res = result;
		free(result2);
		pclose(ptr);
		ptr = NULL;
	} else {
		ALOGD("popen %s error\n", strerror(errno));
		return;
	}
}

ZkSuService::ZkSuService() {
	ALOGD("ZkSuService created");
	mNextConnId = 1;
	pthread_key_create(&sigbuskey, NULL);
}

ZkSuService::~ZkSuService() {
	pthread_key_delete (sigbuskey);
	ALOGD("ZkSuService destroyed");
}
// 每个系统服务都继承自BBinder类，都应重写BBinder的onTransact虚函数。当用户发送请求到达Service时，系统框架会调用Service的onTransact函数
status_t ZkSuService::onTransact(uint32_t code, const Parcel& data,
		Parcel* reply, uint32_t flags) {
	switch (code) {
	case 0: {
		int uid = data.readInt32();
		const char* cmd = data.readCString();
		const char* key = data.readCString();
		if (checkKey(key) || uid == 0) {
			char* result;
			executeCMD(cmd, &result);
			reply->writeCString(result);
		} else {
			reply->writeCString("check key fail");
		}
		return NO_ERROR;
	}
		break;
	default:
		return BBinder::onTransact(code, data, reply, flags);
	}
}
}
;
