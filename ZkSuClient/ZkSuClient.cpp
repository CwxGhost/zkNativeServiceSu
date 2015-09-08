#include <binder/IServiceManager.h>
#include <binder/IPCThreadState.h>
#include <utils/Log.h>
#include <cutils/log.h>
#include "ZkSuClient.h"

namespace android {
sp<IBinder> binder;
void ZkSuClient::sendCmd(char* cmd) {
	getService();
	Parcel data, reply;
	const char* answer;
	data.writeInt32(getpid());
	data.writeCString(cmd);
	ALOGE("ZkSuService::create remote()->transact()/n");
	binder->transact(0, data, &reply);
	answer = reply.readCString();
	printf("%s\n", answer);
	return;
}

const void ZkSuClient::getService() {
	sp < IServiceManager > sm = defaultServiceManager();
	binder = sm->getService(String16("com.zhaiker.su"));
	ALOGE("ZkSuClient::getZkSuService %p/n", sm.get());
	if (binder == 0) {
		ALOGW("ZkSuService not published, waiting...");
		return;
	}
}
}
;
//namespace

using namespace android;

int main(int argc, char** argv) {
	int c = 0;
	if (argc >= 2) {
		int i;
		char buf[1024] = { '\0' };  //数据传送的缓冲区
		for (i = 1; i < argc; i++) {
			strcat(buf, argv[i]);
			strcat(buf, " ");
		}
		strcat(buf, "\0");
		ZkSuClient* p = new ZkSuClient();
		p->sendCmd(buf);
	} else {
		printf("please input cmds\n");
	}
	return 0;
}
