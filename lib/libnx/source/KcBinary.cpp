#include <nx/KcBinary.h>

nx::KcBinary::KcBinary()
{}

nx::KcBinary::KcBinary(const KcBinary & other)
{
	*this = other;
}

void nx::KcBinary::operator=(const KcBinary & other)
{
	clear();
	mThreadInfo = other.mThreadInfo;
	mSystemCalls = other.mSystemCalls;
	mMemoryMap = other.mMemoryMap;
	mInterupts = other.mInterupts;
	mMiscParams = other.mMiscParams;
	mKernelVersion = other.mKernelVersion;
	mHandleTableSize = other.mHandleTableSize;
	mMiscFlags = other.mMiscFlags;
}

bool nx::KcBinary::operator==(const KcBinary & other) const
{
	return (mThreadInfo == other.mThreadInfo) \
		&& (mSystemCalls == other.mSystemCalls) \
		&& (mMemoryMap == other.mMemoryMap) \
		&& (mInterupts == other.mInterupts) \
		&& (mMiscParams == other.mMiscParams) \
		&& (mKernelVersion == other.mKernelVersion) \
		&& (mHandleTableSize == other.mHandleTableSize) \
		&& (mMiscFlags == other.mMiscFlags);
}

bool nx::KcBinary::operator!=(const KcBinary & other) const
{
	return !(*this == other);
}

void nx::KcBinary::toBytes()
{
	fnd::List<KernelCapability> caps;

	// get kernel capabiliteis
	mThreadInfo.exportKernelCapabilityList(caps);
	mSystemCalls.exportKernelCapabilityList(caps);
	mMemoryMap.exportKernelCapabilityList(caps);
	mInterupts.exportKernelCapabilityList(caps);
	mMiscParams.exportKernelCapabilityList(caps);
	mKernelVersion.exportKernelCapabilityList(caps);
	mHandleTableSize.exportKernelCapabilityList(caps);
	mMiscFlags.exportKernelCapabilityList(caps);

	// allocate memory
	mRawBinary.alloc(caps.size() * sizeof(uint32_t));

	// write to binary
	uint32_t* raw_caps = (uint32_t*)mRawBinary.data();
	for (size_t i = 0; i < caps.size(); i++)
	{
		raw_caps[i] = le_word(caps[i].getCap());
	}
}

void nx::KcBinary::fromBytes(const byte_t * data, size_t len)
{
	if ((len % sizeof(uint32_t)) != 0)
	{
		throw fnd::Exception(kModuleName, "KernelCapability list must be aligned to 4 bytes");
	}

	// save copy of KcBinary
	mRawBinary.alloc(len);
	memcpy(mRawBinary.data(), data, len);

	fnd::List<KernelCapability> threadInfoCaps;
	fnd::List<KernelCapability> systemCallCaps;
	fnd::List<KernelCapability> memoryMapCaps;
	fnd::List<KernelCapability> interuptCaps;
	fnd::List<KernelCapability> miscParamCaps;
	fnd::List<KernelCapability> kernelVersionCaps;
	fnd::List<KernelCapability> handleTableSizeCaps;
	fnd::List<KernelCapability> miscFlagsCaps;

	const uint32_t* raw_caps = (const uint32_t*)mRawBinary.data();
	size_t cap_num = mRawBinary.size() / sizeof(uint32_t);
	KernelCapability cap;
	for (size_t i = 0; i < cap_num; i++)
	{
		cap.setCap(le_word(raw_caps[i]));
		switch (cap.getType())
		{
			case (KernelCapability::KC_THREAD_INFO) :
				threadInfoCaps.addElement(cap);
				break;
			case (KernelCapability::KC_ENABLE_SYSTEM_CALLS):
				systemCallCaps.addElement(cap);
				break;
			case (KernelCapability::KC_MEMORY_MAP):
			case (KernelCapability::KC_IO_MEMORY_MAP):
				memoryMapCaps.addElement(cap);
				break;
			case (KernelCapability::KC_ENABLE_INTERUPTS):
				interuptCaps.addElement(cap);
				break;
			case (KernelCapability::KC_MISC_PARAMS):
				miscParamCaps.addElement(cap);
				break;
			case (KernelCapability::KC_KERNEL_VERSION):
				kernelVersionCaps.addElement(cap);
				break;
			case (KernelCapability::KC_HANDLE_TABLE_SIZE):
				handleTableSizeCaps.addElement(cap);
				break;
			case (KernelCapability::KC_MISC_FLAGS):
				miscFlagsCaps.addElement(cap);
				break;
			default:
				throw fnd::Exception(kModuleName, "Unsupported kernel capability type");
		}
	}

	mThreadInfo.importKernelCapabilityList(threadInfoCaps);
	mSystemCalls.importKernelCapabilityList(systemCallCaps);
	mMemoryMap.importKernelCapabilityList(memoryMapCaps);
	mInterupts.importKernelCapabilityList(interuptCaps);
	mMiscParams.importKernelCapabilityList(miscParamCaps);
	mKernelVersion.importKernelCapabilityList(kernelVersionCaps);
	mHandleTableSize.importKernelCapabilityList(handleTableSizeCaps);
	mMiscFlags.importKernelCapabilityList(miscFlagsCaps);
}

const fnd::Vec<byte_t>& nx::KcBinary::getBytes() const
{
	return mRawBinary;
}

void nx::KcBinary::clear()
{
	mRawBinary.clear();
	mThreadInfo.clear();
	mSystemCalls.clear();
	mMemoryMap.clear();
	mInterupts.clear();
	mMiscParams.clear();
	mKernelVersion.clear();
	mHandleTableSize.clear();
	mMiscFlags.clear();
}

const nx::ThreadInfoHandler & nx::KcBinary::getThreadInfo() const
{
	return mThreadInfo;
}

nx::ThreadInfoHandler & nx::KcBinary::getThreadInfo()
{
	return mThreadInfo;
}

const nx::SystemCallHandler & nx::KcBinary::getSystemCalls() const
{
	return mSystemCalls;
}

nx::SystemCallHandler & nx::KcBinary::getSystemCalls()
{
	return mSystemCalls;
}

const nx::MemoryMappingHandler & nx::KcBinary::getMemoryMaps() const
{
	return mMemoryMap;
}

nx::MemoryMappingHandler & nx::KcBinary::getMemoryMaps()
{
	return mMemoryMap;
}

const nx::InteruptHandler & nx::KcBinary::getInterupts() const
{
	return mInterupts;
}

nx::InteruptHandler & nx::KcBinary::getInterupts()
{
	return mInterupts;
}

const nx::MiscParamsHandler & nx::KcBinary::getMiscParams() const
{
	return mMiscParams;
}

nx::MiscParamsHandler & nx::KcBinary::getMiscParams()
{
	return mMiscParams;
}

const nx::KernelVersionHandler & nx::KcBinary::getKernelVersion() const
{
	return mKernelVersion;
}

nx::KernelVersionHandler & nx::KcBinary::getKernelVersion()
{
	return mKernelVersion;
}

const nx::HandleTableSizeHandler & nx::KcBinary::getHandleTableSize() const
{
	return mHandleTableSize;
}

nx::HandleTableSizeHandler & nx::KcBinary::getHandleTableSize()
{
	return mHandleTableSize;
}

const nx::MiscFlagsHandler & nx::KcBinary::getMiscFlags() const
{
	return mMiscFlags;
}

nx::MiscFlagsHandler & nx::KcBinary::getMiscFlags()
{
	return mMiscFlags;
}