/* Copyright (C) 2019 Refinitiv. All rights reserved.      */

/*******************************************************************************
* 
* ATTENTION: 
# This is an AUTO GENERATED file and should NOT be edtied directly.  
* Any changes required should be made in the upstream template header file,
*            rsslVersion.h.in
*
* This file is used in both static and shared library builds
* The rssl.rc and rsslVA.rc files include this file in shared library builds
* 
*******************************************************************************/
#ifndef __RSSL_VERSION_H
#define __RSSL_VERSION_H

#define STR_EXPAND(str) #str
#define MKSTR(str) STR_EXPAND(str)

#ifdef NDEBUG
#define BLDTYPE " Release"
#else
#define BLDTYPE " Debug"
#endif

#define RSSL_Release_Major     3		// Release major version number
#define RSSL_Release_Minor     4		// Release minor version number
#define RSSL_Product_Major     0 	// Release patch number
#define RSSL_Product_Minor     0   	// Release tweak number

#define RSSL_Year				"2019"
#define RSSL_CompanyName		" Refinitiv"
#define RSSL_Version			RSSL_Release_Major,RSSL_Release_Minor,RSSL_Product_Major,RSSL_Product_Minor
#define RSSL_VersionString		MKSTR(RSSL_Release_Major) "." MKSTR(RSSL_Release_Minor) "." MKSTR(RSSL_Product_Major) "." MKSTR(RSSL_Product_Minor) " (" BLDTYPE ")"
#define RSSL_LegalCopyRight		"Copyright (C) " RSSL_Year RSSL_CompanyName ", All Rights Reserved. "
#define RSSL_ExternalName		"eta3.4.0.L1"
#define RSSL_InternalName		"eta3.4.0.0"
#define RSSL_ReleaseType		"rrg"
#define RSSL_ProdName			"Elektron Transport API (ETA) C Edition"
#define RSSL_Package			" PACKAGE " RSSL_ExternalName " " RSSL_ReleaseType

static char rsslOrigin[]    = RSSL_LegalCopyRight;
static char rsslVersion[]   = " VERSION " RSSL_InternalName;
static char rsslWhat[]      = "@(#)rsslVersion.h" RSSL_InternalName;
static char rsslWhere[]     = "s.rsslVersion.h";
static char rsslPackage[]   = RSSL_Package;
static char rsslDeltaDate[] = " Fri Nov 15 17:07:12 CST " RSSL_Year;
static char rsslComponentVersionStart[] = RSSL_ExternalName ".";
static char rsslComponentVersionEnd[] = "." RSSL_ReleaseType;

#define RSSL_ComponentVersionStart_Len strlen(rsslComponentVersionStart)
#define RSSL_ComponentVersionEnd_Len strlen(rsslComponentVersionEnd)

#ifdef WIN32
static char rsslComponentVersionPlatform[] = "win";
#endif
#ifdef Linux
static char rsslComponentVersionPlatform[] = "linux";
#endif

#define Rssl_ComponentVersionPlatform_Len strlen(rsslComponentVersionPlatform)

#ifdef COMPILE_64BITS
static char rsslBits[] = " 64-bit";
#else
static char rsslBits[] = " 32-bit";
#endif
#define Rssl_Bits_Len strlen(rsslBits)

#if WIN32
#ifdef RSSL_EXPORTS		// shared Library build
static char rsslLinkType[] = " Shared Library";
#else
static char rsslLinkType[] = " Static";
#endif
#else
extern char rsslLinkType[];
#endif

#endif

