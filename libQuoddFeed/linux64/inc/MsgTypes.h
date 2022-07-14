/******************************************************************************
*
*  MsgTypes.h
*     UltraCache wire protocol - MsgTypes
*
*  REVISION HISTORY:
*     12 DEC 2012 jcs  Created (from MsgTypes.java).
*      3 SEP 2014 jcs  Build 10: _ixSubSETLVAL / _ixSubSETLSUMM
*      1 JAN 2015 jcs  Build 11: _eqSubLIMITUPDN; _blSubUNDERLYERS; _mtPING
*     28 JAN 2015 jcs  Build 12: _eqSubTRDASOF / _eqSubTRDASOFCXL
*     13 FEB 2015 jcs  Build 13: _blSubFUTOPT_CHAIN
*      7 MAY 2015 jcs  Build 17: _blSubDUMPDB
*      8 JUL 2015 jcs  Build 19: _opSubTRADE_ID
*     22 FEB 2016 jcs  Build 22: _blSubSORT
*     20 MAR 2018 jcs  Build 23: _blSubADDED
*
*  (c) 1994-2018 Gatea Ltd.
*******************************************************************************/
#ifndef __QOD_MSGTYPES_H
#define __QOD_MSGTYPES_H


/**************************
 * Message Types
 *************************/
#define _mtPING       'P'
#define _mtSESSION    'S'
#define _mtEQUITY     'e'
#define _mtOPTION     'o'
#define _mtFUND       'F'
#define _mtINDEX      'I'
#define _mtFUTURE     'T'
#define _mtBOND       'b'
#define _mtBLOB       'B'
#define _mtDEAD       'x'
#define _mtUC_STATUS  'U'
#define _mtSTREAM_OPN '+'


/**************************
 * Message Sub-Types
 *************************/
/*
 * Any
 */
#define _mtSubIMG         'X'
#define _sessSubCHALLENGE 'C'
#define _sessSubSUCCESS   '^'
#define _sessSubFAILURE   'v'
/*
 * _mtEQUITY
 */
#define _eqSubQTESHORT   0x27
#define _eqSubQTELONG    0x28
#define _eqSubBBOSHORT   0x29
#define _eqSubBBOLONG    0x2a
#define _eqSubQTESHORTMM 0x2b
#define _eqSubQTELONGMM  0x2c
#define _eqSubBBOSHORTMM 0x2d
#define _eqSubBBOLONGMM  0x2e
#define _eqSubTRDSHORT   0x2f
#define _eqSubTRDLONG    0x30
#define _eqSubTRDCXL     0x31
#define _eqSubTRDCORR    0x32
#define _eqSubTRDASOF    0x33
#define _eqSubTRDASOFCXL 0x34
#define _eqSubTRDASOFCOR 0x35
#define _eqSubTRDACTION  0x36
#define _eqSubREGSHO     0x38
#define _eqSubSUMMARY    0x39
#define _eqSubMKTCTRSUMM 0x3b
#define _eqSubLIMITUPDN  0x3d
#define _eqSubMKTSTATS   0x7f
/*
 * _mtOPTION
 */
#define _opSubOPENINT   0x25
#define _opSubQTESHORT  0x26
#define _opSubQTELONG   0x27
#define _opSubBBOSHORT  0x28
#define _opSubBBOLONG   0x29
#define _opSubTRADE     0x2a
#define _opSubTRDCXL    0x2b
#define _opSubTRDSUMM   0x2c
#define _opSubTRADE_ID  0x3a  /* "Extended" v1.0 */
#define _opSubTRDCXL_ID 0x3b  /* "Extended" v1.0 */
/*
 * _mtFUND
 */
#define _fundSubFUNDNAV   0x25
#define _fundSubMMNAV     0x26
#define _fundSubCAPDISTRO 0x27
#define _fundSubDIVID_INT 0x28
/*
 * _mtINDEX
 */
#define _ixSubVALUE    0x31
#define _ixSubSUMM     0x33
#define _ixSubSETLVAL  0x40
#define _ixSubSETLSUMM 0x41
#define _ixSubETPINTRA 0x50
#define _ixSubEST_CASH 0x52
#define _ixSubTOT_CASH 0x53
#define _ixSubNAV      0x54
#define _ixSubSHARES   0x55
#define _ixSubETPDIV   0x56
/*
 * _mtBOND
 */
#define _boSubQUOTE    0x26
#define _boSubQTEYLD   0x27
#define _boSubTRADE    0x28
#define _boSubTRDCXL   0x29
#define _boSubTRDCORR  0x2a
/*
 * _mtFUTURE
 */
#define _ftSubCOMBOQTE 0x30
#define _ftSubTRADE    0x31
#define _ftSubTRDCXL   0x32
#define _ftSubTRDCORR  0x33
#define _ftSubQUOTE    0x34
#define _ftSubEXCQTE   0x35
#define _ftSubMKTCOND  0x36
#define _ftSubRFQ      0x37
#define _ftSubHILOLAST 0x38
#define _ftSubOPENINT  0x39
#define _ftSubSUMMARY  0x3a
#define _ftSubSETLPRC  0x3b
#define _ftSubRANGEQTE 0x3c
#define _ftSubCUMVOL   0x3d
#define _ftSubCASHPRC  0x4d
/*
 * _mtBLOB
 */
#define _blSubFIELDLIST    'F'
#define _blSubEXCHANGES    'E'
#define _blSubEXCHLIST     'X'
#define _blSubUNDERLYERS   'U'
#define _blSubOPTIONS      'O'
#define _blSubSNAPSHOT     'S'
#define _blSubIDXLIST      'I'
#define _blSubFUNDS        'f'
#define _blSubINDICES      'i'
#define _blSubFUT_CHAIN    'c'
#define _blSubFUTOPT_CHAIN 'o'
#define _blSubFUT_SNAP     's'
#define _blSubFUTURES      't'
#define _blSubBONDS        'b'
#define _blSubSORT         'T'
#define _blSubMMIDS        'm'
#define _blSubCONTRIB      'C'
#define _blSubFUNDY        'Y'
#define _blSubMKT_CAT      'M'
#define _blSubDUMPDB       'D'
#define _blSubADDED        '+'

/**************************
 * Special Stream ID's
 *************************/

#define _NEW_ISSUE_ID      INFINITEs

#endif /* __QOD_MSGTYPES_H */
