#ifndef MYLOCATOR_H
#define MYLOCATOR_H

#include <iostream>
#include <string>

#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MFloatPointArray.h>
#include <maya/MGlobal.h>
#include <maya/MPxLocatorNode.h>
#include <maya/MTypeId.h> 
#include <maya/MVector.h> 
#include <maya/MQuaternion.h>
#include <maya/MPoint.h> 
#include <maya/MMatrix.h> 
#include <maya/MDataBlock.h> 

#include <maya/MString.h>

#include <maya/MFnDependencyNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixAttribute.h>

class MyLocator : public MPxLocatorNode
{
public:
	MyLocator();
	virtual void        postConstructor();
	virtual				~MyLocator();

	virtual MStatus		compute(const MPlug& plug, MDataBlock& data);

	virtual void		draw(M3dView&, const MDagPath&, M3dView::DisplayStyle, M3dView::DisplayStatus);
	virtual bool        isBounded() const;
	virtual bool        isTransparent() const;
	virtual MBoundingBox boundingBox() const;

	void drawDisc(float radius, int divisions, bool filled);

	static  void*		creator();
	static  MStatus		initialize();

	static MObject      aOutPoint;
	static MObject      aScale;
	static MObject		aInMatrix;

	static	MTypeId		id;

};

#endif

