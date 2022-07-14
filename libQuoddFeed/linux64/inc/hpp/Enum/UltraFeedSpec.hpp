/******************************************************************************
*
*  UltraFeedSpec.hpp
*     Common UltraFeed channel information
*
*  REVISION HISTORY:
*      5 SEP 2014 jcs  Created (from QuoddFeed.Enum.UltraFeed.java)
*     28 JUL 2015 jcs  Build 19: Updated Protocol from v3.0 spec
*
*  (c) 2011-2014 Quodd Financial
*******************************************************************************/
#ifndef __ULTRAFEED_SPEC_H
#define __ULTRAFEED_SPEC_H
#include <Messages.h>
#include <MsgTypes.h>
#include <vector>
#include <string>

namespace ULTRAFEED
{

/////////////////////////////////////////////////////////////////
//
//               c l a s s    U l t r a F e e d
//
/////////////////////////////////////////////////////////////////
/**
 * \class UltraFeedSpec
 * \brief This base class encapsulates the UltraFeed specifications.
 *
 * You typically do not call this class directly.
 */

class UltraFeedSpec
{
	/////////////////////////
	// Class-wide
	/////////////////////////
public:
	/**
	 * \brief Returns true if this QuoddMsg is from the EQUITY channel
	 *
	 * \return true if this QuoddMsg is from the EQUITY channel
	 */
	static bool IsEquity( qHdr qh )
	{
	   return( qh._mt == _mtEQUITY );
	}

	/**
	 * \brief Returns true if this QuoddMsg is from a Canadian protocol
	 *
	 * \return true if this QuoddMsg is from a Canadian protocol
	 */
	static bool IsCanadian( qHdr qh )
	{
	   switch( qh._protocol ) {
	      case  11: // "TL1 (TSX Level 1)"
	      case  12: // "CL1 (TSX Venture Level 1)"
	      case  24: // "TSX"
	      case  25: // "TSX-C"
	         return true;
	   }
	   return false;
	}

	/**
	 * \brief Returns true if this QuoddMsg is from a Pink Sheet protocol
	 *
	 * \return true if this QuoddMsg is from a Pink Sheet protocol
	 */
	static bool IsPink( qHdr qh )
	{
	   switch( qh._protocol ) {
	      case  13: // OTC Quotes
	      case  14: // OTC Insides
	         return true;
	   }
	   return false;
	}

	/**
	 * \brief Returns true if this QuoddMsg is from a Level2 protocol
	 *
	 * \return true if this QuoddMsg is from a Level2 protocol
	 */
	static bool IsNazzLevel2( qHdr qh )
	{
	   switch( qh._protocol ) {
	      case  22: // NASDAQ-LEVEL2
	         return true;
	   }
	   return false;
	}

	/**
	 * \brief Returns name of protocol in this message
	 *
	 * \return Name of protocol in this message - OPRA, etc.
	 */
	static const char *Protocol( qHdr qh )
	{
	   switch( qh._protocol ) {
	      case   1: return "CQS  Network E";
	      case   2: return "CQS  Network F";
	      case   3: return "CTS  Network A";
	      case   4: return "CTS  Network B";
	      case   5: return "CTS  Indices";
	      case   6: return "UQDF";
	      case   7: return "UTDF";
	      case   8: return "OMDF";
	      case   9: return "BBDS";
	      case  10: return "TDDS";
	      case  13: return "OTC Markets  Quotes";
	      case  14: return "OTC Markets  Insides";
	      case  16: return "NASDAQ OMX Last Sale (NLS)";
	      case  17: return "NASDAQ OMX BX Last Sale (BLS)";
	      case  18: return "NASDAQ OMX PSX Last Sale (PLS)";
	      case  19: return "NASDAQ OMX BBO (QBBO)";
	      case  20: return "NASDAQ OMX BX BBO (BBBO)";
	      case  21: return "NASDAQ OMX PSX BBO (PBBO)";
	      case  22: return "NASDAQ OMX Level 2";
	      case  23: return "TSX CDF (Consolidated Data Feed)";
	      case  24: return "TSX Level 1 Quantum Feed (TL1Q)";
	      case  25: return "TSX Venture Level 1 Quantum Feed (CL1Q)";
	      case  26: return "TSX Index Quantum Feed (TX1)";
	      case  27: return "TSX Reference Data Quantum Feed (TRD)";
	      case  28: return "TSX Venture Reference Data Quantum Feed (VRD)";
	      case  29: return "Alpha Level 1 Quantum Feed (AL1)";
	      case  30: return "TSX Level 2 Quantum Feed (TL2Q)";
	      case  31: return "TSX Venture Level 2 Quantum Feed (CL2Q)";
	      case  32: return "TMX Select Level 2 Quantum Feed (SL2Q)";
	      case  33: return "Alpha Level 2 Quantum Feed (AL2Q)";
	      case  34: return "TDDS 2.0";
	      case  35: return "Omega ATS Itch 3.0 Feed";
	      case  36: return "Chi-X Canada CHIXMMD Feed";
	      case  37: return "CX2 Canada CHIXMMD Feed";
	      case  38: return "NYSE RussellTick";
	      case  39: return "NASDAQ OMX GIDS 2.0";
	      case  41: return "NYSE Global Index Feed (GIF)";
	      case  42: return "CBOE Market Data Indices (MDI)";
	      case  46: return "S&P Indices (CME Streamlined FAST-FIX)";
	      case  48: return "Dow Jones Indices (CME Streamlined FAST-FIX)";
	      case  51: return "OPRA v3 (Binary)";
	      case  60: return "CANDEAL (CDL)";
	      case  61: return "ArcaTrade for Bonds";
	      case  62: return "ArcaBond Quotes";
	      case  64: return "E-Speed Book";
	      case  70: return "Mutual Fund Dissemination Service (MFDS)";
	      case  71: return "NASDAQ OMX Last Sale (NLS) 2.0";
	      case  72: return "NASDAQ OMX BX Last Sale (BLS) 2.0";
	      case  73: return "NASDAQ OMX PSX Last Sale (PLS) 2.0";
	      case  74: return "NASDAQ OMX BBO (QBBO) 2.0";
	      case  75: return "NASDAQ OMX BX BBO (BBBO) 2.0";
	      case  76: return "NASDAQ OMX PSX BBO (PBBO) 2.0";
	      case  77: return "NASDAQ OMX Level 2 2.0";
	      case  80: return "CME  ITC (Floor)";
	      case  81: return "CME  FAST-FIX (Electronic)  Pending Removal";
	      case  82: return "CME  FAST-FIX (Electronic)  Pending Removal";
	      case  83: return "CME  FAST-FIX (Electronic)  Pending Removal";
	      case  84: return "NYMEX  ITC (Floor)";
	      case  85: return "NYMEX  FAST-FIX (Electronic)  Pending Removal";
	      case  86: return "NYMEX  FAST-FIX (Electronic)  Pending Removal";
	      case  87: return "NYMEX  FAST-FIX (Electronic)  Pending Removal";
	      case  88: return "COMEX  ITC (Floor)";
	      case  89: return "COMEX  FAST-FIX (Electronic)  Pending Removal";
	      case  90: return "COMEX  FAST-FIX (Electronic)  Pending Removal";
	      case  91: return "COMEX  FAST-FIX (Electronic)  Pending Removal";
	      case  92: return "CBOT  ITC (Floor)";
	      case  93: return "CBOT  FAST-FIX (Electronic)  Pending Removal";
	      case  94: return "CBOT  FAST-FIX (Electronic)  Pending Removal";
	      case  95: return "CBOT  FAST-FIX (Electronic)  Pending Removal";
	      case 130: return "CME-MDP3 (Electronic)";
	      case 131: return "CBOT-MDP3 (Electronic)";
	      case 132: return "NYMEX-MDP3 (Electronic)";
	      case 133: return "COMEX-MDP3 (Electronic)";
	      case 141: return "Dow Jones Indices";
	      case 150: return "ArcaBook  Listed";
	      case 151: return "ArcaBook  OTC";
	      case 152: return "ArcaBook  ETF";
	      case 153: return "NYSE OpenBook Ultra";
	      case 154: return "NYSE MKT OpenBook Ultra";
	      case 155: return "EdgeBook Depth  EDGX";
	      case 156: return "EdgeBook Depth  EDGA";
	      case 157: return "BATS PITCH";
	      case 158: return "BATS-Y PITCH";
	      case 159: return "NASDAQ OMX TotalView ITCH";
	      case 160: return "NASDAQ OMX BX TotalView ITCH";
	      case 161: return "NASDAQ OMX PSX TotalView ITCH";
	      case 162: return "NYSE Imbalance";
	      case 163: return "NYSE MKT Imbalance";
	      case 165: return "NYSE Alerts";
	      case 166: return "NASDAQ TotalView ITCH 5.0";
	      case 167: return "NASDAQ BX TotalView ITCH 5.0";
	      case 168: return "NASDAQ PSX TotalView ITCH 5.0";
	      case 175: return "CSE L2";
	      case 176: return "Canadian Best Bid and Offer";
	      case 177: return "Canadian Best Bid and Offer";
	      case 178: return "TMX Consolidated Last Sale";
	      case 179: return "TMX Consolidated Last Sale";
	      case 255: return "QUODD";
	   }
	   return "Undefined";
	}


	////////////////////////
	// Helpers
	////////////////////////
protected:
	static bool _IsSet( int msk, int bit )
	{
	   return( ( msk & bit ) == bit );
	}

	static bool _IsQuodd( int pro )
	{
	   return( pro == 255 );
	}

}; // class UltraFeedSpec

} // namespace ULTRAFEED

#endif // __ULTRAFEED_SPEC_H
