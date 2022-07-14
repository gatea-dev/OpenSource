/******************************************************************************
*
*  BlobList.hpp
*     QuoddFeed BlobList Message
*
*  REVISION HISTORY:
*      3 SEP 2014 jcs  Created.
*     17 MAR 2015 jcs  Build 14: Dump()
*
*  (c) 1994-2015 Gatea Ltd.
******************************************************************************/
#ifndef __QUODD_BlobList_H
#define __QUODD_BlobList_H
#include <hpp/Message.hpp>

namespace QUODD
{

////////////////////////////////////////////////
//
//      c l a s s   B l o b L i s t
//
////////////////////////////////////////////////

/**
 * \class BlobList
 * \brief This class encapsulates a ::BlobList message structure.
 *
 * A BlobList object is driven into your application via the
 * Channel::OnUpdate( BlobList & ) virtual method that is called 
 * asynchronously when a Blob List message arrives from the UltraCache.
 */

class BlobList : public Message
{
public:
	/**
	 * \brief Returns a reference to the associated ::BlobList struct
	 *
	 * \return ::BlobList encapsulated by this object
	 */
	::BlobList &operator()()
	{
		return qm()._v._BlobList;
	}

	/**
	 * \brief Returns a set of pointers to the list
	 *
	 * \return A set of pointers to the list
	 */
	char **list()
		// Returns list
	{
		return (*this)()._lst;
	}

	/**
	 * \brief Returns the size of this list 
	 *
	 * \return Size of this list
	 */
	int size()
		// Returns num items in list
	{
		return (*this)()._nLst;
	}


	////////////////////////////////////
	// QUODD::Message Overrides
	////////////////////////////////////
public:
	/**
	 * \brief Dumps formatted message header
	 *
	 * \return Pointer to formatted message header
	 */
	virtual const char *Dump()
	{
		char  *bp, *cp;
		char **tkrs;
		int    i, nl, aSz;

		// Header

		Message::Dump();

		// Guts

		nl  = size();
		aSz = gmax( K, nl*_MAX_BLOB_CEL );
		bp  = new char[aSz];
		cp  = bp;
		*cp = '\0';
		tkrs = list();
      cp  += sprintf( cp, "; %d tkrs\n", nl );
		for ( i=0; i<nl; cp += sprintf( cp, "%s\n", tkrs[i++] ) );

		// Return

		dump_ += bp;
		delete[] bp;
		return dump_.data();
	}
};  // class BlobList

} // namespace QUODD

#endif // __QUODD_BlobList_H 
