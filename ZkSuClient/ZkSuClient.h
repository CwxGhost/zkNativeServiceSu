#ifndef ANDROID_ZK_SU_CLIENT_H
#define ANDROID_ZK_SU_CLIENT_H

namespace android {
class ZkSuClient {

public:
	void sendCmd(char* cmd, char* key);
private:
	static const void getService();
};
}
;
#endif
