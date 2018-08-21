#pragma once
#include <string>
#include <fnd/types.h>
#include <fnd/IFile.h>
#include <nn/hac/NpdmBinary.h>
#include "KeyConfiguration.h"

#include "nstool.h"

class NpdmProcess
{
public:
	NpdmProcess();
	~NpdmProcess();

	void process();

	void setInputFile(fnd::IFile* file, bool ownIFile);
	void setKeyCfg(const KeyConfiguration& keycfg);
	void setCliOutputMode(CliOutputMode type);
	void setVerifyMode(bool verify);

	const nn::hac::NpdmBinary& getNpdmBinary() const;

private:
	const std::string kModuleName = "NpdmProcess";

	fnd::IFile* mFile;
	bool mOwnIFile;
	KeyConfiguration mKeyCfg;
	CliOutputMode mCliOutputMode;
	bool mVerify;

	nn::hac::NpdmBinary mNpdm;

	void importNpdm();

	void validateAcidSignature(const nn::hac::AccessControlInfoDescBinary& acid);
	void validateAciFromAcid(const nn::hac::AccessControlInfoBinary& aci, const nn::hac::AccessControlInfoDescBinary& acid);

	void displayNpdmHeader(const nn::hac::NpdmBinary& hdr);
	void displayAciHdr(const nn::hac::AccessControlInfoBinary& aci);
	void displayAciDescHdr(const nn::hac::AccessControlInfoDescBinary& aci);
	void displayFac(const nn::hac::FileSystemAccessControlBinary& fac);
	void displaySac(const nn::hac::ServiceAccessControlBinary& sac);
	void displayKernelCap(const nn::hac::KernelCapabilityBinary& kern);

	const char* getInstructionTypeStr(nn::hac::npdm::InstructionType type) const;
	const char* getProcAddressSpaceTypeStr(nn::hac::npdm::ProcAddrSpaceType type) const;
	const char* getAcidFlagStr(nn::hac::aci::Flag flag) const;
	const char* getMiscFlagStr(nn::hac::MiscFlagsHandler::Flags flag) const;
	const char* getFsaRightStr(nn::hac::fac::FsAccessFlag flag) const;
	const char* getSaveDataOwnerAccessModeStr(nn::hac::fac::SaveDataOwnerIdAccessType type) const;
	const char* getSystemCallStr(byte_t syscall_id) const;
	const char* getMemMapPermStr(nn::hac::MemoryMappingHandler::MemoryPerm type) const;
	const char* getMemMapTypeStr(nn::hac::MemoryMappingHandler::MappingType type) const;
};