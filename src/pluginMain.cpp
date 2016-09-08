#include "myLocator.h"

#include <maya/MFnPlugin.h>

MStatus initializePlugin(MObject obj)
{
	MStatus status;

	MFnPlugin fnPlugin(obj, "David Alve", "1.0", "Any");

	status = fnPlugin.registerNode("myLocator", MyLocator::id, MyLocator::creator,
		MyLocator::initialize, MPxNode::kLocatorNode);
	CHECK_MSTATUS_AND_RETURN_IT(status);


	return MS::kSuccess;
}


MStatus uninitializePlugin(MObject obj)
{
	MStatus status;

	MFnPlugin fnPlugin(obj);

	status = fnPlugin.deregisterNode(MyLocator::id);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	return MS::kSuccess;
}