#ifndef ANDROID_ZK_SU_SERVICE_H
#define ANDROID_ZK_SU_SERVICE_H
#include <utils/threads.h>
#include <utils/RefBase.h>
#include <binder/IInterface.h>
#include <binder/BpBinder.h>
#include <binder/Parcel.h>

namespace android {
class ZkSuService: public BBinder {
	mutable Mutex mLock;
	int32_t mNextConnId;
private:
	void executeCMD(const char *cmd, char** res);
	bool checkKey(const char* key);
public:
	static int instantiate();
	ZkSuService();
	virtual ~ZkSuService();
	virtual status_t onTransact(uint32_t, const Parcel&, Parcel*, uint32_t);
};
}
;
//namespace
#endif
