#include "ZkSuService.h"
#include <utils/Log.h>
#include <cutils/log.h>
#include <binder/IServiceManager.h>
#include <binder/IPCThreadState.h>

namespace android {

static struct sigaction oldact;
static pthread_key_t sigbuskey;

char* result;
const static long lenght = 1024;

int ZkSuService::instantiate() {
	ALOGE("ZkSuService instantiate");
	// 调用ServiceManager的addService方法进行系统服务注册，这样客户端程序就可以通过ServiceManager获得此服务的代理对象，从而请求其提供的服务
	int r = defaultServiceManager()->addService(String16("com.zhaiker.su"),
			new ZkSuService());
	ALOGE("ZkSuService r = %d/n", r);
	return r;
}

void ZkSuService::executeCMD(const char *cmd) {
	char buf_ps[1024];
	char ps[1024] = { 0 };
	int i = 1;
	char *result2 = NULL;
	FILE *ptr = NULL;
	strcpy(ps, cmd);
	if ((ptr = popen(ps, "r")) != NULL) {
		result = (char *) malloc(lenght * sizeof(char));
		char *result2 = (char *) malloc(lenght * sizeof(char));

		while (fgets(buf_ps, 1024, ptr) != NULL) {
			result = (char *) malloc(lenght * i * sizeof(char));
			if (result2 != NULL)
				strcpy(result, result2);
			strcat(result, buf_ps);
			i++;
			result2 = (char *) malloc(lenght * (i - 1) * sizeof(char));
			strcpy(result2, result);
		}
		pclose(ptr);
		ptr = NULL;
	} else {
		ALOGE("popen %s error\n", ps);
	}

}

ZkSuService::ZkSuService() {
	ALOGE("ZkSuService created");
	mNextConnId = 1;
	pthread_key_create(&sigbuskey, NULL);
}

ZkSuService::~ZkSuService() {
	pthread_key_delete (sigbuskey);
	ALOGE("ZkSuService destroyed");
}
// 每个系统服务都继承自BBinder类，都应重写BBinder的onTransact虚函数。当用户发送请求到达Service时，系统框架会调用Service的onTransact函数
status_t ZkSuService::onTransact(uint32_t code, const Parcel& data,
		Parcel* reply, uint32_t flags) {
	switch (code) {
	case 0: {
		pid_t pid = data.readInt32();
		const char *cmd = data.readCString();
		executeCMD(cmd);
		reply->writeCString(result);
		return NO_ERROR;
	}
		break;
	default:
		return BBinder::onTransact(code, data, reply, flags);
	}
}
}
;
