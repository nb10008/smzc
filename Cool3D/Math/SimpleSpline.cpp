#include "stdafx.h"
#include "SimpleSpline.h"
#include "..\Util\DebugMisc.h"

namespace Cool3D {

    //---------------------------------------------------------------------
    SimpleSpline::SimpleSpline()
    {
        // Set up matrix
        // Hermite polynomial
        mCoeffs._11 = 2;
        mCoeffs._12 = -2;
        mCoeffs._13 = 1;
        mCoeffs._14 = 1;
        mCoeffs._21 = -3;
        mCoeffs._22  = 3;
        mCoeffs._23  = -2;
        mCoeffs._24  = -1;
        mCoeffs._31  = 0;
        mCoeffs._32 = 0;
        mCoeffs._33 = 1;
        mCoeffs._34 = 0;
        mCoeffs._41 = 1;
        mCoeffs._42 = 0;
        mCoeffs._43 = 0;
        mCoeffs._44 = 0;

        mAutoCalc = true;
    }
    //---------------------------------------------------------------------
    SimpleSpline::~SimpleSpline()
    {
    }
    //---------------------------------------------------------------------
    void SimpleSpline::addPoint(const Vector3& p)
    {
        mPoints.push_back(p);
        if (mAutoCalc)
        {
            recalcTangents();
        }
    }
    //---------------------------------------------------------------------
    Vector3 SimpleSpline::interpolate(float t)				//！ 返回 t 值 所对应的中间点
    {														//!  这里假设所有的点间距是一样的，不然会造成速度变化
        // Currently assumes points are evenly spaced, will cause velocity
        // change where this is not the case
        // TODO: base on arclength?
		
        
        // Work out which segment this is in
        float fSeg = t * (mPoints.size() - 1);
        unsigned int segIdx = (unsigned int)fSeg;
        // Apportion t 
        t = fSeg - segIdx;

        return interpolate(segIdx, t);					//！ segIdx 是段数，t是段中的小数部分

    }
    //---------------------------------------------------------------------
    Vector3 SimpleSpline::interpolate(unsigned int fromIndex, float t)
    {
        // Bounds check
        ASSERT (fromIndex < mPoints.size() &&
            "fromIndex out of bounds");

        if ((fromIndex + 1) == mPoints.size())
        {
            // Duff request, cannot blend to nothing
            // Just return source
            return mPoints[fromIndex];

        }

        // Fast special cases
        if (t == 0.0f)
        {
            return mPoints[fromIndex];
        }
        else if(t == 1.0f)
        {
            return mPoints[fromIndex + 1];
        }

        // Real interpolation
        // Form a vector of powers of t
        float t2, t3;
        t2 = t * t;
        t3 = t2 * t;
        Vector4 powers(t3, t2, t, 1);


        // Algorithm is ret = powers * mCoeffs * Matrix4(point1, point2, tangent1, tangent2)
        Vector3& point1 = mPoints[fromIndex];
        Vector3& point2 = mPoints[fromIndex+1];
        Vector3& tan1 = mTangents[fromIndex];
        Vector3& tan2 = mTangents[fromIndex+1];
        Matrix4 pt;

        pt._11 = point1.x;
        pt._12  = point1.y;
        pt._13  = point1.z;
        pt._14  = 1.0f;
        pt._21  = point2.x;
        pt._22 = point2.y;
        pt._23 = point2.z;
        pt._24 = 1.0f;
        pt._31 = tan1.x;
        pt._32 = tan1.y;
        pt._33 = tan1.z;
        pt._34 = 1.0f;
        pt._41 = tan2.x;
        pt._42 = tan2.y;
        pt._43 = tan2.z;
        pt._44 = 1.0f;

        Vector4 ret = powers * mCoeffs * pt;


        return Vector3(ret.x, ret.y, ret.z);




    }
    //---------------------------------------------------------------------
    void SimpleSpline::recalcTangents(void)
    {
        // Catmull-Rom approach
        // 
        // tangent[i] = 0.5 * (point[i+1] - point[i-1])
        //
        // Assume endpoint tangents are parallel with line with neighbour

        size_t i, numPoints;
        bool isClosed;

        numPoints = mPoints.size();
        if (numPoints < 2)
        {
            // Can't do anything yet
            return;
        }

        // Closed or open?				//!		是否是循环的环形点
        if (mPoints[0] == mPoints[numPoints-1])
        {
            isClosed = true;
        }
        else
        {
            isClosed = false;
        }

        mTangents.resize(numPoints);



        for(i = 0; i < numPoints; ++i)
        {
            if (i ==0)
            {
                // Special case start
                if (isClosed)
                {
                    // Use numPoints-2 since numPoints-1 is the last point and == [0]
                    mTangents[i] = 0.5 * (mPoints[1] - mPoints[numPoints-2]);
                }
                else
                {
                    mTangents[i] = 0.5 * (mPoints[1] - mPoints[0]);
                }
            }
            else if (i == numPoints-1)
            {
                // Special case end
                if (isClosed)
                {
                    // Use same tangent as already calculated for [0]
                    mTangents[i] = mTangents[0];
                }
                else
                {
                    mTangents[i] = 0.5 * (mPoints[i] - mPoints[i-1]);
                }
            }
            else
            {
                mTangents[i] = 0.5 * (mPoints[i+1] - mPoints[i-1]);
            }
            
        }



    }
    //---------------------------------------------------------------------
    const Vector3& SimpleSpline::getPoint(unsigned short index) const
    {
        assert (index < mPoints.size() && "Point index is out of bounds!!");

        return mPoints[index];
    }
    //---------------------------------------------------------------------
    unsigned short SimpleSpline::getNumPoints(void) const
    {
        return (unsigned short)mPoints.size();
    }
    //---------------------------------------------------------------------
    void SimpleSpline::clear(void)
    {
        mPoints.clear();
        mTangents.clear();
    }
    //---------------------------------------------------------------------
    void SimpleSpline::updatePoint(unsigned short index, const Vector3& value)
    {
        assert (index < mPoints.size() && "Point index is out of bounds!!");

        mPoints[index] = value;
        if (mAutoCalc)
        {
            recalcTangents();
        }
    }
    //---------------------------------------------------------------------
    void SimpleSpline::setAutoCalculate(bool autoCalc)
    {
        mAutoCalc = autoCalc;
    }


}




