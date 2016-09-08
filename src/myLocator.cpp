#include "myLocator.h"

MTypeId     MyLocator::id(0x00000112234);

MObject MyLocator::aOutPoint;
MObject MyLocator::aScale;
MObject MyLocator::aInMatrix;

MyLocator::MyLocator()
{
}


void MyLocator::postConstructor()
{
	MObject oThis = thisMObject();
	MFnDependencyNode fnNode(oThis);
	fnNode.setName("myLocatorShape#");
}


MyLocator::~MyLocator()
{
}


void* MyLocator::creator()
{
	return new MyLocator();
}


MStatus MyLocator::compute(const MPlug& plug, MDataBlock& data)

{
	MStatus status;

	if (plug != aOutPoint)
	{
		return MS::kInvalidParameter;
	}

	MGlobal::displayInfo("Got it working here!");

	MVector upAxis = MVector::yAxis;

	double scale = data.inputValue(aScale).asDouble();
	MMatrix inMatrix = data.inputValue(aInMatrix).asMatrix();

	MTransformationMatrix mTrnMtrx = MTransformationMatrix(inMatrix);

	MVector inTransform = mTrnMtrx.getTranslation(MSpace::kWorld);
	MQuaternion rotation = mTrnMtrx.rotation();

	MVector scaledVector = upAxis * scale;
	MVector rotatedVector = scaledVector.rotateBy(rotation);

	MVector outVector = inTransform + rotatedVector;

	MDataHandle hOutput = data.outputValue(aOutPoint);
	hOutput.set3Float((float)outVector.x, (float)outVector.y, (float)outVector.z);
	hOutput.setClean();

	data.setClean(plug);

	
	return MS::kSuccess;
}


void MyLocator::draw(M3dView& view,
	const MDagPath& DGpath,
	M3dView::DisplayStyle style,
	M3dView::DisplayStatus status)
{
	view.beginGL();
	glPushAttrib(GL_CURRENT_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);

	MColor solidColor, wireColor;
	if (status == M3dView::kActive)
	{
		solidColor = MColor(1.0f, 1.0f, 1.0f, 0.1f);
		wireColor = MColor(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (status == M3dView::kLead)
	{
		solidColor = MColor(.26f, 1.0f, .64f, 0.1f);
		wireColor = MColor(.26f, 1.0f, .64f, 1.0f);
	}
	else
	{
		solidColor = MColor(1.0f, 1.0f, 0.0f, 0.1f);
		wireColor = MColor(1.0f, 1.0f, 0.0f, 1.0f);
	}

	// Draw solid
	glColor4f(solidColor.r, solidColor.g, solidColor.b, solidColor.a);
	drawDisc(1.0f, 32, true);

	// Draw wireframe
	glColor4f(wireColor.r, wireColor.g, wireColor.b, wireColor.a);
	//drawReflection(m_srcPoint, m_destPoint);
	drawDisc(1.0f, 32, false);

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	glPopAttrib();
	view.endGL();
}


void MyLocator::drawDisc(float radius, int divisions, bool filled)
{
	int renderState = filled ? GL_POLYGON : GL_LINE_LOOP;
	float degreesPerDiv = 360.0f / divisions;
	float radiansPerDiv = degreesPerDiv * 0.01745327778f;
	MFloatPointArray points(divisions);
	for (int i = 0; i < divisions; i++)
	{
		float angle = i * radiansPerDiv;
		float x = cos(angle) * radius;
		float z = sin(angle) * radius;
		points[i].x = x;
		points[i].z = z;
	}

	glBegin(renderState);
	for (int i = 0; i < divisions; i++)
	{
		glVertex3f(points[i].x, 0.0f, points[i].z);
	}
	glEnd();
}


bool MyLocator::isBounded() const
{
	return true;
}


bool MyLocator::isTransparent() const
{
	return true;
}


MBoundingBox MyLocator::boundingBox() const
{
	MBoundingBox bbox;
	bbox.expand(MVector(1.0, 0.0, 0.0));
	bbox.expand(MVector(-1.0, 0.0, 0.0));
	bbox.expand(MVector(0.0, 0.0, 1.0));
	bbox.expand(MVector(0.0, 0.0, -1.0));
	return bbox;
}


MStatus MyLocator::initialize()
{
	MFnMatrixAttribute mAttr;
	MFnNumericAttribute nAttr;


	aOutPoint = nAttr.createPoint("outPoint", "outPoint");
	nAttr.setWritable(false);
	nAttr.setStorable(false);
	addAttribute(aOutPoint);


	aScale = nAttr.create("scale", "scale", MFnNumericData::kDouble, 1.0);
	nAttr.setKeyable(true);
	addAttribute(aScale);
	attributeAffects(aScale, aOutPoint);

	aInMatrix = mAttr.create("inMatrix", "inMatrix");
	addAttribute(aInMatrix);
	attributeAffects(aInMatrix, aOutPoint);

	return MS::kSuccess;
}
