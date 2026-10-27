// Copyright (c) Mistfly Games. All Rights Reserved.

#pragma once

#include "Modules/ModuleInterface.h"
#include "UObject/StrongObjectPtrTemplates.h"

class URandom;

class FSeedworkRandomModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
    virtual bool SupportsDynamicReloading() override { return false; }

private:
    TStrongObjectPtr<URandom> SharedRandom;
};
