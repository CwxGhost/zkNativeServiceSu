#ifndef ANDROID_ZK_SU_CLIENT_H
#define ANDROID_ZK_SU_CLIENT_H

namespace android {
class ZkSuClient {

public:
	void sendCmd(char* n);
private:
	static const void getService();
};
}
;
#endif
