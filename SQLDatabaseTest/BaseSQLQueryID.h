#pragma once

//DroN 29-mart-2016: during implementation I have invented, that use queryes ID
// is not possible, cause query results are very different from each other. So
// I need to delete this file and rewrite every use of this enum
//
//enum contains non-negative(!) Base SQL procedure names
enum BaseSQLQueryID
{
	BSQ_UNDEFINED = -1
	
	,BSQ_FIRST = 0

	,BSQ_INIT_SHAPES_TABLE = BSQ_FIRST
	,BSQ_INIT_POINTS_TABLE
	,BSQ_INSERT_SHAPE
	,BSQ_SOME_SELECT

	,BSQ_LAST = BSQ_SOME_SELECT

	,BSQ_COUNT
};

//const BaseSQLQueryID& operator++( BaseSQLQueryID& ID )
//{
//	if( ID == BaseSQLQueryID::BSQ_COUNT )
//		return BaseSQLQueryID::BSQ_COUNT;
//	return static_cast<BaseSQLQueryID>( ID + 1 );
//}
//
//const BaseSQLQueryID operator++( BaseSQLQueryID& ID, int )
//{
//	BaseSQLQueryID ret = ID;
//	++ID;
//	return ret;
//}
//
//const BaseSQLQueryID& operator--( BaseSQLQueryID& ID )
//{
//	if( ID == BaseSQLQueryID::BSQ_UNDEFINED )
//		return BaseSQLQueryID::BSQ_UNDEFINED;
//	return static_cast<BaseSQLQueryID>( ID - 1 );
//}
//
//const BaseSQLQueryID operator--( BaseSQLQueryID& ID, int )
//{
//	BaseSQLQueryID ret = ID;
//	--ID;
//	return ret;
//}
