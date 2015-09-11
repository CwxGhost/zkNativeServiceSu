#include <binder/IServiceManager.h>
#include <binder/IPCThreadState.h>
#include <utils/Log.h>
#include <cutils/log.h>
#include <unistd.h>
#include "ZkSuClient.h"

static void usage() {
	printf("Usage: zksuclient [-options] [args...]\n"
			"   -k   -   the md5 keys\n"
			"   -c   -   the su cmds(-c 'cmds')\n"
			"   -h   -   print help info.\n");
}

namespace android {
sp<IBinder> binder;
void ZkSuClient::sendCmd(char* cmd, char* key) {
	getService();
	Parcel data, reply;
	const char* answer;
	data.writeInt32(getuid());
	data.writeCString(cmd);
	data.writeCString(key);
	ALOGD("ZkSuService::create remote()->transact()\n");
	binder->transact(0, data, &reply);
	answer = reply.readCString();
	printf("%s\n", answer);
	return;
}

const void ZkSuClient::getService() {
	sp < IServiceManager > sm = defaultServiceManager();
	binder = sm->getService(String16("com.zhaiker.su"));
	ALOGD("ZkSuClient::getZkSuService %p\n", sm.get());
	if (binder == 0) {
		ALOGD("ZkSuService not published, waiting...");
		return;
	}
}
}
;
//namespace

using namespace android;

bool checkKey(char* key) {
	if (getuid() == 0) {
		return true;
	}
	return true;
}

int main(int argc, char** argv) {
	int c = 0;
	int ch;
	bool hascmd = false;
	char* cmd;
	char* key = "nokey";
	while ((ch = getopt(argc, argv, "k:c:h")) != -1) {
		switch (ch) {
		case 'k':
			key = optarg;
			break;
		case 'c':
			cmd = optarg;
			hascmd = true;
			break;
		case '?':
		case 'h':
		default:
			usage();
			return 1;
		}
	}
	if (argc == 1) {
		usage();
		return 1;
	}
	if (hascmd) {
		ZkSuClient* p = new ZkSuClient();
		p->sendCmd(cmd, key);
	} else {
		usage();
	}
	return 0;
}
