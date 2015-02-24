#include "cSceneNode.h"


cSceneNode::cSceneNode(void)
{
	mPos = JVector3();
	mRot = JVector3();
	mMovable = false;
	SetVerts("");

	mLog = cLogger::Instance();
	mRen = cRenderer::Instance();
}


cSceneNode::~cSceneNode(void)
{
	for(unsigned int i = 0; i < mChildren.size(); i++)
	{
		if(mChildren[i]) delete mChildren[i]; //not sure if i need this, nodes could be initialised as non-pointer
		mChildren[i] = NULL;
	}
}

/*
adds a child to mChilderen.
if child is already in tree, does not add,
and returns false.
*/
bool cSceneNode::AddChild(cSceneNode* node)
{
	for(unsigned int i = 0; i < mChildren.size(); i++)
	{
		if(mChildren[i] == node) return false;
	}
	mChildren.push_back(node);
	return true;
}

/*
removes a child from mChilderen tree.
if the child does not exist, returns false.
*/
bool cSceneNode::DetachChild(cSceneNode* node)
{
	for(unsigned int i=0; i < mChildren.size(); i++)
	{
		if (node == mChildren[i])
		{
			mChildren.erase(mChildren.begin() + i);
			return true;
		}
		if(mChildren[i]->DetachChild(node) == true) return true;
	}	
	return false;
}

/*
returns true if child perameter is a child
of current node.
false if the child does not exist in mChildren
*/
bool cSceneNode::HasChild(const cSceneNode* child) const
{
	if(!child) return false;

	for(unsigned int i = 0; i < mChildren.size(); i++)
		if(mChildren[i] == child) return true;

	return false;
}

/*
checks whether node is inside the collision
radius of any of the collision tree nodes.
if there is a collision return true, else returns false.
uses pythagoras therum.
*/
bool cSceneNode::CheckCollisions(const cSceneNode* compare_root) const
{
	if(!compare_root) return false;

	//check for collision against root
	if(cMaths::PythagorasSqrd(
		mPos.x - compare_root->GetPosition().x,mPos.y - compare_root->GetPosition().y)
		< mCollisionRadiusSqrd)
		return true;

	//check for collision against children recursivly
	for(unsigned int i = 0; i < compare_root->mChildren.size(); i++)
		if(CheckCollisions(compare_root->mChildren[i])) return true;

	return false;
}

void cSceneNode::UpdateTree(const JVector3& displacement, const float rotation, const cSceneNode* root) const
{
	for(unsigned int i=0; i < mChildren.size(); i++)
	{
		if(displacement != JVector3()) mChildren[i]->Move(displacement, root);
		if(rotation != 0) mChildren[i]->Rotate(rotation,root);
	}
}

void cSceneNode::Move(const JVector3& displacement, const cSceneNode* root)
{
	mPos += displacement;
	if(CheckCollisions(root))
		mPos -= displacement;
	else
		UpdateTree(displacement,0,root);
}

/*
rotates node around specific pivot point,
pivot position reletive to 'this' node local space.
checks for collisions
*/
void cSceneNode::Pivot(float d, const JVector3& position, const cSceneNode* root)
{
	vector<JVector3> oldVerts = mVerts;
	JVector3 oldPos = mPos;

	JMatrix3 affineXY;
	affineXY.AffineRotationMatrix(d,JVector3());
	affineXY.Dot(&mVerts[0],mVerts.size());

	mPos.x += position.x;
	mPos.y += position.y;

	mRot.x = d;
	float s = sin(mRot.x*PIDIV180);
	float c = cos(mRot.x*PIDIV180);
	float xnew = mPos.x * c - mPos.y * s;
	float ynew = mPos.x * s + mPos.y * c;

	mPos.x = xnew;
	mPos.y = ynew;

	mPos.x -= position.x;
	mPos.y -= position.y;

	//if(CheckCollisions(root))
	//	mVerts = oldVerts;
	//else
	//	UpdateTree(JVector3(),d,root);

	printf("x: %f\n",mPos.x);
	printf("y: %f\n",mPos.y);
}

/*
rotates around node's centre,
uses Pivot() and cSceneNode::mPos.
*/
void cSceneNode::Rotate(const float d, const cSceneNode* root)
{
	Pivot(d,JVector3(mVertsCentre),root);
}

void cSceneNode::Scale(float scale, cSceneNode* root)
{
	vector<JVector3> oldVerts = mVerts;

	JMatrix3 sca;
	sca.ScaleMatrix(scale);

	sca.Dot(&mVerts[0],mVerts.size());
	
	if(CheckCollisions(root))
		mVerts = oldVerts;
	//else update scale tree
}

void cSceneNode::Draw()
{
	mRen->RenderVerts(mPos.x,mPos.y,mVerts);
}

void cSceneNode::SetPosition(const JVector3& _pos) { mPos = _pos; }
void cSceneNode::SetX(int x) { mPos.x = (float)x; }
void cSceneNode::SetY(int y) { mPos.y = (float)y; }
void cSceneNode::SetMovable(bool movable) { mMovable = movable; }

void cSceneNode::SetVerts(JVector3* verts)
{
	for(int i = 0; i < sizeof(mVerts)/sizeof(mVerts[0]); i++)
		mVerts[i] = verts[i];
}

bool cSceneNode::SetVerts(char* filename)
{
	//1.load file
	//2.get verts from file
	//3.set verts (temp rectangle)
	/*mVerts[0] = JVector3(mPos.x		,mPos.y - 5,1);
	mVerts[1] = JVector3(mPos.x - 10,mPos.y - 5,1);
	mVerts[2] = JVector3(mPos.x - 10,mPos.y	   ,1);
	mVerts[3] = JVector3(mPos.x - 10,mPos.y + 5,1);
	mVerts[4] = JVector3(mPos.x		,mPos.y + 5,1);
	mVerts[5] = JVector3(mPos.x + 10,mPos.y + 5,1);
	mVerts[6] = JVector3(mPos.x + 10,mPos.y	   ,1);
	mVerts[7] = JVector3(mPos.x + 10,mPos.y - 5,1);*/
	
	mVerts.push_back(JVector3(0,0,1));
	mVerts.push_back(JVector3(0,5,1));
	mVerts.push_back(JVector3(1,5,1));
	mVerts.push_back(JVector3(2,3,1));
	mVerts.push_back(JVector3(3,5,1));
	mVerts.push_back(JVector3(4,5,1));
	mVerts.push_back(JVector3(4,0,1));
	mVerts.push_back(JVector3(3,0,1));
	mVerts.push_back(JVector3(3,3,1));
	mVerts.push_back(JVector3(2,1,1));
	mVerts.push_back(JVector3(1,3,1));
	mVerts.push_back(JVector3(1,0,1));

	CalculateVertsCentre();

	return true;
}

void cSceneNode::CalculateVertsCentre()
{
	float min_x = 0.f, min_y = 0.f, min_z = 0.f;
	float max_x = 0.f, max_y = 0.f, max_z = 0.f;
	for(unsigned int i = 0; i < mVerts.size(); i++)
	{
		//calculate max verts
		if(max_x < mVerts[i].x) max_x = mVerts[i].x;
		if(max_y < mVerts[i].y) max_y = mVerts[i].y;
		if(max_z < mVerts[i].z) max_z = mVerts[i].z;
		//claculate min verts
		if(min_x > mVerts[i].x) min_x = mVerts[i].x;
		if(min_y > mVerts[i].y) min_y = mVerts[i].y;
		if(min_z > mVerts[i].z) min_z = mVerts[i].z;
	}
	
	mVertsCentre.x = (max_x + min_x)/2.f;
	mVertsCentre.y = (max_y + min_y)/2.f;
	mVertsCentre.z = 1.f;//(max_z + min_z)/2;
}

const JVector3& cSceneNode::GetPosition() const { return mPos; }
const JVector3& cSceneNode::GetCentrePosition() const { return mVertsCentre; }
