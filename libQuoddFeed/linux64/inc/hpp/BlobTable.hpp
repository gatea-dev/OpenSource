/******************************************************************************
*
*  BlobTable.hpp
*     QuoddFeed BlobTable Message
*
*  REVISION HISTORY:
*      3 SEP 2014 jcs  Created.
*     17 MAR 2015 jcs  Build 14: Dump()
*
*  (c) 1994-2015 Gatea Ltd.
******************************************************************************/
#ifndef __QUODD_BlobTable_H
#define __QUODD_BlobTable_H
#include <hpp/Message.hpp>


namespace QUODD
{

////////////////////////////////////////////////
//
//      c l a s s   B l o b T a b l e
//
////////////////////////////////////////////////

/**
 * \class BlobTable
 * \brief This class encapsulates a ::BlobTable message structure.
 *
 * A BlobTable object is driven into your application via the
 * Channel::OnUpdate( BlobTable & ) virtual method that is called
 * asynchronously when a Blob Table message arrives from the UltraCache.
 */

class BlobTable : public Message
{
public:
   /**
	 * \brief Returns a reference to the associated ::BlobTable struct
	 *
	 * \return ::BlobTable encapsulated by this object
	 */
	::BlobTable &operator()()
	{
		return qm()._v._BlobTable;
	}

	/**
	 * \brief Returns the number of columns in this table
	 *
	 * \return Number of columns in this table
	 */
	int nCol()
	{
		return (*this)()._colHdr._nCel;
	}

	/**
	 * \brief Returns the number of rows in this table
	 *
	 * \return Number of rows in this table
	 */
	int nRow()
	{
		return (*this)()._nRow;
	}

	/**
	 * \brief Returns name of specific column.
	 *
	 * \param User-defined column number
	 * \return Name of column col
	 */
	const char *GetColName( int col )
		// col - Column of interest 
		// Returns name of column 
	{
		BlobRow hdr = (*this)()._colHdr;

		return ( col < nCol() ) ? hdr._cels[col] : "";
	}

	/**
	 * \brief Returns value of specific ( row,col ) cell.
	 *
	 * \param r - Row of interest
	 * \param c - Column of interest
	 * \return Value of cell ( r,c )
	 */
	const char *GetCell( int r, int c )
	{
		BlobRow *rows = (*this)()._rows;

		// Pre-conditions

		if ( !InRange( 0, r, nRow() ) )
			return "Row out-of-bounds";
		if ( !InRange( 0, c, nCol() ) )
			return "Column out-of-bounds";

		// Safe to search table

		return rows[r]._cels[c];
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
		char *bp, *cp;
		int  r, c, nr, nc, aSz;

		// Header

		Message::Dump();

		// Guts

		nr  = nRow();
		nc  = nCol();
		aSz = gmax( K, nr*nc*_MAX_BLOB_CEL );
		bp  = new char[aSz];
		cp  = bp;
		*cp = '\0';
		cp += sprintf( cp, "; %d rows\n", nr );
		for ( c=0; c<nc; cp += sprintf( cp, "%s\t", GetColName( c++ ) ) );
		cp += sprintf( cp, "\n" );
		for ( c=0; c<nc; cp += sprintf( cp, "------\t" ), c++ );
		cp += sprintf( cp, "\n" );
		for ( r=0; r<nr; r++ ) {
			for ( c=0; c<nc; cp += sprintf( cp, "%s\t", GetCell( r,c++ ) ) );
			cp += sprintf( cp, "\n" );
		}

		// Return

		dump_ += bp;
		delete[] bp;
		return dump_.data();
	}
};  // class BlobTable


} // namespace QUODD

#endif // __QUODD_BlobTable_H 
