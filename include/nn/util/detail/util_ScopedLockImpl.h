#define NN_UTIL_DETAIL_DEFINE_SCOPED_LOCK(typeName, lockCall, unlockCall) \
    class typeName::ScopedLock \
    { \
    private: \
        typeName& mReference; \
    public: \
        ScopedLock(typeName& reference) : mReference(reference) { mReference.lockCall; } \
        ~ScopedLock() { mReference.unlockCall; } \
    }

