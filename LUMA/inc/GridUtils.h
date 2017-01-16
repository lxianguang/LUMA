/*
 * --------------------------------------------------------------
 *
 * ------ Lattice Boltzmann @ The University of Manchester ------
 *
 * -------------------------- L-U-M-A ---------------------------
 *
 *  Copyright (C) 2015, 2016
 *  E-mail contact: info@luma.manchester.ac.uk
 *
 * This software is for academic use only and not available for
 * distribution without written consent.
 *
 */

#ifndef GRIDUTILS_H
#define GRIDUTILS_H

/// \enum eLocationOnRank
/// \brief Enumeration indicating the location of a site when queried using isOnThisRank()
enum eLocationOnRank
{
	eNone,	///< No information provided (default).
	eCore,	///< Site on core (including send layer).
	eHalo	///< Site in halo (recv layer).
	
};

/// \enum eCartesianDirection
/// \brief Enumeration for directional options.
enum eCartesianDirection
{
	eXDirection,	///< X-direction
	eYDirection,	///< Y-direction
	eZDirection		///< Z-direction
};

/// \enum eMinMax
/// \brief	Enumeration for minimum and maximum.
///
///			Some utility methods need to know whether they should be looking
///			at or for a maximum or minimum edge of a grid so we use this 
///			enumeration to specify.
enum eMinMax
{
	eMinimum,		///< Minimum
	eMaximum		///< Maximum
};

/// \enum eCartMinMax
/// \brief	Enumeration for the combination of eCartesianDirection and eMinMax 
///			as these are often used together to index arrays.
enum eCartMinMax
{
	eXMin,
	eXMax,
	eYMin,
	eYMax,
	eZMin,
	eZMax
};

/// \enum eEdgeMinMax
/// \brief	Enumeration for the combination of Left and Right min and max edges.
enum eEdgeMinMax
{
	eLeftMin,
	eLeftMax,
	eRightMin,
	eRightMax
};

#include "stdafx.h"
#include "GridObj.h"

/// \brief	Grid utility class.
///
///			Class provides grid utilities including commonly used logical tests. 
///			This is a static class and so there is no need to instantiate it.
class GridUtils {

	// Properties //

public:
	static std::ofstream* logfile;			///< Handle to output file
	static std::string path_str;            ///< Static string representing output path
	static const int dir_reflect[L_DIMS * 2][L_NUM_VELS];	///< Array with hardcoded direction numbering for specular reflection

	// Methods //

private:
	/// Private constructor since class is static
	GridUtils();
	/// Private destructor
	~GridUtils();

public:
	// IO utilities
	static void createOutputDirectory(std::string path_str);		// Output directory creator

	// Mathematical and numbering utilities
	static std::vector<int> onespace(int min, int max);						// Function: onespace
	static std::vector<double> linspace(double min, double max, int n);		// Function: linspace
	static double vecnorm(double vec[L_DIMS]);								// Function: vecnorm + overloads
	static double vecnorm(double val1, double val2);
	static double vecnorm(double val1, double val2, double val3);
	static double vecnorm(std::vector<double> vec);
	static std::vector<int> getFineIndices(
		int coarse_i, int x_start, int coarse_j, int y_start, int coarse_k, int z_start); // Function: getFineIndices
	static std::vector<int> getCoarseIndices(
		int fine_i, int x_start, int fine_j, int y_start, int fine_k, int z_start); // Function: getCoarseIndices
	static double dotprod(std::vector<double> vec1, std::vector<double> vec2);		// Function: dotprod
	static std::vector<double> subtract(std::vector<double> a, std::vector<double> b);			// Function: subtract
	static std::vector<double> add(std::vector<double> a, std::vector<double> b);				// Function: add
	static std::vector<double> vecmultiply(double scalar, std::vector<double> vec);				// Function: multiply
	static std::vector<double> crossprod(std::vector<double> vec1, std::vector<double> vec2);	// Function: crossprod
	static std::vector<double> matrix_multiply(const std::vector< std::vector<double> >& A, const std::vector<double>& x);	// Function: matrix_multiply

	// LBM-specific utilities
	static int getOpposite(int direction);	// Function: getOpposite
	static void getGrid(GridObj*& Grids, int level, int region, GridObj*& ptr);		// Function to get pointer to grid in hierarchy

	// MPI-related utilities
	static bool isOverlapPeriodic(int i, int j, int k, const GridObj& pGrid);		// Is this halo periodically connected to neighbour
	static bool isOnThisRank(double x, double y, double z, 
		eLocationOnRank loc = eNone, const GridObj *grid = nullptr,
		std::vector<int> *pos = nullptr);											// Is a site on this MPI rank nad if so, where is it?
	static bool isOnThisRank(double xyz, eCartesianDirection dir, 
		eLocationOnRank loc = eNone, const GridObj *grid = nullptr, int *pos = nullptr);
	static bool intersectsRefinedRegion(const GridObj& pGrid, int RegNum);			// Does the refined region interesect the current rank.
	// The following supercede the old isOnEdge function to allow for different sized overlaps produced by different refinement levels.
	static bool isOnSenderLayer(double pos_x, double pos_y, double pos_z);			// Is site on any sender layer
	static bool isOnRecvLayer(double pos_x, double pos_y, double pos_z);			// Is site on any recv layer
	static bool isOnSenderLayer(double site_position, eCartMinMax edge);			// Is site on specified sender layer
	static bool isOnRecvLayer(double site_position, eCartMinMax edge);				// Is site on specified recv layer
	static int getMpiDirection(int offset_vector[]);								// Get MPI direction from vector

	// Coordinate Management
	static bool isOffGrid(int i, int j, int k, const GridObj *g);					// Is site off supplied grid
	static void getEnclosingVoxel(double x, double y, double z, const GridObj *g, std::vector<int> *ijk);	// Take a position and a get a local ijk
	static void getEnclosingVoxel(double x, const GridObj *g, eCartesianDirection dir, int *ijk);
	static bool isOnTransitionLayer(double pos_x, double pos_y, double pos_z, const GridObj *grid);	// Is site on any TL to upper
	static bool isOnTransitionLayer(double position, eCartMinMax edge, const GridObj *grid);		// Is site on specified TL to upper

	// Templated functions //

	/// \brief Computes the L2-norm.
	/// \param a1 first component of the vector
	/// \param a2 second component of the vector
	/// \param a3 third component of the vector
	/// \return NumType scalar quantity
	template <typename NumType>
	static NumType vecnorm(NumType a1, NumType a2, NumType a3) {
		return (NumType)sqrt( a1*a1 + a2*a2 + a3*a3 );
	}
	/// \brief Computes the L2-norm.
	/// \param a1 first component of the vector
	/// \param a2 second component of the vector
	/// \return NumType scalar quantity
	template <typename NumType>
	static NumType vecnorm(NumType a1, NumType a2) {
		return (NumType)sqrt( a1*a1 + a2*a2 );
	}
	
	/// \brief	Rounds a negative value up to zero.
	///
	///			If value is positive, return the value unchanged.
	///
	/// \param x value to be rounded
	/// \return NumType rounded value
	template <typename NumType>
	static NumType upToZero(NumType x) {	
		if (x < static_cast<NumType>(0)) return static_cast<NumType>(0);
		else return x;		
	};

	/// \brief	Rounds a value greater than a limit down to this value.
	///
	///			If value is less than or equal to the limit, return the value 
	///			unchanged.
	///
	/// \param x value to be rounded
	/// \param limit value to be rounded down to
	/// \return NumType rounded value
	template <typename NumType>
	static NumType downToLimit(NumType x, NumType limit) {	
		if (x > limit) return limit;
		else return x;		
	};

	/// \brief	Computes the factorial of the supplied value.
	///
	///			If n == 0 then returns 1.
	///
	/// \param n factorial
	/// \return NumType n factorial
	template <typename NumType>
	static NumType factorial(NumType n) {
		if (n == static_cast<NumType>(0)) 
			return static_cast<NumType>(1);
		else
			return n * factorial(n - 1);
	};

	/// \brief	Performs a strided memcpy.
	///
	///			Memcpy() is designed to copy blocks of contiguous memory.
	///			Strided copy copies a pattern of contiguous blocks.
	///
	/// \param dest		pointer to start of destination memory.
	/// \param src		pointer to start of source memory.
	/// \param block	size of contiguous block.
	/// \param offset	offset from the start of the soruce array.
	/// \param stride	number of elements between start of first block and start of second.
	/// \param count number of blocks in pattern
	/// \param buf_offset offset from start of destination buffer to start writing. Default is zero if not supplied.
	template <typename NumType>
	static void stridedCopy(NumType *dest, NumType *src, size_t block, 
		size_t offset, size_t stride, size_t count,
		size_t buf_offset = 0) {

		for (size_t i = 0; i < count; i++) {
			memcpy(dest + buf_offset, src + offset + (stride * i), block * sizeof(NumType));
			buf_offset += block;
		}
	};

};

#endif